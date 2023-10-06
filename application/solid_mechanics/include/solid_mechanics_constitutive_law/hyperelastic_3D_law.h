#ifndef  _HYPER_ELASTIC_3D_LAW_H_
#define  _HYPER_ELASTIC_3D_LAW_H_

// System includes

// External includes

// Project includes
#include "../../../../include/Constitutive_Law/Constitutive_law.h"

/**
 * Defines a hyperelastic isotropic constitutive law in 3D Neohookean Model
 * This material law is defined by the parameters:
 * 1) YOUNG MODULUS
 * 2) POISSON RATIO
 * As there are no further parameters the functionality is limited
 * to large displacements elasticity.
 */

class Hyper_Elastic_3D_Law : public Constitutive_Law
{
  protected:
    struct MaterialResponseVariables
    {
        //general material properties
        double LameMu;
        double LameLambda;

        //general thermal properties
        double ThermalExpansionCoefficient;
        double ReferenceTemperature;

        //kinematic properties
        double J_pow13;
        double DeterminantF;
        double traceCG;               //LeftCauchyGreen or RightCauchyGreen
        Matrix CauchyGreenMatrix;     //LeftCauchyGreen or InverseRightCauchyGreen
        Matrix DeformationGradientF;  //Deformation Gradient Tensor in 3D
        Matrix Identity;

        //element properties
        const Vector*        mpShapeFunctionsValues;
        const GeometryType*  mpElementGeometry;

    public:
      void SetShapeFunctionsValues (const Vector& rShapeFunctionsValues)      {mpShapeFunctionsValues=&rShapeFunctionsValues;};
      void SetElementGeometry      (const GeometryType& rElementGeometry)     {mpElementGeometry =&rElementGeometry;};
      const Vector& GetShapeFunctionsValues      () const {return *mpShapeFunctionsValues;};
      const GeometryType& GetElementGeometry     () const {return *mpElementGeometry;};
    };

  public:
    /// @name Type Define
    /// @{
      typedef Process_Info      
                                                              ProcessInfoType;
      typedef Constitutive_Law         
                                                                     BaseType;
      typedef std::size_t             
                                                                     SizeType;
      LOTUS_SHARED_POINTER_DEFINE(Hyper_Elastic_3D_Law)

    /// @}


    /// @name Life Circle
    /// @{
          Hyper_Elastic_3D_Law();
          Hyper_Elastic_3D_Law (const Hyper_Elastic_3D_Law& rOther);
          ~Hyper_Elastic_3D_Law() override;

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

        double& CalculateValue(Parameters& rParameterValues,
                               const Variable<double>& rThisVariable, 
                               double& rValue) override;

        void SetValue( const Variable<double>& rVariable,
                       const double& rValue,
                       const Process_Info& rCurrentProcessInfo ) override;
        void SetValue( const Variable<Vector>& rThisVariable,
                       const Vector& rValue,
                       const Process_Info& rCurrentProcessInfo ) override;
        void SetValue( const Variable<Matrix>& rThisVariable,
                       const Matrix& rValue,
                       const Process_Info& rCurrentProcessInfo ) override;

         /**
         * Material parameters are inizialized
         */
        void InitializeMaterial( const Properties& rMaterialProperties,
                                const GeometryType& rElementGeometry,
                                const Vector& rShapeFunctionsValues ) override;
        /* Computes the material response:
         * PK1 stresses and algorithmic ConstitutiveMatrix
         * @param rValues
         * @see   Parameters
         */
        void CalculateMaterialResponsePK1 (Parameters & rValues) override;
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
         * Computes the material response:
         * Cauchy stresses and algorithmic ConstitutiveMatrix
         * @param rValues
         * @see   Parameters
         */
        void CalculateMaterialResponseCauchy (Parameters & rValues) override;
        /**
          * Updates the material response:
          * Cauchy stresses and Internal Variables
          * @param rValues
          * @see   Parameters
          */
        void FinalizeMaterialResponsePK1 (Parameters & rValues) override;
        /**
          * Updates the material response:
          * Cauchy stresses and Internal Variables
          * @param rValues
          * @see   Parameters
          */
        void FinalizeMaterialResponsePK2 (Parameters & rValues) override;
        /**
          * Updates the material response:
          * Cauchy stresses and Internal Variables
          * @param rValues
          * @see   Parameters
          */
        void FinalizeMaterialResponseKirchhoff (Parameters & rValues) override;
        /**
          * Updates the material response:
          * Cauchy stresses and Internal Variables
          * @param rValues
          * @see   Parameters
          */
        void FinalizeMaterialResponseCauchy (Parameters & rValues) override;

