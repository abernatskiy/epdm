#include <map>
#include <string>
#include "parameter.h"
#include "model.h"

/* model.cpp for a elastic collisions model:
 * - For any pair of non-void species A and B, reactions() produces a list
 *   containing the only reaction A+B->A+B.
 * - For any other pair of species an empty list is produced.
 */

/* Global configuration dictionary */
extern std::map<std::string,Parameter> configDict;

Specie::Specie(std::string id) : modelName("elastic-collisions"), m_id(id) {}

std::list<Reaction> Specie::reactions(Specie other)
{
    std::list<Reaction> allReactions;

		// If none of the reactants is a void specie...
    if( !(m_id==std::string("")) && !(other.m_id==std::string("")) )
    {
        // ...get the rate of the collision reaction from parameters.ini...
        float collisionRate = configDict["collisionRate"].getFloat();

        // ...create the elastic collision reaction...
        Reaction elasticCollision(m_id, 1, other.m_id, 1, collisionRate);
        elasticCollision.addProduct(m_id, 1);
        elasticCollision.addProduct(other.m_id, 1);

        // ... and append it to the list of possible reactions
        allReactions.push_back(elasticCollision);
		}

    return allReactions;
}
