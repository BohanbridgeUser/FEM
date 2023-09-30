#ifndef _MATH_UTILITY_H_
#define _MATH_UTILITY_H_
#include "../Node/node.h"
#include "../Geometry/geometry.h"

typedef size_t
                                                        SizeType;
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

/**
 * @brief Transforms a strain vector into a matrix. Strains are assumed to be stored in the following way:
 * \f$ [ e11, e22, e33, 2*e12, 2*e23, 2*e13 ] \f$ for 3D case and
 * \f$ [ e11, e22, e33, 2*e12 ] \f$ for 2D case.
 * \f$ [ e11, e22, 2*e12 ] \f$ for 2D case.
 * @details Hence the deviatoric components of the strain vector are divided by 2 while they are stored into the matrix
 * @param rStrainVector the given strain vector
 * @return the corresponding strain tensor in matrix form
 * @tparam TVector The vector type considered
 * @tparam TMatrixType The matrix returning type
 */
template<class TVector, class TMatrixType = MatrixType>
static inline TMatrixType StrainVectorToTensor( const TVector& rStrainVector)
{
    typedef size_t
                                                                SizeType;

    const SizeType matrix_size = rStrainVector.size() == 3 ? 2 : 3;
    TMatrixType strain_tensor(matrix_size, matrix_size);

    if (rStrainVector.size()==3) {
        strain_tensor(0,0) = rStrainVector[0];
        strain_tensor(0,1) = 0.5*rStrainVector[2];
        strain_tensor(1,0) = 0.5*rStrainVector[2];
        strain_tensor(1,1) = rStrainVector[1];
    } else if (rStrainVector.size()==4) {
        strain_tensor(0,0) = rStrainVector[0];
        strain_tensor(0,1) = 0.5*rStrainVector[3];
        strain_tensor(0,2) = 0;
        strain_tensor(1,0) = 0.5*rStrainVector[3];
        strain_tensor(1,1) = rStrainVector[1];
        strain_tensor(1,2) = 0;
        strain_tensor(2,0) = 0;
        strain_tensor(2,1) = 0;
        strain_tensor(2,2) = rStrainVector[2];
    } else if (rStrainVector.size()==6) {
        strain_tensor(0,0) = rStrainVector[0];
        strain_tensor(0,1) = 0.5*rStrainVector[3];
        strain_tensor(0,2) = 0.5*rStrainVector[5];
        strain_tensor(1,0) = 0.5*rStrainVector[3];
        strain_tensor(1,1) = rStrainVector[1];
        strain_tensor(1,2) = 0.5*rStrainVector[4];
        strain_tensor(2,0) = 0.5*rStrainVector[5];
        strain_tensor(2,1) = 0.5*rStrainVector[4];
        strain_tensor(2,2) = rStrainVector[2];
    }

    return strain_tensor;
}
/**
 * @brief Transforms a given symmetric Strain Tensor to Voigt Notation:
 * @details The following cases:
 *  - In the 3D case: from a second order tensor (3*3) Matrix  to a corresponing (6*1) Vector
 *    \f$ [ e11, e22, e33, 2*e12, 2*e23, 2*e13 ] \f$ for 3D case and
 *  - In the 2D case: from a second order tensor (3*3) Matrix  to a corresponing (4*1) Vector
 *    \f$ [ e11, e22, e33, 2*e12 ] \f$ fir 2D case.
 *  - In the 2D case: from a second order tensor (2*2) Matrix  to a corresponing (3*1) Vector
 *    \f$ [ e11, e22, 2*e12 ] \f$ fir 2D case.
 * @param rStrainTensor the given symmetric second order strain tensor
 * @return the corresponding strain tensor in vector form
 * @tparam TMatrixType The matrix type considered
 * @tparam TVector The vector returning type
 */
typedef Eigen::Matrix<double,Eigen::Dynamic,1> 
                                                    Vector;
template<class TMatrixType, class TVector = Vector>
static inline Vector StrainTensorToVector(
    const TMatrixType& rStrainTensor,
    SizeType rSize = 0
    )
{

    if(rSize == 0) {
        if(rStrainTensor.size1() == 2) {
            rSize = 3;
        } else if(rStrainTensor.size1() == 3) {
            rSize = 6;
        }
    }

    Vector strain_vector(rSize);

    if (rSize == 3) {
        strain_vector[0] = rStrainTensor(0,0);
        strain_vector[1] = rStrainTensor(1,1);
        strain_vector[2] = 2.0*rStrainTensor(0,1);
    } else if (rSize == 4) {
        strain_vector[0] = rStrainTensor(0,0);
        strain_vector[1] = rStrainTensor(1,1);
        strain_vector[2] = rStrainTensor(2,2);
        strain_vector[3] = 2.0*rStrainTensor(0,1);
    } else if (rSize == 6) {
        strain_vector[0] = rStrainTensor(0,0);
        strain_vector[1] = rStrainTensor(1,1);
        strain_vector[2] = rStrainTensor(2,2);
        strain_vector[3] = 2.0*rStrainTensor(0,1);
        strain_vector[4] = 2.0*rStrainTensor(1,2);
        strain_vector[5] = 2.0*rStrainTensor(0,2);
    }
    return strain_vector;
}

#endif