#include "Actor.h"
#include "StudentWorld.h"
#include <cstdlib>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int ID, int X, int Y, Direction dir, int health, StudentWorld* s)
:GraphObject(ID, X, Y, dir), hp(health), n_studentWorld(s)
{
    setVisible(true);
    m_alive = true;
}

Actor::~Actor(){};

bool Actor::isAlive() const
{
    return m_alive;
}

void Actor::setDead(){
    m_alive = false;
    setVisible(false);
}

StudentWorld* Actor::getWorld() const
{
        return n_studentWorld;
}

bool Actor::allowsPlayerColocation() const
{
    return false;
}

bool Actor::allowsBoulder() const
{
    return false;
}

bool Actor::countsInFactoryCensus() const
{
    return false;
}

bool Actor::stopsBullet() const
{
    return false;
}

bool Actor::isDamageable() const
{
    return false;
}

void Actor::damage(unsigned int damageAmt)
{
    hp-=damageAmt;
    if(hp <= 0)
        setDead();
}

bool Actor::isRobot() const
{
    return false;
}

bool Actor::isSwallowable() const
{
    return false;
}

bool Actor::isStealable() const
{
    return false;
}

int Actor::getHitPoints() const
{
    return hp;
}

void Actor::setHitPoints(int amt)
{
    hp = amt;
}

bool Actor::tryToBeKilled(unsigned int damageAmt)
{
    if((hp - damageAmt) <= 0)
        return true;
    return false;
}


//==================PLAYER====================

Player::Player(int x0, int y0, StudentWorld* s_w)
:Actor(IID_PLAYER, x0, y0, right, 20, s_w), m_ammo(20)
{
}

void Player::doSomething()
{
    //KEY_PRESS_LEFT KEY_PRESS_RIGHT KEY_PRESS_UP KEY_PRESS_DOWN KEY_PRESS_SPACE KEY_PRESS_ESCAPE
    if(!isAlive())
        return;
    int key;
    if(getWorld()->getKey(key)){
        int x = getX();
        int y = getY();
        switch (key) {
            case KEY_PRESS_ESCAPE:
                setDead();
                break;
            case KEY_PRESS_SPACE:
                //figure out which direction the bullet should be shot with
                if(getDirection()== GraphObject::up)
                    y++;
                if(getDirection()== GraphObject::down)
                    y--;
                if(getDirection()== GraphObject::left)
                    x--;
                if(getDirection()== GraphObject::right)
                    x++;
                //Only shoot if you have ammo left
                if(m_ammo > 0){
                    getWorld()->addBullet(x, y, getDirection());
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    m_ammo--;
                }
                break;
            case KEY_PRESS_DOWN:
                //MOVE BOULDER IF POSSIBLE
                if(getWorld()->isBoulder(x, y-1)){
                    if(y-2 > 0 && getWorld()->canBoulderMoveTo(x, y-2))
                       getWorld()->getBoulder(x, y-1)->moveTo(x, y-2);
                }
                //MOVE TO LOCATION
                setDirection(GraphObject::down);
                if(getWorld()->canActorMoveTo(x, y-1)){
                    moveTo(x, y-1);
                }
                break;
                
            case KEY_PRESS_UP:
                //MOVE BOULDER IF POSSIBLE
                if(getWorld()->isBoulder(x, y+1)){
                    if(y+2 < VIEW_HEIGHT && getWorld()->canBoulderMoveTo(x, y+2))
                       getWorld()->getBoulder(x, y+1)->moveTo(x, y+2);
                }
                //MOVE TO LOCATION
                setDirection(GraphObject::up);
                if(getWorld()->canActorMoveTo(x, y+1)){
                    moveTo(x, y+1);
                }
                break;
                
            case KEY_PRESS_RIGHT:
                //MOVE BOULDER IF POSSIBLE
                if(getWorld()->isBoulder(x+1, y)){
                    if(x+2 < VIEW_WIDTH && getWorld()->canBoulderMoveTo(x+2, y))
                       getWorld()->getBoulder(x+1, y)->moveTo(x+2, y);
                }
                //MOVE TO LOCATION
                setDirection(GraphObject::right);
                if(getWorld()->canActorMoveTo(x+1, y)){
                    moveTo(x+1, y);
                }
                break;
                
            case KEY_PRESS_LEFT:
                //MOVE BOULDER IF POSSIBLE
                if(getWorld()->isBoulder(x-1, y)){
                    if(x-2 > 0 && getWorld()->canBoulderMoveTo(x-2, y))
                       getWorld()->getBoulder(x-1, y)->moveTo(x-2, y);
                }
                //MOVE TO LOCATION
                setDirection(GraphObject::left);
                if(getWorld()->canActorMoveTo(x-1, y)){
                    moveTo(x-1, y);
                }
                break;
                
            default:
                break;
                
        }
    }
}

