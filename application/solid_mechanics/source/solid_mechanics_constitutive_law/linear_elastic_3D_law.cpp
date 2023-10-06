#include "../../include/solid_mechanics_constitutive_law/linear_elastic_3D_law.h"

#include "../../include/lotus_solid_mechanics_variables.h"

/// @brief public:
/// @name Type Define
/// @{


/// @}


/// @name Life Circle
/// @{
    Linear_Elastic_3D_Law::Linear_Elastic_3D_Law()
    : Hyper_Elastic_3D_Law()
    {
    }
    Linear_Elastic_3D_Law::Linear_Elastic_3D_Law(const Linear_Elastic_3D_Law& rOther)
    : Hyper_Elastic_3D_Law(rOther)
    {
    }
    Linear_Elastic_3D_Law::~Linear_Elastic_3D_Law()
    {
    }
/// @}


/// @name Operators
/// @{


/// @}


/// @name Operations
/// @{
    Constitutive_Law::Pointer Linear_Elastic_3D_Law::Clone() const
    {
        return std::make_shared<Linear_Elastic_3D_Law>(*this);
    }

    
    void  Linear_Elastic_3D_Law::CalculateMaterialResponsePK2 (Parameters& rValues)
    {

        //-----------------------------//

        //a.-Check if the constitutive parameters are passed correctly to the law calculation
        //CheckParameters(rValues);

        //b.- Get Values to compute the constitutive law:
        Flags &Options=rValues.GetOptions();
        mStrainEnergy = 0.0; //When it is not calculated, a zero will be returned

        const Properties& MaterialProperties  = rValues.GetMaterialProperties();

        Vector& StrainVector                  = rValues.GetStrainVector();
        Vector& StressVector                  = rValues.GetStressVector();

        //-----------------------------//

        //1.- Lame constants
        const double& YoungModulus          = MaterialProperties[YOUNG_MODULUS];
        const double& PoissonCoefficient    = MaterialProperties[POISSON_RATIO];

        // //1.1- Thermal constants
        // double ThermalExpansionCoefficient = 0;
        // if( MaterialProperties.Has(THERMAL_EXPANSION_COEFFICIENT) )
        //   ThermalExpansionCoefficient = MaterialProperties[THERMAL_EXPANSION_COEFFICIENT];

        // double ReferenceTemperature = 0;
        // if( MaterialProperties.Has(REFERENCE_TEMPERATURE) )
        //   ReferenceTemperature = MaterialProperties[REFERENCE_TEMPERATURE];


        if(Options.Is( Constitutive_Law::USE_ELEMENT_PROVIDED_STRAIN )) //large strains
        {

            //1.-Compute total deformation gradient
            const Matrix& DeformationGradientF = rValues.GetDeformationGradientF();

            //2.-Right Cauchy-Green tensor C
            Matrix RightCauchyGreen = DeformationGradientF.transpose()*DeformationGradientF;

            //3.-Green-Lagrange Strain:

            //E= 0.5*(FT*F-1)
            this->CalculateGreenLagrangeStrain(RightCauchyGreen,StrainVector);

        }

        //7.-Calculate Total PK2 stress

        if( Options.Is( Constitutive_Law::COMPUTE_STRESS ) )
        {
            if( Options.Is( Constitutive_Law::COMPUTE_CONSTITUTIVE_TENSOR ) ){

              Matrix& ConstitutiveMatrix            = rValues.GetConstitutiveMatrix();
              this->CalculateLinearElasticMatrix( ConstitutiveMatrix, YoungModulus, PoissonCoefficient );
              this->CalculateStress( StrainVector, ConstitutiveMatrix, StressVector );

            }
            else {

              Matrix ConstitutiveMatrix(StrainVector.size(),StrainVector.size());
              ConstitutiveMatrix.setZero(StrainVector.size(), StrainVector.size());
              this->CalculateLinearElasticMatrix( ConstitutiveMatrix, YoungModulus, PoissonCoefficient );
              this->CalculateStress( StrainVector, ConstitutiveMatrix, StressVector );
            }

        }
        else if( Options.Is( Constitutive_Law::COMPUTE_CONSTITUTIVE_TENSOR ) )
        {

            Matrix& ConstitutiveMatrix            = rValues.GetConstitutiveMatrix();
            this->CalculateLinearElasticMatrix( ConstitutiveMatrix, YoungModulus, PoissonCoefficient );

        }

        if( Options.Is( Constitutive_Law::COMPUTE_STRAIN_ENERGY ) )
        {

            if( Options.IsNot( Constitutive_Law::COMPUTE_STRESS ) )
            {

                if(Options.Is( Constitutive_Law::COMPUTE_CONSTITUTIVE_TENSOR ))
                {
            Matrix ConstitutiveMatrix( StrainVector.size(), StrainVector.size() );
            ConstitutiveMatrix.setZero(StrainVector.size(), StrainVector.size());

            this->CalculateLinearElasticMatrix( ConstitutiveMatrix, YoungModulus, PoissonCoefficient );
            this->CalculateStress( StrainVector, ConstitutiveMatrix, StressVector );
                }
                else
                {
                  Matrix& ConstitutiveMatrix = rValues.GetConstitutiveMatrix();
                  this->CalculateStress( StrainVector, ConstitutiveMatrix, StressVector );
                }

            }

            mStrainEnergy = 0.5 * StrainVector.dot(StressVector); //Belytschko Nonlinear Finite Elements pag 226 (5.4.3) : w = 0.5*E:C:E
        }
    }
    void Linear_Elastic_3D_Law::CalculateMaterialResponseKirchhoff (Parameters& rValues)
    {

        //-----------------------------//

        //a.-Check if the constitutive parameters are passed correctly to the law calculation
        //CheckParameters(rValues);

        //b.- Get Values to compute the constitutive law:
        Flags &Options=rValues.GetOptions();

        const Properties& MaterialProperties  = rValues.GetMaterialProperties();

        Vector& StrainVector                  = rValues.GetStrainVector();
        Vector& StressVector                  = rValues.GetStressVector();

        //-----------------------------//

        //1.- Lame constants
        const double& YoungModulus          = MaterialProperties[YOUNG_MODULUS];
        const double& PoissonCoefficient    = MaterialProperties[POISSON_RATIO];

        // //1.1- Thermal constants
        // double ThermalExpansionCoefficient = 0;
        // if( MaterialProperties.Has(THERMAL_EXPANSION_COEFFICIENT) )
        //   ThermalExpansionCoefficient = MaterialProperties[THERMAL_EXPANSION_COEFFICIENT];

        // double ReferenceTemperature = 0;
        // if( MaterialProperties.Has(REFERENCE_TEMPERATURE) )
        //   ReferenceTemperature = MaterialProperties[REFERENCE_TEMPERATURE];


        if(Options.Is( Constitutive_Law::USE_ELEMENT_PROVIDED_STRAIN )) //large strains
          {
      //1.-Compute total deformation gradient
            const Matrix& DeformationGradientF      = rValues.GetDeformationGradientF();

            //2.-Left Cauchy-Green tensor b
            Matrix LeftCauchyGreenMatrix = DeformationGradientF * DeformationGradientF.transpose();

            //3.-Almansi Strain:

            //e= 0.5*(1-invFT*invF)
            this->CalculateAlmansiStrain(LeftCauchyGreenMatrix,StrainVector);


      //LARGE STRAINS OBJECTIVE MESURE KIRCHHOFF MATERIAL:

      // Kirchhoff model is set with S = CE
      this->CalculateMaterialResponsePK2 (rValues);

      //1.- Obtain parameters
      const double& DeterminantF         = rValues.GetDeterminantF();

      //2.-Almansi Strain:
      // if(Options.Is( Constitutive_Law::USE_ELEMENT_PROVIDED_STRAIN ))
      //   {
      //     TransformStrains (StrainVector, DeformationGradientF, StrainMeasure_GreenLagrange, StrainMeasure_Almansi);
      //   }

      //3.-Calculate Total Kirchhoff stress
      if( Options.Is( Constitutive_Law::COMPUTE_STRESS ) )
      {
        TransformStresses(StressVector, DeformationGradientF, DeterminantF, StressMeasure_PK2, StressMeasure_Kirchhoff);
      }

      // COMMENTED BECAUSE THE CONVERGENCE IS NOT IMPROVED AND IS TIME CONSUMING:
      //4.-Calculate Cauchy constitutive tensor
      // if( Options.Is( Constitutive_Law::COMPUTE_CONSTITUTIVE_TENSOR ) )
      //   {
      //     Matrix& ConstitutiveMatrix  = rValues.GetConstitutiveMatrix();
      //     PushForwardConstitutiveMatrix(ConstitutiveMatrix, DeformationGradientF);
      //   }


      if( Options.Is( Constitutive_Law::COMPUTE_STRAIN_ENERGY ) )
      {
        mStrainEnergy *= DeterminantF;
      }

          }
        else{ //small strains

          //7.-Calculate total Kirchhoff stress

          if( Options.Is( Constitutive_Law::COMPUTE_STRESS ) ){

      if( Options.Is( Constitutive_Law::COMPUTE_CONSTITUTIVE_TENSOR ) ){

        Matrix& ConstitutiveMatrix            = rValues.GetConstitutiveMatrix();

        this->CalculateLinearElasticMatrix( ConstitutiveMatrix, YoungModulus, PoissonCoefficient );

        this->CalculateStress( StrainVector, ConstitutiveMatrix, StressVector );

      }
      else {

        Matrix ConstitutiveMatrix( StrainVector.size() ,StrainVector.size());

        ConstitutiveMatrix.setZero(StrainVector.size() ,StrainVector.size());

        this->CalculateLinearElasticMatrix( ConstitutiveMatrix, YoungModulus, PoissonCoefficient );

        this->CalculateStress( StrainVector, ConstitutiveMatrix, StressVector );

      }

          }
          else if( Options.Is( Constitutive_Law::COMPUTE_CONSTITUTIVE_TENSOR ) )
      {

        Matrix& ConstitutiveMatrix            = rValues.GetConstitutiveMatrix();
        this->CalculateLinearElasticMatrix( ConstitutiveMatrix, YoungModulus, PoissonCoefficient );

      }


          if( Options.Is( Constitutive_Law::COMPUTE_STRAIN_ENERGY ) )
      {

        if( Options.IsNot( Constitutive_Law::COMPUTE_STRESS ) ){

          if(Options.Is( Constitutive_Law::COMPUTE_CONSTITUTIVE_TENSOR )){
            Matrix ConstitutiveMatrix( StrainVector.size(), StrainVector.size());
            ConstitutiveMatrix.setZero(StrainVector.size(), StrainVector.size());

            this->CalculateLinearElasticMatrix( ConstitutiveMatrix, YoungModulus, PoissonCoefficient );
            this->CalculateStress( StrainVector, ConstitutiveMatrix, StressVector );
          }
          else{
            Matrix& ConstitutiveMatrix = rValues.GetConstitutiveMatrix();
            this->CalculateStress( StrainVector, ConstitutiveMatrix, StressVector );
          }


        }

        mStrainEnergy = 0.5 * StrainVector.dot(StressVector); //Belytschko Nonlinear Finite Elements pag 226 (5.4.3) : w = 0.5*E:C:E
      }

        }

    }
    int Linear_Elastic_3D_Law::Check(const Properties& rMaterialProperties,
                              const GeometryType& rElementGeometry,
                              const Process_Info& rCurrentProcessInfo) const
    {

        if(YOUNG_MODULUS.Key() == 0 || rMaterialProperties[YOUNG_MODULUS]<= 0.00)
        {
          std::cerr <<"YOUNG_MODULUS has Key zero or invalid value ";
          exit(0);
        }

        const double& nu = rMaterialProperties[POISSON_RATIO];
        const bool check = bool( (nu >0.499 && nu<0.501 ) || (nu < -0.999 && nu > -1.01 ) );

        if(POISSON_RATIO.Key() == 0 || check==true)
        {
          std::cerr <<"POISSON_RATIO has Key zero invalid value";
          exit(0);
        }

        if(DENSITY.Key() == 0 || rMaterialProperties[DENSITY]<0.00)
        {
          std::cerr <<"DENSITY has Key zero or invalid value ";
          exit(0);
        }
        return 0;
    }
    double& Linear_Elastic_3D_Law::CalculateValue(Parameters& rParameterValues, const Variable<double>& rThisVariable, double& rValue )
    {

      return (this->GetValue(rThisVariable,rValue ));

    }

