#ifndef _PROCESS_INFO_H_
#define _PROCESS_INFO_H_
#include "define.h"
#include "Variable/variables.h"
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
             mpPreviousSolutionStepInfo(),
             mpPreviousTimeStepInfo()
            {

            }
            Process_Info(Process_Info const& another)
            :Data_Value_Container(another),
             Flags(another),
             mIsTimeStep(another.mIsTimeStep),
             mpPreviousSolutionStepInfo(another.mpPreviousSolutionStepInfo),
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
                mpPreviousSolutionStepInfo = another.mpPreviousSolutionStepInfo;
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

            

            /**
             * @name Time Step
             * @brief Clone
            */
            void CloneTimeStepInfo(IndexType SolutionStepIndex = 0)
            {
                CreateSolutionStepInfo(SolutionStepIndex);
                mIsTimeStep = true;
            }
            void CloneTimeStepInfo(IndexType SolutionStepIndex,
                                   Process_Info const &  SourceSolutionStepInfo)
            {
                CloneSolutionStepInfo(SolutionStepIndex, SourceSolutionStepInfo);
                mIsTimeStep = true;
            }
            void CloneTimeStepInfo(double NewTime,
                                   IndexType SourceSolutionStepIndex = 0)
            {
                CloneTimeStepInfo(SourceSolutionStepIndex);
                SetCurrentTime(NewTime);
            }
            void CloneTimeStepInfo(double NewTime,
                                   IndexType SolutionStepIndex,
                                   Process_Info const &  SourceSolutionStepInfo)
            {
                CloneTimeStepInfo(SolutionStepIndex, SourceSolutionStepInfo);
                SetCurrentTime(NewTime);
            }


            /**
             * @name Solution Step Data
             * @brief Create
            */
            void CreateSolutionStepInfo(IndexType SolutionStepIndex = 0);

            /**
             * @name Solution Step Data
             * @brief Clone
            */
            void CloneSolutionStepInfo();

            void CloneSolutionStepInfo(IndexType SourceSolutionStepIndex);

            void CloneSolutionStepInfo(IndexType SolutionStepIndex, Process_Info const &  SourceSolutionStepInfo);



            void SetSolutionStepIndex(IndexType index)
            {
                mSolutionStepIndex = index;
            }
            void SetCurrentTime(double NewTime);
            void SetAsTimeStepInfo();
            void SetAsTimeStepInfo(double NewTime);


            void RemoveSolutionStepInfo(IndexType SolutionStepIndex);

            void ClearHistory(IndexType StepsBefore = 0);
        /// @}


        /// @name Access
        /// @{
            IndexType GetSolutionStepIndex() const
            {
                return mSolutionStepIndex;
            }
            
            Process_Info::Pointer pGetPreviousTimeStepInfo(IndexType StepsBefore = 1);

            const Process_Info::Pointer pGetPreviousTimeStepInfo(IndexType StepsBefore = 1) const;

            Process_Info& GetPreviousTimeStepInfo(IndexType StepsBefore = 1)
            {
                return *pGetPreviousTimeStepInfo(StepsBefore);
            }

            Process_Info const& GetPreviousTimeStepInfo(IndexType StepsBefore = 1) const
            {
                return *pGetPreviousTimeStepInfo(StepsBefore);
            }

            IndexType GetPreviousTimeStepIndex(IndexType StepsBefore = 1) const
            {
                return GetPreviousTimeStepInfo(StepsBefore).GetSolutionStepIndex();
            }


            Process_Info::Pointer pGetPreviousSolutionStepInfo(IndexType StepsBefore = 1);

            const Process_Info::Pointer pGetPreviousSolutionStepInfo(IndexType StepsBefore = 1) const;

            Process_Info& GetPreviousSolutionStepInfo(IndexType StepsBefore = 1)
            {
                return *pGetPreviousSolutionStepInfo(StepsBefore);
            }

            Process_Info const& GetPreviousSolutionStepInfo(IndexType StepsBefore = 1) const
            {
                return *pGetPreviousSolutionStepInfo(StepsBefore);
            }

            IndexType GetPreviousSolutionStepIndex(IndexType StepsBefore = 1) const
            {
                return GetPreviousSolutionStepInfo(StepsBefore).GetSolutionStepIndex();
            }



            Process_Info& FindSolutionStepInfo(IndexType ThisIndex);


        /// @}


        /// @name Inquiry
        /// @{


        /// @}

        /// @name Input And Output
        /// @{
            std::string Info() const override
            {
                return "Process Info";
            }

            /// Print information about this object.
            void PrintInfo(std::ostream& rOStream) const override
            {
                rOStream << Info();
            }

            /// Print object's data.
            void PrintData(std::ostream& rOStream) const override
            {
                rOStream << "    Current solution step index : " << mSolutionStepIndex << std::endl;
                Data_Value_Container::PrintData(rOStream);
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
            bool mIsTimeStep;
            IndexType mSolutionStepIndex;
            Process_Info::SharedPointer mpPreviousSolutionStepInfo;
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