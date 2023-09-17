#ifndef _INITIAL_STATE_H_
#define _INITIAL_STATE_H_

#include "../define.h"
#include "../Variable/variables.h"

#include <atomic>
#include <Eigen/Eigen>

class Initial_State
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_POINTER_DEFINE(Initial_State)
            typedef size_t
                                                                SizeType;
            typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                                  Matrix;
            typedef Eigen::Matrix<double,Eigen::Dynamic,1>
                                                                  Vector;                                                      
            
            enum class InitialImposingType
            {
                STRAIN_ONLY = 0,
                STRESS_ONLY = 1,
                DEFORMATION_GRADIENT_ONLY = 2,
                STRAIN_AND_STRESS = 3,
                DEFORMATION_GRADIENT_AND_STRESS = 4
            };
        /// @}


        /// @name Life Circle
        /// @{
            Initial_State()
            {}

            /// Only defining Dimension constructor.
            Initial_State(const SizeType Dimension);

            // Full constructor
            Initial_State(const Vector& rInitialStrainVector,
                          const Vector& rInitialStressVector,
                          const Matrix& rInitialDeformationGradientMatrix);

            // Selective constructor for vectors
            Initial_State(const Vector& rImposingEntity,
                          const InitialImposingType InitialImposition = InitialImposingType::STRAIN_ONLY);

            // Selective constructor for vectors (E, S)
            Initial_State(const Vector& rInitialStrainVector,
                          const Vector& rInitialStressVector);

            // Selective constructor for Deformation Gradient only
            Initial_State(const Matrix& rInitialDeformationGradientMatrix);

            /// Destructor.
            virtual ~Initial_State() {}

        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{
            unsigned int use_count() const noexcept
            {
                return mReferenceCounter;
            }

            /**
             * @brief This method sets the initial strain vector
             * @param rInitialStrainVector The vector to be set
             */
            void SetInitialStrainVector(const Vector& rInitialStrainVector);

            /**
             * @brief This method sets the initial stress vector
             * @param rInitialStressVector The vector to be set
             */
            void SetInitialStressVector(const Vector& rInitialStressVector);

            /**
             * @brief This method sets the initial deformation gradient matrix
             * @param rInitialDeformationGradientMatrix The vector to be set
             */
            void SetInitialDeformationGradientMatrix(const Matrix& rInitialDeformationGradientMatrix);

        /// @}


        /// @name Access
        /// @{
            /**
            * @brief This method returns the initial strain vector if was set before
            */
            const Vector& GetInitialStrainVector() const;

            /**
             * @brief This method returns the initial stress vector if was set before
             */
            const Vector& GetInitialStressVector() const;

            /**
             * @brief This method returns the initial stress vector if was set before
             */
            const Matrix& GetInitialDeformationGradientMatrix() const;


        /// @}


        /// @name Inquiry
        /// @{


        /// @}

        /// @name Input And Output
        /// @{
            virtual std::string Info() const
            {
                std::stringstream buffer;
                buffer << "InitialState" ;

                return buffer.str();
            }

            /// Print information about this object.
            virtual void PrintInfo(std::ostream& rOStream) const  {rOStream << "InitialState";}

            /// Print object's data.
            virtual void PrintData(std::ostream& rOStream) const {};
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
            Vector mInitialStrainVector;
            Vector mInitialStressVector;
            Matrix mInitialDeformationGradientMatrix;


        /// @}


        /// @name Private Operatiors
        /// @{


        /// @}


        /// @name Private Operations
        /// @{
            //this block is needed for refcounting
            mutable std::atomic<int> mReferenceCounter{0};
            friend void intrusive_ptr_add_ref(const Initial_State* x)
            {
                x->mReferenceCounter.fetch_add(1, std::memory_order_relaxed);
            }

            friend void intrusive_ptr_release(const Initial_State* x)
            {
                if (x->mReferenceCounter.fetch_sub(1, std::memory_order_release) == 1) {
                    std::atomic_thread_fence(std::memory_order_acquire);
                    delete x;
                }
            }

        /// @}


        /// @name Private Access
        /// @{


        /// @}


        /// @name Private Inquiry
        /// @{


        /// @}
};

#endif

