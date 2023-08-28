#include "../../include/Registry/registry_item.h"


Registry_Item::SubRegistryItemType& Registry_Item::GetSubRegistryItemMap()
{
    if(HasValue()) {
        std::cout << "Item " << Name() << " has value and cannot be iterated." << std::endl;
        exit(0);
    }
    return *(std::any_cast<SubRegistryItemPointerType>(mpValue));
}

Registry_Item::SubRegistryItemType& Registry_Item::GetSubRegistryItemMap() const
{
    if(HasValue()) {
        std::cout << "Item " << Name() << " has value and cannot be iterated." << std::endl;
        exit(0);
    }
    return *(std::any_cast<SubRegistryItemPointerType>(mpValue));
}

Registry_Item::SubRegistryItemType::iterator Registry_Item::begin()
{
    return GetSubRegistryItemMap().begin();
}

Registry_Item::SubRegistryItemType::const_iterator Registry_Item::cbegin() const
{
    return GetSubRegistryItemMap().cbegin();
}

Registry_Item::SubRegistryItemType::iterator Registry_Item::end()
{
    return GetSubRegistryItemMap().end();
}

Registry_Item::SubRegistryItemType::const_iterator Registry_Item::cend() const
{
    return GetSubRegistryItemMap().cend();
}

Registry_Item::KeyReturnConstIterator Registry_Item::KeyConstBegin() const
{
    return KeyReturnConstIterator(GetSubRegistryItemMap().cbegin());
}

Registry_Item::KeyReturnConstIterator Registry_Item::KeyConstEnd() const
{
    return KeyReturnConstIterator(GetSubRegistryItemMap().cend());
}

std::string Registry_Item::Info() const
{
    return mName + " RegistryItem ";
}

void Registry_Item::PrintInfo(std::ostream &rOStream) const
{
    rOStream << Info();
}

void Registry_Item::PrintData(std::ostream &rOStream) const
{
    if (HasValue()) {
        rOStream << this->GetValueString();
    } else {
        for(auto& item : GetSubRegistryItemMap()){
            rOStream << item.second->GetValueString() << std::endl;
        }
    }
}

std::string Registry_Item::GetValueString() const
{
    return (this->*(this->mGetValueStringMethod))();
}

bool Registry_Item::HasValue() const
{
    return (mpValue.type() != typeid(SubRegistryItemPointerType));
}

bool Registry_Item::HasItem(std::string const& rItemName) const
{
    if (!HasValue()) {
        SubRegistryItemType& r_map = GetSubRegistryItemMap();
        return (r_map.find(rItemName) != r_map.end());
    } else {
        return false;
    }
}

bool Registry_Item::HasItems() const
{
    return (!HasValue() && !GetSubRegistryItemMap().empty());
}

std::size_t Registry_Item::size()
{
    return GetSubRegistryItemMap().size();
}

Registry_Item const& Registry_Item::GetItem(std::string const& rItemName) const
{
    SubRegistryItemType& r_map = GetSubRegistryItemMap();
    auto iterator = r_map.find(rItemName);
    if(iterator == r_map.end()){
        std::cout << "The RegistryItem " << this->Name() << " does not have an item with name " << rItemName << std::endl;
        exit(0);
    } 
    return *(iterator->second);
}

Registry_Item& Registry_Item::GetItem(std::string const& rItemName)
{
    SubRegistryItemType& r_map = GetSubRegistryItemMap();
    auto iterator = r_map.find(rItemName);
    if(iterator == r_map.end()) {
        std::cout << "The RegistryItem " << this->Name() << " does not have an item with name " << rItemName << std::endl;
        exit(0);
    }
    return *(iterator->second);
}

void Registry_Item::RemoveItem(std::string const& rItemName)
{
    SubRegistryItemType& r_map = GetSubRegistryItemMap();
    auto iterator = r_map.find(rItemName);
    if(iterator == r_map.end()) {
        std::cout << "The RegistryItem " << this->Name() << " does not have an item with name " << rItemName << std::endl;
        exit(0);
    }
    r_map.erase(iterator);
}
