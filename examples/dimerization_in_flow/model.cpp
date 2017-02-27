#include <map>
#include <string>
#include "parameter.h"
#include "model.h"

/* model.cpp for a dimeriation model. Dimers are removed as soon as
 * they are created (e.g. by precipitation).
 * - For a pair of void species, reactions() produces a list containing
 *   the only production reaction \varnothing -> A
 * - For two species A, reactions() produces a list containing the only
 *   raection A+A->\varnothing.
 * - For any other pair of species an empty list is produced.
 * Note that for productionRate=10. and dimerizationRate=0.2 the model
 * is identical to one example model shipped with libpssa-1.0.0.
 * The model is at
 * examples/Hreactions/sbml/Homoreaction.sbml
 * libpssa aka pssalib can be found here:
 * http://mosaic.mpi-cbg.de/?q=downloads/stochastic_chemical_net
 */

/* Global configuration dictionary */
extern std::map<std::string,Parameter> configDict;

Specie::Specie(std::string id) : modelName("elastic-collisions"), m_id(id) {}

std::list<Reaction> Specie::reactions(Specie other)
{
    std::list<Reaction> allReactions;

		// If both reactants are void species...
    if( m_id=="" && other.m_id=="" )
    {
        // ...create the production reaction...
        // Note: both stoichiomeric coefficients must be zeros for production reactions!
        Reaction productionOfA(m_id, 0, other.m_id, 0, configDict["productionRate"].getFloat());
        productionOfA.addProduct("A", 1);

        // ... and append it to the list of possible reactions
        allReactions.push_back(productionOfA);
		}
    else if( m_id=="A" && other.m_id=="A" )
    {
        // ...create the dimerization reaction...
        Reaction dimerization(m_id, 1, other.m_id, 1, configDict["dimerizationRate"].getFloat());

        // ... and append it to the list of possible reactions
        allReactions.push_back(dimerization);
		}

    return allReactions;
}
