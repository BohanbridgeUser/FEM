#ifndef _CONSTITUTIVE_LAW_H_
#define _CONSTITUTIVE_LAW_H_
#include "../define.h"
#include "../Container/flags.h"
#include "../Process/process.h"
#include "../Geometry/geometry.h"
#include "../Node/node.h"
#include "../Linear_Solver/dense_space.h"
#include "../Linear_Solver/sparse_space.h"
#include "../process_info.h"
#include "../Property/properties.h"

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
            typedef typename SparseSpace::SparseMatrix 
                                                        VoigtSizeMatrixType;           // Constitutive Matrix
            typedef typename SparseSpace::SparseMatrix 
                                               DeformationGradientMatrixType; // Def. gradient tensor
            LOTUS_POINTER_DEFINE(Constitutive_Law)

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
             *      ConstitutiveLaw::Pointer p_clone(new ConstitutiveLaw());
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
        /// @}

        /// @name Access
        /// @{
            /**
             * @return The size of the strain vector of the current constitutive law
             * @note This function HAS TO BE IMPLEMENTED by any derived class
             */
            virtual SizeType GetStrainSize() const;
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
LOTUS_DEFINE_VARIABLE(Constitutive_Law::Pointer, CONSTITUTIVE_LAW)
#endif