#include "../../include/Constitutive_Law/Constitutive_law.h"

const unsigned int Constitutive_Law::msIndexVoigt3D6C [6][2] = { {0, 0}, {1, 1}, {2, 2}, {0, 1}, {1, 2}, {0, 2} };
const unsigned int Constitutive_Law::msIndexVoigt2D4C [4][2] = { {0, 0}, {1, 1}, {2, 2}, {0, 1} };
const unsigned int Constitutive_Law::msIndexVoigt2D3C [3][2] = { {0, 0}, {1, 1}, {0, 1} };

/// @brief public:
/// @name Type Define
/// @{
    /**
     * Flags related to the Parameters of the Constitutive Law
     */
    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, USE_ELEMENT_PROVIDED_STRAIN,  0 );
    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, COMPUTE_STRESS,               1 );
    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, COMPUTE_CONSTITUTIVE_TENSOR,  2 );
    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, COMPUTE_STRAIN_ENERGY,        3 );

    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, ISOCHORIC_TENSOR_ONLY,        4 );
    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, VOLUMETRIC_TENSOR_ONLY,       5 );

    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, MECHANICAL_RESPONSE_ONLY,     6 );
    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, THERMAL_RESPONSE_ONLY,        7 );
    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, INCREMENTAL_STRAIN_MEASURE,   8 );

    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, INITIALIZE_MATERIAL_RESPONSE, 9 );
    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, FINALIZE_MATERIAL_RESPONSE,  10 );


    /**
     * Flags related to the Features of the Constitutive Law
     */
    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, FINITE_STRAINS,              1 );
    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, INFINITESIMAL_STRAINS,       2 );

    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, THREE_DIMENSIONAL_LAW,       3 );
    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, PLANE_STRAIN_LAW,            4 );
    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, PLANE_STRESS_LAW,            5 );
    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, AXISYMMETRIC_LAW,            6 );

    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, U_P_LAW,                     7 );
    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, ISOTROPIC,                   8 );
    LOTUS_CREATE_LOCAL_FLAGS( Constitutive_Law, ANISOTROPIC,                 9 );


/// @}


/// @name Life Circle
/// @{
    /**
     * Constructor.
     */
    Constitutive_Law::Constitutive_Law() : Flags()
    {
    }



/// @}


/// @name Operators
/// @{


/// @}


