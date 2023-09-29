#include "../../include/Model/model_part.h"


/// @brief public:
/// @name Type Define
/// @{
LOTUS_CREATE_LOCAL_FLAGS(ModelPart, ALL_ENTITIES, 0);
LOTUS_CREATE_LOCAL_FLAGS(ModelPart, OVERWRITE_ENTITIES, 1);

/// @}


/// @name Life Circle
/// @{
    /// Default constructor.
    ModelPart::ModelPart(Variables_List::Pointer pVariablesList,
                         Model& rOwnerModel)
    :ModelPart("Default", pVariablesList, rOwnerModel)
    { }

    /// Constructor with name
    ModelPart::ModelPart(std::string const& NewName,
                         Variables_List::Pointer pVariablesList,
                         Model& rOwnerModel)
    :ModelPart(NewName, 1, pVariablesList, rOwnerModel)
    { }

    /// Constructor with name and bufferSize
    ModelPart::ModelPart(std::string const& NewName,
                         IndexType NewBufferSize,
                         Variables_List::Pointer pVariablesList, 
                         Model& rOwnerModel)
    : Data_Value_Container()
    , Flags()
    , mBufferSize(NewBufferSize)
    , mpProcessInfo(new Process_Info())
    , mGeometries()
    , mpVariablesList(pVariablesList)
    , mpParentModelPart(NULL)
    , mSubModelParts()
    , mrModel(rOwnerModel)
    {
        if(NewName.empty())
        {
            std::cerr<< "Please don't use empty names (\"\") when creating a ModelPart" << std::endl;
            exit(0); 
        }

        if(NewName.find('.') != std::string::npos)
        {
            std::cerr<< "Please don't use names containing (\".\") when creating a ModelPart (used in \"" << NewName << "\")" << std::endl;
            exit(0); 
        } 

        mName = NewName;
        MeshType mesh;
        mMeshes.push_back(std::make_shared<MeshType>(mesh.Clone()));
    }

    /// Destructor.
    ModelPart::~ModelPart()
    {
        Clear();
    }


/// @}


/// @name Operators
/// @{
    
/// @}


