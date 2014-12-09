#include "Game.h"
#include <cstring>

#define SAVE_FILE_PATH "gravity.save"

Game::Game() :
    gameState(SplashScreen),
    gravityAccConst(650000),
    window(sf::VideoMode(800,600),"Gravity Test"),
    Earth(60)
{
    loadTexture(Junk0,"assets/junk0.png");
    loadTexture(Junk1,"assets/junk1.png");
    loadTexture(Junk2,"assets/junk2.png");
    loadTexture(Sat0,"assets/satellite0.png");
    loadTexture(Sat1,"assets/satellite1.png");
    loadTexture(Sat2,"assets/satellite2.png");
    loadTexture(Sat3,"assets/satellite3.png");
    loadTexture(Logo,"assets/gravity.png");
    loadTexture(NewGameBtn,"assets/newgame.png");
    loadTexture(Background,"assets/bg.png");
    loadTexture(LoadGameBtn,"assets/loadgame.png");
    loadTexture(SaveGameBtn,"assets/savegame.png");
    if(!earthTexture.loadFromFile("assets/earth.png"))
        std::cerr << "Unable to load asset." << std::endl;
    srand(time(nullptr));
    Earth.setOrigin(Earth.getRadius(),Earth.getRadius());
    Earth.setPosition(sf::Vector2f(window.getSize()/2u));
    Earth.setTexture(&earthTexture);
    bg.setTexture(getTexture(Background));
    logo.setTexture(getTexture(Logo));
    logo.setOrigin(logo.getLocalBounds().width/2,0);
    logo.setPosition(sf::Vector2f(window.getSize().x/2,10));
    newGame.setTexture(getTexture(NewGameBtn));
    newGame.setPosition(sf::Vector2f(30,window.getSize().y-30-newGame.getGlobalBounds().height));
    loadGame.setTexture(getTexture(LoadGameBtn));
    loadGame.setPosition(sf::Vector2f(window.getSize().x-30-loadGame.getGlobalBounds().width,window.getSize().y-30-newGame.getGlobalBounds().height));
    saveGame.setTexture(getTexture(SaveGameBtn));
    saveGame.setOrigin(saveGame.getLocalBounds().width/2,saveGame.getLocalBounds().height);
    saveGame.setPosition(sf::Vector2f(window.getSize().x/2,window.getSize().y-30));
    font.loadFromFile("assets/Ubuntu-C.ttf");
    message.setFont(font);
    timerText.setString("00.00");
    timerText.setFont(font);
    timerText.setPosition(window.getSize().x/2-timerText.getLocalBounds().width/2,15);
    message.setString("Don't get hit by a satellite or junk , use UP or DOWN arrow keys to change orbit.");
    message.setCharacterSize(20);
    message.setPosition(window.getSize().x/2-message.getLocalBounds().width/2,logo.getPosition().y+logo.getLocalBounds().height+10);
}
void Game::newRandomSatellite()
{
    sats.push_back(unique_ptr<Orbiter>(new Satellite(*this,rand()%150+100,
                    Earth.getPosition()-(Earth.getRadius()-20)*cos_sin((rand()%6283)/100.f))));
}