/// @name Operations
/// @{
    /**
     * Clone function (has to be implemented by any derived class)
     * @return a pointer to a new instance of this constitutive law
     * NOTE: implementation scheme:
     *      Constitutive_Law::Pointer p_clone(new Constitutive_Law());
     *      return p_clone;
     */
    Constitutive_Law::Pointer Constitutive_Law::Clone() const
    {
        std::cerr <<  "Called the virtual function for Clone"<< std::endl;
        exit(0);
    }

    void Constitutive_Law::InitializeMaterial(const Properties& rMaterialProperties,
                                            const GeometryType& rElementGeometry,
                                            const Vector& rShapeFunctionsValues)
    {

    }

    /**
     * Computes the material response in terms of stresses and constitutive tensor
     * @see Parameters
     * @see StressMeasures
     */
    void Constitutive_Law::CalculateMaterialResponse(Parameters& rValues,const StressMeasure& rStressMeasure)
    {
        switch(rStressMeasure)
        {
        case StressMeasure_PK1:
            CalculateMaterialResponsePK1(rValues);
            break;

        case StressMeasure_PK2:
            CalculateMaterialResponsePK2(rValues);
            break;

        case StressMeasure_Kirchhoff:
            CalculateMaterialResponseKirchhoff(rValues);
            break;

        case StressMeasure_Cauchy:
            CalculateMaterialResponseCauchy(rValues);
            break;

        default:
            std::cerr <<  " Stress Measure not Defined "<< std::endl;
            exit(0);
            break;

        }
    }
    /**
     * Computes the material response in terms of 1st Piola-Kirchhoff stresses and constitutive tensor
     * @see Parameters
     */
    void Constitutive_Law::CalculateMaterialResponsePK1 (Parameters& rValues)
    {
        std::cerr <<  "Calling virtual function for CalculateMaterialResponsePK1"<< std::endl;
        exit(0);
    }
    /**
     * Computes the material response in terms of 2nd Piola-Kirchhoff stresses and constitutive tensor
     * @see Parameters
     */
    void Constitutive_Law::CalculateMaterialResponsePK2 (Parameters& rValues)
    {
        std::cerr <<  "Calling virtual function for CalculateMaterialResponsePK2"<< std::endl;
        exit(0);
    }
    /**
     * Computes the material response in terms of Kirchhoff stresses and constitutive tensor
     * @see Parameters
     */
    void Constitutive_Law::CalculateMaterialResponseKirchhoff (Parameters& rValues)
    {
        std::cerr <<  "Calling virtual function for CalculateMaterialResponseKirchhoff"<< std::endl;
        exit(0);
    }
    /**
     * Computes the material response in terms of Cauchy stresses and constitutive tensor
     * @see Parameters
     */
    void Constitutive_Law::CalculateMaterialResponseCauchy (Parameters& rValues)
    {
        std::cerr <<  "Calling virtual function for CalculateMaterialResponseCauchy"<< std::endl;
        exit(0);
    }

    /**
     * @brief Updates the material response,  called by the element in FinalizeSolutionStep.
     * @see Parameters
     * @see StressMeasures
     */
    void Constitutive_Law::FinalizeMaterialResponse(Parameters& rValues,const StressMeasure& rStressMeasure)
    {
        switch(rStressMeasure)
        {
        case StressMeasure_PK1:
            FinalizeMaterialResponsePK1(rValues);
            break;

        case StressMeasure_PK2:
            FinalizeMaterialResponsePK2(rValues);
            break;

        case StressMeasure_Kirchhoff:
            FinalizeMaterialResponseKirchhoff(rValues);
            break;

        case StressMeasure_Cauchy:
            FinalizeMaterialResponseCauchy(rValues);
            break;

        default:
            std::cerr <<  " Stress Measure not Defined "<< std::endl;
            exit(0);
            break;
        }
    }
    void Constitutive_Law::FinalizeMaterialResponsePK1 (Parameters& rValues)
    {
        if(this->RequiresFinalizeMaterialResponse())
        {
            std::cerr<<  "Calling virtual function for FinalizeMaterialResponsePK1. Please implement FinalizeMaterialResponsePK1 or RequiresFinalizeMaterialResponse in case this CL does not require it" << std::endl;
            exit(0);
        } 
    }

    /**
     * Updates the material response in terms of 2nd Piola-Kirchhoff stresses
     * @see Parameters
     */
    void Constitutive_Law::FinalizeMaterialResponsePK2 (Parameters& rValues)
    {
        if(this->RequiresFinalizeMaterialResponse())
        {
            std::cerr<<  "Calling virtual function for FinalizeMaterialResponsePK2. Please implement FinalizeMaterialResponsePK2 or RequiresFinalizeMaterialResponse in case this CL does not require it" << std::endl;
            exit(0);
        } 
    }
    /**
     * Updates the material response in terms of Kirchhoff stresses
     * @see Parameters
     */
    void Constitutive_Law::FinalizeMaterialResponseKirchhoff (Parameters& rValues)
    {
        if(this->RequiresFinalizeMaterialResponse())
        {
            std::cerr<<  "Calling virtual function for FinalizeMaterialResponseKirchhoff. Please implement FinalizeMaterialResponseKirchhoff or RequiresFinalizeMaterialResponse in case this CL does not require it" << std::endl;
            exit(0);
        } 
    }
    /**
     * Updates the material response in terms of Cauchy stresses
     * @see Parameters
     */
    void Constitutive_Law::FinalizeMaterialResponseCauchy (Parameters& rValues)
    {
        if(this->RequiresFinalizeMaterialResponse())
        {
            std::cerr <<  "Calling virtual function for FinalizeMaterialResponseCauchy. Please implement FinalizeMaterialResponseCauchy or RequiresFinalizeMaterialResponse in case this CL does not require it" << std::endl;
            exit(0);
        } 
    }

    /**
     * This function is designed to be called once to perform all the checks needed
     * on the input provided. Checks can be "expensive" as the function is designed
     * to catch user's errors.
     * @param rMaterialProperties
     * @param rElementGeometry
     * @param rCurrentProcessInfo
     * @return
     */
    int Constitutive_Law::Check(const Properties& rMaterialProperties,
                            const GeometryType& rElementGeometry,
                            const Process_Info& rCurrentProcessInfo) const
    {
        return 0;
    }

    
    /**
     * @brief Sets the value of a specified variable (bool)
     * @param rThisVariable the variable to be returned
     * @param Value new value of the specified variable
     * @param rCurrentProcessInfo the process info
     */
    void Constitutive_Law::SetValue(const Variable<bool>& rThisVariable,
                                const bool& Value,
                                const Process_Info& rCurrentProcessInfo)
    {
        std::cerr <<  "Called the virtual function for SetValue"<< std::endl;
        exit(0);
    }
    /**
     * @brief Sets the value of a specified variable (int)
     * @param rThisVariable the variable to be returned
     * @param Value new value of the specified variable
     * @param rCurrentProcessInfo the process info
     */
    void Constitutive_Law::SetValue(const Variable<int>& rThisVariable,
                                const int& Value,
                                const Process_Info& rCurrentProcessInfo)
    {
        std::cerr <<  "Called the virtual function for SetValue"<< std::endl;
        exit(0);
    }
    /**
     * @brief Sets the value of a specified variable (double)
     * @param rVariable the variable to be returned
     * @param rValue new value of the specified variable
     * @param rCurrentProcessInfo the process info
     */
    void Constitutive_Law::SetValue(const Variable<double>& rVariable,
                                const double& rValue,
                                const Process_Info& rCurrentProcessInfo)
    {
        std::cerr <<  "Called the virtual function for SetValue"<< std::endl;
        exit(0);
    }
    /**
     * @brief Sets the value of a specified variable (Vector)
     * @param rVariable the variable to be returned
     * @param rValue new value of the specified variable
     * @param rCurrentProcessInfo the process info
     */
    void Constitutive_Law::SetValue(const Variable<Vector >& rVariable,
                                const Vector& rValue, const Process_Info& rCurrentProcessInfo)
    {
        std::cerr <<  "Called the virtual function for SetValue"<< std::endl;
        exit(0);
    }
    /**
     * @brief Sets the value of a specified variable (Matrix)
     * @param rVariable the variable to be returned
     * @param rValue new value of the specified variable
     * @param rCurrentProcessInfo the process info
     */
    void Constitutive_Law::SetValue(const Variable<Matrix >& rVariable,
                                const Matrix& rValue, const Process_Info& rCurrentProcessInfo)
    {
        std::cerr <<  "Called the virtual function for SetValue"<< std::endl;
        exit(0);
    }
    /**
     * @brief Sets the value of a specified variable (array of 3 components)
     * @param rVariable the variable to be returned
     * @param rValue new value of the specified variable
     * @param rCurrentProcessInfo the process info
     */
    void Constitutive_Law::SetValue(const Variable<std::array<double, 3 > >& rVariable,
                                const std::array<double, 3 > & rValue,
                                const Process_Info& rCurrentProcessInfo)
    {
        std::cerr <<  "Called the virtual function for SetValue"<< std::endl;
        exit(0);
    }
    /**
     * @brief Sets the value of a specified variable (array of 6 components)
     * @param rVariable the variable to be returned
     * @param rValue new value of the specified variable
     * @param rCurrentProcessInfo the process info
     */
    void Constitutive_Law::SetValue(const Variable<std::array<double, 6 > >& rVariable,
                                const std::array<double, 6 > & rValue,
                                const Process_Info& rCurrentProcessInfo)
    {
        std::cerr <<  "Called the virtual function for SetValue"<< std::endl;
        exit(0);
    }
    /**
     * @brief Calculates the value of a specified variable (bool)
     * @param rParameterValues the needed parameters for the CL calculation
     * @param rThisVariable the variable to be returned
     * @param rValue a reference to the returned value
     * @param rValue output: the value of the specified variable
     */
    bool& Constitutive_Law::CalculateValue(Parameters& rParameterValues, const Variable<bool>& rThisVariable, bool& rValue)
    {
        return rValue;
    }
    /**
     * @brief Calculates the value of a specified variable (int)
     * @param rParameterValues the needed parameters for the CL calculation
     * @param rThisVariable the variable to be returned
     * @param rValue a reference to the returned value
     * @param rValue output: the value of the specified variable
     */
    int& Constitutive_Law::CalculateValue(Parameters& rParameterValues, const Variable<int>& rThisVariable, int& rValue)
    {
        return rValue;
    }
    /**
     * returns the value of a specified variable (double)
     * @param rParameterValues the needed parameters for the CL calculation
     * @param rThisVariable the variable to be returned
     * @param rValue a reference to the returned value
     * @param rValue output: the value of the specified variable
     */
    double& Constitutive_Law::CalculateValue(Parameters& rParameterValues, const Variable<double>& rThisVariable, double& rValue)
    {
        return rValue;
    }
    /**
     * returns the value of a specified variable (Vector)
     * @param rParameterValues the needed parameters for the CL calculation
     * @param rThisVariable the variable to be returned
     * @param rValue a reference to the returned value
     * @param rValue output: the value of the specified variable
     */
    typename Constitutive_Law::Vector& Constitutive_Law::CalculateValue(Parameters& rParameterValues, const Variable<Vector>& rThisVariable, Vector& rValue)
    {
        return rValue;
    }
    /**
     * returns the value of a specified variable (Matrix)
     * @param rParameterValues the needed parameters for the CL calculation
     * @param rThisVariable the variable to be returned
     * @param rValue a reference to the returned value
     * @param rValue output: the value of the specified variable
     */
    typename Constitutive_Law::Matrix& Constitutive_Law::CalculateValue(Parameters& rParameterValues, const Variable<Matrix>& rThisVariable, Matrix& rValue)
    {
        return rValue;
    }
    /**
     * returns the value of a specified variable (array of 3 components)
     * @param rParameterValues the needed parameters for the CL calculation
     * @param rThisVariable the variable to be returned
     * @param rValue a reference to the returned value
     * @param rValue output: the value of the specified variable
     */
    std::array<double, 3 > & Constitutive_Law::CalculateValue(Parameters& rParameterValues, const Variable<std::array<double, 3 > >& rVariable,
            std::array<double, 3 > & rValue)
    {
        return rValue;
    }
    /**
     * returns the value of a specified variable (array of 6 components)
     * @param rParameterValues the needed parameters for the CL calculation
     * @param rThisVariable the variable to be returned
     * @param rValue a reference to the returned value
     * @param rValue output: the value of the specified variable
     */
    std::array<double, 6 > & Constitutive_Law::CalculateValue(Parameters& rParameterValues, const Variable<std::array<double, 6 > >& rVariable,
            std::array<double, 6 > & rValue)
    {
        return rValue;
    }
    void Constitutive_Law::CalculateDerivative(
        Parameters& rParameterValues,
        const Variable<double>& rFunctionVariable,
        const Variable<double>& rDerivativeVariable,
        double& rOutput)
    {
        std::cerr << "Derivative of " << rFunctionVariable.Name() << " w.r.t. " << rDerivativeVariable.Name() << " is not implemented.\n";
        exit(0);
    }
    void Constitutive_Law::CalculateDerivative(
        Parameters& rParameterValues,
        const Variable<Vector>& rFunctionVariable,
        const Variable<double>& rDerivativeVariable,
        Vector& rOutput)
    {
        std::cerr << "Derivative of " << rFunctionVariable.Name() << " w.r.t. " << rDerivativeVariable.Name() << " is not implemented.\n";
        exit(0);
    }
    void Constitutive_Law::CalculateDerivative(
        Parameters& rParameterValues,
        const Variable<Matrix>& rFunctionVariable,
        const Variable<double>& rDerivativeVariable,
        Matrix& rOutput)
    {
        std::cerr << "Derivative of " << rFunctionVariable.Name() << " w.r.t. " << rDerivativeVariable.Name() << " is not implemented.\n";
        exit(0);
    }
    void Constitutive_Law::CalculateDerivative(
        Parameters& rParameterValues,
        const Variable<std::array<double, 3>>& rFunctionVariable,
        const Variable<double>& rDerivativeVariable,
        std::array<double, 3>& rOutput)
    {
         

        std::cerr << "Derivative of " << rFunctionVariable.Name() << " w.r.t. " << rDerivativeVariable.Name() << " is not implemented.\n";
        exit(0);
    }


    /**
     * Computes the material response in terms of Cauchy stresses and constitutive tensor
     * @see Parameters
     */
    void Constitutive_Law::CalculateStressResponse (Parameters& rValues, Vector& rInternalVariables)
    {
        std::cerr <<  "Calling virtual function for CalculateStressResponse"<< std::endl;;
    }
    /**
     * @brief Initialize the material response,  called by the element in InitializeSolutionStep.
     * @see Parameters
     * @see StressMeasures
     */
    void Constitutive_Law::InitializeMaterialResponse(Parameters& rValues,const StressMeasure& rStressMeasure)
    {
        switch(rStressMeasure)
        {
            case StressMeasure_PK1:         InitializeMaterialResponsePK1(rValues);
            break;

            case StressMeasure_PK2:         InitializeMaterialResponsePK2(rValues);
            break;

            case StressMeasure_Kirchhoff: 	InitializeMaterialResponseKirchhoff(rValues);
            break;

            case StressMeasure_Cauchy:	InitializeMaterialResponseCauchy(rValues);
            break;

            default:
            std::cerr << " Stress Measure not Defined " << std::endl;
            break;
        }
    }
    /**
     * @brief Initialize the material response in terms of 1st Piola-Kirchhoff stresses
     * @see Parameters
     */
    void Constitutive_Law::InitializeMaterialResponsePK1 (Parameters& rValues)
    {
        if(this->RequiresInitializeMaterialResponse())
        {
            std::cerr <<  "Calling virtual function for InitializeMaterialResponsePK1. Please implement InitializeMaterialResponsePK1 or RequiresInitializeMaterialResponse in case this CL does not require it" << std::endl;
            exit(0);
        } 
    }
    /**
     * @brief Initialize the material response in terms of 2nd Piola-Kirchhoff stresses
     * @see Parameters
     */
    void Constitutive_Law::InitializeMaterialResponsePK2 (Parameters& rValues)
    {
        if(this->RequiresInitializeMaterialResponse())
        {
           std::cerr <<  "Calling virtual function for InitializeMaterialResponsePK2. Please implement InitializeMaterialResponsePK2 or RequiresInitializeMaterialResponse in case this CL does not require it" << std::endl;
            exit(0) ;
        } 
    }
    /**
     * @brief Initialize the material response in terms of Kirchhoff stresses
     * @see Parameters
     */
    void Constitutive_Law::InitializeMaterialResponseKirchhoff (Parameters& rValues)
    {
        if(this->RequiresInitializeMaterialResponse())
        {
            std::cerr <<  "Calling virtual function for InitializeMaterialResponseKirchhoff. Please implement InitializeMaterialResponseKirchhoff or RequiresInitializeMaterialResponse in case this CL does not require it" << std::endl;
            exit(0); 
        } 
    }
    /**
     * @brief Initialize the material response in terms of Cauchy stresses
     * @see Parameters
     */
    void Constitutive_Law::InitializeMaterialResponseCauchy (Parameters& rValues)
    {
        if(this->RequiresInitializeMaterialResponse())
        {
            std::cerr <<  "Calling virtual function for InitializeMaterialResponseCauchy. Please implement InitializeMaterialResponseCauchy or RequiresInitializeMaterialResponse in case this CL does not require it" << std::endl;
            exit(0);
        } 
    }
    /**
     * This can be used in order to reset all internal variables of the
     * constitutive law (e.g. if a model should be reset to its reference state)
     * @param rMaterialProperties the Properties instance of the current element
     * @param rElementGeometry the geometry of the current element
     * @param rShapeFunctionsValues the shape functions values in the current integration point
     * @param the current ProcessInfo instance
     */
    void Constitutive_Law::ResetMaterial(const Properties& rMaterialProperties,
                                        const GeometryType& rElementGeometry,
                                        const Vector& rShapeFunctionsValues)
    {
        std::cerr <<  "Calling virtual function for ResetMaterial"<< std::endl;;
    }

    /**
     * Methods to transform strain Vectors:
     * @param rStrainVector the strain tensor in matrix which its stress measure will be changed
     * @param rF the DeformationGradientF matrix between the configurations
     * @param rdetF the determinant of the DeformationGradientF matrix between the configurations
     * @param rStrainInitial the measure of stress of the given  rStrainVector
     * @param rStrainFinal the measure of stress of the returned rStrainVector
     */
    typename Constitutive_Law::Vector& Constitutive_Law::TransformStrains (Vector& rStrainVector,
                                                                           const Matrix &rF,
                                                                           StrainMeasure rStrainInitial,
                                                                           StrainMeasure rStrainFinal)
    {

        switch(rStrainInitial)
        {
        case StrainMeasure_GreenLagrange:

            switch(rStrainFinal)
            {
            case StrainMeasure_GreenLagrange:
                break;

            case StrainMeasure_Almansi:
            {
                Matrix StrainMatrix = StrainVectorToTensor( rStrainVector );

                CoVariantPushForward (StrainMatrix,rF);  //Almansi

                rStrainVector = StrainTensorToVector( StrainMatrix, rStrainVector.size() );
            }
            break;

            case StrainMeasure_Hencky_Material:
                std::cerr << "Hencky strain has no transformation coded"<< std::endl;;
                break;

            case StrainMeasure_Hencky_Spatial:
                std::cerr << "Hencky strain has no transformation coded"<< std::endl;;
                break;

            default:
                std::cerr << "FINAL STRAIN NOT DEFINED in StrainTransformation"<< std::endl;;
                break;
            }

            break;

        case StrainMeasure_Almansi:

            switch(rStrainFinal)
            {
            case StrainMeasure_GreenLagrange:
            {
                Matrix StrainMatrix = StrainVectorToTensor( rStrainVector );

                CoVariantPullBack (StrainMatrix,rF);  //GreenLagrange

                rStrainVector = StrainTensorToVector( StrainMatrix, rStrainVector.size() );
            }
            break;

            case StrainMeasure_Almansi:
                break;

            case StrainMeasure_Hencky_Material:
                std::cerr << "Hencky strain has no transformation coded"<< std::endl;;
                break;

            case StrainMeasure_Hencky_Spatial:
                std::cerr << "Hencky strain has no transformation coded"<< std::endl;;
                break;

            default:
                std::cerr << "FINAL STRAIN NOT DEFINED in StrainTransformation"<< std::endl;;
                break;
            }

            break;

        case StrainMeasure_Hencky_Material:
            std::cerr << "Hencky strain has no transformation coded"<< std::endl;;
            break;

        case StrainMeasure_Hencky_Spatial:
            std::cerr << "Hencky strain has no transformation coded"<< std::endl;;
            break;

        default:
            std::cerr << "Measure of strain NOT DEFINED in Strains Transformation"<< std::endl;;
            break;
        }


        return rStrainVector;

    }
    /**
     * Methods to transform stress Matrices:
     * @param rStressMatrix the stress tensor in matrix which its stress measure will be changed
     * @param rF the DeformationGradientF matrix between the configurations
     * @param rdetF the determinant of the DeformationGradientF matrix between the configurations
     * @param rStressInitial the measure of stress of the given  rStressMatrix
     * @param rStressFinal the measure of stress of the returned rStressMatrix
     */
    typename Constitutive_Law::Matrix& Constitutive_Law::TransformStresses (Matrix& rStressMatrix,
                                                                            const Matrix &rF,
                                                                            const double &rdetF,
                                                                            StressMeasure rStressInitial,
                                                                            StressMeasure rStressFinal)
    {
        Vector StressVector;

        StressVector = StressTensorToVector( rStressMatrix );

        StressVector=TransformStresses(StressVector,rF,rdetF,rStressInitial,rStressFinal);

        rStressMatrix = StressVectorToTensor( StressVector );

        return rStressMatrix;
    }
    /**
     * Methods to transform stress Vectors:
     * @param rStressVector the stress tensor in matrix which its stress measure will be changed
     * @param rF the DeformationGradientF matrix between the configurations
     * @param rdetF the determinant of the DeformationGradientF matrix between the configurations
     * @param rStressInitial the measure of stress of the given  rStressVector
     * @param rStressFinal the measure of stress of the returned rStressVector
     */
    typename Constitutive_Law::Vector& Constitutive_Law::TransformStresses (Vector& rStressVector,
                                                                            const Matrix &rF,
                                                                            const double &rdetF,
                                                                            StressMeasure rStressInitial,
                                                                            StressMeasure rStressFinal)
    {

        switch(rStressInitial)
        {
        case StressMeasure_PK1:

            TransformPK1Stresses(rStressVector,rF,rdetF,rStressFinal);

            break;

        case StressMeasure_PK2:

            TransformPK2Stresses(rStressVector,rF,rdetF,rStressFinal);

            break;

        case StressMeasure_Kirchhoff:

            TransformKirchhoffStresses(rStressVector,rF,rdetF,rStressFinal);

            break;

        case StressMeasure_Cauchy:

            TransformCauchyStresses(rStressVector,rF,rdetF,rStressFinal);

            break;

        default:
            std::cerr << "INITIAL STRESS NOT DEFINED in StressTransformation"<< std::endl;;
            break;
        }


        return rStressVector;

    }
    /**
     * Methods to transform stress Vectors specialized with the initial stress Measure PK1:
     * @param rStressVector the stress tensor in matrix which its stress measure will be changed
     * @param rF the DeformationGradientF matrix between the configurations
     * @param rdetF the determinant of the DeformationGradientF matrix between the configurations
     * @param rStressFinal the measure of stress of the returned rStressVector
     */
    typename Constitutive_Law::Vector& Constitutive_Law::TransformPK1Stresses (Vector& rStressVector,
                                                                               const Matrix &rF,
                                                                               const double &rdetF,
                                                                               StressMeasure rStressFinal)
    {
        unsigned int size = rF.rows(); //WorkingSpaceDimension();
        
        switch(rStressFinal)
        {
        case StressMeasure_PK1:
            break;

        case StressMeasure_PK2:
        {
            Matrix StressMatrix = StressVectorToTensor( rStressVector );
            Matrix InvF ( size, size );
            double J;
            InvF = rF.inverse();
            StressMatrix = InvF * StressMatrix;

            rStressVector = StressTensorToVector( StressMatrix, rStressVector.size() );
        }
        break;

        case StressMeasure_Kirchhoff:
        {
            Matrix StressMatrix = StressVectorToTensor( rStressVector );
            Matrix InvF ( size, size );
            double J;
            InvF = rF.inverse();

            StressMatrix = InvF * StressMatrix; //PK2

            ContraVariantPushForward (StressMatrix,rF); //Kirchhoff

            rStressVector = StressTensorToVector( StressMatrix, rStressVector.size() );
        }
        break;

        case StressMeasure_Cauchy:
        {
            Matrix StressMatrix = StressVectorToTensor( rStressVector );
            Matrix InvF ( size, size );
            double J;
            InvF = rF.inverse();

            StressMatrix = InvF * StressMatrix; //PK2

            ContraVariantPushForward (StressMatrix,rF); //Kirchhoff

            StressMatrix/=J; //Cauchy

            rStressVector = StressTensorToVector( StressMatrix, rStressVector.size() );
        }
        break;

        default:
            std::cerr << "FINAL STRESS NOT DEFINED in StressTransformation"<< std::endl;;
            break;
        }
        return rStressVector;
    }
    /**
     * Methods to transform stress Vectors specialized with the initial stress Measure PK2:
     * @param rStressVector the stress tensor in matrix which its stress measure will be changed
     * @param rF the DeformationGradientF matrix between the configurations
     * @param rdetF the determinant of the DeformationGradientF matrix between the configurations
     * @param rStressFinal the measure of stress of the returned rStressVector
     */
    typename Constitutive_Law::Vector& Constitutive_Law::TransformPK2Stresses (Vector& rStressVector,
                                                                               const Matrix &rF,
                                                                               const double &rdetF,
                                                                               StressMeasure rStressFinal)
    {

        switch(rStressFinal)
        {
        case StressMeasure_PK1:
        {
            Matrix StressMatrix = StressVectorToTensor( rStressVector );

            StressMatrix = rF * StressMatrix; //PK1

            rStressVector = StressTensorToVector( StressMatrix, rStressVector.size() );
        }
        break;

        case StressMeasure_PK2:
            break;

        case StressMeasure_Kirchhoff:
        {
            Matrix StressMatrix = StressVectorToTensor( rStressVector );

            ContraVariantPushForward (StressMatrix,rF); //Kirchhoff

            rStressVector = StressTensorToVector( StressMatrix, rStressVector.size() );
        }
        break;

        case StressMeasure_Cauchy:
        {

            Matrix StressMatrix = StressVectorToTensor( rStressVector );

            ContraVariantPushForward (StressMatrix,rF); //Kirchhoff

            if(rdetF!=0)
                StressMatrix/=rdetF; //Cauchy

            rStressVector = StressTensorToVector( StressMatrix, rStressVector.size() );

        }
        break;

        default:
            std::cerr << "FINAL STRESS NOT DEFINED in StressTransformation"<< std::endl;;
            break;
        }

        return rStressVector;

    }
    /**
     * Methods to transform stress Vectors specialized with the initial stress Measure Kirchooff:
     * @param rStressVector the stress tensor in matrix which its stress measure will be changed
     * @param rF the DeformationGradientF matrix between the configurations
     * @param rdetF the determinant of the DeformationGradientF matrix between the configurations
     * @param rStressFinal the measure of stress of the returned rStressVector
     */
    typename Constitutive_Law::Vector& Constitutive_Law::TransformKirchhoffStresses (Vector& rStressVector,
                                                                                     const Matrix &rF,
                                                                                     const double &rdetF,
                                                                                     StressMeasure rStressFinal)
    {

        switch(rStressFinal)
        {
        case StressMeasure_PK1:
        {
            Matrix StressMatrix = StressVectorToTensor( rStressVector );

            ContraVariantPullBack (StressMatrix,rF);  //PK2

            StressMatrix = rF * StressMatrix; //PK1

            rStressVector = StressTensorToVector( StressMatrix, rStressVector.size() );
        }
        break;

        case StressMeasure_PK2:
        {
            Matrix StressMatrix = StressVectorToTensor( rStressVector );

            ContraVariantPullBack (StressMatrix,rF);  //PK2

            rStressVector = StressTensorToVector( StressMatrix, rStressVector.size() );
        }
        break;

        case StressMeasure_Kirchhoff:
            break;

        case StressMeasure_Cauchy:
        {
            if(rdetF!=0)
                rStressVector/=rdetF; //Cauchy
        }
        break;

        default:
            std::cerr << "FINAL STRESS NOT DEFINED in StressTransformation"<< std::endl;;
            break;
        }

        return rStressVector;

    }
    /**
     * Methods to transform stress Vectors specialized with the initial stress Measure Cauchy:
     * @param rStressVector the stress tensor in matrix which its stress measure will be changed
     * @param rF the DeformationGradientF matrix between the configurations
     * @param rdetF the determinant of the DeformationGradientF matrix between the configurations
     * @param rStressFinal the measure of stress of the returned rStressVector
     */
    typename Constitutive_Law::Vector& Constitutive_Law::TransformCauchyStresses (Vector& rStressVector,
                                                                                  const Matrix &rF,
                                                                                  const double &rdetF,
                                                                                  StressMeasure rStressFinal)
    {

        switch(rStressFinal)
        {
        case StressMeasure_PK1:
        {
            rStressVector*=rdetF; //Kirchhoff

            Matrix StressMatrix = StressVectorToTensor( rStressVector );

            ContraVariantPullBack (StressMatrix,rF);  //PK2

            StressMatrix = rF * StressMatrix; //PK1

            rStressVector = StressTensorToVector( StressMatrix, rStressVector.size() );
        }
        break;

        case StressMeasure_PK2:
        {
            rStressVector*=rdetF; //Kirchhoff

            Matrix StressMatrix = StressVectorToTensor( rStressVector );

            ContraVariantPullBack (StressMatrix,rF);  //PK2

            rStressVector = StressTensorToVector( StressMatrix, rStressVector.size() );
        }
        break;

        case StressMeasure_Kirchhoff:

            rStressVector*=rdetF; //Kirchhoff

            break;

        case StressMeasure_Cauchy:
            break;

        default:
            std::cerr << "FINAL STRESS NOT DEFINED in StressTransformation"<< std::endl;;
            break;
        }

        return rStressVector;

    }
    /**
     * Methods to transform Constitutive Matrices:
     * @param rConstitutiveMatrix the constitutive matrix
     * @param rF the DeformationGradientF matrix between the configurations
     */
    /**
     * This method performs a pull-back of the constitutive matrix
     */
    void Constitutive_Law::PullBackConstitutiveMatrix ( Matrix& rConstitutiveMatrix,
                                                        const Matrix & rF )
    {
        Matrix OriginalConstitutiveMatrix = rConstitutiveMatrix;

        rConstitutiveMatrix.setZero();

        Matrix InverseF ( 3, 3 );
        double detF = 0;
        InverseF = rF.inverse();

        ConstitutiveMatrixTransformation( rConstitutiveMatrix, OriginalConstitutiveMatrix, InverseF );
    }
    /**
     * This method performs a push-forward of the constitutive matrix
     */
    void Constitutive_Law::PushForwardConstitutiveMatrix ( Matrix& rConstitutiveMatrix,
                                                           const Matrix & rF )
    {
        Matrix OriginalConstitutiveMatrix = rConstitutiveMatrix;

        rConstitutiveMatrix.setZero();

        ConstitutiveMatrixTransformation( rConstitutiveMatrix, OriginalConstitutiveMatrix, rF );
    }
    // VM
    void Constitutive_Law::CalculateCauchyStresses(Vector& Cauchy_StressVector,
                                                   const Matrix& F,
                                                   const Vector& PK2_StressVector,
                                                   const Vector& GreenLagrangeStrainVector)
    {
    }

    /**
     * to be called at the beginning of each solution step
     * (e.g. from Element::InitializeSolutionStep)
     * @param rMaterialProperties the Properties instance of the current element
     * @param rElementGeometry the geometry of the current element
     * @param rShapeFunctionsValues the shape functions values in the current integration point
     * @param the current ProcessInfo instance
     */
    void Constitutive_Law::InitializeSolutionStep(const Properties& rMaterialProperties,
            const GeometryType& rElementGeometry, //this is just to give the array of nodes
            const Vector& rShapeFunctionsValues,
            const Process_Info& rCurrentProcessInfo)
    {
    }

    /**
     * to be called at the end of each solution step
     * (e.g. from Element::FinalizeSolutionStep)
     * @param rMaterialProperties the Properties instance of the current element
     * @param rElementGeometry the geometry of the current element
     * @param rShapeFunctionsValues the shape functions values in the current integration point
     * @param the current ProcessInfo instance
     */
    void Constitutive_Law::FinalizeSolutionStep(const Properties& rMaterialProperties,
            const GeometryType& rElementGeometry,
            const Vector& rShapeFunctionsValues,
            const Process_Info& rCurrentProcessInfo)
    {
    }
    /**
     * to be called at the beginning of each step iteration
     * (e.g. from Element::InitializeNonLinearIteration)
     * @param rMaterialProperties the Properties instance of the current element
     * @param rElementGeometry the geometry of the current element
     * @param rShapeFunctionsValues the shape functions values in the current integration point
     * @param the current ProcessInfo instance
     */
    void Constitutive_Law::InitializeNonLinearIteration(const Properties& rMaterialProperties,
            const GeometryType& rElementGeometry,
            const Vector& rShapeFunctionsValues,
            const Process_Info& rCurrentProcessInfo)
    {
    }
    /**
     * to be called at the end of each step iteration
     * (e.g. from Element::FinalizeNonLinearIteration)
     * @param rMaterialProperties the Properties instance of the current element
     * @param rElementGeometry the geometry of the current element
     * @param rShapeFunctionsValues the shape functions values in the current integration point
     * @param the current ProcessInfo instance
     */
    void Constitutive_Law::FinalizeNonLinearIteration(const Properties& rMaterialProperties,
            const GeometryType& rElementGeometry,
            const Vector& rShapeFunctionsValues,
            const Process_Info& rCurrentProcessInfo)
    {

    }


