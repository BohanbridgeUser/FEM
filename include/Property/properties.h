#ifndef _PROPERTIES_H_
#define _PROPERTIES_H_
#include "../define.h"
#include "../indexed_object.h"
#include "../Geometry/geometry.h"
#include "../Node/node.h"
#include "../process_info.h"
#include "../Container/data_value_container.h"
#include "table.h"
#include "accessor.h"
#include "../Container/pointer_vector_set.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
class Properties : public Indexed_Object
{
    public:
        ///@name Define 
        ///@{ 
            LOTUS_SHARED_POINTER_DEFINE(Properties)
            typedef size_t  
                                                                                IndexType;
            typedef size_t
                                                                                 SizeType;
            typedef size_t
                                                                                  KeyType;
            typedef Geometry<Node >
                                                                             GeometryType;
            typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>
                                                                                   Matrix;
            typedef Eigen::Matrix<double, Eigen::Dynamic, 1>
                                                                                   Vector;
            typedef Data_Value_Container
                                                                            ContainerType;
            typedef Table<double>
                                                                                TableType;
            typedef Accessor::UniquePointer
                                                                      AccessorPointerType;
            typedef std::unordered_map<KeyType, AccessorPointerType>
                                                                   AccessorsContainerType;
            typedef std::unordered_map<std::size_t, TableType>
                                                                      TablesContainerType;
            typedef PointerVectorSet<Properties, Indexed_Object>
                                                               SubPropertiesContainerType;                                                           
        ///}

        ///@name Lift Circle 
        ///@{
            //Constructor
            /// Default constructor.
            explicit Properties(IndexType NewId = 0)
            : Indexed_Object(NewId),
             mData(),
             mTables(),
             mSubPropertiesList(),
             mAccessors() {}

            /// Default of properties with subproperties
            explicit Properties(IndexType NewId, const SubPropertiesContainerType& SubPropertiesList)
            : Indexed_Object(NewId)
            , mData()
            , mTables()
            , mSubPropertiesList(SubPropertiesList)
            , mAccessors() {}

            /// Copy constructor.
            Properties(const Properties& rOther)
            : Indexed_Object(rOther)
            , mData(rOther.mData)
            , mTables(rOther.mTables)
            , mSubPropertiesList(rOther.mSubPropertiesList)
            {
                for (auto& r_item : rOther.mAccessors) {
                    const auto key = r_item.first;
                    const auto& rp_accessor = r_item.second;
                    mAccessors.emplace(key, rp_accessor->Clone());
                }
            }

            /// Destructor.
            virtual ~Properties() {}
        ///}

        /// @name Operators
        /// @{
            Properties& operator=(const Properties& rOther)
            {
                ID = rOther.ID;
                mData = rOther.mData;
                mTables = rOther.mTables;
                mSubPropertiesList = rOther.mSubPropertiesList;
                for (auto& r_item : rOther.mAccessors) {
                    const auto key = r_item.first;
                    const auto& rp_accessor = r_item.second;
                    mAccessors.emplace(key, rp_accessor->Clone());
                }
                return *this;
            }
            
            template<class TVariableType>
            typename TVariableType::Type& operator[](const TVariableType& rV)
            {
                return GetValue(rV);
            }

            template<class TVariableType>
            typename TVariableType::Type const& operator[](const TVariableType& rV) const
            {
                return GetValue(rV);
            }
            
            template<class TVariableType>
            typename TVariableType::Type& operator()(const TVariableType& rV)
            {
                return GetValue(rV);
            }

