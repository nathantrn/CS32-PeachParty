#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Actors
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Actor::getXYtoCheck(int angle, int distance, double& convertedX, double& convertedY) //uses getPositionInthisDirection function but converts the X and Y coordinates from pix to grid coordinates for later use
{
    int futureX = 0;
    int futureY = 0;
    getPositionInThisDirection(angle, distance, futureX, futureY);
    convertedX = (futureX/16);
    convertedY = (futureY/16);
}

bool Actor::isOnBoard(int dir) //checks if the next grid entry over is on the path specified by a direction
{
    double convertedX;
    double convertedY;
    getXYtoCheck(dir, 16, convertedX, convertedY ); //check by 16 to see next grid entry
    if(getWorld()->isOnPath(convertedX , convertedY))
    {
        return true;
    }
    return false;
}
void Actor::WalkingOnPath() //Walk function mainly used for walking forward and corners
{
    if(getX()%16 == 0 && getY()%16 == 0)
    {
        if(isOnBoard(getWalkingDir())) //if walking forward is avaliable
        {
            moveAtAngle(getWalkingDir(), 2); //walking means moving two in the direction intended
            ticks_to_move--;
            ticks_has_move++;
        }
        else //if forward is not avaliable
        {
            if(getWalkingDir() == 0 || getWalkingDir() == 180) //if walking left or right
            {
                if(isOnBoard(90)) //check to see if up is avaliable
                {
                    setWalkingDir(90);
                    moveAtAngle(getWalkingDir(), 2);
                    ticks_to_move--;
                    ticks_has_move++;
                }
                else //if up is not avaliable the down must be
                {
                    setWalkingDir(90+180);
                    moveAtAngle(getWalkingDir(), 2);
                    ticks_to_move--;
                    ticks_has_move++;
                }
            }
            else
            {
            if(getWalkingDir() == 90 || getWalkingDir() == 270) //if walking up or down
            {
                if(isOnBoard(0)) //check if right is avaliable
                {
                    setWalkingDir(0);
                    moveAtAngle(getWalkingDir(), 2);
                    ticks_to_move--;
                    ticks_has_move++;
                }
                else //if not then left must be avaliable
                {
                    setWalkingDir(180);
                    moveAtAngle(getWalkingDir(), 2);
                    ticks_to_move--;
                    ticks_has_move++;
                }
            }
            }

        }
    }
    else
    {
        moveAtAngle(getWalkingDir(), 2);
        ticks_to_move--;
        ticks_has_move++;
    }
}

bool Actor::isAtFork() //checks to see if at a fork
{
    if(getX()%16 == 0 && getY()%16 == 0) //only check if the actor is directly ontop of a square
    {
        if((getWalkingDir() == 0) || (getWalkingDir() == 180)) //if walking left or right
        {
            if(isOnBoard(90) && isOnBoard(270) &&  !(isOnBoard(getWalkingDir()))) //if you can walk up or down but not forward
            {
                return true;
            }
            if((isOnBoard(90) || isOnBoard(270)) &&  (isOnBoard(getWalkingDir()))) //if you can walk forward or either up or down
            {
                if((isOnBoard(getWalkingDir())&&!isOnBoard(0))||(isOnBoard(getWalkingDir())&&!isOnBoard(180)))
                {
                    return false;
                }
                return true;
            }
        }
        else if((getWalkingDir() == 90) || (getWalkingDir() == 270)) //if walking up or down
        {
            if (isOnBoard(0) && isOnBoard(180) &&  !(isOnBoard(getWalkingDir()))) //if you can walk right or left but not forward
            {
                if((isOnBoard(getWalkingDir())&&!isOnBoard(90))||(isOnBoard(getWalkingDir())&&!isOnBoard(270))) //if you can walk forward and not up or forward and not down
                {
                    return false;
                }
                return true;
            }
            if((isOnBoard(0) || isOnBoard(180)) &&  (isOnBoard(getWalkingDir()))) //if you can walk right or left and forward
            {
                return true;
            }
        }
    }
    return false;
}

void Actor::setWalkingDir(int d) //set walking direction
{
    if (d < 0) //if direction given is negative convert
        d = 360 - (-d % 360);
    int temp = d % 360;
    if(temp == 180) //if direction is 180 then set SPRITE direction accordingly
    {
        setDirection(180);
    }
    else
    {
        setDirection(0); //all other times SPRITE is facing right
    }
    m_walkingDir = d % 360;
}

