#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    //Should the destructor be virtual
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void setDisplayText();
    
    // Can agent move to x,y?
    bool canActorMoveTo(int x, int y) const;
    
    // Can an Actor Shoot in a straight path?
    bool canActorShootAt(int x, int y) const;
    
    // Can a boulder move to x,y?
    bool canBoulderMoveTo(int x, int y) const;
    
    // Is there a boulder at x,y?
    bool isBoulder(int x, int y) const;
    
    // Is there a KleptoBot here?
    bool isKleptoBot(int x, int y) const;
    
    //Return the ID of a goodie at x,y
    int getGoodie(int x, int y);
    
    //Check if there is a goodie at x,y
    bool isGoodie(int x, int y) const;
    
    //create a bullet heading in a certian direction
    void addBullet(int x, int y, Actor::Direction d);
    
    //count the number of kleptobots in a certain area
    int kleptoCensus(int x, int y);
    
    //Return Pointer to Boulder at Location
    Actor* getBoulder(int x, int y);
    
    //Return a point to the Player object
    Actor* getPlayer() const;
    
    //Let the bullet attack an item if its in the way
    bool bulletAttack(int x, int y);
    
    //Check for Jewels Remaining
    bool jewelsRemaining() const;
    
    //True if normal KleptoBot, false if angry K.
    void createKleptoBot(int x, int y, bool type);
    
    //Recreate Stolen goodie when KleptoBot dies
    void createGoodie(int x, int y, int id);
    
    //REturn bonus
    int returnBonus() const;
    
    //Set levelFInihsed to true;
    void finishLevel();
private:
    int currentLevel;
    int m_bonus;
    Player* m_player;
    std::vector<Actor*> m_actors;
    bool levelFinished;
};

#endif // STUDENTWORLD_H_
