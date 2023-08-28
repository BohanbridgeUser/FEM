#ifndef _ELEMENT_H_
#define _ELEMENT_H_
#include "../define.h"
#include "../Geometry/geometry_object.h"
#include "../Geometry/geometry_data.h"
#include "../Property/properties.h"
#include "../Constitutive_Law/Constitutive_law.h"
#include "../process_info.h"
#include "../Container/flags.h"
class Element:public Geometry_Object
{
    public:
        ///@name Define 
        ///@{ 
        LOTUS_POINTER_DEFINE(Element)   
        typedef Element 
                                                        ElementType;
        typedef Geometry_Object 
                                                           BaseType;
        typedef Node 
                                                           NodeType;
        typedef Geometry<NodeType> 
                                                       GeometryType;
        typedef Geometry_Data::IntegrationMethod 
                                                  IntegrationMethod;
        typedef Geometry_Data 
                                                   GeometryDataType;
        typedef Properties
                                                     PropertiesTpye;
        typedef Dof 
                                                            DofType;
        typedef std::vector<Dof::Pointer> 
                                                     DofsVectorType;
        typedef std::vector<std::size_t> 
                                               EquationIdVectorType;
        ///@}

        ///@name Lift Circle 
        ///@{
            // Constructor 
            Element():Geometry_Object()
            {

            }
            Element(const int& NewID,GeometryType* ThismpGeometry)
            :Geometry_Object(ThismpGeometry),
             Index(NewID),
             mpProperties()
            {
                
            }
            Element(const int& NewID, GeometryType* ThismpGeometry, Properties& ThisProperties)
            :Geometry_Object(ThismpGeometry),
             Index(NewID),
             mpProperties(&ThisProperties)
            {
                
            }
            Element(Element& another)
            :Geometry_Object(another.mpGeometry),
             Index(another.Index),
             mpProperties(another.mpProperties)
            {

            }
            Element(Element&& another)
            :Geometry_Object(&(another.GetGeometry())),
             Index(another.Index),
             mpProperties(another.mpProperties)
            {
                another.mpProperties = nullptr;
            }
            // Destructor
            virtual ~Element()
            {
                
            }
        ///@}

        /// @name Operators
        /// @{
            friend std::ostream& operator<<(std::ostream& os, const Element& another)
            {
                return os << *another.mpGeometry;
            }
        /// @}

        ///@name Operations 
        ///@{
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

            virtual void GetDofList(DofsVectorType& rElementalDofList, const Process_Info& rCurrentProcessInfo) const
            {

            }
        ///@}

        /// @name Access
        /// @{
            Properties& GetProperties()
            {
                return *mpProperties;
            }
            Properties GetProperties()const
            {
                return *mpProperties;
            }
        /// @}

        /// @name Inquiry
        /// @{
            int ElementID()const
            {
                return Index;
            }
            int& ElementID()
            {
                return Index;
            }
        /// @}
    protected:
        int Index;
    private:
        Properties* mpProperties;
};
#endif