#ifndef _GEOMETRY_OBJECT_H_
#define _GEOMETRY_OBJECT_H_

#include "../define.h"
#include "../Node/node.h"
#include "../Geometry/geometry.h"
#include "../Container/flags.h"
#include "../Container/lotus_flags.h"
#include "../indexed_object.h"

class Geometry_Object : public Indexed_Object, public Flags
{
    public:
        ///@name Define 
        ///@{
            LOTUS_POINTER_DEFINE(Geometry_Object)
            typedef size_t
                                                               IndexType;
            typedef size_t
                                                                SizeType;                                        
            typedef Flags
                                                                FlagType;
            typedef Node                            
                                                                NodeType;
            typedef Geometry<NodeType> 
                                                            GeometryType;
            typedef Geometry_Object 
                                                               ClassType;
            typedef Data_Value_Container
                                                  DataValueContainerType;
            typedef GeometryType::PointsContainerType
                                                      NodesContainerType;
        ///@}

        ///@name Lift Circle 
        ///@{ 
            // Constructor
            Geometry_Object()
            :Indexed_Object(),
            FlagType()
            {
                mpGeometry = nullptr;
            }
            Geometry_Object(GeometryType::Pointer ThismpGeometry)
            :Indexed_Object(),
            FlagType(),
            mpGeometry(ThismpGeometry)
            {
            }
            Geometry_Object(IndexType NewID,
                            GeometryType::Pointer ThismpGeometry)
            :Indexed_Object(NewID),
            FlagType(),
            mpGeometry(ThismpGeometry)
            {
            }
            Geometry_Object(ClassType& another)
            :Indexed_Object(another),
            FlagType(another),
            mpGeometry(another.mpGeometry)
            {
            }
            Geometry_Object(ClassType&& another)
            :Indexed_Object(another),
            FlagType(),
            mpGeometry(another.mpGeometry)
            {
            }
            // Destructor 
            virtual ~Geometry_Object()
            {
            }
        ///@}

        ///@name Operations 
        ///@{
            void SetFlags(Flags const& rThisFlags)
            {
                Flags::operator=(rThisFlags);
            }
            void SetData(DataValueContainerType const& rThisData)
            {
                
            }
        ///@}

        /// @name Access
        /// @{
            GeometryType& GetGeometry()
            {
                return *mpGeometry;
            }
            GeometryType GetGeometry() const
            {
                return *mpGeometry;
            }
            DataValueContainerType& GetData()
            {
                return mpGeometry->GetData();
            }
            DataValueContainerType const& GetData() const
            {
                return mpGeometry->GetData();
            }
            Flags& GetFlag()
            {
                return *this;
            }
            Flags const& GetFlag() const
            {
                return *this;
            }
            template<typename TVariableType>
            typename TVariableType::Type& GetValue(TVariableType const& rVariable)
            {
                return GetData().GetValue(rVariable);
            }
            template<typename TVariableType>
            typename TVariableType::Type const& GetValue(TVariableType const& rVariable) const
            {
                return GetData().GetValue(rVariable);
            }
        /// @}

        /// @name Inquiry
        /// @{
            template<typename TDataType>
            bool Has(Variable<TDataType> const& rVariable) const
            {
                return GetData().Has(rVariable);
            }
        /// @}
    protected:
        GeometryType::Pointer mpGeometry;
};
#endif
