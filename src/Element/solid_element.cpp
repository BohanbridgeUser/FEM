#include "../../include/Element/solid_element.h"

/**********************************************Create Flags**************************************************/
LOTUS_CREATE_LOCAL_FLAGS(Solid_Element,COMPUTE_RHS_VECTOR ,0)
LOTUS_CREATE_LOCAL_FLAGS(Solid_Element,COMPUTE_LHS_MATRIX ,1)
LOTUS_CREATE_LOCAL_FLAGS(Solid_Element,FINALIZED_STEP     ,2)

/********************************************Initialization**************************************************/
void Solid_Element::Initialize(const Process_Info& rCurrentProcessInfo)
{
    //member variables initialization
    InitializeConstitutiveLaw();
}
void Solid_Element::InitializeConstitutiveLaw()
{
    const GeometryType::IntegrationPointsVector& integration_points = GetGeometry().IntegrationPoints( mThisIntegrationMethod );

    //Constitutive Law initialisation
    if ( mConstitutiveLawVector.size() != integration_points.size() )
    {
        mConstitutiveLawVector.resize( integration_points.size() );
    }

    if ( GetProperties()[CONSTITUTIVE_LAW] != NULL )
    {
        for ( SizeType i = 0; i < mConstitutiveLawVector.size(); i++ )
        {
            mConstitutiveLawVector[i] = GetProperties()[CONSTITUTIVE_LAW]->Clone();
            mConstitutiveLawVector[i]->InitializeMaterial( GetProperties(), GetGeometry(),
                    GetGeometry().ShapeFunctionsValues( mThisIntegrationMethod ).row(i) );
        }
    }
    else
    {
        std::cerr << " a constitutive law needs to be specified for the element with ID " << std::endl;
        exit(0);
    }
}

void Solid_Element::InitializeSolutionStep( const Process_Info& rCurrentProcessInfo )
{
    InitializeExplicitContributions();
    this->Set(Solid_Element::FINALIZED_STEP,false);
}
void Solid_Element::InitializeExplicitContributions()
{
    const SizeType number_of_nodes = GetGeometry().PointsNumber();
    for ( SizeType i = 0; i < number_of_nodes; i++ )
    {
        if( GetGeometry()[i].SolutionStepsDataHas(EXTERNAL_FORCE) && GetGeometry()[i].SolutionStepsDataHas(INTERNAL_FORCE) )
        {
            std::array<double, 3 > & ExternalForce = GetGeometry()[i].FastGetSolutionStepValue(EXTERNAL_FORCE);
            std::array<double, 3 > & InternalForce = GetGeometry()[i].FastGetSolutionStepValue(INTERNAL_FORCE);
            ExternalForce.fill(0.0);
            InternalForce.fill(0.0);
        }
    }
}

/**********************************************Finalization**************************************************/
void Solid_Element::FinalizeSolutionStep( const Process_Info& rCurrentProcessInfo )
{
    //create and initialize element variables:
    ElementDataType Variables;
    this->InitializeElementData(Variables,rCurrentProcessInfo);

    //create constitutive law parameters:
    Constitutive_Law::Parameters Values(GetGeometry(),GetProperties(),rCurrentProcessInfo);

    //set constitutive law flags:
    Flags &ConstitutiveLawOptions=Values.GetOptions();

    ConstitutiveLawOptions.Set(Constitutive_Law::COMPUTE_STRESS);

    for ( unsigned int PointNumber = 0; PointNumber < mConstitutiveLawVector.size(); PointNumber++ )
    {

        //compute element kinematic variables B, F, DN_DX ...
        this->CalculateKinematics(Variables,PointNumber);

        //set general variables to constitutivelaw parameters
        this->SetElementData(Variables,Values,PointNumber);

        //call the constitutive law to update material variables
        mConstitutiveLawVector[PointNumber]->FinalizeMaterialResponse(Values, Variables.StressMeasure);

        //call the element internal variables update
        this->FinalizeStepVariables(Variables,PointNumber);
    }

    this->Set(Solid_Element::FINALIZED_STEP,true);

    if(this->Is(SELECTED) && this->Is(ACTIVE)){
      this->Set(SELECTED,false);
      std::cout<<" Undo SELECTED "<<this->Id()<<std::endl;
    }
}
void Solid_Element::FinalizeStepVariables( ElementDataType & rVariables,
                                           const double& rPointNumber )
{
  //to update the internal element variables
}