        /**
         * This function is designed to be called once to perform all the checks needed
         * on the input provided. Checks can be "expensive" as the function is designed
         * to catch user's errors.
         * @param rMaterialProperties
         * @param rElementGeometry
         * @param rCurrentProcessInfo
         * @return
         */
        int Check(const Properties& rMaterialProperties,
                  const GeometryType& rElementGeometry, 
                  const Process_Info& rCurrentProcessInfo) const override;

    /// @}


    /// @name Access
    /// @{
        double& GetValue( const Variable<double>& rThisVariable, double& rValue ) override;
        Vector& GetValue( const Variable<Vector>& rThisVariable, Vector& rValue ) override;
        Matrix& GetValue( const Variable<Matrix>& rThisVariable, Matrix& rValue ) override;

        /**
         * This function is designed to be called once to check compatibility with element
         * @param rFeatures
         */
        void GetLawFeatures(Features& rFeatures) override;

    /// @}


    /// @name Inquiry
    /// @{
          /**
           * Dimension of the law:
           */
          SizeType WorkingSpaceDimension() override
          {
              return 3;
          };
          /**
           * Voigt tensor size:
           */
          SizeType GetStrainSize() const override
          {
              return 6;
          };
          bool Has( const Variable<double>& rThisVariable ) override;
          bool Has( const Variable<Vector>& rThisVariable ) override;
          bool Has( const Variable<Matrix>& rThisVariable ) override;


    /// @}


  protected:
    /// @name Protected Static Member Variables
    /// @{


    /// @}


    /// @name Protected Member Variables
    /// @{
        Matrix mInverseDeformationGradientF0;

        double mDeterminantF0;

        double mStrainEnergy;

    /// @}


    /// @name Protected Operatiors
    /// @{


    /// @}


