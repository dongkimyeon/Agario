#pragma once
#include "PlayScene.h"
#include "Input.h"
#include "Time.h"

#define FOOD_SIZE 50
#define ENEMY_SIZE 1
#define JUMBO_SIZE 1
#define TRAP_SIZE 5

std::mt19937 gen(std::random_device{}()); // 시드 초기화
std::uniform_int_distribution<int> widthUid(20, 1580); // 1600 - 20
std::uniform_int_distribution<int> heightUid(20, 780); // 800 - 20

// 겹침 확인 함수
bool IsOverlapping(float x, float y, float radius, std::vector<Food>& foods,
    std::vector<Enemy>& enemies, std::vector<Trap>& traps,
    std::vector<Jumbo>& jumbos, std::vector<Player>& players)
{
    // Food와 겹침 확인
    for (auto food : foods) {
        float dx = x - food.GetPositionX();
        float dy = y - food.GetPositionY();
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < (radius + food.GetRadius())) {
            return true;
        }
    }

    // Enemy와 겹침 확인
    for (auto enemy : enemies) {
        float dx = x - enemy.GetPositionX();
        float dy = y - enemy.GetPositionY();
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < (radius + enemy.GetRadius())) {
            return true;
        }
    }

    // Trap과 겹침 확인
    for (auto trap : traps) {
        float dx = x - trap.GetPositionX();
        float dy = y - trap.GetPositionY();
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < (radius + trap.GetRadius())) {
            return true;
        }
    }

    // Jumbo와 겹침 확인
    for (auto jumbo : jumbos) {
        float dx = x - jumbo.GetPositionX();
        float dy = y - jumbo.GetPositionY();
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < (radius + jumbo.GetRadius())) {
            return true;
        }
    }

    // Player와 겹침 확인
    for (auto player : players) {
        float dx = x - player.GetPositionX();
        float dy = y - player.GetPositionY();
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < (radius + player.GetRadius())) {
            return true;
        }
    }

    return false;
}

// 새로운 위치 생성 함수
void SetNonOverlappingPosition(float& x, float& y, float radius, std::vector<Food>& foods,
    std::vector<Enemy>& enemies, std::vector<Trap>& traps,
    std::vector<Jumbo>& jumbos, std::vector<Player>& players)
{
    int maxAttempts = 500; // 최대 시도 횟수 증가
    for (int i = 0; i < maxAttempts; ++i) {
        x = static_cast<float>(widthUid(gen));
        y = static_cast<float>(heightUid(gen));
        if (!IsOverlapping(x, y, radius, foods, enemies, traps, jumbos, players)) {
            return;
        }
    }
}

PlayScene::PlayScene()
{
    PlayTime = 0.0f;
    foodSpawnTimer = 0.0f;
    enemySpawnTimer = 0.0f;
    // 음식 객체 초기화
    while (foods.size() < FOOD_SIZE)
    {
        Food obj;
        float x, y;
        SetNonOverlappingPosition(x, y, obj.GetRadius(), foods, enemys, traps, jumbos, player);
        obj.SetPosition(x, y);
        foods.push_back(obj);
    }

    // 적 객체 초기화
    while (enemys.size() < ENEMY_SIZE)
    {
        Enemy obj;
        float x, y;
        SetNonOverlappingPosition(x, y, obj.GetRadius(), foods, enemys, traps, jumbos, player);
        obj.SetPosition(x, y);
        obj.SetId(enemys.size());
        obj.SetLeader(true);
        enemys.push_back(obj);
    }

    // 트랩 객체 초기화
    while (traps.size() < TRAP_SIZE)
    {
        Trap obj;
        float x, y;
        SetNonOverlappingPosition(x, y, obj.GetRadius(), foods, enemys, traps, jumbos, player);
        obj.SetPosition(x, y);
        traps.push_back(obj);
    }

    // 점보 객체 초기화
    while (jumbos.size() < JUMBO_SIZE)
    {
        Jumbo obj;
        float x, y;
        SetNonOverlappingPosition(x, y, obj.GetRadius(), foods, enemys, traps, jumbos, player);
        obj.SetPosition(x, y);
        jumbos.push_back(obj);
    }

    // 플레이어 객체 초기화
    Player obj;
    float x, y;
    SetNonOverlappingPosition(x, y, obj.GetRadius(), foods, enemys, traps, jumbos, player);
    obj.SetPosition(x, y);
    player.push_back(obj);
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
}

