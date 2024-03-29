#include "../../include/solid_mechanics_constitutive_law/hyperelastic_3D_law.h"
#include "../../include/lotus_solid_mechanics_variables.h"

/// @brief public:
/// @name Type Define
/// @{


/// @}


/// @name Life Circle
/// @{
    Hyper_Elastic_3D_Law::Hyper_Elastic_3D_Law()
    : Constitutive_Law()
    {

    }
    Hyper_Elastic_3D_Law::Hyper_Elastic_3D_Law(const Hyper_Elastic_3D_Law& rOther)
        : Constitutive_Law(rOther)
        ,mInverseDeformationGradientF0(rOther.mInverseDeformationGradientF0)
        ,mDeterminantF0(rOther.mDeterminantF0)
        ,mStrainEnergy(rOther.mStrainEnergy)
    {
    }
    Hyper_Elastic_3D_Law::~Hyper_Elastic_3D_Law()
    {
    }

/// @}


/// @name Operators
/// @{


/// @}


/// @name Operations
/// @{
    Constitutive_Law::Pointer Hyper_Elastic_3D_Law::Clone() const
    {
        return std::make_shared<Hyper_Elastic_3D_Law>(*this);
    }

    double& Hyper_Elastic_3D_Law::CalculateValue(Parameters& rParameterValues, const Variable<double>& rThisVariable, double& rValue )
    {
      return (this->GetValue(rThisVariable,rValue ));
    }
    void Hyper_Elastic_3D_Law::SetValue( const Variable<double>& rThisVariable, const double& rValue,
                                      const Process_Info& rCurrentProcessInfo )
    {
      if (rThisVariable == DETERMINANT_F)
        {
          mDeterminantF0 = rValue;
        }
    }
    void Hyper_Elastic_3D_Law::SetValue( const Variable<Vector>& rThisVariable, const Vector& rValue,
                                      const Process_Info& rCurrentProcessInfo )
    {

    }
    void Hyper_Elastic_3D_Law::SetValue( const Variable<Matrix>& rThisVariable, const Matrix& rValue,
                                      const Process_Info& rCurrentProcessInfo )
    {

    }

    void Hyper_Elastic_3D_Law::InitializeMaterial( const Properties& rMaterialProperties,
            const GeometryType& rElementGeometry,
            const Vector& rShapeFunctionsValues )
    {
        mDeterminantF0                = 1;
        mInverseDeformationGradientF0 = Eigen::Matrix<double,3,3>::Identity();
        mStrainEnergy                 = 0;
    }
    void Hyper_Elastic_3D_Law::CalculateMaterialResponsePK2 (Parameters& rValues)
    {
      //-----------------------------//
      //a.-Check if the constitutive parameters are passed correctly to the law calculation
      CheckParameters(rValues);
      mStrainEnergy = 0.0; //When it is not calculated, a zero will be returned

      //b.- Get Values to compute the constitutive law:
      Flags &Options=rValues.GetOptions();

      const Properties& MaterialProperties  = rValues.GetMaterialProperties();
      const Matrix& DeformationGradientF    = rValues.GetDeformationGradientF();
      const double& DeterminantF            = rValues.GetDeterminantF();

      Vector& StrainVector                  = rValues.GetStrainVector();
      Vector& StressVector                  = rValues.GetStressVector();
      Matrix& ConstitutiveMatrix            = rValues.GetConstitutiveMatrix();

      //-----------------------------//

      //0.- Initialize parameters
      MaterialResponseVariables ElasticVariables;
      ElasticVariables.Identity = Eigen::Matrix<double,3,3>::Identity();;

      //1.- Lame constants
      const double& YoungModulus        = MaterialProperties[YOUNG_MODULUS];
      const double& PoissonCoefficient  = MaterialProperties[POISSON_RATIO];

      ElasticVariables.LameLambda       = (YoungModulus*PoissonCoefficient)/((1+PoissonCoefficient)*(1-2*PoissonCoefficient));
      ElasticVariables.LameMu           =  YoungModulus/(2*(1+PoissonCoefficient));

      //2.- Thermal constants
      if( MaterialProperties.Has(THERMAL_EXPANSION_COEFFICIENT) )
        ElasticVariables.ThermalExpansionCoefficient = MaterialProperties[THERMAL_EXPANSION_COEFFICIENT];
      else
        ElasticVariables.ThermalExpansionCoefficient = 0;

      if( MaterialProperties.Has(REFERENCE_TEMPERATURE) )
        ElasticVariables.ReferenceTemperature = MaterialProperties[REFERENCE_TEMPERATURE];
      else
        ElasticVariables.ReferenceTemperature = 0;


      //3.-DeformationGradient Tensor 3D
      ElasticVariables.DeformationGradientF = DeformationGradientF;
      ElasticVariables.DeformationGradientF = Transform2DTo3D( ElasticVariables.DeformationGradientF );

      //4.-Determinant of the Total Deformation Gradient
      ElasticVariables.DeterminantF = DeterminantF;

      //5.-Right Cauchy Green tensor C
      Matrix RightCauchyGreen = ElasticVariables.DeformationGradientF.transpose() * ElasticVariables.DeformationGradientF;

      //6.-Inverse of the Right Cauchy-Green tensor C: (stored in the CauchyGreenMatrix)
      ElasticVariables.traceCG = 0;
      ElasticVariables.CauchyGreenMatrix.resize(3,3);
      ElasticVariables.CauchyGreenMatrix = RightCauchyGreen.inverse();

      //7.-Green-Lagrange Strain:
      if(Options.Is( Constitutive_Law::USE_ELEMENT_PROVIDED_STRAIN ))
        {
          this->CalculateGreenLagrangeStrain(RightCauchyGreen, StrainVector);
        }

      //8.-Calculate Total PK2 stress
      if( Options.Is( Constitutive_Law::COMPUTE_STRESS ) )
        {

          this->CalculateStress( ElasticVariables, StressMeasure_PK2, StressVector );
        }

      //9.-Calculate Constitutive Matrix related to Total PK2 stress
      if( Options.Is( Constitutive_Law::COMPUTE_CONSTITUTIVE_TENSOR ) )
        {
          this->CalculateConstitutiveMatrix ( ElasticVariables, ConstitutiveMatrix );
        }

      if( Options.Is( Constitutive_Law::COMPUTE_STRAIN_ENERGY ) )
        {

          double ln_J = std::log(ElasticVariables.DeterminantF);
          double trace_C = 0.0;

          for (unsigned int i = 0; i<RightCauchyGreen.rows();i++)
      {
        trace_C += RightCauchyGreen(i,i);
      }

          mStrainEnergy =  0.5*ElasticVariables.LameLambda*ln_J*ln_J - ElasticVariables.LameMu*ln_J + 0.5*ElasticVariables.LameMu*(trace_C-3); //see Belytschko page 239


        }
    }
    void Hyper_Elastic_3D_Law::CalculateMaterialResponsePK1 (Parameters& rValues)
    {
        this->CalculateMaterialResponsePK2 (rValues);

        Vector& StressVector               = rValues.GetStressVector();
        const Matrix& DeformationGradientF = rValues.GetDeformationGradientF();
        const double& DeterminantF         = rValues.GetDeterminantF();

        TransformStresses(StressVector,DeformationGradientF,DeterminantF,StressMeasure_PK2,StressMeasure_PK1);
    }
    void Hyper_Elastic_3D_Law::CalculateMaterialResponseKirchhoff (Parameters& rValues)
    {

        //-----------------------------//

        //a.-Check if the constitutive parameters are passed correctly to the law calculation
        CheckParameters(rValues);

        //b.- Get Values to compute the constitutive law:
        Flags &Options=rValues.GetOptions();

        const Properties& MaterialProperties  = rValues.GetMaterialProperties();
        const Matrix&   DeformationGradientF  = rValues.GetDeformationGradientF();
        const double&   DeterminantF          = rValues.GetDeterminantF();

        Vector& StrainVector                  = rValues.GetStrainVector();
        Vector& StressVector                  = rValues.GetStressVector();
        Matrix& ConstitutiveMatrix            = rValues.GetConstitutiveMatrix();

        //-----------------------------//

        //0.- Initialize parameters
        MaterialResponseVariables ElasticVariables;
        ElasticVariables.Identity = Eigen::Matrix<double,3,3>::Identity();;

        //1.- Lame constants
        const double& YoungModulus       = MaterialProperties[YOUNG_MODULUS];
        const double& PoissonCoefficient = MaterialProperties[POISSON_RATIO];

        ElasticVariables.LameLambda      = (YoungModulus*PoissonCoefficient)/((1+PoissonCoefficient)*(1-2*PoissonCoefficient));
        ElasticVariables.LameMu          =  YoungModulus/(2*(1+PoissonCoefficient));

        //2.- Thermal constants
        if( MaterialProperties.Has(THERMAL_EXPANSION_COEFFICIENT) )
          ElasticVariables.ThermalExpansionCoefficient = MaterialProperties[THERMAL_EXPANSION_COEFFICIENT];
        else
          ElasticVariables.ThermalExpansionCoefficient = 0;

        if( MaterialProperties.Has(REFERENCE_TEMPERATURE) )
          ElasticVariables.ReferenceTemperature = MaterialProperties[REFERENCE_TEMPERATURE];
        else
          ElasticVariables.ReferenceTemperature = 0;

        //3.-DeformationGradient Tensor 3D
        ElasticVariables.DeformationGradientF = DeformationGradientF;
        ElasticVariables.DeformationGradientF = Transform2DTo3D( ElasticVariables.DeformationGradientF );

        //4.-Determinant of the Total Deformation Gradient
        ElasticVariables.DeterminantF         = DeterminantF;

        //5.-Left Cauchy Green tensor b: (stored in the CauchyGreenMatrix)
        ElasticVariables.CauchyGreenMatrix.resize(3,3);
        ElasticVariables.CauchyGreenMatrix = ElasticVariables.DeformationGradientF * ElasticVariables.DeformationGradientF.transpose();

        for( unsigned int i=0; i<3; i++)
        {
          ElasticVariables.traceCG += ElasticVariables.CauchyGreenMatrix( i , i );
        }

        //6.-Almansi Strain:
        if(Options.Is( Constitutive_Law::USE_ELEMENT_PROVIDED_STRAIN ))
        {
            // e= 0.5*(1-invbT*invb)
            this->CalculateAlmansiStrain(ElasticVariables.CauchyGreenMatrix,StrainVector);
        }


        //7.-Calculate Total kirchhoff stress
        if( Options.Is( Constitutive_Law::COMPUTE_STRESS ) )
        {
            this->CalculateStress( ElasticVariables, StressMeasure_Kirchhoff, StressVector );

        }

        //8.-Calculate Constitutive Matrix related to Total Kirchhoff stress
        if( Options.Is( Constitutive_Law::COMPUTE_CONSTITUTIVE_TENSOR ) )
        {
            ElasticVariables.CauchyGreenMatrix = ElasticVariables.Identity;
            this->CalculateConstitutiveMatrix ( ElasticVariables, ConstitutiveMatrix );
        }
    }
    void Hyper_Elastic_3D_Law::CalculateMaterialResponseCauchy (Parameters& rValues)
    {

        this->CalculateMaterialResponseKirchhoff (rValues);

        const double& DeterminantF          = rValues.GetDeterminantF();
        Vector& StressVector                = rValues.GetStressVector();
        Matrix& ConstitutiveMatrix          = rValues.GetConstitutiveMatrix();

        //Set to cauchy Stress:
        StressVector       /= DeterminantF;
        ConstitutiveMatrix /= DeterminantF;

    }
    void Hyper_Elastic_3D_Law::FinalizeMaterialResponsePK2 (Parameters& rValues)
    {

        rValues.Set(Constitutive_Law::FINALIZE_MATERIAL_RESPONSE);
        this->CalculateMaterialResponsePK2 (rValues);
        rValues.Reset(Constitutive_Law::FINALIZE_MATERIAL_RESPONSE);

        UpdateInternalVariables( rValues );
    }
    void Hyper_Elastic_3D_Law::FinalizeMaterialResponsePK1 (Parameters& rValues)
    {

        rValues.Set(Constitutive_Law::FINALIZE_MATERIAL_RESPONSE);
        this->CalculateMaterialResponsePK1 (rValues);
        rValues.Reset(Constitutive_Law::FINALIZE_MATERIAL_RESPONSE);

        UpdateInternalVariables( rValues );
    }
    void Hyper_Elastic_3D_Law::FinalizeMaterialResponseKirchhoff (Parameters& rValues)
    {

        rValues.Set(Constitutive_Law::FINALIZE_MATERIAL_RESPONSE);
        this->CalculateMaterialResponseKirchhoff (rValues);
        rValues.Reset(Constitutive_Law::FINALIZE_MATERIAL_RESPONSE);

        UpdateInternalVariables( rValues );
    }
    void Hyper_Elastic_3D_Law::FinalizeMaterialResponseCauchy (Parameters& rValues)
    {
        rValues.Set(Constitutive_Law::FINALIZE_MATERIAL_RESPONSE);
        this->CalculateMaterialResponseCauchy (rValues);
        rValues.Reset(Constitutive_Law::FINALIZE_MATERIAL_RESPONSE);

        UpdateInternalVariables( rValues );
    }
