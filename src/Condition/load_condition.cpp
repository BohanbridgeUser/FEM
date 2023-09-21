#include "../../include/Condition/load_condition.h"

/*******************************************Life Circle**************************************/
Load_Condition::Load_Condition()
:Boundary_Condition()
{

}
Load_Condition::Load_Condition(IndexType NewID,
                               GeometryType::Pointer pThisGeometry)
:Boundary_Condition(NewID,pThisGeometry)
{

}
Load_Condition::Load_Condition(IndexType NewID,
                               GeometryType::Pointer pThisGeometry,
                               Properties::Pointer pThisProperties)
:Boundary_Condition(NewID,pThisGeometry,pThisProperties)
{

}
Load_Condition::Load_Condition(Load_Condition const& another)
:Boundary_Condition(another)
{

}

Condition::SharedPointer Load_Condition::Create(IndexType NewID,
                                                NodesContainerType const& rThisNodes,
                                                Properties::Pointer pThisProperties)const
{
    return std::make_shared<Load_Condition>(NewID,GetGeometry().Create(rThisNodes),pThisProperties);
}

Condition::SharedPointer Load_Condition::Clone(IndexType NewID,
                                               NodesContainerType const& rThisNodes)const
{
    Load_Condition NewCondition(NewID,GetGeometry().Create(rThisNodes),pGetProperties());
    NewCondition.SetData(this->GetData());
    NewCondition.SetFlags(this->GetFlag());
    return std::make_shared<Load_Condition>(NewCondition);
}

/***********************************Utilitu Operations******************************************/
int Load_Condition::Check(Process_Info const& rThisProcessInfo)const
{
    int ErrorCode = 0;
    ErrorCode = Boundary_Condition::Check(rThisProcessInfo);
    return ErrorCode;
}

/************************************Initialization********************************************/
void Load_Condition::InitializeConditionVariables(ConditionVariables& rVariable,
                                                  Process_Info const& rCurrentProcessInfo)
{
    Boundary_Condition::InitializeConditionVariables(rVariable,rCurrentProcessInfo);
}

/************************************Calculation***********************************************/
void Load_Condition::CalculateExternalLoad(ConditionVariables& rVariable)
{
    std::cerr << "calling the base class CalculateExternalLoad method for a load condition... " << std::endl;
    exit(0);
}

void Load_Condition::CalculateAndAddExternalForces(VectorType& rRightHandSideVector,
                                                   ConditionVariables& rVariable,
                                                   double& rIntegrationWeight)
{
    const SizeType number_of_nodes = GetGeometry().PointsNumber();
    const SizeType dimension       = GetGeometry().WorkingSpaceDimension();
    unsigned int index = 0;
    for (int i=0;i<number_of_nodes;++i) 
    {
        index = dimension * i;
        for (int j=0;j<dimension;++j) 
        {
            rRightHandSideVector[index+j] += rVariable.N[i] * rVariable.ExternalVectorValue[j] * rIntegrationWeight;
        }
    }
    if( this->HasVariableDof(ROTATION) )
    {
      // external load
      VectorType ExternalLoad(3);
      ExternalLoad.setZero(3);
      VectorType ExternalCouple(3);
      ExternalCouple.setZero(3);
      MatrixType SkewSymMatrix(3,3);
      SkewSymMatrix.setZero(3,3);
      VectorType IntegrationPointPosition(3);
      IntegrationPointPosition.setZero(3);
      VectorType CurrentValueVector(3);
      CurrentValueVector.setZero(3);

      for ( SizeType i = 0; i < number_of_nodes; i++ )
      {
        for (int j=0;j<3;j++)
            CurrentValueVector[j] = GetGeometry()[i].Coordinates()[j];
        IntegrationPointPosition += rVariable.N[i] * CurrentValueVector;
      }

      unsigned int RowIndex = 0;
      for ( SizeType i = 0; i < number_of_nodes; i++ )
      {
          RowIndex = i * ( (dimension-1) * 3 );
          ExternalLoad  = rIntegrationWeight * rVariable.N[i] * rVariable.ExternalVectorValue * rIntegrationWeight;
          //integration for the moment force vector
          VectorToSkewSymmetricTensor(ExternalLoad,SkewSymMatrix); // m = f x r = skewF · r
          for (int cor=0;cor<3;++cor)
            CurrentValueVector[cor] = GetGeometry()[i].Coordinates()[cor];
          CurrentValueVector -= IntegrationPointPosition;
          ExternalCouple = SkewSymMatrix*CurrentValueVector;
          if( dimension == 2 )
          {
            ExternalLoad[2] = ExternalCouple[2];
            AddVector(ExternalCouple,  rRightHandSideVector, RowIndex);
          }
          else
          {
            AddVector(ExternalLoad,  rRightHandSideVector, RowIndex);
            AddVector(ExternalCouple, rRightHandSideVector, RowIndex+dimension);
          }
      }
    }
}

double& Load_Condition::CalculateAndAddExternalEnergy(double& rEnergy,
                                                      ConditionVariables& rVariables,
						                              double& rIntegrationWeight,
						                              const Process_Info& rCurrentProcessInfo)

{
    const SizeType number_of_nodes = GetGeometry().PointsNumber();
    const SizeType& dimension = GetGeometry().WorkingSpaceDimension();

    // Energy Calculation:
    VectorType CurrentValueVector(dimension);
    CurrentValueVector.setZero(dimension);
    VectorType Displacements(dimension);
    Displacements.setZero(dimension);
    for ( SizeType i = 0; i < number_of_nodes; i++ )
    {
        //current displacements to compute energy
        CurrentValueVector = GetNodalCurrentValue( DISPLACEMENT, CurrentValueVector, i );
        Displacements += rVariables.N[i] * CurrentValueVector;
    }
    //------
    VectorType ForceVector(dimension);
    ForceVector.setZero(dimension);
    for ( SizeType i = 0; i < number_of_nodes; i++ )
    {
        ForceVector += rVariables.N[i] * rVariables.ExternalVectorValue * rIntegrationWeight;
    }
    rEnergy += ForceVector.dot(Displacements);

    return rEnergy;
}