            template<class TVariableType>
            typename TVariableType::Type const& operator()(const TVariableType& rV) const
            {
                return GetValue(rV);
            }
            template<class TVariableType>
            typename TVariableType::Type& operator()(const TVariableType& rV, Node& rThisNode)
            {
                return GetValue(rV, rThisNode);
            }
            template<class TVariableType>
            typename TVariableType::Type const& operator()(const TVariableType& rV, Node const& rThisNode) const
            {
                return GetValue(rV, rThisNode);
            }
            template<class TVariableType>
            typename TVariableType::Type& operator()(const TVariableType& rV,
                                                     Node& rThisNode,
                                                     IndexType SolutionStepIndex)
            {
                return GetValue(rV, rThisNode, SolutionStepIndex);
            }
            template<class TVariableType>
            typename TVariableType::Type const& operator()(const TVariableType& rV,
                                                           Node const& rThisNode, 
                                                           IndexType SolutionStepIndex) const
            {
                return GetValue(rV, rThisNode, SolutionStepIndex);
            }
            template<class TVariableType>
            typename TVariableType::Type& operator()(const TVariableType& rV,
                                                     Node& rThisNode, 
                                                     Process_Info const& rCurrentProcessInfo)
            {
                return GetValue(rV, rThisNode, rCurrentProcessInfo.GetSolutionStepIndex());
            }
            template<class TVariableType>
            typename TVariableType::Type const& operator()(const TVariableType& rV, 
                                                           Node const& rThisNode, 
                                                           Process_Info const& rCurrentProcessInfo) const
            {
                return GetValue(rV, rThisNode, rCurrentProcessInfo.GetSolutionStepIndex());
            }

        /// @}

        /// @name Operations
        /// @{
            template<class TVariableType>
            void Erase(const TVariableType& rV)
            {
                mData.Erase(rV);
            }

            template<class TVariableType>
            void SetValue(TVariableType const& rV, typename TVariableType::Type const& rValue)
            {
                mData.SetValue(rV, rValue);
            }

            /**
             * @brief Set the Accessor object
             * This method sets a variable-accessor pair in current properties accessor container
             * @tparam TVariableType The variable type
             * @param rVariable Variable to which the accessor will refer to
             * @param pAccessor Pointer to the accessor instance
             */
            template <class TVariableType>
            void SetAccessor(const TVariableType& rVariable, AccessorPointerType pAccessor)
            {
                mAccessors.emplace(rVariable.Key(), std::move(pAccessor));
            }

            template<class TXVariableType, class TYVariableType>
            void SetTable(const TXVariableType& XVariable, const TYVariableType& YVariable, TableType const& rThisTable)
            {
                mTables[Key(XVariable.Key(), YVariable.Key())] = rThisTable;
            }

            /**
             * @brief This method insert a new property into the list of subproperties
             * @param pNewSubProperty The new property to be added
             */
            void AddSubProperties(Properties::Pointer pNewSubProperty)
            {
                if(this->HasSubProperties(pNewSubProperty->Id())) std::cout << "SubProperties with ID: " << pNewSubProperty->Id() << " already defined" << std::endl;
                mSubPropertiesList.insert(mSubPropertiesList.begin(), pNewSubProperty);
            }

            /**
             * @brief This method set the whole list of subproperties
             * @param rSubPropertiesList The list of subproperties
             */
            void SetSubProperties(SubPropertiesContainerType& rSubPropertiesList)
            {
                mSubPropertiesList = rSubPropertiesList;
            }

        /// @}

        /// @name Access
        /// @{
             /**
             * @brief This method returns the whole data container
             * @return Data container
             */
            ContainerType& Data()
            {
                return mData;
            }
            /**
             * @brief This method returns the whole data container (constant)
             * @return Data container
             */
            ContainerType const& Data() const
            {
                return mData;
            }
            /**
             * @brief This method returns the tables
             * @return The whole lis of tables
             */
            TablesContainerType& Tables()
            {
                return mTables;
            }
            /**
             * @brief This method returns the tables (constant)
             * @return The whole lis of tables
             */
            TablesContainerType const& Tables() const
            {
                return mTables;
            }


            template<class TVariableType>
            typename TVariableType::Type& GetValue(const TVariableType& rVariable)
            {
                return mData.GetValue(rVariable);
            }

            template<class TVariableType>
            typename TVariableType::Type const& GetValue(const TVariableType& rVariable) const
            {

                return mData.GetValue(rVariable);
            }