void Game::stationDestroyed()
{
    gameState = GameOver;
    newGame.setColor(sf::Color::White);
    message.setString("You survived for " + timerText.getString() + " seconds.");
    message.setPosition(window.getSize().x/2-message.getLocalBounds().width/2,logo.getPosition().y+logo.getLocalBounds().height+10);
}
void Game::run()
{
    float newSatTimer = 0;
    const float satSpawnRateInv = 1; //seconds per new satellite
    sats.push_back(unique_ptr<Orbiter>(new SpaceStation(*this,150)));
    SpaceStation* station = static_cast<SpaceStation*>(sats.back().get());
    while(window.isOpen())
    {
        while(window.pollEvent(winEvent))
        {
            if(winEvent.type == sf::Event::Closed)
                window.close();
            else if(gameState != Playing && winEvent.type == sf::Event::MouseButtonReleased)
            {
                sf::Vector2f mouse{float(winEvent.mouseButton.x),float(winEvent.mouseButton.y)};
                if(newGame.getGlobalBounds().contains(mouse))
                {
                    if(gameState == GameOver)
                    {
                        sats.clear();
                        sats.push_back(unique_ptr<Orbiter>(new SpaceStation(*this,150)));
                        station = static_cast<SpaceStation*>(sats.back().get());
                    }
                    gameState = Playing;
                    survivedTime = 0;
                }
                else if(loadGame.getGlobalBounds().contains(mouse))
                    loadGameFromFile(SAVE_FILE_PATH);
                else if(saveGame.getGlobalBounds().contains(mouse))
                    saveGameToFile(SAVE_FILE_PATH);
            }
            else if(gameState != Playing && winEvent.type == sf::Event::MouseMoved)
            {
                sf::Vector2f mouse{float(winEvent.mouseMove.x),float(winEvent.mouseMove.y)};
                if(newGame.getGlobalBounds().contains(mouse))
                    newGame.setColor(sf::Color(0x9e,0xea,0xfe));
                else if(newGame.getColor() != sf::Color::White)
                        newGame.setColor(sf::Color::White);
                if(loadGame.getGlobalBounds().contains(mouse))
                    loadGame.setColor(sf::Color(0x9e,0xea,0xfe));
                else if(loadGame.getColor() != sf::Color::White)
                    loadGame.setColor(sf::Color::White);
                if(gameState == Pause && saveGame.getGlobalBounds().contains(mouse))
                    saveGame.setColor(sf::Color(0x9e,0xea,0xfe));
                else if(saveGame.getColor() != sf::Color::White)
                    saveGame.setColor(sf::Color::White);
            }
            else if(winEvent.type == sf::Event::KeyReleased && winEvent.key.code == sf::Keyboard::Escape)
            {
                if(gameState == Playing)
                {
                    gameState = Pause;
                    message.setString("Game Paused. Press Escape to continue");
                    message.setPosition(window.getSize().x/2-message.getLocalBounds().width/2,message.getPosition().y);
                }
                else if(gameState == Pause)
                    gameState = Playing;
            }
        }
        window.clear(sf::Color::Black);
        window.draw(bg);
        window.draw(Earth);
        float dt = timer.restart().asSeconds();
        for(auto i = sats.begin(); i != sats.end(); ++i)
        {
            (*i)->render(window);
            if(gameState == Playing || gameState == GameOver)
            {
                if((*i)->isDestroyed())
                {
                    i = sats.erase(i);
                    continue;
                }
                (*i)->update(dt);
                for(auto j = next(i); j != sats.end(); )
                {
                    if((*i)->checkCollision(**j) )
                    {
                        sats.erase(j);
                        i = prev(sats.erase(i));
                        break;
                    }
                    else
                        ++j;
                }
            }
        }
        if(gameState == Playing)
        {
            if(newSatTimer > satSpawnRateInv)
            {
                newSatTimer = 0;
                newRandomSatellite();
            }
            newSatTimer += dt;
            survivedTime += dt;
            string time = to_string(survivedTime);
            timerText.setString(time.substr(0,time.find('.')+2));
            timerText.setPosition(window.getSize().x/2-timerText.getLocalBounds().width/2,20);
            window.draw(timerText);
        }
        else
        {
            if(gameState == SplashScreen)
            {
                station->update(dt,false);
                station->render(window);
            }
            if(gameState == Pause)
                window.draw(saveGame);
            window.draw(logo);
            window.draw(message);
            window.draw(newGame);
            window.draw(loadGame);
        }
        window.display();
    }
}
void Game::loadGameFromFile(string file_path)
{
    ifstream file(file_path,ios::in | ios::binary);
    vector<char> data(sizeof(float));
    file.read(&data[0],sizeof(float));
    extract(0,survivedTime,data);
    char dat_size;
    data.reserve(30);
    sats.clear();
    while(file.get(dat_size))
    {
        data.resize(max(dat_size,char(data.size())));
        if(!file.read(&data[0],dat_size))
        {
            message.setString("ERROR :: Unable to read from save file.");
            message.setPosition(window.getSize().x/2-message.getLocalBounds().width/2,message.getPosition().y);
            gameState = SplashScreen;
            return;
        }
        sats.push_back(move(Orbiter::deserialize(data,*this)));
    }
    gameState = Playing;

}
void Game::saveGameToFile(string file_path)
{
    ofstream file(file_path,ios::out | ios::binary);
    if(!file.is_open())
    {
        message.setString("ERROR :: Couldn't open file to save game.");
        message.setPosition(window.getSize().x/2-message.getLocalBounds().width/2,message.getPosition().y);
        return;
    }
    vector<char> data;
    append(survivedTime,data);
    for(auto& o : sats)
    {
        if(!o->isDestroyed())
        {
            auto _data = o->serialize();
            data.push_back(static_cast<char>(_data.size()));
            data.insert(data.end(),_data.begin(),_data.end());
        }
    }
    if(!file.write(&data[0], data.size()))
    {
        message.setString("ERROR :: Couldn't write file to save game.");
        message.setPosition(window.getSize().x/2-message.getLocalBounds().width/2,message.getPosition().y);
        return;
    }
}

void Game::addDebris(const sf::Vector2f& pos,const sf::Vector2f& sat_vel,bool isstation)
{
    for(int junks = (isstation?25:rand()%4+3); junks > 0; --junks)
    {
        float angle = atan2(sat_vel.y,sat_vel.x) + TO_RAD(rand()%20-10);
        auto vel = (float(rand())/RAND_MAX+0.5f)*magnitude(sat_vel)*cos_sin(angle);
        sats.push_back(unique_ptr<Orbiter>(new Debris(*this,pos,vel,rand()%30)));
    }
}

void Game::loadTexture(TextureName name,string path)
{
    unique_ptr<sf::Texture> newTexture(new sf::Texture);
    if(!newTexture->loadFromFile(path))
        throw runtime_error("Unable to load image.");
    auto ret = textures.insert(make_pair(name,move(newTexture)));
    if(!ret.second)
        throw runtime_error("Unable to insert image.");
}
