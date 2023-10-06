#ifndef _SOLID_ELEMENT_H_
#define _SOLID_ELEMENT_H_
#include "../../../../include/Element/element.h"
#include "../../../../include/Variable/variables.h"
#include "../../../../include/Utility/element_utility.h"

#include <Eigen/Eigen>
class Solid_Element : public Element
{
    public:
        /// @name Type Define
        /// @{
            typedef size_t
                                                                   SizeType;
            typedef Constitutive_Law 
                                                        ConstitutiveLawType;
            typedef Properties  
                                                             PropertiesType;
            typedef Element
                                                                   BaseType;
            ///Pointer type for constitutive laws
            typedef ConstitutiveLawType::Pointer 
                                                 ConstitutiveLawPointerType;
            ///StressMeasure from constitutive laws
            typedef ConstitutiveLawType::StressMeasure 
                                                          StressMeasureType;
            ///Type definition for integration methods
            typedef Geometry_Data::IntegrationMethod 
                                                          IntegrationMethod;
            typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> 
                                                                     Matrix;
            typedef Eigen::Matrix<double,Eigen::Dynamic,1> 
                                                                     Vector;
            typedef BaseType::MatrixType
                                                                 MatrixType;
            typedef BaseType::VectorType
                                                                 VectorType;

            LOTUS_DEFINE_LOCAL_FLAGS( COMPUTE_RHS_VECTOR );
            LOTUS_DEFINE_LOCAL_FLAGS( COMPUTE_LHS_MATRIX );
            LOTUS_DEFINE_LOCAL_FLAGS( FINALIZED_STEP );

            struct ElementData
            {
            private:
                /**
                 * @param pDN_De Shape Functions Gradients Values @param pNcontainer Shap Functions Values
                */
                const GeometryType::ShapeFunctionGradientsType* pDN_De;
                const Matrix* pNcontainer;
                const Process_Info* pProcessInfo;
            public:
                StressMeasureType StressMeasure;
                double  Tau;
                double  IntegrationWeight;
                //for axisymmetric use only
                double  CurrentRadius;
                double  ReferenceRadius;
                //general variables for large displacement use
                double  detF;
                double  detF0;
                double  detH; //Wildcard ( detF(0 to n+1) )
                double  detJ;
                Vector  StrainVector;
                Vector  StressVector;
                Vector  N;
                Matrix  B;
                Matrix  H;    //Wildcard    Displacement Gradient, F(0 to n+1), B-bar, Velocity Gradient...
                Matrix  F;    //Incremental Deformation Gradient (n to n+1)
                Matrix  F0;   //Historical  Deformation Gradient  (0 to n)
                Matrix  DN_DX;
                Matrix  ConstitutiveMatrix;
                //variables including all integration points
                GeometryType::JacobiansType J;
                GeometryType::JacobiansType j;
                Matrix  DeltaPosition;

                /**
                 * sets the value of a specified pointer variable
                 */
                void SetShapeFunctionsGradients(const GeometryType::ShapeFunctionGradientsType &rDN_De)
                {
                    pDN_De=&rDN_De;
                };
                void SetShapeFunctions(const Matrix& rNcontainer)
                {
                    pNcontainer=&rNcontainer;
                };
                void SetProcessInfo(const Process_Info& rProcessInfo)
                {
                    pProcessInfo=&rProcessInfo;
                };

                /**
                 * returns the value of a specified pointer variable
                 */
                const GeometryType::ShapeFunctionGradientsType& GetShapeFunctionsGradients()
                {
                    return *pDN_De;
                };

                const Matrix& GetShapeFunctions()
                {
                    return *pNcontainer;
                };

                const Process_Info& GetProcessInfo()
                {
                    return *pProcessInfo;
                };

