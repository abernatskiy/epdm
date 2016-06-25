#ifndef __PARAMETERS_LOADER_H
#define __PARAMETERS_LOADER_H

#include <string>
#include <map>
#include "parameter.h"

void readConfig(std::map<std::string,Parameter>* dict, std::string filename);
void showConfig(std::map<std::string,Parameter>* dictionary);

#endif // __PARAMETERS_LOADER_H
