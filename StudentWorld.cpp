#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
using namespace std;


GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_Peach = nullptr;
    centralBank = 0;
}

int StudentWorld::init() //initialize the data structures used to keep track of aspects of my game world
{
    Board current;
    string boardFile = "board0" + to_string(getBoardNumber()) + ".txt";
    Board::LoadResult result = current.loadBoard(assetPath() + boardFile); //load in board file from users choice
    if (result == Board::load_success)
    {
    for (int x = 0; x < 16; x++) //check every grid point
    {
        for (int y = 0; y < 16; y++)
        {
        Board::GridEntry ge = current.getContentsOf(x, y); //get the board contents at the grid point
        switch (ge) //switch statement goes to each possible actor on the board and adds them to a vector if they are present
        {
            case Board::empty: //empty do nothing
            {
                break;
            }
            case Board::player: //add peach and yoshi and add a coin square under them
            {
                m_Peach = new Peach(this, x, y);
                m_Yoshi = new Yoshi(this, x, y);
                actors.push_back(new Coin_Square(this, IID_BLUE_COIN_SQUARE, x, y, 0, 3));
                break;
            }
            case Board::blue_coin_square: // coin square that grants coins
            {
                actors.push_back(new Coin_Square(this, IID_BLUE_COIN_SQUARE, x, y, 0, 3));
                break;
            }
            case Board::red_coin_square: // coin square that takes coin
            {
                actors.push_back(new Coin_Square(this, IID_RED_COIN_SQUARE, x, y, 0, -3));
                break;
            }
            case Board::up_dir_square:
            {
                actors.push_back(new Directional_Square(this, x, y, 90));
                break;
            }
            case Board::down_dir_square:
            {
                actors.push_back(new Directional_Square(this, x, y, 270));
                break;
            }
            case Board::right_dir_square:
            {
                actors.push_back(new Directional_Square(this, x, y, 0));
                break;
            }
            case Board::left_dir_square:
            {
                actors.push_back(new Directional_Square(this, x, y, 180));
                break;
            }
            case Board::star_square:
            {
                actors.push_back(new Star_Square(this, x, y, 0));
                break;
            }
            case Board::bank_square:
            {
                actors.push_back(new Bank_Square(this, x, y, 0));
                break;
            }
            case Board::event_square:
            {
                actors.push_back(new Event_Square(this, x, y, 0));
                break;
            }
            case Board::boo: // add boo and a coin square under them
            {
                actors.push_back(new Boo(this, x, y, 0));
                actors.push_back(new Coin_Square(this, IID_BLUE_COIN_SQUARE, x, y, 0, 3));
                break;
            }
            case Board::bowser: // add bowser and a coin square under them
            {
                actors.push_back(new Bowser(this, x, y, 0));
                actors.push_back(new Coin_Square(this, IID_BLUE_COIN_SQUARE, x, y, 0, 3));
                break;
            }
            default:
                break;
                
        }
        }
    }
    }
    else
    {
        return GWSTATUS_BOARD_ERROR;
    }
    startCountdownTimer(99);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    string statText = "";
    if(m_Peach->getHasVortex() || m_Yoshi->getHasVortex()) //decide whether to put VOR in the statText depending on if the Players have a VORTEX
    {
        if(m_Peach->getHasVortex() && m_Yoshi->getHasVortex())
        {
            statText = "P1 Roll: " + to_string(m_Peach->getSpacesToMove()) + " Stars: "+ to_string(m_Peach->getSTAR()) +" $$: " + to_string(m_Peach->getCoin()) + " VOR | Time: " + to_string(timeRemaining()) + " | Bank: "+ to_string(getCentralBank()) +" | P2 Roll: " + to_string(m_Yoshi->getSpacesToMove()) + " Stars: "+ to_string(m_Yoshi->getSTAR()) +" $$: " + to_string(m_Yoshi->getCoin()) + " VOR";
        }
        else if(m_Peach->getHasVortex())
        {
            statText = "P1 Roll: " + to_string(m_Peach->getSpacesToMove()) + " Stars: "+ to_string(m_Peach->getSTAR()) +" $$: " + to_string(m_Peach->getCoin()) + " VOR | Time: " + to_string(timeRemaining()) + " | Bank: "+ to_string(getCentralBank()) +" | P2 Roll: " + to_string(m_Yoshi->getSpacesToMove()) + " Stars: "+ to_string(m_Yoshi->getSTAR()) +" $$: " + to_string(m_Yoshi->getCoin());
        }
        else if(m_Yoshi->getHasVortex())
        {
            statText = "P1 Roll: " + to_string(m_Peach->getSpacesToMove()) + " Stars: "+ to_string(m_Peach->getSTAR()) +" $$: " + to_string(m_Peach->getCoin()) + " | Time: " + to_string(timeRemaining()) + " | Bank: "+ to_string(getCentralBank()) +" | P2 Roll: " + to_string(m_Yoshi->getSpacesToMove()) + " Stars: "+ to_string(m_Yoshi->getSTAR()) +" $$: " + to_string(m_Yoshi->getCoin()) + " VOR";
        }
    }
    else
    {
        statText = "P1 Roll: " + to_string(m_Peach->getSpacesToMove()) + " Stars: "+ to_string(m_Peach->getSTAR()) +" $$: " + to_string(m_Peach->getCoin()) + " | Time: " + to_string(timeRemaining()) + " | Bank: "+ to_string(getCentralBank()) +" | P2 Roll: " + to_string(m_Yoshi->getSpacesToMove()) + " Stars: "+ to_string(m_Yoshi->getSTAR()) +" $$: " + to_string(m_Yoshi->getCoin());

    }
    setGameStatText(statText);
    m_Peach->doSomething(); //ask player to do something
    m_Yoshi->doSomething();//ask player to do something
    for (int i = 0; i < actors.size(); i++) //go through vector and ask each actor to do something
    {
        if(actors[i]->isACTIVE()==true)
        {
        actors[i]->doSomething();
        }
    }
    std::vector<Actor*>::iterator it;
    for (it = actors.begin(); it != actors.end(); it++) //if actor is NOT active then remove actor from the vector
    {
        if ((*it)->isACTIVE() == false)
        {
                delete *it;
                actors.erase(it);
                it = actors.begin();
        }
    }
    if(timeRemaining()==0) //if game is over decide who wins
    {
        playSound(SOUND_GAME_FINISHED);
        if(m_Peach->getSTAR() > m_Yoshi->getSTAR()) //first look at stars
        {
            setFinalScore(m_Peach->getSTAR(), m_Peach->getCoin());
            return GWSTATUS_PEACH_WON;
        }
        else if(m_Peach->getSTAR() < m_Yoshi->getSTAR())
        {
            setFinalScore(m_Yoshi->getSTAR(), m_Yoshi->getCoin());
            return GWSTATUS_YOSHI_WON;
        }
        else if(m_Peach->getSTAR() == m_Yoshi->getSTAR()) //if stars are equal look at coins
        {
            if(m_Peach->getCoin() > m_Yoshi->getCoin())
            {
                setFinalScore(m_Peach->getSTAR(), m_Peach->getCoin());
                return GWSTATUS_PEACH_WON;
            }
            else if(m_Peach->getCoin() < m_Yoshi->getCoin())
            {
                setFinalScore(m_Yoshi->getSTAR(), m_Yoshi->getCoin());
                return GWSTATUS_YOSHI_WON;
            }
            else if(m_Peach->getCoin() == m_Yoshi->getCoin()) //if coins are equal decide randomly
            {
                int num = randInt(1, 2);
                {
                    if(num == 1)
                    {
                        setFinalScore(m_Peach->getSTAR(), m_Peach->getCoin());
                        return GWSTATUS_PEACH_WON;
                    }
                    else
                    {
                        setFinalScore(m_Yoshi->getSTAR(), m_Yoshi->getCoin());
                        return GWSTATUS_YOSHI_WON;
                    }
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_Peach; //delete player pointers
    delete m_Yoshi;
    m_Peach = nullptr; //set to nullptr so that when destructor is called it is not deleting a member variable that is already deleted
    m_Yoshi = nullptr;
    for (int i = 0; i < actors.size(); i++) //delete every actor in the vector
    {
    delete actors[i];
    }
    actors.clear();

}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

bool StudentWorld::isOnPath(double x, double y) //check if the coordinates sent is on path of the game
{
    Board current; //load in board file
    string boardFile = "board0" + to_string(getBoardNumber()) + ".txt";
    current.loadBoard(assetPath() + boardFile);
    Board::GridEntry ge = current.getContentsOf(x, y); //check grid entry of the coordinates passed to the function
    if(ge == Board::empty) //if empty return false
    {
        return false;
    }
    return true;
}
bool StudentWorld::contactOverlap(Actor* a, Actor* b) //determine contact overlap --> when two game aspect are directly over each other
{
    if(a->getX() == b->getX() && a->getY() == b->getY()) //use exact coordinates of game aspects
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool StudentWorld::boundOverlap(Actor* a, Actor* b) //determine bound overlap -->when two game aspect touch
{
    if (a->getX() + SPRITE_WIDTH > b->getX() && a->getX()< b->getX() + SPRITE_WIDTH) //use the bounds created by the sprite width
    {
            if (a->getY() + SPRITE_HEIGHT > b->getY() && a->getY() < b->getY() + SPRITE_HEIGHT)
            {
                return true;
            }
    }
    return false;
}


bool StudentWorld::isOverlappingAlwaysALiveSquare(Actor* a) //check if actor passed to it is overlapping a square that is always alive
{
    for (int i = 0; i < actors.size(); i++)
    {
        if(actors[i]->is_a_square() && actors[i]->is_always_alive() && contactOverlap(a, actors[i])) //uses contact overlap
        {
            return true;
        }
    }
    return false;
}

bool StudentWorld::isOverlappingHittable(Actor* a) //check if actor passed(which will be a vortex actor) is overlapping a hittable character
{
    for (int i = 0; i < actors.size(); i++) //check every actor
    {
        if(actors[i]->can_be_hit_by_vortex() == true) //if the actor can be hit by vortex and is NOT a square
        {
            if(actors[i]->is_a_square() == false)
            {
                if(boundOverlap(a, actors[i])) //and if the actor and the actor passed are bound overlapping
                {
                    actors[i]->moveToRandom(); //move actor to a random place on the board
                    playSound(SOUND_HIT_BY_VORTEX);
                    return true;
                }
            }
        }
    }
    return false;
}

void StudentWorld::addCoins(int numCoins, PlayerAvatar* a) //add coins(could be adding or taking coins away)
{
    if(a->getPlayerStatus())
    {
        if(a->getCoin()+ numCoins < 0) //make sure coins will not be negative due to the change
        {
            a->addCoin(-(a->getCoin()));
            playSound(SOUND_TAKE_COIN);
        }
        else
        {
            a->addCoin(numCoins);
            if(numCoins > 0)
            {
                playSound(SOUND_GIVE_COIN);
            }
            else
            {
                playSound(SOUND_TAKE_COIN);
            }
        }
    }
}



void StudentWorld::changeWalkingDir(int DIR, PlayerAvatar* a) //change walking direction
{
    a->setWalkingDir(DIR);
}

void StudentWorld::addSTAR(int numSTARS,PlayerAvatar* a) // add stars
{
    if(a->getCoin()>20)
    {
        a->addCoin(-20);
        a->addSTAR(1);
        playSound(SOUND_GIVE_STAR);
    }
}


void StudentWorld::addCoinsToCentralBank(PlayerAvatar* a) //add coins to central bank(can be adding or taking coins away)
{
    if(a->getPlayerStatus()) //if player status == true meaning the player is waiting to roll that indicates the player landed on the bank square
    {
       a->addCoin(getCentralBank()); //give player all the coins from the central bank
        playSound(SOUND_WITHDRAW_BANK);
        centralBank = 0;
    }
    else if(a->getTicksHASMoved() < 1) //if the player just started moving off the sqaure do nothing(prevents double activation)
    {
        return;
    }
    else //if player is just moving across the bank square
    {
        if(a->getCoin() > 5) //take coins but ensure that player coins do not become negative
        {
            a->addCoin(-5);
            playSound(SOUND_DEPOSIT_BANK);
            centralBank += 5;
        }
        else
        {
            centralBank += a->getCoin();
            a->addCoin(-(a->getCoin()));
            playSound(SOUND_DEPOSIT_BANK);
        }
    }
}

void StudentWorld::createVortex(PlayerAvatar* a) //create a vortex actor within the gameworld
{
    int newX = 0;
    int newY = 0;
    a->getPositionInThisDirection(a->getWalkingDir(), 16, newX, newY); //get vortex's starting position as the square right infron of the player's current position
    actors.push_back(new Vortex(this, newX/16, newY/16, a->getWalkingDir())); //coordinates need to be divided by 16 to convert from pixel to grid coordinates
}

void StudentWorld::createDroppingSq(Actor* a) //create a dropping sq actor within the game world
{
    for(int i=0;i<actors.size();i++)
    {
        if(contactOverlap(actors[i], a)==true) //find the square that the actor is directly over
        {
            if(actors[i]->is_a_square()==true)
            {
                actors[i]->setACTIVE(false); //set that square to NOT active so that it can be deleted later
                actors.push_back(new Dropping_Square(this, a->getX()/16, a->getY()/16, 0)); //add the dropping square in place of the deleted square
                playSound(SOUND_DROPPING_SQUARE_CREATED);
                return;
            }
        }
    }
}
