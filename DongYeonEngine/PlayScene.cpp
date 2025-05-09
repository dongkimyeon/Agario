#pragma once
#include "PlayScene.h"
#include "Input.h"
#include "Time.h"

#define FOOD_SIZE 50
#define ENEMY_SIZE 1
#define JUMBO_SIZE 1
#define TRAP_SIZE 5

extern int endMin = 0;
extern int endSec = 0;
extern int maxSize = 0;
extern int endEatCnt = 0;

std::mt19937 gen(std::random_device{}()); // 시드 초기화
std::uniform_int_distribution<int> widthUid(20, 1580); // 1600 - 20
std::uniform_int_distribution<int> heightUid(20, 780); // 800 - 20

// 겹침 확인 함수 (기존 코드 유지)
bool IsOverlapping(float x, float y, float radius, std::vector<Food>& foods,
    std::vector<Enemy>& enemies, std::vector<Trap>& traps,
    std::vector<Jumbo>& jumbos, std::vector<Player>& players)
{
    // 기존 코드 유지
    for (auto food : foods) {
        float dx = x - food.GetPositionX();
        float dy = y - food.GetPositionY();
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < (radius + food.GetRadius())) {
            return true;
        }
    }
    for (auto enemy : enemies) {
        float dx = x - enemy.GetPositionX();
        float dy = y - enemy.GetPositionY();
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < (radius + enemy.GetRadius())) {
            return true;
        }
    }
    for (auto trap : traps) {
        float dx = x - trap.GetPositionX();
        float dy = y - trap.GetPositionY();
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < (radius + trap.GetRadius())) {
            return true;
        }
    }
    for (auto jumbo : jumbos) {
        float dx = x - jumbo.GetPositionX();
        float dy = y - jumbo.GetPositionY();
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < (radius + jumbo.GetRadius())) {
            return true;
        }
    }
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

// 새로운 위치 생성 함수 (기존 코드 유지)
void SetNonOverlappingPosition(float& x, float& y, float radius, std::vector<Food>& foods,
    std::vector<Enemy>& enemies, std::vector<Trap>& traps,
    std::vector<Jumbo>& jumbos, std::vector<Player>& players)
{
    int maxAttempts = 500;
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
    Reset(); // 생성자에서 Reset 호출
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
    // 추가 초기화가 필요하면 여기에
}

