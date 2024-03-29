#ifndef _SCHEME_H_
#define _SCHEME_H_
#include <Eigen/Eigen>
#include "../../define.h"
#include "../../Node/dof.h"
#include "../../Model/model.h"
#include "../solve_local_flags.h"
#include "../../Process/solver_process.h"
template<typename TSparseSpace, typename TDenseSpace>
class Scheme : public Flags
{
    public:
        ///@name Define
        ///@{
            typedef Scheme<TSparseSpace,TDenseSpace> 
                                                                                         schemeType;
            LOTUS_SHARED_POINTER_DEFINE(schemeType)                    
            typedef Flags                               
                                                                                           BaseType;
            typedef Solver_Local_Flags
                                                                                     LocalFlagType;
            typedef Dof<double>                                                                     
                                                                                            DofType;
            typedef std::vector<DofType>                                          
                                                                                  DofsContainerType;
            typedef ModelPart
                                                                                      ModelPartType;
            typedef ModelPart
                                                                                         Model_Part;
            typedef Model_Part::NodesContainerType                               
                                                                                 NodesContainerType;
            typedef Model_Part::ElementsContainerType                         
                                                                              ElementsContainerType;
            typedef Model_Part::ConditionsContainerType                     
                                                                            ConditionsContainerType;
            typedef Solver_Process
                                                                                        ProcessType;
            typedef typename Solver_Process::SharedPointer        
                                                                                 ProcessPointerType;
            typedef std::vector<ProcessPointerType>     
                                                                           ProcessPointerVectorType;
            typedef typename TSparseSpace::SparseMatrix
                                                                                       SpMatrixType;
            typedef typename TSparseSpace::SparseVector  
                                                                                       SpVectorType;
            typedef SpMatrixType  
                                                                                   GlobalMatrixType;
            typedef SpVectorType                                                     
                                                                                   GlobalVectorType;
            typedef typename std::shared_ptr<SpMatrixType>
                                                                            GlobalMatrixTypePointer;
            typedef typename std::shared_ptr<SpVectorType>
                                                                            GlobalVectorTypePointer;
            typedef typename TDenseSpace::Matrix
                                                                                    LocalMatrixType;
            typedef typename TDenseSpace::Vector
                                                                                    LocalVectorType;
        ///@}

        ///@name Life Circle
        ///@{
            // Constructor
            explicit Scheme():BaseType()
            {
                this->Set(LocalFlagType::INITIALIZED,false);
                mOptions.Set(LocalFlagType::ELEMENTS_INITIALIZED,false);
                mOptions.Set(LocalFlagType::CONSTANT_SYSTEM_MATRIX,false);
            }

            explicit Scheme(const Scheme& another)
            :BaseType(another),
             mOptions(another.mOptions)
            {

            }

            // Destructor
            virtual ~Scheme()
            {
                
            }
        ///@}

        ///@name Operations
        ///@{
            virtual void Initialize(Model_Part& rModelPart)
            {
                this->InitializeElements(rModelPart);

                this->InitializeConditions(rModelPart);

                this->Set(LocalFlagType::INITIALIZED, true);
            }
            
            virtual void InitializeSolutionStep(Model_Part& rModelPart)
            {
                Process_Info& rCurrentProcessInfo = rModelPart.GetProcessInfo();

                for(typename ProcessPointerVectorType::iterator it=mProcesses.begin(); it!=mProcesses.end(); ++it)
                    (*it)->ExecuteInitializeSolutionStep();

                for(int i=0; i<static_cast<int>(rModelPart.Elements().size()); i++)
                {
                    auto itElem = rModelPart.Elements().begin() + i;
                    itElem->InitializeSolutionStep(rCurrentProcessInfo);
                }

                for(int i=0; i<static_cast<int>(rModelPart.Conditions().size()); i++)
                {
                    auto itCond = rModelPart.Conditions().begin() + i;
                    itCond->InitializeSolutionStep(rCurrentProcessInfo);
                }
            }