    /// @name Protected Operations
    /// @{
        /**
         * Calculates the GreenLagrange strains
         * @param rRightCauchyGreen
         * @param rStrainVector
         */
        virtual void CalculateGreenLagrangeStrain( const Matrix & rRightCauchyGreen,
                Vector& rStrainVector );
        /**
         * Calculates the Almansi strains
         * @param rRightCauchyGreen
         * @param rStrainVector
         */
        virtual void CalculateAlmansiStrain( const Matrix & rLeftCauchyGreen,
                                            Vector& rStrainVector );
        /**
         * Calculates the stress vector
         * @param rElasticVariables
         * matrix is to be generated for
         * @param rStressMeasure measure of stress to be calculated
         * @param rStressVector vector where the stress result is stored
         */
        void CalculateStress( const MaterialResponseVariables& rElasticVariables,
                              StressMeasure rStressMeasure,
                              Vector& rStressVector);
        /**
         * Calculates the isochoric stress vector
         * @param rElasticVariables
         * matrix is to be generated for
         * @param rStressMeasure measure of stress to be calculated
         * @param rIsoStressVector vector where the stress result is stored
         */
        virtual void CalculateIsochoricStress( const MaterialResponseVariables & rElasticVariables,
                                              StressMeasure rStressMeasure,
                Vector& rIsoStressVector);
        /**
         * Calculates the volumetric stress vector
         * @param rElasticResponseVariables the material variables
         * @param rVolStressVector vector where the stress result is stored
         */
        virtual void CalculateVolumetricStress( const MaterialResponseVariables & rElasticVariables,
                                                Vector& rVolStressVector );
        /**
         * Calculates the constitutive matrix
         * @param rElasticVariables
         * matrix is to be generated for
         * @param rResult Matrix the result (Constitutive Matrix) will be stored in
         */
        virtual void CalculateConstitutiveMatrix (const MaterialResponseVariables& rElasticVariables,
                    Matrix& rConstitutiveMatrix);
        /**
         * Constitutive component
         */
        double& ConstitutiveComponent( double & rCabcd,
                                      const MaterialResponseVariables& rElasticVariables,
                                      const unsigned int& a, const unsigned int& b,
                                      const unsigned int& c, const unsigned int& d);
        /**
         * Calculates the isochoric constitutive matrix
         * @param rElasticVariables
         * @param rIsoStressVector the isochoric stress vector
         * matrix is to be generated for
         * @param rConstitutiveMatrix matrix where the constitutive tensor is stored
         */
        virtual void CalculateIsochoricConstitutiveMatrix (const MaterialResponseVariables& rElasticVariables,
                      const Matrix & rIsoStressMatrix,
                      Matrix& rConstitutiveMatrix);
        /**
         * Constitutive isochoric component
         */
        double& IsochoricConstitutiveComponent( double & rCabcd,
                                                const MaterialResponseVariables& rElasticVariables,
                                                const Matrix & rIsoStressMatrix,
                                                const unsigned int& a, const unsigned int& b,
                                                const unsigned int& c, const unsigned int& d);
        /**
         * Calculates the volumetric constitutive matrix
         * @param rElasticVariables
         * matrix is to be generated for
         * @param rConstitutiveMatrix matrix where the constitutive tensor is stored
         */
        virtual void CalculateVolumetricConstitutiveMatrix (const MaterialResponseVariables& rElasticVariables,
                  Matrix& rConstitutiveMatrix);
        /**
         * Constitutive volumetric component
         */
        double& VolumetricConstitutiveComponent( double & rCabcd,
                  const MaterialResponseVariables& rElasticVariables,
                  const Vector& rFactors,
                  const unsigned int& a, const unsigned int& b,
                  const unsigned int& c, const unsigned int& d);
        /**
         * Calculates HyperElasticLaw Factor for the Neo-Hookean model
         * @param rElasticResponseVariables the material variables
         * @param rFactor the calculated factor to be returned
         */
        virtual double& CalculateVolumetricFactor (const MaterialResponseVariables & rElasticVariables,
                    double & rFactor);
        /**
         * Calculates the Pressure of the domain (element)
         * @param rElasticResponseVariables the material variables
         * @param rPressure the calculated pressure to be returned
         */
        virtual double& CalculateVolumetricPressure (const MaterialResponseVariables & rElasticVariables,
                double & rPressure);
        /**
         * Calculates the Volumetric part factors
         * @param rElasticResponseVariables the material variables
         * @param rFactors Volumetric stress factors
         */
        virtual Vector&  CalculateVolumetricPressureFactors (const MaterialResponseVariables & rElasticVariables,
                  Vector & rFactors);
        /**
         * Calculates the Temperature of the domain (element)
         * @param rElementGeometry the element geometry
         * @param rShapeFunctions the element shape functions
         * @param rTemperature the calculated temperature to be returned
         */
        virtual double& CalculateDomainTemperature (const MaterialResponseVariables & rElasticVariables,
                double & rTemperature);
        /**
         * Takes a matrix 2x2 and transforms it to a 3x3 adding a 3rd row and a 3rd column with a 1 in the diagonal
         * if the matrix passed is 3D is does nothing
         * if the matrix passed is bigger or smaller throws an error
         * @param rMatrix : usually the DeformationGradientF
         */
        Matrix& Transform2DTo3D (Matrix& rMatrix);
        /**
          * Updates the material response:
          * Internal Variables
          * @param rValues
          * @see   Parameters
          */
        virtual void UpdateInternalVariables (Parameters & rValues);
        /**
         * This function is designed to be called when before the material response
         * to check if all needed parameters for the constitutive are initialized
         * @param Parameters
         * @return
         */
        virtual bool CheckParameters(Parameters& rValues);

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
