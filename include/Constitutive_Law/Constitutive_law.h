#ifndef _CONSTITUTIVE_LAW_H_
#define _CONSTITUTIVE_LAW_H_
#include "../define.h"
#include "../Variable/variables.h"
#include "../Container/flags.h"
#include "../Process/process.h"
#include "../Geometry/geometry.h"
#include "../Node/node.h"
#include "../Linear_Solver/dense_space.h"
#include "../Linear_Solver/sparse_space.h"
#include "../Utility/math_utility.h"
#include "../process_info.h"
#include "../Property/properties.h"
#include "initial_state.h"

#include <Eigen/Eigen>

class Constitutive_Law : public Flags
{
    public:
        /// @name Type Define
        /// @{
            typedef Process_Info 
                                                            ProcessInfoType;
            typedef std::size_t 
                                                                   SizeType;
            typedef Geometry<Node > 
                                                               GeometryType;
            typedef typename DenseSpace::Vector 
                                                           StrainVectorType;
            typedef typename DenseSpace::Vector 
                                                           StressVectorType;
            typedef typename DenseSpace::Vector
                                                                     Vector;
            typedef typename DenseSpace::Matrix
                                                                     Matrix;
            typedef typename DenseSpace::Matrix
                                                        VoigtSizeMatrixType;           // Constitutive Matrix
            typedef typename DenseSpace::Matrix 
                                               DeformationGradientMatrixType; // Def. gradient tensor
            LOTUS_SHARED_POINTER_DEFINE(Constitutive_Law)

            enum StrainMeasure
            {
                StrainMeasure_Infinitesimal,   //strain measure small displacements
                StrainMeasure_GreenLagrange,   //strain measure reference configuration
                StrainMeasure_Almansi,         //strain measure current configuration

                // True strain:
                StrainMeasure_Hencky_Material, //strain measure reference configuration
                StrainMeasure_Hencky_Spatial,  //strain measure current   configuration

                // Deformation measures:
                StrainMeasure_Deformation_Gradient, //material deformation gradient as a strain measure
                StrainMeasure_Right_CauchyGreen,    //right cauchy-green tensor as a strain measure
                StrainMeasure_Left_CauchyGreen,     //left  cauchy-green tensor as a strain measure
                StrainMeasure_Velocity_Gradient     //spatial velocity gradient as a strain measure
            };

            enum StressMeasure
            {
                StressMeasure_PK1,            //stress related to reference configuration non-symmetric
                StressMeasure_PK2,            //stress related to reference configuration
                StressMeasure_Kirchhoff,      //stress related to current   configuration
                StressMeasure_Cauchy          //stress related to current   configuration
            };
        
            /**
             * Flags related to the Parameters of the Contitutive Law
             */
            LOTUS_DEFINE_LOCAL_FLAGS( USE_ELEMENT_PROVIDED_STRAIN );
            LOTUS_DEFINE_LOCAL_FLAGS( COMPUTE_STRESS );
            LOTUS_DEFINE_LOCAL_FLAGS( COMPUTE_CONSTITUTIVE_TENSOR );
            LOTUS_DEFINE_LOCAL_FLAGS( COMPUTE_STRAIN_ENERGY );

            LOTUS_DEFINE_LOCAL_FLAGS( ISOCHORIC_TENSOR_ONLY );
            LOTUS_DEFINE_LOCAL_FLAGS( VOLUMETRIC_TENSOR_ONLY );

            LOTUS_DEFINE_LOCAL_FLAGS( MECHANICAL_RESPONSE_ONLY );
            LOTUS_DEFINE_LOCAL_FLAGS( THERMAL_RESPONSE_ONLY );

            LOTUS_DEFINE_LOCAL_FLAGS( INCREMENTAL_STRAIN_MEASURE );

            /**
             * the next two flags are designed for internal use within the constitutive law.
             * please DO NOT use them from the API
            */
            LOTUS_DEFINE_LOCAL_FLAGS( INITIALIZE_MATERIAL_RESPONSE );
            LOTUS_DEFINE_LOCAL_FLAGS( FINALIZE_MATERIAL_RESPONSE );

            /**
             * Flags related to the Features of the Contitutive Law
             */
            LOTUS_DEFINE_LOCAL_FLAGS( FINITE_STRAINS );
            LOTUS_DEFINE_LOCAL_FLAGS( INFINITESIMAL_STRAINS );

            LOTUS_DEFINE_LOCAL_FLAGS( THREE_DIMENSIONAL_LAW );
            LOTUS_DEFINE_LOCAL_FLAGS( PLANE_STRAIN_LAW );
            LOTUS_DEFINE_LOCAL_FLAGS( PLANE_STRESS_LAW );
            LOTUS_DEFINE_LOCAL_FLAGS( AXISYMMETRIC_LAW );

            LOTUS_DEFINE_LOCAL_FLAGS( U_P_LAW );

            LOTUS_DEFINE_LOCAL_FLAGS( ISOTROPIC );
            LOTUS_DEFINE_LOCAL_FLAGS( ANISOTROPIC );

            struct Features
            {
                LOTUS_POINTER_DEFINE(Features);

                /**
                 * Structure "Features" to be used by the element to get the the constitutive law characteristics*
                 * its variables will be used to check constitutive law and element compatibility
                 * @param mOptions        flags  with the current constitutive law characteristics
                 * @param mStrainSize     double with the strain vector size
                 * @param mStrainMeasures vector with the strain measures accepted by the constitutive law
                */

                Flags                mOptions;
                double               mStrainSize;
                double               mSpaceDimension;
                std::vector< StrainMeasure > mStrainMeasures;

                /**
                 * Constructor.
                 */
                Features()
                {
                }

                /**
                 * Destructor.
                 */
                ~Features()
                {
                }

                // Set variables
                void SetOptions       (const Flags&  rOptions)      {mOptions=rOptions;};
                void SetStrainSize    (const double StrainSize)     {mStrainSize=StrainSize;};
                void SetSpaceDimension(const double SpaceDimension) {mSpaceDimension=SpaceDimension;};
                void SetStrainMeasure (const StrainMeasure Measure) {mStrainMeasures.push_back(Measure);};
                void SetStrainMeasures (const std::vector<StrainMeasure> MeasuresVector) {mStrainMeasures = MeasuresVector;};

                // Get variables
                const Flags& GetOptions () {return mOptions;};

                const double& GetStrainSize() {return mStrainSize;};
                const double& GetSpaceDimension() {return mSpaceDimension;};
                std::vector<StrainMeasure>& GetStrainMeasures() {return mStrainMeasures;};
            };

            struct Parameters
            {
                LOTUS_POINTER_DEFINE(Parameters);

