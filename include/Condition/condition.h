#ifndef _CONDITION_H_
#define _CONDITION_H_
#include "../define.h"
#include "../Geometry/geometry.h"
#include "../Geometry/geometry_data.h"
#include "../Geometry/geometry_object.h"
#include "../Property/properties.h"
#include "../process_info.h"
class Condition : public Geometry_Object
{
    public: 
        ///@name Define
        ///@{
            LOTUS_POINTER_DEFINE(Condition)
            typedef Condition ConditionType;
            typedef Geometry_Object BaseType;
            typedef Node NodeType;
            typedef Geometry<NodeType> GeometryType;
            typedef Geometry_Data::IntegrationMethod IntegrationMethod;
            typedef Geometry_Data GeometryDataType; 
        ///@}

        /// @name Life Circle
        /// @{
            // Constructor
            Condition():Geometry_Object()
            {
            
            }
            Condition(GeometryType* mpThisGeometry)
            :Geometry_Object(mpThisGeometry),
            mpProperties()
            {
            
            }
            Condition(GeometryType* mpThisGeometry, Properties& ThisProperties)
            :Geometry_Object(mpThisGeometry),
            mpProperties(&ThisProperties)
            {
            
            }
            Condition(const Condition& another)
            :Geometry_Object(another.mpGeometry),
            mpProperties(another.mpProperties)
            {

            }
            Condition(Condition&& another)
            :Geometry_Object(&(another.GetGeometry())),
            mpProperties(&(another.GetProperties()))
            {

            }
            // Destructor
            ~Condition()
            {
            
            }
        /// @}

        /// @name Operations
        /// @{
            Properties& GetProperties()
            {
                return *mpProperties;
            }
            const Properties& GetProperties()const
            {
                return *mpProperties;
            }
            
            virtual int Check()
            {
                return 0;
            }

            virtual void Initialize(Process_Info& rCurrentProcessInfo)
            {
                
            }

            virtual void InitializeSolutionStep(Process_Info& rCurrentProcessInfo)
            {

            }

            virtual void FinalizeSolutionStep(Process_Info& rCurrentProcessInfo)
            {

            }
            /**
             * @brief This class is derived from Geometry_Object. It have methods follow:
             * GeometryType& GetGeometry() 
             * GeometryType GetGeometry() const
            */
        /// @}
    private:
        Properties* mpProperties;
};

#endif