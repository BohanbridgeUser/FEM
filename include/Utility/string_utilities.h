#ifndef _STRING_UTILITIES_H_
#define _STRING_UTILITIES_H_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
class String_Utilities
{
    public:
        /// @name Type Define
        /// @{  

        /// @}


        /// @name Life Circle
        /// @{


        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{
            static std::vector<std::string> SplitStringByDelimiter(const std::string& rString,const char& Delimiter)
            {
                std::stringstream ss(rString);
                std::string token;

                std::vector<std::string> splittedstring;
                while(std::getline(ss,token,Delimiter))
                {
                    splittedstring.push_back(token);
                }
                return splittedstring;
            }

        /// @}


        /// @name Access
        /// @{


        /// @}


        /// @name Inquiry
        /// @{


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