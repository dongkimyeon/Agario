#pragma once
#include "Scene.h"
#include "Player.h"
#include "Food.h"
#include "Enemy.h"
#include "Trap.h"
#include "Jumbo.h"
#include "Camera.h"
#include "SceneManager.h"


class PlayScene : public Scene {
private:
    std::vector<Food> foods;
    std::vector<Enemy> enemys;
    std::vector<Trap> traps;
    std::vector<Jumbo> jumbos;
    std::vector<Player> player;
	Camera camera;
    float PlayTime;
    float foodSpawnTimer;
    float enemySpawnTimer;
    float trapSpawnTimer;

    int enemyCnt;
    int trapCnt;
    int eatCnt;

public:
    PlayScene();
    ~PlayScene();
    void Initialize();
    void Update();
    void LateUpdate();
    void Render(HDC hdc);
};