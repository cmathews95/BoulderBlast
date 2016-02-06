#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(int ID, int X, int Y, Direction dir, int health, StudentWorld* s);
    virtual ~Actor();
    
    // Action to perform each tick
    virtual void doSomething() = 0;
    
    // Is this actor alive?
    bool isAlive() const;
    
    // Mark this actor as dead
    void setDead();
    
    // Reduce this actor's hit points
    void decHitPoints(unsigned int amt);
    
    // Get this actor's world
    StudentWorld* getWorld() const;
    
    // Can an agent occupy the same square as this actor?
    virtual bool allowsPlayerColocation() const;
    
    // Can a boulder occupy the same square as this actor?
    virtual bool allowsBoulder() const;
    
    // Does this actor count when a factory counts items near it?
    virtual bool countsInFactoryCensus() const;
    
    // Does this actor stop bullets from continuing?
    virtual bool stopsBullet() const;
    
    // Can this actor be damaged by bullets?
    virtual bool isDamageable() const;
    
    virtual bool isRobot() const;
    
    // Cause this Actor to sustain damageAmt hit points of damage.
    virtual void damage(unsigned int damageAmt);
    
    // Can this actor be swallowed by a hole?
    virtual bool isSwallowable() const;
    
    // Can this actor be picked up by a KleptoBot?
    virtual bool isStealable() const;
    
    // How many hit points does this actor have left?
    virtual int getHitPoints() const;
    
    // Set this actor's hit points to amt.
    virtual void setHitPoints(int amt);
    
    // Make the actor sustain damage.  Return true if this kills the
    // actor, and false otherwise.
    virtual bool tryToBeKilled(unsigned int damageAmt);

private:
    int hp;
    bool m_alive;
    StudentWorld* n_studentWorld;
};

///////////////////////////PLAYER & ROBOT CLASSES///////////////////////////
class Player : public Actor
{
public:
    Player(int x0, int y0, StudentWorld* s_w);
    void doSomething();
    virtual bool isDamageable() const;
    virtual bool stopsBullet() const;
    //Add Ammo to Player
    virtual void addAmmo(int x);
    //Return Amount of Ammo
    virtual int returnAmmo() const;
    //Reduce the Player's HP
    virtual void damage(unsigned int damageAmt);
private:
    int m_ammo;
};



class Robot : public Actor
{
public:
    Robot(int ID, int x0, int y0, Direction d, int health, int reward, StudentWorld* s_w);
    virtual void doSomething() = 0;
    virtual bool stopsBullet() const;
    virtual bool isDamageable() const;
    virtual bool isRobot() const;
    //If a robot is shot
    virtual void damage(unsigned int damageAmt);
    //return true if there is a clear shot
    virtual bool ClearShot() const;
    // Does this robot shoot?
    virtual bool isShootingRobot() const;
    //Move if possible, else change direction
    virtual void move();
    //Shoot if able to and in correct situation
    virtual bool shoot();
//    Return number of ticks. For movement.
    virtual int getTicks() const;
    //Set Ticks to 0
    virtual void incTicks();
    //Check if the Robot can move. AKA: the correct number of ticks
    virtual bool moveTurn();
private:
    int m_reward;
    int ticks;
    int m_ticks;
};



class SnarlBot : public Robot
{
public:
    SnarlBot(int x0, int y0, Direction dir, StudentWorld* s_w);
    bool typeSnarlbot() const;
    //Return True if Robot Shoots
    virtual bool isShootingRobot() const;
    virtual void doSomething();
private:
    //True if Horizonatal Snarlbot, False if Vertical
    bool m_type;
};


class KleptoBot : public Robot
{
public:
    KleptoBot(int ID, int x0, int y0, int health, int reward, StudentWorld* s_w);
    //Function to Steal if the situation is right
    virtual bool Steal();
    virtual bool isShootingRobot() const;
    virtual void doSomething();
    //Yes KleptoBots Count
    virtual bool countsInFactoryCensus() const;
    virtual void damage(unsigned int damageAmt);

    
private:
    //distance before turning
    int dbTurning;
    //elapsed distance
    int d_T;
    //ID of item Stolen
    int itemStolen;
};



class RegularKleptoBot : public KleptoBot
{
public:
    RegularKleptoBot(int x0, int y0, StudentWorld*s_w);
};



class AngryKleptoBot : public KleptoBot
{
public:
    AngryKleptoBot(int x0, int y0, StudentWorld* s_w);
    virtual bool isShootingRobot() const;
};


///////////////////////////OTHER CLASSES///////////////////////////////////
class Wall : public Actor
{
public:
    Wall(int x0, int y0, StudentWorld* s_w);
    void doSomething();
    virtual bool stopsBullet() const;
private:
};


class Boulder : public Actor
{
public:
    Boulder(int x0, int y0, StudentWorld* s_w);
    void doSomething();
    virtual bool stopsBullet() const;
    virtual bool isDamageable() const;
};


class Bullet : public Actor
{
public:
    Bullet(int x0, int y0, Direction d, StudentWorld* s_w);
    virtual bool allowsPlayerColocation() const;
    void doSomething();
};

class Hole : public Actor
{
public:
    Hole(int x0, int y0, StudentWorld* s_w);
    virtual void doSomething();
    //Returns true because boulders can move in to holes
    virtual bool allowsBoulder() const;
};

class KleptoBotFactory : public Actor
{
public:
    //True if Regular KleptoBot Factory
    //False if Angry KleptoBot Factory
    KleptoBotFactory(int x0, int y0, bool type, StudentWorld* s_w);
    void doSomething();
    bool stopsBullet() const;
private:
    bool factoryType;
};


class Exit : public Actor
{
public:
    Exit(int x0, int y0, StudentWorld* s_w);
    virtual void doSomething();
    //The Player can be on the exit location
    virtual bool allowsPlayerColocation() const;
private:
    bool revealed;
};

////////////////////////////ITEM CLASSES///////////////////////////////////

class Item : public Actor
{
public:
    Item(int ID, int r, int x0, int y0, StudentWorld* s_w, Direction d=none);
    virtual void doSomething();
    virtual int reward();
    virtual int getReward() const;
    //Player can be on the same location as the items
    virtual bool allowsPlayerColocation() const;
private:
    int m_reward;
};

class Jewel : public Item
{
public:
    Jewel(int x0, int y0, StudentWorld* s_w);
};


class Goodie : public Item
{
public:
    Goodie(int ID, int r, int x0, int y0, StudentWorld* s_w);
    //This function takes care of all the goodies with a switch statement
    virtual void doSomething();
    virtual bool isStealable() const;
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(int x0, int y0, StudentWorld* s_w);
};

class RestoreHealthGoodie : public Goodie
{
public:
    RestoreHealthGoodie(int x0, int y0, StudentWorld* s_w);
};

class AmmoGoodie : public Goodie
{
public:
    AmmoGoodie(int x0, int y0, StudentWorld* s_w);
};




#endif // ACTOR_H_