            template<class TVariableType>
            typename TVariableType::Type& GetValue(const TVariableType& rVariable, Node& rThisNode)
            {
                if (mData.Has(rVariable))
                    return mData.GetValue(rVariable);
                return rThisNode.GetValue(rVariable);
            }

            template<class TVariableType>
            typename TVariableType::Type const& GetValue(const TVariableType& rVariable, Node const& rThisNode) const
            {
                if (mData.Has(rVariable))
                    return mData.GetValue(rVariable);
                return rThisNode.GetValue(rVariable);
            }

            template<class TVariableType>
            typename TVariableType::Type& GetValue(const TVariableType& rVariable, Node& rThisNode, IndexType SolutionStepIndex)
            {
                if (mData.Has(rVariable))
                    return mData.GetValue(rVariable);
                return rThisNode.GetValue(rVariable, SolutionStepIndex);
            }

            template<class TVariableType>
            typename TVariableType::Type const& GetValue(const TVariableType& rVariable, Node const& rThisNode, IndexType SolutionStepIndex) const
            {
                if (mData.Has(rVariable))
                    return mData.GetValue(rVariable);
                return rThisNode.GetValue(rVariable, SolutionStepIndex);
            }

            /*
            Custom GetValue in which we check the Accessor
            */
            template<class TVariableType>
            typename TVariableType::Type GetValue(const TVariableType& rVariable, const GeometryType& rGeometry, const Vector& rShapeFunctionVector, const Process_Info& rProcessInfo) const
            {
                auto it_value = mAccessors.find(rVariable.Key());
                if (it_value != mAccessors.end()) {
                    return (it_value->second)->GetValue(rVariable, *this, rGeometry, rShapeFunctionVector, rProcessInfo);
                } else {
                    return mData.GetValue(rVariable);
                }
            }
        
            /**
             * @brief Get the Accessor object
             * If exists, this method returns a pointer to the requested variable accessor
             * If doesn't exist, the method throws an error
             * @tparam TVariableType The variable type
             * @param rVariable Variable to which the accessor refers to
             * @return AccessorPointerType& Pointer to the requested variable accessor
             */
            template <class TVariableType>
            Accessor& GetAccessor(const TVariableType& rVariable)
            {
                auto it_value = mAccessors.find(rVariable.Key());
                if(it_value == mAccessors.end()){
                    std::cerr << "Trying to retrieve inexisting accessor for '" << rVariable.Name() << "' in properties " << Id() << "." << std::endl;
                    exit(0);
                } 
                return *(it_value->second);
            }

            /**
             * @brief Get the Accessor object
             * If exists, this method returns a pointer to the requested variable accessor
             * If doesn't exist, the method throws an error
             * @tparam TVariableType The variable type
             * @param rVariable Variable to which the accessor refers to
             * @return AccessorPointerType& Pointer to the requested variable accessor
             */
            template <class TVariableType>
            Accessor& GetAccessor(const TVariableType& rVariable) const
            {
                const auto it_value = mAccessors.find(rVariable.Key());
                if(it_value == mAccessors.end()){
                    std::cerr << "Trying to retrieve inexisting accessor for '" << rVariable.Name() << "' in properties " << Id() << "." << std::endl;
                    exit(0);
                } 
                return *(it_value->second);
            }

            /**
             * @brief Get the Accessor object
             * If exists, this method returns a pointer to the requested variable accessor
             * If doesn't exist, the method throws an error
             * @tparam TVariableType The variable type
             * @param rVariable Variable to which the accessor refers to
             * @return AccessorPointerType& Pointer to the requested variable accessor
             */
            template <class TVariableType>
            AccessorPointerType& pGetAccessor(const TVariableType& rVariable)
            {
                const auto it_value = mAccessors.find(rVariable.Key());
                if(it_value == mAccessors.end()){
                    std::cerr << "Trying to retrieve inexisting accessor for '" << rVariable.Name() << "' in properties " << Id() << "." << std::endl;
                    exit(0);
                } 
                return it_value->second;
            }
            
