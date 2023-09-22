#ifndef _CONDITION_H_
#define _CONDITION_H_
#include "../define.h"
#include "../Container/lotus_flags.h"
#include "../Variable/variables.h"
#include "../Geometry/geometry.h"
#include "../Geometry/geometry_data.h"
#include "../Geometry/geometry_object.h"
#include "../Property/properties.h"
#include "../Node/dof.h"
#include "../process_info.h"
#include "../Utility/smart_pointer.h"
#include "../Utility/math_utility.h"

class Condition : public Geometry_Object
{
    public: 
        ///@name Define
        ///@{
            LOTUS_SHARED_POINTER_DEFINE(Condition)
            typedef Condition                               
                                                                     ConditionType;
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
            typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                                        MatrixType;
            typedef Eigen::Matrix<double,Eigen::Dynamic,1>
                                                                        VectorType;
            typedef Properties
                                                                    PropertiesType;
            typedef Node::DofPointersContainerType
                                                                    DofsVectorType;
            typedef std::vector<size_t>
                                                              EquationIdVectorType;
            typedef GeometryType::PointsContainerType
                                                                NodesContainerType;
        ///@}

        /// @name Life Circle
        /// @{
            // Constructor
            /**
             * Constructor.
             */
            explicit Condition(IndexType NewId = 0)
            : BaseType(NewId)
            , mpProperties(nullptr)
            {
            }
            /**
             * Constructor using an array of nodes
             */
            Condition(IndexType NewId, const NodesContainerType& ThisNodes)
                : BaseType(NewId,GeometryType::Pointer(new GeometryType(ThisNodes)))
                , mpProperties(nullptr)
            {
            }
            Condition(GeometryType::Pointer mpThisGeometry)
            :Geometry_Object(mpThisGeometry),
             mpProperties()
            {
            
            }
            Condition(GeometryType::Pointer mpThisGeometry, Properties::Pointer ThisProperties)
            :Geometry_Object(mpThisGeometry),
             mpProperties(ThisProperties)
            {
            
            }
            Condition(IndexType NewID,
                      NodesContainerType const& rThisNodes,
                      Properties::Pointer pThisProPerties)
            :Geometry_Object(NewID,GeometryType::Pointer(new GeometryType(rThisNodes))),
            mpProperties(pThisProPerties)
            {

            }
            Condition(IndexType NewID,
                      GeometryType::Pointer mpThisGeometry)
            :Geometry_Object(NewID,mpThisGeometry)
            {

            }
            Condition(IndexType NewID,
                      GeometryType::Pointer mpThisGeometry,
                      Properties::Pointer ThisProperties)
            :Geometry_Object(NewID,mpThisGeometry),
             mpProperties(ThisProperties)
            {
            
            }
            Condition(const Condition& another)
            :Geometry_Object(another.mpGeometry),
             mpProperties(another.mpProperties)
            {

            }
            Condition(Condition&& another)
            :Geometry_Object(another.mpGeometry),
             mpProperties(another.mpProperties)
            {

            }
            Condition(Condition* another)
            :Geometry_Object(another->mpGeometry),
             mpProperties(another->mpProperties)
            {

            }
            // Destructor
            virtual ~Condition()
            {
            
            }
        /// @}

        /// @name Operators
        /// @{
            Condition & operator=(Condition const& rOther)
            {
                BaseType::operator=(rOther);
                mpProperties = rOther.mpProperties;

                return *this;
            }

        /// @}


        /// @name Operations
        /// @{
            virtual Condition::SharedPointer Create(IndexType NewId,
                                                    NodesContainerType const& ThisNodes,
                                                    PropertiesType::Pointer pProperties ) const
            {
                return make_shared<Condition>(new Condition(NewId, GetGeometry().Create(ThisNodes), pProperties));
            }
            /**
             * @brief It creates a new condition pointer
             * @param NewId the ID of the new condition
             * @param pGeom the geometry to be employed
             * @param pProperties the properties assigned to the new condition
             * @return a Pointer to the new condition
             */
            virtual Pointer Create(IndexType NewId,
                                GeometryType::Pointer pGeom,
                                PropertiesType::Pointer pProperties) const
            {
                std::cerr << "Please implement the Second Create method in your derived Condition" << Info() << std::endl;
                exit(0);
                return std::make_shared<Condition>(NewId, pGeom, pProperties);
            }

