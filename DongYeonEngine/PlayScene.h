#pragma once
#include "Scene.h"
#include "Player.h"
#include "Food.h"
#include "Enemy.h"
#include "Trap.h"
#include "Jumbo.h"
#include "Camera.h"
#include "SceneManager.h"


class PlayScene : public Scene
{
public:
    PlayScene();
    ~PlayScene();

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(HDC hdc) override;

    void Reset(); // 리셋 메서드 선언

private:
    // 기존 멤버 변수들
    std::vector<Food> foods;
    std::vector<Enemy> enemys;
    std::vector<Trap> traps;
    std::vector<Jumbo> jumbos;
    std::vector<Player> player;

    float PlayTime;
    float foodSpawnTimer;
    float enemySpawnTimer;
    float trapSpawnTimer;
    float stopTimer;
    int trapCnt;
    int enemyCnt;
    int eatCnt;
    bool printFlag = true;
    bool stopFlag = false;
  
};