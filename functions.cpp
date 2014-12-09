#include "functions.h"
#include <cstring>

void append(float val,vector<char> &data)
{
    auto start = data.size();
    data.resize(start + sizeof(val));
    memcpy(&data[start],&val,sizeof(val));
}
void append(const sf::Vector2f& vec,vector<char> &data)
{
    append(vec.x,data);
    append(vec.y,data);
}

size_t extract(size_t i,sf::Vector2f& res,vector<char> &data)
{
    return extract(extract(i,res.x,data),res.y,data);
}
size_t extract(size_t i,float &res,vector<char> &data)
{
    res = *reinterpret_cast<float*>(&data[i]);
    return i+sizeof(float);
}
size_t extract(size_t i,char &c,vector<char> &data)
{
    c = data[i];
    return i+sizeof(char);
}
