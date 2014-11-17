#ifndef SATELLITE_H
#define SATELLITE_H
#include <memory>
#include <SFML/Graphics.hpp>

class Game;

class Orbiter
{
    public:
        enum OrbiterType { satellite,debris,space_station } type;
        Orbiter(Game& game,OrbiterType t,const sf::Vector2f& pos = {0,0},const sf::Vector2f& vel = {0,0},float ang_vel = 0);
        virtual ~Orbiter() = default;
        virtual bool checkCollision(Orbiter& b) = 0;
        virtual void update(float dt);
        virtual void render(sf::RenderTarget& target) = 0;
        bool isDestroyed(){ return destroyed; }
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
    virtual ~Satellite(){};
    void update(float dt);
    void render(sf::RenderTarget& target);
    bool checkCollision(Orbiter& b);
    friend class SpaceStation;
private:
    float targetOrbit;
    float normal_velocity = 60;
    sf::Sprite satBody;
};

class Debris : public Orbiter
{
public:
    Debris(Game& game,const sf::Vector2f& pos,const sf::Vector2f& vel,float ang_vel);
    virtual ~Debris() = default;
    void render(sf::RenderTarget& target);
    void update(float dt);
    bool checkCollision(Orbiter& b);
    friend class Satellite;
    friend class SpaceStation;
private:
    sf::Sprite debrisSpr;
    float age;
    const float max_age = 10;
};

class SpaceStation : public Orbiter
{
public:
    SpaceStation(Game& game,float radius);
    virtual ~SpaceStation() = default;
    void render(sf::RenderTarget& target);
    void update(float dt);
    void update(float dt,bool input);
    bool checkCollision(Orbiter& b);
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