/**********************************************Calculattions*************************************************/
void Solid_Element::CalculateLocalSystem( MatrixType& rLeftHandSideMatrix,
                                          VectorType& rRightHandSideVector,
                                          const Process_Info& rCurrentProcessInfo )
{
    //create local system components
    LocalSystemComponents LocalSystem;

    //calculation flags
    LocalSystem.CalculationFlags.Set(Solid_Element::COMPUTE_LHS_MATRIX);
    LocalSystem.CalculationFlags.Set(Solid_Element::COMPUTE_RHS_VECTOR);

    //Initialize sizes for the system components:
    this->InitializeSystemMatrices( rLeftHandSideMatrix, rRightHandSideVector, LocalSystem.CalculationFlags );

    //Set Variables to Local system components
    LocalSystem.SetLeftHandSideMatrix(rLeftHandSideMatrix);
    LocalSystem.SetRightHandSideVector(rRightHandSideVector);

    //Calculate elemental system
    this->CalculateElementalSystem( LocalSystem, rCurrentProcessInfo );

    bool test_tangent = false;
    if( test_tangent )
    {
        MatrixType PerturbedLeftHandSideMatrix( rLeftHandSideMatrix.rows(), rLeftHandSideMatrix.cols() );
        PerturbedLeftHandSideMatrix.setZero(rLeftHandSideMatrix.rows(), rLeftHandSideMatrix.cols());
        this->CalculatePerturbedLeftHandSide( PerturbedLeftHandSideMatrix, rCurrentProcessInfo );
    }
}
void Solid_Element::InitializeSystemMatrices(MatrixType& rLeftHandSideMatrix,
					                         VectorType& rRightHandSideVector,
					                         Flags& rCalculationFlags)