/// @}


/// @name Access
/// @{
    double& Hyper_Elastic_3D_Law::GetValue( const Variable<double>& rThisVariable, double& rValue )
    {
      if (rThisVariable == STRAIN_ENERGY)
      {
        rValue = mStrainEnergy;
      }
      else{
        rValue = 0;
      }


        return( rValue );
    }
    typename Hyper_Elastic_3D_Law::Vector& Hyper_Elastic_3D_Law::GetValue( const Variable<Vector>& rThisVariable, Vector& rValue )
    {
        return( rValue );
    }
    typename Hyper_Elastic_3D_Law::Matrix& Hyper_Elastic_3D_Law::GetValue( const Variable<Matrix>& rThisVariable, Matrix& rValue )
    {
        return( rValue );
    }

    void Hyper_Elastic_3D_Law::GetLawFeatures(Features& rFeatures)
    {
          //Set the type of law
      rFeatures.mOptions.Set( THREE_DIMENSIONAL_LAW );
      rFeatures.mOptions.Set( FINITE_STRAINS );
      rFeatures.mOptions.Set( ISOTROPIC );

      //Set strain measure required by the consitutive law
      rFeatures.mStrainMeasures.push_back(StrainMeasure_Deformation_Gradient);

      //Set the strain size
      rFeatures.mStrainSize = GetStrainSize();

      //Set the spacedimension
      rFeatures.mSpaceDimension = WorkingSpaceDimension();

    }
    int Hyper_Elastic_3D_Law::Check(const Properties& rMaterialProperties,
                                const GeometryType& rElementGeometry,
                                const Process_Info& rCurrentProcessInfo) const
    {

        if(YOUNG_MODULUS.Key() == 0 || rMaterialProperties[YOUNG_MODULUS]<= 0.00)
        {
          std::cerr << "YOUNG_MODULUS has Key zero or invalid value \n";
          exit(0);
        }

        const double& nu = rMaterialProperties[POISSON_RATIO];
        const bool check = bool( (nu >0.499 && nu<0.501 ) || (nu < -0.999 && nu > -1.01 ) );

        if(POISSON_RATIO.Key() == 0 || check==true)
        {
          std::cerr << "POISSON_RATIO has Key zero invalid value\n";
          exit(0);
        }

        if(DENSITY.Key() == 0 || rMaterialProperties[DENSITY]<0.00)
        {
          std::cerr << "DENSITY has Key zero or invalid value\n";
          exit(0);
        }
        return 0;
    }
