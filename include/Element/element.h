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
        LOTUS_SHARED_POINTER_DEFINE(Element)   
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
                                                     PropertiesType;
        typedef Dof 
                                                            DofType;
        typedef std::vector<Dof::Pointer> 
                                                     DofsVectorType;
        typedef std::vector<std::size_t> 
                                               EquationIdVectorType;
        typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> 
                                                         MatrixType;
        typedef Eigen::Matrix<double,Eigen::Dynamic,1> 
                                                         VectorType;
        ///@}

        ///@name Lift Circle 
        ///@{
            // Constructor 
            Element():Geometry_Object()
            {

            }
            Element(const IndexType& NewID,typename GeometryType::Pointer ThismpGeometry)
            :Geometry_Object(NewID,ThismpGeometry),
             mpProperties()
            {
                
            }
            Element(const IndexType& NewID,
                    typename GeometryType::Pointer ThismpGeometry, 
                    Properties::Pointer ThisProperties)
            :Geometry_Object(NewID,ThismpGeometry),
             mpProperties(ThisProperties)
            {
                
            }
            Element(const Element& another)
            :Geometry_Object(another.mpGeometry),
             mpProperties(another.mpProperties)
            {

            }
            Element(Element&& another)
            :Geometry_Object(another.mpGeometry),
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
            virtual Element::SharedPointer Create(IndexType NewId,
                                    NodesContainerType const& rNodes,
                                    Properties::Pointer pProperties) const
            {
                std::cerr << "Calling Base Class Create Method!\n";
                return std::make_shared<Element>(NewId,GetGeometry().Create(rNodes),pProperties);
            }
            virtual Element::SharedPointer Clone(IndexType NewId,
                                    NodesContainerType const& rNodes) const
            {
                std::cerr << "Calling Base Class Create Method!\n";
                return std::make_shared<Element>(NewId,GetGeometry().Create(rNodes),pGetProperties());
            }
             /**
             * @brief It creates a new element pointer
             * @param NewId the ID of the new element
             * @param pGeom the geometry to be employed
             * @param pProperties the properties assigned to the new element
             * @return a Pointer to the new element
             */
            virtual Pointer Create(IndexType NewId,
                                GeometryType::Pointer pGeom,
                                PropertiesType::Pointer pProperties) const
            {
                std::cerr << "Please implement the Second Create method in your derived Element" << Info() << std::endl;
                return std::make_shared<Element>(NewId, pGeom, pProperties);
            }


            /**
             * This method provides the place to perform checks on the completeness of the input
             * and the compatibility with the problem options as well as the contitutive laws selected
             * It is designed to be called only once (or anyway, not often) typically at the beginning
             * of the calculations, so to verify that nothing is missing from the input
             * or that no common error is found.
             * @param rCurrentProcessInfo
             * this method is: MANDATORY
             */
            virtual int Check(const Process_Info& rCurrentProcessInfo) const
            {
                if( this->Id() < 1 )
                {
                    std::cerr<< "Element found with Id " << this->Id() << std::endl;
                    exit(0);
                } 
                const double domain_size = this->GetGeometry().DomainSize();
                if( domain_size <= 0.0 )
                {
                    std::cerr << "Element " << this->Id() << " has non-positive size " << domain_size << std::endl;
                    exit(0);
                } 
                GetGeometry().Check();
                return 0;
            }
            
            virtual void Initialize(const Process_Info& rCurrentProcessInfo)
            {
                
            }

            virtual void InitializeSolutionStep(const Process_Info& rCurrentProcessInfo)
            {

            }

            virtual void FinalizeSolutionStep(const Process_Info& rCurrentProcessInfo)
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
        
            /**
             * ELEMENTS inherited from this class have to implement next
             * CalculateLocalSystem, CalculateLeftHandSide and CalculateRightHandSide methods
             * they can be managed internally with a private method to do the same calculations
             * only once: MANDATORY
             */
            /**
             * this is called during the assembling process in order
             * to calculate all elemental contributions to the global system
             * matrix and the right hand side
             * @param rLeftHandSideMatrix the elemental left hand side matrix
             * @param rRightHandSideVector the elemental right hand side
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void CalculateLocalSystem(MatrixType& rLeftHandSideMatrix,
                                              VectorType& rRightHandSideVector,
                                              const Process_Info& rCurrentProcessInfo)
            {
                if (rLeftHandSideMatrix.rows() != 0) 
                {
                    rLeftHandSideMatrix.resize(0, 0);
                }
                if (rRightHandSideVector.size() != 0) 
                {
                    rRightHandSideVector.resize(0);
                }
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
            Properties::Pointer& pGetProperties()
            {
                return mpProperties;
            }
            Properties::Pointer pGetProperties() const
            {
                return mpProperties;
            }
            /**
             * this determines the elemental equation ID vector for all elemental
             * DOFs
             * @param rResult the elemental equation ID vector
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void EquationIdVector(EquationIdVectorType& rResult,
                                          const Process_Info& rCurrentProcessInfo) const
            {
                if (rResult.size() != 0) {
                    rResult.resize(0);
                }
            }
        /// @}

        /// @name Inquiry
        /// @{

        /// @}
    protected:

    private:
        Properties::Pointer mpProperties;
};
#endif