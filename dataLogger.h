#ifndef __DATALOGGER_H
#define __DATALOGGER_H

#include <fstream>
#include <string>

#include "totalPopulation.h"

/*
 *  PUBLIC DATA
    none

    PUBLIC METHODS
    DataLogger(TotalPopulation* tp, float timeStep, float totalTime, std::string fileName)
        Constructor for use in simulateTime-type simulations. Opens the file under fileName.
        Method makeRecords() (see below) records the state of the total population *tp at
        the following times:
            0.0, 1.0*timeStep, 2.0*timeStep, ..., n*timestep=totalTime.
        WARNING: totalTime must be exactly divisible by timeStep in floats, otherwise the
        constructor call will cause the program to exit with error. Examples:
            totalTime   timeStep    Divisible in floats?
            10          1           yes
            10          0.1         yes
            10          0.2         yes
            10          0.5         yes
            3           0.3         no
            300         0.3         no
        If timeStep is set to 0, it will record every reaction to occur before totalTime.
        If simulation has to be stopped due to internal reasons (e.g. because there are no
        more valid reactions), records for times after the stop can be requested using the
        makePostsimulationRecords() function.
    DataLogger(TotalPopulation* tp, int recordingPeriod, int totalReactions, std::string fileName)
        Constructor for use in simulateReactions-type simulations. Opens the file under
        fileName. Method makeRecords() (see below) records all every recordingPeriod-th
        reaction with number less or equal to totalReactions.
        WARNING: totalReactions must be exactly divisible by recordingPeriod, otherwise
        the constructor call will cause the program to exit with error.
        0th reaction is always recorded. makePostsimulationRecords() has no effect.
    ~DataLogger()
        Closes the data file.
    void makeHeader(int argc, char** argv)
        Writes a header of the output file. Format:
        line 0: # Model: <modelName>
        line 1: # Parameters: <parameterName0>=<value0> <parameterName1>=<value1> ...
        line 2: # Command: <fullCommandLine>
        where modelName is the name of the model as indicated in the Specie
        object associated with the first population in totalPopulation,
        parameters described by line 1 are the model parameters from parameters.ini, and
        command at line 2 is the full command line which invoked simulation.
    bool makeRecords()
        An expert system which intellectually decides if it is the right time to make
        a record in the data file. The exact behavior depends on which constructor and
        with which parameters was used to make the object (see the constructors'
        descriptions).
        Returns true iff more steps of the simulation are required to make all necessary
        records.
    void makePostsimulationRecords()
        If for some reason making more steps of the simulation is impossible (e.g.
        because there are no more valid reactions), this function can be called to
        append all the necessary records to the output file with the current state
        of the total population. Exact behavior depends on which constructor was used.
 *
 */

class DataLogger
{
public:
    DataLogger(TotalPopulation* tp, float timeStep, float totalTime, std::string fileName); // will cause the program to fail if called with bad arguments
    DataLogger(TotalPopulation* tp, int recordingPeriod, int totalReactions, std::string fileName); // will cause the program to fail if called with bad arguments
    ~DataLogger();
    void makeHeader(int argc, char** argv);
    bool makeRecords();
    void makePostsimulationRecords();

private:
    TotalPopulation* m_tp;
    int m_type;
    float m_timeStep;
    float m_totalTime;
    int m_recPeriod;
    int m_numSteps;
    int m_i;
    float m_prevTime;
    std::string m_prevPop;
    std::ofstream m_file;

    bool checkTimes(float timeStep, float totalTime); // makes the app exit with error as soon as totalTime is not float-divisible by timeStep
    bool checkReacNum(int recordingPeriod, int totalReactions); // makes the app exit with error as soon as totalReactions is not divisible by recordingPeriod
    void memorizePopulations(); // stores a list of entities of class Population from the saved pointer to TotalPopulation into the internal representation string
    void makeRecord(float time);
};

#endif // __DATALOGGER_H
