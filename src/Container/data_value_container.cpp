#include "../../include/Container/data_value_container.h"

LOTUS_CREATE_LOCAL_FLAGS(Data_Value_Container, OVERWRITE_OLD_VALUES, 0);

void Data_Value_Container::Merge(
    const Data_Value_Container& rOther,
    const Flags Options)
{
    const bool overwrite_values = Options.Is(OVERWRITE_OLD_VALUES);

    if (overwrite_values) {
        for (const_iterator i = rOther.mData.begin(); i != rOther.mData.end(); ++i) {
        bool variable_already_exist = false;
            for (iterator j = mData.begin(); j != mData.end(); ++j) {
                if (i->first == j->first) {
                    variable_already_exist = true;
                    j->first->Delete(j->second);
                    j->second = i->first->Clone(i->second);
                }
            }
        if (!variable_already_exist) mData.push_back(ValueType(i->first, i->first->Clone(i->second)));
        }
    }

    else {
        for (const_iterator i = rOther.mData.begin(); i != rOther.mData.end(); ++i) {
        bool variable_already_exist = false;
            for (iterator j = mData.begin(); j != mData.end(); ++j) {
                if (i->first == j->first) {
                    variable_already_exist = true;
                }
            }
        if (!variable_already_exist) mData.push_back(ValueType(i->first, i->first->Clone(i->second)));
        }
    }
}