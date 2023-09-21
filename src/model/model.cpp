#include "../../include/Model/model.h"

void Model::Reset()
{
    mRootModelPartMap.clear();
}

void Model::CreateRootModelPart(const std::string& ModelPartName, ModelPart::IndexType NewBufferSize)
{
    auto p_var_list = std::make_shared<Variables_List>();

    ModelPart* p_model_part = new ModelPart(ModelPartName, NewBufferSize, p_var_list, *this);
    mRootModelPartMap[ModelPartName] = std::unique_ptr<ModelPart>(p_model_part); // note that i create it separately since Model is friend of ModelPart but unique_ptr is not
}

ModelPart& Model::CreateModelPart( const std::string& ModelPartName, ModelPart::IndexType NewBufferSize )
{
    if( ModelPartName.empty() )
    {
        std::cerr<< "Please don't use empty names (\"\") when creating a ModelPart" << std::endl;
        exit(0);
    } 
    const auto delim_pos = ModelPartName.find('.');
    const std::string& root_model_part_name = ModelPartName.substr(0, delim_pos);

    if (delim_pos == std::string::npos) {
        if (mRootModelPartMap.find(root_model_part_name) == mRootModelPartMap.end()) {
            CreateRootModelPart(root_model_part_name, NewBufferSize);
            return *(mRootModelPartMap[root_model_part_name].get());
        } else {
            std::cerr << ("Model") << "Trying to create a root modelpart with name " << ModelPartName << " however a ModelPart with the same name already exists. \nReturning the already existent ModelPart.\n";
            return *(mRootModelPartMap[root_model_part_name].get());
        }
    } else {
        if (mRootModelPartMap.find(root_model_part_name) == mRootModelPartMap.end()) {
            CreateRootModelPart(root_model_part_name, NewBufferSize);
        }
        return mRootModelPartMap[root_model_part_name]->CreateSubModelPart(ModelPartName.substr(delim_pos + 1));
    }

}

void Model::DeleteModelPart( const std::string& rModelPartName  )
{

    if(this->HasModelPart(rModelPartName)) {
        //NOTE: the corresponding variable list should NOT be removed
        const auto delim_pos = rModelPartName.find('.');
        if (delim_pos == std::string::npos) {
            // It is a root model part
            mRootModelPartMap.erase(rModelPartName); 
        } else {
            // Use root_model_part to delete submodelpart
            const std::string& root_model_part_name = rModelPartName.substr(0, delim_pos);
            ModelPart& r_root_model_part = this->GetModelPart(root_model_part_name);
            r_root_model_part.RemoveSubModelPart(rModelPartName.substr(delim_pos+1));
        }
    } else {
        std::cerr << ("Model") << "Attempting to delete non-existent modelpart : " << rModelPartName << std::endl;
    }
}

void Model::RenameModelPart( const std::string& OldName, const std::string& NewName )
{

    if(!this->HasModelPart(OldName))
    {
        std::cerr<< "The Old Name is not in model (as a root model part). Required old name was : " << OldName << std::endl;
        exit(0); 
    } 

    if(this->HasModelPart(NewName))
    {
        std::cerr<< "The New Name is already existing in model. Proposed name was : " << NewName << std::endl;
        exit(0); 
    } 

    mRootModelPartMap[OldName]->Name() = NewName; //change the name of the existing modelpart

    CreateModelPart(NewName);

    mRootModelPartMap[NewName].swap(mRootModelPartMap[OldName]);

    mRootModelPartMap.erase(OldName);

}

/**
 * @brief This method gets the names of all parent-modelparts given a submodelpart-name
 * @param rModelPart The SubModelPart for which the parents-modelpart-names are to be extracted
 * @param rModelPartNames The names of the ModelParts
 * @TODO remove this function when the flat-map is removed (it will no longer be needed)
 */