{
    //resizing as needed the LHS
    const SizeType MatSize = this->GetDofsSize();
    if ( rCalculationFlags.Is(Solid_Element::COMPUTE_LHS_MATRIX) ) //calculation of the matrix is required
    {
        if ( rLeftHandSideMatrix.rows() != MatSize )
            rLeftHandSideMatrix.resize( MatSize, MatSize );

        rLeftHandSideMatrix.setZero( MatSize, MatSize ); //resetting LHS
    }
    //resizing as needed the RHS
    if ( rCalculationFlags.Is(Solid_Element::COMPUTE_RHS_VECTOR) ) //calculation of the matrix is required
    {
        if ( rRightHandSideVector.size() != MatSize )
	        rRightHandSideVector.resize( MatSize);

	    rRightHandSideVector.setZero(MatSize) ; //resetting RHS
    }
}
void Solid_Element::CalculateElementalSystem( LocalSystemComponents& rLocalSystem,
                                              const Process_Info& rCurrentProcessInfo)
{
    //create and initialize element variables:
    ElementDataType Variables;
    this->InitializeElementData(Variables,rCurrentProcessInfo);

    //create constitutive law parameters:
    Constitutive_Law::Parameters Values(GetGeometry(),GetProperties(),rCurrentProcessInfo);

    //set constitutive law flags:
    Flags &ConstitutiveLawOptions=Values.GetOptions();

    ConstitutiveLawOptions.Set(Constitutive_Law::COMPUTE_STRESS);
    ConstitutiveLawOptions.Set(Constitutive_Law::COMPUTE_CONSTITUTIVE_TENSOR);

    //reading integration points
    const GeometryType::IntegrationPointsVector& integration_points = GetGeometry().IntegrationPoints( mThisIntegrationMethod );

    //auxiliary terms
    const SizeType dimension  = GetGeometry().WorkingSpaceDimension();
    Vector VolumeForce(dimension);
    VolumeForce.setZero(dimension);

    for ( SizeType PointNumber = 0; PointNumber < integration_points.size(); PointNumber++ )
    {
        //compute element kinematic variables B, F, DN_DX ...
        this->CalculateKinematics(Variables,PointNumber);
        //calculate material response
        this->CalculateMaterialResponse(Variables,Values,PointNumber);
	    //some transformation of the configuration can be needed (UL element specially)
        this->TransformElementData(Variables,PointNumber);
        //calculating weights for integration on the "reference configuration"
        Variables.IntegrationWeight = integration_points[PointNumber].Weight() * Variables.detJ;
        Variables.IntegrationWeight = this->CalculateIntegrationWeight( Variables.IntegrationWeight );

        if( !IsSliver() )
        {
            if ( rLocalSystem.CalculationFlags.Is(Solid_Element::COMPUTE_LHS_MATRIX) ) //calculation of the matrix is required
            {
                //contributions to stiffness matrix calculated on the reference config
                this->CalculateAndAddLHS ( rLocalSystem, Variables, Variables.IntegrationWeight );
            }
            if ( rLocalSystem.CalculationFlags.Is(Solid_Element::COMPUTE_RHS_VECTOR) ) //calculation of the vector is required
            {
                //contribution to external forces
                VolumeForce  = this->CalculateVolumeForce( VolumeForce, Variables );
                this->CalculateAndAddRHS ( rLocalSystem, Variables, VolumeForce, Variables.IntegrationWeight );
            }
        }
    }
}
void Solid_Element::InitializeElementData (ElementDataType& rVariables, const Process_Info& rCurrentProcessInfo)
{

    const SizeType number_of_nodes  = GetGeometry().size();
    const SizeType dimension        = GetGeometry().WorkingSpaceDimension();
    const SizeType voigt_size       = dimension * (dimension +1) * 0.5;

    rVariables.Initialize(voigt_size,dimension,number_of_nodes);

    //set variables including all integration points values

    //reading shape functions
    rVariables.SetShapeFunctions(GetGeometry().ShapeFunctionsValues( mThisIntegrationMethod ));

    //reading shape functions local gradients
    rVariables.SetShapeFunctionsGradients(GetGeometry().ShapeFunctionsLocalGradients( mThisIntegrationMethod ));

    //set process info
    rVariables.SetProcessInfo(rCurrentProcessInfo);

    //calculating the current jacobian from cartesian coordinates to parent coordinates for all integration points [dx_n+1/d£]
    rVariables.j = GetGeometry().Jacobian( rVariables.j, mThisIntegrationMethod );

}
void Solid_Element::CalculateKinematics(ElementDataType& rVariables, const double& rPointNumber)
{
    std::cerr << " calling the default method CalculateKinematics for a solid element " << std::endl;
    exit(0);
}
void Solid_Element::CalculateMaterialResponse(ElementDataType& rVariables,
                                              Constitutive_Law::Parameters& rValues,
                                              const int & rPointNumber)
{
    //set general variables to constitutivelaw parameters
    this->SetElementData(rVariables,rValues,rPointNumber);
    //compute stresses and constitutive parameters
    mConstitutiveLawVector[rPointNumber]->CalculateMaterialResponse(rValues, rVariables.StressMeasure);
}
void Solid_Element::SetElementData(ElementDataType& rVariables,
				                   Constitutive_Law::Parameters& rValues,
				                   const int & rPointNumber)
{

}
void Solid_Element::TransformElementData(ElementDataType& rVariables, const double& rPointNumber)
{
  // to be used when different reference configuration is used
}
double& Solid_Element::CalculateIntegrationWeight(double& rIntegrationWeight)
{
    const SizeType dimension  = GetGeometry().WorkingSpaceDimension();
    if( dimension == 2 ){
      if ( this->GetProperties().Has( THICKNESS ) )
        rIntegrationWeight *= GetProperties()[THICKNESS];
    }
    return rIntegrationWeight;
}

/************************************** @name Calculate And Add LHS************************************************/
void Solid_Element::CalculateAndAddLHS(LocalSystemComponents& rLocalSystem,
                                       ElementDataType& rVariables,
                                       double& rIntegrationWeight)
{
    MatrixType& rLeftHandSideMatrix = rLocalSystem.GetLeftHandSideMatrix();
    // operation performed: add Km to the rLefsHandSideMatrix
    this->CalculateAndAddKuum( rLeftHandSideMatrix, rVariables, rIntegrationWeight );
    // operation performed: add Kg to the rLefsHandSideMatrix
    this->CalculateAndAddKuug( rLeftHandSideMatrix, rVariables, rIntegrationWeight );
}
void Solid_Element::CalculateAndAddKuum(MatrixType& rLeftHandSideMatrix,
                                        ElementDataType& rVariables,
                                        double& rIntegrationWeight)
{
    //contributions to stiffness matrix calculated on the reference config
    rLeftHandSideMatrix += rIntegrationWeight * (rVariables.B.transpose() * (rVariables.ConstitutiveMatrix * rVariables.B) );
}
void Solid_Element::CalculateAndAddKuug(MatrixType& rLeftHandSideMatrix,
				                        ElementDataType& rVariables,
				                        double& rIntegrationWeight)

