#ifndef _PROPERTIES_H_
#define _PROPERTIES_H_
#include "define.h"
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
    private:
        TableContainerType mTableContainerType;
};
#endif