            /**
             * @brief Performs all the required operations that should be done (for each step) after solving the solution step.
             * @details this function must be called only once per step.
             */
            virtual void FinalizeSolutionStep(Model_Part& rModelPart)
            {
                Process_Info& rCurrentProcessInfo = rModelPart.GetProcessInfo();

                for(typename ProcessPointerVectorType::iterator it=mProcesses.begin(); it!=mProcesses.end(); ++it)
                    (*it)->ExecuteFinalizeSolutionStep(); 

                for(int i=0; i<static_cast<int>(rModelPart.Elements().size()); i++)
                {
                    auto itElem = rModelPart.Elements().begin() + i;
                    itElem->FinalizeSolutionStep(rCurrentProcessInfo);
                }

                for(int i=0; i<static_cast<int>(rModelPart.Conditions().size()); i++)
                {
                    auto itCond = rModelPart.Conditions().begin() + i;
                    itCond->FinalizeSolutionStep(rCurrentProcessInfo);
                }

            }

            /**
             * @brief Performing the prediction of the solution.
             * @details this function must be called only once per step.
             */
            virtual void Predict(Model_Part& rModelPart,
                                DofsContainerType& rDofSet,
                                GlobalVectorTypePointer& rDx)
            {
    
            }
            
            virtual void Update(Model_Part& rModelPart,
                                DofsContainerType& rDofSet,
                                SpVectorType& rmpDx)
            {
                this->UpdateDofs(rModelPart,rDofSet,rmpDx);
                this->UpdateVariables(rModelPart);
                this->MoveMesh(rModelPart);
            }
            /**
             * @brief Performing the update of the solution Dofs
             * @details this function must be called only once per iteration
             */
            virtual void UpdateDofs(Model_Part& rModelPart,
                               DofsContainerType& rDofSet,
                               SpVectorType& rmpDx)
            {

            }
            /**
             * @brief Performing the update of the solution variables
             * @details this function must be called only once per iteration
             */
            virtual void UpdateVariables(Model_Part& rModelPart)
            {
            }
            virtual void MeshMove(Model_Part& rModelPart)
            {

            }
            
            /**
             * Function that returns the list of Degrees of freedom to be assembled in the system for a Given Element
             * @param rCurrentElement: The element to compute
             * @param rElementalDofsList: The element dofs list
             * @param rCurrentProcessInfo: The current process info instance
             */
            virtual void GetElementalDofList(Element::Pointer pCurrentElement,
                                            Element::DofsVectorType& rElementalDofList,
                                            Process_Info& rCurrentProcessInfo)
            {
                pCurrentElement->GetDofList(rElementalDofList, rCurrentProcessInfo);
            }
            
            /**
             * Function that returns the list of Degrees of freedom to be assembled in the system for a Given Element
             * @param rCurrentCondition: The condition to compute
             * @param rConditionDofsList: The condition dofs list
             * @param rCurrentProcessInfo: The current process info instance
             */
            virtual void GetConditionDofList(Condition::Pointer pCurrentCondition,
                                            Element::DofsVectorType& rConditionDofList,
                                            Process_Info& rCurrentProcessInfo)
            {
                //pCurrentCondition->GetDofList(rConditionDofList, rCurrentProcessInfo);
            }

            /**
             * This function is designed to be called in the builder and solver to introduce
             * @param pCurrentElement: The element to compute
             * @param rLHS_Contribution: The LHS matrix contribution
             * @param rRHS_Contribution: The RHS vector contribution
             * @param rEquationId: The ID's of the element degrees of freedom
             * @param rCurrentProcessInfo: The current process info instance
             */
            virtual void CalculateSystemContributions(Element::Pointer pCurrentElement,
                                                        LocalMatrixType& rLHS_Contribution,
                                                        LocalVectorType& rRHS_Contribution,
                                                        Element::EquationIdVectorType& rEquationId,
                                                        Process_Info& rCurrentProcessInfo)
            {
                pCurrentElement->CalculateLocalSystem(rLHS_Contribution, rRHS_Contribution, rCurrentProcessInfo);
                pCurrentElement->EquationIdVector(rEquationId, rCurrentProcessInfo);
            }