            /**
             * Structure "Parameters" to be used by the element to pass the parameters into the constitutive law *
             * @param mOptions flags for the current Constitutive Law Parameters (input data)

            * KINEMATIC PARAMETERS:

            *** NOTE: Pointers are used only to point to a certain variable, no "new" or "malloc" can be used for this Parameters ***

            * @param mDeterminantF copy of the determinant of the Current DeformationGradient (although Current F  is also included as a matrix) (input data)
            * @param mpDeformationGradientF  pointer to the current deformation gradient (can be an empty matrix if a linear strain measure is used) (input data)
            * @param mpStrainVector pointer to the current strains (total strains) (input data) (*can be also OUTPUT with USE_ELEMENT_PROVIDED_STRAIN flag)
            * @param mpStressVector pointer to the current stresses (*OUTPUT with COMPUTE_STRESS flag)
            * @param mpConstitutiveMatrix pointer to the material tangent matrix (*OUTPUT with COMPUTE_CONSTITUTIVE_TENSOR flag)

            * GEOMETRIC PARAMETERS:
            * @param mpShapeFunctionsValues pointer to the shape functions values in the current integration point (input data)
            * @param mpShapeFunctionsDerivatives pointer to the shape functions derivatives values in the current integration point (input data)
            * @param mpElementGeometry pointer to the element's geometry (input data)

            * MATERIAL PROPERTIES:
            * @param mpMaterialProperties pointer to the material's Properties object (input data)

            * PROCESS PROPERTIES:
            * @param mpCurrentProcessInfo pointer to current ProcessInfo instance (input data)

            */


            private:

            /*** NOTE: Member Pointers are used only to point to a certain variable, no "new" or "malloc" can be used for this Parameters ***/

            Flags                                mOptions;
            double                               mDeterminantF;

            StrainVectorType*                    mpStrainVector;
            StressVectorType*                    mpStressVector;

            const Vector*                        mpShapeFunctionsValues;
            const Matrix*                        mpShapeFunctionsDerivatives;

            const DeformationGradientMatrixType* mpDeformationGradientF;
            VoigtSizeMatrixType*                 mpConstitutiveMatrix;

            const Process_Info*                  mpCurrentProcessInfo;
            const Properties*                    mpMaterialProperties;
            const GeometryType*                  mpElementGeometry;

            public:


            /**
             * Constructor.
             */
            Parameters ()
            {
                //Initialize pointers to NULL
                mDeterminantF=0;
                mpStrainVector=NULL;
                mpStressVector=NULL;
                mpShapeFunctionsValues=NULL;
                mpShapeFunctionsDerivatives=NULL;
                mpDeformationGradientF=NULL;
                mpConstitutiveMatrix=NULL;
                mpCurrentProcessInfo=NULL;
                mpMaterialProperties=NULL;
                mpElementGeometry=NULL;
            };

            /**
             * Constructor with Properties, Geometry and ProcessInfo
             */
            Parameters (
                const GeometryType& rElementGeometry,
                const Properties& rMaterialProperties,
                const Process_Info& rCurrentProcessInfo)
            :mpCurrentProcessInfo(&rCurrentProcessInfo)
            ,mpMaterialProperties(&rMaterialProperties)
            ,mpElementGeometry(&rElementGeometry)
            {
                //Initialize pointers to NULL
                mDeterminantF=0;
                mpStrainVector=NULL;
                mpStressVector=NULL;
                mpShapeFunctionsValues=NULL;
                mpShapeFunctionsDerivatives=NULL;
                mpDeformationGradientF=NULL;
                mpConstitutiveMatrix=NULL;
            };

            /**
             * Copy Constructor.
             */
            Parameters (const Parameters & rNewParameters)
                :mOptions(rNewParameters.mOptions)
                ,mDeterminantF(rNewParameters.mDeterminantF)
                ,mpStrainVector(rNewParameters.mpStrainVector)
                ,mpStressVector(rNewParameters.mpStressVector)
                ,mpShapeFunctionsValues(rNewParameters.mpShapeFunctionsValues)
                ,mpShapeFunctionsDerivatives(rNewParameters.mpShapeFunctionsDerivatives)
                ,mpDeformationGradientF(rNewParameters.mpDeformationGradientF)
                ,mpConstitutiveMatrix(rNewParameters.mpConstitutiveMatrix)
                ,mpCurrentProcessInfo(rNewParameters.mpCurrentProcessInfo)
                ,mpMaterialProperties(rNewParameters.mpMaterialProperties)
                ,mpElementGeometry(rNewParameters.mpElementGeometry)
            {
            };

            /**
             * Destructor.
             */
            ~Parameters()
            {
            }

            /**
             * Verify Parameters
             */
            bool CheckAllParameters ()
            {
                if(CheckMechanicalVariables() &&  CheckShapeFunctions() && CheckInfoMaterialGeometry ())
                    return 1;
                else
                    return 0;
            }
            bool CheckShapeFunctions ()
            {
                if(!mpShapeFunctionsValues)
                    LOTUS_ERROR("ShapeFunctionsValues NOT SET")
                if(!mpShapeFunctionsDerivatives)
                    LOTUS_ERROR("ShapeFunctionsDerivatives NOT SET")
                return 1;
            }
            bool CheckInfoMaterialGeometry ()
            {
                if(!mpCurrentProcessInfo)
                    LOTUS_ERROR("CurrentProcessInfo NOT SET")
                if(!mpMaterialProperties)
                    LOTUS_ERROR("MaterialProperties NOT SET")
                if(!mpElementGeometry)
                    LOTUS_ERROR("ElementGeometry NOT SET")
                return 1;
            }
            bool CheckMechanicalVariables ()
            {
                if(mDeterminantF<=0)
                    LOTUS_ERROR("DeterminantF NOT SET, value <= 0")
                if(!mpDeformationGradientF)
                    LOTUS_ERROR("DeformationGradientF NOT SET")
                if(!mpStrainVector)
                    LOTUS_ERROR("StrainVector NOT SET")
                if(!mpStressVector)
                    LOTUS_ERROR("StressVector NOT SET")
                if(!mpConstitutiveMatrix)
                    LOTUS_ERROR("ConstitutiveMatrix NOT SET")
                return 1;
            }

            /**
             * Public Methods to access variables of the struct class
             */

            /**
             * sets the variable or the pointer of a specified variable: assigns the direction of the pointer for the mpvariables, only non const values can be modified
             */

            void Set                             (Flags ThisFlag)                           {mOptions.Set(ThisFlag);};
            void Reset                           (Flags ThisFlag)                           {mOptions.Reset(ThisFlag);};

            void SetOptions                      (const Flags&  rOptions)                   {mOptions=rOptions;};
            void SetDeterminantF                 (const double DeterminantF)                {mDeterminantF=DeterminantF;};
            //void SetDeterminantF                 (const double& rDeterminantF)              {mDeterminantF=&rDeterminantF;};

            void SetShapeFunctionsValues         (const Vector& rShapeFunctionsValues)      {mpShapeFunctionsValues=&rShapeFunctionsValues;};
            void SetShapeFunctionsDerivatives    (const Matrix& rShapeFunctionsDerivatives) {mpShapeFunctionsDerivatives=&rShapeFunctionsDerivatives;};

