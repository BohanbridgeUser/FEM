#ifndef _VARIABLE_DATA_H_
#define _VARIABLE_DATA_H_
#include "../define.h"
#include "../fnv_1a_hash.h"
#include <string>
#include <sstream>
#include <iostream>
class Variable_Data
{
    public:
        /// @name Type Define
        /// @{
            typedef Variable_Data
                                                    ClassType;
            typedef std::size_t
                                                      KeyType;
            LOTUS_POINTER_DEFINE(Variable_Data)


        /// @}


        /// @name Life Circle
        /// @{
            Variable_Data(const ClassType& another);
            virtual ~Variable_Data()
            {

            }
        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{
            /**
             * Clone creates a copy of the object using a copy constructor of the class. 
             * It is useful to avoid shallow copying of complex objects and also without 
             * actually having information about the variable type.
             * @param pSource The pointer of the variable to be cloned
             * @return A raw pointer of the variable
             */
            virtual void* Clone(const void* pSource) const;

            /**
             * Copy is very similar to Clone except that it also the destination 
             * pointer also passed to it. It is a helpful method specially 
             * to create a copy of heterogeneous data arrays
             * @param pSource The pointer of the variable to be copied
             * @param pDestination The pointer of the destination variable
             * @return A raw pointer of the variable
             */
            virtual void* Copy(const void* pSource, void* pDestination) const;

            /**
             * Assign is very similar to Copy. It just differs in using an assignment 
             * operator besides the copy constructor. Copy creates a new object while 
             * Assign does the assignment for two existing objects. 
             * @param pSource The pointer of the value to be assigned
             * @param pDestination The pointer of the destination value
             */
            virtual void Assign(const void* pSource, void* pDestination) const;

            /**
             * AssignZero is a special case of Assign for which variable zero value used as source. 
             * This method is useful for initializing arrays or resetting values in memory.
             * @param pDestination The pointer of the destination variable
             */
            virtual void AssignZero(void* pDestination) const;

            /**
             *  Delete removes an object of variable type from memory. It calls a 
             * destructor of objects to prevent memory leak and frees the memory 
             * allocated for this object assuming that the object is allocated in heap.
             * @param pSource The pointer of the variable to be deleted
             */
            virtual void Delete(void* pSource) const;
            
            /**
             *  Destruct eliminates an object maintaining the memory it is using. 
             * However, the unlike Delete it does nothing with the memory allocated to it. 
             * So it is very useful in case of reallocating a part of the memory.
             * @param pSource The pointer of the variable to be destructed
             */
            virtual void Destruct(void* pSource) const;

            /**
             *  Print is an auxiliary method to produce output of given variable 
             * knowing its address. For example writing an heterogenous container 
             * in an output stream can be done using this method. Point assumes 
             * that the streaming operator is defined for the variable type.
             * @param pSource The pointer of the variable to be printed
             * @param rOStream The stream used to print the information
             */
            virtual void Print(const void* pSource, std::ostream& rOStream) const;

            /**
             * PrintData is an auxiliary method to produce output only the value of given variable 
             * knowing its address. For example writing an heterogenous container 
             * in an output stream can be done using this method. Point assumes 
             * that the streaming operator is defined for the variable type.
             * @param pSource The pointer of the variable to be printed
             * @param rOStream The stream used to print the information
             */
            virtual void PrintData(const void* pSource, std::ostream& rOStream) const;

            /**
             * This method allocates the data of the variable
             * @param pData A pointer to the data to be allocated
             */
            virtual void Allocate(void** pData) const;
            virtual std::string Info() const;
            virtual void PrintInfo(std::ostream& rOStream) const;
            virtual void PrintData(std::ostream& rOStream) const;
        /// @}


        /// @name Access
        /// @{
            KeyType HashKey() const
            {
                KeyType key = mKey;// >> 8;
                key &= 0xFFFFFFFF00;
                return key;
            }
            KeyType Key() const
            {
                return mKey;
            }
            KeyType SourceKey() const
            {
                return mpSourceVariable->mKey;
            }

            /**
             * @note: This function is for internal use and not 
             * to change arbitrary any variable's key
             */ 
            void SetKey(KeyType NewKey);

            const std::string& Name() const
            {
                return mName;
            }
            std::size_t Size() const
            {
                return mSize;
            }

            bool IsComponent() const
            {
                return mIsComponent;
            }

            bool IsNotComponent() const
            {
                return !mIsComponent;
            }

            /** 
             * @brief  Returns the component index. 
             * @details please note that this method don't check if it is a component or not.
             * It uses the key to reterive the compenent index from its first 7 bits.
             * Component index can be from 0 to 127 at most, because 7 bits are used to store it. 
             * So in case of normal variables it returns 0 (like being the first componet)
             **/
            KeyType GetComponentIndex() const {
                constexpr KeyType first_7_bits=127;
                return (mKey & first_7_bits);
            }

            const Variable_Data& GetSourceVariable() const
            {   
                if(mpSourceVariable == nullptr) 
                    std::cout << "No source variable is defined for the component" << std::endl;
                return *mpSourceVariable;
            }

            virtual const void* pZero() const 
            {
                std::cout << "Calling base class method." << std::endl;
                return nullptr;
            }

        /// @}

        /// @name Static
        /// @{
            static KeyType GenerateKey(const std::string& Name, 
                                       std::size_t Size, 
                                       bool IsComponent, 
                                       char ComponentIndex);
        /// @}

        /// @name Inquiry
        /// @{


        /// @}
        
        /// @name Friend 
        /// @{
            friend bool operator==(const Variable_Data& rFirstVariable, const Variable_Data& rSecondVariable)
            {
                return (rFirstVariable.mKey == rSecondVariable.mKey);
            }
        /// @}

    protected:
        /// @name Protected Static Member Variables
        /// @{


        /// @}


        /// @name Protected Member Variables
        /// @{


        /// @}

        /// @name Protected Life Circle
        /// @{
            Variable_Data(const std::string& NewName, std::size_t NewSize);
            Variable_Data(const std::string& NewName, std::size_t NewSize,
                          const Variable_Data* pSourceVariable,
                          char ComponentIndex);
            Variable_Data() {}
        /// @}

        /// @name Protected Operatiors
        /// @{
            Variable_Data& operator=(const Variable_Data& rOtherVariable)
            {
                mName = rOtherVariable.mName;
                mKey = rOtherVariable.mKey;
                mSize = rOtherVariable.mSize;
                mpSourceVariable = rOtherVariable.mpSourceVariable;
                mIsComponent = rOtherVariable.mIsComponent;
                return *this;
            }

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
            std::string mName;

            /** Key value of this variable. Each variable will be locate by this
            value in each data structure. Variable constructor will initialize it. */
            KeyType mKey;

            std::size_t mSize;

            const Variable_Data* mpSourceVariable;

            bool mIsComponent;

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
#endif