/// @name Operations
/// @{

    void ModelPart::Clear()
    {
        // Call recursively clear of all submodel parts
        for (auto& r_sub_model_part : mSubModelParts) {
            r_sub_model_part.Clear();
        }

        // Clear sub model parts list
        mSubModelParts.clear();

        // Clear meshes
        for(auto& r_mesh : mMeshes) {
            r_mesh.Clear();
        }

        // Clear meshes list
        mMeshes.clear();
        mMeshes.emplace_back(std::make_shared<MeshType>());

        // Clear geometries
        mGeometries.Clear();

        mTables.clear();

        this->AssignFlags(Flags());

    }
    void ModelPart::Reset()
    {
        // Clears the model part
        Clear();

        // construct a new variable list and process info. Old data ptrs is not destroyed
        // since, same data may be shared with some other model parts as well.
        mpVariablesList = std::make_shared<Variables_List>();
        mpProcessInfo = std::make_shared<Process_Info>();
        mBufferSize = 0;
    }

    ModelPart::IndexType ModelPart::CreateSolutionStep()
    {
        std::cerr << "This method needs updating and is not working. Pooyan", "";
        exit(0);
        return 0;
    }
    ModelPart::IndexType ModelPart::CloneSolutionStep()
    {
        if(IsSubModelPart())
        {
            std::cerr << "Calling the method of the sub model part "
            << Name() << " please call the one of the root model part: "
            << GetRootModelPart().Name() << std::endl;
            exit(0);
        } 

        const int nnodes = static_cast<int>(Nodes().size());
        auto nodes_begin = NodesBegin();
        #pragma omp parallel for firstprivate(nodes_begin,nnodes)
        for(int i = 0; i<nnodes; ++i)
        {
            auto node_iterator = nodes_begin + i;
            node_iterator->CloneSolutionStepData();
        }

        mpProcessInfo->CloneSolutionStepInfo();

        mpProcessInfo->ClearHistory(mBufferSize);

        return 0;
    }
    ModelPart::IndexType ModelPart::CloneTimeStep()
    {
        if(IsSubModelPart())
        {
            std::cerr<< "Calling the method of the sub model part "
            << Name() << " please call the one of the root model part: "
            << GetRootModelPart().Name() << std::endl;
            exit(0);
        } 

        IndexType new_index = CloneSolutionStep();
        mpProcessInfo->SetAsTimeStepInfo();

        return new_index;
    }


    ModelPart::IndexType ModelPart::CreateTimeStep(double NewTime)
    {
        if(IsSubModelPart())
        {
            std::cerr<< "Calling the method of the sub model part "
            << Name() << " please call the one of the root model part: "
            << GetRootModelPart().Name() << std::endl;
            exit(0);
        } 

        IndexType new_index = CreateSolutionStep();
        mpProcessInfo->SetAsTimeStepInfo(NewTime);

        return new_index;
    }
    ModelPart::IndexType ModelPart::CloneTimeStep(double NewTime)
    {
        if(IsSubModelPart()) 
        {   std::cerr<< "Calling the method of the sub model part "
            << Name() << " please call the one of the root model part: "
            << GetRootModelPart().Name() << std::endl;
            exit(0);
        }
        IndexType new_index = CloneSolutionStep();
        mpProcessInfo->SetAsTimeStepInfo(NewTime);
        return new_index;
    }

    void ModelPart::OverwriteSolutionStepData(IndexType SourceSolutionStepIndex, IndexType DestinationSourceSolutionStepIndex)
    {
        if(IsSubModelPart())
        {
            std::cerr<< "Calling the method of the sub model part "
            << Name() << " please call the one of the root model part: "
            << GetRootModelPart().Name() << std::endl;
            exit(0);
        } 
        for (NodeIterator node_iterator = NodesBegin(); node_iterator != NodesEnd(); node_iterator++)
            node_iterator->OverwriteSolutionStepData(SourceSolutionStepIndex, DestinationSourceSolutionStepIndex);
    }
    void ModelPart::ReduceTimeStep(ModelPart& rModelPart, double NewTime)
    {
        //ATTENTION: this function does not touch the coordinates of the nodes.
        //It just resets the database values to the values at the beginning of the time step
        if(IsSubModelPart())
        {
            std::cerr<< "Calling the method of the sub model part "
            << Name() << " please call the one of the root model part: "
            << GetRootModelPart().Name() << std::endl;
            exit(0);
        } 
        rModelPart.OverwriteSolutionStepData(1, 0);
        rModelPart.GetProcessInfo().SetCurrentTime(NewTime);

    }


    /// @name Node
    /// @{
        /** Inserts a node in the mesh with ThisIndex.
        */
        void ModelPart::AddNode(ModelPart::NodeType::Pointer pNewNode, ModelPart::IndexType ThisIndex)
        {
            if (IsSubModelPart())
            {
                mpParentModelPart->AddNode(pNewNode, ThisIndex);
                GetMesh(ThisIndex).AddNode(pNewNode);
            }
            else
            {
                auto existing_node_it = this->GetMesh(ThisIndex).Nodes().find(pNewNode->Id());
                if( existing_node_it == GetMesh(ThisIndex).NodesEnd()) //node did not exist
                {
                    GetMesh(ThisIndex).AddNode(pNewNode);
                }
                else //node did exist already
                {
                    if(&(*existing_node_it) != &(*pNewNode))//check if the pointee coincides
                    {
                        std::cerr << "attempting to add pNewNode with Id :" << pNewNode->Id() << ", unfortunately a (different) node with the same Id already exists" << std::endl;
                        exit(0);
                    }
                        
                }
            }
        }
        /** Inserts a list of nodes in a submodelpart provided their Id. Does nothing if applied to the top model part
        */
        void ModelPart::AddNodes(std::vector<IndexType> const& NodeIds, IndexType ThisIndex)
        {
            if(IsSubModelPart()) //does nothing if we are on the top model part
            {
                //obtain from the root model part the corresponding list of nodes
                ModelPart* root_model_part = &this->GetRootModelPart();
                ModelPart::NodesContainerType  aux;
                aux.reserve(NodeIds.size());
                for(unsigned int i=0; i<NodeIds.size(); i++)
                {
                    ModelPart::NodesContainerType::iterator it = root_model_part->Nodes().find(NodeIds[i]);
                    if(it!=root_model_part->NodesEnd())
                        aux.push_back(*(it.base()));
                    else
                    {
                        std::cerr<< "while adding nodes to submodelpart, the node with Id " << NodeIds[i] << " does not exist in the root model part";
                        exit(0);
                    }       
                }
                ModelPart* current_part = this;
                while(current_part->IsSubModelPart())
                {
                    for(auto it = aux.begin(); it!=aux.end(); it++)
                        current_part->Nodes().push_back( *(it.base()) );

                    current_part->Nodes().Unique();
                    current_part = &(current_part->GetParentModelPart());
                }
            }
        }
        /** Inserts a node in the mesh with ThisIndex.
        */
        ModelPart::NodeType::Pointer ModelPart::CreateNewNode(int Id, double x, double y, double z, VariablesList::Pointer pNewVariablesList, ModelPart::IndexType ThisIndex)
        {
            if (IsSubModelPart())
            {
                NodeType::Pointer p_new_node = mpParentModelPart->CreateNewNode(Id, x, y, z, pNewVariablesList, ThisIndex);
                GetMesh(ThisIndex).AddNode(p_new_node);

                return p_new_node;
            }
            //verify if the node exists and eventually give back the existing node
            auto& root_nodes = this->Nodes(); //note that if we are here than we are working with the root model_part
            auto existing_node_it = root_nodes.find(Id);
            if( existing_node_it != root_nodes.end())
            {
                //the node already exists - now check if the position we ask for coincides with the one of the existing one
                double distance = std::sqrt( std::pow(existing_node_it->x() - x,2) + std::pow(existing_node_it->y() - y,2) + std::pow(existing_node_it->z() - z,2) );

                if(distance > std::numeric_limits<double>::epsilon()*1000)
                {
                    std::cerr<< "trying to create a node with Id " << Id << " however a node with the same Id already exists in the root model part. Existing node coordinates are " << *existing_node_it << " coordinates of the nodes we are attempting to create are :" << x << " " << y << " " << z;
                    exit(0);
                }
                //if the node we attempt to create is in the same position as the one that is already there, we return the old one
                return *(existing_node_it.base());
            }
            //create a new node
            NodeType::SharedPointer p_new_node = std::make_shared< NodeType >( Id, x, y, z );
            // Giving model part's variables list to the node
            p_new_node->SetSolutionStepVariablesList(pNewVariablesList);
            //set buffer size
            p_new_node->SetBufferSize(mBufferSize);
            //add the new node to the list of nodes
            GetMesh(ThisIndex).AddNode(p_new_node);
            return p_new_node;
        }
        ModelPart::NodeType::Pointer ModelPart::CreateNewNode(ModelPart::IndexType Id, double x, double y, double z, ModelPart::IndexType ThisIndex)
        {
            return CreateNewNode(Id, x, y, z, mpVariablesList, ThisIndex);
        }
        ModelPart::NodeType::Pointer ModelPart::CreateNewNode(ModelPart::IndexType Id, double x, double y, double z, double* pThisData, ModelPart::IndexType ThisIndex)
        {
            if (IsSubModelPart())
            {
                NodeType::Pointer p_new_node = mpParentModelPart->CreateNewNode(Id, x, y, z, pThisData, ThisIndex);
                GetMesh(ThisIndex).AddNode(p_new_node);
                return p_new_node;
            }
            //verify if the node exists and eventually give back the existing node
            NodesContainerType::iterator existing_node_it = this->GetMesh(ThisIndex).Nodes().find(Id);
            if( existing_node_it != GetMesh(ThisIndex).NodesEnd())
            {
                //the node already exists - now check if the position we ask for coincides with the one of the existing one
                double distance = std::sqrt( std::pow( existing_node_it->x() - x,2) + std::pow(existing_node_it->y() - y,2) + std::pow(existing_node_it->z() - z,2) );
                if(distance > std::numeric_limits<double>::epsilon()*1000)
                {
                    std::cerr<< "trying to create a node with Id " << Id << " however a node with the same Id already exists in the root model part. Existing node coordinates are " << *existing_node_it << " coordinates of the nodes we are attempting to create are :" << x << " " << y << " " << z;
                    exit(0);
                }
                //if the node we attempt to create is in the same position as the one that is already there, we return the old one
                return *(existing_node_it.base());
            }
            //create a new node
            NodeType::Pointer p_new_node = std::make_shared<NodeType>( Id, x, y, z, mpVariablesList, pThisData, mBufferSize);
            //add the new node to the list of nodes
            GetMesh(ThisIndex).AddNode(p_new_node);
            return p_new_node;
        }
        ModelPart::NodeType::Pointer ModelPart::CreateNewNode(ModelPart::IndexType NodeId, ModelPart::NodeType const& rSourceNode, ModelPart::IndexType ThisIndex)
        {
            return CreateNewNode(NodeId, rSourceNode.x(), rSourceNode.y(), rSourceNode.z(), mpVariablesList, ThisIndex);
        }
        void ModelPart::AssignNode(ModelPart::NodeType::Pointer pThisNode, ModelPart::IndexType ThisIndex)
        {
            if (IsSubModelPart())
            {
                mpParentModelPart->AssignNode(pThisNode, ThisIndex);

                //add the new node to the list of nodes
                GetMesh(ThisIndex).AddNode(pThisNode);

                return;
            }
            // Giving model part's variables list to the node
            pThisNode->SetSolutionStepVariablesList(mpVariablesList);
            //set buffer size
            pThisNode->SetBufferSize(mBufferSize);
            //add the new node to the list of nodes
            GetMesh(ThisIndex).AddNode(pThisNode);
        }
        /** Remove the node with given Id from mesh with ThisIndex in this modelpart and all its subs.
        */
        void ModelPart::RemoveNode(ModelPart::IndexType NodeId, ModelPart::IndexType ThisIndex)
        {
            GetMesh(ThisIndex).RemoveNode(NodeId);
            for (SubModelPartIterator i_sub_model_part = SubModelPartsBegin(); i_sub_model_part != SubModelPartsEnd(); i_sub_model_part++)
                i_sub_model_part->RemoveNode(NodeId, ThisIndex);
        }
        /** Remove given node from mesh with ThisIndex in this modelpart and all its subs.
        */
        void ModelPart::RemoveNode(ModelPart::NodeType& ThisNode, ModelPart::IndexType ThisIndex)
        {
            GetMesh(ThisIndex).RemoveNode(ThisNode);
            for (SubModelPartIterator i_sub_model_part = SubModelPartsBegin(); i_sub_model_part != SubModelPartsEnd(); i_sub_model_part++)
                i_sub_model_part->RemoveNode(ThisNode, ThisIndex);
        }
        /** Remove given node from mesh with ThisIndex in this modelpart and all its subs.
        */
        void ModelPart::RemoveNode(ModelPart::NodeType::Pointer pThisNode, ModelPart::IndexType ThisIndex)
        {
            GetMesh(ThisIndex).RemoveNode(pThisNode);
            for (SubModelPartIterator i_sub_model_part = SubModelPartsBegin(); i_sub_model_part != SubModelPartsEnd(); i_sub_model_part++)
                i_sub_model_part->RemoveNode(pThisNode, ThisIndex);
        }
        /** Remove the node with given Id from mesh with ThisIndex in parents, itself and children.
        */
        void ModelPart::RemoveNodeFromAllLevels(ModelPart::IndexType NodeId, ModelPart::IndexType ThisIndex)
        {
            if (IsSubModelPart())
            {
                mpParentModelPart->RemoveNodeFromAllLevels(NodeId, ThisIndex);
                return;
            }
            RemoveNode(NodeId, ThisIndex);
        }
        /** Remove given node from mesh with ThisIndex in parents, itself and children.
        */
        void ModelPart::RemoveNodeFromAllLevels(ModelPart::NodeType& ThisNode, ModelPart::IndexType ThisIndex)
        {
            if (IsSubModelPart())
            {
                mpParentModelPart->RemoveNode(ThisNode, ThisIndex);
                return;
            }
            RemoveNode(ThisNode, ThisIndex);
        }
        /** Remove given node from mesh with ThisIndex in parents, itself and children.
        */
        void ModelPart::RemoveNodeFromAllLevels(ModelPart::NodeType::Pointer pThisNode, ModelPart::IndexType ThisIndex)
        {
            if (IsSubModelPart())
            {
                mpParentModelPart->RemoveNode(pThisNode, ThisIndex);
                return;
            }
            RemoveNode(pThisNode, ThisIndex);
        }
        void ModelPart::RemoveNodes(Flags IdentifierFlag)
        {
            // Lambda to remove nodes from a mesh
            auto remove_nodes_from_mesh = [&](ModelPart::MeshType& r_mesh) {
                //count the nodes to be erase
                const unsigned int nnodes = r_mesh.Nodes().size();
                unsigned int erase_count = 0;
                #pragma omp parallel for reduction(+:erase_count)
                for(int i=0; i<static_cast<int>(nnodes); ++i) {
                    ModelPart::NodesContainerType::iterator i_node = r_mesh.NodesBegin() + i;

                    if( i_node->IsNot(IdentifierFlag) )
                        erase_count++;
                }

                ModelPart::NodesContainerType temp_nodes_container;
                temp_nodes_container.reserve(r_mesh.Nodes().size() - erase_count);

                temp_nodes_container.swap(r_mesh.Nodes());

                for(ModelPart::NodesContainerType::iterator i_node = temp_nodes_container.begin() ; i_node != temp_nodes_container.end() ; ++i_node) {
                    if( i_node->IsNot(IdentifierFlag) )
                        (r_mesh.Nodes()).push_back(std::move(*(i_node.base())));
                }
            };

            // This method is optimized to free the memory
            // Loop over all the local meshes (Is this still necessary with Submodelparts?)
            for(auto& r_mesh: this->GetMeshes()) {
                remove_nodes_from_mesh(r_mesh);
            }

            // Now recursively remove the nodes in the submodelparts
            for (auto& r_sub_model_part : SubModelParts()) {
                r_sub_model_part.RemoveNodes(IdentifierFlag);
            }
        }
        void ModelPart::RemoveNodesFromAllLevels(Flags IdentifierFlag)
        {
            ModelPart& root_model_part = GetRootModelPart();
            root_model_part.RemoveNodes(IdentifierFlag);
        }

        ModelPart& ModelPart::GetRootModelPart()
        {
            if (IsSubModelPart())
                return mpParentModelPart->GetRootModelPart();
            else
                return *this;
        }
        const ModelPart& ModelPart::GetRootModelPart() const
        {
            if (IsSubModelPart())
                return mpParentModelPart->GetRootModelPart();
            else
                return *this;
        }
        void ModelPart::SetNodalSolutionStepVariablesList()
        {
            if(IsSubModelPart())
            {
                std::cerr<< "Calling the method of the sub model part "
                << Name() << " please call the one of the root model part: "
                << GetRootModelPart().Name() << std::endl;
                exit(0);
            } 
            // Iterate over nodes
            auto& r_nodes_array = this->Nodes();
            for_each(r_nodes_array.begin(),r_nodes_array.end(),[&](NodeType& rNode){
                rNode.SetSolutionStepVariablesList(mpVariablesList);
            });
        }

        /** Inserts a Table
        */
        void ModelPart::AddTable(ModelPart::IndexType TableId, ModelPart::TableType::Pointer pNewTable)
        {
            if (IsSubModelPart())
                mpParentModelPart->AddTable(TableId, pNewTable);
            mTables.insert(TableId, pNewTable);
        }
        /** Remove the Table with given Id from current mesh.
        */
        void ModelPart::RemoveTable(ModelPart::IndexType TableId)
        {
            mTables.erase(TableId);

            for (SubModelPartIterator i_sub_model_part = SubModelPartsBegin(); i_sub_model_part != SubModelPartsEnd(); i_sub_model_part++)
                i_sub_model_part->RemoveTable(TableId);
        }
        /** Remove the Table with given Id from current mesh in parents, itself and all children.
        */
        void ModelPart::RemoveTableFromAllLevels(ModelPart::IndexType TableId)
        {
            if (IsSubModelPart())
            {
                mpParentModelPart->RemoveTableFromAllLevels(TableId);
                return;
            }
            RemoveTable(TableId);
        }

        /// @name Properties
        /// @{
            ModelPart::SizeType ModelPart::NumberOfProperties(IndexType ThisIndex) const
            {
                return GetMesh(ThisIndex).NumberOfProperties();
            }
            /** Inserts a properties in the mesh with ThisIndex.
            */
            void ModelPart::AddProperties(ModelPart::PropertiesType::Pointer pNewProperties, ModelPart::IndexType ThisIndex)
            {
                if (IsSubModelPart())
                {
                    mpParentModelPart->AddProperties(pNewProperties, ThisIndex);
                }

                auto existing_prop_it = GetMesh(ThisIndex).Properties().find(pNewProperties->Id());
                if( existing_prop_it != GetMesh(ThisIndex).Properties().end() )
                {
                    if( &(*existing_prop_it) != &(*pNewProperties) )
                    {
                        std::cerr<< "trying to add a property with existing Id within the model part : " << Name() << ", property Id is :" << pNewProperties->Id();
                        exit(0);
                    }
                        
                }
                else
                {
                    GetMesh(ThisIndex).AddProperties(pNewProperties);
                }
            }

            bool ModelPart::HasProperties(
                IndexType PropertiesId,
                IndexType MeshIndex
                ) const
            {
                auto pprop_it = GetMesh(MeshIndex).Properties().find(PropertiesId);
                if(pprop_it != GetMesh(MeshIndex).Properties().end()) { // Property does exist
                    return true;
                }

                return false;
            }

            bool ModelPart::RecursivelyHasProperties(
                IndexType PropertiesId,
                IndexType MeshIndex
                ) const
            {
                auto pprop_it = GetMesh(MeshIndex).Properties().find(PropertiesId);
                if(pprop_it != GetMesh(MeshIndex).Properties().end()) { // Property does exist
                    return true;
                } else {
                    if(IsSubModelPart()) {
                        return mpParentModelPart->RecursivelyHasProperties(PropertiesId, MeshIndex);
                    } else {
                        return false;
                    }
                }
            }

            ModelPart::PropertiesType::Pointer ModelPart::CreateNewProperties(
                IndexType PropertiesId,
                IndexType MeshIndex
                )
            {
                auto pprop_it = GetMesh(MeshIndex).Properties().find(PropertiesId);
                if(pprop_it != GetMesh(MeshIndex).Properties().end()) 
                { // Property does exist
                    std::cerr << "Property #" << PropertiesId << " already existing. Please use pGetProperties() instead" << std::endl;
                    exit(0);
                } else {
                    if(IsSubModelPart()) {
                        PropertiesType::Pointer pprop =  mpParentModelPart->CreateNewProperties(PropertiesId, MeshIndex);
                        GetMesh(MeshIndex).AddProperties(pprop);
                        return pprop;
                    } else {
                        PropertiesType::Pointer pnew_property = std::make_shared<PropertiesType>(PropertiesId);
                        GetMesh(MeshIndex).AddProperties(pnew_property);
                        return pnew_property;
                    }
                }
            }
            ModelPart::PropertiesType::Pointer ModelPart::pGetProperties(
                IndexType PropertiesId,
                IndexType MeshIndex
                )
            {
                auto pprop_it = GetMesh(MeshIndex).Properties().find(PropertiesId);
                if(pprop_it != GetMesh(MeshIndex).Properties().end()) { // Property does exist
                    return *(pprop_it.base());
                } else {
                    if(IsSubModelPart()) {
                        PropertiesType::Pointer pprop =  mpParentModelPart->pGetProperties(PropertiesId, MeshIndex);
                        GetMesh(MeshIndex).AddProperties(pprop);
                        return pprop;
                    } else {
                        std::cerr << ("ModelPart") << "Property " << PropertiesId << " does not exist!. Creating and adding new property. Please use CreateNewProperties() instead" << std::endl;
                        PropertiesType::Pointer pnew_property = std::make_shared<PropertiesType>(PropertiesId);
                        GetMesh(MeshIndex).AddProperties(pnew_property);
                        return pnew_property;
                    }
                }
            }
            const ModelPart::PropertiesType::Pointer ModelPart::pGetProperties(
                IndexType PropertiesId,
                IndexType MeshIndex
                ) const
            {
                auto pprop_it = GetMesh(MeshIndex).Properties().find(PropertiesId);
                if(pprop_it != GetMesh(MeshIndex).Properties().end()) { // Property does exist
                    return *(pprop_it.base());
                } else {
                    if(IsSubModelPart()) {
                        PropertiesType::Pointer pprop =  mpParentModelPart->pGetProperties(PropertiesId, MeshIndex);
                        return pprop;
                    } else {
                        std::cerr << "Property " << PropertiesId << " does not exist!. This is constant model part and cannot be created a new one" << std::endl;
                        exit(0);
                    }
                }
            }
            ModelPart::PropertiesType& ModelPart::GetProperties(
                IndexType PropertiesId,
                IndexType MeshIndex
                )
            {
                auto pprop_it = GetMesh(MeshIndex).Properties().find(PropertiesId);
                if(pprop_it != GetMesh(MeshIndex).Properties().end()) { // Property does exist
                    return *pprop_it;
                } else {
                    if(IsSubModelPart()) {
                        PropertiesType::Pointer pprop =  mpParentModelPart->pGetProperties(PropertiesId, MeshIndex);
                        GetMesh(MeshIndex).AddProperties(pprop);
                        return *pprop;
                    } else {
                        std::cerr << ("ModelPart") << "Property " << PropertiesId << " does not exist!. Creating and adding new property. Please use CreateNewProperties() instead" << std::endl;
                        PropertiesType::Pointer pnew_property = std::make_shared<PropertiesType>(PropertiesId);
                        GetMesh(MeshIndex).AddProperties(pnew_property);
                        return *pnew_property;
                    }
                }
            }
            const ModelPart::PropertiesType& ModelPart::GetProperties(
                IndexType PropertiesId,
                IndexType MeshIndex
                ) const
            {
                auto pprop_it = GetMesh(MeshIndex).Properties().find(PropertiesId);
                if(pprop_it != GetMesh(MeshIndex).Properties().end()) { // Property does exist
                    return *pprop_it;
                } else {
                    if(IsSubModelPart()) {
                        PropertiesType::Pointer pprop =  mpParentModelPart->pGetProperties(PropertiesId, MeshIndex);
                        return *pprop;
                    } else {
                        std::cerr << "Property " << PropertiesId << " does not exist!. This is constant model part and cannot be created a new one" << std::endl;
                        exit(0);
                    }
                }
            }
            bool ModelPart::HasProperties(
                const std::string& rAddress,
                IndexType MeshIndex
                ) const
            {
                const std::vector<IndexType> component_name = TrimComponentName(rAddress);
                if (HasProperties(component_name[0], MeshIndex)) {
                    bool has_properties = true;
                    Properties::Pointer p_prop = pGetProperties(component_name[0], MeshIndex);
                    for (IndexType i = 1; i < component_name.size(); ++i) {
                        if (p_prop->HasSubProperties(component_name[i])) {
                            p_prop = p_prop->pGetSubProperties(component_name[i]);
                        } else {
                            return false;
                        }
                    }
                    return has_properties;
                } else {
                    return false;
                }
            }
            Properties::Pointer ModelPart::pGetProperties(
                const std::string& rAddress,
                IndexType MeshIndex
                )
            {
                const std::vector<IndexType> component_name = TrimComponentName(rAddress);
                if (HasProperties(component_name[0], MeshIndex)) {
                    Properties::Pointer p_prop = pGetProperties(component_name[0], MeshIndex);
                    for (IndexType i = 1; i < component_name.size(); ++i) {
                        if (p_prop->HasSubProperties(component_name[i])) {
                            p_prop = p_prop->pGetSubProperties(component_name[i]);
                        } else {
                            std::cerr << "Index is wrong, does not correspond with any sub Properties Id: " << rAddress << std::endl;
                            exit(0);
                        }
                    }
                    return p_prop;
                } else {
                    std::cerr << "First index is wrong, does not correspond with any sub Properties Id: " << component_name[0] << std::endl;
                    exit(0);
                }
            }
            const Properties::Pointer ModelPart::pGetProperties(
                const std::string& rAddress,
                IndexType MeshIndex
                ) const
            {
                const std::vector<IndexType> component_name = TrimComponentName(rAddress);
                if (HasProperties(component_name[0], MeshIndex)) {
                    Properties::Pointer p_prop = pGetProperties(component_name[0], MeshIndex);
                    for (IndexType i = 1; i < component_name.size(); ++i) {
                        if (p_prop->HasSubProperties(component_name[i])) {
                            p_prop = p_prop->pGetSubProperties(component_name[i]);
                        } else {
                            std::cerr << "Index is wrong, does not correspond with any sub Properties Id: " << rAddress << std::endl;
                            exit(0);
                        }
                    }
                    return p_prop;
                } else {
                    std::cerr << "First index is wrong, does not correspond with any sub Properties Id: " << component_name[0] << std::endl;
                    exit(0);
                }
            }
            Properties& ModelPart::GetProperties(
                const std::string& rAddress,
                IndexType MeshIndex
                )
            {
                return *pGetProperties(rAddress, MeshIndex);
            }
            const Properties& ModelPart::GetProperties(
                const std::string& rAddress,
                IndexType MeshIndex
                ) const
            {
                return *pGetProperties(rAddress, MeshIndex);
            }
            void ModelPart::RemoveProperties(ModelPart::IndexType PropertiesId, IndexType ThisIndex)
            {
                GetMesh(ThisIndex).RemoveProperties(PropertiesId);

                for (SubModelPartIterator i_sub_model_part = SubModelPartsBegin(); i_sub_model_part != SubModelPartsEnd(); i_sub_model_part++)
                    i_sub_model_part->RemoveProperties(PropertiesId, ThisIndex);
            }
            /** Remove given Properties from mesh with ThisIndex in this modelpart and all its subs.
            */
            void ModelPart::RemoveProperties(ModelPart::PropertiesType& ThisProperties, ModelPart::IndexType ThisIndex)
            {
                GetMesh(ThisIndex).RemoveProperties(ThisProperties);

                for (SubModelPartIterator i_sub_model_part = SubModelPartsBegin(); i_sub_model_part != SubModelPartsEnd(); i_sub_model_part++)
                    i_sub_model_part->RemoveProperties(ThisProperties, ThisIndex);
            }
            /** Remove given Properties from mesh with ThisIndex in this modelpart and all its subs.
            */
            void ModelPart::RemoveProperties(ModelPart::PropertiesType::Pointer pThisProperties, ModelPart::IndexType ThisIndex)
            {
                GetMesh(ThisIndex).RemoveProperties(pThisProperties);

                for (SubModelPartIterator i_sub_model_part = SubModelPartsBegin(); i_sub_model_part != SubModelPartsEnd(); i_sub_model_part++)
                    i_sub_model_part->RemoveProperties(pThisProperties, ThisIndex);
            }
            /** Remove the Properties with given Id from mesh with ThisIndex in parents, itself and children.
            */
            void ModelPart::RemovePropertiesFromAllLevels(ModelPart::IndexType PropertiesId, IndexType ThisIndex)
            {
                if (IsSubModelPart())
                {
                    mpParentModelPart->RemovePropertiesFromAllLevels(PropertiesId, ThisIndex);
                    return;
                }

                RemoveProperties(PropertiesId, ThisIndex);
            }
            /** Remove given Properties from mesh with ThisIndex in parents, itself and children.
            */
            void ModelPart::RemovePropertiesFromAllLevels(ModelPart::PropertiesType& ThisProperties, ModelPart::IndexType ThisIndex)
            {
                if (IsSubModelPart())
                {
                    mpParentModelPart->RemoveProperties(ThisProperties, ThisIndex);
                }
                RemoveProperties(ThisProperties, ThisIndex);
            }
            /** Remove given Properties from mesh with ThisIndex in parents, itself and children.
            */
            void ModelPart::RemovePropertiesFromAllLevels(ModelPart::PropertiesType::Pointer pThisProperties, ModelPart::IndexType ThisIndex)
            {
                if (IsSubModelPart())
                {
                    mpParentModelPart->RemoveProperties(pThisProperties, ThisIndex);
                }

                RemoveProperties(pThisProperties, ThisIndex);
            }
        /// @}

        /// @name Elements
        /// @{
            /** Inserts a element in the mesh with ThisIndex.
            */
            void ModelPart::AddElement(ModelPart::ElementType::Pointer pNewElement, ModelPart::IndexType ThisIndex)
            {
                if (IsSubModelPart())
                {
                    mpParentModelPart->AddElement(pNewElement, ThisIndex);
                    GetMesh(ThisIndex).AddElement(pNewElement);
                }
                else
                {
                    auto existing_element_it = this->GetMesh(ThisIndex).Elements().find(pNewElement->Id());
                    if( existing_element_it == GetMesh(ThisIndex).ElementsEnd()) //node did not exist
                    {
                        GetMesh(ThisIndex).AddElement(pNewElement);
                    }
                    else //node did exist already
                    {
                        if(&(*existing_element_it) != (&*(pNewElement)))
                        {
                            std::cerr //check if the pointee coincides
                            << "attempting to add pNewElement with Id :" << pNewElement->Id() << ", unfortunately a (different) element with the same Id already exists" << std::endl;
                            exit(0);
                        }
                    }
                }
            }
            /** Inserts a list of conditions to a submodelpart provided their Id. Does nothing if applied to the top model part
            */
            void ModelPart::AddElements(std::vector<IndexType> const& ElementIds, IndexType ThisIndex)
            {
                if(IsSubModelPart()) //does nothing if we are on the top model part
                {
                    //obtain from the root model part the corresponding list of nodes
                    ModelPart* root_model_part = &this->GetRootModelPart();
                    ModelPart::ElementsContainerType  aux;
                    aux.reserve(ElementIds.size());
                    for(unsigned int i=0; i<ElementIds.size(); i++)
                    {
                        ModelPart::ElementsContainerType::iterator it = root_model_part->Elements().find(ElementIds[i]);
                        if(it!=root_model_part->ElementsEnd())
                            aux.push_back(*(it.base()));
                        else
                        {
                            std::cerr << "the element with Id " << ElementIds[i] << " does not exist in the root model part";
                            exit(0);
                        }
                            
                    }
                    ModelPart* current_part = this;
                    while(current_part->IsSubModelPart())
                    {
                        for(auto it = aux.begin(); it!=aux.end(); it++)
                            current_part->Elements().push_back( *(it.base()) );

                        current_part->Elements().Unique();

                        current_part = &(current_part->GetParentModelPart());
                    }
                }
            }
            /** Inserts an element in the mesh with ThisIndex.
            */
            ModelPart::ElementType::Pointer ModelPart::CreateNewElement(std::string ElementName,
                    ModelPart::IndexType Id, std::vector<ModelPart::IndexType> ElementNodeIds,
                    ModelPart::PropertiesType::Pointer pProperties, ModelPart::IndexType ThisIndex)
            {   
                if (IsSubModelPart())
                {
                    ElementType::Pointer p_new_element = mpParentModelPart->CreateNewElement(ElementName, Id, ElementNodeIds, pProperties, ThisIndex);
                    GetMesh(ThisIndex).AddElement(p_new_element);
                    return p_new_element;
                }
                Geometry< Node >::PointsContainerType pElementNodes;
                for (unsigned int i = 0; i < ElementNodeIds.size(); i++)
                {
                    pElementNodes.push_back(pGetNode(ElementNodeIds[i]));
                }
                return CreateNewElement(ElementName, Id, pElementNodes, pProperties, ThisIndex);
            }
            /** Inserts an element in the mesh with ThisIndex.
            */
            ModelPart::ElementType::Pointer ModelPart::CreateNewElement(std::string ElementName,
                                                                        ModelPart::IndexType Id, Geometry< Node >::PointsContainerType pElementNodes,
                                                                        ModelPart::PropertiesType::Pointer pProperties, ModelPart::IndexType ThisIndex)
            {
                if (IsSubModelPart())
                {
                    ElementType::Pointer p_new_element = mpParentModelPart->CreateNewElement(ElementName, Id, pElementNodes, pProperties, ThisIndex);
                    GetMesh(ThisIndex).AddElement(p_new_element);
                    return p_new_element;
                }
                auto existing_element_iterator = GetMesh(ThisIndex).Elements().find(Id);
                if(existing_element_iterator != GetMesh(ThisIndex).ElementsEnd() )
                {
                    std::cerr << "trying to construct an element with ID " << Id << " however an element with the same Id already exists";
                    exit(0);
                }
                    
                //create the new element
                ElementType const& r_clone_element = Lotus_Components<ElementType>::Get(ElementName);
                Element::Pointer p_element = r_clone_element.Create(Id, pElementNodes, pProperties);

                //add the new element
                GetMesh(ThisIndex).AddElement(p_element);
                return p_element;
            }

            /** Inserts an element in the mesh with ThisIndex.
            */
            ModelPart::ElementType::Pointer ModelPart::CreateNewElement(std::string ElementName,
                    ModelPart::IndexType Id, typename GeometryType::Pointer pGeometry,
                    ModelPart::PropertiesType::Pointer pProperties, ModelPart::IndexType ThisIndex)
            {
                if (IsSubModelPart())
                {
                    ElementType::Pointer p_new_element = mpParentModelPart->CreateNewElement(ElementName, Id, pGeometry, pProperties, ThisIndex);
                    GetMesh(ThisIndex).AddElement(p_new_element);
                    return p_new_element;
                }
                auto existing_element_iterator = GetMesh(ThisIndex).Elements().find(Id);
                if(existing_element_iterator != GetMesh(ThisIndex).ElementsEnd() )
                {
                    std::cerr<< "trying to construct an element with ID " << Id << " however an element with the same Id already exists";
                    exit(0); 
                }   
                //create the new element
                ElementType const& r_clone_element = Lotus_Components<ElementType>::Get(ElementName);
                Element::Pointer p_element = r_clone_element.Create(Id, pGeometry, pProperties);
                //add the new element
                GetMesh(ThisIndex).AddElement(p_element);
                return p_element;
            }
            /** Remove the element with given Id from mesh with ThisIndex in this modelpart and all its subs.
            */
            void ModelPart::RemoveElement(ModelPart::IndexType ElementId, ModelPart::IndexType ThisIndex)
            {
                GetMesh(ThisIndex).RemoveElement(ElementId);

                for (SubModelPartIterator i_sub_model_part = SubModelPartsBegin(); i_sub_model_part != SubModelPartsEnd(); i_sub_model_part++)
                    i_sub_model_part->RemoveElement(ElementId, ThisIndex);
            }
            /** Remove given element from mesh with ThisIndex in this modelpart and all its subs.
            */
            void ModelPart::RemoveElement(ModelPart::ElementType& ThisElement, ModelPart::IndexType ThisIndex)
            {
                GetMesh(ThisIndex).RemoveElement(ThisElement);

                for (SubModelPartIterator i_sub_model_part = SubModelPartsBegin(); i_sub_model_part != SubModelPartsEnd(); i_sub_model_part++)
                    i_sub_model_part->RemoveElement(ThisElement, ThisIndex);
            }
            /** Remove given element from mesh with ThisIndex in this modelpart and all its subs.
            */
            void ModelPart::RemoveElement(ModelPart::ElementType::Pointer pThisElement, ModelPart::IndexType ThisIndex)
            {
                GetMesh(ThisIndex).RemoveElement(pThisElement);
                for (SubModelPartIterator i_sub_model_part = SubModelPartsBegin(); i_sub_model_part != SubModelPartsEnd(); i_sub_model_part++)
                    i_sub_model_part->RemoveElement(pThisElement, ThisIndex);
            }
            /** Remove the element with given Id from mesh with ThisIndex in parents, itself and children.
            */
            void ModelPart::RemoveElementFromAllLevels(ModelPart::IndexType ElementId, ModelPart::IndexType ThisIndex)
            {
                if (IsSubModelPart())
                {
                    mpParentModelPart->RemoveElement(ElementId, ThisIndex);
                    return;
                }
                RemoveElement(ElementId, ThisIndex);
            }
            /** Remove given element from mesh with ThisIndex in parents, itself and children.
            */
            void ModelPart::RemoveElementFromAllLevels(ModelPart::ElementType& ThisElement, ModelPart::IndexType ThisIndex)
            {
                if (IsSubModelPart())
                {
                    mpParentModelPart->RemoveElement(ThisElement, ThisIndex);
                    return;
                }
                RemoveElement(ThisElement, ThisIndex);
            }
            /** Remove given element from mesh with ThisIndex in parents, itself and children.
            */
            void ModelPart::RemoveElementFromAllLevels(ModelPart::ElementType::Pointer pThisElement, ModelPart::IndexType ThisIndex)
            {
                if (IsSubModelPart())
                {
                    mpParentModelPart->RemoveElement(pThisElement, ThisIndex);
                    return;
                }
                RemoveElement(pThisElement, ThisIndex);
            }
            void ModelPart::RemoveElements(Flags IdentifierFlag)
            {
                // This method is optimized to free the memory
                //loop over all the meshes
                auto& meshes = this->GetMeshes();
                for(ModelPart::MeshesContainerType::iterator i_mesh = meshes.begin() ; i_mesh != meshes.end() ; i_mesh++)
                {
                    //count the elements to be erase
                    const unsigned int nelements = i_mesh->Elements().size();
                    unsigned int erase_count = 0;
                    #pragma omp parallel for reduction(+:erase_count)
                    for(int i=0; i<static_cast<int>(nelements); ++i)
                    {
                        auto i_elem = i_mesh->ElementsBegin() + i;

                        if( i_elem->IsNot(IdentifierFlag) )
                            erase_count++;
                    }

                    ModelPart::ElementsContainerType temp_elements_container;
                    temp_elements_container.reserve(i_mesh->Elements().size() - erase_count);

                    temp_elements_container.swap(i_mesh->Elements());

                    for(ModelPart::ElementsContainerType::iterator i_elem = temp_elements_container.begin() ; i_elem != temp_elements_container.end() ; i_elem++)
                    {
                        if( i_elem->IsNot(IdentifierFlag) )
                            (i_mesh->Elements()).push_back(std::move(*(i_elem.base())));
                    }
                }

                //now recursively remove the elements in the submodelparts
                for (SubModelPartIterator i_sub_model_part = SubModelPartsBegin(); i_sub_model_part != SubModelPartsEnd(); i_sub_model_part++)
                    i_sub_model_part->RemoveElements(IdentifierFlag);
            }
            void ModelPart::RemoveElementsFromAllLevels(Flags IdentifierFlag)
            {
                ModelPart& root_model_part = GetRootModelPart();
                root_model_part.RemoveElements(IdentifierFlag);
            }

        /// @}


        /// @name Condions
        /// @{
            /** Inserts a condition in the mesh with ThisIndex.
            */
            void ModelPart::AddCondition(ModelPart::ConditionType::Pointer pNewCondition, ModelPart::IndexType ThisIndex)
            {
                if (IsSubModelPart())
                {
                    mpParentModelPart->AddCondition(pNewCondition, ThisIndex);
                    GetMesh(ThisIndex).AddCondition(pNewCondition);
                }
                else
                {
                    auto existing_condition_it = this->GetMesh(ThisIndex).Conditions().find(pNewCondition->Id());
                    if( existing_condition_it == GetMesh(ThisIndex).ConditionsEnd()) //node did not exist
                    {
                        GetMesh(ThisIndex).AddCondition(pNewCondition);
                    }
                    else //node did exist already
                    {
                        if(&(*existing_condition_it) != &(*pNewCondition))//check if the pointee coincides
                        {
                            std::cerr<< "attempting to add pNewCondition with Id :" << pNewCondition->Id() << ", unfortunately a (different) condition with the same Id already exists" << std::endl;
                            exit(0);
                        }
                            
                    }
                }
            }
            /** Inserts a list of conditions to a submodelpart provided their Id. Does nothing if applied to the top model part
            */
            void ModelPart::AddConditions(std::vector<IndexType> const& ConditionIds, IndexType ThisIndex)
            {
                if(IsSubModelPart()) //does nothing if we are on the top model part
                {
                    //obtain from the root model part the corresponding list of nodes
                    ModelPart* root_model_part = &this->GetRootModelPart();
                    ModelPart::ConditionsContainerType  aux;
                    aux.reserve(ConditionIds.size());
                    for(unsigned int i=0; i<ConditionIds.size(); i++)
                    {
                        ModelPart::ConditionsContainerType::iterator it = root_model_part->Conditions().find(ConditionIds[i]);
                        if(it!=root_model_part->ConditionsEnd())
                            aux.push_back(*(it.base()));
                        else
                        {
                            std::cerr<< "the condition with Id " << ConditionIds[i] << " does not exist in the root model part";
                            exit(0);
                        }
                    }

                    ModelPart* current_part = this;
                    while(current_part->IsSubModelPart())
                    {
                        for(auto it = aux.begin(); it!=aux.end(); it++)
                            current_part->Conditions().push_back( *(it.base()) );

                        current_part->Conditions().Unique();

                        current_part = &(current_part->GetParentModelPart());
                    }
                }
            }
            /** Inserts a condition in the mesh with ThisIndex.
            */
            ModelPart::ConditionType::Pointer ModelPart::CreateNewCondition(std::string ConditionName,
                    ModelPart::IndexType Id, std::vector<IndexType> ConditionNodeIds,
                    ModelPart::PropertiesType::Pointer pProperties, ModelPart::IndexType ThisIndex)
            {
                Geometry< Node >::PointsContainerType pConditionNodes;

                for (unsigned int i = 0; i < ConditionNodeIds.size(); i++)
                {
                    pConditionNodes.push_back(pGetNode(ConditionNodeIds[i]));
                }
                return CreateNewCondition(ConditionName, Id, pConditionNodes, pProperties, ThisIndex);
            }
            /** Inserts a condition in the mesh with ThisIndex.
            */
            ModelPart::ConditionType::Pointer ModelPart::CreateNewCondition(std::string ConditionName,
                        ModelPart::IndexType Id, Geometry< Node >::PointsContainerType pConditionNodes,
                        ModelPart::PropertiesType::Pointer pProperties, ModelPart::IndexType ThisIndex)
            {
                if (IsSubModelPart())
                {
                    ConditionType::Pointer p_new_condition = mpParentModelPart->CreateNewCondition(ConditionName, Id, pConditionNodes, pProperties, ThisIndex);
                    GetMesh(ThisIndex).AddCondition(p_new_condition);
                    return p_new_condition;
                }
                auto existing_condition_iterator = GetMesh(ThisIndex).Conditions().find(Id);
                if(existing_condition_iterator != GetMesh(ThisIndex).ConditionsEnd() )
                {
                    std::cerr<< "trying to construct a condition with ID " << Id << " however a condition with the same Id already exists";
                    exit(0);
                }
                //get the condition
                ConditionType const& r_clone_condition = Lotus_Components<ConditionType>::Get(ConditionName);
                ConditionType::Pointer p_condition = r_clone_condition.Create(Id, pConditionNodes, pProperties);
                //add the new element
                GetMesh(ThisIndex).AddCondition(p_condition);
                return p_condition;
            }
            /** Inserts a condition in the mesh with ThisIndex.
            */
            ModelPart::ConditionType::Pointer ModelPart::CreateNewCondition(std::string ConditionName,
                    ModelPart::IndexType Id, typename GeometryType::Pointer pGeometry,
                    ModelPart::PropertiesType::Pointer pProperties, ModelPart::IndexType ThisIndex)
            {
                if (IsSubModelPart())
                {
                    ConditionType::Pointer p_new_condition = mpParentModelPart->CreateNewCondition(ConditionName, Id, pGeometry, pProperties, ThisIndex);
                    GetMesh(ThisIndex).AddCondition(p_new_condition);
                    return p_new_condition;
                }

                auto existing_condition_iterator = GetMesh(ThisIndex).Conditions().find(Id);
                if(existing_condition_iterator != GetMesh(ThisIndex).ConditionsEnd() )
                {
                    std::cerr<< "trying to construct a condition with ID " << Id << " however a condition with the same Id already exists";
                    exit(0);
                }
                //get the condition
                ConditionType const& r_clone_condition = Lotus_Components<ConditionType>::Get(ConditionName);
                ConditionType::Pointer p_condition = r_clone_condition.Create(Id, pGeometry, pProperties);

                //add the new element
                GetMesh(ThisIndex).AddCondition(p_condition);
                return p_condition;
            }
            /** Remove the condition with given Id from mesh with ThisIndex in this modelpart and all its subs.
            */
            void ModelPart::RemoveCondition(ModelPart::IndexType ConditionId, ModelPart::IndexType ThisIndex)
            {
                GetMesh(ThisIndex).RemoveCondition(ConditionId);

                for (SubModelPartIterator i_sub_model_part = SubModelPartsBegin(); i_sub_model_part != SubModelPartsEnd(); i_sub_model_part++)
                    i_sub_model_part->RemoveCondition(ConditionId, ThisIndex);
            }
            /** Remove given condition from mesh with ThisIndex in this modelpart and all its subs.
            */
            void ModelPart::RemoveCondition(ModelPart::ConditionType& ThisCondition, ModelPart::IndexType ThisIndex)
            {
                GetMesh(ThisIndex).RemoveCondition(ThisCondition);

                for (SubModelPartIterator i_sub_model_part = SubModelPartsBegin(); i_sub_model_part != SubModelPartsEnd(); i_sub_model_part++)
                    i_sub_model_part->RemoveCondition(ThisCondition, ThisIndex);
            }
            /** Remove given condition from mesh with ThisIndex in this modelpart and all its subs.
            */
            void ModelPart::RemoveCondition(ModelPart::ConditionType::Pointer pThisCondition, ModelPart::IndexType ThisIndex)
            {
                GetMesh(ThisIndex).RemoveCondition(pThisCondition);

                for (SubModelPartIterator i_sub_model_part = SubModelPartsBegin(); i_sub_model_part != SubModelPartsEnd(); i_sub_model_part++)
                    i_sub_model_part->RemoveCondition(pThisCondition, ThisIndex);
            }
            /** Remove the condition with given Id from mesh with ThisIndex in parents, itself and children.
            */
            void ModelPart::RemoveConditionFromAllLevels(ModelPart::IndexType ConditionId, ModelPart::IndexType ThisIndex)
            {
                if (IsSubModelPart())
                {
                    mpParentModelPart->RemoveCondition(ConditionId, ThisIndex);
                    return;
                }

                RemoveCondition(ConditionId, ThisIndex);
            }
            /** Remove given condition from mesh with ThisIndex in parents, itself and children.
            */
            void ModelPart::RemoveConditionFromAllLevels(ModelPart::ConditionType& ThisCondition, ModelPart::IndexType ThisIndex)
            {
                if (IsSubModelPart())
                {
                    mpParentModelPart->RemoveCondition(ThisCondition, ThisIndex);
                    return;
                }

                RemoveCondition(ThisCondition, ThisIndex);
            }
            /** Remove given condition from mesh with ThisIndex in parents, itself and children.
            */
            void ModelPart::RemoveConditionFromAllLevels(ModelPart::ConditionType::Pointer pThisCondition, ModelPart::IndexType ThisIndex)
            {
                if (IsSubModelPart())
                {
                    mpParentModelPart->RemoveCondition(pThisCondition, ThisIndex);
                    return;
                }

                RemoveCondition(pThisCondition, ThisIndex);
            }
            void ModelPart::RemoveConditions(Flags IdentifierFlag)
            {
                // This method is optimized to free the memory
                //loop over all the meshes
                auto& meshes = this->GetMeshes();
                for(ModelPart::MeshesContainerType::iterator i_mesh = meshes.begin() ; i_mesh != meshes.end() ; i_mesh++)
                {
                    //count the conditions to be erase
                    const unsigned int nconditions = i_mesh->Conditions().size();
                    unsigned int erase_count = 0;
                    #pragma omp parallel for reduction(+:erase_count)
                    for(int i=0; i<static_cast<int>(nconditions); ++i)
                    {
                        auto i_cond = i_mesh->ConditionsBegin() + i;

                        if( i_cond->IsNot(IdentifierFlag) )
                            erase_count++;
                    }

                    ModelPart::ConditionsContainerType temp_conditions_container;
                    temp_conditions_container.reserve(i_mesh->Conditions().size() - erase_count);

                    temp_conditions_container.swap(i_mesh->Conditions());

                    for(ModelPart::ConditionsContainerType::iterator i_cond = temp_conditions_container.begin() ; i_cond != temp_conditions_container.end() ; i_cond++)
                    {
                        if( i_cond->IsNot(IdentifierFlag) )
                            (i_mesh->Conditions()).push_back(std::move(*(i_cond.base())));
                    }
                }

                //now recursively remove the conditions in the submodelparts
                for (SubModelPartIterator i_sub_model_part = SubModelPartsBegin(); i_sub_model_part != SubModelPartsEnd(); i_sub_model_part++)
                    i_sub_model_part->RemoveConditions(IdentifierFlag);
            }
            void ModelPart::RemoveConditionsFromAllLevels(Flags IdentifierFlag)
            {
                ModelPart& root_model_part = GetRootModelPart();
                root_model_part.RemoveConditions(IdentifierFlag);
            }

        /// @}

        /// @name Geometries
        /// @{
            ModelPart::GeometryType::Pointer ModelPart::CreateNewGeometry(
                const std::string& rGeometryTypeName,
                const std::vector<IndexType>& rGeometryNodeIds
                )
            {
                if (IsSubModelPart()) {
                    GeometryType::Pointer p_new_geometry = mpParentModelPart->CreateNewGeometry(rGeometryTypeName, rGeometryNodeIds);
                    this->AddGeometry(p_new_geometry);
                    return p_new_geometry;
                }
                GeometryType::PointsContainerType p_geometry_nodes;
                for (IndexType i = 0; i < rGeometryNodeIds.size(); ++i) {
                    p_geometry_nodes.push_back(pGetNode(rGeometryNodeIds[i]));
                }
                return CreateNewGeometry(rGeometryTypeName, p_geometry_nodes);
            }
            ModelPart::GeometryType::Pointer ModelPart::CreateNewGeometry(
                const std::string& rGeometryTypeName,
                GeometryType::PointsContainerType pGeometryNodes
                )
            {
                if (IsSubModelPart()) {
                    GeometryType::Pointer p_new_geometry = mpParentModelPart->CreateNewGeometry(rGeometryTypeName, pGeometryNodes);
                    this->AddGeometry(p_new_geometry);
                    return p_new_geometry;
                }
                // Create the new geometry
                GeometryType const& r_clone_geometry = Lotus_Components<GeometryType>::Get(rGeometryTypeName);
                GeometryType::Pointer p_geometry = r_clone_geometry.Create(pGeometryNodes);
                //add the new geometry
                this->AddGeometry(p_geometry);
                return p_geometry;
            }
            ModelPart::GeometryType::Pointer ModelPart::CreateNewGeometry(
                const std::string& rGeometryTypeName,
                GeometryType::Pointer pGeometry
                )
            {
                if (IsSubModelPart()) {
                    GeometryType::Pointer p_new_geometry = mpParentModelPart->CreateNewGeometry(rGeometryTypeName, pGeometry);
                    this->AddGeometry(p_new_geometry);
                    return p_new_geometry;
                }
                // Create the new geometry
                GeometryType const& r_clone_geometry = Lotus_Components<GeometryType>::Get(rGeometryTypeName);
                GeometryType::SharedPointer p_geometry = r_clone_geometry.Create(*pGeometry);

                //add the new geometry
                this->AddGeometry(p_geometry);
                return p_geometry;

            }
            ModelPart::GeometryType::Pointer ModelPart::CreateNewGeometry(
                const std::string& rGeometryTypeName,
                const IndexType GeometryId,
                const std::vector<IndexType>& rGeometryNodeIds
                )
            {
                if (IsSubModelPart()) {
                    GeometryType::Pointer p_new_geometry = mpParentModelPart->CreateNewGeometry(rGeometryTypeName, GeometryId, rGeometryNodeIds);
                    this->AddGeometry(p_new_geometry);
                    return p_new_geometry;
                }
                GeometryType::PointsContainerType p_geometry_nodes;
                for (IndexType i = 0; i < rGeometryNodeIds.size(); ++i) {
                    p_geometry_nodes.push_back(pGetNode(rGeometryNodeIds[i]));
                }

                return CreateNewGeometry(rGeometryTypeName, GeometryId, p_geometry_nodes);
            }
            ModelPart::GeometryType::Pointer ModelPart::CreateNewGeometry(
                const std::string& rGeometryTypeName,
                const IndexType GeometryId,
                GeometryType::PointsContainerType pGeometryNodes
                )
            {
                if (IsSubModelPart()) {
                    GeometryType::Pointer p_new_geometry = mpParentModelPart->CreateNewGeometry(rGeometryTypeName, GeometryId, pGeometryNodes);
                    this->AddGeometry(p_new_geometry);
                    return p_new_geometry;
                }
                if(this->HasGeometry(GeometryId))
                {
                    std::cerr<< "Trying to construct an geometry with ID: " << GeometryId << ". A geometry with the same Id exists already." << std::endl;
                    exit(0);
                } 
                // Create the new geometry
                GeometryType const& r_clone_geometry = Lotus_Components<GeometryType>::Get(rGeometryTypeName);
                GeometryType::Pointer p_geometry = r_clone_geometry.Create(GeometryId, pGeometryNodes);
                //add the new geometry
                this->AddGeometry(p_geometry);
                return p_geometry;
            }
            ModelPart::GeometryType::Pointer ModelPart::CreateNewGeometry(
                const std::string& rGeometryTypeName,
                const IndexType GeometryId,
                GeometryType::Pointer pGeometry
                )
            {
                if (IsSubModelPart()) {
                    GeometryType::Pointer p_new_geometry = mpParentModelPart->CreateNewGeometry(rGeometryTypeName, GeometryId, pGeometry);
                    this->AddGeometry(p_new_geometry);
                    return p_new_geometry;
                }
                if(this->HasGeometry(GeometryId))
                {
                    std::cerr<< "Trying to construct an geometry with ID: " << GeometryId << ". A geometry with the same Id exists already." << std::endl;
                    exit(0);
                } 

                // Create the new geometry
                GeometryType const& r_clone_geometry = Lotus_Components<GeometryType>::Get(rGeometryTypeName);
                GeometryType::Pointer p_geometry = r_clone_geometry.Create(GeometryId, *pGeometry);
                //add the new geometry
                this->AddGeometry(p_geometry);
                return p_geometry;
            }
            ModelPart::GeometryType::Pointer ModelPart::CreateNewGeometry(
                const std::string& rGeometryTypeName,
                const std::string& rGeometryIdentifierName,
                const std::vector<IndexType>& rGeometryNodeIds
                )
            {
                if (IsSubModelPart()) {
                    GeometryType::Pointer p_new_geometry = mpParentModelPart->CreateNewGeometry(rGeometryTypeName, rGeometryIdentifierName, rGeometryNodeIds);
                    this->AddGeometry(p_new_geometry);
                    return p_new_geometry;
                }
                GeometryType::PointsContainerType p_geometry_nodes;
                for (IndexType i = 0; i < rGeometryNodeIds.size(); ++i) {
                    p_geometry_nodes.push_back(pGetNode(rGeometryNodeIds[i]));
                }
                return CreateNewGeometry(rGeometryTypeName, rGeometryIdentifierName, p_geometry_nodes);
            }
            ModelPart::GeometryType::Pointer ModelPart::CreateNewGeometry(
                const std::string& rGeometryTypeName,
                const std::string& rGeometryIdentifierName,
                GeometryType::PointsContainerType pGeometryNodes
                )
            {
                if (IsSubModelPart()) {
                    GeometryType::Pointer p_new_geometry = mpParentModelPart->CreateNewGeometry(rGeometryTypeName, rGeometryIdentifierName, pGeometryNodes);
                    this->AddGeometry(p_new_geometry);
                    return p_new_geometry;
                }

                if(this->HasGeometry(rGeometryIdentifierName)) 
                {
                    std::cerr<< "Trying to construct an geometry with name: " << rGeometryIdentifierName << ". A geometry with the same name exists already." << std::endl;
                    exit(0);
                }
                // Create the new geometry
                GeometryType const& r_clone_geometry = Lotus_Components<GeometryType>::Get(rGeometryTypeName);
                GeometryType::Pointer p_geometry = r_clone_geometry.Create(rGeometryIdentifierName, pGeometryNodes);
                //add the new geometry
                this->AddGeometry(p_geometry);
                return p_geometry;
            }
            ModelPart::GeometryType::Pointer ModelPart::CreateNewGeometry(
                const std::string& rGeometryTypeName,
                const std::string& rGeometryIdentifierName,
                GeometryType::Pointer pGeometry
                )
            {
                if (IsSubModelPart()) {
                    GeometryType::Pointer p_new_geometry = mpParentModelPart->CreateNewGeometry(rGeometryTypeName, rGeometryIdentifierName, pGeometry);
                    this->AddGeometry(p_new_geometry);
                    return p_new_geometry;
                }

                if(this->HasGeometry(rGeometryIdentifierName))
                {
                    std::cerr<< "Trying to construct an geometry with name: " << rGeometryIdentifierName << ". A geometry with the same name exists already." << std::endl;
                    exit(0);
                } 

                // Create the new geometry
                GeometryType const& r_clone_geometry = Lotus_Components<GeometryType>::Get(rGeometryTypeName);
                GeometryType::Pointer p_geometry = r_clone_geometry.Create(rGeometryIdentifierName, *pGeometry);

                //add the new geometry
                this->AddGeometry(p_geometry);
                return p_geometry;
            }
            /// Adds a geometry to the geometry container.
            void ModelPart::AddGeometry(
                typename GeometryType::Pointer pNewGeometry)
            {
                if (IsSubModelPart()) {
                    if (!mpParentModelPart->HasGeometry(pNewGeometry->Id())) {
                        mpParentModelPart->AddGeometry(pNewGeometry);
                    }
                }
                /// Check if geometry id already used, is done within the geometry container.
                mGeometries.AddGeometry(pNewGeometry);
            }
            /** Inserts a list of geometries to a submodelpart provided their Id. Does nothing if applied to the top model part
             */
            void ModelPart::AddGeometries(std::vector<IndexType> const& GeometriesIds)
            {
                if(IsSubModelPart()) { // Does nothing if we are on the top model part
                    // Obtain from the root model part the corresponding list of geomnetries
                    ModelPart* p_root_model_part = &this->GetRootModelPart();
                    std::vector<GeometryType::Pointer> aux;
                    aux.reserve(GeometriesIds.size());
                    for(auto& r_id : GeometriesIds) {
                        auto it_found = p_root_model_part->Geometries().find(r_id);
                        if(it_found != p_root_model_part->GeometriesEnd()) {
                            aux.push_back( it_found.operator->() );
                        } else {
                            std::cerr << "The geometry with Id " << r_id << " does not exist in the root model part" << std::endl;
                            exit(0);
                        }
                    }

                    ModelPart* p_current_part = this;
                    while(p_current_part->IsSubModelPart()) {
                        for(auto& p_geom : aux) {
                            p_current_part->AddGeometry(p_geom);
                        }
                        p_current_part = &(p_current_part->GetParentModelPart());
                    }
                }
            }
            /// Removes a geometry by id.
            void ModelPart::RemoveGeometry(
                const IndexType GeometryId)
            {
                mGeometries.RemoveGeometry(GeometryId);

                for (SubModelPartIterator i_sub_model_part = SubModelPartsBegin();
                    i_sub_model_part != SubModelPartsEnd();
                    ++i_sub_model_part)
                    i_sub_model_part->RemoveGeometry(GeometryId);
            }
            /// Removes a geometry by name.
            void ModelPart::RemoveGeometry(
                std::string GeometryName)
            {
                mGeometries.RemoveGeometry(GeometryName);

                for (SubModelPartIterator i_sub_model_part = SubModelPartsBegin();
                    i_sub_model_part != SubModelPartsEnd();
                    ++i_sub_model_part)
                    i_sub_model_part->RemoveGeometry(GeometryName);
            }
            /// Removes a geometry by id from all root and sub model parts.
            void ModelPart::RemoveGeometryFromAllLevels(const IndexType GeometryId)
            {
                if (IsSubModelPart())
                {
                    mpParentModelPart->RemoveGeometry(GeometryId);
                    return;
                }
                RemoveGeometry(GeometryId);
            }
            /// Removes a geometry by name from all root and sub model parts.
            void ModelPart::RemoveGeometryFromAllLevels(std::string GeometryName)
            {
                if (IsSubModelPart())
                {
                    mpParentModelPart->RemoveGeometry(GeometryName);
                    return;
                }
                RemoveGeometry(GeometryName);
            }

        /// @}

        /// @name SubModelPart
        /// @{
            ModelPart& ModelPart::CreateSubModelPart(std::string const& NewSubModelPartName)
            {
                const auto delim_pos = NewSubModelPartName.find('.');
                const std::string& sub_model_part_name = NewSubModelPartName.substr(0, delim_pos);
                if (delim_pos == std::string::npos) {
                    if(mSubModelParts.find(NewSubModelPartName) != mSubModelParts.end())
                    {
                        std::cerr<< "There is an already existing sub model part with name \"" << NewSubModelPartName
                        << "\" in model part: \"" << FullName() << "\"" << std::endl;
                        exit(0);
                    }
                    ModelPart* praw = new ModelPart(NewSubModelPartName, this->mpVariablesList, this->GetModel());
                    std::shared_ptr<ModelPart> p_model_part(praw); //we need to construct first a raw pointer
                    p_model_part->SetParentModelPart(this);
                    p_model_part->mBufferSize = this->mBufferSize;
                    p_model_part->mpProcessInfo = this->mpProcessInfo;
                    mSubModelParts.insert(p_model_part);
                    return *p_model_part;
                } else {
                    ModelPart *p;
                    SubModelPartIterator i = mSubModelParts.find(sub_model_part_name);
                    if (i == mSubModelParts.end()) {
                        p = &CreateSubModelPart(sub_model_part_name);
                    } else {
                        p = &(*i);
                    }
                    return p->CreateSubModelPart(NewSubModelPartName.substr(delim_pos + 1));
                }
            }
            ModelPart& ModelPart::GetSubModelPart(std::string const& SubModelPartName)
            {
                const auto delim_pos = SubModelPartName.find('.');
                const std::string& sub_model_part_name = SubModelPartName.substr(0, delim_pos);

                SubModelPartIterator i = mSubModelParts.find(sub_model_part_name);
                if (i == mSubModelParts.end()) {
                    ErrorNonExistingSubModelPart(sub_model_part_name);
                }

                if (delim_pos == std::string::npos) {
                    return *i;
                } else {
                    return i->GetSubModelPart(SubModelPartName.substr(delim_pos + 1));
                }
            }
            const ModelPart& ModelPart::GetSubModelPart(std::string const& SubModelPartName) const
            {
                const auto delim_pos = SubModelPartName.find('.');
                const std::string& r_sub_model_part_name = SubModelPartName.substr(0, delim_pos);

                const auto i = mSubModelParts.find(r_sub_model_part_name);
                if (i == mSubModelParts.end()) {
                    ErrorNonExistingSubModelPart(r_sub_model_part_name);
                }

                if (delim_pos == std::string::npos) {
                    return *i;
                } else {
                    return i->GetSubModelPart(SubModelPartName.substr(delim_pos + 1));
                }
            }
            ModelPart* ModelPart::pGetSubModelPart(std::string const& SubModelPartName)
            {
                const auto delim_pos = SubModelPartName.find('.');
                const std::string& sub_model_part_name = SubModelPartName.substr(0, delim_pos);
                SubModelPartIterator i = mSubModelParts.find(sub_model_part_name);
                if (i == mSubModelParts.end()) {
                    ErrorNonExistingSubModelPart(sub_model_part_name);
                }

                if (delim_pos == std::string::npos) {
                    return  (i.base()->second).get();
                } else {
                    return i->pGetSubModelPart(SubModelPartName.substr(delim_pos + 1));
                }
            }
            /** Remove a sub modelpart with given name.
            */
            void ModelPart::RemoveSubModelPart(std::string const& ThisSubModelPartName)
            {
                const auto delim_pos = ThisSubModelPartName.find('.');
                const std::string& sub_model_part_name = ThisSubModelPartName.substr(0, delim_pos);

                SubModelPartIterator i = mSubModelParts.find(sub_model_part_name);
                if (delim_pos == std::string::npos) {
                    if (i == mSubModelParts.end()) {
                        std::stringstream warning_msg;
                        warning_msg << "Trying to remove sub model part with name \"" << ThisSubModelPartName
                                << "\" in model part \"" << FullName() << "\" which does not exist.\n"
                                << "The the following sub model parts are available:";
                        for (const auto& r_avail_smp_name : GetSubModelPartNames()) {
                            warning_msg << "\n\t" "\"" << r_avail_smp_name << "\"";
                        }
                        std::cerr << ("ModelPart") << warning_msg.str() << std::endl;
                    } else {
                        mSubModelParts.erase(ThisSubModelPartName);
                    }
                } else {
                    if (i == mSubModelParts.end()) {
                        ErrorNonExistingSubModelPart(sub_model_part_name);
                    }
                    return i->RemoveSubModelPart(ThisSubModelPartName.substr(delim_pos + 1));
                }
            }
            /** Remove given sub model part.
            */
            void ModelPart::RemoveSubModelPart(ModelPart& ThisSubModelPart)
            {
                std::string name = ThisSubModelPart.Name();
                // finding the sub model part
                SubModelPartIterator i_sub_model_part = mSubModelParts.find(name);
                if(i_sub_model_part == mSubModelParts.end())
                {
                    std::cerr<< "The sub model part  \"" << name << "\" does not exist in the \"" << Name() << "\" model part to be removed" << std::endl;
                    exit(0);
                } 

                mSubModelParts.erase(name);
            }
            ModelPart& ModelPart::GetParentModelPart()
            {
                if (IsSubModelPart()) {
                    return *mpParentModelPart;
                } else {
                    return *this;
                }
            }
            const ModelPart& ModelPart::GetParentModelPart() const
            {
                if (IsSubModelPart()) {
                    return *mpParentModelPart;
                } else {
                    return *this;
                }
            }
            bool ModelPart::HasSubModelPart(std::string const& ThisSubModelPartName) const
            {
                const auto delim_pos = ThisSubModelPartName.find('.');
                const std::string& sub_model_part_name = ThisSubModelPartName.substr(0, delim_pos);

                auto i = mSubModelParts.find(sub_model_part_name);
                if (i == mSubModelParts.end()) {
                    return false;
                } else {
                    if (delim_pos != std::string::npos) {
                        return i->HasSubModelPart(ThisSubModelPartName.substr(delim_pos + 1));
                    } else {
                        return true;
                    }
                }
            }
            std::vector<std::string> ModelPart::GetSubModelPartNames() const
            {
                std::vector<std::string> SubModelPartsNames;
                SubModelPartsNames.reserve(NumberOfSubModelParts());

                for(auto& r_sub_model_part : mSubModelParts) {
                    SubModelPartsNames.push_back(r_sub_model_part.Name());
                }

                return SubModelPartsNames;
            }
            void ModelPart::SetBufferSize(ModelPart::IndexType NewBufferSize)
            {
                if(IsSubModelPart())
                {
                    std::cerr<< "Calling the method of the sub model part "
                    << Name() << " please call the one of the root model part: "
                    << GetRootModelPart().Name() << std::endl;
                    exit(0);
                } 
                for(auto& r_sub_model_part : mSubModelParts) {
                    r_sub_model_part.SetBufferSizeSubModelParts(NewBufferSize);
                }
                mBufferSize = NewBufferSize;
                auto nodes_begin = NodesBegin();
                const int nnodes = static_cast<int>(Nodes().size());
                #pragma omp parallel for firstprivate(nodes_begin,nnodes)
                for(int i = 0; i<nnodes; ++i)
                {
                    auto node_iterator = nodes_begin + i;
                    node_iterator->SetBufferSize(mBufferSize);
                }

            }
            void ModelPart::SetBufferSizeSubModelParts(ModelPart::IndexType NewBufferSize)
            {
                for(auto& r_sub_model_part : mSubModelParts) {
                    r_sub_model_part.SetBufferSizeSubModelParts(NewBufferSize);
                }

                mBufferSize = NewBufferSize;
            }
                /// @}
    /// @}

    int ModelPart::Check() const
    {
        const Process_Info& r_current_process_info = this->GetProcessInfo();

        // Checks for all of the elements
        for_each(this->Elements().begin(),this->Elements().end(), [&r_current_process_info](const Element& rElement){
            rElement.Check(r_current_process_info);
        });

        // Checks for all of the conditions
        for_each(this->Conditions().begin(),this->Conditions().end(), [&r_current_process_info](const Condition& rCondition){
            rCondition.Check(r_current_process_info);
        });

        return 0;
    }


    void ModelPart::PrintInfo(std::ostream& rOStream) const
    {
        rOStream << Info();
    }

    /// Print object's data.

    void ModelPart::PrintData(std::ostream& rOStream) const
    {
        Data_Value_Container::PrintData(rOStream);

        if (!IsSubModelPart()) {
            rOStream  << "    Buffer Size : " << mBufferSize << std::endl;
        }
        rOStream << "    Number of tables : " << NumberOfTables() << std::endl;
        rOStream << "    Number of sub model parts : " << NumberOfSubModelParts() << std::endl;
        if (!IsSubModelPart()) {
            mpProcessInfo->PrintData(rOStream);
        }
        rOStream << std::endl;
        rOStream << "    Number of Geometries  : " << mGeometries.NumberOfGeometries() << std::endl;
        for (IndexType i = 0; i < mMeshes.size(); i++) {
            rOStream << "    Mesh " << i << " :" << std::endl;
            GetMesh(i).PrintData(rOStream, "    ");
        }
        rOStream << std::endl;

        // Printing the submodelparts by their names in alphabetical order
        std::vector< std::string > submodel_part_names;
        submodel_part_names.reserve(NumberOfSubModelParts());
        for (const auto& r_sub_model_part : mSubModelParts) {
            submodel_part_names.push_back(r_sub_model_part.Name());
        }
        std::sort(submodel_part_names.begin(),submodel_part_names.end());

        for (const auto& r_sub_model_part_name : submodel_part_names) {
            const auto& r_sub_model_part = *(mSubModelParts.find(r_sub_model_part_name));
            r_sub_model_part.PrintInfo(rOStream, "    ");
            rOStream << std::endl;
            r_sub_model_part.PrintData(rOStream, "    ");
        }
    }

    /// Print information about this object.

    void ModelPart::PrintInfo(std::ostream& rOStream, std::string const& PrefixString) const
    {
        rOStream << PrefixString << Info();
    }

    /// Print object's data.

    void ModelPart::PrintData(std::ostream& rOStream, std::string const& PrefixString) const
    {
        if (!IsSubModelPart()) {
            rOStream << PrefixString << "    Buffer Size : " << mBufferSize << std::endl;
        }
        rOStream << PrefixString << "    Number of tables : " << NumberOfTables() << std::endl;
        rOStream << PrefixString << "    Number of sub model parts : " << NumberOfSubModelParts() << std::endl;

        if (!IsSubModelPart()) {
            mpProcessInfo->PrintData(rOStream);
        }
        rOStream << std::endl;
        rOStream << PrefixString << "    Number of Geometries  : " << mGeometries.NumberOfGeometries() << std::endl;

        for (IndexType i = 0; i < mMeshes.size(); i++) {
            rOStream << PrefixString << "    Mesh " << i << " :" << std::endl;
            GetMesh(i).PrintData(rOStream, PrefixString + "    ");
        }

        // Printing the submodelparts by their names in alphabetical order
        std::vector< std::string > submodel_part_names;
        submodel_part_names.reserve(NumberOfSubModelParts());
        for (const auto& r_sub_model_part : mSubModelParts) {
            submodel_part_names.push_back(r_sub_model_part.Name());
        }
        std::sort(submodel_part_names.begin(),submodel_part_names.end());

        for (const auto& r_sub_model_part_name : submodel_part_names) {
            const auto& r_sub_model_part = *(mSubModelParts.find(r_sub_model_part_name));
            r_sub_model_part.PrintInfo(rOStream, PrefixString + "    ");
            rOStream << std::endl;
            r_sub_model_part.PrintData(rOStream, PrefixString + "    ");
        }
    }

/// @}


/// @name Access
/// @{


/// @}


/// @name Inquiry
/// @{


/// @}


/// @brief protected:
/// @name Protected Static Member Variables
/// @{


/// @}


/// @name Protected Member Variables
/// @{


/// @}


/// @name Protected Operatiors
/// @{


/// @}


/// @name Protected Operations
/// @{


/// @}


/// @name Protected Access
/// @{


/// @}


/// @name Protected Inquiry
/// @{


/// @}


/// @brief private:
/// @name Private Static Member Variables
/// @{


/// @}


/// @name Private Member Variables
/// @{


/// @}


/// @name Private Operatiors
/// @{


/// @}


/// @name Private Operations
/// @{


/// @}


/// @name Private Access
/// @{


/// @}


/// @name Private Inquiry
/// @{


/// @}