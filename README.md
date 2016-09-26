Expandable Partial Propensity Direct Method in C++
==================================================
EPDM is an algorithm which allows efficient stochastic simulations of 
systems composed of multiple interacting agents (e.g. molecules). 
It works especially well if the number of molecular species (agent 
types) which the system can potentially generate is much greater than 
the number of species that are typically present in the system at any 
given time. Such systems include heteropolymerization, genetic 
regulatory networks, metabolic networks etc. Only interactions 
involving two or less molecules/agents are supported.

The complexity of executing one reaction is linear in the number of 
species currently present in the system. The algorithm is described 
in detail at http://arxiv.org/abs/1609.06403.

This repository contains a C++11 implementation of EPDM.

Interface
---------
To define a system, the user must design two representations of 
molecular species/agent types - a string identifier and a special 
class `Specie` (see examples/generic/model[.h|.cpp]).

String representation is the default format for storing and looking 
up species. It must uniquely identify a specie while being 
relatively compact. This is because the number of records stored by 
the algorithm in this format grows quadratically with the number of 
species present in the system `N`. It cannot contain spaces.

Class `Specie` is used when the algorithm determines which reactions 
are possible between any given pair of species. For any given pair of 
species `S1` and `S2` calls `S1.reactions(S2)` and `S2.reactions(S1)` 
must return identical lists, each describing all reactions possible 
between `S1` and `S2`. In addition to the `reactions()` method, the 
class must have a constructor from the string representation which 
saves the copy of the representation into a public member variable 
`m_id`.

Only `N` instances of the class `Specie` are kept in memory. This 
makes it suitable for storing rich, memory intensive (e.g. 
graph-based) representations of species. The intention is to utilize 
this additonal freedom to make the execution of `reactions()` faster. 
Note, however, that although class `Specie` may represent the species 
differently than the string identifier, the behavior of any two 
`Specie` instances constructed from identical strings must be 
identical.

Overall, the workflow is as follows:
1. Design a unique string representations for the species/agent kinds 
that can be present in your system.
2. Make a working folder by copying examples/generic.
3. Define class `Specie` at model.cpp in accordance to the detailed 
description in comments to model.cpp. You can use the configuration 
dictionary provided in the code that is automatically loaded from 
a .ini configuration file to control the parameters of the model.
4. Open Makefile and make sure that `epdmDir` variable contains the 
path to EPDM base folder. If you added any .cpp files, append their 
names with .cpp changed to .o to `modelObjects` variable. You can 
also change the name of the excutable by modifying `epdmExecName` 
variable.
5. Run
```bash
$ make
```
at your working folder. Test the executable, e.g.
```bash
$ ./epdm 
Unrecognized arguments
Usage:
  epdm simulateTime <totalTime> <timeBetweenRecords> <outputFileName> [<options>]
  epdm simulateReactions <numberOfReactions> <recordingPeriod> <outputFileName> [<options>]
Possible options:
  -c <parametersFileName> -i <initialPopulationFileName> -r <randomSeed>
totalTime=0 causes the program to run the simulation until it runs out of possible reactions or indefinitely
timeBetweenRecords=0 causes the program to record the population after every reaction
numberOfReactions must be a multiple of recordingPeriod
parametersFileName is a path to the configuration .ini file. Defaults to parameters.ini
initialPopulationFileName is a path to the file holding initial configurations. Defaults to populations.txt
```
6.Define the initial state of the system in an initial populations 
file (populations.txt by default). Format:
```
<id_of_specie_0> <molecular_count_of_specie_0>
<id_of_specie_1> <molecular_count_of_specie_1>
...
<id_of_specie_NS> <molecular_count_of_specie_NS>
```
where `NS` is the number of species that are initially present in the 
system. If the system is designed to contain no molecules initially 
and instead produce them over time with source reactions, just create 
an empty file. Default file name is populations.txt.
7. (optional) If you used the configuration dictionary, create and populate 
the .ini configuration file (parameters.ini will be loaded by default).
8. You can now run simulations! For example,
```bash
$ epdm simulateTime 10. 0.1 trajectory.txt -r 9001 -i initialPopulations.txt
```
will produce a trajectory of your system with the initial system 
state described at initialPopulations.txt, with 9001 for a random 
seed, simulating 10 seconds of the system's history and making 
records every 100 milliseconds.

For full description of the command line options run the executable 
without arguments (see above at step 5).

Credits
-------
Idea was proposed by @gelisa, who also did a lot of testing and 
benchmarks; algorithm design and implementation is by @abernatskiy.
