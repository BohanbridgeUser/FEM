#ifndef _REGISTRY_ITEM_H_
#define _REGISTRY_ITEM_H_
#include "../define.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <any>
#include <unordered_map>
#include <memory>
#include <functional>
class Registry_Item
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_POINTER_DEFINE(Registry_Item)
            typedef Registry_Item
                                                                                           RegistryItemType;
            typedef std::unordered_map<std::string, std::shared_ptr<Registry_Item>>
                                                                                        SubRegistryItemType;
            typedef std::shared_ptr<SubRegistryItemType>
                                                                                 SubRegistryItemPointerType;
            
            class KeyReturnConstIterator
            {
            public:
                ///@name Type Definitions
                ///@{

                using BaseIterator      = SubRegistryItemType::const_iterator;
                using iterator_category = std::forward_iterator_tag;
                using difference_type   = BaseIterator::difference_type;
                using value_type        = SubRegistryItemType::key_type;
                using const_pointer     = const value_type*;
                using const_reference   = const value_type&;

                ///@}
                ///@name Life Cycle
                ///@{

                KeyReturnConstIterator()
                {}

                KeyReturnConstIterator(const BaseIterator Iterator)
                    : mIterator(Iterator)
                {}

                KeyReturnConstIterator(const KeyReturnConstIterator& rIterator)
                    : mIterator(rIterator.mIterator)
                {}

                ///@}
                ///@name Operators
                ///@{

                KeyReturnConstIterator& operator=(const KeyReturnConstIterator& rIterator)
                {
                    this->mIterator = rIterator.mIterator;
                    return *this;
                }

                const_reference operator*() const
                {
                    return mIterator->first;
                }

                const_pointer operator->() const
                {
                    return &(mIterator->first);
                }

                KeyReturnConstIterator& operator++()
                {
                    ++mIterator;
                    return *this;
                }

                KeyReturnConstIterator operator++(int)
                {
                    KeyReturnConstIterator tmp(*this);
                    ++(*this);
                    return tmp;
                }

                bool operator==(const KeyReturnConstIterator& rIterator) const
                {
                    return this->mIterator == rIterator.mIterator;
                }

                bool operator!=(const KeyReturnConstIterator& rIterator) const
                {
                    return this->mIterator != rIterator.mIterator;
                }

                ///@}
            private:
                ///@name Member Variables
                ///@{

                BaseIterator mIterator;

                ///@}
            };

        /// @}


        /// @name Life Circle
        /// @{
            /// Default constructor deleted.
            Registry_Item() = delete;
            /// Constructor with the name
            Registry_Item(const std::string& rName)
                : mName(rName),
                mpValue(std::make_shared<SubRegistryItemType>()),
                mGetValueStringMethod(&Registry_Item::GetRegistryItemType) {}
            /// Constructor with the name and lambda
            template <typename TItemType, typename... TArgs>
            Registry_Item(
                const std::string &rName,
                const std::function<std::shared_ptr<TItemType>(TArgs...)> &rValue)
                : mName(rName),
                mpValue(rValue()),
                mGetValueStringMethod(&Registry_Item::GetItemString<TItemType>) {}
            /// Constructor with the name and value
            template<class TItemType>
            Registry_Item(
                const std::string&  rName,
                const TItemType& rValue)
                : mName(rName),
                mpValue(std::make_shared<TItemType>(rValue)),
                mGetValueStringMethod(&Registry_Item::GetItemString<TItemType>) {}
            /// Constructor with the name and shared ptr
            template<class TItemType>
            Registry_Item(
                const std::string&  rName,
                const std::shared_ptr<TItemType>& pValue)
                : mName(rName),
                mpValue(pValue),
                mGetValueStringMethod(&Registry_Item::GetItemString<TItemType>) {}
            // Copy constructor deleted
            Registry_Item(Registry_Item const& rOther) = delete;
            /// Destructor.
            ~Registry_Item() = default;


        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{
            // Adding sub value item
            template<typename TItemType, class... TArgumentsList>
            Registry_Item& AddItem(
                std::string const& ItemName,
                TArgumentsList&&... Arguments)
            {
                if(this->HasItem(ItemName))
                {
                    std::cout << "The RegistryItem '" << this->Name() 
                    << "' already has an item with name "
                    << ItemName << "." << std::endl;
                    exit(0);
                }

                /* Get Value Type*/   
                using ValueType = 
                typename std::conditional<std::is_same<TItemType, Registry_Item>::value, SubRegistryItemFunctor, SubValueItemFunctor<TItemType>>::type;

                auto insert_result = GetSubRegistryItemMap().emplace(
                    std::make_pair(
                        ItemName,
                        ValueType::Create(ItemName, std::forward<TArgumentsList>(Arguments)...)
                        )
                    );

                if(!insert_result.second){
                    std::cout << "Error in inserting '" << ItemName
                    << "' in registry item with name '" << this->Name() << "'." << std::endl;
                    exit(0);
                }
                return *insert_result.first->second;
            }

        /// @}


        /// @name Access
        /// @{
            SubRegistryItemType::iterator begin();

            SubRegistryItemType::const_iterator cbegin() const;

            SubRegistryItemType::iterator end();

            SubRegistryItemType::const_iterator cend() const;

            KeyReturnConstIterator KeyConstBegin() const;

            KeyReturnConstIterator KeyConstEnd() const;

            const std::string& Name() const  { return mName; }

            Registry_Item const& GetItem(std::string const& rItemName) const;

            Registry_Item& GetItem(std::string const& rItemName);

            template<typename TDataType> 
            TDataType const& GetValue() const
            {
                return *(std::any_cast<std::shared_ptr<TDataType>>(mpValue));
            }

            void RemoveItem(std::string const& rItemName);

        /// @}


        /// @name Inquiry
        /// @{
            std::size_t size();

            bool HasValue() const;

            bool HasItems() const;

            bool HasItem(std::string const& rItemName) const;

        /// @}

        /// @name Input and Output
        /// @{
            // Turn back information as a string.
            std::string Info() const;

            /// Print information about this object.
            void PrintInfo(std::ostream& rOStream) const;

            /// Print object's data.
            void PrintData(std::ostream& rOStream) const;
        /// @}

    protected:
        /// @name Protected Static Member Variables
        /// @{


        /// @}


        /// @name Protected Member Variables
        /// @{
            std::string                 mName;
            std::any                    mpValue;
            std::string (Registry_Item::*mGetValueStringMethod)() const;

            class SubRegistryItemFunctor
            {
                public:
                    template<class... TArgumentsList>
                    static inline Registry_Item::SharedPointer Create(
                        std::string const& ItemName,
                        TArgumentsList&&... Arguments)
                    {
                        return std::make_shared<Registry_Item>(ItemName);
                    }
            };

            template<typename TItemType>
            class SubValueItemFunctor
            {
            public:
                template<class... TArgumentsList, class TFunctionType = std::function<std::shared_ptr<TItemType>(TArgumentsList...)>>
                static inline Registry_Item::SharedPointer Create(std::string const& ItemName,
                                                            TFunctionType && Function)
                {
                    return std::make_shared<Registry_Item>(ItemName, std::forward<TFunctionType>(Function));
                }

                template<class... TArgumentsList>
                static inline Registry_Item::SharedPointer Create(std::string const& ItemName,
                                                            TArgumentsList&&... Arguments)
                {
                    return std::make_shared<Registry_Item>(ItemName, make_shared<TItemType>(std::forward<TArgumentsList>(Arguments)...));
                }
            };
        /// @}


        /// @name Protected Operators
        /// @{


        /// @}


        /// @name Protected Operations
        /// @{
            std::string GetRegistryItemType() const
            {
                return mpValue.type().name();
            }

            template<class TItemType>
            std::string GetItemString() const
            {
                std::stringstream buffer;
                buffer << this->GetValue<TItemType>();
                return buffer.str();
            }

            std::string GetValueString() const;

            SubRegistryItemType& GetSubRegistryItemMap();

            SubRegistryItemType& GetSubRegistryItemMap() const;
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