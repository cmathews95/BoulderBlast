#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(int ID, int X, int Y, Direction dir, int health, StudentWorld* s);
    virtual void doSomething() = 0;
    virtual ~Actor();
    StudentWorld* World();
    bool isAlive();
    void kill();
private:
    int hp;
    bool m_alive;
    StudentWorld* n_studentWorld;
};


class Player : public Actor
{
public:
    Player(int x0, int y0, StudentWorld* s_w);
    void doSomething();
private:
    int m_ammo;
};


class Wall : public Actor
{
public:
    Wall(int x0, int y0, StudentWorld* s_w);
    void doSomething();
private:
};

#endif // ACTOR_H_