void PlayScene::Update()
{
    // 플레이타임 갱신
    PlayTime += Time::DeltaTime();
    foodSpawnTimer += Time::DeltaTime();
    enemySpawnTimer += Time::DeltaTime();
    // 분열 타임 갱신
    for (int i = 0; i < player.size(); ++i)
    {
        player[i].PlusTime(Time::DeltaTime());
    }

    int minutes = static_cast<int>(PlayTime) / 60;
    int seconds = static_cast<int>(PlayTime) % 60;

    // 플레이어 <-> 음식 충돌 체크 (원 기반)
    for (auto it = player.begin(); it != player.end();)
    {
        for (auto jt = foods.begin(); jt != foods.end();)
        {
            float dx = jt->GetPositionX() - it->GetPositionX();
            float dy = jt->GetPositionY() - it->GetPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);

            float radiusSum = jt->GetRadius() + it->GetRadius();
            if (distance <= radiusSum)
            {
                float deltaRadius = it->GetRadius() + (jt->GetRadius() / 4);
                it->Setradius(deltaRadius);
                jt = foods.erase(jt);
            }
            else
            {
                ++jt;
            }
        }
        it++;
    }

    // 적 <-> 먹이 충돌 체크
    for (auto it = enemys.begin(); it != enemys.end();)
    {
        for (auto jt = foods.begin(); jt != foods.end();)
        {
            float dx = jt->GetPositionX() - it->GetPositionX();
            float dy = jt->GetPositionY() - it->GetPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);

            float radiusSum = jt->GetRadius() + it->GetRadius();
            if (distance <= radiusSum)
            {
                float deltaRadius = it->GetRadius() + (jt->GetRadius() / 4);
                it->Setradius(deltaRadius);
                jt = foods.erase(jt);
            }
            else
            {
                ++jt;
            }
        }
        it++;
    }

    // 플레이어 <-> 적 충돌 체크
    std::vector<Player*> playersToRemove;
    for (auto& p : player) {
        for (auto& e : enemys) {
            float dx = e.GetPositionX() - p.GetPositionX();
            float dy = e.GetPositionY() - p.GetPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);
            float radiusSum = e.GetRadius() + p.GetRadius();
            if (distance <= radiusSum && e.GetRadius() > p.GetRadius()) {
                playersToRemove.push_back(&p);
            }
        }
    }
    for (auto* p : playersToRemove) {
        player.erase(std::find_if(player.begin(), player.end(),
            [p](const Player& pl) { return &pl == p; }));
    }

    // 1초마다 음식 1개 생성
    if (foodSpawnTimer >= 1.0f && foods.size() < FOOD_SIZE)
    {
        Food obj;
        float x, y;
        SetNonOverlappingPosition(x, y, obj.GetRadius(), foods, enemys, traps, jumbos, player);
        obj.SetPosition(x, y);
        foods.push_back(obj);
        foodSpawnTimer = 0.0f; // 타이머 리셋
    }
    // 20초마다 적 추가
    if (enemySpawnTimer >= 20.0f)
    {
        Enemy obj;
        float x, y;
        SetNonOverlappingPosition(x, y, obj.GetRadius(), foods, enemys, traps, jumbos, player);
        obj.SetPosition(x, y);
        obj.SetLeader(true);
        enemys.push_back(obj);
        enemySpawnTimer = 0.0f; // 타이머 리셋
    }

    // 플레이어 분열
    if (Input::GetKeyDown(eKeyCode::LButton) && !player.empty()) {
        POINT mousePos;
        mousePos.x = Input::GetMousePosition().x;
        mousePos.y = Input::GetMousePosition().y;

        std::vector<Player> newPlayers;

        for (auto it = player.begin(); it != player.end(); ++it) {
            float radius = it->GetRadius();

            if (radius < 25.0f) {
                continue;
            }

            float newRadius = radius / 2.0f;
            it->Setradius(newRadius);

            Player newPlayer;
            newPlayer.Setradius(newRadius);
            newPlayer.SetColor(it->GetColor());
            newPlayer.SetSpeed(it->GetSpeed());
            newPlayer.OnSplit();

            float x = it->GetPositionX();
            float y = it->GetPositionY();
            float dx = static_cast<float>(mousePos.x) - x;
            float dy = static_cast<float>(mousePos.y) - y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < 0.1f) {
                continue;
            }

            dx /= distance;
            dy /= distance;
            float offset = newRadius * 2.0f;

            it->SetPosition(x - dx * offset * 0.5f, y - dy * offset * 0.5f);
            newPlayer.SetPosition(x + dx * offset * 0.5f, y + dy * offset * 0.5f);

            newPlayers.push_back(newPlayer);
        }

        for (auto& newPlayer : newPlayers) {
            player.push_back(newPlayer);
        }
    }

    // 플레이어 합치기
    for (int i = 1; i < player.size(); ++i)
    {
        if ((int)player[i].GetSplitTime() >= 3 && player.size() > 1)
        {
            auto basePlayer = player.begin();
            for (auto it = std::next(player.begin()); it != player.end();)
            {
                float dx = basePlayer->GetPositionX() - it->GetPositionX();
                float dy = basePlayer->GetPositionY() - it->GetPositionY();
                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance < 0.1f)
                {
                    ++it;
                    continue;
                }

                dx /= distance;
                dy /= distance;

                float moveSpeed = it->GetSpeed() * 2 * Time::DeltaTime();
                it->SetPosition(
                    it->GetPositionX() + dx * moveSpeed,
                    it->GetPositionY() + dy * moveSpeed
                );

                float radiusSum = basePlayer->GetRadius() + it->GetRadius();
                if (distance <= radiusSum)
                {
                    float deltaRadius = basePlayer->GetRadius() + (it->GetRadius());
                    basePlayer->Setradius(deltaRadius);
                    it = player.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
    }

    // 적 분열
    std::vector<Enemy> newEnemies;
    for (auto it = enemys.begin(); it != enemys.end(); ++it)
    {
        if (it->GetRadius() > 50.0f)
        {
            float radius = it->GetRadius();

            if (radius < 25.0f) {
                continue;
            }

            float newRadius = radius / 2.0f;
            it->Setradius(newRadius);

            Enemy newEnemy;
            newEnemy.Setradius(newRadius);
            newEnemy.SetColor(it->GetColor());
            newEnemy.OnSplit();
            newEnemy.SetLeader(false);
            newEnemy.SetId(it->GetId());
            float x = it->GetPositionX();
            float y = it->GetPositionY();

            float dirX = it->GetDirectionX();
            float dirY = it->GetDirectionY();

            float length = std::sqrt(dirX * dirX + dirY * dirY);
            if (length != 0.0f) {
                dirX /= length;
                dirY /= length;
            }

            float offset = newRadius * 2.0f;

            it->SetPosition(x - dirX * offset * 0.5f, y - dirY * offset * 0.5f);
            newEnemy.SetPosition(x + dirX * offset * 0.5f, y + dirY * offset * 0.5f);

            newEnemies.push_back(newEnemy);
        }
    }
    for (auto& newEnemy : newEnemies) {
        enemys.push_back(newEnemy);
    }

    // 플레이어 <-> 플레이어 충돌 처리
    for (int i = 0; i < player.size(); ++i) {
        for (int j = i + 1; j < player.size(); ++j)
        {
            float dx = player[j].GetPositionX() - player[i].GetPositionX();
            float dy = player[j].GetPositionY() - player[i].GetPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);
            float radiusSum = player[i].GetRadius() + player[j].GetRadius();

            if (distance < radiusSum && distance > 0.0f) {
                float overlap = radiusSum - distance;

                float nx = dx / distance;
                float ny = dy / distance;

                float pushX = nx * (overlap / 2.0f);
                float pushY = ny * (overlap / 2.0f);

                player[i].SetPosition(player[i].GetPositionX() - pushX, player[i].GetPositionY() - pushY);
                player[j].SetPosition(player[j].GetPositionX() + pushX, player[j].GetPositionY() + pushY);
            }
        }
    }

    // 적 <-> 적 충돌처리
    for (int i = 0; i < enemys.size(); ++i) {
        for (int j = i + 1; j < enemys.size(); ++j)
        {
            float dx = enemys[j].GetPositionX() - enemys[i].GetPositionX();
            float dy = enemys[j].GetPositionY() - enemys[i].GetPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);
            float radiusSum = enemys[i].GetRadius() + enemys[j].GetRadius();

            if (distance < radiusSum && distance > 0.0f) {
                float overlap = radiusSum - distance;

                float nx = dx / distance;
                float ny = dy / distance;

                float pushX = nx * (overlap / 2.0f);
                float pushY = ny * (overlap / 2.0f);

                enemys[i].SetPosition(enemys[i].GetPositionX() - pushX, enemys[i].GetPositionY() - pushY);
                enemys[j].SetPosition(enemys[j].GetPositionX() + pushX, enemys[j].GetPositionY() + pushY);
            }
        }
    }

    // 플레이어 업데이트
    for (auto it = player.begin(); it != player.end(); ++it)
    {
        it->Update();
    }

    // 푸드 업데이트
    for (auto it = foods.begin(); it != foods.end(); ++it)
    {
        it->Update();
    }

    // 적 업데이트
    for (auto it = enemys.begin(); it != enemys.end(); ++it)
    {
        it->Update(foods, player, enemys);
    }

    // 트랩 업데이트
    for (auto it = traps.begin(); it != traps.end(); ++it)
    {
        it->Update();
    }

    // 점보 업데이트
    for (auto it = jumbos.begin(); it != jumbos.end(); ++it)
    {
        it->Update();
    }

    camera.Update(&player[0]); // 카메라 업데이트 (첫 번째 플레이어를 기준으로)
}