/// @}


/// @name Access
/// @{
    /**
     * returns the size of the strain vector of the current constitutive law
     * NOTE: this function HAS TO BE IMPLEMENTED by any derived class
     */
    Constitutive_Law::SizeType Constitutive_Law::GetStrainSize() const
    {
        std::cerr <<  "Called the virtual function for GetStrainSize"<< std::endl;
        exit(0);
    }

    /**
     * This function is designed to be called once to check compatibility with element
     * @param rFeatures
     */
    void Constitutive_Law::GetLawFeatures(Features& rFeatures)
    {

        std::cerr <<  "Calling virtual function for GetConstitutive_LawFeatures"<< std::endl;
        exit(0);
    }

    /**
     * returns the value of a specified variable
     * @param rThisVariable the variable to be returned
     * @param rValue a reference to the returned value
     * @param rValue output: the value of the specified variable
     */
    bool& Constitutive_Law::GetValue(const Variable<bool>& rThisVariable, bool& rValue)
    {
        return rValue;
    }
    /**
     * returns the value of a specified variable
     * @param rThisVariable the variable to be returned
     * @param rValue a reference to the returned value
     * @param rValue output: the value of the specified variable
     */
    int& Constitutive_Law::GetValue(const Variable<int>& rThisVariable, int& rValue)
    {
        return rValue;
    }
    /**
     * returns the value of a specified variable
     * @param rThisVariable the variable to be returned
     * @param rValue a reference to the returned value
     * @param rValue output: the value of the specified variable
     */
    double& Constitutive_Law::GetValue(const Variable<double>& rThisVariable, double& rValue)
    {
        return rValue;
    }
    /**
     * returns the value of a specified variable
     * @param rThisVariable the variable to be returned
     * @param rValue a reference to the returned value
     * @return the value of the specified variable
     */
    typename Constitutive_Law::Vector& Constitutive_Law::GetValue(const Variable<Vector>& rThisVariable, Vector& rValue)
    {
        return rValue;
    }
    /**
     * returns the value of a specified variable
     * @param rThisVariable the variable to be returned
     * @return the value of the specified variable
     */
    typename Constitutive_Law::Matrix& Constitutive_Law::GetValue(const Variable<Matrix>& rThisVariable, Matrix& rValue)
    {
        return rValue;
    }
    /**
     * returns the value of a specified variable
     * @param rThisVariable the variable to be returned
     * @param rValue a reference to the returned value
     * @return the value of the specified variable
     */
    std::array<double, 3 > & Constitutive_Law::GetValue(const Variable<std::array<double, 3 > >& rThisVariable,
            std::array<double, 3 > & rValue)
    {
        return rValue;
    }
    /**
     * returns the value of a specified variable
     * @param rThisVariable the variable to be returned
     * @param rValue a reference to the returned value
     * @return the value of the specified variable
     */
    std::array<double, 6 > & Constitutive_Law::GetValue(const Variable<std::array<double, 6 > >& rThisVariable,
            std::array<double, 6 > & rValue)
    {
        return rValue;
    }
    
