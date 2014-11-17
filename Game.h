#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <cmath>
#include <memory>
#include <list>
#include <SFML/Graphics.hpp>
#ifndef M_PI    //Not defined on non-POSIX systems
    #define M_PI 3.1415926535898
#endif // M_PI
#include "Satellite.h"

using namespace std;

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
    return {cos(angle),sin(angle)};
}

inline float magnitude(const sf::Vector2f& v)
{
    return hypot(v.x,v.y);
}

enum TextureName
{
    Junk0,Junk1,Junk2,Sat0,Sat1,Sat2,Sat3,Logo,NewGameBtn,Background
};

class Game
{
    public:
        Game();
        void run();
        void addDebris(const sf::Vector2f& pos,const sf::Vector2f& sat_vel,bool isstation = false);
        const sf::Vector2f& gravitySrc(){ return Earth.getPosition(); }
        float planetRadius(){ return Earth.getRadius(); }
        sf::RenderWindow& getWindow(){ return window; }
        float gravityConst(){ return gravityAccConst; }
        void stationDestroyed();
        sf::Texture& getTexture(TextureName name){ return *textures[name]; } //assuming texture were loaded successfully
    private:
        enum state
        {
            Playing,
            GameOver,
            SplashScreen,
        } gameState;
        void loadTexture(TextureName name,string path);
        void newRandomSatellite();

        float gravityAccConst;// = G * Mass of planet
        sf::RenderWindow window;
        sf::Event winEvent;
        sf::CircleShape Earth;
        sf::Texture earthTexture;
        sf::Sprite bg,logo,newGame;
        sf::Font font;
        sf::Text message;
        sf::Text timerText;
        list<unique_ptr<Orbiter>> sats;
        map<TextureName,unique_ptr<sf::Texture>> textures;
};

#endif // GAME_H