            virtual Condition::SharedPointer Clone(IndexType NewId,
                                                   NodesContainerType const& ThisNodes ) const
            {
                Condition NewCondition(NewId,GetGeometry().Create(ThisNodes), pGetProperties());
                NewCondition.SetData(this->GetData());
                NewCondition.SetFlags(this->GetFlag());
                return make_shared<Condition>(NewCondition);
            }

            Properties& GetProperties()
            {
                return *mpProperties;
            }
            Properties const& GetProperties()const
            {
                return *mpProperties;
            }
            const PropertiesType::Pointer pGetProperties()const
            {
                return mpProperties;
            }
            virtual int Check()
            {
                return 0;
            }

            virtual void Initialize(const Process_Info& rCurrentProcessInfo)
            {
                
            }

            virtual void InitializeSolutionStep(const Process_Info& rCurrentProcessInfo)
            {

            }
            virtual void InitializeNonLinearIteration(const Process_Info& rCurrentProcessInfo)
            {
            }
            virtual void FinalizeSolutionStep(const Process_Info& rCurrentProcessInfo)
            {

            }
            
            /**
             * this is called during the assembling process in order
             * to calculate all condition contributions to the global system
             * matrix and the right hand side
             * @param rLeftHandSideMatrix the condition left hand side matrix
             * @param rRightHandSideVector the condition right hand side
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void CalculateLocalSystem(MatrixType& rLeftHandSideMatrix,
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
             * to calculate the condition left hand side matrix only
             * @param rLeftHandSideMatrix the condition left hand side matrix
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
             * to calculate the condition right hand side vector only
             * @param rRightHandSideVector the condition right hand side vector
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
             * CONDITIONS inherited from this class must implement this methods
             * if they need to add dynamic condition contributions
             * note: first derivatives means the velocities if the displacements are the dof of the analysis
             * note: time integration parameters must be set in the rCurrentProcessInfo before calling these methods
             * CalculateFirstDerivativesContributions,
             * CalculateFirstDerivativesLHS, CalculateFirstDerivativesRHS methods are : OPTIONAL
             */

            /**
             * this is called during the assembling process in order
             * to calculate the first derivatives contributions for the LHS and RHS
             * @param rLeftHandSideMatrix the condition left hand side matrix
             * @param rRightHandSideVector the condition right hand side
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
             * to calculate the condition left hand side matrix for the first derivatives contributions
             * @param rLeftHandSideMatrix the condition left hand side matrix
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
             * to calculate the condition right hand side vector for the first derivatives contributions
             * @param rRightHandSideVector the condition right hand side vector
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
             * @param rLeftHandSideMatrix the condition left hand side matrix
             * @param rRightHandSideVector the condition right hand side
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void CalculateSecondDerivativesContributions(MatrixType& rLeftHandSideMatrix,
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
             * to calculate the condition left hand side matrix for the second derivatives contributions
             * @param rLeftHandSideMatrix the condition left hand side matrix
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void CalculateSecondDerivativesLHS(MatrixType& rLeftHandSideMatrix,
                                                    const Process_Info& rCurrentProcessInfo)
            {
                if (rLeftHandSideMatrix.rows() != 0) {
                    rLeftHandSideMatrix.resize(0, 0);
                }
            }

            /**
             * this is called during the assembling process in order
             * to calculate the condition right hand side vector for the second derivatives contributions
             * @param rRightHandSideVector the condition right hand side vector
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
             * this is called during the assembling process in order
             * to calculate the condition mass matrix
             * @param rMassMatrix the condition mass matrix
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void CalculateMassMatrix(MatrixType& rMassMatrix, const Process_Info& rCurrentProcessInfo)
            {
                if (rMassMatrix.rows() != 0) {
                    rMassMatrix.resize(0, 0);
                }
            }

            /**
             * this is called during the assembling process in order
             * to calculate the condition damping matrix
             * @param rDampingMatrix the condition damping matrix
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void CalculateDampingMatrix(MatrixType& rDampingMatrix, const Process_Info& rCurrentProcessInfo)
            {
                if (rDampingMatrix.rows() != 0) {
                    rDampingMatrix.resize(0, 0);
                }
            }
            /**
             * @brief This class is derived from Geometry_Object. It have methods follow:
             * GeometryType& GetGeometry() 
             * GeometryType GetGeometry() const
            */