            template<class TXVariableType, class TYVariableType>
            TableType& GetTable(const TXVariableType& XVariable, const TYVariableType& YVariable)
            {
                return mTables[Key(XVariable.Key(), YVariable.Key())];
            }

            template<class TXVariableType, class TYVariableType>
            TableType const& GetTable(const TXVariableType& XVariable, const TYVariableType& YVariable) const
            {
                return mTables.at(Key(XVariable.Key(), YVariable.Key()));
            }

            /**
             * @brief This method gets the subproperty from the index corresponding to the property id
             * @param SubPropertyIndex The index of the subproperty to be get
             * @return The pointer to the subproperty of interest
             */
            Properties::Pointer pGetSubProperties(const IndexType SubPropertyIndex)
            {
                // Looking into the database
                auto property_iterator = mSubPropertiesList.find(SubPropertyIndex);
                if (property_iterator != mSubPropertiesList.end()) {
                    return *(property_iterator.base());
                } else {
                    std::cerr << "Subproperty ID: " << SubPropertyIndex << " is not defined on the current Properties ID: " << this->Id() << " creating a new one with ID: " << SubPropertyIndex << std::endl;
                    return nullptr;
                }
            }

            /**
             * @brief This method gets the subproperty from the index corresponding to the property id (constant version)
             * @param SubPropertyIndex The index of the subproperty to be get
             * @return The pointer to the subproperty of interest
             */
            const Properties::Pointer pGetSubProperties(const IndexType SubPropertyIndex) const
            {
                // Looking into the database
                auto property_iterator = mSubPropertiesList.find(SubPropertyIndex);
                if (property_iterator != mSubPropertiesList.end()) {
                    return *(property_iterator.base());
                } else {
                    std::cerr << "Subproperty ID: " << SubPropertyIndex << " is not defined on the current Properties ID: " << this->Id() << " creating a new one with ID: " << SubPropertyIndex << std::endl;
                    return nullptr;
                }
            }

            /**
             * @brief This method gets the subproperty from the index corresponding to the property id
             * @param SubPropertyIndex The index of the subproperty to be get
             * @return The reference to the subproperty of interest
             */
            Properties& GetSubProperties(const IndexType SubPropertyIndex)
            {
                // Looking into the database
                auto property_iterator = mSubPropertiesList.find(SubPropertyIndex);
                if (property_iterator != mSubPropertiesList.end()) {
                    return *(property_iterator);
                } else {
                    std::cerr << "Subproperty ID: " << SubPropertyIndex << " is not defined on the current Properties ID: " << this->Id() << " creating a new one with ID: " << SubPropertyIndex << std::endl;
                    return *this;
                }
            }

            /**
             * @brief This method gets the subproperty from the index corresponding to the property id (constant version)
             * @param SubPropertyIndex The index of the subproperty to be get
             * @return The reference to the subproperty of interest
             */
            const Properties& GetSubProperties(const IndexType SubPropertyIndex) const
            {
                // Looking into the database
                if (mSubPropertiesList.find(SubPropertyIndex) != mSubPropertiesList.end()) {
                    return *(mSubPropertiesList.find(SubPropertyIndex));
                } else {
                    std::cerr << "Subproperty ID: " << SubPropertyIndex << " is not defined on the current Properties ID: " << this->Id() << std::endl;
                    exit(0);
                }
            }

            /**
             * @brief This method returns the whole list of subproperties
             * @return The whole lis of subproperties
             */
            SubPropertiesContainerType& GetSubProperties()
            {
                return mSubPropertiesList;
            }

            /**
             * @brief This method returns the whole list of subproperties
             * @return The whole lis of subproperties
             */
            SubPropertiesContainerType const& GetSubProperties() const
            {
                return mSubPropertiesList;
            }


        /// @}

