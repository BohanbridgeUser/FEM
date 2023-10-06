#include "../../include/solid_mechanics_condition/boundary_condition.h"
#include "../../include/lotus_solid_mechanics_variables.h"


/***********************************Define Flags*********************************************/
LOTUS_CREATE_LOCAL_FLAGS( Boundary_Condition, COMPUTE_RHS_VECTOR, 0 );
LOTUS_CREATE_LOCAL_FLAGS( Boundary_Condition, COMPUTE_LHS_MATRIX, 1 );

/************************************Life Circle*********************************************/
Boundary_Condition::Boundary_Condition()
:Condition()
{
}
Boundary_Condition::Boundary_Condition(IndexType NewId, 
                                       GeometryType::Pointer pGeometry)
:Condition(NewId, pGeometry)
{
}
Boundary_Condition::Boundary_Condition(IndexType NewId, 
                                       GeometryType::Pointer pGeometry, 
                                       PropertiesType::Pointer pProperties)
:Condition(NewId, pGeometry, pProperties)
{
    mThisIntegrationMethod = GetGeometry().GetDefaultIntegrationMethod();
}
Boundary_Condition::Boundary_Condition( Boundary_Condition const& rOther )
:Condition(rOther),
 mThisIntegrationMethod(rOther.mThisIntegrationMethod)
{
}

Condition::SharedPointer Boundary_Condition::Create(IndexType NewId,
                                                    NodesContainerType const& ThisNodes,
                                                    PropertiesType::Pointer pProperties) const
{
    return std::make_shared<Boundary_Condition>(NewId, GetGeometry().Create(ThisNodes), pProperties);
}

Condition::SharedPointer Boundary_Condition::Clone(IndexType NewId,
                                                   NodesContainerType const& ThisNodes) const
{
    Boundary_Condition NewCondition(NewId, GetGeometry().Create(ThisNodes),pGetProperties());
    NewCondition.SetData(this->GetData());
    NewCondition.SetFlags(this->GetFlag());
    return make_shared<Boundary_Condition>(NewCondition);
}

/************************************Utility Operations**************************************/
void Boundary_Condition::GetDofList(DofsVectorType& rConditionDofList,
				                    const Process_Info& rCurrentProcessInfo) const
{
    rConditionDofList.resize(0);
    const SizeType number_of_nodes = GetGeometry().PointsNumber();
    const SizeType& dimension = GetGeometry().WorkingSpaceDimension();

    for (SizeType i = 0; i < number_of_nodes; i++)
    {
        if( HasVariableDof(DISPLACEMENT_X) ){
          rConditionDofList.push_back(GetGeometry()[i].pGetDof(DISPLACEMENT_X));
          rConditionDofList.push_back(GetGeometry()[i].pGetDof(DISPLACEMENT_Y));
          if( dimension == 3 )
            rConditionDofList.push_back(GetGeometry()[i].pGetDof(DISPLACEMENT_Z));
        }
        else if( HasVariableDof(VELOCITY_X) ){
          rConditionDofList.push_back(GetGeometry()[i].pGetDof(VELOCITY_X));
          rConditionDofList.push_back(GetGeometry()[i].pGetDof(VELOCITY_Y));
          if( dimension == 3 )
            rConditionDofList.push_back(GetGeometry()[i].pGetDof(VELOCITY_Z));
        }
        if( HasVariableDof(ROTATION_X) ){
            if( dimension == 2 ){
                rConditionDofList.push_back(GetGeometry()[i].pGetDof(ROTATION_Z));
            }
            else{
                rConditionDofList.push_back(GetGeometry()[i].pGetDof(ROTATION_X));
                rConditionDofList.push_back(GetGeometry()[i].pGetDof(ROTATION_Y));
                rConditionDofList.push_back(GetGeometry()[i].pGetDof(ROTATION_Z));
            }
        }
    }
  }