void GetNameWithAscendants(const ModelPart& rModelPart, std::vector<std::string>& rModelPartNames)
{
    rModelPartNames.insert(rModelPartNames.begin(), rModelPart.Name()); // "push_front"
    if (rModelPart.IsSubModelPart()) {
        GetNameWithAscendants(rModelPart.GetParentModelPart(), rModelPartNames);
    }
}
ModelPart& Model::GetModelPart(const std::string& rFullModelPartName)
{

    if( rFullModelPartName.empty() )
    {
        std::cerr << "Attempting to find a "
        << "ModelPart with empty name (\"\")!" << std::endl;
        exit(0);
    } 

    const auto delim_pos = rFullModelPartName.find('.');
    const std::string& root_model_part_name = rFullModelPartName.substr(0, delim_pos);

    if (delim_pos == std::string::npos) { //it is a root model part
        auto search = mRootModelPartMap.find(root_model_part_name);
        if(search != mRootModelPartMap.end()) {
            return *(search->second);
        } else { //let's also search it as a flat name - a feature that SHOULD BE DEPRECATED
            for(auto it = mRootModelPartMap.begin(); it!=mRootModelPartMap.end(); it++) {
                ModelPart* pmodel_part = RecursiveSearchByName(root_model_part_name, (it->second.get()));
                if (pmodel_part != nullptr) { //give back the first one that was found
                    // Get the names of the parent-modelparts to print them in the warning
                    std::vector<std::string> model_part_names;
                    GetNameWithAscendants(*pmodel_part, model_part_names);

                    std::stringstream msg;
                    msg << model_part_names[0];
                    for (std::size_t i=1; i<model_part_names.size(); ++i) {
                        msg << "." << model_part_names[1];
                    }

                    std::cerr << "DEPRECATION: The ModelPart \"" << root_model_part_name << "\" is retrieved from the Model by using the flat-map!\nThis was removed end of November 2019\nPlease prepend the Parent-ModelPart-Names like this:\n\"" << msg.str() << "\"" << std::endl;
                    exit(0);
                    return *pmodel_part;
                }
            }

            //if we are here we did not find it
            std::cerr << "The ModelPart named : \"" << root_model_part_name
                    << "\" was not found either as root-ModelPart or as a flat name. The total input string was \""
                    << rFullModelPartName << "\"" << std::endl;
            exit(0);
        }
    }
    else //it is a submodelpart with the full name provided
    {
        auto search = mRootModelPartMap.find(root_model_part_name);
        if(search != mRootModelPartMap.end()) {
            ModelPart* p_model_part = (search->second).get();
            return p_model_part->GetSubModelPart(rFullModelPartName.substr(delim_pos + 1));
        } else {
            std::cerr << "root model part " << rFullModelPartName << " not found" << std::endl;
            exit(0);
        }

    }
}

const ModelPart& Model::GetModelPart(const std::string& rFullModelPartName) const
{

    if( rFullModelPartName.empty() )
    {
        std::cerr << "Attempting to find a "
        << "ModelPart with empty name (\"\")!" << std::endl;
        exit(0);
    } 

    const auto delim_pos = rFullModelPartName.find('.');
    const std::string& root_model_part_name = rFullModelPartName.substr(0, delim_pos);

    if (delim_pos == std::string::npos) { //it is a root model part
        auto search = mRootModelPartMap.find(root_model_part_name);
        if(search != mRootModelPartMap.end()) {
            return *(search->second);
        } else { //let's also search it as a flat name - a feature that SHOULD BE DEPRECATED
            for(auto it = mRootModelPartMap.begin(); it!=mRootModelPartMap.end(); it++) {
                ModelPart* p_model_part = RecursiveSearchByName(root_model_part_name, (it->second.get()));
                if (p_model_part != nullptr) { //give back the first one that was found
                    // Get the names of the parent-modelparts to print them in the warning
                    std::vector<std::string> model_part_names;
                    GetNameWithAscendants(*p_model_part, model_part_names);

                    std::stringstream msg;
                    msg << model_part_names[0];
                    for (std::size_t i=1; i<model_part_names.size(); ++i) {
                        msg << "." << model_part_names[1];
                    }

                    std::cerr << "DEPRECATION: The ModelPart \"" << root_model_part_name << "\" is retrieved from the Model by using the flat-map!\nThis was removed end of November 2019\nPlease prepend the Parent-ModelPart-Names like this:\n\"" << msg.str() << "\"" << std::endl;
                    exit(0);
                    return *p_model_part;
                }
            }

            //if we are here we did not find it
            std::cerr << "The ModelPart named : \"" << root_model_part_name
                    << "\" was not found either as root-ModelPart or as a flat name. The total input string was \""
                    << rFullModelPartName << "\"" << std::endl;
            exit(0);
        }
    } else { //it is a submodelpart with the full name provided
        auto search = mRootModelPartMap.find(root_model_part_name);
        if(search != mRootModelPartMap.end()) {
            ModelPart* p_model_part = (search->second).get();
            return p_model_part->GetSubModelPart(rFullModelPartName.substr(delim_pos + 1));
        } else {
            std::cerr << "root model part " << rFullModelPartName << " not found" << std::endl;
            exit(0);
        }

    }
}