/// @}


/// @name Inquiry
/// @{
    
    /**
     * @return the working space dimension of the current constitutive law
     * NOTE: this function HAS TO BE IMPLEMENTED by any derived class
     */
    Constitutive_Law::SizeType Constitutive_Law::WorkingSpaceDimension()
    {
        std::cerr <<  "Called the virtual function for WorkingSpaceDimension"<< std::endl;
        exit(0);
    }

     /**
     * returns whether this constitutive Law has specified variable
     * @param rThisVariable the variable to be checked for
     * @return true if the variable is defined in the constitutive law
     */
    bool Constitutive_Law::Has(const Variable<bool>& rThisVariable)
    {
        return false;
    }
    /**
     * returns whether this constitutive Law has specified variable
     * @param rThisVariable the variable to be checked for
     * @return true if the variable is defined in the constitutive law
     */
    bool Constitutive_Law::Has(const Variable<int>& rThisVariable)
    {
        return false;
    }
    /**
     * returns whether this constitutive Law has specified variable
     * @param rThisVariable the variable to be checked for
     * @return true if the variable is defined in the constitutive law
     */
    bool Constitutive_Law::Has(const Variable<double>& rThisVariable)
    {
        return false;
    }
    /**
     * returns whether this constitutive Law has specified variable
     * @param rThisVariable the variable to be checked for
     * @return true if the variable is defined in the constitutive law
     */
    bool Constitutive_Law::Has(const Variable<Vector>& rThisVariable)
    {
        return false;
    }
    /**
     * returns whether this constitutive Law has specified variable
     * @param rThisVariable the variable to be checked for
     * @return true if the variable is defined in the constitutive law
     */
    bool Constitutive_Law::Has(const Variable<Matrix>& rThisVariable)
    {
        return false;
    }
    /**
     * returns whether this constitutive Law has specified variable
     * @param rThisVariable the variable to be checked for
     * @return true if the variable is defined in the constitutive law
     * NOTE: fixed size array of 3 doubles (e.g. for 2D stresses, plastic strains, ...)
     */
    bool Constitutive_Law::Has(const Variable<std::array<double, 3 > >& rThisVariable)
    {
        return false;
    }
    /**
     * returns whether this constitutive Law has specified variable
     * @param rThisVariable the variable to be checked for
     * @return true if the variable is defined in the constitutive law
     * NOTE: fixed size array of 6 doubles (e.g. for stresses, plastic strains, ...)
     */
    bool Constitutive_Law::Has(const Variable<std::array<double, 6 > >& rThisVariable)
    {
        return false;
    }

    /**
     * Is called to check whether the provided material parameters in the Properties
     * match the requirements of current constitutive model.
     * @param rMaterialProperties the current Properties to be validated against.
     * @return true, if parameters are correct; false, if parameters are insufficient / faulty
     * NOTE: this has to implemented by each constitutive model. Returns false in base class since
     * no valid implementation is contained here.
     */
    bool Constitutive_Law::ValidateInput(const Properties& rMaterialProperties)
    {
        return false;
    }
    /**
     * returns the expected strain measure of this constitutive law (by default linear strains)
     * @return the expected strain measure
     */
    Constitutive_Law::StrainMeasure Constitutive_Law::GetStrainMeasure()
    {
        return StrainMeasure_Infinitesimal;
    }
    /**
     * returns the stress measure of this constitutive law (by default 1st Piola-Kirchhoff stress in voigt notation)
     * @return the expected stress measure
     */
    Constitutive_Law::StressMeasure Constitutive_Law::GetStressMeasure()
    {
        return StressMeasure_PK1;
    }
    /**
     * returns whether this constitutive model is formulated in incremental strains/stresses
     * NOTE: by default, all constitutive models should be formulated in total strains
     * @return true, if formulated in incremental strains/stresses, false otherwise
     */
    bool Constitutive_Law::IsIncremental()
    {
        return false;
    }

