#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject //base class for all my other classes
{
public:
    Actor(StudentWorld* world, int imageID, double startX, double startY, int startDirection, int depth, double size) : GraphObject(imageID, SPRITE_WIDTH * startX, SPRITE_HEIGHT * startY, startDirection, depth, size), m_world(world)
        {m_walkingDir = 0;
        ticks_to_move = 0;};
    virtual void doSomething() = 0;
    virtual bool is_a_square()=0;
    virtual bool is_always_alive()=0;
    virtual bool can_be_hit_by_vortex()=0;
    
    virtual bool isACTIVE(){return ACTIVE;}; //this determines if it should be deleted later in studentworld
    virtual void setACTIVE(bool a){ACTIVE = a;};
    
    virtual void moveToRandom(); //teleporting to random sq

    void getXYtoCheck(int angle, int distance, double& convertedX, double& convertedY);
    void WalkingOnPath();
    
    int getWalkingDir(){return m_walkingDir;};
    void setWalkingDir(int d);
    
    bool isOnBoard(int dir);
    bool isAtFork();
    
    int getTicksToMove() {return ticks_to_move;}; //ticks TO move
    int getTicksHASMoved() {return ticks_has_move;}; //ticks the actor has already moved
    void setTicksHASMoved(int num) {ticks_has_move = num;};
    void setTicksToMove(int ticks) {ticks_to_move = ticks;};
    int getSpacesToMove(){return ticks_to_move/8;};
    
    void setWalkInvalid(bool w) {m_walkInvalid = w;};
    bool getWalkInvalid() {return m_walkInvalid;};
    
    StudentWorld* getWorld(){return m_world;};
private:
    StudentWorld* m_world;
    int m_walkingDir;
    int ticks_to_move;
    int ticks_has_move;
    bool m_walkInvalid;
    bool ACTIVE;
};

class PlayerAvatar : public Actor
{
public:
    PlayerAvatar(StudentWorld* world, int imageID, double startX, double startY);
    
    virtual void doSomething();
    
    void setStatusToWaiting() {m_playerIsWaiting = true;};
    void setStatusToWalking() {m_playerIsWaiting = false;};
    
    bool getPlayerStatus() {return m_playerIsWaiting;}; //returns true if player is waiting false if walking
    void setPlayerStatus(bool w) { m_playerIsWaiting = w;};
    
    void addCoin(int numCoins){m_coins += numCoins;};
    void addSTAR(int numStars) {m_stars += numStars;};
    int getCoin(){return m_coins;};
    int getSTAR(){return m_stars;};
    void setCoin(int num){m_coins = num;};
    void setSTAR(int num){m_stars = num;};
    
    void setPlayerNum(int i){m_playerNum = i;};
    int getPlayerNum(){return m_playerNum;};
    
    int getNumRolled(){return m_numRolled; };

    bool getHasVortex(){return m_has_vortex;};
    void setHasVortex(bool hasVortex ){m_has_vortex = hasVortex;};

    virtual bool is_a_square(){return false;};
    virtual bool is_always_alive(){return false;};
    virtual bool can_be_hit_by_vortex(){return false;};

    void swapPeachYoshi(); //Swaps peach and yoshi's position

private:
    int spacesToMove;
    int m_numRolled;
    int m_coins;
    int m_stars;
    int m_playerNum;
    bool m_playerIsWaiting;
    bool m_has_vortex;
    double m_startX;
    double m_startY;
};

class Peach : public PlayerAvatar
{
public:
    Peach(StudentWorld* world, double startX, double startY);
private:
};

class Yoshi : public PlayerAvatar
{
public:
    Yoshi(StudentWorld* world, double startX, double startY);
private:
};


class ActivateOnPlayer : public Actor //anything that can have an effect on other actors
{
public:
    ActivateOnPlayer(StudentWorld* world, int imageID, double startX, double startY, int startDirection, int depth, double size, bool activate_on_land);
    virtual void doSomething(); //has a doSomething and within that doSomething calls an Activity that is unique to its derived classes
    virtual void doActivity(PlayerAvatar* a)=0;
    bool getAlive1() {return isAlive1;}; //alive for player 1
    void setAlive1() {isAlive1 = true;};
    void setDead1() {isAlive1 = false;};
    bool getAlive2() {return isAlive2;}; //alive for player 2
    void setAlive2() {isAlive2 = true;};
    void setDead2() {isAlive2 = false;};
    virtual bool can_be_hit_by_vortex()=0;
private:
    bool m_activate_on_land;
    bool isAlive1;
    bool isAlive2;
};

