#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <cstdarg>
#include <sstream>
namespace Utils {
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
static std::string doubleToStr(double v){
        std::stringstream sstream;
        std::string output;
        sstream << v;
        sstream >> output;
        return output;
    }
}

#endif // UTILS_H