void Boundary_Condition::EquationIdVector(EquationIdVectorType& rResult,
					                      const Process_Info& rCurrentProcessInfo) const
{
    const SizeType number_of_nodes = GetGeometry().PointsNumber();
    const SizeType& dimension       = GetGeometry().WorkingSpaceDimension();
    unsigned int       dofs_size       = this->GetDofsSize();

    if ( rResult.size() != dofs_size )
        rResult.resize( dofs_size, false );

    unsigned int index = 0;

    if( HasVariableDof(ROTATION_X) && HasVariableDof(DISPLACEMENT_X) ){
        if( dimension == 2 ){
            for ( SizeType i = 0; i < number_of_nodes; i++ )
            {
                index = i * ( (dimension-1) * 3 );
                rResult[index]   = GetGeometry()[i].GetDof(DISPLACEMENT_X).EquationId();
                rResult[index+1] = GetGeometry()[i].GetDof(DISPLACEMENT_Y).EquationId();
                rResult[index+2] = GetGeometry()[i].GetDof(ROTATION_Z).EquationId();
            }
        }
        else{
            for ( SizeType i = 0; i < number_of_nodes; i++ )
            {
                index = i * ( (dimension-1) * 3 );
                rResult[index]   = GetGeometry()[i].GetDof(DISPLACEMENT_X).EquationId();
                rResult[index+1] = GetGeometry()[i].GetDof(DISPLACEMENT_Y).EquationId();
                rResult[index+2] = GetGeometry()[i].GetDof(DISPLACEMENT_Z).EquationId();

                rResult[index+3] = GetGeometry()[i].GetDof(ROTATION_X).EquationId();
                rResult[index+4] = GetGeometry()[i].GetDof(ROTATION_Y).EquationId();
                rResult[index+5] = GetGeometry()[i].GetDof(ROTATION_Z).EquationId();
            }
        }
    }
    else if( HasVariableDof(DISPLACEMENT_X) ){
        for (SizeType i = 0; i < number_of_nodes; i++)
        {
            index = i * dimension;
            rResult[index]     = GetGeometry()[i].GetDof(DISPLACEMENT_X).EquationId();
            rResult[index + 1] = GetGeometry()[i].GetDof(DISPLACEMENT_Y).EquationId();
            if( dimension == 3)
                rResult[index + 2] = GetGeometry()[i].GetDof(DISPLACEMENT_Z).EquationId();
        }
    }
    else if( HasVariableDof(VELOCITY_X) ){
        for (SizeType i = 0; i < number_of_nodes; i++)
        {
            index = i * dimension;
            rResult[index]     = GetGeometry()[i].GetDof(VELOCITY_X).EquationId();
            rResult[index + 1] = GetGeometry()[i].GetDof(VELOCITY_Y).EquationId();
            if( dimension == 3)
                rResult[index + 2] = GetGeometry()[i].GetDof(VELOCITY_Z).EquationId();
        }
    }
}

void Boundary_Condition::GetValuesVector(VectorType& rValues, int Step) const
{
    const SizeType number_of_nodes = GetGeometry().PointsNumber();
    const SizeType& dimension       = GetGeometry().WorkingSpaceDimension();
    unsigned int       dofs_size       = this->GetDofsSize();

    if ( rValues.size() != dofs_size )
        rValues.resize( dofs_size, false );
    unsigned int index = 0;
    if( HasVariableDof(ROTATION) ){
        if( dimension == 2 ){
            for ( SizeType i = 0; i < number_of_nodes; i++ )
            {
            index = i * ( (dimension-1) * 3 );
            rValues[index]   = GetGeometry()[i].GetSolutionStepValue( DISPLACEMENT_X, Step );
            rValues[index+1] = GetGeometry()[i].GetSolutionStepValue( DISPLACEMENT_Y, Step );
            rValues[index+2] = GetGeometry()[i].GetSolutionStepValue( ROTATION_Z, Step );
            }
        }
        else{
            for ( SizeType i = 0; i < number_of_nodes; i++ )
            {
            index = i * ( (dimension-1) * 3 );
            rValues[index]   = GetGeometry()[i].GetSolutionStepValue( DISPLACEMENT_X, Step );
            rValues[index+1] = GetGeometry()[i].GetSolutionStepValue( DISPLACEMENT_Y, Step );
            rValues[index+2] = GetGeometry()[i].GetSolutionStepValue( DISPLACEMENT_Z, Step );

            rValues[index+3] = GetGeometry()[i].GetSolutionStepValue( ROTATION_X, Step );
            rValues[index+4] = GetGeometry()[i].GetSolutionStepValue( ROTATION_Y, Step );
            rValues[index+5] = GetGeometry()[i].GetSolutionStepValue( ROTATION_Z, Step );
            }
        }
    }
    else{
        for (SizeType i = 0; i < number_of_nodes; i++)
        {
            index = i * dimension;
            rValues[index]     = GetGeometry()[i].GetSolutionStepValue( DISPLACEMENT_X, Step );
            rValues[index + 1] = GetGeometry()[i].GetSolutionStepValue( DISPLACEMENT_Y, Step );

            if ( dimension == 3 )
                rValues[index + 2] = GetGeometry()[i].GetSolutionStepValue( DISPLACEMENT_Z, Step );
        }
    }
}

