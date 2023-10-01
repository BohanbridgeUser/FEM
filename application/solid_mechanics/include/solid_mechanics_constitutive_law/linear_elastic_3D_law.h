#ifndef  _LINEAR_ELASTIC_3D_LAW_H_
#define  _LINEAR_ELASTIC_3D_LAW_H_
#include "hyperelastic_3D_law.h"


/**
 * Defines a linear isotropic constitutive law in 2D (Plane Strain)
 * This material law is defined by the parameters:
 * 1) YOUNG MODULUS
 * 2) POISSON RATIO
 * As there are no further parameters the functionality is valid
 * for small and large displacements elasticity.
 */
class Linear_Elastic_3D_Law : public Hyper_Elastic_3D_Law
{
    public:
        /// @name Type Define
        /// @{
            typedef Process_Info      
                                                ProcessInfoType;
            typedef Constitutive_Law                 
                                                       BaseType;
            typedef std::size_t            
                                                       SizeType;
            LOTUS_SHARED_POINTER_DEFINE(Linear_Elastic_3D_Law)
        /// @}


        /// @name Life Circle
        /// @{
            Linear_Elastic_3D_Law();
            /**
             * Copy constructor.
             */
            Linear_Elastic_3D_Law (const Linear_Elastic_3D_Law& rOther);
            ~Linear_Elastic_3D_Law() override;

        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{
            /**
             * Clone function (has to be implemented by any derived class)
             * @return a pointer to a new instance of this constitutive law
             */
            Constitutive_Law::Pointer Clone() const override;

             /**
             * Computes the material response:
             * PK2 stresses and algorithmic ConstitutiveMatrix
             * @param rValues
             * @see   Parameters
             */
            void CalculateMaterialResponsePK2 (Parameters & rValues) override;
            /**
             * Computes the material response:
             * Kirchhoff stresses and algorithmic ConstitutiveMatrix
             * @param rValues
             * @see   Parameters
             */
            void CalculateMaterialResponseKirchhoff (Parameters & rValues) override;
            /**
             * This function is designed to be called once to perform all the checks needed
             * on the input provided. Checks can be "expensive" as the function is designed
             * to catch user's errors.
             * @param rMaterialProperties
             * @param rElementGeometry
             * @param rCurrentProcessInfo
             * @return
             */
            int Check(const Properties& rMaterialProperties, const GeometryType& rElementGeometry, const Process_Info& rCurrentProcessInfo) const override;

        /// @}


        /// @name Access
        /// @{
             /**
             * This function is designed to be called once to check compatibility with element
             * @param rFeatures
             */
            void GetLawFeatures(Features& rFeatures) override;


        /// @}


        /// @name Inquiry
        /// @{


        /// @}


    protected:
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
            * Calculates the stresses for given strain state
            * @param rStrainVector
            * @param rConstitutiveMatrix
            * @param rStressVector the stress vector corresponding to the deformation
            */
            virtual void CalculateStress( const Vector &rStrainVector,
                                        const Matrix &rConstitutiveMatrix,
                                        Vector& rStressVector);
            /**
             * calculates the linear elastic constitutive matrix in terms of Young's modulus and
             * Poisson ratio
             * @param E the Young's modulus
             * @param NU the Poisson ratio
             * @return the linear elastic constitutive matrix
             */
            virtual void CalculateLinearElasticMatrix( Matrix& rConstitutiveMatrix,
                    const double &rYoungModulus,
                    const double &rPoissonCoefficient );
            /**
             * This function is designed to be called when before the material response
             * to check if all needed parameters for the constitutive are initialized
             * @param Parameters
             * @return
             */
            bool CheckParameters(Parameters& rValues) override;

            double& CalculateValue(Parameters& rParameterValues, const Variable<double>& rThisVariable, double& rValue) override;


        /// @}


        /// @name Protected Access
        /// @{
            double& GetValue( const Variable<double>& rThisVariable, double& rValue ) override;

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
