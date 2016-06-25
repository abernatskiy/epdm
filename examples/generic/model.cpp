#include <map>
#include <string>
#include <iostream>
#include "parameter.h"
#include "model.h"
/* generic model
 * produces empty reaction list for every pair of species
 *
 */

/* Global configuration dictionary */
extern std::map<std::string,Parameter> configDict;

/* HP-model-specific global variables */
//extern std::map<std::string,std::string> catPatterns;
//extern std::map<std::string,int> wellDepths;

Specie::Specie(std::string id){
    modelName = std::string("generic-model");
    m_id = id;
}

// Overloading <<
std::ostream& operator<<(std::ostream& os, const Specie& sp){
    os << sp.m_id ;
    return os;
}

/* Methods */

// Defining reactions here
std::list<Reaction> Specie::reactions(Specie specie){
    std::list<Reaction> allReactions; // all the reactions two species can have
    return allReactions;
}

Specie::~Specie(){

}