/// @}


/// @name Access
/// @{
      void Linear_Elastic_3D_Law::GetLawFeatures(Features& rFeatures)
      {
            //Set the type of law
        rFeatures.mOptions.Set( THREE_DIMENSIONAL_LAW );
        rFeatures.mOptions.Set( INFINITESIMAL_STRAINS );
        rFeatures.mOptions.Set( ISOTROPIC );

        //Set strain measure required by the consitutive law
        rFeatures.mStrainMeasures.push_back(StrainMeasure_Infinitesimal);
        rFeatures.mStrainMeasures.push_back(StrainMeasure_Deformation_Gradient);

        //Set the strain size
        rFeatures.mStrainSize = GetStrainSize();

        //Set the spacedimension
        rFeatures.mSpaceDimension = WorkingSpaceDimension();

      }
    

/// @}


/// @name Inquiry
/// @{


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
    void Linear_Elastic_3D_Law::CalculateStress( const Vector & rStrainVector,
					  const Matrix & rConstitutiveMatrix,
					  Vector& rStressVector )
    {

        //1.-2nd Piola Kirchhoff StressVector or Cauchy StressVector
        if( rStressVector.size() != rStrainVector.size() )
          rStressVector.resize(rStrainVector.size());

        rStressVector = rConstitutiveMatrix*rStrainVector;


    }
    void Linear_Elastic_3D_Law::CalculateLinearElasticMatrix( Matrix& rConstitutiveMatrix,
            const double &rYoungModulus,
            const double &rPoissonCoefficient )
    {
        rConstitutiveMatrix.setZero();

        // 3D linear elastic constitutive matrix
        rConstitutiveMatrix ( 0 , 0 ) = (rYoungModulus*(1.0-rPoissonCoefficient)/((1.0+rPoissonCoefficient)*(1.0-2.0*rPoissonCoefficient)));
        rConstitutiveMatrix ( 1 , 1 ) = rConstitutiveMatrix ( 0 , 0 );
        rConstitutiveMatrix ( 2 , 2 ) = rConstitutiveMatrix ( 0 , 0 );

        rConstitutiveMatrix ( 3 , 3 ) = rConstitutiveMatrix ( 0 , 0 )*(1.0-2.0*rPoissonCoefficient)/(2.0*(1.0-rPoissonCoefficient));
        rConstitutiveMatrix ( 4 , 4 ) = rConstitutiveMatrix ( 3 , 3 );
        rConstitutiveMatrix ( 5 , 5 ) = rConstitutiveMatrix ( 3 , 3 );

        rConstitutiveMatrix ( 0 , 1 ) = rConstitutiveMatrix ( 0 , 0 )*rPoissonCoefficient/(1.0-rPoissonCoefficient);
        rConstitutiveMatrix ( 1 , 0 ) = rConstitutiveMatrix ( 0 , 1 );

        rConstitutiveMatrix ( 0 , 2 ) = rConstitutiveMatrix ( 0 , 1 );
        rConstitutiveMatrix ( 2 , 0 ) = rConstitutiveMatrix ( 0 , 1 );

        rConstitutiveMatrix ( 1 , 2 ) = rConstitutiveMatrix ( 0 , 1 );
        rConstitutiveMatrix ( 2 , 1 ) = rConstitutiveMatrix ( 0 , 1 );

    }
    bool Linear_Elastic_3D_Law::CheckParameters(Parameters& rValues)
    {
        return rValues.CheckAllParameters();
    }
/// @}


/// @name Protected Access
/// @{
    double& Linear_Elastic_3D_Law::GetValue( const Variable<double>& rThisVariable, double& rValue )
    {
        if (rThisVariable == STRAIN_ENERGY)
        {
          rValue = mStrainEnergy;
        }

      return( rValue );
    }
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