/// @}


/// @brief protected:
/// @name Protected Static Member Variables
/// @{


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
        void Constitutive_Law::ContraVariantPushForward( Matrix& rMatrix,
                                                         const Matrix& rF)  //i.e. 2nd PK stress to Kirchhoff stress
        {
            unsigned int size = rF.rows(); //WorkingSpaceDimension();
            Matrix temp ( size, size );

            temp       = rF * rMatrix;
            rMatrix    = temp * rF.transpose();
        }

        /**
         * This method performs a contra-variant pull-back between to tensors
         * i.e. Kirchhoff stress to 2nd PK stress
         */
        void Constitutive_Law::ContraVariantPullBack( Matrix& rMatrix,
                                                      const Matrix& rF)     //i.e. Kirchhoff stress to 2nd PK stress
        {
            unsigned int size = rF.rows(); //WorkingSpaceDimension();
            Matrix InvF ( size, size );
            double J;
            InvF = rF.inverse();

            Matrix temp ( size, size );

            temp      = InvF * rMatrix ;
            rMatrix   = temp * InvF.transpose();
        }
        /**
         * This method performs a co-variant push-forward between to tensors
         * i.e. Green-Lagrange strain to Almansi strain
         */
        void Constitutive_Law::CoVariantPushForward( Matrix& rMatrix,
                                                     const Matrix& rF)      //i.e. Green-Lagrange strain to Almansi strain
        {
            unsigned int size = rF.rows(); //WorkingSpaceDimension();
            Matrix InvF ( size, size );
            double J;
            InvF = rF.inverse();

            Matrix temp ( size, size );

            temp       = InvF.transpose() * rMatrix;
            rMatrix    = temp * InvF ;
        }
        /**
         * This method performs a co-variant pull-back between to tensors
         * i.e. Almansi strain to Green-Lagrange strain
         */
        void Constitutive_Law::CoVariantPullBack( Matrix& rMatrix,
                                                  const Matrix& rF)         //i.e. Almansi strain to Green-Lagrange strain
        {

            unsigned int size = rF.rows(); //WorkingSpaceDimension();
            Matrix temp ( size, size );

            temp       = rF.transpose()  * rMatrix ;
            rMatrix    = temp * rF;

        }
        /**
         * This method performs a pull-back or a push-forward between two constitutive matrices
         */
        void Constitutive_Law::ConstitutiveMatrixTransformation ( Matrix& rConstitutiveMatrix,
                                                                  const Matrix& rOriginalConstitutiveMatrix,
                                                                  const Matrix & rF )
        {
            unsigned int size = rOriginalConstitutiveMatrix.rows();
            if(  size == 6 )
            {

                for(unsigned int i=0; i<6; i++)
                {
                    for(unsigned int j=0; j<6; j++)
                    {
                        rConstitutiveMatrix( i, j ) = TransformConstitutiveComponent(rConstitutiveMatrix( i, j ), rOriginalConstitutiveMatrix, rF,
                                                    this->msIndexVoigt3D6C[i][0], this->msIndexVoigt3D6C[i][1], this->msIndexVoigt3D6C[j][0], this->msIndexVoigt3D6C[j][1]);
                    }

                }
            }
            else if( size == 4 )
            {


                for(unsigned int i=0; i<4; i++)
                {
                    for(unsigned int j=0; j<4; j++)
                    {
                        rConstitutiveMatrix( i, j ) = TransformConstitutiveComponent(rConstitutiveMatrix( i, j ), rOriginalConstitutiveMatrix, rF,
                                                    this->msIndexVoigt2D4C[i][0], this->msIndexVoigt2D4C[i][1], this->msIndexVoigt2D4C[j][0], this->msIndexVoigt2D4C[j][1]);
                    }

                }
            }
            else if( size == 3 )
            {


                for(unsigned int i=0; i<3; i++)
                {
                    for(unsigned int j=0; j<3; j++)
                    {
                        rConstitutiveMatrix( i, j ) = TransformConstitutiveComponent(rConstitutiveMatrix( i, j ), rOriginalConstitutiveMatrix, rF,
                                                    this->msIndexVoigt2D3C[i][0], this->msIndexVoigt2D3C[i][1], this->msIndexVoigt2D3C[j][0], this->msIndexVoigt2D3C[j][1]);
                    }

                }
            }


        }
        /**
         * This method performs a pull-back or a push-forward between two constitutive tensor components
         */
        double& Constitutive_Law::TransformConstitutiveComponent(double & rCabcd,
                                                                 const Matrix & rConstitutiveMatrix,
                                                                 const Matrix & rF,
                                                                 const unsigned int& a, const unsigned int& b,
                                                                 const unsigned int& c, const unsigned int& d)

        {

            rCabcd = 0;
            double Cijkl=0;

            unsigned int dimension = rF.rows();

            //Cabcd
            for(unsigned int j=0; j<dimension; j++)
            {
                for(unsigned int l=0; l<dimension; l++)
                {
                    for(unsigned int k=0; k<dimension; k++)
                    {
                        for(unsigned int i=0; i<dimension; i++)
                        {
                            //Cijkl
                            rCabcd +=rF(a,i)*rF(b,j)*rF(c,k)*rF(d,l)*GetConstitutiveComponent(Cijkl,rConstitutiveMatrix,i,j,k,l);
                        }
                    }
                }
            }

            return rCabcd;

        }
        /**
         * This method gets the constitutive tensor components
         * from a consitutive matrix supplied in voigt notation
         */
        double& Constitutive_Law::GetConstitutiveComponent(double & rCabcd,
                                                           const Matrix& rConstitutiveMatrix,
                                                           const unsigned int& a, const unsigned int& b,
                                                           const unsigned int& c, const unsigned int& d)
        {
            // matrix indices
            unsigned int k=0, l= 0;

            unsigned int size = rConstitutiveMatrix.rows();

            if( size == 3 )
            {

                //index k
                for(unsigned int i=0; i<3; i++)
                {
                    if( a == b )
                    {
                        if( this->msIndexVoigt2D3C[i][0] == a && this->msIndexVoigt2D3C[i][1] == b )
                        {
                            k = i;
                            break;
                        }
                    }
                    else
                    {
                        if( (this->msIndexVoigt2D3C[i][0] == a && this->msIndexVoigt2D3C[i][1] == b) ||
                                (this->msIndexVoigt2D3C[i][1] == a && this->msIndexVoigt2D3C[i][0] == b) )
                        {
                            k = i;
                            break;
                        }
                    }
                }

                //index l
                for(unsigned int i=0; i<3; i++)
                {
                    if( c == d )
                    {
                        if( this->msIndexVoigt2D3C[i][0] == c && this->msIndexVoigt2D3C[i][1] == d )
                        {
                            l = i;
                            break;
                        }
                    }
                    else
                    {
                        if( (this->msIndexVoigt2D3C[i][0] == c && this->msIndexVoigt2D3C[i][1] == d) ||
                                (this->msIndexVoigt2D3C[i][1] == c && this->msIndexVoigt2D3C[i][0] == d) )
                        {
                            l = i;
                            break;
                        }
                    }
                }


            }
            else if( size == 4 )
            {

                //index k
                for(unsigned int i=0; i<4; i++)
                {
                    if( a == b )
                    {
                        if( this->msIndexVoigt2D4C[i][0] == a && this->msIndexVoigt2D4C[i][1] == b )
                        {
                            k = i;
                            break;
                        }
                    }
                    else
                    {
                        if( (this->msIndexVoigt2D4C[i][0] == a && this->msIndexVoigt2D4C[i][1] == b) ||
                                (this->msIndexVoigt2D4C[i][1] == a && this->msIndexVoigt2D4C[i][0] == b) )
                        {
                            k = i;
                            break;
                        }
                    }
                }

                //index l
                for(unsigned int i=0; i<4; i++)
                {
                    if( c == d )
                    {
                        if( this->msIndexVoigt2D4C[i][0] == c && this->msIndexVoigt2D4C[i][1] == d )
                        {
                            l = i;
                            break;
                        }
                    }
                    else
                    {
                        if( (this->msIndexVoigt2D4C[i][0] == c && this->msIndexVoigt2D4C[i][1] == d) ||
                                (this->msIndexVoigt2D4C[i][1] == c && this->msIndexVoigt2D4C[i][0] == d) )
                        {
                            l = i;
                            break;
                        }
                    }
                }

            }
            else if( size == 6 )
            {

                //index k
                for(unsigned int i=0; i<6; i++)
                {
                    if( a == b )
                    {
                        if( this->msIndexVoigt3D6C[i][0] == a && this->msIndexVoigt3D6C[i][1] == b )
                        {
                            k = i;
                            break;
                        }
                    }
                    else
                    {
                        if( (this->msIndexVoigt3D6C[i][0] == a && this->msIndexVoigt3D6C[i][1] == b) ||
                                (this->msIndexVoigt3D6C[i][1] == a && this->msIndexVoigt3D6C[i][0] == b) )
                        {
                            k = i;
                            break;
                        }
                    }
                }

                //index l
                for(unsigned int i=0; i<6; i++)
                {
                    if( c == d )
                    {
                        if( this->msIndexVoigt3D6C[i][0] == c && this->msIndexVoigt3D6C[i][1] == d )
                        {
                            l = i;
                            break;
                        }
                    }
                    else
                    {
                        if( (this->msIndexVoigt3D6C[i][0] == c && this->msIndexVoigt3D6C[i][1] == d) ||
                                (this->msIndexVoigt3D6C[i][1] == c && this->msIndexVoigt3D6C[i][0] == d) )
                        {
                            l = i;
                            break;
                        }
                    }
                }
            }

            rCabcd = rConstitutiveMatrix(k,l);

            return rCabcd;
        }

/// @}


/// @name Protected Access
/// @{


/// @}


/// @name Protected Inquiry
/// @{


/// @}


/// @brief private:
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
