#include "StudentWorld.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir), currentLevel(0), m_player(nullptr), m_bonus(1000), levelFinished(false)
{
    
}

StudentWorld::~StudentWorld(){
    if(m_player!=nullptr)
        delete m_player;
    for(int i = 0; i < m_actors.size(); i++)
        delete m_actors[i];
    m_actors.clear();
};

int StudentWorld::init()
{
    m_bonus = 1000;
    if(getLives()<=0)
        return GWSTATUS_PLAYER_WON;
    if(levelFinished){
        levelFinished = false;
        currentLevel++;
    }
    //String Stream to cycle through level data files
    std::ostringstream oss;
    oss << "level0" << currentLevel;
    std::string level1 = oss.str();
    std::cout << level1 << std::endl;
    level1 += ".dat";
    Level lev(assetDirectory());
    Level::LoadResult result = lev.loadLevel(level1);
    if (result == Level::load_fail_file_not_found){
        cerr << "Could not find " << level1 << " data file\n";
        return GWSTATUS_PLAYER_WON;
    }else if (result == Level::load_fail_bad_format)
        cerr << "Your level was improperly formatted\n";
    else if (result == Level::load_success)
        cerr << "Successfully loaded level\n";
    
    //Create Level
    //Using a for loop to traverse the 2D Array
    for(int i = 0; i < VIEW_WIDTH; i++){
        for(int j = 0; j < VIEW_HEIGHT; j++){
            switch (lev.getContentsOf(i, j)) {
                case Level::wall:
                    m_actors.push_back(new Wall(i, j, this));
                    break;
                case Level::player:
                    m_player = new Player(i, j, this);
                    break;
                case Level::boulder:
                    m_actors.push_back(new Boulder(i, j, this));
                    break;
                case Level::hole:
                    m_actors.push_back(new Hole(i, j, this));
                    break;
                case Level::jewel:
                    m_actors.push_back(new Jewel(i, j, this));
                    break;
                case Level::extra_life:
                    m_actors.push_back(new ExtraLifeGoodie(i, j, this));
                    break;
                case Level::restore_health:
                    m_actors.push_back(new RestoreHealthGoodie(i, j, this));
                    break;
                case Level::ammo:
                    m_actors.push_back(new AmmoGoodie(i, j, this));
                    break;
                case Level::vert_snarlbot:
                    m_actors.push_back(new SnarlBot(i, j, Actor::down, this));
                    break;
                case Level::horiz_snarlbot:
                    m_actors.push_back(new SnarlBot(i, j, Actor::right, this));
                    break;
                case Level::kleptobot_factory:
                    //True if Regular KleptoBot Factory, False otherwise
                    m_actors.push_back(new KleptoBotFactory(i, j, true, this));
                    break;
                case Level::angry_kleptobot_factory:
                    m_actors.push_back(new KleptoBotFactory(i, j, false, this));
                    break;
                case Level::exit:
                    m_actors.push_back(new Exit(i, j, this));
                    break;
                default:
                    break;
            }
        }
    }

    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    setDisplayText();
    m_bonus--;
    //Move All the Actors
    m_player->doSomething();
    for(int i = 0; i < m_actors.size(); i++)
        if(m_actors[i]!=nullptr)
            m_actors[i]->doSomething();
   
    //If the Player has died deal with this
    if(!m_player->isAlive()){
        decLives();
        std::cout << "2LIVES--" << std::endl;
        return GWSTATUS_PLAYER_DIED;
    }
    
    //Deal with all the actos that died
    for(int i = 0; i < m_actors.size(); i++)
        if(m_actors[i]!=nullptr)
            if(!(m_actors[i]->isAlive())){
                delete m_actors[i];
                m_actors[i]=nullptr;
            }
    if(levelFinished)
        return GWSTATUS_FINISHED_LEVEL;
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_player;
    m_player = nullptr;
    for(int i = 0; i < m_actors.size(); i++){
        if(m_actors[i]!=nullptr)
            delete m_actors[i];
    }
    m_actors.clear();
}

