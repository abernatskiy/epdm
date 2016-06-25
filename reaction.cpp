#include "reaction.h"

Reaction::Reaction(std::string reactant0, int stoichiometry0, std::string reactant1, int stoichiometry1, PROPFLOAT rate){
	addSpecie(reactant0, -1*stoichiometry0);
	addSpecie(reactant1, -1*stoichiometry1);
	m_rate = rate;
    m_partialPropensity = -1.f;
}

std::ostream& operator<<(std::ostream& os, const Reaction& rc){
    int i = 0;
    for(auto itRec=rc.m_records.begin(); itRec != rc.m_records.end(); itRec++){
        std::string s; //specie id
        int st; //specie's stoichiometry
        std::tie (s, st) = *itRec;
        switch(i){//case is the position of the ?? in the reactions vector?? TODO
        case 0:
            os << (-1)*st << " of " << s;
            break;
        case 1:
            os << " + " << (-1)*st << " of " << s << " -> " ;
            break;
        case 2:
            os << st << " of " << s;
            break;
        default:
            os << " + " << st << " of " << s;
            break;
        }
        i++;
    }
    if( rc.m_partialPropensity < 0.f )
        os << " (rate " << rc.m_rate << ", Pi has not been computed yet)";
    else
        os << " (rate " << rc.m_rate << ", Pi " << rc.m_partialPropensity << " w respect to " << rc.m_pPWRespectTo << ")";
    return os;
}

void Reaction::computePartialPropensity(std::string wRespectToSp, MOLINT populationOfSp)
{
    std::string s0, s1;
    int st0, st1;
    std::tie (s0, st0) = m_records[0];
    std::tie (s1, st1) = m_records[1];

    if( s0 != wRespectToSp && s1 != wRespectToSp ){
        std::cout << "Reaction: Computation of partial propensity with respect to an unknown specie " << wRespectToSp << " attempted, exiting" << std::endl;
        std::cout << "Reaction: " << (*this) << std::endl;
        exit(EXIT_FAILURE);
    }

    // from here on we are sure that wRespectToSp is one of our reagents
    m_pPWRespectTo = wRespectToSp;

    // if both reagents are "vacuum", then the reaction is of source type, and Pi is the rate
    if( s0 == "" && s1 == "" ){
        m_partialPropensity = m_rate;
        return;
    }

    // if the reagents are the same, we can have either unimolecular or bimolecular reaction
    if( s0 == s1 ){
        // for unimolecular reaction Pi is the rate
        if( (st0 == 0 && st1 == -1) || (st1 == 0 && st0 == -1) ){
            m_partialPropensity = m_rate;
            return;
        }
        // for reactions involving two molecules of the same specie, Pi is computed using formula from RG-SS
        if( st0 == -1 && st1 == -1 ){
            m_partialPropensity = 0.5f*((PROPFLOAT) (populationOfSp - 1))*m_rate;
            return;
        }
    }

    // if the reagents are different, both stoichiometries must be -1: we do NOT handle trinary reactions
    if( s0 != s1 && st0 == -1 && st1 == -1 ){
        m_partialPropensity = ((PROPFLOAT) populationOfSp)*m_rate;
        return;
    }

    // we do not like situations when none of the patterns above are matched
    std::cout << "Reaction: Couldn't find a rule to compute partial propensity with respect to " << wRespectToSp << ", exiting" << std::endl;
    std::cout << "Reaction: " << (*this) << std::endl;
    exit(EXIT_FAILURE);
}

bool Reaction::isValid()
{
    std::string s, s0, s1;
    int st, st0, st1;
    std::tie (s0, st0) = m_records[0];
    std::tie (s1, st1) = m_records[1];

    if( (st0 != -1 && st0 != 0) || (st1 != -1 && st1 != 0) )
        return false;

    if( s0 == "" && s1 == "" && (st0 != 0 || st1 != 0) )
        return false;

    if( ((st0 == 0 && st1 != 0) || (st0 != 0 && st1 == 0)) && ( s0 != s1 ) )
        return false;

    auto itRec = m_records.begin();
    itRec++;
    itRec++;
    for(; itRec != m_records.end(); itRec++){
        std::tie (s, st) = *itRec;
        if(st <= 0)
            return false;
    }

    return true;
}

void Reaction::addSpecie(std::string specie, int stoichiometry){
    m_records.push_back(specieRecord_t(specie, stoichiometry));
}
