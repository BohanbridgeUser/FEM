#ifndef _DENSE_SPACE_H_
#define _DENSE_SPACE_H_
#include <Eigen/Eigen>
#include "../define.h"
class DenseSpace
{
    public:
        /// @name Type Define
        /// @{
            typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> Matrix;
            typedef Eigen::Matrix<double,Eigen::Dynamic, 1> Vector;
            typedef std::shared_ptr<Matrix> MatrixPointer;
            typedef std::shared_ptr<Vector> VectorPointer;
        /// @}


        /// @name Life Circle
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