void PlayScene::LateUpdate()
{
    Scene::LateUpdate();
}

void PlayScene::Render(HDC hdc)
{
    // 플레이어 렌더링
    for (auto it = player.begin(); it != player.end(); ++it)
    {
        it->Render(hdc);
    }

    // 음식 객체 렌더링
    for (auto it = foods.begin(); it != foods.end(); ++it)
    {
        it->Render(hdc);
    }

    // 적 객체 렌더링
    for (auto it = enemys.begin(); it != enemys.end(); ++it)
    {
        it->Render(hdc);
    }

    // 트랩 객체 렌더링
    for (auto it = traps.begin(); it != traps.end(); ++it)
    {
        it->Render(hdc);
    }

    // 점보 객체 렌더링
    for (auto it = jumbos.begin(); it != jumbos.end(); ++it)
    {
        it->Render(hdc);
    }

    if (!player.empty()) {
        camera.Update(&player[0]);
    }

    // 마우스 좌표 출력
    WCHAR Text[100];
    wsprintf(Text, L"X : %d Y : %d", (int)Input::GetMousePosition().x, (int)Input::GetMousePosition().y);
    TextOut(hdc, Input::GetMousePosition().x + 10, Input::GetMousePosition().y, Text, lstrlen(Text));

    WCHAR TimeText[100];
    int minutes = static_cast<int>(PlayTime) / 60;
    int seconds = static_cast<int>(PlayTime) % 60;

    wsprintf(TimeText, L"Play Time: %02d분 : %02d초", minutes, seconds);
    TextOut(hdc, 10, 10, TimeText, lstrlen(TimeText));
}