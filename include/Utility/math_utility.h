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
        if(rStrainTensor.rows() == 2) {
            rSize = 3;
        } else if(rStrainTensor.rows() == 3) {
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
/**
 * @brief Transforms a stess vector into a matrix. Stresses are assumed to be stored in the following way:
 * \f$ [ s11, s22, s33, s12, s23, s13 ] \f$ for 3D case and
 * \f$ [ s11, s22, s33, s12 ] \f$ for 2D case.
 * \f$ [ s11, s22, s12 ] \f$ for 2D case.
 * @param rStressVector the given stress vector
 * @return the corresponding stress tensor in matrix form
 * @tparam TVector The vector type considered
 * @tparam TMatrixType The matrix returning type
 */
template<class TVector, class TMatrixType = MatrixType>
static inline TMatrixType StressVectorToTensor(const TVector& rStressVector)
{
    const SizeType matrix_size = rStressVector.size() == 3 ? 2 : 3;
    TMatrixType stress_tensor(matrix_size, matrix_size);

    if (rStressVector.size()==3) {
        stress_tensor(0,0) = rStressVector[0];
        stress_tensor(0,1) = rStressVector[2];
        stress_tensor(1,0) = rStressVector[2];
        stress_tensor(1,1) = rStressVector[1];
    } else if (rStressVector.size()==4) {
        stress_tensor(0,0) = rStressVector[0];
        stress_tensor(0,1) = rStressVector[3];
        stress_tensor(0,2) = 0.0;
        stress_tensor(1,0) = rStressVector[3];
        stress_tensor(1,1) = rStressVector[1];
        stress_tensor(1,2) = 0.0;
        stress_tensor(2,0) = 0.0;
        stress_tensor(2,1) = 0.0;
        stress_tensor(2,2) = rStressVector[2];
    } else if (rStressVector.size()==6) {
        stress_tensor(0,0) = rStressVector[0];
        stress_tensor(0,1) = rStressVector[3];
        stress_tensor(0,2) = rStressVector[5];
        stress_tensor(1,0) = rStressVector[3];
        stress_tensor(1,1) = rStressVector[1];
        stress_tensor(1,2) = rStressVector[4];
        stress_tensor(2,0) = rStressVector[5];
        stress_tensor(2,1) = rStressVector[4];
        stress_tensor(2,2) = rStressVector[2];
    }
    return stress_tensor;
}
/**
 * @brief Transforms a given symmetric Stress Tensor to Voigt Notation:
 * @details Components are assumed to be stored in the following way:
 * \f$ [ s11, s22, s33, s12, s23, s13 ] \f$ for 3D case and
 * \f$ [ s11, s22, s33, s12 ] \f$ for 2D case.
 * \f$ [ s11, s22, s12 ] \f$ for 2D case.
 * In the 3D case: from a second order tensor (3*3) Matrix  to a corresponing (6*1) Vector
 * In the 3D case: from a second order tensor (3*3) Matrix  to a corresponing (4*1) Vector
 * In the 2D case: from a second order tensor (2*2) Matrix  to a corresponing (3*1) Vector
 * @param rStressTensor the given symmetric second order stress tensor
 * @return the corresponding stress tensor in vector form
 * @tparam TMatrixType The matrix type considered
 * @tparam TVector The vector returning type
 */
template<class TMatrixType, class TVector = Vector>
static inline TVector StressTensorToVector(
    const TMatrixType& rStressTensor,
    SizeType rSize = 0
    )
{
    if(rSize == 0) {
        if(rStressTensor.rows() == 2) {
            rSize = 3;
        } else if(rStressTensor.rows() == 3) {
            rSize = 6;
        }
    }

    TVector stress_vector(rSize);

    if (rSize == 3) {
        stress_vector[0] = rStressTensor(0,0);
        stress_vector[1] = rStressTensor(1,1);
        stress_vector[2] = rStressTensor(0,1);
    } else if (rSize == 4) {
        stress_vector[0] = rStressTensor(0,0);
        stress_vector[1] = rStressTensor(1,1);
        stress_vector[2] = rStressTensor(2,2);
        stress_vector[3] = rStressTensor(0,1);
    } else if (rSize == 6) {
        stress_vector[0] = rStressTensor(0,0);
        stress_vector[1] = rStressTensor(1,1);
        stress_vector[2] = rStressTensor(2,2);
        stress_vector[3] = rStressTensor(0,1);
        stress_vector[4] = rStressTensor(1,2);
        stress_vector[5] = rStressTensor(0,2);
    }

    return stress_vector;
}
#endif