class Coin_Square : public ActivateOnPlayer
{
public:
    Coin_Square(StudentWorld* world, int imageID, double startX, double startY, int startDirection, int adjust_coin_by);
    virtual void doActivity(PlayerAvatar* a);
    virtual bool is_a_square(){return true;};
    virtual bool is_always_alive(){return false;};
    virtual bool can_be_hit_by_vortex(){return false;};
private:
    int m_adjust_coin_by;
};

class Directional_Square : public ActivateOnPlayer
{
public:
    Directional_Square(StudentWorld* world, double startX, double startY, int startDirection);
    virtual void doActivity(PlayerAvatar* a);
    int getForceDIR() {return m_force_DIR;}; //forces player in a specific direction
    virtual bool is_a_square(){return true;};
    virtual bool is_always_alive(){return true;};
    virtual bool can_be_hit_by_vortex(){return false;};
private:
    int m_force_DIR;
};

class Star_Square : public ActivateOnPlayer
{
public:
    Star_Square(StudentWorld* world, double startX, double startY, int startDirection);
    virtual void doActivity(PlayerAvatar* a);
    virtual bool is_a_square(){return true;};
    virtual bool is_always_alive(){return false;};
    virtual bool can_be_hit_by_vortex(){return false;};
private:
};

class Bank_Square : public ActivateOnPlayer
{
public:
    Bank_Square(StudentWorld* world, double startX, double startY, int startDirection);
    virtual void doActivity(PlayerAvatar* a);
    virtual bool is_a_square(){return true;};
    virtual bool is_always_alive(){return true;};
    virtual bool can_be_hit_by_vortex(){return false;};
private:
};

class Dropping_Square : public ActivateOnPlayer
{
public:
    Dropping_Square(StudentWorld* world, double startX, double startY, int startDirection);
    virtual void doActivity(PlayerAvatar* a);
    virtual bool is_a_square(){return true;};
    virtual bool is_always_alive(){return false;};
    virtual bool can_be_hit_by_vortex(){return false;};
private:
};

class Event_Square : public ActivateOnPlayer
{
public:
    Event_Square(StudentWorld* world, double startX, double startY, int startDirection);
    virtual void doActivity(PlayerAvatar* a);
    virtual bool is_a_square(){return true;};
    virtual bool is_always_alive(){return false;};
    virtual bool can_be_hit_by_vortex(){return false;};
private:
};

class Vortex : public ActivateOnPlayer
{
public:
    Vortex(StudentWorld* world, double startX, double startY, int startDirection);
    virtual void doSomething(); //vortex has its own doSomething function
    virtual bool is_a_square(){return false;};
    virtual bool is_always_alive(){return false;};
    virtual bool can_be_hit_by_vortex(){return false;};
    virtual void doActivity(PlayerAvatar* a);
private:
};

class Enemy : public ActivateOnPlayer //enemies activate on player but also move around the board unlike the other derived classes
{
public:
    Enemy(StudentWorld* world, int imageID, int startX, int startY,
           int startDirection, int num_to_move, int pauseCounter);
    virtual void doSomething(); //has its own do something function
    virtual void doSecondary()=0;
    virtual int getSquaresToMove()=0;
    
    bool is_a_square(){return false;};
    virtual bool is_always_alive(){return false;};
    virtual bool can_be_hit_by_vortex(){return true;};
    
    int getNumtoMove(){return m_num_to_move;};
    int getNumTicksPause(){return m_pauseCounter;}
    
    void setNumtoMove(int num){m_num_to_move = num;};
    void setNumTicksPause(int num){m_pauseCounter= num;};
    virtual void moveToRandom();
private:
    int m_dir;
    int m_num_to_move;
    int m_pauseCounter;
    
};
class Bowser : public Enemy
{
public:
    Bowser(StudentWorld* world, double startX, double startY, int startDirection);
    virtual void doActivity(PlayerAvatar* a);
    virtual void doSecondary();
    virtual int getSquaresToMove();
private:
};

class Boo : public Enemy
{
public:
    Boo(StudentWorld* world, double startX, double startY, int startDirection);
    virtual void doActivity(PlayerAvatar* a);
    virtual void doSecondary();
    virtual int getSquaresToMove();
private:
};

#endif // ACTOR_H_
