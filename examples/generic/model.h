#ifndef __SPECIE_H
#define __SPECIE_H

#include <vector>
#include <string>
#include <list>
#include "parameter.h"
#include "reaction.h"

/* generic model
 * produces empty reaction list for every pair of species
 *
 */

class Specie
{
public:
    std::string modelName;
    Specie(){};
    Specie(std::string id);
    ~Specie();

    std::string m_id; // must be unique for each Specie object

    std::list<Reaction> reactions(Specie other); // required method: must return a list of reactions between two Species (this and other)

    friend std::ostream& operator<<(std::ostream& os, const Specie& sp); // required friend function
};

#endif // __SPECIE_H