                void Initialize( const unsigned int& voigt_size,
                                 const unsigned int& dimension,
                                 const unsigned int& number_of_nodes )
                {
                    StressMeasure = Constitutive_Law::StressMeasure_PK2;
                    //stabilization
                    Tau = 0;
                    //time step
                    IntegrationWeight = 1;
                    //radius
                    CurrentRadius = 0;
                    ReferenceRadius = 0;
                    //jacobians
                    detF  = 1;
                    detF0 = 1;
                    detH  = 1;
                    detJ  = 1;
                    //vectors
                    StrainVector.resize(voigt_size);
                    StrainVector.setZero(voigt_size);
                    StressVector.resize(voigt_size);
                    StressVector.setZero(voigt_size);
                    N.resize(number_of_nodes);
                    N.setZero(voigt_size);
                    //matrices
                    B.resize(voigt_size, dimension*number_of_nodes);
                    H.resize(dimension,dimension);
                    F.resize(dimension,dimension);
                    F0.resize(dimension,dimension);
                    DN_DX.resize(number_of_nodes, dimension);
                    ConstitutiveMatrix.resize(voigt_size, voigt_size);
                    DeltaPosition.resize(number_of_nodes, dimension);

                    B.setZero();
                    H.setZero();
                    F.setZero();
                    F0.setZero();
                    DN_DX.setZero();
                    ConstitutiveMatrix.setZero();
                    DeltaPosition.setZero();
                    //others
                    J.resize(1);
                    j.resize(1);
                    J[0].resize(dimension,dimension);
                    j[0].resize(dimension,dimension);
                    J[0].setZero();
                    J[0].setZero();
                    //pointers
                    pDN_De = NULL;
                    pNcontainer = NULL;
                    pProcessInfo = NULL;
                }
            };
            typedef ElementData
                                                            ElementDataType;

            struct LocalSystemComponents
            {
                private:
                    //for calculation local system with compacted LHS and RHS
                    Matrix* mpLeftHandSideMatrix;
                    Vector* mpRightHandSideVector;
                public:
                    //calculation flags
                    Flags  CalculationFlags;
                    /**
                     * sets the value of a specified pointer variable
                     */
                    void SetLeftHandSideMatrix( Matrix& rLeftHandSideMatrix ) { mpLeftHandSideMatrix = &rLeftHandSideMatrix; };
                    void SetRightHandSideVector( Vector& rRightHandSideVector ) { mpRightHandSideVector = &rRightHandSideVector; };
                    /**
                     * returns the value of a specified pointer variable
                     */
                    Matrix& GetLeftHandSideMatrix() { return *mpLeftHandSideMatrix; };
                    Vector& GetRightHandSideVector() { return *mpRightHandSideVector; };
            };

        /// @}


        /// @name Life Circle
        /// @{
            Solid_Element()
            :BaseType()
            {

            }
            Solid_Element(IndexType NewId, GeometryType::Pointer pGeometry)
            :BaseType(NewId,pGeometry)
            {

            }
            Solid_Element(IndexType NewId, GeometryType::Pointer pGeometry, typename PropertiesType::Pointer pProperties)
            :BaseType(NewId,pGeometry,pProperties)
            {
                this->Set(SOLID);
                mThisIntegrationMethod = GetGeometry().GetDefaultIntegrationMethod();
            }
            ///Copy constructor
            Solid_Element(const Solid_Element& another)
            :BaseType(another),
             mThisIntegrationMethod(another.mThisIntegrationMethod),
             mConstitutiveLawVector(another.mConstitutiveLawVector)
            {

            }
            Solid_Element(Solid_Element&& another)
            :BaseType(another),
             mThisIntegrationMethod(another.mThisIntegrationMethod),
             mConstitutiveLawVector(another.mConstitutiveLawVector)
            {
            
            }
            /// Destructor.
            ~Solid_Element() override
            {

            }
        /// @}


        /// @name Operators
        /// @{
            Solid_Element& operator=(const Solid_Element& another)
            {
                this->mpGeometry = another.mpGeometry;
                this->mThisIntegrationMethod = another.mThisIntegrationMethod;

                this->mConstitutiveLawVector.clear();
                this->mConstitutiveLawVector.resize(another.mConstitutiveLawVector.size());
                for(auto item:another.mConstitutiveLawVector)
                    this->mConstitutiveLawVector.push_back(item);
                return *this; 
            }
        /// @}