void Boundary_Condition::GetFirstDerivativesVector( VectorType& rValues, int Step ) const
{

    const SizeType number_of_nodes = GetGeometry().PointsNumber();
    const SizeType& dimension       = GetGeometry().WorkingSpaceDimension();
    unsigned int       dofs_size       = this->GetDofsSize();

    if ( rValues.size() != dofs_size )
      rValues.resize( dofs_size, false );

    unsigned int index = 0;

    if( HasVariableDof(ROTATION) ){
        if( dimension == 2 ){
            for ( SizeType i = 0; i < number_of_nodes; i++ )
            {
                index = i * ( (dimension-1) * 3 );
                rValues[index]   = GetGeometry()[i].GetSolutionStepValue( VELOCITY_X, Step );
                rValues[index+1] = GetGeometry()[i].GetSolutionStepValue( VELOCITY_Y, Step );
                rValues[index+2] = GetGeometry()[i].GetSolutionStepValue( VELOCITY_Z, Step );
            }
        }
        else{
            for ( SizeType i = 0; i < number_of_nodes; i++ )
            {
                index = i * ( (dimension-1) * 3 );
                rValues[index]   = GetGeometry()[i].GetSolutionStepValue( VELOCITY_X, Step );
                rValues[index+1] = GetGeometry()[i].GetSolutionStepValue( VELOCITY_Y, Step );
                rValues[index+2] = GetGeometry()[i].GetSolutionStepValue( VELOCITY_Z, Step );

                rValues[index+3] = GetGeometry()[i].GetSolutionStepValue( ROTATION_X, Step );
                rValues[index+4] = GetGeometry()[i].GetSolutionStepValue( ROTATION_Y, Step );
                rValues[index+5] = GetGeometry()[i].GetSolutionStepValue( ROTATION_Z, Step );
            }
        }
    }
    else{
        for ( SizeType i = 0; i < number_of_nodes; i++ )
        {
            index = i * dimension;
            rValues[index]     = GetGeometry()[i].GetSolutionStepValue( VELOCITY_X, Step );
            rValues[index + 1] = GetGeometry()[i].GetSolutionStepValue( VELOCITY_Y, Step );

            if ( dimension == 3 )
                rValues[index + 2] = GetGeometry()[i].GetSolutionStepValue( VELOCITY_Z, Step );
        }
    }

}

