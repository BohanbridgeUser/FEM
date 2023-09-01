#ifndef _BOUNDARY_CONDITION_H_
#define _BOUNDARY_CONDITION_H_
#include "../define.h"
#include "condition.h"

class Boundary_Condition : public Condition
{
    public:
        /// @name Type Define
        /// @{
            typedef Variable<std::array<double,3> > 
                                                                VariableVectorType;
            typedef Variable<double>
                                                                VariableScalarType;
            typedef Geometry<Node>
                                                                      GeometryType;
            typedef typename Geometry<Node>::SizeType
                                                                          SizeType;
            typedef Boundary_Condition
                                                                         ClassType;
            typedef Condition
                                                                     ConditionType;
            typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                                        MatrixType;
            typedef Eigen::Matrix<double,Eigen::Dynamic,1>
                                                                        VectorType;
            typedef ConditionType::PropertiesType
                                                                    PropertiesType;
            typedef Node::DofPointersContainerType
                                                                    DofsVectorType;
            typedef std::vector<size_t>
                                                              EquationIdVectorType;
            typedef ConditionType::NodesContainerType
                                                                NodesContainerType;
            LOTUS_POINTER_DEFINE(Boundary_Condition)
        /// @}


        /// @name Life Circle
        /// @{
            /// Empty constructor needed for serialization
            Boundary_Condition();

            /// Default constructor.
            Boundary_Condition( IndexType NewId, 
                                GeometryType::Pointer pGeometry );

            Boundary_Condition( IndexType NewId, 
                                GeometryType::Pointer pGeometry, 
                                PropertiesType::Pointer pProperties );

            /// Copy constructor
            Boundary_Condition( Boundary_Condition const& rOther);

