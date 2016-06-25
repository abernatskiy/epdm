#ifndef __POPULATION_H
#define __POPULATION_H

#include <string>
#include <list>
#include "types.h"
#include "reaction.h"
#include "model.h" // class Specie
#include "relation.h"

/* Class representing a population of molecules of a specie.
 */

struct relationAddr_t {
    void* ptrPopAddr;
    std::list<Relation>::iterator itRelAddr;
    std::list<struct relationAddr_t>* ptrListSelf;
    std::list<struct relationAddr_t>::iterator itSelf;
};

typedef struct relationAddr_t relationAddr_t;

/* Defines a type for records of the lookup table listing dependents (a.k.a. U3). Each record contains an
 * iterator to the population and an iterator to the relationship of that population.
 */

class Population
{
public:
    // Attributes
    PROPFLOAT m_ksi;
    MOLINT m_n;
    Specie m_specie;

    // Constructors
    Population(std::string id, int initPop);

    // Methods
    Reaction sampleReaction(PROPFLOAT remainingJuice);
    /* Take the remaining juice from the TotalPopulation-level sampler and subsamples
     * within the Population. See comment to TotalPopulation::sampleReaction()'s
     * implementation in totalPopulation.cpp for details.
     */
    void update(int moleculesAdded);
    /* Updates the population given the change in the number of molecules that
     * occured. Also updates all dependent relations and partial propensity
     * sums (m_ksi and m_lambda). Note that the argument may be negative.
     */
    void buildRelation(std::list<Population>::iterator itSelf, std::list<Population>::reverse_iterator itOther);
    /* Checks if this Population's Specie can react with itOther's and appends
     * Relation object to its internal list of relationships (if the Relation
     * object is not empty). Updates the intermediate sums of propensities
     * accordingly.
     *
     * Takes an iterator to this Population within Total Population's list.
     * This is required to make lookup lists (U3). Not pretty, but can't think
     * of a better way.
     *
     * This function is tricky to think about. A useful fact to keep in mind
     * is that for all calls of the function, the second argument is an iterator
     * to the highest Population in TotalPopulation's list.
     */
    void updateRelation(std::list<Relation>::iterator itRelation, MOLINT newN);
    /* For some Relation object from local list, update all Pi values, recompute
     * their sum Psi and update the local sums (m_ksi abd m_lambda) accordingly
     */
    void removeRelation(std::list<Relation>::iterator itRelation);
    /* Removes a relation of this population and updates the partial propensity
     * sums (m_ksi and m_lambda) accordingly.
     */
    void addDependentRelation(std::list<Population>::iterator itPop, std::list<Relation>::iterator itRel);
    /* Adds a dependent relation record. Updates a pointer inside the Relation
     * reflect the address of the record.
     */
    void removeDependentRelations();
    /* Iterates through all dependent relations of other populations and
     * removes them.
     */
    void removeDependencyRecordsOnOwnRelations();
    /* Removes all dependency records pointing to Relations of this Population.
     */
    void eraseTracesOfExistence();
    /* Removes all Relations of this Population together with dependence
     * records mentioning them, then removes all Relations of other Populations
     * which depend on this Population (according to our records)
     */

    // Operator overloads
    friend std::ostream& operator<<(std::ostream& os, const Population& pop);

private:
//public:
    // Attributes
    std::list<Relation> m_listOfRelations;
    std::list<relationAddr_t> m_dependentRelations; // a.k.a. U3
    PROPFLOAT m_lambda;

    // Methods
    void computeKsi(){};
    void updateKsi(){};
};

#endif // __POPULATION_H