void Boundary_Condition::GetSecondDerivativesVector( VectorType& rValues, int Step ) const
{
    const SizeType number_of_nodes = GetGeometry().PointsNumber();
    const SizeType& dimension       = GetGeometry().WorkingSpaceDimension();
    unsigned int       dofs_size       = this->GetDofsSize();

    if ( rValues.size() != dofs_size )
      rValues.resize( dofs_size, false );

    unsigned int index = 0;

    if( HasVariableDof(ROTATION) ){
        if( dimension == 2 ){
            for ( SizeType i = 0; i < number_of_nodes; i++ )
            {
                index = i * ( (dimension-1) * 3 );
                rValues[index]   = GetGeometry()[i].GetSolutionStepValue( ACCELERATION_X, Step );
                rValues[index+1] = GetGeometry()[i].GetSolutionStepValue( ACCELERATION_Y, Step );
                rValues[index+2] = GetGeometry()[i].GetSolutionStepValue( ANGULAR_ACCELERATION_Z, Step );
            }
        }
        else{
            for ( SizeType i = 0; i < number_of_nodes; i++ )
            {
                index = i * ( (dimension-1) * 3 );
                rValues[index]   = GetGeometry()[i].GetSolutionStepValue( ACCELERATION_X, Step );
                rValues[index+1] = GetGeometry()[i].GetSolutionStepValue( ACCELERATION_Y, Step );
                rValues[index+2] = GetGeometry()[i].GetSolutionStepValue( ACCELERATION_Z, Step );

                rValues[index+3] = GetGeometry()[i].GetSolutionStepValue( ANGULAR_ACCELERATION_X, Step );
                rValues[index+4] = GetGeometry()[i].GetSolutionStepValue( ANGULAR_ACCELERATION_Y, Step );
                rValues[index+5] = GetGeometry()[i].GetSolutionStepValue( ANGULAR_ACCELERATION_Z, Step );
            }
        }
    }
    else{
        for ( SizeType i = 0; i < number_of_nodes; i++ )
        {
            index = i * dimension;
            rValues[index]     = GetGeometry()[i].GetSolutionStepValue( ACCELERATION_X, Step );
            rValues[index + 1] = GetGeometry()[i].GetSolutionStepValue( ACCELERATION_Y, Step );

            if ( dimension == 3 )
                rValues[index + 2] = GetGeometry()[i].GetSolutionStepValue( ACCELERATION_Z, Step );
        }
    }
}

bool Boundary_Condition::HasVariableDof(VariableVectorType& rVariable) const
{
    //usually if the dofs do not exist condition adds them, standard conditions do not work like this
    if( GetGeometry()[0].HasDofFor(rVariable) == true )
        return true;

    return false;
}

bool Boundary_Condition::HasVariableDof(VariableScalarType& rVariable) const
{
    //usually if the dofs do not exist condition adds them, standard conditions do not work like this
    if( GetGeometry()[0].HasDofFor(rVariable) == true )
        return true;

    return false;
}

unsigned int Boundary_Condition::GetDofsSize() const
{

    const SizeType& dimension       = GetGeometry().WorkingSpaceDimension();
    const SizeType number_of_nodes = GetGeometry().PointsNumber();
    unsigned int size = number_of_nodes * dimension;
    if( HasVariableDof(ROTATION) ){
        if(dimension == 2){
    size += number_of_nodes;
        }
        else{
    size += number_of_nodes * dimension;
        }
    }
    return size;

}

