#ifndef _REGISTRY_H_
#define _REGISTRY_H_
#include "../define.h"
#include "registry_item.h"

class Registry
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_POINTER_DEFINE(Registry)

        /// @}


        /// @name Life Circle
        /// @{
            Registry(){}

            ~Registry(){}

        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{
            template<typename TItemType, class... TArgumentsList >
            static Registry_Item& AddItem(std::string const& rItemFullName,
                                         TArgumentsList&&... Arguments)
            {
                auto item_path = String_Utilities::SplitStringByDelimiter(rItemFullName, '.');
                
                if(item_path.empty()) 
                {
                    std::cout << "The item full name is empty" << std::endl;
                    exit(0);
                }

                Registry_Item* p_current_item = &GetRootRegistryItem();

                for(std::size_t i = 0 ; i < item_path.size() - 1 ; i++)
                {
                    auto& r_item_name = item_path[i];
                    if(p_current_item->HasItem(r_item_name)){
                        p_current_item = &p_current_item->GetItem(r_item_name);
                    }
                    else{
                        p_current_item = &p_current_item->AddItem<Registry_Item>(r_item_name);
                    }
                }

                // I am doing the last one out of the loop to create it with the given type and argument
                auto& r_item_name = item_path.back();
                if(p_current_item->HasItem(r_item_name)){
                    std::cout  << "The item \"" << rItemFullName << "\" is already registered." << std::endl;
                    exit(0);
                }
                else{
                    p_current_item = &p_current_item->AddItem<TItemType>(r_item_name, std::forward<TArgumentsList>(Arguments)...);
                }

                return *p_current_item;

            }

            static void RemoveItem(std::string const& ItemName);
        /// @}


        /// @name Access
        /// @{
            static Registry_Item& GetItem(std::string const& rItemFullName);

             static auto begin()
            {
                return mspRootRegistryItem->begin();
            }

            static auto cbegin()
            {
                return mspRootRegistryItem->cbegin();
            }

            static auto end()
            {
                return mspRootRegistryItem->end();
            }

            static auto const cend()
            {
                return mspRootRegistryItem->cend();
            }

            template<typename TDataType>
            static TDataType const& GetValue(std::string const& rItemFullName)
            {
                return GetItem(rItemFullName).GetValue<TDataType>();
            }

           
        /// @}


        /// @name Inquiry
        /// @{
            static std::size_t size();

            static bool HasItem(std::string const& rItemFullName);

            static bool HasValue(std::string const& rItemFullName);

            static bool HasItems(std::string const& rItemFullName);

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
            static Registry_Item& GetRootRegistryItem();

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
            static Registry_Item* mspRootRegistryItem;
            static std::once_flag flag_once;
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