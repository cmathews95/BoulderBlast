#include "StudentWorld.h"
#include <string>
#include <typeinfo>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir), currentLevel(-1), m_player(nullptr)
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
    //String Stream to cycle through level data files
    currentLevel++;
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
                default:
                    break;
            }
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_player->doSomething();
    for(int i = 0; i < m_actors.size(); i++)
        m_actors[i]->doSomething();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_player;
    m_player = nullptr;
    for(int i = 0; i < m_actors.size(); i++)
        delete m_actors[i];
    m_actors.clear();
}

bool StudentWorld::open(int x, int y){
    for(int i = 0; i < m_actors.size(); i++)
//        if(typeid(m_actors[i]) == typeid(Wall))
            if(m_actors[i]->getX()==x && m_actors[i]->getY()==y)
                return false;
    return true;
}