            /**
             * this is called during the assembling process in order
             * to calculate the condition contribution in explicit calculation.
             * NodalData is modified Inside the function, so the
             * The "AddEXplicit" FUNCTIONS THE ONLY FUNCTIONS IN WHICH A CONDITION
             * IS ALLOWED TO WRITE ON ITS NODES.
             * the caller is expected to ensure thread safety hence
             * SET/UNSETLOCK MUST BE PERFORMED IN THE STRATEGY BEFORE CALLING THIS FUNCTION
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void AddExplicitContribution(const Process_Info& rCurrentProcessInfo)
            {
            }
            /**
             * @brief This function is designed to make the condition to assemble an rRHS vector identified by a variable rRHSVariable by assembling it to the nodes on the variable rDestinationVariable. (This is the double version)
             * @details The "AddExplicit" FUNCTIONS THE ONLY FUNCTIONS IN WHICH A CONDITION IS ALLOWED TO WRITE ON ITS NODES. The caller is expected to ensure thread safety hence SET-/UNSET-LOCK MUST BE PERFORMED IN THE STRATEGY BEFORE CALLING THIS FUNCTION
             * @param rRHSVector input variable containing the RHS vector to be assembled
             * @param rRHSVariable variable describing the type of the RHS vector to be assembled
             * @param rDestinationVariable variable in the database to which the rRHSvector will be assembled
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void AddExplicitContribution(
                const VectorType& rRHSVector,
                const Variable<VectorType>& rRHSVariable,
                const Variable<double >& rDestinationVariable,
                const Process_Info& rCurrentProcessInfo
                )
            {
                std::cerr << "Base condition class is not able to assemble rRHS to the desired variable. destination variable is " << rDestinationVariable << std::endl;
            }
            /**
             * @brief This function is designed to make the condition to assemble an rRHS vector identified by a variable rRHSVariable by assembling it to the nodes on the variable rDestinationVariable. (This is the vector version)
             * @details The "AddExplicit" FUNCTIONS THE ONLY FUNCTIONS IN WHICH A CONDITION IS ALLOWED TO WRITE ON ITS NODES. The caller is expected to ensure thread safety hence SET-/UNSET-LOCK MUST BE PERFORMED IN THE STRATEGY BEFORE CALLING THIS FUNCTION
             * @param rRHSVector input variable containing the RHS vector to be assembled
             * @param rRHSVariable variable describing the type of the RHS vector to be assembled
             * @param rDestinationVariable variable in the database to which the rRHSvector will be assembled
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void AddExplicitContribution(
                const VectorType& rRHSVector,
                const Variable<VectorType>& rRHSVariable,
                const Variable<std::array<double,3> >& rDestinationVariable,
                const Process_Info& rCurrentProcessInfo
                )
            {
                std::cerr << "Base condition class is not able to assemble rRHS to the desired variable. destination variable is " << rDestinationVariable << std::endl;
            }
            /**
             * @brief This function is designed to make the condition to assemble an rRHS vector identified by a variable rRHSVariable by assembling it to the nodes on the variable rDestinationVariable. (This is the matrix version)
             * @details The "AddExplicit" FUNCTIONS THE ONLY FUNCTIONS IN WHICH A CONDITION IS ALLOWED TO WRITE ON ITS NODES. The caller is expected to ensure thread safety hence SET-/UNSET-LOCK MUST BE PERFORMED IN THE STRATEGY BEFORE CALLING THIS FUNCTION
             * @param rRHSVector input variable containing the RHS vector to be assembled
             * @param rRHSVariable variable describing the type of the RHS vector to be assembled
             * @param rDestinationVariable variable in the database to which the rRHSvector will be assembled
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void AddExplicitContribution(
                const MatrixType& rLHSMatrix,
                const Variable<MatrixType>& rLHSVariable,
                const Variable<MatrixType>& rDestinationVariable,
                const Process_Info& rCurrentProcessInfo
                )
            {
                std::cerr << "Base condition class is not able to assemble rLHS to the desired variable. destination variable is " << rDestinationVariable << std::endl;
            }

            virtual void CalculateOnIntegrationPoints(const Variable<bool>& rVariable,
                          std::vector<bool>& rOutput,
                          const Process_Info& rCurrentProcessInfo)
            {
            }
            virtual void CalculateOnIntegrationPoints(const Variable<int>& rVariable,
                                std::vector<int>& rOutput,
                                const Process_Info& rCurrentProcessInfo)
            {
            }
            virtual void CalculateOnIntegrationPoints(const Variable<double>& rVariable,
                                std::vector<double>& rOutput,
                                const Process_Info& rCurrentProcessInfo)
            {
            }
            virtual void CalculateOnIntegrationPoints(const Variable<std::array<double, 3 > >& rVariable,
                                std::vector< std::array<double, 3 > >& rOutput,
                                const Process_Info& rCurrentProcessInfo)
            {
            }
            virtual void CalculateOnIntegrationPoints(const Variable<std::array<double, 6 > >& rVariable,
                                std::vector< std::array<double, 6 > >& rOutput,
                                const Process_Info& rCurrentProcessInfo)
            {
            }
            virtual void CalculateOnIntegrationPoints(const Variable<VectorType >& rVariable,
                                std::vector< VectorType >& rOutput,
                                const Process_Info& rCurrentProcessInfo)
            {
            }
            virtual void CalculateOnIntegrationPoints(const Variable<MatrixType >& rVariable,
                                std::vector< MatrixType >& rOutput,
                                const Process_Info& rCurrentProcessInfo)
            {
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
                    std::cerr << "Condition found with Id " << this->Id() << std::endl;
                } 
                const double domain_size = this->GetGeometry().DomainSize();
                if( domain_size < 0.0 )
                {
                    std::cerr<< "Condition " << this->Id() << " has negative size " << domain_size << std::endl;
                } 
                GetGeometry().Check();
                return 0;
            }

        /// @}

        /// @name Access
        /// @{
            
            /**
             * determines the condition list of DOFs
             * @param ConditionDofList the list of DOFs
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void GetDofList(DofsVectorType& rElementalDofList,
                                    const Process_Info& rCurrentProcessInfo) const
            {
                if (rElementalDofList.size() != 0) {
                    rElementalDofList.resize(0);
                }
            }
            
            /**
             * this determines the condition equation ID vector for all condition
             * DOFs
             * @param rResult the condition equation ID vector
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void EquationIdVector(EquationIdVectorType& rResult,
                                        const Process_Info& rCurrentProcessInfo) const
            {
                if (rResult.size() != 0) {
                    rResult.resize(0);
                }
            }
  
            /**
             * Getting method to obtain the variable which defines the degrees of freedom
             */
            virtual void GetValuesVector(VectorType& values, int Step = 0) const
            {
                if (values.size() != 0) {
                    values.resize(0, false);
                }
            }

            /**
             * Getting method to obtain the time derivative of variable which defines the degrees of freedom
             */
            virtual void GetFirstDerivativesVector(VectorType& values, int Step = 0) const
            {
                if (values.size() != 0) {
                    values.resize(0, false);
                }
            }

            /**
             * Getting method to obtain the second time derivative of variable which defines the degrees of freedom
             */
            virtual void GetSecondDerivativesVector(VectorType& values, int Step = 0) const
            {
                if (values.size() != 0) {
                    values.resize(0, false);
                }
            }

            /**
             * @name Get Properties 
            */
            Properties::Pointer pGetProperties()
            {
                return mpProperties;
            }
        /// @}
    private:
        Properties::Pointer mpProperties;
};

#endif