            /// Destructor
            ~Boundary_Condition() override
            {};
        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{
            /**
             * creates a new condition pointer
             * @param NewId: the ID of the new condition
             * @param ThisNodes: the nodes of the new condition
             * @param pProperties: the properties assigned to the new condition
             * @return a Pointer to the new condition
             */
            Condition::SharedPointer Create(IndexType NewId,
                        NodesContainerType const& ThisNodes,
                        PropertiesType::Pointer pProperties ) const override;

            void Initialize(const Process_Info& rCurrentProcessInfo) override;

            void InitializeSolutionStep(const Process_Info& rCurrentProcessInfo) override;

            void InitializeNonLinearIteration(const Process_Info& rCurrentProcessInfo) override;

            void CalculateLocalSystem(MatrixType& rLeftHandSideMatrix,
                                      VectorType& rRightHandSideVector,
                                      const Process_Info& rCurrentProcessInfo ) override;

            /**
             * this is called during the assembling process in order
             * to calculate the condition right hand side vector only
             * @param rRightHandSideVector: the condition right hand side vector
             * @param rCurrentProcessInfo: the current process info instance
             */
            void CalculateRightHandSide(VectorType& rRightHandSideVector,
                                        const Process_Info& rCurrentProcessInfo ) override;

            /**
             * this is called during the assembling process in order
             * to calculate the condition left hand side matrix only
             * @param rLeftHandSideMatrix: the condition left hand side matrix
             * @param rCurrentProcessInfo: the current process info instance
             */
            void CalculateLeftHandSide(MatrixType& rLeftHandSideMatrix,
                                       const Process_Info& rCurrentProcessInfo) override;

            /**
             * this is called during the assembling process in order
             * to calculate the condition mass matrix
             * @param rMassMatrix: the condition mass matrix
             * @param rCurrentProcessInfo: the current process info instance
             */
            void CalculateMassMatrix(MatrixType& rMassMatrix,
                                     const Process_Info& rCurrentProcessInfo ) override;

            /**
             * this is called during the assembling process in order
             * to calculate the condition damping matrix
             * @param rDampingMatrix: the condition damping matrix
             * @param rCurrentProcessInfo: the current process info instance
             */
            void CalculateDampingMatrix(MatrixType& rDampingMatrix,
                                        const Process_Info& rCurrentProcessInfo ) override;

            /**
             * this function is designed to make the element to assemble an rRHS vector
             * identified by a variable rRHSVariable by assembling it to the nodes on the variable
             * rDestinationVariable.
             * @param rRHSVector: input variable containing the RHS vector to be assembled
             * @param rRHSVariable: variable describing the type of the RHS vector to be assembled
             * @param rDestinationVariable: variable in the database to which the rRHSvector will be assembled
             * @param rCurrentProcessInfo: the current process info instance
             */
            void AddExplicitContribution(const VectorType& rRHS,
                            const Variable<VectorType>& rRHSVariable,
                            const Variable<std::array<double,3> >& rDestinationVariable,
                            const Process_Info& rCurrentProcessInfo) override;


            /**
             * Calculate a double Variable
             */
            void CalculateOnIntegrationPoints(const Variable<double>& rVariable,
                            std::vector<double>& rOutput,
                            const Process_Info& rCurrentProcessInfo) override;

             /**
             * This function provides the place to perform checks on the completeness of the input.
             * It is designed to be called only once (or anyway, not often) typically at the beginning
             * of the calculations, so to verify that nothing is missing from the input
             * or that no common error is found.
             * @param rCurrentProcessInfo
             */
            int Check( const Process_Info& rCurrentProcessInfo ) const override;

        /// @}


        /// @name Access
        /// @{
            /**
             * Sets on rConditionDofList the degrees of freedom of the considered element geometry
             */
            void GetDofList(DofsVectorType& rConditionDofList,
                    const Process_Info& rCurrentProcessInfo ) const override;
            /**
             * Sets on rResult the ID's of the element degrees of freedom
             */
            void EquationIdVector(EquationIdVectorType& rResult,
                    const Process_Info& rCurrentProcessInfo ) const override;
            /**
             * Sets on rValues the nodal displacements
             */
            void GetValuesVector(VectorType& rValues,
                    int Step = 0 ) const override;
            /**
             * Sets on rValues the nodal velocities
             */
            void GetFirstDerivativesVector(VectorType& rValues,
                        int Step = 0 ) const override;
            /**
             * Sets on rValues the nodal accelerations
             */
            void GetSecondDerivativesVector(VectorType& rValues,
                            int Step = 0 ) const override;

        /// @}


        /// @name Inquiry
        /// @{


        /// @}


    protected:
        /// @name Protected Define
        /// @{
            LOTUS_DEFINE_LOCAL_FLAGS( COMPUTE_RHS_VECTOR );
            LOTUS_DEFINE_LOCAL_FLAGS( COMPUTE_LHS_MATRIX );

            struct ConditionVariables
            {
                private:
                    const Geometry<Node>::ShapeFunctionGradientsType* 
                                                                pDN_De;
                    const Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>* 
                                                                pNcontainer;
                
                public:
                    /* for axisymmetric use only */
                    double  CurrentRadius;
                    double  ReferenceRadius;

                    /* general variables */
                    double  GeometrySize;
                    double  Jacobian;
                    VectorType  N;
                    MatrixType  DN_De;
                    MatrixType  DeltaPosition;
                    /* external boundary values */
                    double  ExternalScalarValue;
                    VectorType  ExternalVectorValue;
                    /* boundary characteristics */
                    VectorType  Normal;
                    VectorType  Tangent1;
                    VectorType  Tangent2;
                    /* variables including all integration points */
                    GeometryType::JacobiansType j;
                    GeometryType::JacobiansType J;
                    
                    /**
                     * sets the value of a specified pointer variable
                     */
                    void SetShapeFunctionsGradients(const GeometryType::ShapeFunctionGradientsType &rDN_De)
                    {
                        pDN_De=&rDN_De;
                    };

                    void SetShapeFunctions(const MatrixType& rNcontainer)
                    {
                        pNcontainer=&rNcontainer;
                    };

                    /**
                     * returns the value of a specified pointer variable
                     */
                    const GeometryType::ShapeFunctionGradientsType& GetShapeFunctionsGradients()
                    {
                        return *pDN_De;
                    };
                    const MatrixType& GetShapeFunctions()
                    {
                        return *pNcontainer;
                    };

                    void Initialize( const unsigned int& dimension,
                        const unsigned int& local_dimension,
                        const unsigned int& number_of_nodes )
                    {
                        CurrentRadius   = 0;
                        ReferenceRadius = 0;
                        GeometrySize = 1;
                        Jacobian     = 1;
                        ExternalScalarValue  = 0;
                        ExternalVectorValue.resize(dimension);
                        ExternalVectorValue.setZero(dimension);
                        N.resize(number_of_nodes);
                        Normal.resize(dimension);
                        Tangent1.resize(dimension);
                        Tangent2.resize(dimension);
                        N.setZero(number_of_nodes);
                        Normal.setZero(dimension);
                        Tangent1.setZero(dimension);
                        Tangent2.setZero(dimension);
                        //matrices
                        DN_De.resize(number_of_nodes, local_dimension);
                        DN_De.setZero(number_of_nodes,local_dimension);
                        DeltaPosition.resize(number_of_nodes, dimension);
                        DeltaPosition.setZero(number_of_nodes, dimension);
                        //others
                        J.resize(1);
                        j.resize(1);
                        J[0].resize(dimension,dimension);
                        j[0].resize(dimension,dimension);
                        J[0].setZero(dimension,dimension);
                        j[0].setZero(dimension,dimension);
                        //pointers
                        pDN_De = NULL;
                        pNcontainer = NULL;
                    }

            };

            struct LocalSystemComponents
            {
            private:
                //for calculation local system with compacted LHS and RHS
                MatrixType *mpLeftHandSideMatrix;
                VectorType *mpRightHandSideVector;

            public:
                //calculation flags
                Flags  CalculationFlags;

                /**
                 * sets the value of a specified pointer variable
                 */
                void SetLeftHandSideMatrix( MatrixType& rLeftHandSideMatrix ) { mpLeftHandSideMatrix = &rLeftHandSideMatrix; };
                void SetRightHandSideVector( VectorType& rRightHandSideVector ) { mpRightHandSideVector = &rRightHandSideVector; };
                /**
                 * returns the value of a specified pointer variable
                 */
                MatrixType& GetLeftHandSideMatrix() { return *mpLeftHandSideMatrix; };
                VectorType& GetRightHandSideVector() { return *mpRightHandSideVector; };
            };
        /// @}

        /// @name Protected Static Member Variables
        /// @{


        /// @}


        /// @name Protected Member Variables
        /// @{
            IntegrationMethod mThisIntegrationMethod;

        /// @}


        /// @name Protected Operatiors
        /// @{


        /// @}


        /// @name Protected Operations
        /// @{
            
            /**
             * Initialize Explicit Contributions
             */
            void InitializeExplicitContributions();

            /**
             * Check dof for a vector variable
             */
            virtual bool HasVariableDof(VariableVectorType& rVariable) const;

            /**
             * Check dof for a double variable
             */
            virtual bool HasVariableDof(VariableScalarType& rVariable) const;

            /**
             * Get condition size from the dofs
             */
            virtual unsigned int GetDofsSize() const;

            /**
             * Initialize System Matrices
             */
            virtual void InitializeSystemMatrices(MatrixType& rLeftHandSideMatrix,
                            VectorType& rRightHandSideVector,
                            Flags& rCalculationFlags);

            /**
             * Initialize General Variables
             */
            virtual void InitializeConditionVariables(ConditionVariables& rVariables,
                                const Process_Info& rCurrentProcessInfo);

            /**
             * Calculate Condition Kinematics
             */
            virtual void CalculateKinematics(ConditionVariables& rVariables,
                            const double& rPointNumber);

            /**
             * Calculates the condition contributions
             */
            virtual void CalculateConditionSystem(LocalSystemComponents& rLocalSystem,
                            const Process_Info& rCurrentProcessInfo);

            /**
             * Calculation and addition of the matrices of the LHS
             */
            virtual void CalculateAndAddLHS(LocalSystemComponents& rLocalSystem,
                                            ConditionVariables& rVariables,
                                            double& rIntegrationWeight);

            /**
             * Calculation and addition of the vectors of the RHS
             */
            virtual void CalculateAndAddRHS(LocalSystemComponents& rLocalSystem,
                                            ConditionVariables& rVariables,
                                            double& rIntegrationWeight);

            /**
             * Calculation of the Load Stiffness Matrix which usually is subtracted to the global stiffness matrix
             */
            virtual void CalculateAndAddKuug(MatrixType& rLeftHandSideMatrix,
                            ConditionVariables& rVariables,
                            double& rIntegrationWeight);

            /**
             * Calculation of the External Forces Vector for a force or pressure vector
             */
            virtual void CalculateAndAddExternalForces(VectorType& rRightHandSideVector,
                                ConditionVariables& rVariables,
                                double& rIntegrationWeight);

            /**
             * Calculation of the External Forces Vector for a force or pressure vector
             */
            virtual double& CalculateAndAddExternalEnergy(double& rEnergy,
                                ConditionVariables& rVariables,
                                double& rIntegrationWeight,
                                const Process_Info& rCurrentProcessInfo);
            /**
             * Get Node Movements for energy computation
             */
            void GetNodalDeltaMovements(VectorType& rValues, const int& rNode);

            /**
             * Get Current Value, buffer 0 with FastGetSolutionStepValue
             */
            VectorType& GetNodalCurrentValue(const Variable<std::array<double,3> >&rVariable,
                                             VectorType& rValue,
                                             const unsigned int& rNode);

            /**
             * Get Previous Value, buffer 1 with FastGetSolutionStepValue
             */
            VectorType& GetNodalPreviousValue(const Variable<std::array<double,3> >&rVariable,
                                              VectorType& rValue, 
                                              const unsigned int& rNode);


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