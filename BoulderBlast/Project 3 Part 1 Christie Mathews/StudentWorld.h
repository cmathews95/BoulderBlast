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
    bool open(int x, int y);

private:
    int currentLevel;
    Player* m_player;
    std::vector<Actor*> m_actors;
};

#endif // STUDENTWORLD_H_
