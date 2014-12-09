#ifndef SATELLITE_H
#define SATELLITE_H
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "functions.h"

class Game;

class Orbiter
{
    public:
        enum OrbiterType { satellite=0,debris,space_station } type;
        Orbiter(Game& game,OrbiterType t,const sf::Vector2f& pos = {0,0},const sf::Vector2f& vel = {0,0},float ang_vel = 0,float rot = 0);
        virtual ~Orbiter() = default;
        virtual bool checkCollision(Orbiter& b) = 0;
        virtual void update(float dt);
        virtual void render(sf::RenderTarget& target) = 0;
        virtual std::vector<char> serialize();
        bool isDestroyed(){ return destroyed; }
        static std::unique_ptr<Orbiter> deserialize(std::vector<char> &data,Game& g);
        static size_t size(){ return sizeof(position)+sizeof(velocity)+sizeof(angularVelocity)+sizeof(rotation)+1;};//1 is for 'type'
    protected:
        sf::Vector2f position;
        sf::Vector2f velocity;
        float angularVelocity;
        float rotation;
        bool destroyed;
        Game& game;
};


class Satellite : public Orbiter
{
public:
    Satellite(Game& _game,float orbitRadius,const sf::Vector2f& pos = {0,0});
    Satellite(Game& _game,const sf::Vector2f& pos,const sf::Vector2f& vel,float avel,float rot,float orbit,float nvel,char t);
    virtual ~Satellite(){};
    void update(float dt);
    void render(sf::RenderTarget& target);
    bool checkCollision(Orbiter& b);
    std::vector<char> serialize();
    static size_t size(){ return Orbiter::size()+sizeof(targetOrbit)+sizeof(normal_velocity)+sizeof(sat_type);}
    friend class SpaceStation;
private:
    float targetOrbit;
    float normal_velocity = 60;
    char sat_type;
    sf::Sprite satBody;
};

class Debris : public Orbiter
{
public:
    Debris(Game& _game,const sf::Vector2f& pos,const sf::Vector2f& vel,float ang_vel);
    Debris(Game& _game,const sf::Vector2f& pos,const sf::Vector2f& vel,float avel,float rot,float _age,char t);
    virtual ~Debris() = default;
    void render(sf::RenderTarget& target);
    void update(float dt);
    bool checkCollision(Orbiter& b);
    std::vector<char> serialize();
    size_t size(){ return Orbiter::size()+sizeof(age)+sizeof(debris_type);}
    friend class Satellite;
    friend class SpaceStation;
private:
    sf::Sprite debrisSpr;
    float age;
    char debris_type;
    const float max_age = 10;
};

class SpaceStation : public Orbiter
{
public:
    SpaceStation(Game& game,float orbit);
    SpaceStation(Game& _game,const sf::Vector2f& pos,const sf::Vector2f& vel,float avel,float rot,float orbit,float nvel,char s);
    virtual ~SpaceStation() = default;
    void render(sf::RenderTarget& target);
    void update(float dt);
    void update(float dt,bool input);
    bool checkCollision(Orbiter& b);
    std::vector<char> serialize();
    static size_t size(){ return Orbiter::size()+sizeof(orbit_radius)+sizeof(normal_vel)+sizeof(sense);}
private:
    void moveStation(char dir,float dt);
    sf::Sprite station;
    sf::Texture stationImg;
    sf::CircleShape trajectory;
    float orbit_radius;
    float normal_vel;
    char sense; //direction of rotation i.e. clockwise or anti-clockwise
    const float normal_acc = 10;
    const float normal_vel_max = 100;
    const float normal_vel_min = 60;
    const float max_altitude = 250;
};
#endif // SATELLITE_H
