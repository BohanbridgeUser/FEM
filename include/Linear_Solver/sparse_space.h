#ifndef _SPARSE_SPACE_H_
#define _SPARSE_SPACE_H_
#include <Eigen/Eigen>

class SparseSpace
{
    public:
        /// @name Type Define
        /// @{
            typedef Eigen::SparseMatrix<double> SparseMatrix;
            typedef Eigen::SparseVector<double> SparseVector;
        /// @}


        /// @name Life Circle
        /// @{


        /// @}

        /// @name Operators
        /// @{

        /// @}

        /// @name Operations
        /// @{
            static double TwoNorm(Eigen::SparseVector<double> rb) 
            {
                return rb.norm();
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
#endif
