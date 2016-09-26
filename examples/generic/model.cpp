#include <map>
#include <string>
#include <iostream>
#include "parameter.h"
#include "model.h"

/* model.cpp for a generic model
 * reactions() produces empty reaction list
 * for any pair of species
 */

// TODO: write all the requirements to the models in this files

/* Global configuration dictionary */
extern std::map<std::string,Parameter> configDict;

Specie::Specie(std::string id)
{
    modelName = std::string("generic-model");
    m_id = id;
}

std::list<Reaction> Specie::reactions(Specie specie)
{
    std::list<Reaction> allReactions; // all the reactions two species can have
    return allReactions;
}