            void SetDeformationGradientF         (const DeformationGradientMatrixType& rDeformationGradientF)      {mpDeformationGradientF=&rDeformationGradientF;};

            void SetStrainVector                 (StrainVectorType& rStrainVector)                       {mpStrainVector=&rStrainVector;};
            void SetStressVector                 (StressVectorType& rStressVector)                       {mpStressVector=&rStressVector;};
            void SetConstitutiveMatrix           (VoigtSizeMatrixType& rConstitutiveMatrix)              {mpConstitutiveMatrix =&rConstitutiveMatrix;};

            void SetProcessInfo                  (const Process_Info& rProcessInfo)          {mpCurrentProcessInfo =&rProcessInfo;};
            void SetMaterialProperties           (const Properties&  rMaterialProperties)   {mpMaterialProperties =&rMaterialProperties;};
            void SetElementGeometry              (const GeometryType& rElementGeometry)     {mpElementGeometry =&rElementGeometry;};

            /**
             * Returns the reference or the value of a specified variable: returns the value of the parameter, only non const values can be modified
             */
            Flags& GetOptions () {return mOptions;};

            const double& GetDeterminantF()
            {
                if(IsSetDeterminantF()) 
                {
                    std::cerr << "DeterminantF is not set!" << std::endl;
                    exit(0);
                }
                return mDeterminantF;
            }
            const Vector& GetShapeFunctionsValues()
            {
                if(IsSetShapeFunctionsValues())
                {
                   std::cerr << "ShapeFunctionsValues is not set!" << std::endl;
                   exit(0);
                } 
                return *mpShapeFunctionsValues;
            }
            const Matrix& GetShapeFunctionsDerivatives()
            {
                if(IsSetShapeFunctionsDerivatives())
                {
                    std::cerr << "ShapeFunctionsDerivatives is not set!" << std::endl;
                    exit(0);
                } 
                return *mpShapeFunctionsDerivatives;
            }
            const DeformationGradientMatrixType& GetDeformationGradientF()
            {
                if(IsSetDeformationGradientF())
                {
                    std::cerr << "DeformationGradientF is not set!" << std::endl;
                    exit(0);
                } 
                return *mpDeformationGradientF;
            }
            StrainVectorType& GetStrainVector()
            {
                if(!IsSetStrainVector())
                {
                    std::cerr << "StrainVector is not set!" << std::endl;
                    exit(0);
                } 
                return *mpStrainVector;
            }
            StressVectorType& GetStressVector()
            {
                if(!IsSetStressVector())
                {
                    std::cerr << "StressVector is not set!" << std::endl;
                    exit(0);
                } 
                return *mpStressVector;
            }
            VoigtSizeMatrixType& GetConstitutiveMatrix()
            {
                if(!IsSetConstitutiveMatrix())
                {
                    std::cerr << "ConstitutiveMatrix is not set!" << std::endl;
                    exit(0);
                } 
                return *mpConstitutiveMatrix;
            }

            const Process_Info& GetProcessInfo()
            {
                if(!IsSetProcessInfo())
                {
                    std::cerr << "ProcessInfo is not set!" << std::endl;
                    exit(0);
                } 
                return *mpCurrentProcessInfo;
            }
            const Properties& GetMaterialProperties()
            {
                if(!IsSetMaterialProperties())
                {
                    std::cerr << "MaterialProperties is not set!" << std::endl;
                    exit(0);
                } 
                return *mpMaterialProperties;
            }
            const GeometryType& GetElementGeometry()
            {
                if(!IsSetElementGeometry()) 
                {
                    std::cerr << "ElementGeometry is not set!" << std::endl;
                    exit(0);
                }
                return *mpElementGeometry;
            }

            /**
             * Returns the reference to the value of a specified variable with not constant access
             */

            double& GetDeterminantF                                (double & rDeterminantF) {rDeterminantF=mDeterminantF; return rDeterminantF;};
            StrainVectorType& GetStrainVector                      (StrainVectorType & rStrainVector) {rStrainVector=*mpStrainVector; return rStrainVector;};
            DeformationGradientMatrixType& GetDeformationGradientF (DeformationGradientMatrixType & rDeformationGradientF)  {rDeformationGradientF=*mpDeformationGradientF;   return rDeformationGradientF;};
            StressVectorType& GetStressVector                      (StressVectorType & rStressVector) {rStressVector=*mpStressVector; return rStressVector;};
            VoigtSizeMatrixType& GetConstitutiveMatrix             (VoigtSizeMatrixType & rConstitutiveMatrix) {rConstitutiveMatrix=*mpConstitutiveMatrix; return rConstitutiveMatrix;};

            /**
             * Returns if the different components has been set
             */

            bool IsSetDeterminantF              () {return (mDeterminantF > 0.0);};
            bool IsSetShapeFunctionsValues      () {return (mpShapeFunctionsValues != NULL);};
            bool IsSetShapeFunctionsDerivatives () {return (mpShapeFunctionsDerivatives != NULL);};
            bool IsSetDeformationGradientF      () {return (mpDeformationGradientF != NULL);};

            bool IsSetStrainVector              () {return (mpStrainVector != NULL);};
            bool IsSetStressVector              () {return (mpStressVector != NULL);};

            bool IsSetConstitutiveMatrix        () {return (mpConstitutiveMatrix != NULL);};

            bool IsSetProcessInfo               () {return (mpCurrentProcessInfo != NULL);};
            bool IsSetMaterialProperties        () {return (mpMaterialProperties != NULL);};
            bool IsSetElementGeometry           () {return (mpElementGeometry != NULL);};

            };// struct Parameters end
        /// @}


        /// @name Life Circle
        /// @{
            Constitutive_Law();
            ~Constitutive_Law() override{};
        /// @}

        /// @name Operators
        /// @{

        /// @}

        /// @name Operations
        /// @{
            /**
             * @brief Clone function (has to be implemented by any derived class)
             * @return a pointer to a new instance of this constitutive law
             * @note implementation scheme:
             *      Constitutive_Law::Pointer p_clone(new Constitutive_Law());
             *      return p_clone;
             */
            virtual Constitutive_Law::Pointer Clone() const;


            /**
             * This is to be called at the very beginning of the calculation
             * (e.g. from InitializeElement) in order to initialize all relevant
             * attributes of the constitutive law
             * @param rMaterialProperties the Properties instance of the current element
             * @param rElementGeometry the geometry of the current element
             * @param rShapeFunctionsValues the shape functions values in the current integration point
             */
            virtual void InitializeMaterial(const Properties& rMaterialProperties,
                                            const GeometryType& rElementGeometry,
                                            const Vector& rShapeFunctionsValues);
             /**
             * @return The initial state of strains/stresses/F
             */
            void SetInitialState(Initial_State::Pointer pInitialState)
            {
                mpInitialState = pInitialState;
            }



