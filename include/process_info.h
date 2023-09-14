#ifndef _PROCESS_INFO_H_
#define _PROCESS_INFO_H_
#include "define.h"
class Process_Info
{
public:
    /// @name Define {
        LOTUS_POINTER_DEFINE(Process_Info)
        typedef size_t
                                                        IndexType;
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

    /// @name Operators
    /// @{

    /// @}

    /// @name Operations
    /// @{
        IndexType GetSolutionStepIndex() const
        {
            return mSolutionStepIndex;
        }
    /// @}
private:
    IndexType mSolutionStepIndex;
    Process_Info::SharedPointer mpPreviousSolutionPointer;
    Process_Info::SharedPointer mpPreviousTimeStepInfo;
};

#endif