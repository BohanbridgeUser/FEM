#ifndef _STRATEGIES_H_
#define _STRATEGIES_H_
#include "../../define.h"
#include "../../Model/model.h"
#include "../schemes/scheme.h"
#include "../builder_and_solver/builder_and_solver.h"
#include "../convergencecriterias/convergencecriterias.h"
#include <Eigen/Sparse>
#include <Eigen/Dense>

template<typename TSparseSpace, typename TDenseSpace, typename TLinearSolver>
class Solve_Stategy
{
    public:
        ///@name Define 
        ///@{
            typedef Solve_Stategy<TSparseSpace,TDenseSpace,TLinearSolver> 
                                                                          Solve_StategyType;
            LOTUS_POINTER_DEFINE(Solve_StategyType)

            typedef Model_Part                                              
                                                                              ModelPartType;
        ///@}

        ///@name Lift Circle
        ///@{
            /* Constructor */
            explicit Solve_Stategy()
            {

            }
            explicit Solve_Stategy(Model_Part& rModelPart)
                                  :mrModelPart(rModelPart)
            {

            }
            /* Destructor */
            virtual ~Solve_Stategy()
            {

            }
        ///@}

        ///@name Operations
        ///@{
            /**
             * @brief Initialization of member 
            */
            virtual void Initialize()
            {

            }
            virtual void InitializeSolutionStep()
            {

            }
            virtual void Predict() 
            {

            }
            virtual bool SolveSolutionStep()
            {
                return true;
            }
            virtual void FinalizeSolutionStep()
            {

            }
            virtual double Solve()
            {
                Initialize();
                InitializeSolutionStep();
                Predict();
                SolveSolutionStep();
                FinalizeSolutionStep();
            }
            
        ///@}
    private:
        Model_Part& mrModelPart;
};
#endif