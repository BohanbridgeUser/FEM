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
            explicit Element(IndexType NewId = 0)
            : BaseType(NewId)
            , mpProperties(nullptr)
            {
            }
            /**
             * Constructor using an array of nodes
             */
            Element(IndexType NewId, const NodesContainerType& ThisNodes)
            : BaseType(NewId,GeometryType::Pointer(new GeometryType(ThisNodes)))
            , mpProperties(nullptr)
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
            Element & operator=(Element const& rOther)
            {
                BaseType::operator=(rOther);
                mpProperties = rOther.mpProperties;
                return *this;
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
            /**
             * this is called for non-linear analysis at the beginning of the iteration process
             */
            virtual void InitializeNonLinearIteration(const Process_Info& rCurrentProcessInfo)
            {
            }

            virtual void FinalizeSolutionStep(const Process_Info& rCurrentProcessInfo)
            {

            }
            /**
             * this is called for non-linear analysis at the end of the iteration process
             */
            virtual void FinalizeNonLinearIteration(const Process_Info& rCurrentProcessInfo)
            {
            }
            /**
             * @brief This class is derived from Geometry_Object. It have methods follow:
             * GeometryType& GetGeometry() 
             * GeometryType GetGeometry() const
            */

            /**
             * is called to reset the constitutive law parameters and the material properties
             * the elemental variables will be changed and reset using this method
             */
            virtual void ResetConstitutiveLaw()
            {
            }


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
            /**
             * this is called during the assembling process in order
             * to calculate the elemental left hand side matrix only
             * @param rLeftHandSideMatrix the elemental left hand side matrix
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void CalculateLeftHandSide(MatrixType& rLeftHandSideMatrix,
                                            const Process_Info& rCurrentProcessInfo)
            {
                if (rLeftHandSideMatrix.rows() != 0) {
                    rLeftHandSideMatrix.resize(0, 0);
                }
            }
            /**
             * this is called during the assembling process in order
             * to calculate the elemental right hand side vector only
             * @param rRightHandSideVector the elemental right hand side vector
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void CalculateRightHandSide(VectorType& rRightHandSideVector,
                                                const Process_Info& rCurrentProcessInfo)
            {
                if (rRightHandSideVector.size() != 0) {
                    rRightHandSideVector.resize(0);
                }
            }

            /**
             * ELEMENTS inherited from this class must implement this methods
             * if they need to add dynamic element contributions
             * note: first derivatives means the velocities if the displacements are the dof of the analysis
             * note: time integration parameters must be set in the rCurrentProcessInfo before calling these methods
             * CalculateFirstDerivativesContributions,
             * CalculateFirstDerivativesLHS, CalculateFirstDerivativesRHS methods are : OPTIONAL
             */

            /**
             * this is called during the assembling process in order
             * to calculate the first derivatives contributions for the LHS and RHS
             * @param rLeftHandSideMatrix the elemental left hand side matrix
             * @param rRightHandSideVector the elemental right hand side
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void CalculateFirstDerivativesContributions(MatrixType& rLeftHandSideMatrix,
                                                                VectorType& rRightHandSideVector,
                                                                const Process_Info& rCurrentProcessInfo)
            {
                if (rLeftHandSideMatrix.rows() != 0) {
                    rLeftHandSideMatrix.resize(0, 0);
                }
                if (rRightHandSideVector.size() != 0) {
                    rRightHandSideVector.resize(0);
                }
            }
            /**
             * this is called during the assembling process in order
             * to calculate the elemental left hand side matrix for the first derivatives contributions
             * @param rLeftHandSideMatrix the elemental left hand side matrix
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void CalculateFirstDerivativesLHS(MatrixType& rLeftHandSideMatrix,
                                                    const Process_Info& rCurrentProcessInfo)
            {
                if (rLeftHandSideMatrix.rows() != 0) {
                    rLeftHandSideMatrix.resize(0, 0);
                }
            }
            /**
             * this is called during the assembling process in order
             * to calculate the elemental right hand side vector for the first derivatives contributions
             * @param rRightHandSideVector the elemental right hand side vector
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void CalculateFirstDerivativesRHS(VectorType& rRightHandSideVector,
                                                    const Process_Info& rCurrentProcessInfo)
            {
                if (rRightHandSideVector.size() != 0) {
                    rRightHandSideVector.resize(0);
                }
            }

            /**
             * this is called during the assembling process in order
             * to calculate the second derivative contributions for the LHS and RHS
             * @param rLeftHandSideMatrix the elemental left hand side matrix
             * @param rRightHandSideVector the elemental right hand side
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void CalculateSecondDerivativesContributions(MatrixType& rLeftHandSideMatrix,
                                                                VectorType& rRightHandSideVector,
                                                                const Process_Info& rCurrentProcessInfo)
            {
                if (rLeftHandSideMatrix.rows() != 0) {
                    rLeftHandSideMatrix.resize(0,0);
                }
                if (rRightHandSideVector.size() != 0) {
                    rRightHandSideVector.resize(0);
                }
            }
            /**
             * this is called during the assembling process in order
             * to calculate the elemental left hand side matrix for the second derivatives contributions
             * @param rLeftHandSideMatrix the elemental left hand side matrix
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void CalculateSecondDerivativesLHS(MatrixType& rLeftHandSideMatrix,
                                                    const Process_Info& rCurrentProcessInfo)
            {
                if (rLeftHandSideMatrix.rows() != 0) {
                    rLeftHandSideMatrix.resize(0,0);
                }
            }
            /**
             * this is called during the assembling process in order
             * to calculate the elemental right hand side vector for the second derivatives contributions
             * @param rRightHandSideVector the elemental right hand side vector
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void CalculateSecondDerivativesRHS(VectorType& rRightHandSideVector,
                                                    const Process_Info& rCurrentProcessInfo)
            {
                if (rRightHandSideVector.size() != 0) {
                    rRightHandSideVector.resize(0);
                }
            }
            /**
             * ELEMENTS inherited from this class must implement this methods
             * if they need to add dynamic element contributions
             * CalculateMassMatrix, CalculateDampingMatrix and CalculateLumpedMassVector methods are: OPTIONAL
             */
            /**
             * this is called during the assembling process in order
             * to calculate the elemental mass matrix
             * @param rMassMatrix the elemental mass matrix
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void CalculateMassMatrix(MatrixType& rMassMatrix, const Process_Info& rCurrentProcessInfo)
            {
                if (rMassMatrix.rows() != 0) {
                    rMassMatrix.resize(0,0);
                }
            }
            /**
             * this is called during the assembling process in order
             * to calculate the elemental damping matrix
             * @param rDampingMatrix the elemental damping matrix
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void CalculateDampingMatrix(MatrixType& rDampingMatrix, const Process_Info& rCurrentProcessInfo)
            {
                if (rDampingMatrix.rows() != 0) {
                    rDampingMatrix.resize(0,0);
                }
            }
            /**
             * this is called during the initialize of the builder
             * to calculate the lumped mass vector
             * @param rLumpedMassVector the elemental lumped mass vector
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void CalculateLumpedMassVector(
                VectorType& rLumpedMassVector,
                const Process_Info& rCurrentProcessInfo) const
                {
                    std::cerr << "Calling the CalculateLumpedMassVector() method of the base element. The method must be implemented in the derived element.";
                    exit(0);
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

        /// @name Input And Output
        /// @{
            /// Turn back information as a string.

            std::string Info() const override
            {
                std::stringstream buffer;
                buffer << "Element #" << Id();
                return buffer.str();
            }

            /// Print information about this object.

            void PrintInfo(std::ostream& rOStream) const override
            {
                rOStream << "Element #" << Id();
            }

            /// Print object's data.

            void PrintData(std::ostream& rOStream) const override
            {
                GetGeometry().PrintData(rOStream);
            }

        /// @}
    protected:

    private:
        Properties::Pointer mpProperties;
};
#endif