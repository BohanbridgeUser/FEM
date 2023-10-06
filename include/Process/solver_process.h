#ifndef _SOLVER_PROCESS_H_
#define _SOLVER_PROCESS_H_

// Project includes
#include "../Model/model_part.h"
#include "process.h"


/**
   They are used for the configuration of the solver
*/

class Solver_Process : public Process
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of Solver_Process
    LOTUS_SHARED_POINTER_DEFINE(Solver_Process);

    ///@}
    ///@name Life Cycle
    ///@{ç

    /// Default constructor.
    Solver_Process() : Process() {}

    /// Constructor.
    Solver_Process(Flags options) : Process( options ) {}

    /// Destructor.
    ~Solver_Process() override {}


    ///@}
    ///@name Operators
    ///@{

    /// This operator is provided to call the process as a function and simply calls the Execute method.
    void operator()()
    {
        Execute();
    }

    ///@}
    ///@name Operations
    ///@{


    /// Execute method is used to execute the Solver_Process algorithms.
    void Execute()  override
    {
    }

    /// this function is designed for being called at the beginning of the computations
    /// right after reading the model and the groups
    void ExecuteInitialize() override
    {
    }

    /// this function is designed for being execute once before the solution loop but after all of the
    /// solvers where built
    void ExecuteBeforeSolutionLoop() override final
    {
      std::cout << (" Solver_Process ") << " method not available " << std::endl;
    }

    /// this function will be executed at every time step BEFORE performing the solve phase
    void ExecuteInitializeSolutionStep() override
    {
    }

    /// this function will be executed at every time step BEFORE performing a non linear iteration
    virtual void ExecuteInitializeNonLinearIteration()
    {
    }

    /// this function will be executed at every time step AFTER performing a non linear iteration
    virtual void ExecuteFinalizeNonLinearIteration()
    {
    }


    /// this function will be executed at every time step AFTER performing the solve phase
    void ExecuteFinalizeSolutionStep() override
    {
      std::cout << (" Solver_Process ") << " method not available " << std::endl;
    }


    /// this function will be executed at every time step BEFORE  writing the output
    void ExecuteBeforeOutputStep() override final
    {
      std::cout << (" Solver_Process ") << " method not available " << std::endl;
    }


    /// this function will be executed at every time step AFTER writing the output
    void ExecuteAfterOutputStep() override final
    {
    }


    /// this function is designed for being called at the end of the computations
    /// right after reading the model and the groups
    void ExecuteFinalize() override
    {
    }


    ///@}
    ///@name Access
    ///@{


    ///@}
    ///@name Inquiry
    ///@{


    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    std::string Info() const override
    {
        return "Solver_Process";
    }

    /// Print information about this object.
    void PrintInfo(std::ostream& rOStream) const override
    {
        rOStream << "Solver_Process";
    }

    /// Print object's data.
    void PrintData(std::ostream& rOStream) const override
    {
    }


    ///@}
    ///@name Friends
    ///@{
    ///@}

protected:

    ///@name Protected static Member Variables
    ///@{
    ///@}
    ///@name Protected member Variables
    ///@{
    ///@}
    ///@name Protected Operators
    ///@{

    /// Copy constructor.
    Solver_Process(Solver_Process const& rOther);

    ///@}
    ///@name Protected Operations
    ///@{
    ///@}
    ///@name Protected  Access
    ///@{
    ///@}
    ///@name Protected Inquiry
    ///@{
    ///@}
    ///@name Protected LifeCycle
    ///@{
    ///@}

private:

    ///@name Static Member Variables
    ///@{
    ///@}
    ///@name Member Variables
    ///@{
    ///@}
    ///@name Private Operators
    ///@{
    ///@}
    ///@name Private Operations
    ///@{
    ///@}
    ///@name Private  Access
    ///@{

    /// Assignment operator.
    Solver_Process& operator=(Solver_Process const& rOther);


    ///@}
    ///@name Serialization
    ///@{
    ///@name Private Inquiry
    ///@{
    ///@}
    ///@name Un accessible methods
    ///@{
    ///@}

}; // Class Solver_Process

///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{

/// output stream function
inline std::ostream& operator << (std::ostream& rOStream,
                                  const Solver_Process& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}
///@}
#endif