            /**
             * Computes the material response in terms of stresses and constitutive tensor
             * @see Parameters
             * @see StressMeasure
             */
            void CalculateMaterialResponse (Parameters& rValues,const StressMeasure& rStressMeasure);
            /**
             * Computes the material response in terms of 1st Piola-Kirchhoff stresses and constitutive tensor
             * @see Parameters
             */
            virtual void CalculateMaterialResponsePK1 (Parameters& rValues);
            /**
             * Computes the material response in terms of 2nd Piola-Kirchhoff stresses and constitutive tensor
             * @see Parameters
             */
            virtual void CalculateMaterialResponsePK2 (Parameters& rValues);
            /**
             * Computes the material response in terms of Kirchhoff stresses and constitutive tensor
             * @see Parameters
             */
            virtual void CalculateMaterialResponseKirchhoff (Parameters& rValues);
            /**
             * Computes the material response in terms of Cauchy stresses and constitutive tensor
             * @see Parameters
             */
            virtual void CalculateMaterialResponseCauchy (Parameters& rValues);

            
            /**
             * @brief Finalize the material response,  called by the element in FinalizeSolutionStep.
             * @see Parameters
             * @see StressMeasures
             */
            void FinalizeMaterialResponse(Parameters& rValues,const StressMeasure& rStressMeasure);
            /**
             * Finalize the material response in terms of 1st Piola-Kirchhoff stresses
             * @see Parameters
             */
            virtual void FinalizeMaterialResponsePK1 (Parameters& rValues);
            /**
             * @brief If the CL requires to finalize the material response, called by the element in FinalizeSolutionStep.
             */
            virtual bool RequiresFinalizeMaterialResponse()
            {
                return true;
            }
            /**
             * Finalize the material response in terms of 2nd Piola-Kirchhoff stresses
             * @see Parameters
             */
            virtual void FinalizeMaterialResponsePK2 (Parameters& rValues);
            /**
             * Finalize the material response in terms of Kirchhoff stresses
             * @see Parameters
             */
            virtual void FinalizeMaterialResponseKirchhoff (Parameters& rValues);
            /**
             * Finalize the material response in terms of Cauchy stresses
             * @see Parameters
             */
            virtual void FinalizeMaterialResponseCauchy (Parameters& rValues);
            /**
             * This function is designed to be called once to perform all the checks needed
             * on the input provided. Checks can be "expensive" as the function is designed
             * to catch user's errors.
             * @param rMaterialProperties
             * @param rElementGeometry
             * @param rCurrentProcessInfo
             * @return
             */
            virtual int Check(const Properties& rMaterialProperties,
                            const GeometryType& rElementGeometry,
                            const Process_Info& rCurrentProcessInfo) const;
        
            /**
             * @brief Adds the initial stress vector if it is defined in the InitialState
             */
            template<typename TVectorType>
            void AddInitialStressVectorContribution(TVectorType& rStressVector)
            {
                if (this->HasInitialState()) {
                    const auto& r_initial_state = GetInitialState();
                    noalias(rStressVector) += r_initial_state.GetInitialStressVector();
                }
            }
            /**
             * @brief Adds the initial strain vector if it is defined in the InitialState
             */
            template<typename TVectorType>
            void AddInitialStrainVectorContribution(TVectorType& rStrainVector)
            {
                if (this->HasInitialState()) {
                    const auto& r_initial_state = GetInitialState();
                    noalias(rStrainVector) -= r_initial_state.GetInitialStrainVector();
                }
            }
            /**
             * @brief Adds the initial strain vector if it is defined in the InitialState
             */
            template<typename TMatrixType>
            void AddInitialDeformationGradientMatrixContribution(TMatrixType& rF)
            {
                if (this->HasInitialState()) {
                    const auto& r_initial_state = GetInitialState();
                    rF = prod(r_initial_state.GetInitialDeformationGradientMatrix(), rF);
                }
            }

            /**
             * @brief Sets the value of a specified variable (boolean)
             * @param rVariable the variable to be returned
             * @param Value new value of the specified variable
             * @param rCurrentProcessInfo the process info
             */
            virtual void SetValue(const Variable<bool>& rVariable,
                                const bool& Value,
                                const Process_Info& rCurrentProcessInfo);
            /**
             * @brief Sets the value of a specified variable (integer)
             * @param rVariable the variable to be returned
             * @param Value new value of the specified variable
             * @param rCurrentProcessInfo the process info
             */
            virtual void SetValue(const Variable<int>& rVariable,
                                const int& Value,
                                const Process_Info& rCurrentProcessInfo);
            /**
             * @brief Sets the value of a specified variable (double)
             * @param rVariable the variable to be returned
             * @param rValue new value of the specified variable
             * @param rCurrentProcessInfo the process info
             */
            virtual void SetValue(const Variable<double>& rVariable,
                                const double& rValue,
                                const Process_Info& rCurrentProcessInfo);
            /**
             * @brief Sets the value of a specified variable (Vector)
             * @param rVariable the variable to be returned
             * @param rValue new value of the specified variable
             * @param rCurrentProcessInfo the process info
             */
            virtual void SetValue(const Variable<Vector >& rVariable,
                                const Vector& rValue,
                    const Process_Info& rCurrentProcessInfo);
            /**
             * @brief Sets the value of a specified variable (Matrix)
             * @param rVariable the variable to be returned
             * @param rValue new value of the specified variable
             * @param rCurrentProcessInfo the process info
             */
            virtual void SetValue(const Variable<Matrix >& rVariable,
                                const Matrix& rValue,
                    const Process_Info& rCurrentProcessInfo);
            /**
             * @brief Sets the value of a specified variable (array of 3 components)
             * @param rVariable the variable to be returned
             * @param rValue new value of the specified variable
             * @param rCurrentProcessInfo the process info
             */
            virtual void SetValue(const Variable<std::array<double, 3 > >& rVariable,
                                const std::array<double, 3 > & rValue,
                                const Process_Info& rCurrentProcessInfo);
            /**
             * @brief Sets the value of a specified variable (array of 6 components)
             * @param rVariable the variable to be returned
             * @param rValue new value of the specified variable
             * @param rCurrentProcessInfo the process info
             */
            virtual void SetValue(const Variable<std::array<double, 6 > >& rVariable,
                                const std::array<double, 6 > & rValue,
                                const Process_Info& rCurrentProcessInfo);

