#ifndef _ENEMYSHIP_
#define _ENEMYSHIP_

#include "Ship.hpp"

/* Current Enemy Mode */
enum EnemyMode {
    /* Ship Is Not Moving */
    ENEMY_MODE_IDLE,
    /* Ship Is Moving Arround */
    ENEMY_MODE_NAVEGATING,
    /* Ship Is Working In A Certain Task */
    ENEMY_MODE_WORKING,
    /* Ship Is Retreating From Battle */
    ENEMY_MODE_RETREAT,
    /* Ship Is Fighting Someone */
    ENEMY_MODE_FIGHTING
};

/* Enemy Agressivenese Twords Nearby Ships */
enum EnemyAgressivenese {
    /* Ship Will Avoid Any Confrontation And Flee If There Is Any Enemies Nearby */
    ENEMY_AGRESSIVENESE_FLEE,
    /* Ship Wont React To Nearby Enemies Will Flee If Attacked Or Enemies Get Too Close */
    ENEMY_AGRESSIVENESE_RETREAT,
    /* Ship Wont React To Nearby Enemies Will Engange If Attacked */
    ENEMY_AGRESSIVENESE_NONE,
    /* Ship Will Persue Any Nearby Enemie */
    ENEMY_AGRESSIVENESE_ATTACK
};

/* Enemy Profession */
enum EnemyProfession {
    ///Scounts For A Certain Task (Enemies, Minerals, etc...)
    ENEMY_PROFESSION_SCOUNTER,
    ///Mines Nearby Asteroids
    ENEMY_PROFESSION_MINER,
    ///Protect Nearby Friendly Ships From Enemies
    ENEMY_PROFESSION_PROTECTOR,
};

/* Enemy Retreat Modes */
enum EnemyRetreat {
    /* Enemy Will If A Enemy Is Nearby */
    ENEMY_RETREAT_ENEMY_NEARBY,
    /* Enemy Will Retreat If Attacked */
    ENEMY_RETREAT_ATTACKED,
    /* Enemy Will Retreat If Damage Is Above 20% */
    ENEMY_RETREAT_LOW_DAMAGE,
    /* Enemy Will Retreat If Damage Is Above 50% */
    ENEMY_RETREAT_MEDIUM_DAMAGE,
    /* Enemy Will Retreat If Damave Is Above 80% */
    ENEMY_RETREAT_HIGH_DAMAGE,
    /* Enemy Will Never Retrat */
    ENEMY_RETREAT_NEVER,
};

/* Enemy Personality */
struct EnemyPersonality {
    EnemyAgressivenese Agressivenese;
    EnemyProfession Profession;
    EnemyRetreat Retreat;
};


/* Enemy Ship */
class EnemyShip : public Ship {
public:
    /* Ship Personality */
    EnemyPersonality Personality;
    /* What The Ship Is Currently Doing */
    EnemyMode Mode;
    /* Work Object Type Target */
    string WorkObjectType;
    
    /* Ship Target (Can Be Space Ship Or A Space Object (Asteroids, etc...)) */
    SpaceObject* Target;

    EnemyShip();

    virtual void LookForJob();
    virtual void ProcessBattle();
    virtual void ProcessMovement();
    virtual void ProcessWork();

    /* Start Mining For The Nearest Valid Target If None Is Valid Start Looking For Nother Place */
    virtual void StartMining(vector<SpaceObject*> Targets);

    virtual void Load() override;
    virtual void Update() override;
};

#endif