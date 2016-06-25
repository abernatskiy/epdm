#include "parametersLoader.h"

#include "inih/ini.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <algorithm>

static int handler(void* user, const char* section, const char* name, const char* value){
    // The function which is called every time a new entry is read from the parameters INI file

    std::map<std::string,Parameter>* dict = (std::map<std::string,Parameter>*) user;

    if(strcmp(section, "kinetic model") == 0){ // processing the "kinetic model" section entries

        if(strcmp(value, "true") == 0){
            dict->emplace(name, true);
            return 0;
        }
        if(strcmp(value, "false") == 0){
            dict->emplace(name, false);
            return 0;
        }

        std::string tempstring = std::string(value);
        if(tempstring.find_first_not_of("1234567890.-") == std::string::npos){
            std::size_t minuses = std::count(tempstring.begin(), tempstring.end(), '-');
            if(minuses == 0 || (minuses == 1 && tempstring[0] == '-'))
            {
                std::size_t periods = std::count(tempstring.begin(), tempstring.end(), '.');
                if(periods == 0){
                    dict->emplace(name, std::stoi(value));
                    return 0;
                }
                if(periods == 1){
                    dict->emplace(name, std::stof(value));
                    return 0;
                }
            }
        }

        std::cout << "Invalid value " << value << " of parameter " << name << ", section " << section << std::endl;
        exit(1);
    }
    else{ // no other sections allowed
        std::cout << "Cannot recognize parameter INI file section " << section << std::endl;
        exit(1);
    }
    return 1;
}

void readConfig(std::map<std::string,Parameter>* dict, std::string filename){
    std::cout << "Reading parameters from " << filename << std::endl;
    if(ini_parse(filename.c_str(), handler, (void*) dict) < 0){
        std::cout << "Can't load parameters\n";
        exit(1);
    }
    return;
}

void showConfig(std::map<std::string,Parameter>* dictionary){
    std::cout << "Parameters: ";
    for(auto it=dictionary->begin(); it!=dictionary->end(); it++)
        std::cout << it->first << "=" << (it->second).getString() << " ";
    std::cout << std::endl;
    return;
}
