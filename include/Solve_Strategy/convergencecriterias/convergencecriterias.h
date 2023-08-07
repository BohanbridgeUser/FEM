#ifndef _CONVERGENCECRITERIAS_H_
#define _CONVERGENCECRITERIAS_H_
#include "../../define.h"

template<typename TSparseSpace, typename TDenseSpace>
class ConvergenceCriterias
{
    public:
        /// @name Type Define
        /// @{
            typedef ConvergenceCriterias<TSparseSpace,TDenseSpace> ConvergenceCriteriasType;
            LOTUS_POINTER_DEFINE(ConvergenceCriteriasType)
        /// @}

        /// @name Life Circle
        /// @{
            explicit ConvergenceCriterias()
            {

            }
            virtual ~ConvergenceCriterias()
            {
                
            }
        /// @}
    protected:
        ///@name Protected static Member Variables
        ///@{

        ///@}
        ///@name Protected member Variables
        ///@{
            /**
             * @brief This "flag" is set in order to know if it is necessary to actualize the RHS
            */
            bool mActualizeRHSIsNeeded = false;             

            /**
             * @brief This "flag" is set in order to know if it is convergence criteria is initialized
            */
            bool mConvergenceCriteriaIsInitialized = false;

            /**
             * @brief The echo level
            */
            int mEchoLevel;  
        ///@}
        ///@name Protected Operators
        ///@{

        ///@}
        ///@name Protected Operations
        ///@{

        ///@}
};
#endif