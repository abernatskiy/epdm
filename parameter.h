#ifndef __PARAMETER_H
#define __PARAMETER_H

#include <string>

//do not call with Constants
class Parameter
{
public:
    /* Generic interface */
    Parameter();
    ~Parameter();
    std::string getType();
    std::string getString(); //LISA// TODO from .cpp

    /* Type-specific interfaces */
    //bool
    Parameter(bool value);
    bool getBool();
    //int
    Parameter(int value);
    int getInt();
    //float
    Parameter(float value);
    float getFloat();

    /* Variables */
    short m_type;
    void* m_valuePtr;
private:
    template<typename T>
    Parameter(T value);
};

#endif // __PARAMETER_H
