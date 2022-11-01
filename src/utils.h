#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <cstdarg>
#include <sstream>
/*!
 * \brief Some utilities functions
 */
namespace Utils {
/*!
 * \brief format
 * \details format a string with strings passed in parameters. The strings have to be passed after th earguments containing the number of parameters
 * \param string, the string to format
 * \param nb, the number of parameters, the
 * \return std::string, the formatted string
 */
static std::string format(std::string string,int nb, ...){
        va_list ap;
        va_start(ap,nb);
        std::string output;
        std::string::iterator it;
        it = string.begin();
        while(it<string.end()){

            //Search through string
            bool found = false;
            while(it<string.end() & !found){
                if(*it=='#'){
                    char* replacement_char;
                    replacement_char=va_arg(ap,char*);
                    std::string replacement_string = replacement_char;
                    output += replacement_string;
                    found = true;
                    nb--;
                }else{
                    output+= *it;
                }
                it++;
            }
        }
        return output;
    }
/*!
 * \brief doubleToStr, converts a double to a std::string
 * \param v
 * \return std::string, the converted string
 */
static std::string doubleToStr(double v){
        std::stringstream sstream;
        std::string output;
        sstream << v;
        sstream >> output;
        return output;
    }
}

#endif // UTILS_H
