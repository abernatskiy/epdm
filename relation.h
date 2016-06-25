#ifndef __RELATION_H
#define __RELATION_H

#include <list>
#include "reaction.h"
#include "model.h" // class Specie
#include "types.h"

/* This class represents a relation of a population of
 * some species SI to a population of other species SJ.
 * It contains a list of all reactions that can happen
 * between SI and SJ, together with the sum of partial
 * propensities of all these reactions, computed with
 * respect to SI (i.e., one needs to multiply this sum
 * by the population of SI to get full propensity).
 *
 * Species SI and SJ may coincide.
 */

class Relation
{
public:
    // Attributes
    PROPFLOAT m_psi; // the sum of partial propensities
    void* m_ptrRecordOfSelf; // pointer to the dependence record about this Relation which is kept in Population in the other Specie

    // Constructors
    Relation(Specie ofSp, Specie toSp, MOLINT popToSp);
    /* fromSp is the owner, Specie object of the Populaiton which keeps this Relation
     * toSp is the Specie, on the population of which partial propensities may depend
     * popToSp is the population of the latter
     */

    // Methods
    Reaction sampleReaction(PROPFLOAT remainingJuice);
    /* Takes the remaining juice from the Population-level sampler and subsamples
     * within the Relation. See comment to TotalPopulation::sampleReaction()'s
     * implementation in totalPopulation.cpp for details.
     */
    void update(MOLINT newNToSp);
    /* Updates the relation when the population of the other specie changes. The
     * other specie here is the one TO which the Relation is defined and on molecular
     * count of which its partial propensities depend.
     */
    bool isEmpty(){return m_listOfReactions.empty();};
    /* Returns true iff there are no reactions between the species
     */

    // Operator overloads
    friend std::ostream& operator<<(std::ostream& os, const Relation& rel);
private:
    // Attributes
    std::string m_fromSpId; // id of the owner specie
    std::list<Reaction> m_listOfReactions; // list of the reactions between the owner and the other specie
};

#endif // __RELATION_H
