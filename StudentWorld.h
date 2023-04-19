#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld();
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
    virtual ~StudentWorld();
    bool isOnPath(double x, double y);
    bool contactOverlap(Actor* a, Actor* b);
    bool boundOverlap(Actor* a, Actor* b);
    void addCoins(int numCoins, PlayerAvatar* a);
    void addSTAR(int numSTARS, PlayerAvatar* a);
    void changeWalkingDir(int DIR, PlayerAvatar* a);
    int getCentralBank() {return centralBank;} ;
    void addCoinsToCentralBank(PlayerAvatar* a);
    bool isOverlappingAlwaysALiveSquare(Actor* a);
    void createVortex(PlayerAvatar *a);
    void createDroppingSq(Actor* a);
    Peach* getPeach(){return m_Peach;};
    Yoshi* getYoshi() {return m_Yoshi;};
    bool isOverlappingHittable(Actor* a);
private:
    Peach* m_Peach;
    Yoshi* m_Yoshi;
    std::vector<Actor*> actors;
    int centralBank;
};

#endif // STUDENTWORLD_H_
