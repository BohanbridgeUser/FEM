#pragma once

#include <string>
#include <iostream>
#include <map>
#include <typeinfo>
#include <Eigen/Eigen>

#include "define.h"
#include "Container/flags.h"
#include "Variable/variable.h"

template<class TComponentType>
class Components
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_SHARED_POINTER_DEFINE(Components)
            using ComponentsContainerType = std::map<std::string, const TComponentType*>;
            /// Component type
            using ValueType = typename ComponentsContainerType::value_type;
        /// @}

        /// @name Life Circle
        /// @{
            /// Default constructor.
            Components() = default;
            /// Destructor.
            virtual ~Components() = default;
        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{
            /**
             * @brief Adds a component to the collection.
             * @param rName the name of the component
             * @param rComponent the component to add
             * @throws std::runtime_error if a different object was already registered with the same name
             */
            static void Add(const std::string& rName, const TComponentType& rComponent)
            {
                // Check if a different object was already registered with this name, since this is undefined behavior
                auto it_comp =  msComponents.find(rName);
                if(it_comp != msComponents.end() && typeid(*(it_comp->second)) != typeid(rComponent))
                {
                    std::cerr<< "An object of different type was already registered with name \"" << rName << "\"!" << std::endl;
                    exit(0);
                } 
                msComponents.insert(ValueType(rName , &rComponent));
            }
            /**
             * @brief Removes a component with the specified name.
             * @param rName The name of the component to remove.
             * @throws ErrorType If the component with the specified name does not exist.
             */
            static void Remove(const std::string& rName)
            {
                std::size_t num_erased = msComponents.erase(rName);
                if(num_erased == 0)
                {
                    std::cerr<< "Trying to remove inexistent component \"" << rName << "\"." << std::endl;
                    exit(0);
                } 
            }
             /**
             * @brief Registers the function.
             */
            static void Register()
            {

            }

        /// @}


        /// @name Access
        /// @{
            /**
             * @brief Retrieves a component with the specified name.
             * @details This function retrieves a component from the ComponentsContainer using the provided name.
             * @param rName The name of the component to retrieve.
             * @return A reference to the retrieved component.
             * @note If the component is not found in debug, an error message will be printed and the program may terminate.
             */
            static const TComponentType& Get(const std::string& rName)
            {
                auto it_comp =  msComponents.find(rName);
                if(it_comp == msComponents.end())
                {
                    std::cerr<< GetMessageUnregisteredComponent(rName) << std::endl;
                    exit(0);
                } 
                return *(it_comp->second);
            }
             /**
             * @brief Retrieves the ComponentsContainer.
             * @details This function returns a reference to the ComponentsContainer, which stores all the components.
             * @return A reference to the ComponentsContainer.
             */
            static ComponentsContainerType& GetComponents()
            {
                return msComponents;
            }
            /**
             * @brief Retrieves the pointer to the ComponentsContainerType object.
             * @return Pointer to the ComponentsContainerType object.
             */
            static ComponentsContainerType* pGetComponents()
            {
                return &msComponents;
            }

        /// @}


        /// @name Inquiry
        /// @{
            /**
             * @brief Check if the given name exists in the set of components.
             * @param rName the name to check
             * @return true if the name exists, false otherwise
             */
            static bool Has(const std::string& rName)
            {
                return (msComponents.find(rName) != msComponents.end());
            }
        /// @}

        /// @name Input And Output
        /// @{
            /// Turn back information as a string.
            virtual std::string Info() const
            {
                return "Kratos components";
            }
            /// Print information about this object.
            virtual void PrintInfo(std::ostream& rOStream) const
            {
                rOStream << "Kratos components";
            }
            /// Print object's data.
            virtual void PrintData(std::ostream& rOStream) const 
            {
                for (const auto& r_comp : msComponents) {
                    rOStream << "    " << r_comp.first << std::endl;
                }
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
            static ComponentsContainerType msComponents; /// Component container


        /// @}


        /// @name Private Member Variables
        /// @{


        /// @}


        /// @name Private Operatiors
        /// @{
            Components& operator=(const Components& rOther);
        /// @}


        /// @name Private Operations
        /// @{
            /// Copy constructor.
            Components(const Components& rOther);

        /// @}


        /// @name Private Access
        /// @{
            /**
             * @brief Retrieves a message indicating that the component with the given name is not registered.
             * @param rName The name of the component that is not registered.
             * @return A string containing the error message.
             */
            static std::string GetMessageUnregisteredComponent(const std::string& rName)
            {
                std::stringstream msg;
                msg << "The component \"" << rName << "\" is not registered!\nMaybe you need to import the application where it is defined?\nThe following components of this type are registered:" << std::endl;
                Components instance; // creating an instance for using "PrintData"
                instance.PrintData(msg);
                return msg.str();
            }

        /// @}


        /// @name Private Inquiry
        /// @{


        /// @}
};

template<>
class Components<Variable_Data>
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_SHARED_POINTER_DEFINE(Components)
            /// The map type used to store the components // TODO: Replace std::map with faster alternative
            using ComponentsContainerType = std::map<std::string, Variable_Data*>;
            /// Component type
            using ValueType = ComponentsContainerType::value_type;

        /// @}


        /// @name Life Circle
        /// @{
            /// Default constructor.
            Components() = default;

            /// Destructor.
            virtual ~Components() = default;


        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{
             /**
             * @brief Adds a new element to the msComponents map.
             * @param rName the name of the element to add
             * @param rComponent the VariableData object to add
             */
            static void Add(const std::string& rName, Variable_Data& rComponent)
            {
                msComponents.insert(ValueType(rName, &rComponent));
            }
            /**
             * @brief Remove a component from the list by name.
             * @param rName the name of the component to remove
             */
            static void Remove(const std::string& rName)
            {
                std::size_t num_erased = msComponents.erase(rName);
                if(num_erased == 0)
                {
                    std::cerr<< "Trying to remove inexistent component \"" << rName << "\"." << std::endl;
                    exit(0);
                } 
            }
            /**
             * @brief Get the size of the components.
             * @return The size of the components.
             */
            static std::size_t Size()
            {
                return msComponents.size();
            }
            /**
             * @brief Retrieves the VariableData with the specified name.
             * @details This function retrieves the VariableData associated with the provided name from the msComponents container.
             * @param rName The name of the VariableData to retrieve.
             * @return A reference to the retrieved VariableData.
             * @note If the VariableData is not found in debug, an error message will be printed and the program may terminate.
             */
            static Variable_Data& Get(const std::string& rName)
            {
                auto it_comp =  msComponents.find(rName);
                if(it_comp == msComponents.end()) 
                {
                    std::cerr<< GetMessageUnregisteredVariable(rName) << std::endl;
                    exit(0);
                }
                return *(it_comp->second);
            }
            /**
             * @brief Retrieves the variable data associated with the given name.
             * @param rName the name of the variable
             * @return a pointer to the variable data, or nullptr if not found
             */
            static Variable_Data* pGet(const std::string& rName)
            {
                auto it_comp =  msComponents.find(rName);
                if(it_comp == msComponents.end())
                {
                    std::cerr<< GetMessageUnregisteredVariable(rName) << std::endl;
                    exit(0);
                } 
                return it_comp->second;
            }
            /**
             * @brief Registers the function.
             */
            static void Register()
            {

            }


        /// @}


        /// @name Access
        /// @{
            /**
             * @brief Retrieves the ComponentsContainer.
             * @details This function returns a reference to the ComponentsContainer, which stores all the components.
             * @return A reference to the ComponentsContainer.
             */
            static ComponentsContainerType& GetComponents()
            {
                return msComponents;
            }

            /**
             * @brief Returns a pointer to the ComponentsContainerType object.
             * @return a pointer to the ComponentsContainerType object
             */
            static ComponentsContainerType* pGetComponents()
            {
                return &msComponents;
            }


        /// @}


        /// @name Inquiry
        /// @{
             /**
             * @brief Checks if the specified name exists in the set of components.
             * @param rName the name to check
             * @return true if the name exists in the set, false otherwise
             */
            static bool Has(const std::string& rName)
            {
                return (msComponents.find(rName) != msComponents.end());
            }


        /// @}

        /// @name Input And Output
        /// @{
            
            /// Turn back information as a string.
            virtual std::string Info() const
            {
                return "Kratos components <VariableData>";
            }

            /// Print information about this object.
            virtual void PrintInfo(std::ostream& rOStream) const
            {
                rOStream << "Kratos components <VariableData>";
            }

            /// Print object's data.
            virtual void PrintData(std::ostream& rOStream) const
            {
                for (const auto& r_comp : msComponents) {
                    rOStream << "    " << r_comp.first << std::endl;
                }
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
            static ComponentsContainerType msComponents; /// Component container

        /// @}


        /// @name Private Member Variables
        /// @{


        /// @}


        /// @name Private Operatiors
        /// @{
            /// Assignment operator.
            Components& operator=(const Components& rOther);

            /// Copy constructor.
            Components(const Components& rOther);


             


        /// @}


        /// @name Private Operations
        /// @{
            /**
             * @brief Generates the error message for an unregistered variable.
             * @param rName The name of the unregistered variable.
             * @return The error message string.
             */
            static std::string GetMessageUnregisteredVariable(const std::string& rName)
            {
                std::stringstream msg;
                msg << "The variable \"" << rName << "\" is not registered!\nMaybe you need to import the application where it is defined?\nThe following variables are registered:" << std::endl;
                Components instance; // creating an instance for using "PrintData"
                instance.PrintData(msg);
                return msg.str();
            }

        /// @}


        /// @name Private Access
        /// @{


        /// @}


        /// @name Private Inquiry
        /// @{


        /// @}
};

template<class TComponentType>
typename Components<TComponentType>::ComponentsContainerType Components<TComponentType>::msComponents;
/// output stream function
template<class TComponentType>
inline std::ostream& operator << (std::ostream& rOStream,
                                  const Components<TComponentType>& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}

extern template class Components<Variable<bool>>;
extern template class Components<Variable<int>>;
extern template class Components<Variable<unsigned int>>;
extern template class Components<Variable<double>>;
extern template class Components<Variable<std::array<double, 3>>>;
extern template class Components<Variable<std::array<double, 4>>>;
extern template class Components<Variable<std::array<double, 6>>>;
extern template class Components<Variable<std::array<double, 9>>>;
extern template class Components<Variable<Eigen::Matrix<double,Eigen::Dynamic,1>>>;
extern template class Components<Variable<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>>>;
extern template class Components<Variable<std::string>>;
extern template class Components<Variable<Flags>>;
extern template class Components<Flags>;

void AddComponent(const std::string& rName, const Variable<bool>& rComponent);
void AddComponent(const std::string& rName, const Variable<int>& rComponent);
void AddComponent(const std::string& rName, const Variable<unsigned int>& rComponent);
void AddComponent(const std::string& rName, const Variable<double>& rComponent);
void AddComponent(const std::string& rName, const Variable<std::array<double, 3>>& rComponent);
void AddComponent(const std::string& rName, const Variable<std::array<double, 4>>& rComponent);
void AddComponent(const std::string& rName, const Variable<std::array<double, 6>>& rComponent);
void AddComponent(const std::string& rName, const Variable<std::array<double, 9>>& rComponent);
void AddComponent(const std::string& rName, const Variable<Eigen::Matrix<double,Eigen::Dynamic,1>>& rComponent);
void AddComponent(const std::string& rName, const Variable<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>>& rComponent);
void AddComponent(const std::string& rName, const Variable<std::string>& rComponent);
void AddComponent(const std::string& rName, const Flags& rComponent);
void AddComponent(const std::string& rName, const Variable<Flags>& rComponent);

template<class TComponentType> 
void AddComponent(const std::string& rName, const TComponentType& rComponent)
{
}
