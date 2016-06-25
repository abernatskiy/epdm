#include <iostream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <cmath>

#include "dataLogger.h"

extern std::map<std::string,Parameter> configDict;

DataLogger::DataLogger(TotalPopulation* tp, float timeStep, float totalTime, std::string fileName){
    if(!checkTimes(timeStep, totalTime)){
        std::cout << "ERROR: Total time must be divisible by time step\n";
        exit(EXIT_FAILURE); // TODO: throw exception instead
    }
    m_tp = tp;
    m_type = 0;
    m_timeStep = timeStep;
    m_totalTime = totalTime;
    m_recPeriod = 1;
    m_numSteps = (int) (totalTime/timeStep); // checkTimes() must guarantee, among other stuff, that we do not underestimate the number of iterations here
    m_i = 0;
    m_prevTime = 0.0;

    memorizePopulations();
    m_file.open(fileName);
}

DataLogger::DataLogger(TotalPopulation* tp, int recordingPeriod, int totalReactions, std::string fileName){
    if(!checkReacNum(recordingPeriod, totalReactions)){
        std::cout << "ERROR: Total numer of reactions must be divisible by the recording period\n";
        exit(EXIT_FAILURE); // TODO: throw exception instead
    }
    m_tp = tp;
    m_type = 1;
    m_timeStep = -1.f;
    m_totalTime = -1.f;
    m_recPeriod = recordingPeriod;
    m_numSteps = totalReactions;
    m_i = 0;
    m_prevTime = -1.f;

    memorizePopulations();
    m_file.open(fileName);
}

DataLogger::~DataLogger(){
    m_file.close();
}

void DataLogger::makeHeader(int argc, char** argv){
    std::string modelName = (((m_tp->m_listOfPopulations).begin())->m_specie).modelName;
    m_file << "# Model: " << modelName << std::endl;
    m_file << "# Parameters:";
    for(auto it = configDict.begin(); it != configDict.end(); it++)
        m_file << " " << it->first << "=" << (it->second).getString();
    m_file << std::endl;
    m_file << "# Command:";
    for(int i=0; i<argc; i++)
        m_file << " " << std::string(argv[i]);
    m_file << std::endl;
    return;
}

bool DataLogger::makeRecords(){
    float curtime = m_tp->m_t;
    if(m_type == 0){ // simulateTime
        if(m_totalTime == 0.0){
            std::cout << "Total simulation time is 0 - output not implemented" << std::endl;
            return false;
        }
        else{
            if(m_timeStep == 0.0){ // Timestep is 0 - we're recording state at every reaction
                if(curtime > m_totalTime)
                    return false;
                else{
                    memorizePopulations();
                    makeRecord(curtime);
                    return curtime < m_totalTime;
                }
            }
            else{ // We're making time-periodic records
                int numRecords = 1;
                while( m_timeStep*((float) m_i) >= m_prevTime && m_timeStep*((float) m_i) < curtime ){
                    if(numRecords > 1)
                        std::cout << "writing to the file for the " << numRecords << "th time after one reaction\n";
                    makeRecord(m_timeStep*((float) m_i));
                    numRecords += 1;
                    m_i++;
                    if(m_i > m_numSteps)
                        return false;
                }
                memorizePopulations();
                m_prevTime = curtime;
                return true;
            }
        }
    }
    else if(m_type == 1){ // simulateReactions
        if( m_i % m_recPeriod == 0 ){
            memorizePopulations();
            makeRecord(curtime);
        }
        m_i++;
        return m_i <= m_numSteps;
    }
    std::cout << "ERROR: Undefined simulation type\n";
    return false;
}

void DataLogger::makePostsimulationRecords(){
    while(m_type == 0 && m_i <= m_numSteps){
        makeRecord(m_timeStep*((float) m_i));
        m_i++;
    }
}

bool DataLogger::checkTimes(float timeStep, float totalTime){
    float ratio = totalTime/timeStep;
    return floorf(ratio) == ratio;
}

bool DataLogger::checkReacNum(int recordingPeriod, int totalReactions){
    return totalReactions % recordingPeriod == 0;
}

void DataLogger::memorizePopulations(){
    std::stringstream ss;
    for(auto pops_it = m_tp->m_listOfPopulations.begin(); pops_it != m_tp->m_listOfPopulations.end(); pops_it++)
    {
        if( pops_it->m_specie.m_id != "" )
            ss << pops_it->m_specie.m_id << " " << pops_it->m_n << ",";
    }
    m_prevPop = ss.str();
}

void DataLogger::makeRecord(float time){
    std::cout << "writing to a file at time " << time << std::endl;
    m_file << time << "," << m_prevPop << std::endl;
}
