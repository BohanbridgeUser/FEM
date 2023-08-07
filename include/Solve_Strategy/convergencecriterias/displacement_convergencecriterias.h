#ifndef _DISPLACEMENT_CONVERGENCECRITERIAS_H_
#define _DISPLACEMENT_CONVERGENCECRITERIAS_H_
#include "convergencecriterias.h"

template<typename TSpaeseSpace, typename TDenseSpace>
class Displacement_Convergencecriterias : public ConvergenceCriterias<TSpaeseSpace,TDenseSpace>
{
    public:
        /// @name Type Define
        /// @{
            typedef Displacement_Convergencecriterias<TSpaeseSpace,TDenseSpace> ClassType;
            LOTUS_POINTER_DEFINE(ClassType)

        /// @}

        
    protected:
        ///@name Protected static Member Variables
        ///@{

        ///@}
        ///@name Protected member Variables
        ///@{

        ///@}
        ///@name Protected Operators
        ///@{

        ///@}
        ///@name Protected Operations
        ///@{

        ///@}
};

#endif