            /**
             * @brief Calculates the value of a specified variable (bool)
             * @param rParameterValues the needed parameters for the CL calculation
             * @param rThisVariable the variable to be returned
             * @param rValue a reference to the returned value
             * @param rValue output: the value of the specified variable
             */
            virtual bool& CalculateValue(Parameters& rParameterValues,
                                         const Variable<bool>& rThisVariable, 
                                         bool& rValue);
            /**
             * @brief Calculates the value of a specified variable (int)
             * @param rParameterValues the needed parameters for the CL calculation
             * @param rThisVariable the variable to be returned
             * @param rValue a reference to the returned value
             * @param rValue output: the value of the specified variable
             */
            virtual int& CalculateValue(Parameters& rParameterValues, 
                                        const Variable<int>& rThisVariable, 
                                        int& rValue);
            /**
             * @brief Calculates the value of a specified variable (double)
             * @param rParameterValues the needed parameters for the CL calculation
             * @param rThisVariable the variable to be returned
             * @param rValue a reference to the returned value
             * @param rValue output: the value of the specified variable
             */
            virtual double& CalculateValue(Parameters& rParameterValues,
                                           const Variable<double>& rThisVariable,
                                           double& rValue);
            /**
             * @brief Calculates the value of a specified variable (Vector)
             * @param rParameterValues the needed parameters for the CL calculation
             * @param rThisVariable the variable to be returned
             * @param rValue a reference to the returned value
             * @param rValue output: the value of the specified variable
             */
            virtual Vector& CalculateValue(Parameters& rParameterValues,
                                           const Variable<Vector>& rThisVariable, 
                                           Vector& rValue);
            /**
             * @brief Calculates the value of a specified variable (Matrix)
             * @param rParameterValues the needed parameters for the CL calculation
             * @param rThisVariable the variable to be returned
             * @param rValue a reference to the returned value
             * @param rValue output: the value of the specified variable
             */
            virtual Matrix& CalculateValue(Parameters& rParameterValues, 
                                           const Variable<Matrix>& rThisVariable, 
                                           Matrix& rValue);
            /**
             * @brief Calculates the value of a specified variable (array of 3 components)
             * @param rParameterValues the needed parameters for the CL calculation
             * @param rThisVariable the variable to be returned
             * @param rValue a reference to the returned value
             * @param rValue output: the value of the specified variable
             */
            virtual std::array<double, 3 > & CalculateValue(Parameters& rParameterValues, 
                                                            const Variable<std::array<double, 3 > >& rVariable,
                                                            std::array<double, 3 > & rValue);
            /**
             * returns the value of a specified variable (array of 6 components)
             * @param rThisVariable the variable to be returned
             * @param rValue a reference to the returned value
             * @return the value of the specified variable
             */
            virtual std::array<double, 6 > & CalculateValue(Parameters& rParameterValues, 
                                                            const Variable<std::array<double, 6 > >& rVariable,
                                                            std::array<double, 6 > & rValue);
            /**
             * @brief Calculates derivatives of a given function
             *
             * This method calculates derivative of a scalar function (denoted by rFunctionVariable) w.r.t.
             * rDerivativeVariable and stores the output in rOutput. The rDerivativeVariable represents
             * a gauss point scalar variable only.
             *
             * Eg: Assume following function (gauss point evaluated):
             *      \[
             *          \nu = \nu_{fluid} + \nu_t = \nu_{fluid} + \left(\frac{y}{\omega}\right)^2 \frac{\partial k}{\partial x_i}\frac{\partial \omega}{\partial x_i}
             *      \]
             *
             *      Then in here we use rFunctionVariable = EFFECTIVE_VISCOSITY
             *
             *      Then if we want to take derivatives w.r.t. $\omega$ (i.e. rDerivativeVariable = OMEGA).
             *      So following steps needs to be used.
             *
             *           1. First calculate derivatives w.r.t. omega (rDerivativeVariable = OMEGA)
             *              using the call:
             *                   CalculateDerivative(Values, EFFECTIVE_VISCOSITY, OMEGA, output);
             *              The output will hold the following:
             *                   \[
             *                       \frac{\partial \nu}{\partial \omega} = \frac{\partial \nu_t}{\partial \omega} = -2\frac{y^2}{\omega^3}\frac{\partial k}{\partial x_i}\frac{\partial \omega}{\partial x_i}
             *                   \]
             *           2. Then calculate derivatives w.r.t. omega gradients (rDerivativeVariable = OMEGA_GRADIENT_X)
             *              using the call: (where OMEGA_GRADIENT is a 3D vector with components)
             *                   CalculateDerivative(Values, EFFECTIVE_VISCOSITY, OMEGA_GRADIENT_X, output);
             *              The output will hold the following:
             *                   \[
             *                       \frac{\partial \nu}{\partial \nabla\omega_x} = \frac{\partial \nu_t}{\partial \nabla\omega_x} =  \left(\frac{y}{\omega}\right)^2 \frac{\partial k}{\partial x_x}
             *                   \]
             *              Once you have these outputs, you can transform it to a nodal derivative (eg: discrete adjoint computation)
             *              within your element by using the chain rule. [Where $c$ is the node index of the geometry.]
             *                   \[
             *                       \frac{\partial \nu}{\partial \omega^c} = \frac{\partial \nu}{\partial \omega}\frac{\partial \omega}{\partial \omega^c} + \frac{\partial \nu}{\partial \nabla\omega_i}\frac{\partial \nabla\omega_i}{\partial \omega^c}
             *                   \]
             *
             * @param rParameterValues      Input for the derivative calculation
             * @param rFunctionVariable     Variable to identify the function for which derivatives are computed
             * @param rDerivativeVariable   Scalar derivative variable
             * @param rOutput               Output having the same type as the rFunctionVariable
             */
            virtual void CalculateDerivative(Parameters& rParameterValues,
                                             const Variable<double>& rFunctionVariable,
                                             const Variable<double>& rDerivativeVariable,
                                             double& rOutput);
            /**
             * @brief Calculates derivatives of a given function
             *
             * This method calculates derivative of a Vector function (denoted by rFunctionVariable) w.r.t.
             * rDerivativeVariable and stores the output in rOutput. The rDerivativeVariable represents
             * a gauss point scalar variable only.
             *
             * @see double overload of this method for more explanations
             *
             * @param rParameterValues      Input for the derivative calculation
             * @param rFunctionVariable     Variable to identify the function for which derivatives are computed
             * @param rDerivativeVariable   Scalar derivative variable
             * @param rOutput               Output having the same type as the rFunctionVariable
             */
            virtual void CalculateDerivative(Parameters& rParameterValues,
                                             const Variable<Vector>& rFunctionVariable,
                                             const Variable<double>& rDerivativeVariable,
                                             Vector& rOutput);
            /**
             * @brief Calculates derivatives of a given function
             *
             * This method calculates derivative of a Matrix function (denoted by rFunctionVariable) w.r.t.
             * rDerivativeVariable and stores the output in rOutput. The rDerivativeVariable represents
             * a gauss point scalar variable only.
             *
             * @see double overload of this method for more explanations
             *
             * @param rParameterValues      Input for the derivative calculation
             * @param rFunctionVariable     Variable to identify the function for which derivatives are computed
             * @param rDerivativeVariable   Scalar derivative variable
             * @param rOutput               Output having the same type as the rFunctionVariable
             */
            virtual void CalculateDerivative(Parameters& rParameterValues,
                                             const Variable<Matrix>& rFunctionVariable,
                                             const Variable<double>& rDerivativeVariable,
                                             Matrix& rOutput);
            /**
             * @brief Calculates derivatives of a given function
             *
             * This method calculates derivative of a std::array<double, 3> function (denoted by rFunctionVariable) w.r.t.
             * rDerivativeVariable and stores the output in rOutput. The rDerivativeVariable represents
             * a gauss point scalar variable only.
             *
             * @see double overload of this method for more explanations
             *
             * @param rParameterValues      Input for the derivative calculation
             * @param rFunctionVariable     Variable to identify the function for which derivatives are computed
             * @param rDerivativeVariable   Scalar derivative variable
             * @param rOutput               Output having the same type as the rFunctionVariable
             */
            virtual void CalculateDerivative(Parameters& rParameterValues,
                                             const Variable<std::array<double, 3>>& rFunctionVariable,
                                             const Variable<double>& rDerivativeVariable,
                                             std::array<double, 3>& rOutput);