{

}

/************************************** @name Calculate And Add RHS**********************************************/
Solid_Element::Vector& Solid_Element::CalculateVolumeForce( Solid_Element::Vector& rVolumeForce, ElementDataType& rVariables )
{
    const SizeType number_of_nodes = GetGeometry().PointsNumber();
    const SizeType dimension        = GetGeometry().WorkingSpaceDimension();

    if(rVolumeForce.size() != dimension)
      rVolumeForce.resize(dimension);
    rVolumeForce.setZero(dimension);

    for ( SizeType j = 0; j < number_of_nodes; j++ )
    {
      if( GetGeometry()[j].SolutionStepsDataHas(VOLUME_ACCELERATION) )
      { // it must be checked once at the begining only
        std::array<double, 3 >& VolumeAcceleration = GetGeometry()[j].FastGetSolutionStepValue(VOLUME_ACCELERATION);
        for( SizeType i = 0; i < dimension; i++ )
          rVolumeForce[i] += rVariables.N[j] * VolumeAcceleration[i] ;
      }
    }
    double VolumeChange = 1;
    VolumeChange = this->CalculateVolumeChange( VolumeChange, rVariables );
    rVolumeForce *= VolumeChange * GetProperties()[DENSITY];
    return rVolumeForce;
}
double& Solid_Element::CalculateVolumeChange( double& rVolumeChange, ElementDataType& rVariables )
{
    rVolumeChange = 1; //no change (reference configuration considered by default)
    return rVolumeChange;
}

void Solid_Element::CalculateAndAddRHS(LocalSystemComponents& rLocalSystem,
                                       ElementDataType& rVariables,
                                       Vector& rVolumeForce,
                                       double& rIntegrationWeight)
{
    VectorType& rRightHandSideVector = rLocalSystem.GetRightHandSideVector();
    // operation performed: rRightHandSideVector += ExtForce*IntToReferenceWeight
    this->CalculateAndAddExternalForces( rRightHandSideVector, rVariables, rVolumeForce, rIntegrationWeight );
    // operation performed: rRightHandSideVector -= IntForce*IntToReferenceWeight
    this->CalculateAndAddInternalForces( rRightHandSideVector, rVariables, rIntegrationWeight );
}
void Solid_Element::CalculateAndAddExternalForces(VectorType& rRightHandSideVector,
                                                  ElementDataType& rVariables,
                                                  Vector& rVolumeForce,
                                                  double& rIntegrationWeight)

{
    SizeType number_of_nodes = GetGeometry().PointsNumber();
    const SizeType dimension = GetGeometry().WorkingSpaceDimension();

    for ( SizeType i = 0; i < number_of_nodes; i++ )
    {
        int index = dimension * i;
        for ( SizeType j = 0; j < dimension; j++ )
        {
	        rRightHandSideVector[index + j] += rIntegrationWeight * rVariables.N[i] * rVolumeForce[j];
        }
    }
}
void Solid_Element::CalculateAndAddInternalForces(VectorType& rRightHandSideVector,
                                                  ElementDataType & rVariables,
                                                  double& rIntegrationWeight)
{
    VectorType InternalForces = rIntegrationWeight * (rVariables.B.transpose() * rVariables.StressVector);
    rRightHandSideVector -= InternalForces;
}


