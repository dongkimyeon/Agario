#pragma once
#include "Scene.h"
#include "Player.h"
#include "Food.h"
#include "Enemy.h"
#include "Trap.h"
#include "Jumbo.h"


class PlayScene : public Scene {
private:
    std::vector<Food> foods;
    std::vector<Enemy> enemys;
    std::vector<Trap> traps;
    std::vector<Jumbo> jumbos;
    Player player;
    

public:
    PlayScene();
    ~PlayScene();
    void Initialize();
    void Update();
    void LateUpdate();
    void Render(HDC hdc);
};