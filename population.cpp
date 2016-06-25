#include <iostream>
#include <cstdlib>
#include "population.h"

Population::Population(std::string id, int initPop){
    m_specie = Specie(id);
    m_n = (MOLINT) initPop;
    m_ksi = 0.f;
    m_lambda = 0.f;
}

Reaction Population::sampleReaction(PROPFLOAT remainingJuice){
    if( m_n <= 0 ){
        std::cout << "ERROR: Sampling from a population of " << m_n << " molecules. This shouldn't happen. Exiting.\n";
        exit(EXIT_FAILURE);
    }

    PROPFLOAT localJuice = remainingJuice/((PROPFLOAT) m_n);
    for( auto itRel = m_listOfRelations.begin(); itRel != m_listOfRelations.end(); itRel++ ){
        localJuice -= itRel->m_psi;
        if( localJuice < 0.f ){
            PROPFLOAT juiceRemainingAfterLocalSampling = localJuice + itRel->m_psi;
            return itRel->sampleReaction(juiceRemainingAfterLocalSampling);
        }
    }

/*  Sampling sometimes gets broken due to floating point rounding errors. To fix it, I would need a library providing
 *  truly associative addition with reversible subtraction. Such library does not seem to exist yet, although
 *  there is an algorithm for associative addition described here:
 *
 *  http://www.eecs.berkeley.edu/~hdnguyen/public/papers/ARITH21_Fast_Sum.pdf
 *
 *  Won't fix for the time being. */

//    std::cout << "WARNING: Population-level sampling failed, perhaps due to the numerical error. Returning an invalid reaction to cause resampling.\n";
//    std::cout << std::scientific << "Sampled with remaining juice of " << remainingJuice << ", sum of partial propensities of " << m_ksi << ", difference " << remainingJuice - m_ksi << std::endl;

    Reaction invalidReaction("", -100, "", -100, 1000000.0);
    return invalidReaction;

////  Older, less precise solution to the problem of numerical errors at sampling phase
//    auto itRel = m_listOfRelations.end();
//    itRel--;
//    return itRel->sampleReaction(itRel->m_psi);
}

void Population::update(int moleculesAdded){
    // Note that the argument may negative, while m_n may be of potentially unsigned type MOLINT. This works, I checked.
//    std::cout << "Population::update called on " << (*this) << " with molecular change of " << moleculesAdded << std::endl;
    m_n += moleculesAdded;

    // Go through all records of dependent relations (all reacords in U3) and ask the other Populations to update their relations
    for(auto itRec = m_dependentRelations.begin(); itRec != m_dependentRelations.end(); itRec++ ){
        // Unpacking the record
        Population* ptrOtherPop = (Population*) (itRec->ptrPopAddr);
        std::list<Relation>::iterator itOtherRel = itRec->itRelAddr;

        // Asking for the update
        ptrOtherPop->updateRelation(itOtherRel, m_n);
    }
}

void Population::buildRelation(std::list<Population>::iterator itSelf, std::list<Population>::reverse_iterator itOther){
//    std::cout << "Building relation\nof " << (*this) << "to " << (*itOther) << std::endl;

    Relation newRel(m_specie, itOther->m_specie, itOther->m_n);
    if( newRel.isEmpty() ){
//        std::cout << "Done building empty relation from " << m_specie.m_id << " to " << itOther->m_specie.m_id << std::endl;
        return;
    }
//    std::cout << "Nonempty relation found from " << m_specie.m_id << " to " << itOther->m_specie.m_id << ": " << newRel;

    m_lambda += newRel.m_psi;
    m_ksi = m_lambda*((PROPFLOAT) m_n);
    m_listOfRelations.push_back(newRel);
    auto itToNewRelation = m_listOfRelations.end();
    itToNewRelation--;

    itOther->addDependentRelation(itSelf, itToNewRelation);

//    std::cout << "Done building relation\n\n";
}

void Population::updateRelation(std::list<Relation>::iterator itRelation, MOLINT newN){
//    std::cout << "Population::updateRelation called on " << (*this) << " to update " << *itRelation;

    m_lambda -= itRelation->m_psi;
    itRelation->update(newN);
    m_lambda += itRelation->m_psi;

    m_ksi = m_lambda*((PROPFLOAT) m_n);
}

void Population::removeRelation(std::list<Relation>::iterator itRelation){
//    std::cout << "Population::removeRelation called on " << (*this) << " to remove " << *itRelation;

    m_lambda -= itRelation->m_psi;
    m_listOfRelations.erase(itRelation);

    m_ksi = m_lambda*((PROPFLOAT) m_n);
}

void Population::addDependentRelation(std::list<Population>::iterator itPop, std::list<Relation>::iterator itRel){
    relationAddr_t relRecord;
    relRecord.ptrPopAddr = (void*) &(*itPop);
    relRecord.itRelAddr = itRel;
    relRecord.ptrListSelf = &m_dependentRelations;

    m_dependentRelations.push_back(relRecord);
    auto itLastRec = m_dependentRelations.end();
    itLastRec--;
    itLastRec->itSelf = itLastRec;
    itRel->m_ptrRecordOfSelf = ((void*) &(*itLastRec));
}

void Population::removeDependentRelations(){
//    std::cout << "Population::removeDependentRelations called on " << (*this);
    for(auto itRec = m_dependentRelations.begin(); itRec != m_dependentRelations.end(); itRec++ ){
        Population* ptrOtherPop = (Population*) (itRec->ptrPopAddr);
        std::list<Relation>::iterator itOtherRel = itRec->itRelAddr;

        ptrOtherPop->removeRelation(itOtherRel);
    }
}

void Population::removeDependencyRecordsOnOwnRelations(){
    // do not forget to erase the records of Relations' existence together with the relations
    for( auto itRel = m_listOfRelations.begin(); itRel != m_listOfRelations.end(); itRel++ ){
        relationAddr_t* ptrRel = (relationAddr_t*) (itRel->m_ptrRecordOfSelf);
        std::list<struct relationAddr_t>::iterator itRec = ptrRel->itSelf;
        (ptrRel->ptrListSelf)->erase(itRec);
    }
}

void Population::eraseTracesOfExistence(){
    removeDependencyRecordsOnOwnRelations();
    removeDependentRelations();
}

std::ostream& operator<<(std::ostream& os, const Population& pop){
    os << "Population of specie " << pop.m_specie << " with " << pop.m_n << " molecules in it\n";
    os << "Full propensity of the population: " << pop.m_ksi << std::endl;
    os << "Relations: ";
    for( auto itRel = pop.m_listOfRelations.begin(); itRel != pop.m_listOfRelations.end(); itRel++ )
        os << (*itRel) << std::endl;
    os << "Dependent relations: up to " << pop.m_dependentRelations.size() << std::endl;
//    for(auto itRec = pop.m_dependentRelations.begin(); itRec != pop.m_dependentRelations.end(); itRec++ ){
//        std::list<Population>::iterator itOtherPop;
//        std::list<Relation>::iterator itOtherRel;
 //       std::tie (itOtherPop, itOtherRel) = *itRec;
//
//        std::cout << " in population of " << itOtherPop->m_specie.m_id << ",";
 //   }
    return os;
}