bool Player::isDamageable() const
{
    return true;
}

bool Player::stopsBullet() const
{
    return true;
}

void Player::addAmmo(int x)
{
    m_ammo += x;
}

int Player::returnAmmo() const
{
    return m_ammo;
}

void Player::damage(unsigned int damageAmt)
{
    Actor::damage(damageAmt);
    if(getHitPoints() <= 0)
        getWorld()->playSound(SOUND_PLAYER_DIE);
    else
        getWorld()->playSound(SOUND_PLAYER_IMPACT);
}

//=================ROBOT===================
Robot::Robot(int ID, int x0, int y0, Direction d, int health, int reward, StudentWorld* s_w)
:Actor(ID, x0, y0, d, health, s_w), m_reward(reward), ticks(0), m_ticks(0)
{
    //This switch is to set the Robot's tick standard depending on the level
    m_ticks = (28 - dynamic_cast<GameWorld*>(s_w)->getLevel()) / 4;
    if (m_ticks < 3)
        m_ticks = 3;
}

bool Robot::moveTurn()
{
    //increment ticks
    ticks++;
    //Check if the Robot is alive and is on the right tick to move.
    if(!isAlive() || ticks != Robot::getTicks())
        return false;
    return true;
}

void Robot::move()
{
    int x = getX(), y = getY();
    if(getDirection()== up)
        y++;
    else if(getDirection() == down)
        y--;
    else if(getDirection() == left)
        x--;
    else
        x++;
    //Move if possible
    if(getWorld()->canActorMoveTo(x, y))
        moveTo(x, y);
    else if(getDirection() == up)
        setDirection(down);
    else if(getDirection() == down)
        setDirection(up);
    else if(getDirection() == left)
        setDirection(right);
    else
        setDirection(left);
    
    ticks = 0;
}

bool Robot::shoot()
{
//    if(!isShootingRobot())
//        return false;
    int x = getX(), y = getY();
    if(getDirection()== up)
        y++;
    else if(getDirection() == down)
        y--;
    else if(getDirection() == left)
        x--;
    else
        x++;
    
    if(ClearShot()){
        switch (getDirection()) {
            case up:
                getWorld()->addBullet(x, y, getDirection());
                break;
            case down:
                getWorld()->addBullet(x, y, getDirection());
                break;
            case left:
                getWorld()->addBullet(x, y, getDirection());
                break;
            case right:
                getWorld()->addBullet(x, y, getDirection());
                break;
            default:
                break;
        }
        getWorld()->playSound(SOUND_ENEMY_FIRE);
        ticks = 0;
        return true;
    }
    return false;
}

bool Robot::isDamageable() const
{
    return true;
}

bool Robot::isRobot() const
{
    return true;
}

bool Robot::stopsBullet() const
{
    return true;
}