void PlayScene::Reset()
{
    // 모든 벡터 초기화
    foods.clear();
    enemys.clear();
    traps.clear();
    jumbos.clear();
    player.clear();

    // 타이머 및 카운터 초기화
    PlayTime = 0.0f;
    foodSpawnTimer = 0.0f;
    enemySpawnTimer = 0.0f;
    trapSpawnTimer = 0.0f;
    enemySplitTimer = 0.0f;
    trapCnt = TRAP_SIZE;
    enemyCnt = 0;
    eatCnt = 0;

    // 전역 변수 초기화
    endMin = 0;
    endSec = 0;
    maxSize = 0;
    endEatCnt = 0;
    allowEnemySplit = false;
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
        obj.SetId(enemyCnt);
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

void PlayScene::Update()
{
    Scene::Update();

    // S 키로 일시정지 토글
    if (Input::GetKeyDown(eKeyCode::S))
    {
        stopFlag = true; // 멈춤 플래그 활성화
        stopTimer = 0.0f; // 타이머 초기화
    }

    // 멈춤 상태 처리
    if (stopFlag)
    {
        stopTimer += Time::DeltaTime();
        if (stopTimer >= 2.0f) // 2초 경과 시 멈춤 해제
        {
            stopFlag = false;
            stopTimer = 0.0f;
        }
        return; // 멈춤 중에는 나머지 업데이트 로직 실행 안 함
    }
   
    // R 키로 리셋
    if (Input::GetKeyDown(eKeyCode::R))
    {
        Reset();
        return; // 리셋 후 즉시 Update 종료
    }
    if (Input::GetKeyDown(eKeyCode::P))
    {
        printFlag = !printFlag;
    }
    if (Input::GetKeyDown(eKeyCode::Q))
    {
        SceneManager::LoadScene(L"EndScene");
        int minutes = static_cast<int>(PlayTime) / 60;
        int seconds = static_cast<int>(PlayTime) % 60;
        endSec = seconds;
        endMin = minutes;
        endEatCnt = eatCnt;
        Reset();
        return;
    }
    if (Input::GetKeyDown(eKeyCode::S))
    {
        stopFlag = !stopFlag;
    }
    
    // 플레이타임 갱신
    enemySplitTimer += Time::DeltaTime();
    PlayTime += Time::DeltaTime();
    foodSpawnTimer += Time::DeltaTime();
    enemySpawnTimer += Time::DeltaTime();
    trapSpawnTimer += Time::DeltaTime();

    // 5초마다 분열 확률 평가
    static std::uniform_int_distribution<int> splitChance(1, 4); // 25% 확률
    if (enemySplitTimer >= 2.0f)
    {
        allowEnemySplit = (splitChance(gen) == 1); // 25% 확률로 분열 허용
        enemySplitTimer = 0.0f; // 타이머 초기화
        std::cout << "Enemy split chance evaluated: allowEnemySplit = " << allowEnemySplit << std::endl;
    }


    for (int i = 0; i < player.size(); ++i)
    {
        player[i].PlusTime(Time::DeltaTime());
    }
    for (int i = 0; i < enemys.size(); ++i)
    {
        enemys[i].PlusTime(Time::DeltaTime());
    }
    int minutes = static_cast<int>(PlayTime) / 60;
    int seconds = static_cast<int>(PlayTime) % 60;

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

    // 플레이어 <-> 음식 충돌 체크
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
                eatCnt++;
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
    for (auto it = player.begin(); it != player.end();) {
        bool playerErased = false;
        for (auto jt = enemys.begin(); jt != enemys.end();) {
            float dx = jt->GetPositionX() - it->GetPositionX();
            float dy = jt->GetPositionY() - it->GetPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);
            float radiusSum = jt->GetRadius() + it->GetRadius();
            if (distance <= radiusSum && distance > 0.0f) {
                if (it->GetRadius() > jt->GetRadius()) {
                    float deltaRadius = it->GetRadius() + (jt->GetRadius() / 4);
                    it->Setradius(deltaRadius);
                    jt = enemys.erase(jt);
                }
                else if (jt->GetRadius() > it->GetRadius()) {
                    float deltaRadius = jt->GetRadius() + (it->GetRadius() / 4);
                    jt->Setradius(deltaRadius);
                    it = player.erase(it);
                    playerErased = true;
                    if (player.empty()) // 조건 수정
                    {
                        SceneManager::LoadScene(L"EndScene");
                        int minutes = static_cast<int>(PlayTime) / 60;
                        int seconds = static_cast<int>(PlayTime) % 60;
                        endSec = seconds;
                        endMin = minutes;
                        endEatCnt = eatCnt;
                    }
                    break;
                }
                else {
                    ++jt;
                }
            }
            else {
                ++jt;
            }
        }
        if (!playerErased) {
            ++it;
        }
    }

    // 음식 생성
    if (foodSpawnTimer >= 0.5f && foods.size() < FOOD_SIZE)
    {
        Food obj;
        float x, y;
        SetNonOverlappingPosition(x, y, obj.GetRadius(), foods, enemys, traps, jumbos, player);
        obj.SetPosition(x, y);
        foods.push_back(obj);
        foodSpawnTimer = 0.0f;
    }

    // 적 추가
    if (enemySpawnTimer >= 20.0f)
    {
        Enemy obj;
        float x, y;
        SetNonOverlappingPosition(x, y, obj.GetRadius(), foods, enemys, traps, jumbos, player);
        obj.SetPosition(x, y);
        obj.SetId(++enemyCnt);
        obj.SetLeader(true);
        enemys.push_back(obj);
        enemySpawnTimer = 0.0f;
    }

    // 트랩 추가
    if (trapSpawnTimer >= 20.0f && traps.size() < TRAP_SIZE) {
        Trap obj;
        float x, y;
        SetNonOverlappingPosition(x, y, obj.GetRadius(), foods, enemys, traps, jumbos, player);
        obj.SetPosition(x, y);
        traps.push_back(obj);
        trapSpawnTimer = 0.0f;
    }

    // 플레이어 <-> 플레이어 충돌 처리
    if (!player.empty()) {
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
    }
    // 플레이어 합치기
    if (!player.empty() && player.size() > 1)
    {
        auto basePlayer = player.begin();
        for (auto it = std::next(player.begin()); it != player.end();)
        {
            if ((int)it->GetSplitTime() >= 0.5f)
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

                float moveSpeed = it->GetSpeed() * 3 * Time::DeltaTime();
                it->SetPosition(
                    it->GetPositionX() + dx * moveSpeed,
                    it->GetPositionY() + dy * moveSpeed
                );

                float radiusSum = basePlayer->GetRadius() + it->GetRadius();
                if (distance <= radiusSum)
                {
                    float deltaRadius = basePlayer->GetRadius() + it->GetRadius();
                    basePlayer->Setradius(deltaRadius);
                    it = player.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            else
            {
                ++it;
            }
        }
    }

    
    // 적 분열
    std::vector<Enemy> newEnemies;
    for (auto it = enemys.begin(); it != enemys.end(); ++it)
    {
        if (it->GetRadius() > 30.0f && allowEnemySplit)
        {
            allowEnemySplit = false;
            float radius = it->GetRadius();
            if (radius < 25.0f) {
                continue;
            }

            float newRadius = radius / 2.0f;
            it->Setradius(newRadius);

            Enemy newEnemy;
            newEnemy.Setradius(newRadius);
            newEnemy.SetColor(it->GetColor());
            newEnemy.SetSpeed(it->GetSpeed());
            newEnemy.SetId(it->GetId());
            newEnemy.SetLeader(false);

            float x = it->GetPositionX();
            float y = it->GetPositionY();

            // 임의의 방향으로 분리
            float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.1415926535f;
            float dx = std::cos(angle);
            float dy = std::sin(angle);
            float offset = newRadius * 2.0f;

            it->SetPosition(x - dx * offset * 0.5f, y - dy * offset * 0.5f);
            it->OnSplit(-dx, -dy); // 분열 방향 전달
            newEnemy.SetPosition(x + dx * offset * 0.5f, y + dy * offset * 0.5f);
            newEnemy.OnSplit(dx, dy); // 분열 방향 전달

            newEnemies.push_back(newEnemy);
        }
    }
    for (auto& newEnemy : newEnemies) {
        enemys.push_back(newEnemy);
    }

    // 적 합체
    if (!enemys.empty())
    {
        for (auto it = enemys.begin(); it != enemys.end();)
        {
            if (!it->GetLeaderFlag() && it->GetSplitTime() >= 2.0f) 
            {
                // 같은 ID의 리더 찾기
                Enemy* leaderEnemy = nullptr;
                for (auto& enemy : enemys)
                {
                    if (enemy.GetId() == it->GetId() && enemy.GetLeaderFlag())
                    {
                        leaderEnemy = &enemy;
                        break;
                    }
                }

                if (leaderEnemy)
                {
                    float dx = leaderEnemy->GetPositionX() - it->GetPositionX();
                    float dy = leaderEnemy->GetPositionY() - it->GetPositionY();
                    float distance = std::sqrt(dx * dx + dy * dy);

                    if (distance < 0.1f)
                    {
                        ++it;
                        continue;
                    }

                    dx /= distance;
                    dy /= distance;

                    float moveSpeed = it->GetSpeed() * 3 * Time::DeltaTime(); // 플레이어와 동일한 속도
                    it->SetPosition(
                        it->GetPositionX() + dx * moveSpeed,
                        it->GetPositionY() + dy * moveSpeed
                    );

                    float radiusSum = leaderEnemy->GetRadius() + it->GetRadius();
                    if (distance <= radiusSum)
                    {
                        float deltaRadius = leaderEnemy->GetRadius() + it->GetRadius();
                        leaderEnemy->Setradius(deltaRadius);
                        it = enemys.erase(it); // 비리더 제거
                    }
                    else
                    {
                        ++it;
                    }
                }
                else
                {
                    ++it;
                }
            }
            else
            {
                ++it;
            }
        }
    }

    // 적 <-> 적 충돌처리
    if (!enemys.empty())
    {
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
    }

    // 플레이어 업데이트
    for (auto it = player.begin(); it != player.end(); ++it) {
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
        std::cout << "Enemy ID: " << it->GetId() << std::endl;
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

    // 최대 크기 갱신
    for (auto it = player.begin(); it != player.end(); ++it)
    {
        if (maxSize < it->GetRadius())
            maxSize = it->GetRadius();
    }
}

void PlayScene::LateUpdate()
{
    Scene::LateUpdate();
}

#include <gdiplus.h> // GDI+ 헤더 추가
#pragma comment(lib, "gdiplus.lib") // GDI+ 라이브러리 링크

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

void PlayScene::Render(HDC hdc)
{
    // GDI+ Graphics 객체 생성
    Gdiplus::Graphics graphics(hdc);
    if (graphics.GetLastStatus() != Gdiplus::Ok) {
        std::cout << "Failed to create GDI+ Graphics object" << std::endl;
        return;
    }

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
        //camera.Update(&player[0]); // 주석 처리된 카메라 업데이트
    }

    // GDI+ 텍스트 렌더링 준비
    Gdiplus::Font font(L"Arial", 25, Gdiplus::FontStyleBold, Gdiplus::UnitPixel); // 폰트 크기 24로 증가
    if (font.GetLastStatus() != Gdiplus::Ok) {
        std::cout << "Failed to create font" << std::endl;
        return;
    }

    Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255, 255)); // 빨간색 텍스트로 가시성 테스트
    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentNear); // 왼쪽 정렬

    // 반투명 배경 사각형 (가독성 향상)
    Gdiplus::SolidBrush bgBrush(Gdiplus::Color(128, 0, 0, 0)); // 반투명 검은색
    

    // 플레이 시간, 최대 반지름, 먹은 음식 개수 계산
    int minutes = static_cast<int>(PlayTime) / 60;
    int seconds = static_cast<int>(PlayTime) % 60;
    float max = 0;
    for (auto it = player.begin(); it != player.end(); ++it)
    {
        if (it->GetRadius() > max)
            max = it->GetRadius();
    }

    // 텍스트 문자열 생성
    std::wstring timeText = L"Play Time: " + std::to_wstring(minutes) + L"분 " + std::to_wstring(seconds) + L"초";
    std::wstring radiusText = L"Player Radius: " + std::to_wstring((int)max);
    std::wstring eatCntText = L"Player Food Cnt: " + std::to_wstring(eatCnt);

    // 텍스트 렌더링

    if (printFlag)
    {
        graphics.FillRectangle(&bgBrush, 5, 5, 300, 100); // 텍스트 배경
        graphics.DrawString(timeText.c_str(), -1, &font, Gdiplus::PointF(10.0f, 10.0f), &format, &brush);
        graphics.DrawString(radiusText.c_str(), -1, &font, Gdiplus::PointF(10.0f, 40.0f), &format, &brush);
        graphics.DrawString(eatCntText.c_str(), -1, &font, Gdiplus::PointF(10.0f, 70.0f), &format, &brush);
      

    }
    
    Gdiplus::SolidBrush brush2(Gdiplus::Color(255, 0,0 , 0)); //마우스 검은색
    // 마우스 좌표 출력
    std::wstring mouseText = L"X: " + std::to_wstring((int)Input::GetMousePosition().x) +
                             L" Y: " + std::to_wstring((int)Input::GetMousePosition().y);
    graphics.DrawString(mouseText.c_str(), -1, &font, 
                       Gdiplus::PointF(Input::GetMousePosition().x + 10.0f, Input::GetMousePosition().y), 
                       &format, &brush2);
}