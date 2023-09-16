#ifndef _PROCESS_INFO_H_
#define _PROCESS_INFO_H_
#include "define.h"
#include "Container/data_value_container.h"
#include "Container/flags.h"


class Process_Info : public Data_Value_Container , public Flags
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_POINTER_DEFINE(Process_Info)
            typedef size_t
                                                            IndexType;
            typedef size_t
                                                             SizeType;
        /// @}


        /// @name Life Circle
        /// @{
            Process_Info()
            :Data_Value_Container(),
             Flags(),
             mIsTimeStep(true),
             mpPreviousSolutionPointer(),
             mpPreviousTimeStepInfo()
            {

            }
            Process_Info(Process_Info const& another)
            :Data_Value_Container(another),
             Flags(another),
             mIsTimeStep(another.mIsTimeStep),
             mpPreviousSolutionPointer(another.mpPreviousSolutionPointer),
             mpPreviousTimeStepInfo(another.mpPreviousTimeStepInfo)
            {

            }
            // Destructor
            ~Process_Info() override
            {

            }

        /// @}


        /// @name Operators
        /// @{
            Process_Info& operator=(Process_Info const& another)
            {
                Data_Value_Container::operator=(another);
                Flags::operator=(another);

                mIsTimeStep = another.mIsTimeStep;
                mSolutionStepIndex = another.mSolutionStepIndex;
                mpPreviousSolutionPointer = another.mpPreviousSolutionPointer;
                mpPreviousTimeStepInfo = another.mpPreviousTimeStepInfo;
                return *this;
            }

        /// @}


        /// @name Operations
        /// @{
            /**
             * @name Time Step 
             * @brief Create
            */
            void CreateTimeStepInfo(IndexType SolutionStepIndex = 0)
            {
                CreateSolutionStepInfo(SolutionStepIndex);
                mIsTimeStep = true;
            }

            void CreateTimeStepInfo(double NewTime, IndexType SolutionStepIndex = 0)
            {
                CreateTimeStepInfo(SolutionStepIndex);
                SetCurrentTime(NewTime);
            }

            void SetCurrentTime(double NewTime);
            /**
             * @name Solution Step Data
             * @brief Create
            */
            void CreateSolutionStepInfo(IndexType SolutionStepIndex = 0);


            void SetSolutionStepIndex(IndexType index)
            {
                mSolutionStepIndex = index;
            }

        /// @}


        /// @name Access
        /// @{
            IndexType GetSolutionStepIndex() const
            {
                return mSolutionStepIndex;
            }
            
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
            bool mIsTimeStep;
            IndexType mSolutionStepIndex;
            Process_Info::SharedPointer mpPreviousSolutionPointer;
            Process_Info::SharedPointer mpPreviousTimeStepInfo;

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