void Solid_Element::CalculatePerturbedLeftHandSide( MatrixType& rLeftHandSideMatrix,
                                                    const Process_Info& rCurrentProcessInfo )
{
    //create local system components
    LocalSystemComponents LocalSystem;

    //calculation flags
    LocalSystem.CalculationFlags.Set(Solid_Element::COMPUTE_LHS_MATRIX);
    LocalSystem.CalculationFlags.Set(Solid_Element::COMPUTE_RHS_VECTOR);

    VectorType RightHandSideVector = Vector();

    //Initialize sizes for the system components:
    this->InitializeSystemMatrices( rLeftHandSideMatrix, RightHandSideVector, LocalSystem.CalculationFlags );

    //Set Variables to Local system components
    LocalSystem.SetLeftHandSideMatrix(rLeftHandSideMatrix);
    LocalSystem.SetRightHandSideVector(RightHandSideVector);

    LocalSystem.CalculationFlags.Set(Solid_Element::COMPUTE_LHS_MATRIX,false);

    DofsVectorType ElementalDofList;
    this->GetDofList( ElementalDofList, rCurrentProcessInfo );

    unsigned int size = ElementalDofList.size();
    for( unsigned int i=0; i<size; i++)
      {
	//Set perturbation in "i" dof component
	double& value    = ElementalDofList[i]->GetSolutionStepValue();
	double  original = value;

	double  deltavalue = 1e-10;
	if( value !=0 )
	  deltavalue = value * 1e-8;

	//Calculate elemental system
	RightHandSideVector.resize(RightHandSideVector.size(),false);
	RightHandSideVector.setZero(RightHandSideVector.size());
	value = original + deltavalue;
	this->CalculateElementalSystem( LocalSystem, rCurrentProcessInfo );
	VectorType RightHandSideVectorI = RightHandSideVector;

	//Calculate elemental system
	RightHandSideVector.resize(RightHandSideVector.size(),false);
	RightHandSideVector.setZero(RightHandSideVector.size());
	value = original - deltavalue;
	this->CalculateElementalSystem( LocalSystem, rCurrentProcessInfo );
	VectorType RightHandSideVectorII = RightHandSideVector;

	for( unsigned int j=0; j<size; j++)
	  {
	    rLeftHandSideMatrix(j,i) = (-1) * (RightHandSideVectorI[j] - RightHandSideVectorII[j]) / (2.0*deltavalue);
	  }

	value = original;
        }
}

/************************************************ @name Utility Operations ****************************************/
void Solid_Element::EquationIdVector( EquationIdVectorType& rResult, const Process_Info& rCurrentProcessInfo ) const
{
    const SizeType number_of_nodes = GetGeometry().size();
    const SizeType dimension = GetGeometry().WorkingSpaceDimension();
    const SizeType dofs_size = GetDofsSize();

    if ( rResult.size() != dofs_size )
        rResult.resize( dofs_size);

    SizeType index = 0;
    for ( SizeType i = 0; i < number_of_nodes; i++ )
    {
        index = i * dimension;
        rResult[index]     = GetGeometry()[i].GetDof( DISPLACEMENT_X ).EquationId();
        rResult[index + 1] = GetGeometry()[i].GetDof( DISPLACEMENT_Y ).EquationId();
        if( dimension == 3)
            rResult[index + 2] = GetGeometry()[i].GetDof( DISPLACEMENT_Z ).EquationId();
    }
}
int Solid_Element::Check( const Process_Info& rCurrentProcessInfo ) const
{
    // Perform base element checks
    int ErrorCode = 0;
    ErrorCode = Element::Check(rCurrentProcessInfo);

    // Check that the constitutive law exists
    if ( this->GetProperties().Has( CONSTITUTIVE_LAW ) == false )
    {
      std::cerr << "constitutive law not provided for property " << this->GetProperties().Id() << std::endl;
    }
    else
    {
      const SizeType dimension  = GetGeometry().WorkingSpaceDimension();
      if( dimension == 3 &&  this->GetProperties().GetValue( CONSTITUTIVE_LAW )->GetStrainSize() != 6 )
	    std::cerr <<  "wrong constitutive law used. This is a 3D element. Expected strain size is 6 :: element id " << this->Id() << std::endl;
      // Check constitutive law
      this->GetProperties().GetValue( CONSTITUTIVE_LAW )->Check( this->GetProperties(), this->GetGeometry(), rCurrentProcessInfo );

    }
    return ErrorCode;
}

/**********************************************Access****************************************************/
Solid_Element::SizeType Solid_Element::GetDofsSize() const
{
    const SizeType dimension       = GetGeometry().WorkingSpaceDimension();
    const SizeType number_of_nodes = GetGeometry().PointsNumber();
    SizeType size = number_of_nodes * dimension; //usual size for displacement based elements
    return size;
}
