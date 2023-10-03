#ifndef _LINEAR_SOLVER_H_
#define _LINEAR_SOLVER_H_
#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "../define.h"
#include "dense_space.h"
#include "sparse_space.h"
template<typename TSparseSpace, typename TDenseSpace>
class Linear_Solver
{
    public:
        /// @name Type Define
        /// @{
            typedef Linear_Solver<TSparseSpace,TDenseSpace> 
                                                                    ClassType;
            typedef typename TSparseSpace::SparseMatrix
                                                                 SpMatrixType;
            typedef typename TSparseSpace::SparseVector
                                                                 SpVectorType;
            LOTUS_SHARED_POINTER_DEFINE(ClassType)

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
            virtual void Clear()
            {

            }

            virtual void Solve(const SpMatrixType& rA, SpVectorType& rDx, const SpVectorType& rb)  
            {
                Eigen::SparseLU<Eigen::SparseMatrix<double> > LU_Solver;
                LU_Solver.compute(rA);
                if(LU_Solver.info() != Eigen::Success)
                {
                    std::cerr << "Matrix Decomposiont Failed!\n";
                    exit(0);
                }
                rDx = LU_Solver.solve(rb);
                if(LU_Solver.info() != Eigen::Success)
                {
                    std::cerr << "Matrix Solving Failed!\n";
                    exit(0);
                }
            }  
            
        /// @}


        /// @name Access
        /// @{


        /// @}


        /// @name Inquiry
        /// @{
            bool AdditionalPhysicalDataIsNeeded()
            {
                return false;
            }

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
template<typename TSparseSpace, typename TDenseSpace>
std::ostream& operator<<(std::ostream& os, Linear_Solver<TSparseSpace,TDenseSpace> const& item)
{
    os << "Basic Linear Solver!\n";
    return os;
}
#endif