#include "../../include/Constitutive_Law/Constitutive_law.h"

const unsigned int Constitutive_Law::msIndexVoigt3D6C [6][2] = { {0, 0}, {1, 1}, {2, 2}, {0, 1}, {1, 2}, {0, 2} };
const unsigned int Constitutive_Law::msIndexVoigt2D4C [4][2] = { {0, 0}, {1, 1}, {2, 2}, {0, 1} };
const unsigned int Constitutive_Law::msIndexVoigt2D3C [3][2] = { {0, 0}, {1, 1}, {0, 1} };


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

/**
 * Constructor.
 */
Constitutive_Law::Constitutive_Law() : Flags()
{
}


/**
 * Clone function (has to be implemented by any derived class)
 * @return a pointer to a new instance of this constitutive law
 * NOTE: implementation scheme:
 *      ConstitutiveLaw::Pointer p_clone(new ConstitutiveLaw());
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
 * returns the size of the strain vector of the current constitutive law
 * NOTE: this function HAS TO BE IMPLEMENTED by any derived class
 */
Constitutive_Law::SizeType Constitutive_Law::GetStrainSize() const
{
    std::cerr <<  "Called the virtual function for GetStrainSize"<< std::endl;
    exit(0);
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