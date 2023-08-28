#include "../../include/Registry/registry.h"
Registry_Item* Registry::mspRootRegistryItem = nullptr;
std::once_flag Registry::flag_once;

Registry_Item& Registry::GetRootRegistryItem()
{
    if (!mspRootRegistryItem) {
        std::call_once(flag_once, [](){
            static Registry_Item root_item("Registry");
            mspRootRegistryItem = &root_item;
        });
    }

    return *mspRootRegistryItem;
}

std::size_t Registry::size()
{
    return mspRootRegistryItem->size();
}

Registry_Item& Registry::GetItem(std::string const& rItemFullName)
{
    auto item_path = String_Utilities::SplitStringByDelimiter(rItemFullName, '.');
    if(item_path.empty()) {
        std::cout << "The item full name is empty" << std::endl;
        exit(0);
    }

    Registry_Item* p_current_item = &GetRootRegistryItem();

    for(std::size_t i = 0 ; i < item_path.size() ; i++){
        auto& r_item_name = item_path[i];
        if(p_current_item->HasItem(r_item_name)){
            p_current_item = &p_current_item->GetItem(r_item_name);
        }
        else{
            std::cout << "The item \"" << rItemFullName << "\" is not found in the registry. The item \"" << p_current_item->Name() << "\" does not have \"" << r_item_name << "\"" << std::endl;
            exit(0);
        }
    }

    return *p_current_item;
}

bool Registry::HasItem(std::string const& rItemFullName)
{
    auto item_path = String_Utilities::SplitStringByDelimiter(rItemFullName, '.');
    if(item_path.empty()) {
        std::cout << "The item full name is empty" << std::endl;
        exit(0);
    }


    Registry_Item* p_current_item = &GetRootRegistryItem();

    for(std::size_t i = 0 ; i < item_path.size() ; i++){
        auto& r_item_name = item_path[i];
        if(p_current_item->HasItem(r_item_name)){
            p_current_item = &p_current_item->GetItem(r_item_name);
        }
        else{
            return false;
        }
    }
    return true;
}

bool Registry::HasValue(std::string const& rItemFullName)
{
    return GetItem(rItemFullName).HasValue();
}

bool Registry::HasItems(std::string const& rItemFullName)
{
    return GetItem(rItemFullName).HasItems();
}

void Registry::RemoveItem(std::string const& rItemFullName)
    {

        auto item_path = String_Utilities::SplitStringByDelimiter(rItemFullName, '.');
        if(item_path.empty()) {
            std::cout<< "The item full name is empty" << std::endl;
            exit(0);
        }

        Registry_Item* p_current_item = &GetRootRegistryItem();

        for(std::size_t i = 0 ; i < item_path.size() - 1 ; i++){
            auto& r_item_name = item_path[i];
            if(p_current_item->HasItem(r_item_name)){
                p_current_item = &p_current_item->GetItem(r_item_name);
            }
            else{
                std::cout  << "The item \"" << rItemFullName << "\" is not found in the registry. The item \"" << p_current_item->Name() << "\" does not have \"" << r_item_name << "\"" << std::endl;
                exit(0);
            }
        }

        auto& r_item_name = item_path.back();
        if(p_current_item->HasItem(r_item_name)){
            p_current_item->RemoveItem(r_item_name);
        }
        else{
            std::cout << "The item \"" << rItemFullName << "\" is not found in the registry. The item \"" << p_current_item->Name() << "\" does not have \"" << r_item_name << "\"" << std::endl;
            exit(0);
        }
    }