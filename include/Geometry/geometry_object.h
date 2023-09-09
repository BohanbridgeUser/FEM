#ifndef _GEOMETRY_OBJECT_H_
#define _GEOMETRY_OBJECT_H_

#include "../define.h"
#include "../Node/node.h"
#include "../Geometry/geometry.h"
#include "../Container/flags.h"
#include "../Container/lotus_flags.h"
class Geometry_Object : public Flags
{
    public:
        ///@name Define 
        ///@{
            LOTUS_POINTER_DEFINE(Geometry_Object)
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
            :FlagType()
            {
                numbers++;
                ID = numbers;
                mpGeometry = nullptr;
            }
            Geometry_Object(GeometryType* ThismpGeometry)
            :FlagType(),
            mpGeometry(ThismpGeometry)
            {
                ID = ++numbers;
            }
            Geometry_Object(IndexType NewID, NodesContainerType const& rThisNodes)
            :FlagType(),
            ID(NewID),
            mpGeometry(GeometryType::Pointer(new GeometryType(rThisNodes)))
            {
                numbers++;
            }
            Geometry_Object(IndexType NewID, GeometryType* ThismpGeometry)
            :FlagType(),
            mpGeometry(ThismpGeometry)
            {
                numbers++;
                ID = NewID;
            }
            Geometry_Object(ClassType& another)
            :FlagType(),
            mpGeometry(another.mpGeometry)
            {
                ID = ++numbers;
            }
            Geometry_Object(ClassType&& another)
            :FlagType(),
            mpGeometry(another.mpGeometry)
            {
                ID = ++numbers;
            }
            // Destructor 
            virtual ~Geometry_Object()
            {
                if (numbers > 0 ) --numbers;
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
            IndexType Id() const
            {
                return ID;
            }
            IndexType& Id() 
            {
                return ID;
            }
            template<typename TDataType>
            bool Has(Variable<TDataType> const& rVariable) const
            {
                return GetData().Has(rVariable);
            }
        /// @}
    protected:
        IndexType ID;
        static int numbers;
        GeometryType* mpGeometry;
};
#endif