            /**
             * to be called at the beginning of each solution step
             * (e.g. from Element::InitializeSolutionStep)
             * @param rMaterialProperties the Properties instance of the current element
             * @param rElementGeometry the geometry of the current element
             * @param rShapeFunctionsValues the shape functions values in the current integration point
             * @param the current ProcessInfo instance
             */
            //("Please do not use this method - Use InitializeMaterialResponse instead\"")
            virtual void InitializeSolutionStep(const Properties& rMaterialProperties,
                                                const GeometryType& rElementGeometry, //this is just to give the array of nodes
                                                const Vector& rShapeFunctionsValues,
                                                const Process_Info& rCurrentProcessInfo);
            /**
             * to be called at the end of each solution step
             * (e.g. from Element::FinalizeSolutionStep)
             * @param rMaterialProperties the Properties instance of the current element
             * @param rElementGeometry the geometry of the current element
             * @param rShapeFunctionsValues the shape functions values in the current integration point
             * @param the current ProcessInfo instance
             */
            //("Please do not use this method - Use FinalizeMaterialResponse instead\"")
            virtual void FinalizeSolutionStep(const Properties& rMaterialProperties,
                                            const GeometryType& rElementGeometry,
                                            const Vector& rShapeFunctionsValues,
                                            const Process_Info& rCurrentProcessInfo);
            /**
             * to be called at the beginning of each step iteration
             * (e.g. from Element::InitializeNonLinearIteration)
             * @param rMaterialProperties the Properties instance of the current element
             * @param rElementGeometry the geometry of the current element
             * @param rShapeFunctionsValues the shape functions values in the current integration point
             * @param the current ProcessInfo instance
             */
            //("Please do not use this method - There is no equivalent for this\"")
            virtual void InitializeNonLinearIteration(const Properties& rMaterialProperties,
                                const GeometryType& rElementGeometry,
                                const Vector& rShapeFunctionsValues,
                                const Process_Info& rCurrentProcessInfo);
            /**
             * to be called at the end of each step iteration
             * (e.g. from Element::FinalizeNonLinearIteration)
             * @param rMaterialProperties the Properties instance of the current element
             * @param rElementGeometry the geometry of the current element
             * @param rShapeFunctionsValues the shape functions values in the current integration point
             * @param the current ProcessInfo instance
             */
            //("Please do not use this method - There is no equivalent for this\"")
            virtual void FinalizeNonLinearIteration(const Properties& rMaterialProperties,
                                const GeometryType& rElementGeometry,
                                const Vector& rShapeFunctionsValues,
                                const Process_Info& rCurrentProcessInfo);
            /**
             * @brief If the CL requires to initialize the material response, called by the element in InitializeSolutionStep.
             */
            virtual bool RequiresInitializeMaterialResponse()
            {
                return true;
            }
            /**
             * Computes the material response in terms of Cauchy stresses and constitutive tensor, returns internal variables.
             * @see Parameters
             */
            virtual void CalculateStressResponse (Parameters& rValues, Vector& rInternalVariables);
            /**
             * @brief Initialize the material response,  called by the element in InitializeSolutionStep.
             * @see Parameters
             * @see StressMeasures
             */
            void InitializeMaterialResponse (Parameters& rValues,const StressMeasure& rStressMeasure);
            /**
             * Initialize the material response in terms of 1st Piola-Kirchhoff stresses
             * @see Parameters
             */
            virtual void InitializeMaterialResponsePK1 (Parameters& rValues);
            /**
             * Initialize the material response in terms of 2nd Piola-Kirchhoff stresses
             * @see Parameters
             */
            virtual void InitializeMaterialResponsePK2 (Parameters& rValues);
            /**
             * Initialize the material response in terms of Kirchhoff stresses
             * @see Parameters
             */
            virtual void InitializeMaterialResponseKirchhoff (Parameters& rValues);
            /**
             * Initialize the material response in terms of Cauchy stresses
             * @see Parameters
             */

            virtual void InitializeMaterialResponseCauchy (Parameters& rValues);

