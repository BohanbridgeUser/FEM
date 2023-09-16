#include "../include/process_info.h"

void Process_Info::CreateSolutionStepInfo(IndexType SolutionStepIndex)
{
    mpPreviousSolutionPointer = SharedPointer(new Process_Info(*this));
    mSolutionStepIndex = SolutionStepIndex;
    if (mIsTimeStep)
        mpPreviousTimeStepInfo = mpPreviousSolutionPointer;
    mIsTimeStep = false;
    Data_Value_Container::Clear();
}

void Process_Info::SetCurrentTime(double NewTime)
{
    (*this)(TIME) = NewTime;
    if(!mpPreviousTimeStepInfo)
        (*this)(DELTA_TIME) = NewTime;
    else
        (*this)(DELTA_TIME) = NewTime -  mpPreviousTimeStepInfo->GetValue(TIME);
}