// Can agent move to x,y?  (dx and dy indicate the direction of motion)
bool StudentWorld::canActorMoveTo(int x, int y) const
{
    for(int i = 0; i < m_actors.size(); i++){
        if(m_actors[i]!=nullptr){
            if(m_player->getX() == x && m_player->getY() == y)
                return false;
            
            if(m_actors[i]->getX()==x && m_actors[i]->getY()==y)
                if(!m_actors[i]->allowsPlayerColocation())
                    return false;
//            if(m_actors[i]->getX()==x && m_actors[i]->getY()==y){
//                switch (m_actors[i]->getID()) {
//                    case IID_BOULDER:
//                    case IID_WALL:
//                    case IID_SNARLBOT:
//                    case IID_ANGRY_KLEPTOBOT:
//                    case IID_KLEPTOBOT:
//                    case IID_ROBOT_FACTORY:
//                        return false;
//                        break;
//                    case IID_JEWEL:
//                    case IID_EXTRA_LIFE:
//                    case IID_RESTORE_HEALTH:
//                    case IID_AMMO:
//                    case IID_BULLET:
//                        return true;
//                        break;
//                    default:
//                        return true;
//                        break;
//                }
//            }
        }
    }
    return true;
}

bool StudentWorld::canActorShootAt(int x, int y) const
{
    for(int i = 0; i < m_actors.size(); i++){
        if(m_actors[i]!=nullptr){
            if(m_player->getX() == x && m_player->getY() == y)
                return true;
            if(m_actors[i]->getX()==x && m_actors[i]->getY()==y){
                switch (m_actors[i]->getID()) {
                    case IID_BOULDER:
                    case IID_WALL:
                    case IID_SNARLBOT:
                    case IID_ANGRY_KLEPTOBOT:
                    case IID_KLEPTOBOT:
                    
                        return false;
                        break;
                    case IID_ROBOT_FACTORY:
                    case IID_JEWEL:
                    case IID_EXTRA_LIFE:
                    case IID_RESTORE_HEALTH:
                    case IID_AMMO:
                    case IID_BULLET:
                    case IID_HOLE:
                        return true;
                        break;
                    default:
                        return true;
                        break;
                }
            }
        }
    }
    return true;
}

// Can a boulder move to x,y?
bool StudentWorld::canBoulderMoveTo(int x, int y) const
{
    for(int i = 0; i < m_actors.size(); i++){
        if(m_actors[i]!=nullptr)
            if(m_actors[i]->getX()==x && m_actors[i]->getY()==y){
                if(!m_actors[i]->allowsBoulder())
                    return false;
            }
    }
    return true;
}

bool StudentWorld::isBoulder(int x, int y) const
{
    for(int i = 0; i < m_actors.size(); i++){
        if(m_actors[i]!=nullptr)
            if(m_actors[i]->getX() == x && m_actors[i]->getY() == y)
                if(m_actors[i]->getID()== IID_BOULDER)
                    return true;
    }
    return false;
}

int StudentWorld::getGoodie(int x, int y)
{
    for(int i = 0; i < m_actors.size(); i++)
        if(m_actors[i]!=nullptr)
            if(m_actors[i]->getX()==x && m_actors[i]->getY()==y)
                if(m_actors[i]->isStealable()){
                    int temp = m_actors[i]->getID();
                    m_actors[i]->setDead();
                    return temp;
                }
    return 0;

}

bool StudentWorld::isGoodie(int x, int y) const
{
    for(int i = 0; i < m_actors.size(); i++)
        if(m_actors[i]!=nullptr)
            if(m_actors[i]->getX()==x && m_actors[i]->getY()==y)
                if(m_actors[i]->isStealable())
                    return true;
    return false;
}


void StudentWorld::addBullet(int x, int y, Actor::Direction d)
{
    m_actors.push_back(new Bullet(x, y, d, this));
}

//Return Boulder
Actor* StudentWorld::getBoulder(int x, int y)
{
    for(int i = 0; i < m_actors.size(); i++)
        if(m_actors[i]!=nullptr)
            if(m_actors[i]->getX()==x && m_actors[i]->getY()==y)
                if(m_actors[i]->getID()==IID_BOULDER)
                    return m_actors[i];
    return nullptr;
}