        /// @name Operations
        /// @{
            virtual Element::SharedPointer Create(IndexType NewId,
                                          NodesContainerType const& rNodes,
                                          Properties::Pointer pProperties) const override;
            virtual Element::SharedPointer Clone(IndexType NewId,
                                    NodesContainerType const& rNodes) const override;


            void GetDofList(DofsVectorType& rElementalDofList, const Process_Info& rCurrentProcessInfo) const override
            {
                rElementalDofList.resize(0);
                const int dimension = GetGeometry().WorkingSpaceDimension();

                for (int i=0;i<GetGeometry().size();++i)
                {
                    rElementalDofList.push_back( GetGeometry()[i].pGetDof( DISPLACEMENT_X ) );
                    rElementalDofList.push_back( GetGeometry()[i].pGetDof( DISPLACEMENT_Y ) );
                    if( dimension == 3 )
                        rElementalDofList.push_back( GetGeometry()[i].pGetDof( DISPLACEMENT_Z ) );
                }
            }

            void CalculateLocalSystem(MatrixType& rLeftHandSideMatrix,
			                          VectorType& rRightHandSideVector,
			                          const Process_Info& rCurrentProcessInfo) override;

             /**
             * Called to initialize the element.
             * Must be called before any calculation is done
             */
            void Initialize(const Process_Info& rCurrentProcessInfo) override;

            /**
             * Initialize Element General Variables
             */
            virtual void InitializeElementData(ElementDataType & rVariables,
                                               const Process_Info& rCurrentProcessInfo);
            
            /**
             * Initialize Material Properties on the Constitutive Law
             */
            void InitializeConstitutiveLaw();

            /**
             * Called at the beginning of each solution step
             */
            void InitializeSolutionStep(const Process_Info& rCurrentProcessInfo) override;

            /**
             * Clear Nodal Forces
             */
            void InitializeExplicitContributions();

            /**
             * Called at the end of eahc solution step
             */
            void FinalizeSolutionStep(const Process_Info& rCurrentProcessInfo) override;

            /**
             * Finalize Element Internal Variables
             */
            virtual void FinalizeStepVariables(ElementDataType & rVariables,
                                               const double& rPointNumber);
            
            /**
             * Initialize System Matrices
             */
            virtual void InitializeSystemMatrices(MatrixType& rLeftHandSideMatrix,
                                                VectorType& rRightHandSideVector,
                                                Flags& rCalculationFlags);
            /**
             * Calculates the elemental contributions
             */
            virtual void CalculateElementalSystem(LocalSystemComponents& rLocalSystem,
                                                const Process_Info& rCurrentProcessInfo);
            /**
             * Calculate Element Kinematics
             */
            virtual void CalculateKinematics(ElementDataType& rVariables,
                                            const double& rPointNumber);

            /**
             * Set Parameters for the Constitutive Law and Calculate Material Response
             */
            virtual void CalculateMaterialResponse(ElementDataType& rVariables,
                                                Constitutive_Law::Parameters& rValues,
                                                const int & rPointNumber);

            /**
             * Set Variables of the Element to the Parameters of the Constitutive Law
             */
            virtual void SetElementData(ElementDataType& rVariables,
                                        Constitutive_Law::Parameters& rValues,
                                        const int & rPointNumber);
            /**
             * Calculation and addition of the matrices of the LHS
             */
            virtual void CalculateAndAddLHS(LocalSystemComponents& rLocalSystem,
                                            ElementDataType& rVariables,
                                            double& rIntegrationWeight);

             /**
             * Calculation of the Volume Force of the Element
             */
            virtual Vector& CalculateVolumeForce(Vector& rVolumeForce, ElementDataType& rVariables);

            /**
             * Calculation and addition of the vectors of the RHS
             */
            virtual void CalculateAndAddRHS(LocalSystemComponents& rLocalSystem,
                                            ElementDataType& rVariables,
                                            Vector& rVolumeForce,
                                            double& rIntegrationWeight);

