#ifndef GAME_H
#define GAME_H
#include <memory>
#include <list>
#include <fstream>
#include "functions.h"
#include "Satellite.h"

using namespace std;


enum TextureName
{
    Junk0,Junk1,Junk2,Sat0,Sat1,Sat2,Sat3,Logo,NewGameBtn,LoadGameBtn,SaveGameBtn,Background
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
            Pause,
            SplashScreen,
        } gameState;
        void loadTexture(TextureName name,string path);
        void newRandomSatellite();
        void loadGameFromFile(string file_path);
        void saveGameToFile(string file_path);

        float gravityAccConst;// = G * Mass of planet
        SpaceStation* station;
        sf::RenderWindow window;
        sf::Event winEvent;
        sf::CircleShape Earth;
        sf::Texture earthTexture;
        sf::Sprite bg,logo,newGame,loadGame,saveGame;
        sf::Font font;
        sf::Text message,timerText;
        sf::Clock timer;
        float survivedTime = 0;
        list<unique_ptr<Orbiter>> sats;
        map<TextureName,unique_ptr<sf::Texture>> textures;
};

#endif // GAME_H
