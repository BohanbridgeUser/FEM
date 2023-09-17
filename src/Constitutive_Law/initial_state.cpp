#include "../../include/Constitutive_Law/initial_state.h"


/// @brief public:
/// @name Type Define
/// @{


/// @}


/// @name Life Circle
/// @{
    /// Only defining Dimension constructor.
    Initial_State::Initial_State(const SizeType Dimension)
    : mReferenceCounter(0)
    {
        const SizeType voigt_size = (Dimension == 3) ? 6 : 3;
        mInitialStressVector.resize(voigt_size);
        mInitialStrainVector.resize(voigt_size);
        mInitialDeformationGradientMatrix.resize(Dimension, Dimension);

        mInitialStressVector.setZero(voigt_size);
        mInitialStrainVector.setZero(voigt_size);
        mInitialDeformationGradientMatrix.setZero(Dimension, Dimension);
    }

    // Full constructor
    Initial_State::Initial_State(const Vector& rInitialStrainVector,
                                 const Vector& rInitialStressVector,
                                 const Matrix& rInitialDeformationGradientMatrix)
        : mReferenceCounter(0)
    {
        const SizeType voigt_size = rInitialStrainVector.size();
        const SizeType dimension  = rInitialDeformationGradientMatrix.rows();

        if(voigt_size <= 0 || dimension <= 0){
            std::cerr << "The imposed vector or matrix is null..." << std::endl;
            exit(0); 
        } 

        mInitialStressVector.resize(voigt_size);
        mInitialStrainVector.resize(voigt_size);
        mInitialDeformationGradientMatrix.resize(dimension, dimension);

        mInitialStressVector = rInitialStressVector;
        mInitialStrainVector = rInitialStrainVector;
        mInitialDeformationGradientMatrix = rInitialDeformationGradientMatrix;
    }

    // Selective constructor for vectors
    Initial_State::Initial_State(const Vector& rImposingEntity,
                                 const InitialImposingType InitialImposition)
        : mReferenceCounter(0)
    {
        const SizeType voigt_size = rImposingEntity.size();
        const SizeType dimension = (voigt_size == 6) ? 3 : 2;

        mInitialStrainVector.resize(voigt_size);
        mInitialStressVector.resize(voigt_size);
        mInitialDeformationGradientMatrix.resize(dimension, dimension);

        mInitialDeformationGradientMatrix.setZero(dimension, dimension);
        mInitialStrainVector.setZero(voigt_size);
        mInitialStressVector.setZero(voigt_size);

        if (InitialImposition == InitialImposingType::STRAIN_ONLY) {
            mInitialStrainVector = rImposingEntity;
        } else if (InitialImposition == InitialImposingType::STRESS_ONLY) {
            mInitialStressVector = rImposingEntity;
        }
    }

    // Selective constructor for vectors (E, S)
    Initial_State::Initial_State(const Vector& rInitialStrainVector,
                    const Vector& rInitialStressVector)
        : mReferenceCounter(0)
    {
        const SizeType voigt_size_1 = rInitialStrainVector.size();
        const SizeType voigt_size_2 = rInitialStressVector.size();
        const SizeType dimension = (voigt_size_1 == 6) ? 3 : 2;
        if(voigt_size_1 <= 0 || voigt_size_2 <= 0)
        {
            std::cerr << "The imposed vector is null..." << std::endl;
            exit(0);
        } 

        mInitialStressVector.resize(voigt_size_1);
        mInitialStrainVector.resize(voigt_size_1);
        mInitialDeformationGradientMatrix.resize(dimension, dimension);

        mInitialDeformationGradientMatrix.setZero(dimension, dimension);
        mInitialStressVector = rInitialStressVector;
        mInitialStrainVector = rInitialStrainVector;
    }

    // Selective constructor for Deformation Gradient only
    Initial_State::Initial_State(const Matrix& rInitialDeformationGradientMatrix)
        : mReferenceCounter(0)
    {
        const SizeType dimension = rInitialDeformationGradientMatrix.size1();
        const SizeType voigt_size = (dimension == 3) ? 6 : 3;
        if(dimension <= 0) 
        {
            std::cerr<< "The imposed Matrix is null..." << std::endl;
            exit(0);
        }
        mInitialDeformationGradientMatrix.resize(dimension, dimension);
        mInitialDeformationGradientMatrix = rInitialDeformationGradientMatrix;

        mInitialStressVector.resize(voigt_size);
        mInitialStrainVector.resize(voigt_size);
        mInitialStrainVector.setZero(voigt_size);
        mInitialStressVector.setZero(voigt_size);
    }

/// @}


/// @name Operators
/// @{


/// @}


/// @name Operations
/// @{


/// @}


/// @name Access
/// @{
    /**
     * @brief This method sets the initial strain vector
     * @param rInitialStrainVector The vector to be set
     */
    void Initial_State::SetInitialStrainVector(const Vector& rInitialStrainVector) {
        const SizeType voigt_size = rInitialStrainVector.size();
        if(voigt_size <= 0)
        {
            std::cerr << "The imposed vector is null..." << std::endl;
            exit(0);
        } 

        mInitialStrainVector.resize(voigt_size);
        mInitialStrainVector = rInitialStrainVector;
    }

    /**
     * @brief This method sets the initial stress vector
     * @param rInitialStressVector The vector to be set
     */
    void Initial_State::SetInitialStressVector(const Vector& rInitialStressVector) {
        const SizeType voigt_size = rInitialStressVector.size();
        if(voigt_size <= 0)
        {
            std::cerr << "The imposed vector is null..." << std::endl;
            exit(0);
        } 

        mInitialStressVector.resize(voigt_size);
        mInitialStressVector = rInitialStressVector;
    }

    /**
     * @brief This method sets the initial deformation gradient matrix
     * @param rInitialDeformationGradientMatrix The vector to be set
     */
    void Initial_State::SetInitialDeformationGradientMatrix(const Matrix& rInitialDeformationGradientMatrix) 
    {
        const SizeType dimension = rInitialDeformationGradientMatrix.rows();
        if(dimension <= 0)
        {
            std::cerr << "The imposed Matrix is null..." << std::endl;
        } 

        mInitialDeformationGradientMatrix.resize(dimension, dimension);
        mInitialDeformationGradientMatrix = rInitialDeformationGradientMatrix;
    }

/// @}


/// @name Inquiry
/// @{
    /**
     * @brief This method returns the initial strain vector if was set before
     */
    const Vector& Initial_State::GetInitialStrainVector() const
    {
        return mInitialStrainVector;
    }

    /**
     * @brief This method returns the initial stress vector if was set before
     */
    const Vector& Initial_State::GetInitialStressVector() const
    {
        return mInitialStressVector;
    }

    /**
     * @brief This method returns the initial stress vector if was set before
     */
    const Matrix& Initial_State::GetInitialDeformationGradientMatrix() const
    {
        return mInitialDeformationGradientMatrix;
    }

/// @}


/// @brief protected:
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


/// @brief private:
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


