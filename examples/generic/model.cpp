#include <map>
#include <string>
#include <iostream>
#include "parameter.h"
#include "model.h"

/* model.cpp for a generic model:
 * reactions() produces empty reaction list for any pair of species
 */

/* Global configuration dictionary */
extern std::map<std::string,Parameter> configDict;

Specie::Specie(std::string id) : modelName("generic-model"), m_id(id)
{
    // If you declared any auxiliary variables in the header, define them here.
    // While doing that, keep in mind that any two Species objects constructed
    // from the same string must behave in exactly the same way.
}

std::list<Reaction> Specie::reactions(Specie other)
{
    std::list<Reaction> allReactions; // all the reactions two species can have

    // Populate the allReactions list with reactions possible between this and
    // the other Specie here. Tips:
    //
    //  - Do not forget to define the behavior for the case when one or both
    //    Species are void species! In case you don't know, void species are
    //    special Species useful for representing source and unary reactions as
    //    binary reactions. By convention, void species have std::string("") for
    //    id and zero stoichiometry in all reactions.
    //
    //  - Reaction objects are constructed from four arguments: id of the first
    //    reactant, its stoichiometry, id of the second reactant, stoichiometry
    //    of the second reactant. Once a Reaction is constructed, you can add
    //    products by calling the addProducts() method which takes id of the
    //    product and its stoichiometry. For example, reaction A + B -> AB can
    //    be written as
    //
    //      Reaction ab("A", 1, "B", 1);
    //      ab.addProduct("AB", 1);
    //
		//  - For convenience of tuning the model's parameters, a global
    //    configuration dictionary configDict is provided. Parameters are
    //    represented as Parameter objects and can be converted into strings,
    //    bools, ints or floats, e.g. like this
    //
    //      float baseRate = configDict['baseRate'].getFloat();
    //
    //    This converts a variable 'baseRate' read from within the 'kinetic
    //    model' section of the parameters.ini file and converts it into float.
    //    Note: for bool parameters use "true" and "false" string values in the
    //    config.
    //
    //  - Once you have defined the reaction, append it to allReactions using its
    //    push_back() method.
    //
    //  - (exhales) Last bit - the function must commute, i.e. for any pair of
    //    Species A and B A.reactions(B) must be equal to B.reactions(A).
    //    Unfortunately, we haven't removed the burden of ensuring that from the
    //    user (you) yet (2017-02-20). If you know a way to do so which does not
    //    sacrifices readability, please contact me at abernats@uvm.edu.
    //
    // You're on your own now. Good luck.

    return allReactions;
}
