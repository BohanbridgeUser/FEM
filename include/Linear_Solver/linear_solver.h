#ifndef _LINEAR_SOLVER_H_
#define _LINEAR_SOLVER_H_
#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "../define.h"
template<typename TSparseSpace, typename TDenseSpace>
class Linear_Solver
{
    public:
        /// @name Type Define
        /// @{
            typedef Linear_Solver<TSparseSpace,TDenseSpace> 
                                                                    ClassType;
            LOTUS_POINTER_DEFINE(ClassType)

        /// @}


        /// @name Life Circle
        /// @{
            Linear_Solver()
            {

            }

            ~Linear_Solver()
            {

            }
        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{
            virtual void Clear() override
            {

            }

        /// @}


        /// @name Access
        /// @{


        /// @}


        /// @name Inquiry
        /// @{


        /// @}


    protected:
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


        /// @}


        /// @name Protected Access
        /// @{


        /// @}


        /// @name Protected Inquiry
        /// @{


        /// @}


    private:
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
};

#endif