            /**
             * Calculation of the Material Stiffness Matrix. Kuum = BT * C * B
             */
            virtual void CalculateAndAddKuum(MatrixType& rLeftHandSideMatrix,
                                             ElementDataType& rVariables,
                                             double& rIntegrationWeight);

            /**
             * Calculation of the Geometric Stiffness Matrix. Kuug = BT * S
             */
            virtual void CalculateAndAddKuug(MatrixType& rLeftHandSideMatrix,
                                             ElementDataType& rVariables,
                                             double& rIntegrationWeight);

            /**
             * Calculation of the External Forces Vector. Fe = N * t + N * b
             */
            virtual void CalculateAndAddExternalForces(VectorType& rRightHandSideVector,
                                                       ElementDataType& rVariables,
                                                       Vector& rVolumeForce,
                                                       double& rIntegrationWeight);


            /**
             * Calculation of the Internal Forces Vector. Fi = B * sigma
             */
            virtual void CalculateAndAddInternalForces(VectorType& rRightHandSideVector,
                                                       ElementDataType & rVariables,
                                                       double& rIntegrationWeight);

            /**
             * Calculation of the Integration Weight
             */
            virtual double& CalculateIntegrationWeight(double& rIntegrationWeight);

            /**
             * Calculation of the Volume Change of the Element
             */
            virtual double& CalculateVolumeChange(double& rVolumeChange, ElementDataType& rVariables);


            /**
             * Transform Element General Variables
             */
            virtual void TransformElementData(ElementDataType & rVariables,
                                              const double& rPointNumber);

            /**
             * Calculation of the tangent via perturbation of the dofs variables : testing purposes
             */
            void CalculatePerturbedLeftHandSide (MatrixType& rLeftHandSideMatrix,
                                                 const Process_Info& rCurrentProcessInfo);

            /**
             * This function provides the place to perform checks on the completeness of the input.
             * It is designed to be called only once (or anyway, not often) typically at the beginning
             * of the calculations, so to verify that nothing is missing from the input
             * or that no common error is found.
             * @param rCurrentProcessInfo
             */
            int Check(const Process_Info& rCurrentProcessInfo) const override;


        /// @}


        /// @name Access
        /// @{
            Solid_Element::IntegrationMethod GetIntegrationMethod() const
            {
                return mThisIntegrationMethod;
            }
        
            /**
             * Sets on rResult the ID's of the element degrees of freedom
             */
            void EquationIdVector(EquationIdVectorType& rResult, const Process_Info& rCurrentProcessInfo) const override;
        /// @}


        /// @name Inquiry
        /// @{
            /**
             * Get element calculation flag
             */
            bool IsSliver()
            {
                if( this->IsDefined(SELECTED) )
                    return this->Is(SELECTED);
                else
                    return false;
            }

            
            /**
             * Get element size from the dofs
             */
            virtual SizeType GetDofsSize() const;
        /// @}


    protected:
        /// @name Protected Static Member Variables
        /// @{


        /// @}


        /// @name Protected Member Variables
        /// @{
            IntegrationMethod mThisIntegrationMethod;
            std::vector<Constitutive_Law::Pointer> mConstitutiveLawVector;
        /// @}


        /// @name Protected Operatiors
        /// @{


        /// @}


        /// @name Protected Operations
        /// @{

            /**
             * Calculation of the Total Position Increment
             */
            virtual Matrix& CalculateTotalDeltaPosition(Matrix & rDeltaPosition);

        /// @}


        /// @name Protected Access
        /// @{


        /// @}


        /// @name Protected Inquiry
        /// @{


        /// @}


    private:
        /// @name Private Static Member Variables
        /// @{


        /// @}


        /// @name Private Member Variables
        /// @{


        /// @}


        /// @name Private Operatiors
        /// @{


        /// @}


        /// @name Private Operations
        /// @{


        /// @}


        /// @name Private Access
        /// @{


        /// @}


        /// @name Private Inquiry
        /// @{


        /// @}
};

#endif