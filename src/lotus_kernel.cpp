#include "../include/lotus_kernel.h"
#include "omp.h"

/// @brief public:
/// @name Type Define
/// @{


/// @}


/// @name Life Circle
/// @{
    Lotus_Kernel::Lotus_Kernel()
    :mpLotusCoreApplication(std::make_shared<Lotus_Application>(std::string("LotusMultiphysics"))) 
    {
        Initialize();
    }
    Lotus_Kernel::Lotus_Kernel(bool IsDistributedRun)
    :mpLotusCoreApplication(std::make_shared<Lotus_Application>(std::string("LotusMultiphysics"))) 
    {
        mIsDistributedRun = IsDistributedRun;
        Initialize();
    }


/// @}


/// @name Operators
/// @{


/// @}


/// @name Operations
/// @{
    void Lotus_Kernel::Initialize() 
    {
        std::cout << ("")   << "           LOTUS \n"
                            << "           Multi-Physics " << Lotus_Kernel::Version() << "\n"
                            << "           Compiled for "  << Lotus_Kernel::OSName()  
                            << " and " << Lotus_Kernel::PythonVersion() 
                            << " with " << Lotus_Kernel::Compiler() << std::endl;

        PrintParallelismSupportInfo();

        if (!IsImported("LotusMultiphysics")) 
        {
            this->ImportApplication(mpLotusCoreApplication);
        }
    }

    void Lotus_Kernel::ImportApplication(Lotus_Application::Pointer pNewApplication) 
    {
        if (IsImported(pNewApplication->Name()))
        {
            std::cerr << "importing more than once the application : "
                        << pNewApplication->Name() << std::endl;
            exit(0);
        }
        pNewApplication->Register();
        Lotus_Kernel::GetApplicationsList().insert(pNewApplication->Name());
    }

    void Lotus_Kernel::SetPythonVersion(std::string pyVersion) 
    {
        mPyVersion = pyVersion;
    }
/// @}


/// @name Access
/// @{
    std::unordered_set<std::string> &Lotus_Kernel::GetApplicationsList() 
    {
        static std::unordered_set<std::string> application_list;
        return application_list;
    }

    // To be removed with the new entry points
    std::string Lotus_Kernel::BuildType() 
    {
        return GetBuildType();
    }
    // To be removed with the new entry points
    std::string Lotus_Kernel::Version() 
    {
        return GetVersionString();
    }
    std::string Lotus_Kernel::OSName() 
    {
        return GetOSName();
    }
    std::string Lotus_Kernel::PythonVersion() 
    {
        return mPyVersion;
    }
    std::string Lotus_Kernel::Compiler() 
    {
        return GetCompiler();
    }
/// @}


/// @name Inquiry
/// @{
    bool Lotus_Kernel::IsImported(const std::string& ApplicationName) const 
    {
        if (GetApplicationsList().find(ApplicationName) !=
            GetApplicationsList().end())
            return true;
        else
            return false;
    }

    bool Lotus_Kernel::IsDistributedRun() 
    {
        return mIsDistributedRun;
    }

/// @}

/// @name Input And Output
/// @{
    std::string Lotus_Kernel::Info() const { return "kernel"; }

    void Lotus_Kernel::PrintInfo(std::ostream& rOStream) const { rOStream << "kernel"; }

    /// Print object's data.
    void Lotus_Kernel::PrintData(std::ostream& rOStream) const 
    {
        rOStream << "Variables:" << std::endl;
        Lotus_Components<Variable_Data>().PrintData(rOStream);
        rOStream << std::endl;
        rOStream << "Geometries:" << std::endl;
        Lotus_Components<Geometry<Node>>().PrintData(rOStream);
        rOStream << std::endl;
        rOStream << "Elements:" << std::endl;
        Lotus_Components<Element>().PrintData(rOStream);
        rOStream << std::endl;
        rOStream << "Conditions:" << std::endl;
        Lotus_Components<Condition>().PrintData(rOStream);
        rOStream << std::endl;
        rOStream << "Loaded applications:" << std::endl;
        auto& application_list = Lotus_Kernel::GetApplicationsList();
        rOStream << "    Number of loaded applications = " << application_list.size() << std::endl;
        for (auto it = application_list.begin(); it != application_list.end(); ++it)
            rOStream << "    " << *it << std::endl;
    }
    void Lotus_Kernel::PrintParallelismSupportInfo() const
    {
        #ifdef KRATOS_SMP_NONE
        constexpr bool threading_support = false;
        #else
        constexpr bool threading_support = true;
        #endif

        #ifdef KRATOS_USING_MPI
        constexpr bool mpi_support = true;
        #else
        constexpr bool mpi_support = false;
        #endif

        if (threading_support) {
            if (mpi_support) {
                std::cout << "Compiled with threading and MPI support." << std::endl;
            }
            else {
                std::cout << "Compiled with threading support." << std::endl;
            }
        }
        else if (mpi_support) {
            std::cout << "Compiled with MPI support." << std::endl;
        }
        else {
            std::cout << "Serial compilation." << std::endl;
        }

        if (threading_support) 
        {
            std::cout << "Maximum number of threads: " << omp_get_num_threads() << "." << std::endl;
        }
    }
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
    bool Lotus_Kernel::mIsDistributedRun = false;
    std::string Lotus_Kernel::mPyVersion = std::string("Undefined");
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
