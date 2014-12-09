#include "Satellite.h"
#include "Game.h"
#include "obbcollision.h"

Orbiter::Orbiter(Game& _game,OrbiterType t,const sf::Vector2f& pos,const sf::Vector2f& vel,float ang_vel,float rot) :
    type(t),
    position(pos),
    velocity(vel),
    angularVelocity(ang_vel),
    rotation(rot),
    destroyed(false),
    game(_game)
{}

void Orbiter::update(float dt)
{
    sf::Vector2f r = position-game.gravitySrc();
    float sin_theta = r.y/magnitude(r),cos_theta = r.x/magnitude(r);
    sf::Vector2f gravityAccel(-game.gravityConst()*cos_theta/(r.x*r.x+r.y*r.y),
                              -game.gravityConst()*sin_theta/(r.x*r.x+r.y*r.y));
    velocity += gravityAccel*dt;
    position += velocity*dt;
    rotation += angularVelocity*dt;
}

vector<char> Orbiter::serialize()
{
    vector<char> data;
    data.reserve(25);
    data.push_back(static_cast<char>(type));
    append(position,data);
    append(velocity,data);
    append(angularVelocity,data);
    append(rotation,data);
    return data;
}
std::unique_ptr<Orbiter> Orbiter::deserialize(std::vector<char> &data,Game& g)
{
    unique_ptr<Orbiter> orbiter;
    char type;
    size_t i = 0;
    i = extract(i,type,data);
    if(type < 0 || type > 2 || data.size() < Orbiter::size())
        return {nullptr};
    sf::Vector2f pos,vel;
    float avel,rot;
    i = extract(extract(extract(extract(i,pos,data),vel,data),avel,data),rot,data);
    switch(static_cast<OrbiterType>(type))
    {
        case satellite:
        {
            float o,nv;
            char t;
            extract(extract(extract(i,o,data),nv,data),t,data);
            orbiter.reset(static_cast<Orbiter*>(new Satellite(g,pos,vel,avel,rot,o,nv,t)));
            break;
        }
        case debris:
        {
            float a;
            char t;
            extract(extract(i,a,data),t,data);
            orbiter.reset(static_cast<Orbiter*>(new Debris(g,pos,vel,avel,rot,a,t)));
            break;
        }
        case space_station:
        {
            float o,nv;
            char s;
            extract(extract(extract(i,o,data),nv,data),s,data);
            orbiter.reset(static_cast<Orbiter*>(new SpaceStation(g,pos,vel,avel,rot,o,nv,s)));
            break;
        }
    }
    return orbiter;
}


//Implementations of functions of Satellite class
Satellite::Satellite(Game& _game,float orbitRadius,const sf::Vector2f& pos):
    Orbiter(_game,satellite,pos),
    targetOrbit(orbitRadius),
    sat_type(rand()%4)
{
    satBody.setTexture(game.getTexture(static_cast<TextureName>(Sat0+sat_type)));
    satBody.setOrigin(satBody.getLocalBounds().width/2,satBody.getLocalBounds().height);
    satBody.setPosition(position);
    sf::Vector2f normal = position - game.gravitySrc();
    rotation = atan2(-normal.x,normal.y);
}

Satellite::Satellite(Game& _game,const sf::Vector2f& pos,const sf::Vector2f& vel,float avel,float rot,float orbit,float nvel,char t) :
    Orbiter(_game,satellite,pos,vel,avel,rot),
    targetOrbit(orbit),
    normal_velocity(nvel),
    sat_type(t)
{
    satBody.setTexture(game.getTexture(static_cast<TextureName>(Sat0+sat_type)));
    satBody.setOrigin(satBody.getLocalBounds().width/2,satBody.getLocalBounds().height);
    satBody.setPosition(position);
}

std::vector<char> Satellite::serialize()
{
    std::vector<char> data{Orbiter::serialize()};
    append(targetOrbit,data);
    append(normal_velocity,data);
    data.push_back(sat_type);
    return data;
}

