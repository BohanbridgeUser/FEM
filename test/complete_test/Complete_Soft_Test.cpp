#include "../../include/lotus_kernel.h"
<<<<<<< HEAD
=======
#include "../../application/solid_mechanics/include/lotus_solid_mechanics_application.h"

#include "../../include/Linear_Solver/linear_solver.h"
#include "../../include/Solve_Strategy/strategies/linear_solving_strategy.h"
#include "../../include/Solve_Strategy/builder_and_solver/block_builder_and_solver.h"
#include "../../include/Solve_Strategy/schemes/static_scheme.h"
#include "../../include/Solve_Strategy/time_integration_methods/static_method.h"

>>>>>>> 0f56d880bf85eb5fa82c51453c53aea3a72490a2
#include "../../include/process_info.h"
#include "../../include/Model/model.h"
#include "../../include/Variable/variable.h"
#include "../../include/Solve_Strategy/builder_and_solver/block_builder_and_solver.h"
#include "../../include/Solve_Strategy/strategies/linear_solving_strategy.h"
#include "../../include/Solve_Strategy/convergencecriterias/convergencecriterias.h"
#include "../../include/Solve_Strategy/schemes/scheme.h"
#include "../../include/Container/lotus_flags.h"

#include <memory>
int main()
{
    Lotus_Kernel Kernel;
    Lotus_Solid_Mechanics_Application::Pointer pNewApplication = std::make_shared<Lotus_Solid_Mechanics_Application>();
    Kernel.ImportApplication(pNewApplication);
    Kernel.Initialize();

    Variables_List::Pointer V_L = std::make_shared<Variables_List>();
    V_L->Add(DISPLACEMENT);
    V_L->Add(REACTION);
    V_L->Add(REACTION_MOMENT);
    V_L->Add(EXTERNAL_FORCE);
    V_L->Add(EXTERNAL_MOMENT);
    V_L->Add(INTERNAL_FORCE);
    Model Model1;
    Model1.CreateModelPart
    (
        std::string("Cube")
    );
    ModelPart& Cube = Model1.GetModelPart("Cube");
    /** Create Nodes **/
    unsigned int NodeCount = 0;
    for(int i=0;i<9;++i)
    {
        for(int j=0;j<9;++j)
        {
            for(int k=0;k<9;++k)
            {
                Cube.CreateNewNode(NodeCount,1.00*i,1.00*j,1.00*k,V_L);
                Cube.GetNode(NodeCount).AddDof(DISPLACEMENT_X);
                Cube.GetNode(NodeCount).AddDof(DISPLACEMENT_Y);
                Cube.GetNode(NodeCount).AddDof(DISPLACEMENT_Z);
                NodeCount++;
            }
        }
    }
    // for(int i=0;i<NodeCount;++i)
    // {
    //     std::cout << Cube.GetNode(i);
    // }
    /** Create Geometry **/
    unsigned int GeometryCount = 0;
    for (int i=0;i<8;++i) 
    {
        for (int j=0;j<8;++j) 
        {
            for(int k=0;k<8;++k)
            {
                std::vector<ModelPart::IndexType> NodesId;
                NodesId.push_back(i*81+j*9+k);
                NodesId.push_back((i+1)*81+j*9+k);
                NodesId.push_back((i+1)*81+(j+1)*9+k);
                NodesId.push_back(i*81+(j+1)*9+k);
                NodesId.push_back(i*81+j*9+(k+1));
                NodesId.push_back((i+1)*81+j*9+(k+1));
                NodesId.push_back((i+1)*81+(j+1)*9+(k+1));
                NodesId.push_back(i*81+(j+1)*9+(k+1));
                Cube.CreateNewGeometry("Hexahedron",GeometryCount,NodesId);
                GeometryCount++;
            }
        }
    }
    // for(int i=0;i<GeometryCount;++i)
    // {
    //     std::cout << "Hexahedron " << i << "\n" << Cube.GetGeometry(i);
    // }

    /** Create Property **/
    Properties Prop;
    Prop.SetId(1);
    Prop.SetValue(DENSITY,7850.00);
    Prop.SetValue(ELASTICITY_MODULUS,2.1E9);
    Prop.SetValue(POISON,0.31);
    Prop.SetValue(YOUNG_MODULUS,2.1E9);
    Prop.SetValue(POISSON_RATIO,0.31);
    Constitutive_Law const& r_clone_constitutive = Lotus_Components<Constitutive_Law>::Get("Linear_Elastic_3D_Law");

    Prop.SetValue(CONSTITUTIVE_LAW,r_clone_constitutive.Clone());
    Cube.AddProperties(std::make_shared<Properties>(Prop));
    std::cout << "Property DENSITY           : " << Cube.GetProperties(1,0).GetValue(DENSITY) << std::endl;
    std::cout << "Property ELASTICITY_MODULUS: " << Cube.GetProperties(1,0).GetValue(ELASTICITY_MODULUS)<< std::endl;
    std::cout << "Property POISON            : " << Cube.GetProperties(1,0).GetValue(POISON)<< std::endl;
    
    /** Create Elments **/
    unsigned int ElementsCount = 0;
    for(int i=0;i<GeometryCount;++i)
    {
        Cube.CreateNewElement("Small_Displacement_Element3D8N",++ElementsCount,Cube.pGetGeometry(i)->pPointsVector(),Cube.pGetProperties(1));
    }
    // for(int i=0;i<ElementsCount;++i)
    // {
    //     std::cout << "Element " << i << "\n"<< Cube.GetElement(i);
    // }

    /** Create Conditions **/
    unsigned int ConditionCount = 0;
    for(int i=8;i<729;i+=9)
    {
        Geometry< Node >::PointsContainerType N_C;
        N_C.push_back(Cube.pGetNode(i));
        Cube.GetNode(i).SolutionStepData().GetVariablesList().Add(FORCE_RESIDUAL);
        Cube.GetNode(i).SolutionStepData().GetVariablesList().Add(MOMENT_RESIDUAL);
        Cube.CreateNewCondition("Point_Load_Condition3D1N",ConditionCount,N_C,Cube.pGetProperties(1));
        std::array<double, 3 > & PointLoad = Cube.GetCondition(ConditionCount).GetData().GetValue(FORCE_LOAD);
        PointLoad[2] = 100000.00;
        ConditionCount++;
    }
    // for(int i=0;i<ConditionCount;++i)
    // {
    //     std::cout << "Condition " << i << "\n" << Cube.GetCondition(i); 
    // }

    /** Constrain Dofs **/
    for(int i=0;i<729;i+=9)
    {
        Cube.GetNode(i).pGetDof(DISPLACEMENT_X)->FixDof();
        Cube.GetNode(i).pGetDof(DISPLACEMENT_Y)->FixDof();
        Cube.GetNode(i).pGetDof(DISPLACEMENT_Z)->FixDof();
    }

    Process_Info P_I;
    Cube.SetProcessInfo(P_I);
    Cube.SetNodalSolutionStepVariablesList(V_L);
    Cube.SetBufferSize(2);
                                                            

    typedef Static_Scheme<SparseSpace,DenseSpace>
                                                                StaticSchemeType;
    typedef Static_Method<Variable<std::array<double, 3>>,std::array<double, 3>>
                                                                StaticMethodType;
    StaticMethodType::Pointer pTIM = std::make_shared<StaticMethodType>(DISPLACEMENT);
    StaticSchemeType::IntegrationMethodsVectorType v_TIM;
    v_TIM.push_back(pTIM);                                                            
    
    StaticSchemeType::Pointer S_P 
    = std::make_shared<Static_Scheme<SparseSpace,DenseSpace> >(v_TIM);
    typedef Linear_Solver<SparseSpace,DenseSpace>                    SolverType;
    SolverType::Pointer pSolver = std::make_shared<SolverType>();

    typedef Block_Builder_And_Solver<SparseSpace,
                             DenseSpace,
                             Linear_Solver<SparseSpace,DenseSpace> > BuilderAndSolverType;
    BuilderAndSolverType::Pointer B_S = std::make_shared<BuilderAndSolverType>(pSolver);

    Flags SolverFlag;
    SolverFlag.Set(Solver_Local_Flags::COMPUTE_REACTIONS);
    Linear_Solving_Strategy<SparseSpace,
                            DenseSpace,
                            Linear_Solver<SparseSpace,DenseSpace> > solver(Cube,S_P,B_S,SolverFlag);
    
    std::cout << "Check Solve Input : " << solver.Check() << std::endl;

    solver.SetEchoLevel(3);
    solver.InitializeSolutionStep();
    solver.SolveSolutionStep();
    solver.FinalizeSolutionStep();
    return 0;
}