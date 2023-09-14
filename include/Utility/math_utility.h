#ifndef _MATH_UTILITY_H_
#define _MATH_UTILITY_H_
#include "../Geometry/geometry.h"

/**
 * Transform a rotation vector to a skew symmetric matrix
 * @param rVector rotation vector (input parameter)
 * @param rSkewSymmetricTensor skew symmetric matrix (output parameter)
 */
template<class TVector3, class TMatrix3>
static inline void VectorToSkewSymmetricTensor(const TVector3& rVector, TMatrix3& rSkewSymmetricTensor)
{
    //Initialize Local Matrices
    if( rSkewSymmetricTensor.rows() != 3 )
        rSkewSymmetricTensor.resize(3, 3);

    rSkewSymmetricTensor( 0, 0 ) = 0.0;
    rSkewSymmetricTensor( 1, 1 ) = 0.0;
    rSkewSymmetricTensor( 2, 2 ) = 0.0;

    rSkewSymmetricTensor( 0, 1 ) = -rVector[2];
    rSkewSymmetricTensor( 0, 2 ) =  rVector[1];
    rSkewSymmetricTensor( 1, 2 ) = -rVector[0];

    rSkewSymmetricTensor( 1, 0 ) =  rVector[2];
    rSkewSymmetricTensor( 2, 0 ) = -rVector[1];
    rSkewSymmetricTensor( 2, 1 ) =  rVector[0];
}

/**
 * Add a Nodal vector to a Elemental vector
 * @param rInputVector LocalNodalVector (input parameter)
 * @param rOutputVector LocalElementalVector (output parameter)
 * @param InitialRow InitialRowNumber, initial index of the OutputVector
 * note the initialization of the outputvector must be done previously to the call of the method
 */
typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                                               MatrixType;
typedef Eigen::Matrix<double,Eigen::Dynamic,1>
                                                                               VectorType;
static inline void AddVector(const VectorType& rInputVector,VectorType& rOutputVector,const unsigned int InitialRow)
{
    for(unsigned int i=0; i<rInputVector.size(); i++)
        rOutputVector[InitialRow+i] += rInputVector[i];
}

template<typename TMatrixType>
static inline double GerneralizedDet(TMatrixType const& rA)
{
    if (rA.rows() == rA.cols()) 
    {
        return rA.determinant();
    } else if (rA.rows() < rA.cols()) 
    { // Right determinant
        const MatrixType AAT = rA * rA.transpose();
        return std::sqrt(AAT.determinant());
    } else 
    { // Left determinant
        const MatrixType ATA = rA.transpose() * rA;
        return std::sqrt(ATA.determinant());
    }
}
#endif