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
            GeometryType& GetGeometry()
            {
                return *mpGeometry;
            }
            GeometryType GetGeometry() const
            {
                return *mpGeometry;
            }
        ///@}

        /// @name Access
        /// @{

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
        /// @}
    protected:
        IndexType ID;
        static int numbers;
        GeometryType* mpGeometry;
};
#endif