/***********************************Initialization*******************************************/
void Boundary_Condition::InitializeSystemMatrices(MatrixType& rLeftHandSideMatrix,
						                          VectorType& rRightHandSideVector,
						                          Flags& rCalculationFlags)
{
    //resizing as needed the LHS
    unsigned int MatSize = this->GetDofsSize();
    if ( rCalculationFlags.Is(Boundary_Condition::COMPUTE_LHS_MATRIX) ) //calculation of the matrix is required
    {
        if ( rLeftHandSideMatrix.rows() != MatSize )
        rLeftHandSideMatrix.resize(MatSize, MatSize);

        rLeftHandSideMatrix.setZero(MatSize, MatSize); //resetting LHS
    }

    //resizing as needed the RHS
    if ( rCalculationFlags.Is(Boundary_Condition::COMPUTE_RHS_VECTOR) ) //calculation of the matrix is required
    {
        if ( rRightHandSideVector.size() != MatSize )
        rRightHandSideVector.resize( MatSize);
        rRightHandSideVector.setZero( MatSize ); //resetting RHS
    }
}
void Boundary_Condition::InitializeConditionVariables(ConditionVariables& rVariables, const Process_Info& rCurrentProcessInfo)
{
    const SizeType number_of_nodes  = GetGeometry().PointsNumber();
    const SizeType& local_dimension = GetGeometry().LocalSpaceDimension();
    const SizeType& dimension       = GetGeometry().WorkingSpaceDimension();

    rVariables.Initialize(dimension, local_dimension, number_of_nodes);

    //set variables including all integration points values

    //reading shape functions
    rVariables.SetShapeFunctions(GetGeometry().ShapeFunctionsValues( mThisIntegrationMethod ));

    //reading shape functions local gradients
    rVariables.SetShapeFunctionsGradients(GetGeometry().ShapeFunctionsLocalGradients( mThisIntegrationMethod ));

}
void Boundary_Condition::Initialize( const Process_Info& rCurrentProcessInfo )
{

}
void Boundary_Condition::InitializeSolutionStep( const Process_Info& rCurrentProcessInfo )
{

InitializeExplicitContributions();

}
void Boundary_Condition::InitializeNonLinearIteration( const Process_Info& rCurrentProcessInfo )
{

}
void Boundary_Condition::InitializeExplicitContributions()
{
    const SizeType number_of_nodes = GetGeometry().PointsNumber();
    for ( SizeType i = 0; i < number_of_nodes; i++ )
    {
        if( GetGeometry()[i].SolutionStepsDataHas(EXTERNAL_FORCE) && GetGeometry()[i].SolutionStepsDataHas(FORCE_RESIDUAL) ){
            std::array<double, 3 > & ExternalForce = GetGeometry()[i].FastGetSolutionStepValue(EXTERNAL_FORCE);
            std::array<double, 3 > & ResidualForce = GetGeometry()[i].FastGetSolutionStepValue(FORCE_RESIDUAL);
            ExternalForce.fill(0.0);
            ResidualForce.fill(0.0);
        }
        if( HasVariableDof(ROTATION) ){
            if( GetGeometry()[i].SolutionStepsDataHas(EXTERNAL_MOMENT) && GetGeometry()[i].SolutionStepsDataHas(MOMENT_RESIDUAL) ){
                std::array<double, 3 > & ExternalMoment = GetGeometry()[i].FastGetSolutionStepValue(EXTERNAL_MOMENT);
                std::array<double, 3 > & ResidualMoment = GetGeometry()[i].FastGetSolutionStepValue(MOMENT_RESIDUAL);
                ExternalMoment.fill(0.0);
                ResidualMoment.fill(0.0);
            }
	    }
    }
}

