#ifndef _PROCESS_INFO_H_
#define _PROCESS_INFO_H_
#include "define.h"
class Process_Info
{
public:
    /// @name Define {
    LOTUS_POINTER_DEFINE(Process_Info)
    /// @ }

    /// @name Life Circle 
    /// @{
        // Constructor
        Process_Info()
        {

        }

        // Destructor
        ~Process_Info()
        {

        }
    /// @}
private:
    int mSolutionStepIndex;
    Process_Info::SharedPointer mpPreviousSolutionPointer;
    Process_Info::SharedPointer mpPreviousTimeStepInfo;
};

#endif