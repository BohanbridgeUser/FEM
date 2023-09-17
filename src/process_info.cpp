#include "../include/process_info.h"



/// @brief public:
/// @name Type Define
/// @{


/// @}


/// @name Life Circle
/// @{


/// @}


/// @name Operators
/// @{


/// @}


/// @name Operations
/// @{
    /**
     * @name Time Step 
     * @brief Create
    */

    /**
     * @name Time Step
     * @brief Clone
    */

    /**
     * @name Solution Step Data
     * @brief Create
    */
    void Process_Info::CreateSolutionStepInfo(IndexType SolutionStepIndex)
    {
        mpPreviousSolutionStepInfo = SharedPointer(new Process_Info(*this));
        mSolutionStepIndex = SolutionStepIndex;
        if (mIsTimeStep)
            mpPreviousTimeStepInfo = mpPreviousSolutionStepInfo;
        mIsTimeStep = false;
        Data_Value_Container::Clear();
    }

    /**
     * @name Solution Step Data
     * @brief Clone
    */
    void Process_Info::CloneSolutionStepInfo()
    {
        mpPreviousSolutionStepInfo = SharedPointer(new Process_Info(*this));
        mSolutionStepIndex = 0;
        if(mIsTimeStep)
            mpPreviousTimeStepInfo = mpPreviousSolutionStepInfo;
        mIsTimeStep = false;
    }

    void Process_Info::CloneSolutionStepInfo(IndexType SourceSolutionStepIndex)
    {
        Process_Info& source_info = FindSolutionStepInfo(SourceSolutionStepIndex);
        if(source_info.GetSolutionStepIndex() == SourceSolutionStepIndex)
        {
            mpPreviousSolutionStepInfo = SharedPointer(new Process_Info(*this));
            mSolutionStepIndex = 0;
            Data_Value_Container::operator=(source_info);
            if(mIsTimeStep)
                mpPreviousTimeStepInfo = mpPreviousSolutionStepInfo;
            mIsTimeStep = false;
        }
        else
            CreateSolutionStepInfo(0);
    }

    void Process_Info::CloneSolutionStepInfo(IndexType SolutionStepIndex, Process_Info const&  SourceSolutionStepInfo)
    {
        mpPreviousSolutionStepInfo = SharedPointer(new Process_Info(*this));
        mSolutionStepIndex = SolutionStepIndex;
        Data_Value_Container::operator=(SourceSolutionStepInfo);
        if(mIsTimeStep)
            mpPreviousTimeStepInfo = mpPreviousSolutionStepInfo;
        mIsTimeStep = false;
    }

    /**
     * @name Utility
    */
    void Process_Info::SetCurrentTime(double NewTime)
    {
        (*this)(TIME) = NewTime;
        if(!mpPreviousTimeStepInfo)
            (*this)(DELTA_TIME) = NewTime;
        else
            (*this)(DELTA_TIME) = NewTime -  mpPreviousTimeStepInfo->GetValue(TIME);
    }
    void Process_Info::SetAsTimeStepInfo()
    {
        mIsTimeStep = true;
        SetCurrentTime((*this)(TIME));
    }
    void Process_Info::SetAsTimeStepInfo(double NewTime)
    
    {
        mIsTimeStep = true;
        SetCurrentTime(NewTime);
    }

    void Process_Info::RemoveSolutionStepInfo(IndexType SolutionStepIndex)
    {
        if(!mpPreviousSolutionStepInfo)
            return;

        if(mpPreviousSolutionStepInfo->GetSolutionStepIndex() == SolutionStepIndex)
            mpPreviousSolutionStepInfo = SharedPointer(mpPreviousSolutionStepInfo->pGetPreviousSolutionStepInfo());
        else
            mpPreviousSolutionStepInfo->RemoveSolutionStepInfo(SolutionStepIndex);
    }

    void Process_Info::ClearHistory(IndexType StepsBefore)
    {
        if(StepsBefore == 0)
        {
            mpPreviousTimeStepInfo = SharedPointer();
            mpPreviousSolutionStepInfo = SharedPointer();
        }
        else
        {
            if(mpPreviousTimeStepInfo)
                mpPreviousTimeStepInfo->ClearHistory(--StepsBefore);
            if(mpPreviousSolutionStepInfo)
                mpPreviousSolutionStepInfo->ClearHistory(--StepsBefore);
        }
    }
/// @}


/// @name Access
/// @{
    Process_Info::Pointer Process_Info::pGetPreviousTimeStepInfo(IndexType StepsBefore)
    {
        if(StepsBefore > 1)
            return mpPreviousTimeStepInfo->pGetPreviousTimeStepInfo(--StepsBefore);

        if(StepsBefore == 0)
        {
            std::cerr << "Steps before = 0" << std::endl;
            exit(0);
        } 
        if(!mpPreviousTimeStepInfo){
            std::cerr << "No previous time step exists." << std::endl;
            exit(0);
        } 

        return &(*mpPreviousTimeStepInfo);
    }

    const Process_Info::Pointer Process_Info::pGetPreviousTimeStepInfo(IndexType StepsBefore) const
    {
        if(StepsBefore > 1)
            return mpPreviousTimeStepInfo->pGetPreviousTimeStepInfo(--StepsBefore);

        if(StepsBefore == 0)
        {
            std::cerr << "Steps before = 0" << std::endl;
            exit(0);
        } 
        if(!mpPreviousTimeStepInfo){
            std::cerr << "No previous time step exists." << std::endl;
            exit(0);
        } 

        return &(*mpPreviousTimeStepInfo);
    }

    Process_Info& Process_Info::FindSolutionStepInfo(IndexType ThisIndex)
    {
        if(mSolutionStepIndex == ThisIndex)
            return *this;

        if(!mpPreviousSolutionStepInfo)
            return *this;

        return mpPreviousTimeStepInfo->FindSolutionStepInfo(ThisIndex);
    }


    Process_Info::Pointer Process_Info::pGetPreviousSolutionStepInfo(IndexType StepsBefore)
    {
        if(StepsBefore > 1)
            return mpPreviousSolutionStepInfo->pGetPreviousSolutionStepInfo(--StepsBefore);

        if(StepsBefore == 0)
        {
            std::cerr << "Steps before = 0" << std::endl;
            exit(0);
        } 
        if(!mpPreviousTimeStepInfo){
            std::cerr << "No previous time step exists." << std::endl;
            exit(0);
        } 

        return &(*mpPreviousSolutionStepInfo);
    }

    const Process_Info::Pointer Process_Info::pGetPreviousSolutionStepInfo(IndexType StepsBefore) const
    {
        if(StepsBefore > 1)
            return mpPreviousSolutionStepInfo->pGetPreviousSolutionStepInfo(--StepsBefore);

        if(StepsBefore == 0)
        {
            std::cerr << "Steps before = 0" << std::endl;
            exit(0);
        } 
        if(!mpPreviousTimeStepInfo){
            std::cerr << "No previous time step exists." << std::endl;
            exit(0);
        } 
        return &(*mpPreviousSolutionStepInfo);
    }

/// @}


/// @name Inquiry
/// @{


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