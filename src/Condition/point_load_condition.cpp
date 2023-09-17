#include "../../include/Condition/point_load_condition.h"

/********************************Life Circle************************************/
Point_Load_Condition::Point_Load_Condition()
:Load_Condition()
{

}
Point_Load_Condition::Point_Load_Condition(IndexType NewId,
                                           GeometryType::Pointer pThisGeometry)                                    
:Load_Condition(NewId,pThisGeometry)
{

}
Point_Load_Condition::Point_Load_Condition(IndexType NewId,
                                           GeometryType::Pointer pThisGeometry,
                                           Properties::Pointer pThisProperties)
:Load_Condition(NewId,pThisGeometry,pThisProperties)
{
    mThisIntegrationMethod = GetGeometry().GetDefaultIntegrationMethod();
}
Point_Load_Condition::Point_Load_Condition(Point_Load_Condition const& another)
:Load_Condition(another)
{

}

/*******************************Utility Operations******************************/
Condition::SharedPointer Point_Load_Condition::Create(IndexType NewId,
                                                      NodesContainerType const& rThisNodes,
                                                      Properties::Pointer pThisProperties)const
{
    return std::make_shared<Point_Load_Condition>(NewId,&(*GetGeometry().Create(rThisNodes)),pThisProperties);
}
Condition::SharedPointer Point_Load_Condition::Clone(IndexType NewId,
                                                     NodesContainerType const& rThisNodes)const
{
    Point_Load_Condition NewCondition(NewId,&(*GetGeometry().Create(rThisNodes)),pGetProperties());
    NewCondition.SetData(this->GetData());
    NewCondition.SetFlags(this->GetFlag());
    return std::make_shared<Point_Load_Condition>(NewCondition);
}
int Point_Load_Condition::Check(Process_Info const& rCurrentProcessInfo) const
{
    int ErrorNumber = 0;
    ErrorNumber = Load_Condition::Check(rCurrentProcessInfo);
    return ErrorNumber;
}

/*****************************Initialization**************************************/
void Point_Load_Condition::InitializeConditionVariables(ConditionVariables& rVariable,
                                                        Process_Info const& rCurrentProcessInfo)
{
    const int number_of_nodes = GetGeometry().PointsNumber();
    const int dimension = GetGeometry().WorkingSpaceDimension();
    const unsigned int local_dimension = GetGeometry().LocalSpaceDimension();
    rVariable.Initialize(dimension,local_dimension,number_of_nodes);
    rVariable.N[0] = 1.0;
}

/*******************************Calculation**************************************/
void Point_Load_Condition::CalculateKinematics(ConditionVariables& rVariable,
                                               double const& rPointNumber)
{
    rVariable.Jacobian = 1.0;
    this->CalculateExternalLoad(rVariable);
}
void Point_Load_Condition::CalculateExternalLoad(ConditionVariables& rVariable)
{
    const SizeType number_of_nodes = GetGeometry().size();
    const SizeType& dimension      = GetGeometry().WorkingSpaceDimension();

    if( rVariable.ExternalVectorValue.size() != dimension )
        rVariable.ExternalVectorValue.resize(dimension);

    rVariable.ExternalVectorValue.setZero(dimension);

    //FORCE CONDITION:
    //defined on condition
    if( this->Has( FORCE_LOAD ) )
    {
        std::array<double, 3 > & PointLoad = this->GetValue( FORCE_LOAD );
        for ( SizeType i = 0; i < number_of_nodes; i++ )
        {
            for( SizeType k = 0; k < dimension; k++ )
                rVariable.ExternalVectorValue[k] += rVariable.N[i] * PointLoad[k];
        }
    }

    //defined on condition nodes
    if( this->Has( FORCE_LOAD_VECTOR ) ){
        std::array<double,3>& PointLoads = this->GetValue( FORCE_LOAD_VECTOR );
        unsigned int counter = 0;
        for ( SizeType i = 0; i < number_of_nodes; i++ )
        {
        counter = i*3;
        for( SizeType k = 0; k < dimension; k++ )
            {
                rVariable.ExternalVectorValue[k] += rVariable.N[i] * PointLoads[counter+k];
            }

        }
    }

    //defined on condition nodes (legacy)
    for (SizeType i = 0; i < number_of_nodes; i++)
    {
        if( GetGeometry()[i].SolutionStepsDataHas( POINT_LOAD ) ){
          std::array<double, 3 > & PointLoad = GetGeometry()[i].FastGetSolutionStepValue( POINT_LOAD );
          for( SizeType k = 0; k < dimension; k++ )
            rVariable.ExternalVectorValue[k] += rVariable.N[i] * PointLoad[k];

        }
    }

    //defined on condition nodes
    for (SizeType i = 0; i < number_of_nodes; i++)
    {
        if( GetGeometry()[i].SolutionStepsDataHas( FORCE_LOAD ) ){
          std::array<double, 3 > & PointLoad = GetGeometry()[i].FastGetSolutionStepValue( FORCE_LOAD );
          for( SizeType k = 0; k < dimension; k++ )
            rVariable.ExternalVectorValue[k] += rVariable.N[i] * PointLoad[k];

        }
    }
}
void Point_Load_Condition::CalculateConditionSystem(LocalSystemComponents& rLocalSystem,
                                                    const Process_Info& rCurrentProcessInfo)
{
    ConditionVariables Variable;
    this->InitializeConditionVariables(Variable,rCurrentProcessInfo);
    unsigned int PointNumber = 0;
    this->CalculateKinematics(Variable,PointNumber);
    double IntegrationWeight = 1;

    if (rLocalSystem.CalculationFlags.Is(Point_Load_Condition::COMPUTE_LHS_MATRIX))
    {
        this->CalculateAndAddLHS(rLocalSystem,Variable,IntegrationWeight);
    }
}

