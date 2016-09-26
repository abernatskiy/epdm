#ifndef __SPECIE_H
#define __SPECIE_H

#include <vector>
#include <string>
#include <list>
#include "parameter.h"
#include "reaction.h"

/* model.h for a generic model
 * reactions() produces an empty reaction list
 * for any pair of species
 */

class Specie
{
public:
    Specie(){}; // do not modify
    ~Specie(){}; // do not modify

    std::string modelName; // name of the model to use in logs, messages and trajectory output comments
    Specie(std::string id); // this constructor must ensure that the object is defined by id uniquely and that the id is saved at m_id

    std::string m_id; // must be unique for each Specie object

    std::list<Reaction> reactions(Specie other); // must return a list of reactions between two Species (this and other); must be symmetric (s1.reactions(s2) == s2.reactions(s1))

    friend std::ostream& operator<<(std::ostream& os, const Specie& sp){os << sp.m_id; return os;}; // used in logs and messages

private:
    // feel free to declare any private methods here
};

#endif // __SPECIE_H
