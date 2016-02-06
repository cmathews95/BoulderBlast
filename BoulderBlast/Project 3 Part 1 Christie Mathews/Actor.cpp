#include "Actor.h"
#include "StudentWorld.h"


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int ID, int X, int Y, Direction dir, int health, StudentWorld* s)
:GraphObject(ID, X, Y, dir), hp(health), n_studentWorld(s)
{
    setVisible(true);
    m_alive = true;
}

Actor::~Actor(){};

StudentWorld* Actor::World(){
        return n_studentWorld;
}

bool Actor::isAlive(){
    return m_alive;
}

void Actor::kill(){
    m_alive = false;
}


Player::Player(int x0, int y0, StudentWorld* s_w)
:Actor(IID_PLAYER,x0,y0,right, 20, s_w), m_ammo(20)
{
}

void Player::doSomething()
{
    //KEY_PRESS_LEFT KEY_PRESS_RIGHT KEY_PRESS_UP KEY_PRESS_DOWN KEY_PRESS_SPACE KEY_PRESS_ESCAPE
    if(!isAlive())
        return;
    int key;
    if(World()->getKey(key)){
        int x = getX();
        int y = getY();
        switch (key) {
            case KEY_PRESS_DOWN:
                if(World()->open(x, y-1)){
                    moveTo(x, y-1);
                    setDirection(GraphObject::down);
                }
                break;
            case KEY_PRESS_UP:
                if(World()->open(x, y+1)){
                    moveTo(x, y+1);
                    setDirection(GraphObject::up);
                }
                break;
            case KEY_PRESS_RIGHT:
                if(World()->open(x+1, y)){
                    moveTo(x+1, y);
                    setDirection(GraphObject::right);
                }
                break;
            case KEY_PRESS_LEFT:
                if(World()->open(x-1, y)){
                    moveTo(x-1, y);
                    setDirection(GraphObject::left);
                }
                break;

        }
    }
}


Wall::Wall(int x0, int y0, StudentWorld* s_w)
//no direction
:Actor(IID_WALL,x0,y0,none, 0, s_w)
{
}

void Wall::doSomething()
{
    
}
