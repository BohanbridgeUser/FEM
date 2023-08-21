#ifndef _PROCESS_H_
#define _PROCESS_H_
#include "../define.h"
#include "../Container/flags.h"

class Process : public Flags
{
    public:
        /// @name Type Define
        /// @{
            typedef Process
                                                            ClassType;;
            
        /// @}


        /// @name Life Circle
        /// @{
            /* Constructor */
            Process()
            {

            }
            explicit Process(const Flags& rOptions):Flags(rOptions)
            {

            }
            /* Destructor */
            ~Process()
            {

            }
        /// @}

        /// @name Operators
        /// @{
            void operator()()
            {
                Execute();
            }

        /// @}


        /// @name Operations
        /// @{
            virtual void Execute() 
            {

            }

            /**
             * @brief This function is designed for being called at the beginning of the computations
             * right after reading the model and the groups
             */
            virtual void ExecuteInitialize()
            {
            }

            /**
             * @brief This function is designed for being execute once before the solution loop but after
             * all of the solvers where built
             */
            virtual void ExecuteBeforeSolutionLoop()
            {
            }


            /**
             * @brief This function will be executed at every time step BEFORE performing the solve phase
             */
            virtual void ExecuteInitializeSolutionStep()
            {
            }

            /**
             * @brief This function will be executed at every time step AFTER performing the solve phase
             */
            virtual void ExecuteFinalizeSolutionStep()
            {
            }


            /**
             * @brief This function will be executed at every time step BEFORE  writing the output
             */
            virtual void ExecuteBeforeOutputStep()
            {
            }


            /**
             * @brief This function will be executed at every time step AFTER writing the output
             */
            virtual void ExecuteAfterOutputStep()
            {
            }


            /**
             * @brief This function is designed for being called at the end of the computations
             */
            virtual void ExecuteFinalize()
            {
            }

            /**
             * @brief This function is designed for being called after ExecuteInitialize ONCE
             * to verify that the input is correct.
             */
            virtual int Check()
            {
                return 0;
            }
        /// @}


        /// @name Access
        /// @{


        /// @}


        /// @name Inquiry
        /// @{


        /// @}

        /// @name Input and Output
        /// @{
            /* Turn back information as a string. */
            std::string Info() const 
            {
                return "Process";
            }

            /* Print information about this object. */ 
            void PrintInfo(std::ostream& rOStream) const 
            {
                rOStream << Info();
            }

            /* Print object's data. */ 
            void PrintData(std::ostream& rOStream) const 
            {
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


        /// @}


        /// @name Private Operatiors
        /// @{
            Process& operator=(Process const& another)
            {
                this->AssignFlags(another);
                return *this;
            }

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