void Satellite::update(float dt)
{
    if(abs(normal_velocity) > 0.01)
    {
        sf::Vector2f normal = position - game.gravitySrc();
        float currentRadius = magnitude(normal);
        normal /= currentRadius; //normalize
        position += normal*normal_velocity*dt;
        if(currentRadius >= targetOrbit)
        {
            //circular orbital velocity , v_o = sqrt(GM/r)
            float v_o = (1-2*(rand()%2))*sqrt(game.gravityConst()/currentRadius);
            velocity = {v_o*-normal.y,v_o*normal.x};
            normal_velocity = 0;
            angularVelocity = v_o/currentRadius;
        }
    }
    else    Orbiter::update(dt);
}
void Satellite::render(sf::RenderTarget &target)
{
    satBody.setPosition(position);
    satBody.setRotation(TO_DEG(rotation));
    target.draw(satBody);
    if(satBody.getColor() == sf::Color::Red)
        satBody.setColor(sf::Color::White);
}
bool Satellite::checkCollision(Orbiter &b)
{
    bool result = false;
    switch(b.type)
    {
    case satellite:
        {
            auto satelliteB = static_cast<Satellite&>(b);
            if(satBody.getGlobalBounds().intersects(satelliteB.satBody.getGlobalBounds()) &&
               iscolliding(satBody.getPosition(),sf::Vector2f{satBody.getLocalBounds().width,satBody.getLocalBounds().height},rotation,
                           satelliteB.satBody.getPosition(),sf::Vector2f{satelliteB.satBody.getLocalBounds().width,satelliteB.satBody.getLocalBounds().height},satelliteB.rotation))
            {
                auto r = satelliteB.position-position;
                auto r_mag = magnitude(r);
                result = true;
                destroyed = true;
                satelliteB.destroyed = true;
                game.addDebris(position,velocity+r*normal_velocity/r_mag);
                game.addDebris(satelliteB.position,satelliteB.velocity-r*satelliteB.normal_velocity/r_mag);
            }
        }
        break;
    case debris:
        {
            auto junk = static_cast<Debris&>(b);
            if(satBody.getGlobalBounds().intersects(junk.debrisSpr.getGlobalBounds()) &&
               iscolliding(satBody.getPosition(),sf::Vector2f{satBody.getLocalBounds().width,satBody.getLocalBounds().height},rotation,
                           junk.debrisSpr.getPosition(),sf::Vector2f{junk.debrisSpr.getLocalBounds().width,junk.debrisSpr.getLocalBounds().height},junk.rotation))
            {
                result = true;
                destroyed = true;
                junk.destroyed = true;
                auto r = junk.position-position;
                auto r_mag = magnitude(r);
                game.addDebris(position,velocity+r*normal_velocity/r_mag);
            }
        }
        break;
    default:
        break;
    }
    return result;
}

Debris::Debris(Game& _game,const sf::Vector2f& pos,const sf::Vector2f& vel,float ang_vel) :
    Debris(_game,pos,vel,ang_vel,0.0,0.0,rand()%3)
{}

Debris::Debris(Game& _game,const sf::Vector2f& pos,const sf::Vector2f& vel,float avel,float rot,float _age,char t) :
    Orbiter(_game,debris,pos,vel,avel,rot),
    age(_age),
    debris_type(t)
{
    debrisSpr.setTexture(game.getTexture(static_cast<TextureName>(Junk0+debris_type)));
    debrisSpr.setOrigin(debrisSpr.getLocalBounds().width/2,debrisSpr.getLocalBounds().height);
    debrisSpr.setPosition(position);
}

std::vector<char> Debris::serialize()
{
    std::vector<char> data{Orbiter::serialize()};
    append(age,data);
    data.push_back(debris_type);
    return data;
}
void Debris::update(float dt)
{
    Orbiter::update(dt);
    age += dt;
    auto r = magnitude(position-game.gravitySrc());
    if(r < game.planetRadius() || r > 900 || age > max_age)
        destroyed = true;
}
void Debris::render(sf::RenderTarget& target)
{
    debrisSpr.setPosition(position);
    debrisSpr.setRotation(rotation);
    target.draw(debrisSpr);
}
bool Debris::checkCollision(Orbiter& b)
{
    if(b.type == satellite)
        return b.checkCollision(*this);
    return false;
}