/// @}


/// @name Inquiry
/// @{
    bool Hyper_Elastic_3D_Law::Has( const Variable<double>& rThisVariable )
    {
        return false;
    }
    bool Hyper_Elastic_3D_Law::Has( const Variable<Vector>& rThisVariable )
    {
        return false;
    }
    bool Hyper_Elastic_3D_Law::Has( const Variable<Matrix>& rThisVariable )
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
    void Hyper_Elastic_3D_Law::UpdateInternalVariables(Parameters& rValues)
    {
        const Matrix& DeformationGradientF    = rValues.GetDeformationGradientF();
        const double& DeterminantF            = rValues.GetDeterminantF();

        Matrix DeformationGradientF0          = DeformationGradientF;
        DeformationGradientF0 = Transform2DTo3D(DeformationGradientF0);
        this->mInverseDeformationGradientF0 = DeformationGradientF0.inverse();
        mDeterminantF0 = DeterminantF; //special treatment of the determinant
    }
    void Hyper_Elastic_3D_Law::CalculateGreenLagrangeStrain( const Matrix & rRightCauchyGreen,
            Vector& rStrainVector )
    {

        //E= 0.5*(FT*F-1) or E = 0.5*(C-1)
        rStrainVector[0] = 0.5 * ( rRightCauchyGreen( 0, 0 ) - 1.00 );
        rStrainVector[1] = 0.5 * ( rRightCauchyGreen( 1, 1 ) - 1.00 );
        rStrainVector[2] = 0.5 * ( rRightCauchyGreen( 2, 2 ) - 1.00 );
        rStrainVector[3] = rRightCauchyGreen( 0, 1 ); // xy
        rStrainVector[4] = rRightCauchyGreen( 1, 2 ); // yz
        rStrainVector[5] = rRightCauchyGreen( 0, 2 ); // xz

        // Matrix StrainMatrix(3,3);
        // noalias(StrainMatrix) = ZeroMatrix(3,3);
        // CalculateAlmansiStrain( rRightCauchyGreen, rStrainMatrix );
        // rStrainVector = StrainTensorToVector( StrainMatrix, rStrainVector.size() );

    }
    void Hyper_Elastic_3D_Law::CalculateAlmansiStrain( const Matrix & rLeftCauchyGreen,
                Vector& rStrainVector )
    {

        // e = 0.5*(1-invFT*invF) or e = 0.5*(1-inv(b))

        //Calculating the inverse of the jacobian
        Matrix InverseLeftCauchyGreen ( 3, 3 );
        double det_b=0;
        InverseLeftCauchyGreen = rLeftCauchyGreen.inverse();

        rStrainVector[0] = 0.5 * (  1.00 - InverseLeftCauchyGreen( 0, 0 ) );
        rStrainVector[1] = 0.5 * (  1.00 - InverseLeftCauchyGreen( 1, 1 ) );
        rStrainVector[2] = 0.5 * (  1.00 - InverseLeftCauchyGreen( 2, 2 ) );
        rStrainVector[3] = - InverseLeftCauchyGreen( 0, 1 ); // xy
        rStrainVector[4] = - InverseLeftCauchyGreen( 1, 2 ); // yz
        rStrainVector[5] = - InverseLeftCauchyGreen( 0, 2 ); // xz
    }
    double &  Hyper_Elastic_3D_Law::CalculateDomainTemperature (const MaterialResponseVariables & rElasticVariables,
                    double & rTemperature)
    {

        //1.-Temperature from nodes
        const GeometryType& DomainGeometry = rElasticVariables.GetElementGeometry();
        const Vector& ShapeFunctionsValues = rElasticVariables.GetShapeFunctionsValues();
        const unsigned int number_of_nodes = DomainGeometry.size();

        rTemperature=0;

        for ( unsigned int j = 0; j < number_of_nodes; j++ )
          {
      if( DomainGeometry[j].SolutionStepsDataHas(TEMPERATURE) )
        rTemperature += ShapeFunctionsValues[j] * DomainGeometry[j].GetSolutionStepValue(TEMPERATURE);
          }

        //2.-Temperature not included
        //rTemperature = 0;

        return rTemperature;
    }
    double & Hyper_Elastic_3D_Law::CalculateVolumetricFactor (const MaterialResponseVariables & rElasticVariables,
                      double & rFactor)
    {
      //(J²-1)/2
      //rFactor = 0.5*(rElasticVariables.DeterminantF*rElasticVariables.DeterminantF-1);

      //(ln(J))
      rFactor = std::log(rElasticVariables.DeterminantF);

      return rFactor;

    }
    double & Hyper_Elastic_3D_Law::CalculateVolumetricPressure (const MaterialResponseVariables & rElasticVariables,
                  double & rPressure)
    {

        double BulkModulus = rElasticVariables.LameLambda + (2.0/3.0) * rElasticVariables.LameMu;

        //Mechanical volumetric factor:
        double Factor = 0;
        Factor = this->CalculateVolumetricFactor( rElasticVariables, Factor );


        //Thermal volumetric factor:
        double DeltaTemperature     = 0;
        double CurrentTemperature   = 0;

        CurrentTemperature = this->CalculateDomainTemperature(rElasticVariables, CurrentTemperature);
        DeltaTemperature   = CurrentTemperature - rElasticVariables.ReferenceTemperature;

        Factor            += 3.0 * rElasticVariables.ThermalExpansionCoefficient * ( (1.0 - std::log(rElasticVariables.DeterminantF)) / (rElasticVariables.DeterminantF) ) * DeltaTemperature;


        rPressure = BulkModulus * Factor;

        return rPressure;
    }

    Vector&  Hyper_Elastic_3D_Law::CalculateVolumetricPressureFactors (const MaterialResponseVariables & rElasticVariables,
							       Vector & rFactors)

    {

        double BulkModulus = rElasticVariables.LameLambda + (2.0/3.0) * rElasticVariables.LameMu;

        if(rFactors.size()!=3) rFactors.resize(3);

        //(J²-1)/2
        // rFactors[0] =  rElasticVariables.DeterminantF*rElasticVariables.DeterminantF;
        // rFactors[1] =  (rElasticVariables.DeterminantF*rElasticVariables.DeterminantF-1);
        // rFactors[2] =  BulkModulus*rElasticVariables.DeterminantF;

        //(ln(J))
        rFactors[0] =  1.0;
        rFactors[1] =  (2.0*std::log(rElasticVariables.DeterminantF));
        rFactors[2] =  BulkModulus;

        return rFactors;
    }
    void Hyper_Elastic_3D_Law::CalculateStress( const MaterialResponseVariables & rElasticVariables,
              StressMeasure rStressMeasure,
              Vector& rStressVector )
    {

        Matrix StressMatrix( 3, 3 );

        //1.- Temporary and selected law

        double Factor = 0;
        Factor = this->CalculateVolumetricFactor( rElasticVariables, Factor );

        //(J²-1)/2
        //double Factor = 0.5*(rElasticVariables.DeterminantF*rElasticVariables.DeterminantF-1);

        //(ln(J))
        //double Factor = (std::log(rElasticVariables.DeterminantF));


        if(rStressMeasure == StressMeasure_PK2)  // the description corresponds to the neohookean material in Belytschko nonlinear finite elements, pag 239
        {
            //rElasticVariables.CauchyGreenMatrix is InverseRightCauchyGreen C^-1

            //2.-2nd Piola Kirchhoff Stress Matrix
            StressMatrix  = rElasticVariables.LameLambda * Factor * rElasticVariables.CauchyGreenMatrix;
            StressMatrix += rElasticVariables.LameMu * ( rElasticVariables.Identity - rElasticVariables.CauchyGreenMatrix );

        }

        if(rStressMeasure == StressMeasure_Kirchhoff) // the description corresponds to the neohookean material in Belytschko nonlinear finite elements, pag 239
        {
            //rElasticVariables.CauchyGreenMatrix is LeftCauchyGreen B

            //2.-Kirchhoff Stress Matrix
            StressMatrix  = rElasticVariables.LameLambda * Factor * rElasticVariables.Identity;

            StressMatrix += rElasticVariables.LameMu * ( rElasticVariables.CauchyGreenMatrix - rElasticVariables.Identity );

      }

        rStressVector = StressTensorToVector( StressMatrix, rStressVector.size());

    }
    void Hyper_Elastic_3D_Law::CalculateIsochoricStress( const MaterialResponseVariables & rElasticVariables,
                  StressMeasure rStressMeasure,
                  Vector& rIsoStressVector )
    {

        //1.-Identity build
        Matrix IsoStressMatrix ( 3, 3 );

        //note.- rElasticVariables.traceCG is "traceCG"

        if(rStressMeasure == StressMeasure_PK2)
        {

            //rElasticVariables.CauchyGreenMatrix is InverseRightCauchyGreen

            //2.-Incompressible part of the 2nd Piola Kirchhoff Stress Matrix
            IsoStressMatrix  = (rElasticVariables.Identity - (rElasticVariables.traceCG/3.0)*rElasticVariables.CauchyGreenMatrix );
            IsoStressMatrix *= rElasticVariables.LameMu*pow(rElasticVariables.DeterminantF,(-2.0/3.0));

            //std::cout<<" PK2 "<<std::endl;
        }

        if(rStressMeasure == StressMeasure_Kirchhoff)
        {

            //rElasticVariables.CauchyGreenMatrix is LeftCauchyGreen

            //2.-Incompressible part of the Kirchhoff Stress Matrix
            IsoStressMatrix  = (rElasticVariables.CauchyGreenMatrix - (rElasticVariables.traceCG/3.0)*rElasticVariables.Identity );
            IsoStressMatrix *= rElasticVariables.LameMu*pow(rElasticVariables.DeterminantF,(-2.0/3.0));

            //std::cout<<" Kirchooff "<<std::endl;

        }

        rIsoStressVector = StressTensorToVector(IsoStressMatrix,rIsoStressVector.size());

    }
    void Hyper_Elastic_3D_Law::CalculateVolumetricStress(const MaterialResponseVariables & rElasticVariables,
                  Vector& rVolStressVector )
    {

        //1.- Declaration
        Matrix VolStressMatrix ( 3 , 3 );

        double Pressure = 0;

        Pressure = this->CalculateVolumetricPressure (rElasticVariables, Pressure);

        //2.- Volumetric part of the Kirchhoff StressMatrix from nodal pressures
        VolStressMatrix = rElasticVariables.DeterminantF * Pressure * rElasticVariables.CauchyGreenMatrix;


        rVolStressVector = StressTensorToVector(VolStressMatrix,rVolStressVector.size());

    }
    void Hyper_Elastic_3D_Law::CalculateConstitutiveMatrix ( const MaterialResponseVariables& rElasticVariables,
                      Matrix& rConstitutiveMatrix)
    {

        rConstitutiveMatrix.setZero();

        for(unsigned int i=0; i<6; i++)
        {
            for(unsigned int j=0; j<6; j++)
            {
                rConstitutiveMatrix( i, j ) = ConstitutiveComponent(rConstitutiveMatrix( i, j ), rElasticVariables,
                                              this->msIndexVoigt3D6C[i][0], this->msIndexVoigt3D6C[i][1], this->msIndexVoigt3D6C[j][0], this->msIndexVoigt3D6C[j][1]);
            }

        }


    }
    void Hyper_Elastic_3D_Law::CalculateIsochoricConstitutiveMatrix (const MaterialResponseVariables & rElasticVariables,
                        const Matrix & rIsoStressMatrix,
                        Matrix& rConstitutiveMatrix)
    {

        rConstitutiveMatrix.setZero();

        for(unsigned int i=0; i<6; i++)
        {
            for(unsigned int j=0; j<6; j++)
            {
                rConstitutiveMatrix( i, j ) = IsochoricConstitutiveComponent(rConstitutiveMatrix( i, j ), rElasticVariables, rIsoStressMatrix,
                                              this->msIndexVoigt3D6C[i][0], this->msIndexVoigt3D6C[i][1], this->msIndexVoigt3D6C[j][0], this->msIndexVoigt3D6C[j][1]);
            }

        }


    }
    void Hyper_Elastic_3D_Law::CalculateVolumetricConstitutiveMatrix ( const MaterialResponseVariables & rElasticVariables,
                    Matrix& rConstitutiveMatrix)
    {

        rConstitutiveMatrix.setZero();

        Vector Factors(3);
        Factors.setZero(3);
        Factors = this->CalculateVolumetricPressureFactors( rElasticVariables, Factors );


        for(unsigned int i=0; i<6; i++)
        {
            for(unsigned int j=0; j<6; j++)
            {
                rConstitutiveMatrix( i, j ) = VolumetricConstitutiveComponent(rConstitutiveMatrix( i, j ), rElasticVariables, Factors,
                                              this->msIndexVoigt3D6C[i][0], this->msIndexVoigt3D6C[i][1], this->msIndexVoigt3D6C[j][0], this->msIndexVoigt3D6C[j][1]);
            }

        }


    }
    double& Hyper_Elastic_3D_Law::ConstitutiveComponent(double & rCabcd,
                const MaterialResponseVariables& rElasticVariables,
                const unsigned int& a, const unsigned int& b,
                const unsigned int& c, const unsigned int& d)
    {

        //1.- Temporary and selected law
        Vector Factors(3);
        Factors.setZero(3);
        Factors = this->CalculateVolumetricPressureFactors( rElasticVariables, Factors );

        double auxiliar1 = Factors[0];
        double auxiliar2 = Factors[1];

        //(J²-1)/2
        //double auxiliar1 =  rElasticVariables.DeterminantF*rElasticVariables.DeterminantF;
        //double auxiliar2 =  (rElasticVariables.DeterminantF*rElasticVariables.DeterminantF-1);

        //(ln(J))
        //double auxiliar1 =  1.0;
        //double auxiliar2 =  (2.0*std::log(rElasticVariables.DeterminantF));


        //1.Elastic constitutive tensor component:
        rCabcd =(rElasticVariables.LameLambda*auxiliar1*rElasticVariables.CauchyGreenMatrix(a,b)*rElasticVariables.CauchyGreenMatrix(c,d));
        rCabcd+=((2*rElasticVariables.LameMu-rElasticVariables.LameLambda*auxiliar2)*0.5*(rElasticVariables.CauchyGreenMatrix(a,c)*rElasticVariables.CauchyGreenMatrix(b,d)+rElasticVariables.CauchyGreenMatrix(a,d)*rElasticVariables.CauchyGreenMatrix(b,c)));

        return rCabcd;
    }
    double& Hyper_Elastic_3D_Law::IsochoricConstitutiveComponent(double & rCabcd,
                    const MaterialResponseVariables & rElasticVariables,
                    const Matrix & rIsoStressMatrix,
                    const unsigned int& a, const unsigned int& b,
                    const unsigned int& c, const unsigned int& d)
    {

        //Isochoric part of the hyperelastic constitutive tensor component

        //note.- rElasticVariables.traceCG is "traceCG_bar"

        rCabcd  = (1.0/3.0)*(rElasticVariables.CauchyGreenMatrix(a,b)*rElasticVariables.CauchyGreenMatrix(c,d));

        rCabcd -= (0.5*(rElasticVariables.CauchyGreenMatrix(a,c)*rElasticVariables.CauchyGreenMatrix(b,d)+rElasticVariables.CauchyGreenMatrix(a,d)*rElasticVariables.CauchyGreenMatrix(b,c)));

        rCabcd *= rElasticVariables.traceCG * rElasticVariables.LameMu;

        rCabcd += (rElasticVariables.CauchyGreenMatrix(c,d)*rIsoStressMatrix(a,b) + rIsoStressMatrix(c,d)*rElasticVariables.CauchyGreenMatrix(a,b));

        rCabcd *= (-2.0/3.0);


        return rCabcd;
    }
    double& Hyper_Elastic_3D_Law::VolumetricConstitutiveComponent(double & rCabcd,
                    const MaterialResponseVariables & rElasticVariables,
                    const Vector & rFactors,
                    const unsigned int& a, const unsigned int& b,
                    const unsigned int& c, const unsigned int& d)
    {

        //Volumetric part of the hyperelastic constitutive tensor component: (J²-1)/2  -  (ln(J)/J)

        //1.Volumetric Elastic constitutive tensor component:
        rCabcd  = rFactors[0]*(rElasticVariables.CauchyGreenMatrix(a,b)*rElasticVariables.CauchyGreenMatrix(c,d));
        rCabcd -= rFactors[1]*(0.5*(rElasticVariables.CauchyGreenMatrix(a,c)*rElasticVariables.CauchyGreenMatrix(b,d)+rElasticVariables.CauchyGreenMatrix(a,d)*rElasticVariables.CauchyGreenMatrix(b,c)));
        rCabcd *= rFactors[2];


        return rCabcd;
    }
    /**
     * Takes a matrix 2x2 and transforms it to a 3x3 adding a 3rd row and a 3rd column with a 1 in the diagonal
     * if the matrix passed is 3D is does nothing
     * if the matrix passed is bigger or smaller throws an error
     * @param rMatrix : usually the DeformationGradientF
     */
    typename Hyper_Elastic_3D_Law::Matrix& Hyper_Elastic_3D_Law::Transform2DTo3D (Matrix& rMatrix)
    {
        if (rMatrix.rows() == 2 && rMatrix.cols() == 2)
        {
            rMatrix.resize( 3, 3);

            rMatrix( 0 , 2 ) = 0.0;
            rMatrix( 1 , 2 ) = 0.0;

            rMatrix( 2 , 0 ) = 0.0;
            rMatrix( 2 , 1 ) = 0.0;

            rMatrix( 2 , 2 ) = 1.0;
        }
        else if(rMatrix.rows() != 3 && rMatrix.cols() != 3)
        {
            std::cerr << "Matrix Dimensions are not correct ";
            exit(0);
        }
        return rMatrix;
    }

    bool Hyper_Elastic_3D_Law::CheckParameters(Parameters& rValues)
    {
        return rValues.CheckAllParameters();
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