        /// @name Inquiry
        /// @{
            template<class TVariableType>
            bool Has(TVariableType const& rThisVariable) const
            {
                return mData.Has(rThisVariable);
            }
            /**
             * @brief Check if current properties have an accessor
             * This method checks if current properties have an accessor for the requested variable
             * @tparam TVariableType The variable type
             * @param rVariable Variable to which we are checking if an accessor exists
             * @return true If there is accessor for the requested variable
             * @return false If there is no accessor for the requested variable
             */
            template <class TVariableType>
            bool HasAccessor(const TVariableType& rVariable) const
            {
                return (mAccessors.find(rVariable.Key()) == mAccessors.end()) ? false : true;
            }
            template<class TXVariableType, class TYVariableType>
            bool HasTable(const TXVariableType& XVariable, const TYVariableType& YVariable) const
            {
                return (mTables.find(Key(XVariable.Key(), YVariable.Key())) != mTables.end());
            }
            bool HasTables() const
            {
                return !mTables.empty();
            }
            bool HasVariables() const
            {
                return !mData.IsEmpty();
            }
            bool IsEmpty() const
            {
                return !( HasVariables() || HasTables() );
            }

            int64_t Key(std::size_t XKey, std::size_t YKey) const
            {
                int64_t result_key = XKey;
                result_key = result_key << 32;
                result_key |= YKey; // I know that the key is less than 2^32 so I don't need zeroing the upper part
                return result_key;
            }
            /**
             * @brief This method returns the number of subproperties
             * @return The current number of subproperties
             */
            std::size_t NumberOfSubproperties() const
            {
                return mSubPropertiesList.size();
            }
            /**
             * @brief This method checks if the subproperty exists from the index corresponding to the property id
             * @param SubPropertyIndex The index of the subproperty to be get
             * @return True if there is such subproperty, false otherwise
             */
            bool HasSubProperties(const IndexType SubPropertyIndex) const
            {
                return mSubPropertiesList.find(SubPropertyIndex) != mSubPropertiesList.end();
            }
        /// @}

        /// @name Input And Output
        /// @{
            /// Turn back information as a string.
            std::string Info() const 
            {
                return "Properties";
            }

            /// Print information about this object.
            void PrintInfo(std::ostream& rOStream) const 
            {
                rOStream <<  "Properties";
            }

            /// Print object's data.
            void PrintData(std::ostream& rOStream) const
            {
                // Id
                rOStream << "Id : " << this->Id() << "\n";

                // Data
                mData.PrintData(rOStream);

                // Tables
                if (mTables.size() > 0) {
                    // Print the tables
                    rOStream << "This properties contains " << mTables.size() << " tables\n";
                    for (auto& r_table : mTables) {
                        rOStream << "Table key: " << r_table.first << "\n";
                        String_Utilities::PrintDataWithIdentation(rOStream, r_table.second);
                    }
                }

                // Subproperties
                if (mSubPropertiesList.size() > 0) {
                    // Print the subproperties
                    rOStream << "\nThis properties contains " << mSubPropertiesList.size() << " subproperties\n";
                    for (auto& r_subprop : mSubPropertiesList) {
                        String_Utilities::PrintDataWithIdentation(rOStream, r_subprop);
                    }
                }

                // Accessors
                if (mAccessors.size() > 0) {
                    // Print the accessors
                    rOStream << "\nThis properties contains " << mAccessors.size() << " accessors\n";
                    for (auto& r_entry : mAccessors) {
                        rOStream << "Accessor for variable key: " << r_entry.first << "\n";
                        String_Utilities::PrintDataWithIdentation(rOStream, *r_entry.second);
                    }
                }
            }
        /// @}
    private:
        /// @name Private Member
        /// @{
            IndexType ID;

            ContainerType mData;                        /// The data stored on the properties

            TablesContainerType mTables;                /// The tables contained on the properties

            SubPropertiesContainerType mSubPropertiesList; /// The vector containing the list of subproperties

            AccessorsContainerType mAccessors = {}; /// The map containing the variable and corresponding accessor pairs
        /// @}
        
};
inline std::ostream& operator << (std::ostream& rOStream,
                                  const Properties& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);
    return rOStream;
}
#endif