/**********************************Calculation Operations****************************************/
void Boundary_Condition::CalculateLocalSystem( MatrixType& rLeftHandSideMatrix, VectorType& rRightHandSideVector, const Process_Info& rCurrentProcessInfo )
{
    //create local system components
    LocalSystemComponents LocalSystem;
    //calculation flags
    LocalSystem.CalculationFlags.Set(Boundary_Condition::COMPUTE_LHS_MATRIX);
    LocalSystem.CalculationFlags.Set(Boundary_Condition::COMPUTE_RHS_VECTOR);

    //Initialize sizes for the system components:
    this->InitializeSystemMatrices( rLeftHandSideMatrix, rRightHandSideVector, LocalSystem.CalculationFlags );

    //Set Variables to Local system components
    LocalSystem.SetLeftHandSideMatrix(rLeftHandSideMatrix);
    LocalSystem.SetRightHandSideVector(rRightHandSideVector);

    //Calculate condition system
    this->CalculateConditionSystem( LocalSystem, rCurrentProcessInfo );
}
void Boundary_Condition::CalculateConditionSystem(LocalSystemComponents& rLocalSystem,
                                                  const Process_Info& rCurrentProcessInfo)
{

    //create and initialize condition variables:
    ConditionVariables Variables;
    this->InitializeConditionVariables(Variables,rCurrentProcessInfo);
    //reading integration points
    const GeometryType::IntegrationPointsVector& integration_points = GetGeometry().IntegrationPoints( mThisIntegrationMethod );
    double IntegrationWeight = 1;

    for ( unsigned int PointNumber = 0; PointNumber < integration_points.size(); PointNumber++ )
    {
        //compute element kinematics B, F, DN_DX ...
        this->CalculateKinematics(Variables,PointNumber);
        IntegrationWeight = Variables.Jacobian * integration_points[PointNumber].Weight();
        if ( rLocalSystem.CalculationFlags.Is(Boundary_Condition::COMPUTE_LHS_MATRIX) ) //calculation of the matrix is required
        {
            //contributions to stiffness matrix calculated on the reference config
            this->CalculateAndAddLHS ( rLocalSystem, Variables, IntegrationWeight );
        }
        if ( rLocalSystem.CalculationFlags.Is(Boundary_Condition::COMPUTE_RHS_VECTOR) ) //calculation of the vector is required
        {
            //contribution to external forces
            this->CalculateAndAddRHS ( rLocalSystem, Variables, IntegrationWeight );
        }
    }
}
void Boundary_Condition::CalculateKinematics(ConditionVariables& rVariables,
					                         const double& rPointNumber)
{
}
void Boundary_Condition::CalculateAndAddLHS(LocalSystemComponents& rLocalSystem,
                                            ConditionVariables& rVariables,
                                            double& rIntegrationWeight)
{
    //contributions of the stiffness matrix calculated on the reference configuration
    MatrixType& rLeftHandSideMatrix = rLocalSystem.GetLeftHandSideMatrix();
    // operation performed: add Kg to the rLefsHandSideMatrix
    this->CalculateAndAddKuug( rLeftHandSideMatrix, rVariables, rIntegrationWeight );
}
void Boundary_Condition::CalculateAndAddKuug(MatrixType& rLeftHandSideMatrix,
					                         ConditionVariables& rVariables,
					                         double& rIntegrationWeight)

{
    unsigned int MatSize = this->GetDofsSize();
    if(rLeftHandSideMatrix.rows() != MatSize)
      rLeftHandSideMatrix.resize(MatSize,MatSize);
    rLeftHandSideMatrix.setZero(MatSize,MatSize);
}
void Boundary_Condition::CalculateAndAddRHS(LocalSystemComponents& rLocalSystem,
                                            ConditionVariables& rVariables,
                                            double& rIntegrationWeight)
{
    VectorType& rRightHandSideVector = rLocalSystem.GetRightHandSideVector();
    // operation performed: rRightHandSideVector += ExtForce*IntToReferenceWeight
    this->CalculateAndAddExternalForces( rRightHandSideVector, rVariables, rIntegrationWeight );
}
void Boundary_Condition::CalculateAndAddExternalForces(VectorType& rRightHandSideVector,
							                           ConditionVariables& rVariables,
							                           double& rIntegrationWeight)

{
    std::cerr << "calling the base class CalculateAndAddExternalForces method for a boundary condition... " << std::endl;
}
double& Boundary_Condition::CalculateAndAddExternalEnergy(double& rEnergy,
                                                          ConditionVariables& rVariables,
                                                          double& rIntegrationWeight,
                                                          const Process_Info& rCurrentProcessInfo)

{

    std::cerr << "calling the base class CalculateAndAddExternalEnergy method for a boundary condition... " << std::endl;
    return rEnergy;
}