SpaceStation::SpaceStation(Game& _game,float orbit) :
    SpaceStation(_game,_game.gravitySrc()+sf::Vector2f{0,orbit},{0,0},0,0,orbit,0,1-2*(rand()%2))
{
    sf::Vector2f normal = {0,1};
    float v_o = sense*sqrt(game.gravityConst()/orbit);
    velocity = {v_o*-normal.y,v_o*normal.x};
    angularVelocity = v_o/orbit;
}
SpaceStation::SpaceStation(Game& _game,const sf::Vector2f& pos,const sf::Vector2f& vel,float avel,float rot,float orbit,float nvel,char s) :
    Orbiter(_game,space_station,pos,vel,avel,rot),
    trajectory(orbit,50),
    orbit_radius(orbit-1),
    normal_vel(nvel),
    sense(s)
{
    stationImg.loadFromFile("assets/station.png");
    station.setTexture(stationImg);
    station.setOrigin(station.getLocalBounds().width/2,station.getLocalBounds().height/2);
    station.setPosition(position);
    trajectory.setOrigin(orbit,orbit);
    trajectory.setFillColor(sf::Color::Transparent);
    trajectory.setOutlineColor(sf::Color(0x9e,0xea,0xfe));
    trajectory.setOutlineThickness(1);
    trajectory.setPosition(game.gravitySrc());
}
std::vector<char> SpaceStation::serialize()
{
    std::vector<char> data{Orbiter::serialize()};
    append(orbit_radius,data);
    append(normal_vel,data);
    data.push_back(sense);
    return data;
}

void SpaceStation::update(float dt)
{
    update(dt,true);
}
void SpaceStation::update(float dt,bool input)
{
    Orbiter::update(dt);
    if(input && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        moveStation(-1,dt);
    else if(input && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        moveStation(1,dt);
    else
        normal_vel = 0;
}
void SpaceStation::moveStation(char dir,float dt)
{
    normal_vel += dir*normal_acc*dt;
    normal_vel = dir*std::min(dir*normal_vel,normal_vel_max);
    normal_vel = dir*std::max(dir*normal_vel,normal_vel_min);
    float dx = normal_vel*dt;
    if(orbit_radius+dx <= max_altitude && orbit_radius+dx > game.planetRadius()+station.getLocalBounds().height/2+10)
    {
        sf::Vector2f normal = position-game.gravitySrc();
        normal /= magnitude(normal);
        trajectory.setRadius(orbit_radius);
        trajectory.setOrigin(orbit_radius,orbit_radius);
        position += normal*dx;
        orbit_radius = magnitude(position-game.gravitySrc());
        float v_o = sense*sqrt(game.gravityConst()/orbit_radius);
        velocity = {v_o*-normal.y,v_o*normal.x};
        rotation = atan2(-normal.x,normal.y);
        angularVelocity = v_o/orbit_radius;
    }
}
void SpaceStation::render(sf::RenderTarget& target)
{
    if(position != station.getPosition())
    {
        station.setPosition(position);
        station.setRotation(TO_DEG(rotation));
    }
    target.draw(trajectory);
    target.draw(station);
}

bool SpaceStation::checkCollision(Orbiter &b)
{
    bool result = false;
    switch(b.type)
    {
    case satellite:
        {
            auto satelliteB = static_cast<Satellite&>(b);
            if(station.getGlobalBounds().intersects(satelliteB.satBody.getGlobalBounds()) &&
               iscolliding(station.getPosition(),sf::Vector2f{station.getLocalBounds().width,station.getLocalBounds().height},rotation,
                           satelliteB.satBody.getPosition(),sf::Vector2f{satelliteB.satBody.getLocalBounds().width,satelliteB.satBody.getLocalBounds().height},satelliteB.rotation))
            {
                auto r = satelliteB.position-position;
                auto r_mag = magnitude(r);
                result = true;
                destroyed = true;
                satelliteB.destroyed = true;
                game.addDebris(position,velocity,true);
                game.addDebris(satelliteB.position,satelliteB.velocity-r*satelliteB.normal_velocity/r_mag);
            }
        }
        break;
    case debris:
        {
            auto junk = static_cast<Debris&>(b);
            if(station.getGlobalBounds().intersects(junk.debrisSpr.getGlobalBounds()) &&
               iscolliding(station.getPosition(),sf::Vector2f{station.getLocalBounds().width,station.getLocalBounds().height},rotation,
                           junk.debrisSpr.getPosition(),sf::Vector2f{junk.debrisSpr.getLocalBounds().width,junk.debrisSpr.getLocalBounds().height},junk.rotation))
            {
                result = true;
                destroyed = true;
                junk.destroyed = true;
                game.addDebris(position,velocity,true);
            }
        }
        break;
    default:
        break;
    }
    if(result == true)
        game.stationDestroyed();
    return result;
}