            /**
             * This can be used in order to reset all internal variables of the
             * constitutive law (e.g. if a model should be reset to its reference state)
             * @param rMaterialProperties the Properties instance of the current element
             * @param rElementGeometry the geometry of the current element
             * @param rShapeFunctionsValues the shape functions values in the current integration point
             * @param the current ProcessInfo instance
             */
            virtual void ResetMaterial(const Properties& rMaterialProperties,
                                    const GeometryType& rElementGeometry,
                                    const Vector& rShapeFunctionsValues);
            /**
             * Methods to transform strain Vectors:
             * @param rStrainVector the strain tensor in matrix which its stress measure will be changed
             * @param rF the DeformationGradientF matrix between the configurations
             * @param rdetF the determinant of the DeformationGradientF matrix between the configurations
             * @param rStrainInitial the measure of stress of the given  rStrainVector
             * @param rStrainFinal the measure of stress of the returned rStrainVector
             */
            virtual Vector& TransformStrains (Vector& rStrainVector,
                            const Matrix &rF,
                            StrainMeasure rStrainInitial,
                            StrainMeasure rStrainFinal);
            /**
             * Methods to transform stress Matrices:
             * @param rStressMatrix the stress tensor in matrix which its stress measure will be changed
             * @param rF the DeformationGradientF matrix between the configurations
             * @param rdetF the determinant of the DeformationGradientF matrix between the configurations
             * @param rStressInitial the measure of stress of the given  rStressMatrix
             * @param rStressFinal the measure of stress of the returned rStressMatrix
             */
            virtual Matrix& TransformStresses (Matrix& rStressMatrix,
                            const Matrix &rF,
                            const double &rdetF,
                            StressMeasure rStressInitial,
                            StressMeasure rStressFinal);
            /**
             * Methods to transform stress Vectors:
             * @param rStressVector the stress tensor in matrix which its stress measure will be changed
             * @param rF the DeformationGradientF matrix between the configurations
             * @param rdetF the determinant of the DeformationGradientF matrix between the configurations
             * @param rStressInitial the measure of stress of the given  rStressVector
             * @param rStressFinal the measure of stress of the returned rStressVector
             */
            virtual Vector& TransformStresses (Vector& rStressVector,
                            const Matrix &rF,
                            const double &rdetF,
                            StressMeasure rStressInitial,
                            StressMeasure rStressFinal);
            /**
             * Methods to transform stress Vectors specialized with the initial stress Measure PK1:
             * @param rStressVector the stress tensor in matrix which its stress measure will be changed
             * @param rF the DeformationGradientF matrix between the configurations
             * @param rdetF the determinant of the DeformationGradientF matrix between the configurations
             * @param rStressFinal the measure of stress of the returned rStressVector
             */
            Vector& TransformPK1Stresses (Vector& rStressVector,
                        const Matrix &rF,
                        const double &rdetF,
                        StressMeasure rStressFinal);
            /**
             * Methods to transform stress Vectors specialized with the initial stress Measure PK2:
             * @param rStressVector the stress tensor in matrix which its stress measure will be changed
             * @param rF the DeformationGradientF matrix between the configurations
             * @param rdetF the determinant of the DeformationGradientF matrix between the configurations
             * @param rStressFinal the measure of stress of the returned rStressVector
             */
            Vector& TransformPK2Stresses (Vector& rStressVector,
                        const Matrix &rF,
                        const double &rdetF,
                        StressMeasure rStressFinal);
            /**
             * Methods to transform stress Vectors specialized with the initial stress Measure Kirchhoff:
             * @param rStressVector the stress tensor in matrix which its stress measure will be changed
             * @param rF the DeformationGradientF matrix between the configurations
             * @param rdetF the determinant of the DeformationGradientF matrix between the configurations
             * @param rStressFinal the measure of stress of the returned rStressVector
             */
            Vector& TransformKirchhoffStresses (Vector& rStressVector,
                            const Matrix &rF,
                            const double &rdetF,
                            StressMeasure rStressFinal);
            /**
             * Methods to transform stress Vectors specialized with the initial stress Measure Cauchy:
             * @param rStressVector the stress tensor in matrix which its stress measure will be changed
             * @param rF the DeformationGradientF matrix between the configurations
             * @param rdetF the determinant of the DeformationGradientF matrix between the configurations
             * @param rStressFinal the measure of stress of the returned rStressVector
             */
            Vector& TransformCauchyStresses (Vector& rStressVector,
                            const Matrix &rF,
                            const double &rdetF,
                            StressMeasure rStressFinal);
            /**
             * Methods to transform Constitutive Matrices:
             * @param rConstitutiveMatrix the constitutive matrix
             * @param rF the DeformationGradientF matrix between the configurations
             */
            /**
             * This method performs a pull-back of the constitutive matrix
             */
            void PullBackConstitutiveMatrix ( Matrix& rConstitutiveMatrix,
                            const Matrix & rF );
            /**
             * This method performs a push-forward of the constitutive matrix
             */
            void PushForwardConstitutiveMatrix ( Matrix& rConstitutiveMatrix,
                            const Matrix & rF );

            virtual void CalculateCauchyStresses(Vector& Cauchy_StressVector,
                                                 const Matrix& F,
                                                 const Vector& PK2_StressVector,
                                                 const Vector& GreenLagrangeStrainVector);


        /// @}

        /// @name Access
        /// @{
            /**
             * @return The size of the strain vector of the current constitutive law
             * @note This function HAS TO BE IMPLEMENTED by any derived class
             */
            virtual SizeType GetStrainSize() const;
            /**
             * @return The working space dimension of the current constitutive law
             * @note This function HAS TO BE IMPLEMENTED by any derived class
             */
            virtual SizeType WorkingSpaceDimension();
            /**
             * This function is designed to be called once to check compatibility with element
             * @param rFeatures
             */
            virtual void GetLawFeatures(Features& rFeatures);
            /**
             * @return The initial state of strains/stresses/F
             */
            Initial_State::Pointer pGetInitialState()
            {
                return mpInitialState;
            }
            /**
             * @return The reference to initial state of strains/stresses/F
             */
            Initial_State& GetInitialState()
            {
                return *mpInitialState;
            }

             /**
             * @brief Returns the value of a specified variable (boolean)
             * @param rThisVariable the variable to be returned
             * @param rValue a reference to the returned value
             * @return rValue output: the value of the specified variable
             */
            virtual bool& GetValue(const Variable<bool>& rThisVariable, bool& rValue);
            /**
             * Returns the value of a specified variable (integer)
             * @param rThisVariable the variable to be returned
             * @param rValue a reference to the returned value
             * @return rValue output: the value of the specified variable
             */
            virtual int& GetValue(const Variable<int>& rThisVariable, int& rValue);
            /**
             * @brief Returns the value of a specified variable (double)
             * @param rThisVariable the variable to be returned
             * @param rValue a reference to the returned value
             * @return rValue output: the value of the specified variable
             */
            virtual double& GetValue(const Variable<double>& rThisVariable, double& rValue);
            /**
             * @brief Returns the value of a specified variable (Vector)
             * @param rThisVariable the variable to be returned
             * @param rValue a reference to the returned value
             * @return rValue output: the value of the specified variable
             */
            virtual Vector& GetValue(const Variable<Vector>& rThisVariable, Vector& rValue);
            /**
             * @brief Returns the value of a specified variable (Matrix)
             * @param rThisVariable the variable to be returned
             * @return rValue output: the value of the specified variable
             */
            virtual Matrix& GetValue(const Variable<Matrix>& rThisVariable, Matrix& rValue);
            /**
             * @brief Returns the value of a specified variable (array of 3 components)
             * @param rThisVariable the variable to be returned
             * @param rValue a reference to the returned value
             * @return rValue output: the value of the specified variable
             */
            virtual std::array<double, 3 > & GetValue(const Variable<std::array<double, 3 > >& rThisVariable,
                                                    std::array<double, 3 > & rValue);
            /**
             * @brief Returns the value of a specified variable (array of 6 components)
             * @param rThisVariable the variable to be returned
             * @param rValue a reference to the returned value
             * @return the value of the specified variable
             */
            virtual std::array<double, 6 > & GetValue(const Variable<std::array<double, 6 > >& rThisVariable,
                                                    std::array<double, 6 > & rValue);

            

        /// @}  