            /**
             * Functions totally analogous to the precedent but applied to the "condition" objects
             * @param pCurrentCondition: The condition to compute
             * @param rLHS_Contribution: The LHS matrix contribution
             * @param rRHS_Contribution: The RHS vector contribution
             * @param rEquationId: The ID's of the element degrees of freedom
             * @param rCurrentProcessInfo: The current process info instance
             */
            virtual void Condition_CalculateSystemContributions(Condition::Pointer pCurrentCondition,
                                                                LocalMatrixType& rLHS_Contribution,
                                                                LocalVectorType& rRHS_Contribution,
                                                                Element::EquationIdVectorType& rEquationId,
                                                                Process_Info& rCurrentProcessInfo)
            {
                pCurrentCondition->CalculateLocalSystem(rLHS_Contribution, rRHS_Contribution, rCurrentProcessInfo);
                pCurrentCondition->EquationIdVector(rEquationId, rCurrentProcessInfo);
            }

            /**
             * @brief Liberates internal storage for an element
             */
            virtual void Clear(Element::Pointer rCurrentElement)
            {
            }
            /**
             * @brief Liberates internal storage for a condition
             */
            virtual void Clear(Condition::Pointer rCurrentCondition)
            {
            }
            virtual void Clear()
            {

            }

        /// @}

        /// @name Access
        /// @{
            
        /// @}
    protected:
        ///@name Static Member Variables
        ///@{

        ///@}
        ///@name Member Variables
        ///@{
            /**
             * @brief Options includes flags
             * @param ELEMENT_INITIALIZED
             * @param CONDITION_INITIALIZED
             * @details BaseType tells if Scheme is initialized
            */
            Flags mOptions;

            ProcessPointerVectorType mProcesses;
        ///@}
        ///@name Private Operators
        ///@{

        ///@}
        ///@name Private Operations
        ///@{
            virtual void InitializeElements(Model_Part& rModelPart)
            {
                if (this->IsNot(Solver_Local_Flags::INITIALIZED))
                {
                    for (int i=0;i<static_cast<int>(rModelPart.Elements().size());i++)
                    {
                        auto rElement = rModelPart.Elements().begin() + i;
                        rElement->Initialize(rModelPart.GetProcessInfo());
                    }
                    this->Set(Solver_Local_Flags::ELEMENTS_INITIALIZED, true);
                }
            }
            virtual void InitializeConditions(Model_Part& rModelPart)
            {
                if (this->IsNot(Solver_Local_Flags::INITIALIZED))
                {
                    for (int i=0;i<static_cast<int>(rModelPart.Conditions().size());i++)
                    {
                        auto rCondition = rModelPart.Conditions().begin() + i;
                        rCondition->Initialize(rModelPart.GetProcessInfo());
                    }
                    this->Set(Solver_Local_Flags::ELEMENTS_INITIALIZED, true);
                }
            }
      
        ///@}
        ///@name Private  Access
        ///@{

        ///@}
        ///@name Private Inquiry
        ///@{

        ///@}
        ///@name Un accessible methods
        ///@{

        ///@}
            
    private:
        ///@name Static Member Variables
        ///@{

        ///@}
        ///@name Member Variables
        ///@{

        ///@}
        ///@name Private Operators
        ///@{

        ///@}
        ///@name Private Operations
        ///@{

        ///@}
        ///@name Private  Access
        ///@{

        ///@}
        ///@name Private Inquiry
        ///@{

        ///@}
        ///@name Un accessible methods
        ///@{

        ///@}
};

#endif