void Boundary_Condition::CalculateLeftHandSide( MatrixType& rLeftHandSideMatrix, const Process_Info& rCurrentProcessInfo )
{
    //create local system components
    LocalSystemComponents LocalSystem;

    //calculation flags
    LocalSystem.CalculationFlags.Set(Boundary_Condition::COMPUTE_LHS_MATRIX);

    VectorType RightHandSideVector = VectorType();

    //Initialize sizes for the system components:
    this->InitializeSystemMatrices( rLeftHandSideMatrix, RightHandSideVector, LocalSystem.CalculationFlags );

    //Set Variables to Local system components
    LocalSystem.SetLeftHandSideMatrix(rLeftHandSideMatrix);
    LocalSystem.SetRightHandSideVector(RightHandSideVector);

    //Calculate condition system
    this->CalculateConditionSystem( LocalSystem, rCurrentProcessInfo );

}
void Boundary_Condition::CalculateRightHandSide( VectorType& rRightHandSideVector, const Process_Info& rCurrentProcessInfo )
{
    //create local system components
    LocalSystemComponents LocalSystem;

    //calculation flags
    LocalSystem.CalculationFlags.Set(Boundary_Condition::COMPUTE_RHS_VECTOR);

    MatrixType LeftHandSideMatrix = MatrixType();

    //Initialize sizes for the system components:
    this->InitializeSystemMatrices( LeftHandSideMatrix, rRightHandSideVector, LocalSystem.CalculationFlags );

    //Set Variables to Local system components
    LocalSystem.SetLeftHandSideMatrix(LeftHandSideMatrix);
    LocalSystem.SetRightHandSideVector(rRightHandSideVector);

    //Calculate condition system
    this->CalculateConditionSystem( LocalSystem, rCurrentProcessInfo );
}
void Boundary_Condition::CalculateMassMatrix( MatrixType& rMassMatrix, const Process_Info& rCurrentProcessInfo)
{
    rMassMatrix.resize(0, 0);
}
void Boundary_Condition::CalculateDampingMatrix( MatrixType& rDampingMatrix, const Process_Info& rCurrentProcessInfo)
{
    rDampingMatrix.resize(0, 0);
}



void Boundary_Condition::AddExplicitContribution(const VectorType& rRHS,
                        const Variable<VectorType>& rRHSVariable,
                        const Variable<std::array<double,3> >& rDestinationVariable,
                        const Process_Info& rCurrentProcessInfo)
{
    const SizeType number_of_nodes  = GetGeometry().PointsNumber();
    const SizeType& dimension       = GetGeometry().WorkingSpaceDimension();

    if( rRHSVariable == EXTERNAL_FORCES_VECTOR && rDestinationVariable == EXTERNAL_FORCE )
    {

        for(SizeType i=0; i< number_of_nodes; i++)
        {
            int index = dimension * i;

            std::array<double, 3 > &ExternalForce = GetGeometry()[i].FastGetSolutionStepValue(EXTERNAL_FORCE);
            for(SizeType j=0; j<dimension; j++)
                {
                    ExternalForce[j] += rRHS[index + j];
                }
        }
    }

    if( rRHSVariable == RESIDUAL_VECTOR && rDestinationVariable == FORCE_RESIDUAL )
    {
        for(SizeType i=0; i< number_of_nodes; i++)
        {
            int index = dimension * i;

            std::array<double, 3 > &ForceResidual = GetGeometry()[i].FastGetSolutionStepValue(FORCE_RESIDUAL);
            for(SizeType j=0; j<dimension; j++)
            {
                ForceResidual[j] += rRHS[index + j];
            }
        }
    }

    if( HasVariableDof(ROTATION) )
    {
        if( rRHSVariable == EXTERNAL_FORCES_VECTOR && rDestinationVariable == EXTERNAL_MOMENT )
        {

            for(SizeType i=0; i< number_of_nodes; i++)
            {
                int index = dimension * i;
                std::array<double, 3 > &ExternalForce = GetGeometry()[i].FastGetSolutionStepValue(EXTERNAL_FORCE);
                for(SizeType j=0; j<dimension; j++)
                {
                    ExternalForce[j] += rRHS[index + j];
                }
            }
        }

        if( rRHSVariable == RESIDUAL_VECTOR && rDestinationVariable == MOMENT_RESIDUAL )
        {

            for(SizeType i=0; i< number_of_nodes; i++)
            {
                int index = dimension * i;
                std::array<double, 3 > &MomentResidual = GetGeometry()[i].FastGetSolutionStepValue(MOMENT_RESIDUAL);
                for(SizeType j=0; j<dimension; j++)
                {
                    MomentResidual[j] += rRHS[index + j];
                }
            }
        }
    }
}

