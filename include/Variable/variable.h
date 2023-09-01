#ifndef _VARIABLE_H_
#define _VARIABLE_H_
#include "variable_data.h"
#include "../Registry/registry.h"
template<typename TDataType>
class Variable : public Variable_Data
{
    public:
        /// @name Type Define
        /// @{
            typedef TDataType
                                                             Type;
            typedef Variable<TDataType>
                                                     VariableType;
            typedef Variable<TDataType>
                                                        ClassType;
            typedef Variable_Data
                                                         BaseType;
            typedef Variable_Data::KeyType
                                                          KeyType;
            LOTUS_POINTER_DEFINE(Variable<TDataType>)

        /// @}


        /// @name Life Circle
        /// @{
            /**
             * @brief Constructor with specific name and zero value
             * @param NewName The name to be assigned to the new variable
             * @param Zero The value to be assigned to the variable as zero. In case of not definition will take the value given by the constructor of the time
             * @param pTimeDerivativeVariable Pointer to the time derivative variable
             */
            explicit Variable(
                const std::string& NewName,
                const TDataType Zero = TDataType(),
                const VariableType* pTimeDerivativeVariable = nullptr
                )
                : Variable_Data(NewName, sizeof(TDataType)),
                mZero(Zero),
                mpTimeDerivativeVariable(pTimeDerivativeVariable)
            {
                RegisterThisVariable();
            }

            /**
             * @brief Constructor with specific name and zero value
             * @param NewName The name to be assigned to the new variable
             * @param pTimeDerivativeVariable Pointer to the time derivative variable
             */
            explicit Variable(
                const std::string& NewName,
                const VariableType* pTimeDerivativeVariable
                )
                : Variable_Data(NewName, sizeof(TDataType)),
                mZero(TDataType()),
                mpTimeDerivativeVariable(pTimeDerivativeVariable)
            {
                RegisterThisVariable();
            }

            /**
             * @brief Constructor for creating a component of other variable
             * @param rNewName The name to be assigned to the compoenent
             * @param Zero The value to be assigned to the variable as zero. In case of not definition will take the value given by the constructor of the time
             */
            template<typename TSourceVariableType>
            explicit Variable(
                const std::string& rNewName,
                TSourceVariableType* pSourceVariable,
                char ComponentIndex,
                const TDataType Zero = TDataType()
                )
                : Variable_Data(rNewName, sizeof(TDataType), pSourceVariable, ComponentIndex),
                mZero(Zero)
            {
                RegisterThisVariable();
            }

            /**
             * @brief Constructor for creating a component of other variable
             * @param rNewName The name to be assigned to the compoenent
             * @param pTimeDerivativeVariable Pointer to the time derivative variable
             * @param Zero The value to be assigned to the variable as zero. In case of not definition will take the value given by the constructor of the time
             */
            template<typename TSourceVariableType>
            explicit Variable(
                const std::string& rNewName,
                TSourceVariableType* pSourceVariable,
                char ComponentIndex,
                const VariableType* pTimeDerivativeVariable,
                const TDataType Zero = TDataType()
                )
                : Variable_Data(rNewName, sizeof(TDataType), pSourceVariable, ComponentIndex),
                mZero(Zero),
                mpTimeDerivativeVariable(pTimeDerivativeVariable)
            {
                RegisterThisVariable();
            }

            /**
             * Copy constructor.
             * @brief Copy constructor.
             * @param rOtherVariable The old variable to be copied
             */
            explicit Variable(const VariableType& rOtherVariable) :
                Variable_Data(rOtherVariable),
                mZero(rOtherVariable.mZero),
                mpTimeDerivativeVariable(rOtherVariable.mpTimeDerivativeVariable)
            {
                // Here we don't register as we asume that the origin is already registered
            }

            /// Destructor.
            ~Variable() override {}
        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{
            void Destruct(void* pSource) const override
            {
                static_cast<TDataType* >(pSource)->~TDataType();
            }
            /**
             * @brief Clone creates a copy of the object using a copy constructor of the class.
             * @details It is useful to avoid shallow copying of complex objects and also without actually having information about the variable type.
             * @param pSource The pointer of the variable to be cloned
             * @return A raw pointer of the variable
             */
            void* Clone(const void* pSource) const override
            {
                return new TDataType(*static_cast<const TDataType* >(pSource) );
            }
            /**
             * @brief Copy is very similar to Clone except that it also the destination pointer also passed to it.
             * @details It is a helpful method specially to create a copy of heterogeneous data arrays
             * @param pSource The pointer of the variable to be copied
             * @param pDestination The pointer of the destination variable
             * @return A raw pointer of the variable
             */
            void* Copy(const void* pSource, void* pDestination) const override
            {
                return new(pDestination) TDataType(*static_cast<const TDataType* >(pSource) );
            }
            /**
             * @brief Assign is very similar to Copy. It just differs in using an assignment operator besides the copy constructor. Copy creates a new object while
             * @details Assign does the assignment for two existing objects.
             * @param pSource The pointer of the value to be assigned
             * @param pDestination The pointer of the destination value
             */
            void Assign(const void* pSource, void* pDestination) const override
            {
                (*static_cast<TDataType* >(pDestination) ) = (*static_cast<const TDataType* >(pSource) );
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
            /**
             * Turn back information as a string.
             */
            std::string Info() const override
            {
                std::stringstream buffer;
                buffer << Name() << " variable" <<" #" << static_cast<unsigned int>(Key());
                if(IsComponent()){
                    buffer << Name() << " variable #" << static_cast<unsigned int>(Key()) << " component " << GetComponentIndex() << " of " <<  GetSourceVariable().Name();
                }
                else {
                    buffer << Name() << " variable #" << static_cast<unsigned int>(Key());
                }
                return buffer.str();
            }

            /**
             * Print information about this object.
             * @param rOStream The stream used to print the information
             */
            void PrintInfo(std::ostream& rOStream) const override
            {
                rOStream << Info();
            }

            /// Print object's data.
            void PrintData(std::ostream& rOStream) const override{
                Variable_Data::PrintData(rOStream);
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
            void RegisterThisVariable(){
                std::string variable_path = "variables.all." + Name();
                if(!Registry::HasItem(variable_path)){
                    Registry::AddItem<VariableType>(variable_path, *this);
                }
            }

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
            /* Definition of the static variable */
            static const VariableType msStaticObject;

        /// @}


        /// @name Private Member Variables
        /// @{
            /* The zero type contains the null value of the current variable type */
            TDataType mZero;
            /* Definition of the pointer to the variable for the time derivative */
            const VariableType* mpTimeDerivativeVariable = nullptr; 


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
};
template<class TDataType>
inline std::ostream& operator << (std::ostream& rOStream,
                                  const Variable<TDataType>& rThis)
{
    rThis.PrintInfo(rOStream);
    rThis.PrintData(rOStream);

    return rOStream;
}
#endif