bool StudentWorld::bulletAttack(int x, int y)
{
    if(m_player!=nullptr)
        if(m_player->getX()==x && m_player->getY()==y){
            m_player->damage(2);
            return true;
        }
    for(int i = 0; i < m_actors.size(); i++)
        if(m_actors[i]!=nullptr)
            if(m_actors[i]->getID()!=IID_BULLET)
                if(m_actors[i]->getX()==x && m_actors[i]->getY()==y)
                    if(m_actors[i]->stopsBullet()){
                        if(m_actors[i]->getID()== IID_ROBOT_FACTORY){
                            if(isKleptoBot(x, y)){
                                //Damage this Klepto Bot
                                //Function to return klept at location
                                return true;
                            }else{
                                return true;
                            }
                        }
                        if(m_actors[i]->isRobot()){
                            m_actors[i]->damage(2);
                            return true;
                        }
                        if(m_actors[i]->isDamageable()){
                            m_actors[i]->damage(2);
                            return true;
                        }
                        return true;
                    }
    return false;
}

Actor* StudentWorld::getPlayer() const
{
    return m_player;
}

int StudentWorld::kleptoCensus(int x, int y)
{
    int temp = 0;
    int leftlimit, rightlimit, upperlimit, lowerlimit;
    
    if(x - 3 < 0)leftlimit = 0;
    else leftlimit = x - 3;
    
    if(x + 3 > 14)rightlimit = 14;
    else rightlimit = x + 3;
    
    if(y - 3 < 0)lowerlimit = 0;
    else lowerlimit = y - 3;
    
    if(y + 3 > 14) upperlimit = 14;
    else upperlimit = y + 3;
    
    for(int i = lowerlimit; i <= upperlimit; i++)
        for(int j = leftlimit; j<= rightlimit; j++)
            if(isKleptoBot(j,i))
                temp++;
    
    return temp;
}
    
               
bool StudentWorld::isKleptoBot(int x, int y) const
{
    for(int i = 0; i < m_actors.size(); i++)
        if(m_actors[i]!=nullptr)
            if(m_actors[i]->getID()==IID_KLEPTOBOT||m_actors[i]->getID()==IID_ANGRY_KLEPTOBOT)
                if(m_actors[i]->getX() == x && m_actors[i]->getY())
                    return true;
    return false;
}

void StudentWorld::createKleptoBot(int x, int y, bool type)
{
    if(type){
        m_actors.push_back(new RegularKleptoBot(x, y, this));
    }else
        m_actors.push_back(new AngryKleptoBot(x, y, this));
}

void StudentWorld::createGoodie(int x, int y, int id)
{
    switch (id) {
        case IID_AMMO:
            m_actors.push_back(new AmmoGoodie(x, y, this));
            break;
        case IID_EXTRA_LIFE:
            m_actors.push_back(new ExtraLifeGoodie(x, y, this));
            break;
        case IID_RESTORE_HEALTH:
            m_actors.push_back(new RestoreHealthGoodie(x, y, this));
            break;
        default:
            break;
    }
}

bool StudentWorld::jewelsRemaining() const
{
    for(int i = 0; i < m_actors.size(); i++)
        if(m_actors[i]!=nullptr)
            if(m_actors[i]->getID()==IID_JEWEL)
                    return false;
    return true;
}

int StudentWorld::returnBonus() const
{
    return m_bonus;
}

void StudentWorld::finishLevel()
{
    levelFinished = true;
}


void StudentWorld::setDisplayText()
{
    int score = getScore();
    int level = getLevel();
    int lives = getLives();
    float h = (m_player->getHitPoints()/20.0)*100.0;
//    int health = (m_player->getHitPoints()/20)*100;
    int ammo = (m_player->returnAmmo());
    std::ostringstream oss;
    oss.fill('0');
    oss << "Score: " << std::setw(7) << score;
    oss.fill('0');
    oss << "  Level: " << std::setw(2) << level;
    oss.fill(' ');
    oss << "  Lives: " << std::setw(2) << lives;
    oss.fill(' ');
    oss << "  Health: " << std::setw(3) << h;
    oss << "%";
    oss.fill(' ');
    oss << "  Ammo: " << std::setw(3) << ammo;
    oss.fill(' ');
    oss <<"  Bonus: " << std::setw(4) << m_bonus;
    setGameStatText(oss.str());
}




