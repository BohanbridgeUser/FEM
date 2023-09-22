#ifndef _LOTUS_KERNEL_H_
#define _LOTUS_KERNEL_H_

/**
 * @brief system includes
*/
#include <string>
#include <iostream>
#include <unordered_set>

#include "define.h"
#include "lotus_application.h"

class Lotus_Kernel
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_SHARED_POINTER_DEFINE(Lotus_Kernel)

        /// @}


        /// @name Life Circle
        /// @{
            Kernel();
            Kernel(bool IsDistributedRun);
            Kernel(Kernel const& rOther) {}
            virtual ~Kernel() {}
        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{
            /// Plugging an application into Kratos.
            /** This method first call the register method of the new application in order to create the
                components list of the application and then synchronizes the lists of its components with Kratos ones.
                The synchronized lists are
            - Variables
            - Elements
            - Conditions

            @param NewApplication The application to be added and synchronized
            */
            void ImportApplication(KratosApplication::Pointer pNewApplication);
            /// To be deprecated because variables have their own hash key.
            /** The keys of Variables are not sequential anymore, so this method will be deprecated
            */
            void Initialize();
            /// Initializes and synchronizes the list of variables, elements and conditions in each application.
            /** This method gives the application the list of all variables, elements and condition which is registered
                by kratos and all other added applications.
                @see ImportApplication
                @see Initialize
            */
            void InitializeApplication(KratosApplication& NewApplication) {}

            static void SetPythonVersion(std::string);

        /// @}


        /// @name Access
        /// @{
            static std::unordered_set<std::string>& GetApplicationsList();

        /// @}


        /// @name Inquiry
        /// @{
            bool IsImported(const std::string& ApplicationName) const;
            static bool IsDistributedRun();

             static std::string Version();

            static std::string BuildType();

            static std::string OSName();

            static std::string PythonVersion();

            static std::string Compiler();
        /// @}

        /// @name Input And Output
        /// @{
            virtual std::string Info() const;
            virtual void PrintInfo(std::ostream& rOStream) const;
            virtual void PrintData(std::ostream& rOStream) const;

            void PrintParallelismSupportInfo() const;
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
            static bool         mIsDistributedRun;
            static std::string  mPyVersion;

        /// @}


        /// @name Private Member Variables
        /// @{
            Lotus_Application::Pointer mpLotusCoreApplication;

        /// @}


        /// @name Private Operatiors
        /// @{
            Lotus_Kernel& operator=(Lotus_Kernel const& rOther);
        /// @}


        /// @name Private Operations
        /// @{
             void RegisterKratosCore();

        /// @}


        /// @name Private Access
        /// @{


        /// @}


        /// @name Private Inquiry
        /// @{


        /// @}
};
inline std::ostream& operator<<(std::ostream& rOStream, const Lotus_Kernel& rThis) 
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);
    return rOStream;
}

#endif