void Boundary_Condition::CalculateOnIntegrationPoints( const Variable<double>& rVariable, std::vector<double>& rOutput, const Process_Info& rCurrentProcessInfo )
{
    const unsigned int& integration_points_number = GetGeometry().IntegrationPointsNumber( mThisIntegrationMethod );

    unsigned int integration_points = 0;
    if( integration_points_number == 0 )
      integration_points = 1;

    if ( rOutput.size() != integration_points )
      rOutput.resize( integration_points, false );


    if ( rVariable == EXTERNAL_ENERGY )
    {
        //create and initialize condition variables:
        ConditionVariables Variables;
        this->InitializeConditionVariables(Variables,rCurrentProcessInfo);
        //reading integration points
        const GeometryType::IntegrationPointsVector& integration_points = GetGeometry().IntegrationPoints( mThisIntegrationMethod );

        double Energy = 0;
        double IntegrationWeight = 0;
        for ( unsigned int PointNumber = 0; PointNumber < integration_points.size(); PointNumber++ )
        {
            //compute element kinematics B, F, DN_DX ...
            this->CalculateKinematics(Variables,PointNumber);
            IntegrationWeight = Variables.Jacobian * integration_points[PointNumber].Weight();
            Energy = 0;
            Energy = this->CalculateAndAddExternalEnergy( Energy, Variables, IntegrationWeight, rCurrentProcessInfo);
            rOutput[PointNumber] = Energy;
        }
    }
}



void Boundary_Condition::GetNodalDeltaMovements(VectorType& rValues, const int& rNode)
{
    const SizeType& dimension = GetGeometry().WorkingSpaceDimension();
    if( rValues.size() != dimension )
      rValues.resize(dimension);

    rValues.setZero(dimension);
    VectorType CurrentValueVector(3);
    CurrentValueVector.setZero(3);
    CurrentValueVector = GetNodalCurrentValue( DISPLACEMENT, CurrentValueVector, rNode );
    VectorType PreviousValueVector(3);
    PreviousValueVector.setZero(3);
    CurrentValueVector = GetNodalPreviousValue( DISPLACEMENT, CurrentValueVector, rNode );
    rValues[0] = CurrentValueVector[0] - PreviousValueVector[0];
    rValues[1] = CurrentValueVector[1] - PreviousValueVector[1];
    if( dimension == 3 )
      rValues[2] = CurrentValueVector[2] - PreviousValueVector[2];
}
typename Boundary_Condition::VectorType& Boundary_Condition::GetNodalCurrentValue(const Variable<std::array<double,3> >&rVariable, VectorType& rValue, const unsigned int& rNode)
{
    const SizeType& dimension       = GetGeometry().WorkingSpaceDimension();

    if( rValue.size() != dimension )
      rValue.resize(dimension, false);

    for (int i=0;i<3;i++)
        rValue[i] = GetGeometry()[rNode].FastGetSolutionStepValue(rVariable)[i];

    return rValue;
}
typename Boundary_Condition::VectorType& Boundary_Condition::GetNodalPreviousValue(const Variable<std::array<double,3> >&rVariable, VectorType& rValue, const unsigned int& rNode)
{
    const SizeType& dimension       = GetGeometry().WorkingSpaceDimension();
    if( rValue.size() != dimension )
      rValue.resize(dimension, false);

    for (int i=0;i<3;i++)
        rValue[i] = GetGeometry()[rNode].FastGetSolutionStepValue(rVariable,1)[i];
    return rValue;
}

int Boundary_Condition::Check( const Process_Info& rCurrentProcessInfo ) const
{
    // Perform base element checks
    int ErrorCode = 0;
    ErrorCode = Condition::Check(rCurrentProcessInfo);
    return ErrorCode;
}
