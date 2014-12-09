#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include <vector>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#ifndef M_PI    //Not defined on non-POSIX systems
    #define M_PI 3.1415926535898
#endif // M_PI

using namespace std;

#ifdef __MINGW32__
#include <sstream>
template<class T>
string to_string(T val)
{
    stringstream ss;
    ss << val;
    return ss.str();
}
#endif // __MINGW32__

inline float TO_DEG(float rad)
{
    return rad*180.0f/M_PI;
}

inline float TO_RAD(float deg)
{
    return deg*M_PI/180.0f;
}

inline sf::Vector2f cos_sin(float angle)
{
    return {float(cos(angle)),float(sin(angle))};
}

inline float magnitude(const sf::Vector2f& v)
{
    return hypot(v.x,v.y);
}

void append(float val,vector<char> &data);
void append(const sf::Vector2f& vec,vector<char> &data);
size_t extract(size_t i,sf::Vector2f& res,vector<char> &data);
size_t extract(size_t i,float &res,vector<char> &data);
size_t extract(size_t i,char &c,vector<char> &data);

#endif // FUNCTIONS_H_INCLUDED
