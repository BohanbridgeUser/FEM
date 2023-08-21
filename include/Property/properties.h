#ifndef _PROPERTIES_H_
#define _PROPERTIES_H_
#include "../define.h"
#include <vector>
#include <unordered_map>
class Properties
{
    public:
        ///@name Define 
        ///@{ 
            LOTUS_POINTER_DEFINE(Properties)
            typedef std::vector<double> DataContainerType;
            typedef std::unordered_map<std::string,DataContainerType,std::hash<std::string>> TableContainerType;
        ///}

        ///@name Lift Circle 
        ///@{
            //Constructor
            Properties()
            {

            }
            Properties(const TableContainerType& ThisTableContainer)
            :mTableContainerType(ThisTableContainer)
            {

            }
            Properties(TableContainerType& another)
            :mTableContainerType(another)
            {

            }
            //Destructor
            ~Properties()
            {
                
            }
        ///}

        /// @name Operators
        /// @{

        /// @}

        /// @name Operations
        /// @{
            /**
             * @details This function uses `insert_or_assign` method of cxx17.
            */
            void InsertProperty(const std::string& name, DataContainerType& datacontainer)
            {
                mTableContainerType.insert_or_assign(name,datacontainer);
            }
            
        /// @}

        /// @name Access
        /// @{
            friend std::ostream& operator<<(std::ostream& os, const Properties& another)
            {
                for (auto it=another.mTableContainerType.begin();it!=another.mTableContainerType.end();it++)
                {
                    os << it->first << ": ";
                    for (auto j=it->second.begin();j!=it->second.end();++j)
                    {
                        os << *j << " ";
                    }
                    os << std::endl;
                }
                return os;
            }
        /// @}
    private:
        TableContainerType mTableContainerType;
};
#endif