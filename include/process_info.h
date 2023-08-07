#ifndef _PROCESS_INFO_H_
#define _PROCESS_INFO_H_
#include "define.h"
class Process_info
{
public:
    /// @name Define {
    LOTUS_POINTER_DEFINE(Process_info)
    /// @ }

    /// @name Life Circle 
    /// @{
        // Constructor
        Process_info()
        {

        }

        // Destructor
        ~Process_info()
        {

        }
    /// @}
private:
    int mSolutionStepIndex;
    Process_info::SharedPointer mpPreviousSolutionPointer;
    Process_info::SharedPointer mpPreviousTimeStepInfo;
};

#endif