void Actor::moveToRandom() //move actor to a random square
{
    int randX = randInt(1, 10); //choose random number 1-10
    int randY = 1;
    if(getWorld()->isOnPath(randX , randY)) //check if random coordinates are on path
    {
        moveTo(randX * 16, randY *16); //if they are move to pixel location
    }
    else
    {
        while(!(getWorld()->isOnPath(randX , randY))) //if they are not increment Y value until an appropraite XY value is found
        {
            randY++;
        }
        moveTo(randX * 16, randY *16);
    }
    setWalkInvalid(true); //when actor is moved to a random square it can choose direction to proceed
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//PlayerAvatar
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

PlayerAvatar::PlayerAvatar(StudentWorld* world, int imageID, double startX, double startY): Actor(world, imageID, startX, startY, 0, 0, 1) //constructor
{
    m_coins = 0;
    setStatusToWaiting();
    m_numRolled = 0;
    m_stars = 0;
    m_startX = startX;
    m_startY = startY;
    setHasVortex(false);
    setACTIVE(true);
}

void PlayerAvatar::swapPeachYoshi() //swapping peach and yoshi
{
    if(getPlayerNum() == 1) //if player is peach
    { //make temp values of XY coords, ticks to move, walking dir, and playerstatus
        int tempX = getX();
        int tempY = getY();
        int tempTick = getTicksToMove();
        int tempDIR = getWalkingDir();
        bool tempStatus = getPlayerStatus();
        moveTo(getWorld()->getYoshi()->getX(),getWorld()->getYoshi()->getY()); //change all peach's values accordingly to yoshi's
        setTicksToMove( getWorld()->getYoshi()->getTicksToMove());
        setWalkingDir(getWorld()->getYoshi()->getWalkingDir());
        setPlayerStatus(getWorld()->getYoshi()->getPlayerStatus());
        getWorld()->getYoshi()->moveTo(tempX, tempY); //change all yoshi's values accordingly to peach's from the temp values
        getWorld()->getYoshi()->setTicksToMove(tempTick);
        getWorld()->getYoshi()->setWalkingDir(tempDIR);
        getWorld()->getYoshi()->setPlayerStatus(tempStatus);
        
    }
    else //do same if player is yoshi
    {
        int tempX = getX();
        int tempY = getY();
        int tempTick = getTicksToMove();
        int tempDIR = getWalkingDir();
        bool tempStatus = getPlayerStatus();
        moveTo(getWorld()->getPeach()->getX(),getWorld()->getPeach()->getY());
        setTicksToMove( getWorld()->getPeach()->getTicksToMove());
        setWalkingDir(getWorld()->getPeach()->getWalkingDir());
        setPlayerStatus(getWorld()->getPeach()->getPlayerStatus());
        getWorld()->getPeach()->moveTo(tempX, tempY);
        getWorld()->getPeach()->setTicksToMove(tempTick);
        getWorld()->getPeach()->setWalkingDir(tempDIR);
        getWorld()->getPeach()->setPlayerStatus(tempStatus);
    }
}

void PlayerAvatar::doSomething()
{
    int ch = getWorld()->getAction(getPlayerNum()); //get action from player
    if(getPlayerStatus()) //if player is waiting to roll
    {
        if(getWalkInvalid()) //if walk is invalid(from moving to a random sq) choose direction to proceed
        {
            switch(ch)
            {
                case ACTION_RIGHT:
                    if(isOnBoard(0))
                    {
                        setWalkingDir(0);
                        setWalkInvalid(false);
                    }
                    else
                    {
                        return;
                    }
                    break;
                case ACTION_LEFT:
                    if(isOnBoard(180))
                    {
                        setWalkingDir(180);
                        setWalkInvalid(false);
                    }
                    else
                    {
                        return;
                    }
                    break;
                case ACTION_UP:
                    if(isOnBoard(90))
                    {
                        setWalkingDir(90);
                        setWalkInvalid(false);
                    }
                    else
                    {
                        return;
                    }
                    break;
                case ACTION_DOWN:
                    if(isOnBoard(270))
                    {
                        setWalkingDir(270);
                        setWalkInvalid(false);
                    }
                    else
                    {
                        return;
                    }
                    break;
                default:
                    return;
            }
        }
        switch(ch) //get actions for rolling a die or firing a vortex
        {
            case ACTION_ROLL:
            {
                int die_roll = randInt(1, 10);;
                m_numRolled = die_roll;
                int t_moves = die_roll * 8;
                setTicksToMove(t_moves);
                setStatusToWalking();
                break;
            }
            case ACTION_FIRE:
            {
                if(getHasVortex())
                {
                    getWorld()->createVortex(this);
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    setHasVortex(false);
                }
                break;
            }
        default:
            break;
        }
    }
    else if(!getPlayerStatus()) //if player is walking
    {
        if(isAtFork() && !(getWorld()->isOverlappingAlwaysALiveSquare(this))) //if player is at a fork and not at a direcitonal square
        {
            switch(ch) //choose direction to proceed however direction choosen must be on the path
            {
                case ACTION_RIGHT:
                    if(getWalkingDir() != 180 && isOnBoard(0))
                    {
                        setWalkingDir(0);
                    }
                    else
                    {
                        return;
                    }
                    break;
                case ACTION_LEFT:
                    if(getWalkingDir() != 0  && isOnBoard(180))
                    {
                        setWalkingDir(180);
                    }
                    else
                    {
                        return;
                    }
                    break;
                case ACTION_UP:
                    if(getWalkingDir() != 270  && isOnBoard(90))
                    {
                        setWalkingDir(90);
                    }
                    else
                    {
                        return;
                    }
                    break;
                case ACTION_DOWN:
                    if(getWalkingDir() != 90  && isOnBoard(270))
                    {
                        setWalkingDir(270);
                    }
                    else
                    {
                        return;
                    }
                    break;
                default:
                    return;
            }
        }
        WalkingOnPath();
    if(getTicksToMove() == 0) //if no more ticks to move set player back to waiting to roll
    {
        setTicksHASMoved(0);
        setStatusToWaiting();
    }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//PEACH
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Peach::Peach(StudentWorld* world, double startX, double startY): PlayerAvatar(world, IID_PEACH, startX, startY)
{
    setPlayerNum(1);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Yoshi
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Yoshi::Yoshi(StudentWorld* world, double startX, double startY): PlayerAvatar(world, IID_YOSHI, startX, startY)
{
    setPlayerNum(2);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ActivateOnPlayer
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ActivateOnPlayer::ActivateOnPlayer(StudentWorld* world, int imageID, double startX, double startY, int startDirection, int depth, double size, bool activate_on_land) : Actor(world, imageID, startX, startY, startDirection, depth, size)
{
    m_activate_on_land = activate_on_land;
}

void ActivateOnPlayer::doSomething()
{
    if(m_activate_on_land) //if actor only causes an effect when landed on
    {
        if(getWorld()->contactOverlap(this, getWorld()->getPeach()) || getWorld()->contactOverlap(this, getWorld()->getYoshi())) //if actor is in contact overlap with one of the layers
        {
            if(getAlive1()) //if it is alive for peach and overlapping
            {
                if(getWorld()->contactOverlap(this, getWorld()->getPeach()) && getWorld()->getPeach()->getPlayerStatus())
                {
                    doActivity(getWorld()->getPeach()); //do actor's activity and set it to dead
                    setDead1();
                }
            }
            if(getAlive2()) //if it is alive for yoshi and overlapping
            {
                if(getWorld()->contactOverlap(this,  getWorld()->getYoshi()) && getWorld()->getYoshi()->getPlayerStatus())
                {
                    doActivity(getWorld()->getYoshi()); //do actor's activity and set it to dead
                    setDead2();
                }
            }
        }
        else //if they are not overlapping set it to alive
        {
            setAlive1();
            setAlive2();
        }
    }
    else //if square is always active and they are overlapping then do activity
    {
        if(getWorld()->contactOverlap(this, getWorld()->getPeach()) || getWorld()->contactOverlap(this, getWorld()->getYoshi()) )
        {
                if(getWorld()->contactOverlap(this, getWorld()->getPeach()))
                {
                    doActivity(getWorld()->getPeach());
                }
                if(getWorld()->contactOverlap(this,  getWorld()->getYoshi()))
                {
                    doActivity(getWorld()->getYoshi());
                }
        }
    }
        
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CoinSquare
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Coin_Square::Coin_Square(StudentWorld* world, int imageID, double startX, double startY, int startDirection, int adjust_coin_by): ActivateOnPlayer(world, imageID, startX,  startY, startDirection,1, 1, true)
{
    m_adjust_coin_by = adjust_coin_by;
    setACTIVE(true);
}

void Coin_Square::doActivity(PlayerAvatar* a) //adjust coins of player
{
    getWorld()->addCoins(m_adjust_coin_by, a);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//DirectionalSquare
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Directional_Square::Directional_Square(StudentWorld* world, double startX, double startY, int startDirection): ActivateOnPlayer(world, IID_DIR_SQUARE, startX, startY, startDirection, 1, 1, false)
{
    m_force_DIR = startDirection;
    setACTIVE(true);
}

void Directional_Square::doActivity(PlayerAvatar* a) //adjust walking dir of player
{
    getWorld()->changeWalkingDir(getForceDIR(), a);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//StarSquare
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Star_Square::Star_Square(StudentWorld* world, double startX, double startY, int startDirection): ActivateOnPlayer(world, IID_STAR_SQUARE, startX, startY, startDirection, 1, 1, false)
{
    setACTIVE(true);
}

void Star_Square::doActivity(PlayerAvatar* a) //adjust stars of player
{
    getWorld()->addSTAR(1, a);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//BankSquare
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Bank_Square::Bank_Square(StudentWorld* world, double startX, double startY, int startDirection): ActivateOnPlayer(world, IID_BANK_SQUARE, startX, startY, startDirection, 1, 1, false)
{
    setACTIVE(true);
}

void Bank_Square::doActivity(PlayerAvatar* a) //adjust contents of central bank and adjust coins of player
{
    getWorld()->addCoinsToCentralBank(a);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//DroppingSquare
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Dropping_Square::Dropping_Square(StudentWorld* world, double startX, double startY, int startDirection): ActivateOnPlayer(world, IID_DROPPING_SQUARE, startX, startY, startDirection, 1, 1, true)
{
    setACTIVE(true);
}

void Dropping_Square::doActivity(PlayerAvatar* a) //adjust coins of player or adjust stars of player chosen at random
{
    int num = randInt(1, 2);
    {
        if(num == 1)
        {
            if(a->getCoin() < 10) //adjust coins but make sure not to make player's coins negative
            {
                a->setCoin(0);
            }
            else
            {
                a->setCoin(a->getCoin()-10);
            }
        }
        if(num == 2)
        {
            if(a->getSTAR() >= 1) //adjust stars if they have any
            {
                a->setSTAR(a->getSTAR()-1);
            }
        }
        
    }
    getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Event Square
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Event_Square::Event_Square(StudentWorld* world, double startX, double startY, int startDirection): ActivateOnPlayer(world, IID_EVENT_SQUARE, startX, startY, startDirection, 1, 1, true)
{
    setACTIVE(true);
}

void Event_Square::doActivity(PlayerAvatar* a)
{
    int rand = randInt(1, 3); //choose random event
    if(a->getPlayerStatus())
    {
        if(rand == 1) //move player to random square
        {
            a->moveToRandom();
            getWorld()->playSound(SOUND_PLAYER_TELEPORT);
        }
        else if(rand ==2) //swap position of peach and yoshi
        {
            a->swapPeachYoshi();
            ActivateOnPlayer::setDead1(); //set dead so they are not swapping position infinitely
            ActivateOnPlayer::setDead2();
            getWorld()->playSound(SOUND_PLAYER_TELEPORT);
        }
        else if(rand == 3) //give player a vortex if they dont have one
        {
            if(a->getHasVortex()==false)
            {
                a->setHasVortex(true);
                getWorld()->playSound(SOUND_GIVE_VORTEX);
            }
        }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Vortex
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Vortex::Vortex(StudentWorld* world, double startX, double startY, int startDirection): ActivateOnPlayer(world, IID_VORTEX, startX, startY, startDirection, 0, 1, false)
{
    setAlive1();
    setWalkingDir(startDirection);
    setACTIVE(true);
}

void Vortex::doSomething() //vortex doSomething overrides the doSomething of ActiveOnPlayer class
{
    if(getAlive1())
    {
        moveAtAngle(getWalkingDir(),2); //vortex travels in one direction across the screen
    }
    if(getWorld()->isOverlappingHittable(this)) //if it bound overlaps a hittable character then it stops moving and is no longer active
    {
        setDead1();
        setTicksToMove(0);
        setACTIVE(false);
    }
    if(getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_HEIGHT) //if it travels off screen it is no longer active
    {
        setDead1();
        setACTIVE(false);
    }
}

void Vortex::doActivity(PlayerAvatar* a)
{
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Enemy
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Enemy::Enemy(StudentWorld* world, int imageID, int startX, int startY,
             int startDirection, int num_to_move, int pauseCounter): ActivateOnPlayer(world, imageID, startX, startY, startDirection, 0, 1, true)
{
    m_dir = startDirection;
    m_num_to_move = num_to_move;
    m_pauseCounter = pauseCounter;
    setACTIVE(true);
}

void Enemy::moveToRandom() //Enemy's move to random function overrides Actor's move to random function
{
    setNumTicksPause(180); //when enemy is moved to a random square its walking direction is to the right and it is reset to a paused state
    setWalkingDir(0);
    int randX = randInt(1, 10);
    int randY = 1;
    if(getWorld()->isOnPath(randX , randY)) //chooses random x coordinate and checks to see if XY are valid positions
    {
        moveTo(randX * 16, randY *16); //if they are move there
    }
    else
    {
        while(!(getWorld()->isOnPath(randX , randY))) //if they are not, increment Y coord until their coordinates are valid
        {
            randY++;
        }
        moveTo(randX * 16, randY *16);
    }
}

void Enemy::doSomething()
{
    if(m_pauseCounter > 0) //if enemy is in paused state
    {
        if(getWorld()->contactOverlap(this, getWorld()->getPeach()) || getWorld()->contactOverlap(this, getWorld()->getYoshi())) //if they contact overlap a player
        {
            if(getAlive1())
            {
                if(getWorld()->contactOverlap(this, getWorld()->getPeach()) && getWorld()->getPeach()->getPlayerStatus()) //if overlap and peach is waiting to roll
                {
                    doActivity(getWorld()->getPeach()); //do enemies activity to peach
                    setDead1();
                }
            }
            if(getAlive2())
            {
                if(getWorld()->contactOverlap(this,  getWorld()->getYoshi()) && getWorld()->getYoshi()->getPlayerStatus()) //if overlap and yoshi is waiting to roll
                {
                    doActivity(getWorld()->getYoshi()); //do enemies activity to yoshi
                    setDead2();
                }
            }
        }
        else
        {
            setAlive1();
            setAlive2();
        }
        m_pauseCounter--; //pause counter decrements
        setTicksToMove(getSquaresToMove()*8); //detemine how many ticks it can move according to which enemy they are(bowser or boo)
    }
    else if(m_pauseCounter == 0) //if no longer in paused state
    {
        if(isAtFork()) //if they are at a fork, choose random direction to walk
        {
            int rand = randInt(1, 4);
            if(rand == 2 && isOnBoard(0))
            {
                setWalkingDir(0);
            }
            else if( rand == 1 && isOnBoard(90))
            {
                setWalkingDir(90);
            }
            else if( rand == 3 && isOnBoard(180))
            {
                setWalkingDir(180);
            }
            else if( rand == 4 && isOnBoard(270))
            {
                setWalkingDir(270);
            }
        }
        WalkingOnPath(); // walk
        if(getTicksToMove() == 0) //if no longer can move set back to paused state
        {
            m_pauseCounter = 180;
            doSecondary(); //do secondary action if they have one
        }
    }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Browser
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Bowser::Bowser(StudentWorld* world, double startX, double startY, int startDirection): Enemy(world, IID_BOWSER, startX, startY, startDirection, 0, 180)
{
}

void Bowser::doActivity(PlayerAvatar* a)
{
    int num = randInt(1, 2); //bowser has a 50% chance of setting player's coins and stars to zero
    if(num == 1)
    {
        a->setCoin(0);
        a->setSTAR(0);
        getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
    }
   
}

void Bowser::doSecondary() //bowser's secondary creates a dropping square under him
{
    int num = randInt(1,4); //has a 25% chance of doing so
    if(num == 1)
    {
        getWorld()->createDroppingSq(this);
    }
}


int Bowser::getSquaresToMove() //bowsers avaliable squares to move is random from 1-10
{
    return randInt(1, 10);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Boo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Boo::Boo(StudentWorld* world, double startX, double startY, int startDirection): Enemy(world, IID_BOO, startX, startY, startDirection, 0, 180)
{
}

void Boo::doActivity(PlayerAvatar* a)
{
    int num = randInt(1, 2); //boo has 2 possible activities with 50 chance of doing it
    if(num == 1) //option 1 swap coins of players
    {
        if(a->getPlayerNum() == 1)
        {
        int tempCoins = a->getCoin();
        a->setCoin(getWorld()->getYoshi()->getCoin());
        getWorld()->getYoshi()->setCoin(tempCoins);
        }
        else
        {
            int tempCoins = a->getCoin();
            a->setCoin(getWorld()->getPeach()->getCoin());
            getWorld()->getPeach()->setCoin(tempCoins);
        }
    }
    else //option 2 swap stars of players
    {
        if(a->getPlayerNum() == 2)
        {
        int tempCoins = a->getSTAR();
        a->setSTAR(getWorld()->getYoshi()->getSTAR());
        getWorld()->getYoshi()->setSTAR(tempCoins);
        }
        else
        {
            int tempSTARs = a->getSTAR();
            a->setSTAR(getWorld()->getPeach()->getSTAR());
            getWorld()->getPeach()->setSTAR(tempSTARs);
        }
    }
    getWorld()->playSound(SOUND_BOO_ACTIVATE);
}
void Boo::doSecondary() //boo does not have secondary
{
    return;
}


int Boo::getSquaresToMove() //squares to move is random from 1-3
{
    return randInt(1, 3);
}

