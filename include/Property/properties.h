#ifndef _PROPERTIES_H_
#define _PROPERTIES_H_
#include "../define.h"
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
class Properties
{
    public:
        ///@name Define 
        ///@{ 
            LOTUS_POINTER_DEFINE(Properties)
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
            typedef std::unordered_map<std::size_t, Properties::Pointer>
                                                               SubPropertiesContainerType;                                                           
        ///}

        ///@name Lift Circle 
        ///@{
            //Constructor
            /// Default constructor.
            explicit Properties(IndexType NewId = 0)
            : ID(NewId)
            , mData()
            , mTables()
            , mSubPropertiesList()
            , mAccessors() {}

            /// Default of properties with subproperties
            explicit Properties(IndexType NewId, const SubPropertiesContainerType& SubPropertiesList)
            : ID(NewId)
            , mData()
            , mTables()
            , mSubPropertiesList(SubPropertiesList)
            , mAccessors() {}

            /// Copy constructor.
            Properties(const Properties& rOther)
            : ID(rOther.ID)
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
        /// @}

        /// @name Operations
        /// @{
            
        /// @}

        /// @name Access
        /// @{
            IndexType& Id()
            {
                return ID;
            }
            IndexType Id() const
            {
                return ID;
            }
            

        /// @}

        /// @name Inquiry
        /// @{
            template<class TVariableType>
            bool Has(TVariableType const& rThisVariable) const
            {
                return mData.Has(rThisVariable);
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
#endif