void Robot::damage(unsigned int damageAmt)
{
    Actor::damage(damageAmt);
    if(getHitPoints() <= 0){
        getWorld()->playSound(SOUND_ROBOT_DIE);
        getWorld()->increaseScore(m_reward);
        setDead();
    }else{
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
}

bool Robot::ClearShot() const
{
//    //if this type of robot doesn't shoot return false
//    if(!isShootingRobot())
//        return false;
    
    //If the Player isn't in the same row or column, return false
    if(getX() != getWorld()->getPlayer()->getX() && getY() != getWorld()->getPlayer()->getY())
        return false;
    //Check if the direction the robot is facing is clear of obstacles
    Direction x = getDirection();
    if(x == up){
        for(int i = getY()+1; i < VIEW_HEIGHT;i++){
            if(getX()==getWorld()->getPlayer()->getX() && i==getWorld()->getPlayer()->getY())
                return true;
            if(!getWorld()->canActorShootAt(getX(), i))
                return false;
        }
    }else if(x == down){
        for(int i = getY()-1; i >= 0;i--){
            if(getX()==getWorld()->getPlayer()->getX() && i==getWorld()->getPlayer()->getY())
                return true;
            if(!getWorld()->canActorShootAt(getX(), i))
                return false;
        }
    }else if(x == left){
        for(int i = getX()-1; i >= 0;i--){
            if(i==getWorld()->getPlayer()->getX() && getY()==getWorld()->getPlayer()->getY())
                return true;
            if(!getWorld()->canActorShootAt(i, getY()))
                return false;
        }
    }else{
        for(int i = getX()+1; i < VIEW_WIDTH;i++){
            if(i==getWorld()->getPlayer()->getX() && getY()==getWorld()->getPlayer()->getY())
                return true;
            if(!getWorld()->canActorShootAt(i, getY()))
                return false;
        }
    }
    return true;
}

bool Robot::isShootingRobot() const
{
    return false;
}

int Robot::getTicks() const
{
    return m_ticks;
}

void Robot::incTicks()
{
    ticks=0;
}

//=================SNARLBOT===================
SnarlBot::SnarlBot(int x0, int y0, Direction dir, StudentWorld* s_w)
:Robot(IID_SNARLBOT, x0, y0, dir, 10, 100, s_w)
{
}

bool SnarlBot::isShootingRobot() const
{
    return true;
}

void SnarlBot::doSomething()
{
    if(!moveTurn())
        return;
    if(isShootingRobot())
        if(shoot())
            return;
    move();
}

//=================KLEPTOBOT===================
KleptoBot::KleptoBot(int ID, int x0, int y0, int health, int reward, StudentWorld* s_w)
:Robot(ID, x0, y0, right, health, reward, s_w), itemStolen(0), d_T(0)
{
    dbTurning = rand()%6;
}

void KleptoBot::doSomething()
{
    if(!moveTurn())
        return;
    if(Steal())
        return;
    if(this->isShootingRobot()){
        if(shoot())
            return;
    }
    Direction prev = getDirection(), curr = getDirection();
    bool x = true;
    //These bools will be true if visited and false otherwise
    bool l = false, r = false, u = false, d = false;
    if(d_T == dbTurning){
    Retry:
        if(l && r && u && d){
            setDirection(prev);
            return;
        }
        
        switch (rand()%4) {
            case 0:
                if(u)goto Retry;
                curr = up;
                if(x){
                    prev = curr;
                    x = false;
                }
                move();
                if(getDirection() == curr)
                    return;
                else(u = true);
            case 1:
                if(d)goto Retry;
                curr = down;
                if(x){
                    prev = curr;
                    x = false;
                }
                move();
                if(getDirection() == curr)
                    return;
                else(d = true);
            case 2:
                if(l)goto Retry;
                curr = left;
                if(x){
                    prev = curr;
                    x = false;
                }
                move();
                if(getDirection() == curr)
                    return;
                else(l = true);
            case 3:
                if(r)goto Retry;
                curr = right;
                if(x){
                    prev = curr;
                    x = false;
                }
                move();
                if(getDirection() == curr)
                    return;
                else(r = true);
            default:
                break;
        }
    }
    Direction a = getDirection();
    move();
    if(getDirection() != a){
        setDirection(a);
        goto Retry;
    }
}

void KleptoBot::damage(unsigned int damageAmt){
    Robot::damage(damageAmt);
    if(getHitPoints() <= 0 && itemStolen != 0){
        getWorld()->createGoodie(getX(), getY(), itemStolen);
    }
}


bool KleptoBot::Steal()
{
    if(!getWorld()->isGoodie(getX(), getY()))
        return false;
    int a = (rand()%10);
    if(a != 5)
        return false;
    if(itemStolen!=0)
        return false;
    itemStolen = getWorld()->getGoodie(getX(), getY());
    if(itemStolen != 0){
        getWorld()->playSound(SOUND_ROBOT_MUNCH);
        incTicks();
        return true;
    }
    return false;
}

bool KleptoBot::isShootingRobot() const
{
    return false;
}

bool KleptoBot::countsInFactoryCensus() const
{
    return true;
}

//=================REGULAR KLEPTOBOT===================
RegularKleptoBot::RegularKleptoBot(int x0, int y0, StudentWorld* s_w)
:KleptoBot(IID_KLEPTOBOT, x0, y0, 5, 10, s_w)
{
    
}



//=================ANGRY KLEPTOBOT===================
AngryKleptoBot::AngryKleptoBot(int x0, int y0, StudentWorld* s_w)
:KleptoBot(IID_ANGRY_KLEPTOBOT, x0, y0,8, 20, s_w)
{
    
}

bool AngryKleptoBot::isShootingRobot() const
{
    return true;
}

//==================WALL====================
Wall::Wall(int x0, int y0, StudentWorld* s_w)
//no direction
:Actor(IID_WALL, x0, y0, none, 0, s_w)
{
}

void Wall::doSomething()
{
    
}

bool Wall::stopsBullet() const
{
    return true;
}
//==================BOULDER====================
Boulder::Boulder(int x0, int y0, StudentWorld* s_w)
:Actor(IID_BOULDER, x0, y0, none, 10, s_w)
{
    
}

void Boulder::doSomething()
{
    
}

bool Boulder::stopsBullet() const
{
    return true;
}

bool Boulder::isDamageable() const
{
    return true;
}

//==================BULLET====================
Bullet::Bullet(int x0, int y0, Direction d, StudentWorld* s_w)
:Actor(IID_BULLET, x0, y0, d, 0, s_w)
{
    
}

bool Bullet::allowsPlayerColocation() const
{
    return true;
}

void Bullet::doSomething()
{
    if(!isAlive())
        return;

    if(getWorld()->bulletAttack(getX(), getY())){
        setDead();
        return;
    }
    
    switch (getDirection()) {
        case GraphObject::up:
            moveTo(getX(), getY()+1);
            break;
        case GraphObject::down:
            moveTo(getX(), getY()-1);
            break;
        case GraphObject::left:
            moveTo(getX()-1, getY());
            break;
        case GraphObject::right:
            moveTo(getX()+1, getY());
            break;
        default:
            break;
    }

}

//==================WALL====================
Hole::Hole(int x0, int y0, StudentWorld* s_w)
:Actor(IID_HOLE, x0, y0, none, 0, s_w)
{
    
}

void Hole::doSomething()
{
    if(getWorld()->isBoulder(getX(), getY())){
        setDead();
        getWorld()->getBoulder(getX(), getY())->setDead();
    }
}

bool Hole::allowsBoulder() const
{
    return true;
}

//==================KLEPTOBOT FACTORY====================
KleptoBotFactory::KleptoBotFactory(int x0, int y0, bool type, StudentWorld* s_w)
:Actor(IID_ROBOT_FACTORY, x0, y0, none, 0, s_w), factoryType(type)
{
    
}

void KleptoBotFactory::doSomething()
{
    if(getWorld()->kleptoCensus(getX(), getY()) < 3 && !getWorld()->isKleptoBot(getX(), getY())){
        int a = rand()%50;
        if(a == 25){
            getWorld()->createKleptoBot(getX(), getY(), factoryType);
            getWorld()->playSound(SOUND_ROBOT_BORN);
        }
    }
            //if it's a regular factory, a regular KleptoBot will be created
    return;
}

bool KleptoBotFactory::stopsBullet() const
{
    return true;
}

//==================EXIT====================
Exit::Exit(int x0, int y0, StudentWorld* s_w)
:Actor(IID_EXIT, x0, y0, none, 0, s_w), revealed(false)
{
    setVisible(false);
}

void Exit::doSomething()
{
    if(getWorld()->jewelsRemaining()){
        setVisible(true);
        if(!revealed)
            getWorld()->playSound(SOUND_REVEAL_EXIT);
        revealed = true;
    }else
        return;
    if(getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY()){
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->increaseScore(2000);
        getWorld()->increaseScore(getWorld()->returnBonus());
        getWorld()->finishLevel();
    }
        
}

bool Exit::allowsPlayerColocation() const
{
    return true;
}

//==================ITEM MASTER CLASS====================
Item::Item(int ID, int r, int x0, int y0, StudentWorld* s_w, Direction d)
:Actor(ID, x0, y0, d, 0, s_w), m_reward(r)
{
    
}

void Item::doSomething(){
    if(getX() == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY()){
        getWorld()->increaseScore(m_reward);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        setDead();
    }
}

int Item::getReward() const
{
    return m_reward;
}

int Item::reward()
{
    return m_reward;
}

bool Item::allowsPlayerColocation() const
{
    return true;
}

//==================JEWEL====================
Jewel::Jewel(int x0, int y0, StudentWorld* s_w)
:Item(IID_JEWEL, 50, x0, y0, s_w)
{
    
}


//==================GOODIE MASTER CLASS====================
Goodie::Goodie(int ID, int r, int x0, int y0, StudentWorld* s_w)
:Item(ID, r, x0, y0, s_w)
{
    
}

bool Goodie::isStealable() const
{
    return true;
}


void Goodie::doSomething()
{
    if(!isAlive())
        return;
    Item::doSomething();
    if(!isAlive())
        switch (getID()) {
            case IID_EXTRA_LIFE:
                getWorld()->incLives();
                getWorld()->increaseScore(getReward());
                getWorld()->playSound(SOUND_GOT_GOODIE);
                break;
            case IID_RESTORE_HEALTH:
                getWorld()->getPlayer()->setHitPoints(20);
                getWorld()->increaseScore(getReward());
                getWorld()->playSound(SOUND_GOT_GOODIE);
                break;
            case IID_AMMO:
                dynamic_cast<Player*>(getWorld()->getPlayer())->addAmmo(20);
                getWorld()->increaseScore(getReward());
                getWorld()->playSound(SOUND_GOT_GOODIE);
                break;
            default:
                break;
        }
    
            
    
}


//==================EXTRA LIFE GOODIE====================
ExtraLifeGoodie::ExtraLifeGoodie(int x0, int y0, StudentWorld* s_w)
:Goodie(IID_EXTRA_LIFE, 1000, x0, y0, s_w)
{
    
}

//==================RESTORE HEALTH GOODIE====================
RestoreHealthGoodie::RestoreHealthGoodie(int x0, int y0, StudentWorld* s_w)
:Goodie(IID_RESTORE_HEALTH, 500, x0, y0, s_w)
{
    
}

//==================AMMO GOODIE====================
AmmoGoodie::AmmoGoodie(int x0, int y0, StudentWorld* s_w)
:Goodie(IID_AMMO, 100, x0, y0, s_w)
{
    
}






