bool Model::HasModelPart(const std::string& rFullModelPartName) const
{

    if( rFullModelPartName.empty() )
    {
        std::cerr<< "Attempting to find a "
        << "ModelPart with empty name (\"\")!" << std::endl;
        exit(0);
    } 

    const auto delim_pos = rFullModelPartName.find('.');
    const std::string& root_model_part_name = rFullModelPartName.substr(0, delim_pos);

    // token 0 is the root
    auto search = mRootModelPartMap.find(root_model_part_name);
    if(search != mRootModelPartMap.end()) {
        if (delim_pos == std::string::npos) {
            return true;
        } else {
            ModelPart* p_model_part = (search->second).get();
            return p_model_part->HasSubModelPart(rFullModelPartName.substr(delim_pos + 1));
        }
    } else {
        return false;
    }

}

std::vector<std::string> Model::GetModelPartNames() const
{
    std::vector<std::string> model_parts_names;

    // We fill the vector
    for (auto& mps : mRootModelPartMap) {
        const std::string& r_root_mp_name = mps.first;
        model_parts_names.push_back(r_root_mp_name);

        // First level of submodelparts
        auto& p_root_mp = mps.second;
        if (p_root_mp->NumberOfSubModelParts() > 0) {
            const std::vector<std::string>& sub_model_part_names = p_root_mp->GetSubModelPartNames();
            for (auto& r_sub_name : sub_model_part_names) {
                model_parts_names.push_back(r_root_mp_name + "." + r_sub_name);
            }

            // Second level of submodelparts
            for (auto& r_sub_mp : p_root_mp->SubModelParts()) {
                if (r_sub_mp.NumberOfSubModelParts() > 0) {
                    const std::string& r_sub_name = r_sub_mp.Name();
                    const std::vector<std::string>& sub_sub_model_part_names = r_sub_mp.GetSubModelPartNames();
                    for (auto& r_sub_sub_name : sub_sub_model_part_names) {
                        model_parts_names.push_back(r_root_mp_name + "." + r_sub_name + "." + r_sub_sub_name);
                    }
                }
            }
        }
    }

    return model_parts_names;
}

std::string Model::Info() const
{
    std::stringstream ss;
    for(auto it = mRootModelPartMap.begin(); it!=mRootModelPartMap.end(); it++)
    {
            ss<< *((it->second).get()) << std::endl << std::endl;
    }
    return ss.str();
}

ModelPart* Model::RecursiveSearchByName(const std::string& ModelPartName, ModelPart* pModelPart) const
{
    for(auto& part : pModelPart->SubModelParts())
    {
        if(part.Name() == ModelPartName)
            return &part;
        else
        {
            ModelPart* pmodel_part = RecursiveSearchByName(ModelPartName, &part);
            if(pmodel_part != nullptr)
                return pmodel_part;
        }
    }
    return nullptr;
}

/// Turn back information as a string.
std::string ModelPart::Info() const
{
    return "-" + mName + "- model part";
}

/// Print information about this object.

void Model::PrintInfo(std::ostream& rOStream) const
{
    rOStream << Info();
}

/// Print object's data.

void Model::PrintData(std::ostream& rOStream) const
{
}

void ModelPart::ErrorNonExistingSubModelPart(const std::string& rSubModelPartName) const
{
    std::stringstream err_msg;
    err_msg << "There is no sub model part with name \"" << rSubModelPartName
            << "\" in model part \"" << FullName() << "\"\n"
            << "The following sub model parts are available:";
    for (const auto& r_avail_smp_name : GetSubModelPartNames()) {
        err_msg << "\n\t" << "\""<<r_avail_smp_name << "\"";
    }
    std::cerr << err_msg.str() << std::endl;
    exit(0);
}
