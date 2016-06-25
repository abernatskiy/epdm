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

    //attributes
    std::string m_id;

    //methods
    std::list<Reaction> reactions(Specie specie);

    // overloading <<
    friend std::ostream& operator<<(std::ostream& os, const Specie& sp);
};

#endif // __SPECIE_H