        /// @name Inquiry
        /// @{
            /**
             * @brief Returns whether this constitutive Law has specified variable (boolean)
             * @param rThisVariable the variable to be checked for
             * @return true if the variable is defined in the constitutive law
             */
            virtual bool Has(const Variable<bool>& rThisVariable);
            /**
             * @brief Returns whether this constitutive Law has specified variable (integer)
             * @param rThisVariable the variable to be checked for
             * @return true if the variable is defined in the constitutive law
             */
            virtual bool Has(const Variable<int>& rThisVariable);
            /**
             * @brief Returns whether this constitutive Law has specified variable (double)
             * @param rThisVariable the variable to be checked for
             * @return true if the variable is defined in the constitutive law
             */
            virtual bool Has(const Variable<double>& rThisVariable);
            /**
             * @brief Returns whether this constitutive Law has specified variable (Vector)
             * @param rThisVariable the variable to be checked for
             * @return true if the variable is defined in the constitutive law
             */
            virtual bool Has(const Variable<Vector>& rThisVariable);
            /**
             * @brief Returns whether this constitutive Law has specified variable (Matrix)
             * @param rThisVariable the variable to be checked for
             * @return true if the variable is defined in the constitutive law
             */
            virtual bool Has(const Variable<Matrix>& rThisVariable);
            /**
             * @brief Returns whether this constitutive Law has specified variable (array of 3 components)
             * @param rThisVariable the variable to be checked for
             * @return true if the variable is defined in the constitutive law
             * @note Fixed size array of 3 doubles (e.g. for 2D stresses, plastic strains, ...)
             */
            virtual bool Has(const Variable<std::array<double, 3 > >& rThisVariable);
            /**
             * @brief Returns whether this constitutive Law has specified variable (array of 6 components)
             * @param rThisVariable the variable to be checked for
             * @return true if the variable is defined in the constitutive law
             * @note Fixed size array of 6 doubles (e.g. for stresses, plastic strains, ...)
             */
            virtual bool Has(const Variable<std::array<double, 6 > >& rThisVariable);
            /**
             * @return The true if InitialState is defined
             */
            bool HasInitialState() const
            {
                return mpInitialState != nullptr;
            }
             /**
             * Is called to check whether the provided material parameters in the Properties
             * match the requirements of current constitutive model.
             * @param rMaterialProperties the current Properties to be validated against.
             * @return true, if parameters are correct; false, if parameters are insufficient / faulty
             * NOTE: this has to be implemented by each constitutive model. Returns false in base class since
             * no valid implementation is contained here.
             */
            virtual bool ValidateInput(const Properties& rMaterialProperties);
            /**
             * returns the expected strain measure of this constitutive law (by default linear strains)
             * @return the expected strain measure
             */
            virtual StrainMeasure GetStrainMeasure();
            /**
             * returns the stress measure of this constitutive law (by default 1st Piola-Kirchhoff stress in voigt notation)
             * @return the expected stress measure
             */
            virtual StressMeasure GetStressMeasure();
             /**
             * returns whether this constitutive model is formulated in incremental strains/stresses
             * NOTE: by default, all constitutive models should be formulated in total strains
             * @return true, if formulated in incremental strains/stresses, false otherwise
             */
            virtual bool IsIncremental();

            /**
             * @brief This method is used to check that two Constitutive Laws are the same type (references)
             * @param rLHS The first argument
             * @param rRHS The second argument
             */
            inline static bool HasSameType(const Constitutive_Law& rLHS, const Constitutive_Law& rRHS) 
            {
                return (typeid(rLHS) == typeid(rRHS));
            }
            /**
             * @brief This method is used to check that two Constitutive Laws are the same type (pointers)
             * @param rLHS The first argument
             * @param rRHS The second argument
             */
            inline static bool HasSameType(const Constitutive_Law* rLHS, const Constitutive_Law* rRHS) 
            {
                return Constitutive_Law::HasSameType(*rLHS, *rRHS);
            }


        /// @}

        /// @name Input And Output
        /// @{
            std::string Info() const override
            {
                std::stringstream buffer;
                buffer << "ConstitutiveLaw";
                return buffer.str();
            }
            void PrintInfo(std::ostream& rOStream) const override
            {
                rOStream << "ConstitutiveLaw";
            }
            void PrintData(std::ostream& rOStream) const override
            {
            rOStream << "ConstitutiveLaw has no data";
            }

        /// @}
    protected:
        /// @name Protected Static Member Variables
        /// @{
            static const unsigned int msIndexVoigt3D6C [6][2];
            static const unsigned int msIndexVoigt2D4C [4][2];
            static const unsigned int msIndexVoigt2D3C [3][2];
        /// @}


        /// @name Protected Member Variables
        /// @{
            

        /// @}


        /// @name Protected Operatiors
        /// @{


        /// @}


        /// @name Protected Operations
        /// @{
            /**
             * This method performs a contra-variant push-forward between to tensors
             * i.e. 2nd PK stress to Kirchhoff stress
             */
            void ContraVariantPushForward( Matrix& rMatrix,
                                           const Matrix& rF );
            /**
             * This method performs a contra-variant pull-back between to tensors
             * i.e. Kirchhoff stress to 2nd PK stress
             */
            void ContraVariantPullBack( Matrix& rMatrix,
                                        const Matrix& rF );
            /**
             * This method performs a co-variant push-forward between to tensors
             * i.e. Green-Lagrange strain to Almansi strain
             */
            void CoVariantPushForward( Matrix& rMatrix,
                                       const Matrix& rF );
            /**
             * This method performs a co-variant pull-back between to tensors
             * i.e. Almansi strain to Green-Lagrange strain
             */
            void CoVariantPullBack( Matrix& rMatrix,
                                    const Matrix& rF );
            /**
             * This method performs a pull-back or a push-forward between two constitutive matrices
             */
            void ConstitutiveMatrixTransformation ( Matrix& rConstitutiveMatrix,
                                                    const Matrix& rOriginalConstitutiveMatrix,
                                                    const Matrix & rF );
            /**
             * This method performs a pull-back or a push-forward between two constitutive tensor components
             */
            double& TransformConstitutiveComponent(double & rCabcd,
                                                   const Matrix & rConstitutiveMatrix,
                                                   const Matrix & rF,
                                                   const unsigned int& a, const unsigned int& b,
                                                   const unsigned int& c, const unsigned int& d);
            /**
             * This method gets the constitutive tensor components
             * from a consitutive matrix supplied in voigt notation
             */
            double& GetConstitutiveComponent(double & rCabcd,
                                             const Matrix& rConstitutiveMatrix,
                                             const unsigned int& a, const unsigned int& b,
                                             const unsigned int& c, const unsigned int& d);


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
            Initial_State::Pointer mpInitialState = nullptr;

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
extern template class Lotus_Components<Constitutive_Law >;
extern template class Lotus_Components<Variable<Constitutive_Law::Pointer> >;

void AddComponent(std::string const& Name, Constitutive_Law const& ThisComponent);
void AddComponent(std::string const& Name, Variable<Constitutive_Law::Pointer> const& ThisComponent);

LOTUS_DEFINE_VARIABLE(Constitutive_Law::Pointer, CONSTITUTIVE_LAW)
#endif