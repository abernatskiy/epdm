#ifndef __REACTION_H
#define __REACTION_H

#include <string>
#include <vector>
#include <iostream>
#include <tuple>
#include <cstdlib>
#include "types.h"

/* Class describing a chemical reaction.
 * It stores the involved species in form of their IDs - unique identifier strings.
 * All data on which the chemical behavior of specie depends (such as its activation
 * state) must be encoded into these strings.
 *
 * Each of these strings is paired an integer describing stoichiometry. The pairs
 * are then stored as a vector. First two pairs in that vector always describe
 * reactants. The reagents are always removed as a result of reaction, so their
 * stoichiometries are stored as non-positive numbers. Despite that, the constructor
 * takes them to be non-negative. This is done for convenience of human users writing
 * the codes with chemical equations in their standard forms in mind. Example:
 *
 * 2H2 + O2 -> 2H20
 *
 * In this equation, stoichiometries of hydrogen, oxygen and water are written as
 * 2, 1 and 2, and this class follows that convention construction-time. However,
 * internally it stores them as -2, -1 and 2, since as a result of this reaction
 * two molecules of hydrogen and one molecule of oxygen were removed and two
 * molecules of water were added.
 *
 * Rate of reaction is also stored in the objects of this class.
 */

/* ON CONVENTIONS
 * This code is designed for handling bimolecular reactions only. This
 * means that no stoichiometry can ever have absolute value greater that 1.
 *
 * Conventions:
 *   bimolecular reaction with different reactants: 1 "A" + 1 "B" -> products
 *   bimolecular reaction with identical reactants: 1 "A" + 1 "A" -> products
 *   unimolecular reaction:                         1 "A" + 0 "A" -> products
 *   source reaction:                               0 "" + 0 "" -> products
 * Here, "" is an empty specie, or "vacuum". Its ID is an empty string. It's
 * stoichiometries should always be 0. You can choose to add or not to add
 * it as a product into degeneracy reactions, although the former will lead
 * to a small performance penalty.
 */

#define specieRecord_t std::tuple<std::string, int>
/* A type defining a record for a specie involved into a reaction:
 * tuple containing an id string and a stoichoimetric coefficient
 */
// optimization may be possible here by using a C structure instead

class Reaction
{
public:
    // Attributes
    std::vector<specieRecord_t> m_records; // vector of tuples holding ids of all species involved in the reaction and their stoichiometric coefficients
    PROPFLOAT m_rate;
    PROPFLOAT m_partialPropensity; // a.k.a. Pi
    std::string m_pPWRespectTo; // id of the reagent, with respect to which the partial propensity was last computed (holder of the Relation)

    // Constructor
    Reaction(std::string reactant0, int stoichiometry0, std::string reactant1, int stoichiomentry1, PROPFLOAT rate);

    // Operator reaload
    friend std::ostream& operator<<(std::ostream& os, const Reaction& rc);

    // Methods
    void addProduct(std::string product, int stoichiometry){addSpecie(product, stoichiometry);};
    void computePartialPropensity(std::string wRespectToSp, MOLINT populationOfOtherSp);
    /* Computes a partial propensity with respect to a given reactant, knowing the population of
     * the other reactant.
     * WARNING! The second argument is the population of the OTHER reagent, not wRespectToSp.
     */

    bool isValid();
    /* Is true if all of the below is true.
     * - stoichiometry of reactants is in {-1,0}
     * - if both reactants are empty stings, both stoichiometries are 0
     * - if one of stoichiometries of reactants is zero and the other is not, the reactants are the same
     * - stoichiometry of products is greater than 0
     * More conditions may be added later.
     */
private:
    void addSpecie(std::string specie, int stoichiometry);
};

#endif
