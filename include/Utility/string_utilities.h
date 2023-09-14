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

        /// @name Input And Output
        /// @{
            /**
             * @brief Prints the data of an object of type TClass to the given output stream with indentation.
             * @param rOStream The output stream where the data will be printed.
             * @param rThisClass The object of type TClass whose data will be printed.
             * @param Identation (optional) The string used for the indentation. Default is four spaces.
             */
            template<class TClass>
            static void PrintDataWithIdentation(
                std::ostream& rOStream,
                const TClass& rThisClass,
                const std::string Identation = "\t"
                )
            {
                // Auxiliary stream and line
                std::stringstream ss;
                std::string line;
                rThisClass.PrintData(ss);

                // Get the output string from the stringstream.
                const std::string& r_output = ss.str();

                // Split the output string into lines.
                std::istringstream iss(r_output);
                while (std::getline(iss, line)) {
                    // Here, 'line' is a single line from the output.
                    rOStream << Identation << line << "\n";
                }
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