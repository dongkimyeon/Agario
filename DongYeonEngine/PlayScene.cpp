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
extern float maxSize = 0;
extern int endEatCnt = 0;



std::mt19937 gen(std::random_device{}()); // 시드 초기화
std::uniform_int_distribution<int> widthUid(20, 1580); // 1600 - 20
std::uniform_int_distribution<int> heightUid(20, 780); // 800 - 20

// 겹침 확인 함수 
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

// 새로운 위치 생성 함수
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
    Reset();
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&mGdiplusToken, &gdiplusStartupInput, nullptr);
}

PlayScene::~PlayScene()
{
    Reset();
}

void PlayScene::Initialize()
{
    mWarnningImage = new Gdiplus::Image(L"resources/warnning.png");
    mWarnningImage2 = new Gdiplus::Image(L"resources/warnning2.png");
    mWarnningImage3 = new Gdiplus::Image(L"resources/warnning3.png");
    mWarnningImage4 = new Gdiplus::Image(L"resources/warnning4.png");
    mWarnningImage5 = new Gdiplus::Image(L"resources/warnning5.png");
    mZoomtogleImage = new Gdiplus::Image(L"resources/zoomtogleKey.png");
    if (mWarnningImage->GetLastStatus() != Gdiplus::Ok) {
        std::cout << "Failed to load warnning.png" << std::endl;
        delete mWarnningImage;
        mWarnningImage = nullptr;
    }
}
void PlayScene::Reset()
{
    // 모든 벡터 초기화
    foods.clear();
    enemys.clear();
    traps.clear();
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
    obj.SetPosition(800, 400);
    player.push_back(obj);


    

    // 카메라 초기화 추가
    if (!player.empty()) {
        mCameraX = player[0].GetPositionX();
        mCameraY = player[0].GetPositionY();
    }
    mZoomScale = 1.0f; // 기본 2배 확대

    mCamType = true;
   

}

void PlayScene::Update()
{
   
    if (mCamType)
    {
        mZoomScale = 1.0f;
    }
    else
    {
        
        if (Input::GetKeyDown(eKeyCode::WheelUp)) {
            mZoomScale += 0.05f; 
            if (mZoomScale > 3.0f) mZoomScale = 3.0f; 
            std::cout << "Zoom In: mZoomScale = " << mZoomScale << std::endl;
        }
        if (Input::GetKeyDown(eKeyCode::WheelDown)) {
            mZoomScale -= 0.05f; // 줌 아웃
            if (mZoomScale < 1.0f) mZoomScale = 1.0f; 
            std::cout << "Zoom Out: mZoomScale = " << mZoomScale << std::endl;
        }
    }
    
    
    
    if (player[0].GetRadius() > maxSize) {
        maxSize = static_cast<int>(player[0].GetRadius());
    }
    // 입력 처리
    if (Input::GetKeyDown(eKeyCode::S)) {
        stopFlag = true;
        stopTimer = 0.0f;
    }
    if (Input::GetKeyDown(eKeyCode::Z)) {
        mCamType = !mCamType;
    }
    if (stopFlag) {
        stopTimer += Time::DeltaTime();
        if (stopTimer >= 2.0f) {
            stopFlag = false;
            stopTimer = 0.0f;
        }
        return;
    }

    if (Input::GetKeyDown(eKeyCode::R) ) {
        Reset();
        return;
    }
    if (Input::GetKeyDown(eKeyCode::P)) {
        printFlag = !printFlag;
    }
    if (Input::GetKeyDown(eKeyCode::Q))
    {
        int minutes = static_cast<int>(PlayTime) / 60;
        int seconds = static_cast<int>(PlayTime) % 60;

        endSec = seconds;
        endMin = minutes;
        endEatCnt = eatCnt;

        SceneManager::LoadScene(L"EndScene");
        Reset();

        return;
    }
    if (Input::GetKeyDown(eKeyCode::S)) {
        stopFlag = !stopFlag;
    }

    // 타이머 및 시간 갱신
    enemySplitTimer += Time::DeltaTime();
    PlayTime += Time::DeltaTime();
    foodSpawnTimer += Time::DeltaTime();
    enemySpawnTimer += Time::DeltaTime();
    trapSpawnTimer += Time::DeltaTime();

    static std::uniform_int_distribution<int> splitChance(1, 4);

    if (enemySplitTimer >= 1.0f) {
        allowEnemySplit = (splitChance(gen) == 1);
        enemySplitTimer = 0.0f;
      
    }

    for (int i = 0; i < player.size(); ++i) {
        player[i].PlusTime(Time::DeltaTime());
    }
    for (int i = 0; i < enemys.size(); ++i) {
        enemys[i].PlusTime(Time::DeltaTime());
    }
    int minutes = static_cast<int>(PlayTime) / 60;
    int seconds = static_cast<int>(PlayTime) % 60;

    if (Input::GetKeyDown(eKeyCode::RButton) && !player.empty()  &&!player[0].GetJumbo())
    {
        std::cout << "먹이 뱉기" << std::endl;
        for (auto& p : player)
        {
            float radius = p.GetRadius();
            if (radius < 15.0f) // 최소 크기 제한
                continue;

            POINT mousePos = { (INT)Input::GetMousePosition().x, (INT)Input::GetMousePosition().y };
            float clientWidth = 1600.0f;
            float clientHeight = 800.0f;
            float worldX = mCameraX + (mousePos.x - clientWidth / 2.0f) / mZoomScale;
            float worldY = mCameraY + (mousePos.y - clientHeight / 2.0f) / mZoomScale;

            // 플레이어에서 마우스 방향 계산
            float x = p.GetPositionX();
            float y = p.GetPositionY();
            float dx = worldX - x;
            float dy = worldY - y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < 0.1f) // 너무 가까우면 무시
                continue;

            // 방향 단위 벡터
            dx /= distance;
            dy /= distance;

            // 새로운 먹이 생성
            Food newFood;
            float foodRadius = 5.0f; // 뱉는 먹이의 크기
            newFood.Setradius(foodRadius);
            newFood.SetPlayerCreate(true); // 뱉어진 먹이로 표시
            newFood.SetMoveTime(0.5f); // 1초 동안 이동

            // 먹이 위치 설정 (플레이어 바깥쪽에서 시작)
            float offset = p.GetRadius() + foodRadius + 3.0f;
            newFood.SetPosition(x + dx * offset, y + dy * offset);

            // 먹이 속도 설정
            float spitSpeed = 300.0f; // 뱉는 속도
            newFood.SetDirection(dx, dy);
            newFood.SetSpeed(spitSpeed);

            // 먹이 추가
            foods.push_back(newFood);

            // 플레이어 크기 감소
            float newRadius = radius - (foodRadius / 2.0f);
            if (newRadius < 10.0f) newRadius = 10.0f; // 최소 크기 보장
            p.Setradius(newRadius);

            std::cout << "먹이 뱉기: 방향 (" << dx << ", " << dy << "), 위치 (" << x + dx * offset << ", " << y + dy * offset << ")" << std::endl;
        }
    }
    // 플레이어 분열
    if (Input::GetKeyDown(eKeyCode::LButton) && !player.empty() && !player[0].GetJumbo()) {
        POINT mousePos = { (INT)Input::GetMousePosition().x, (INT)Input::GetMousePosition().y };
        // 화면 좌표를 월드 좌표로 변환
        float clientWidth = 1600.0f;
        float clientHeight = 800.0f;
        float worldX = mCameraX + (mousePos.x - clientWidth / 2.0f) / mZoomScale;
        float worldY = mCameraY + (mousePos.y - clientHeight / 2.0f) / mZoomScale;

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
            float dx = worldX - x; // 월드 좌표 사용
            float dy = worldY - y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < 0.1f) {
                continue;
            }

            dx /= distance;
            dy /= distance;
            float offset = newRadius * 2.0f;

            newPlayer.SetPosition(x + dx * offset, y + dy * offset);
            it->SetPosition(x, y);

            newPlayers.push_back(newPlayer);
            std::cout << "새 플레이어 분열: (" << x + dx * offset << ", " << y + dy * offset << ")" << std::endl;
        }

        for (auto& newPlayer : newPlayers) {
            player.push_back(newPlayer);
        }
    }

    // 플레이어 합치기
    if (!player.empty() && player.size() > 1) 
    {
        auto basePlayer = player.begin();
        for (auto it = std::next(player.begin()); it != player.end();) 
        {
            if (it->GetSplitTime() >= 0.5f) 
            {
                float dx = basePlayer->GetPositionX() - it->GetPositionX();
                float dy = basePlayer->GetPositionY() - it->GetPositionY();
                float distance = std::sqrt(dx * dx + dy * dy);

               

                dx /= distance;
                dy /= distance;

                float moveSpeed = it->GetSpeed() * 2 * Time::DeltaTime();
                it->SetPosition(
                    it->GetPositionX() + dx * moveSpeed,
                    it->GetPositionY() + dy * moveSpeed
                );

                float radiusSum = basePlayer->GetRadius() + it->GetRadius();
                if (distance <= radiusSum) {
                    float deltaRadius = basePlayer->GetRadius() + it->GetRadius();
                    basePlayer->Setradius(deltaRadius);
                    if (it->GetRadius() > maxSize) {
                        maxSize = static_cast<int>(it->GetRadius());
                    }
                    
                    it = player.erase(it);
                }
                else {
                    ++it;
                }
            }
            else {
                ++it;
            }
        }
    }

    // 플레이어 <-> 트랩 충돌 처리
    for (size_t i = 0; i < player.size(); ++i)
    {
        for (auto jt = traps.begin(); jt != traps.end();)
        {
            float dx = jt->GetPositionX() - player[i].GetPositionX();
            float dy = jt->GetPositionY() - player[i].GetPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);
            float radiusSum = jt->GetRadius() + player[i].GetRadius();

            if (distance <= radiusSum && distance > 0.0f)
            {
                if (jt->GetRadius() > player[i].GetRadius())
                {
                    player[i].SetProtected(true);
                    std::cout << "asd" << std::endl;
                    ++jt;
                }
                else if (jt->GetRadius() < player[i].GetRadius())
                {
                    
                    player[i].SetProtected(false);
                    float radius = player[i].GetRadius();
                    if (radius >= 25.0f)
                    {
                       
                        float newRadius = radius / 4.0f;

                        
                        player[i].Setradius(newRadius);

                        for (int k = 0; k < 3; ++k)
                        {
                            Player newPlayer;
                            newPlayer.Setradius(newRadius);
                            newPlayer.SetColor(player[i].GetColor());
                            newPlayer.SetSpeed(player[i].GetSpeed());
                            newPlayer.OnSplit();

                          
                            float x = player[i].GetPositionX();
                            float y = player[i].GetPositionY();
                           
                            float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.1415926535f;
                            float dx = std::cos(angle);
                            float dy = std::sin(angle);
                            float offset = newRadius * 2.0f;

                            if (k == 0)
                            {
                                player[i].SetPosition(x - dx * offset * 0.5f, y - dy * offset * 0.5f);
                                newPlayer.SetPosition(x + dx * offset * 0.5f, y + dy * offset * 0.5f);
                            }
                            else
                            {
                               
                                angle += (k * 2.0f * 3.1415926535f / 3.0f);
                                dx = std::cos(angle);
                                dy = std::sin(angle);
                                newPlayer.SetPosition(x + dx * offset * 2.0f, y + dy * offset * 2.0f);
                            }

                            player.push_back(newPlayer);
                        }
                    }

                    jt = traps.erase(jt);
                }
            }
            else
            {
                player[i].SetProtected(false);
                ++jt;
            }
        }
    }
    // 플레이어 <-> 음식 충돌 체크
    for (auto it = player.begin(); it != player.end();) {
        for (auto jt = foods.begin(); jt != foods.end();) {
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
                if (it->GetRadius() > maxSize) {
                    maxSize = static_cast<int>(it->GetRadius());
                }
            }
            else {
                ++jt;
            }
        }
        it++;
    }

    //플레이어 <-> 적 충돌 체크
    for (auto it = player.begin(); it != player.end();) {
        bool playerErased = false;
        for (auto jt = enemys.begin(); jt != enemys.end();) {
            float dx = jt->GetPositionX() - it->GetPositionX();
            float dy = jt->GetPositionY() - it->GetPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);
            float radiusSum = jt->GetRadius() + it->GetRadius();
            if (distance <= radiusSum && distance > 0.0f) 
            {
                if (it->GetProtected()) {
                  
                    ++jt;
                    continue;
                }
                if (it->GetRadius() > jt->GetRadius()) {
                    float deltaRadius = it->GetRadius() + (jt->GetRadius() / 4);
                    it->Setradius(deltaRadius);
                    jt = enemys.erase(jt);
                    if (it->GetRadius() > maxSize) {
                        maxSize = static_cast<int>(it->GetRadius());
                    }

                }
                else if (jt->GetRadius() > it->GetRadius()) {
                    float deltaRadius = jt->GetRadius() + (it->GetRadius() / 4);
                    jt->Setradius(deltaRadius);
                    it = player.erase(it);
                    playerErased = true;
                    if (player.empty())
                    {
                        int minutes = static_cast<int>(PlayTime) / 60;
                        int seconds = static_cast<int>(PlayTime) % 60;

                        endSec = seconds;
                        endMin = minutes;
                        endEatCnt = eatCnt;

                        SceneManager::LoadScene(L"EndScene");
                        Reset();

                        return;
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

    // 적 <-> 음식 충돌 체크
    for (auto it = enemys.begin(); it != enemys.end();) {
        for (auto jt = foods.begin(); jt != foods.end();) {
            float dx = jt->GetPositionX() - it->GetPositionX();
            float dy = jt->GetPositionY() - it->GetPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);

            float radiusSum = jt->GetRadius() + it->GetRadius();
            if (distance <= radiusSum) {
                float deltaRadius = it->GetRadius() + (jt->GetRadius() / 4);
                it->Setradius(deltaRadius);
                jt = foods.erase(jt);
            }
            else {
                ++jt;
            }
        }
        it++;
    }

    // 적 분열
    std::vector<Enemy> newEnemies;
    for (auto it = enemys.begin(); it != enemys.end(); ++it) {
        if (it->GetRadius() > 20.0f && allowEnemySplit) {
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

            float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.1415926535f;
            float dx = std::cos(angle);
            float dy = std::sin(angle);
            float offset = newRadius * 2.0f;

            it->SetPosition(x - dx * offset * 0.5f, y - dy * offset * 0.5f);
            it->OnSplit(-dx, -dy);
            newEnemy.SetPosition(x + dx * offset * 0.5f, y + dy * offset * 0.5f);
            newEnemy.OnSplit(dx, dy);

            newEnemies.push_back(newEnemy);
        }
    }
    for (auto& newEnemy : newEnemies) {
        enemys.push_back(newEnemy);
    }

    // 적 합체
    if (!enemys.empty()) {
        for (auto it = enemys.begin(); it != enemys.end();) {
            if (!it->GetLeaderFlag() && it->GetSplitTime() >= 2.0f) {
                Enemy* leaderEnemy = nullptr;
                for (auto& enemy : enemys) {
                    if (enemy.GetId() == it->GetId() && enemy.GetLeaderFlag()) {
                        leaderEnemy = &enemy;
                        break;
                    }
                }

                if (leaderEnemy) {
                    float dx = leaderEnemy->GetPositionX() - it->GetPositionX();
                    float dy = leaderEnemy->GetPositionY() - it->GetPositionY();
                    float distance = std::sqrt(dx * dx + dy * dy);

                    if (distance < 0.1f) {
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

                    float radiusSum = leaderEnemy->GetRadius() + it->GetRadius();
                    if (distance <= radiusSum) {
                        float deltaRadius = leaderEnemy->GetRadius() + it->GetRadius();
                        leaderEnemy->Setradius(deltaRadius);
                        it = enemys.erase(it);
                    }
                    else {
                        ++it;
                    }
                }
                else {
                    ++it;
                }
            }
            else {
                ++it;
            }
        }
    }

    // 음식 생성
    if (foodSpawnTimer >= 0.5f ) {
        Food obj;
        float x, y;
        SetNonOverlappingPosition(x, y, obj.GetRadius(), foods, enemys, traps, jumbos, player);
        obj.SetPosition(x, y);
        foods.push_back(obj);
        foodSpawnTimer = 0.0f;
    }

    // 적 추가
    if (enemySpawnTimer >= 20.0f) {
        Enemy obj;
        float x, y;
        SetNonOverlappingPosition(x, y, obj.GetRadius(), foods, enemys, traps, jumbos, player);
        obj.SetPosition(x, y);
        obj.SetId(++enemyCnt);
        obj.SetLeader(true);
        enemys.push_back(obj);
        enemySpawnTimer = 0.0f;
    }

    // 트랩 추가 및 느낌표 위치 설정
    if (trapSpawnTimer >= 8.0f && !isExclamationPositionSet) {
        Trap tempObj;
        exclamationX = static_cast<float>(widthUid(gen));
        exclamationY = static_cast<float>(heightUid(gen));
        showExclamation = true;
        isExclamationPositionSet = true;
    }
    if (trapSpawnTimer >= 10.0f) {
        Trap obj;
        obj.SetPosition(exclamationX, exclamationY);
        traps.push_back(obj);
        trapSpawnTimer = 0.0f;
        showExclamation = false;

        isExclamationPositionSet = false;
    }


 

    // 플레이어 <-> 점보 충돌 처리
    if (!player.empty())
    {
        for (int i = 0; i < player.size(); ++i)
        {
            // 점보 상태 해제 시 점보 생성
            if (player[i].ShouldSpawnJumbo())
            {
                Jumbo newJumbo;
                newJumbo.SetPosition(player[i].GetPositionX(), player[i].GetPositionY());
                jumbos.push_back(newJumbo);
                player[i].ResetSpawnJumbo(); // 생성 플래그 리셋
            }

            // 점보 충돌 처리
            for (auto it = jumbos.begin(); it != jumbos.end();)
            {
                if (it->IsInvincible()) // 무적 상태면 충돌 무시
                {
                    ++it;
                    continue;
                }

                float dx = it->GetPositionX() - player[0].GetPositionX();
                float dy = it->GetPositionY() - player[0].GetPositionY();
                float distance = std::sqrt(dx * dx + dy * dy);
                float radiusSum = it->GetRadius() + player[0].GetRadius();

                if (distance <= radiusSum && distance > 0.0f)
                {
                    player[0].SetJumbo(true);
                    it = jumbos.erase(it); // 점보 제거
                }
                else
                {
                    ++it;
                }
            }

            // 2. 점보 상태 플레이어가 음식 끌어당기기
            if (player[0].GetJumbo())
            {
                const float attractionRange = 200.0f;
                const float attractionSpeed = 100.0f;

                for (auto& food : foods)
                {
                    float dx = player[0].GetPositionX() - food.GetPositionX();
                    float dy = player[0].GetPositionY() - food.GetPositionY();
                    float distance = std::sqrt(dx * dx + dy * dy);

                    if (distance < attractionRange && distance > 0.0f)
                    {
                        dx /= distance;
                        dy /= distance;
                        food.SetDirection(dx, dy);
                        food.SetSpeed(attractionSpeed);
                    }
                }

            }
        }
    }

    // 플레이어 <-> 플레이어 처리
    if (!player.empty()) {
        for (int i = 1; i < player.size(); ++i) {
            for (int j = i + 1; j < player.size(); ++j) {
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
	
    //  적 <-> 적 충돌 처리
    if (!enemys.empty()) {
        for (int i = 0; i < enemys.size(); ++i) {
            for (int j = i + 1; j < enemys.size(); ++j) {
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

   
    for (auto it = player.begin(); it != player.end(); ++it) {
        it->Update(mZoomScale, mCameraX, mCameraY, 1600, 800);
    }


    for (auto it = foods.begin(); it != foods.end(); ++it) {
        it->Update();
    }


    for (auto it = enemys.begin(); it != enemys.end(); ++it) {
        it->Update(foods, player, enemys);
    }


    for (auto it = traps.begin(); it != traps.end(); ++it) {
        it->Update();
    }


    for (auto it = jumbos.begin(); it != jumbos.end(); ++it) {
        it->Update(player);
    }


    if (!player.empty()) {
        mCameraX = player[0].GetPositionX();
        mCameraY = player[0].GetPositionY();

        
        float clientWidth = 1600.0f;
        float clientHeight = 800.0f;
        float worldWidth = 1600.0f;
        float worldHeight = 800.0f;
        float scaledWidth = clientWidth / mZoomScale;
        float scaledHeight = clientHeight / mZoomScale;

        // mCameraX 제한
        if (mCameraX < scaledWidth / 2.0f) {
            mCameraX = scaledWidth / 2.0f;
        }
        else if (mCameraX > worldWidth - scaledWidth / 2.0f) {
            mCameraX = worldWidth - scaledWidth / 2.0f;
        }

        // mCameraY 제한
        if (mCameraY < scaledHeight / 2.0f) {
            mCameraY = scaledHeight / 2.0f;
        }
        else if (mCameraY > worldHeight - scaledHeight / 2.0f) {
            mCameraY = worldHeight - scaledHeight / 2.0f;
        }
    }
}

void PlayScene::LateUpdate()
{
    Scene::LateUpdate();
}

void PlayScene::Render(HDC hdc)
{
    Gdiplus::Graphics graphics(hdc);
    
    if (graphics.GetLastStatus() != Gdiplus::Ok) {
        std::cout << "Failed to create GDI+ Graphics object" << std::endl;
        return;
    }

    // 카메라 변환 설정
    float clientWidth = 1600.0f;
    float clientHeight = 800.0f;
    Gdiplus::Matrix transform;
    // 1. 화면 중심으로 이동
    transform.Translate(clientWidth / 2.0f, clientHeight / 2.0f);
    // 2. 줌 적용
    transform.Scale(mZoomScale, mZoomScale);
    // 3. 카메라 위치로 이동
    transform.Translate(-mCameraX, -mCameraY);
    graphics.SetTransform(&transform);

    // 격자 배경 그리기
    Gdiplus::Pen gridPen(Gdiplus::Color(255, 122, 122, 122), 1.0f);
    const float gridSize = 50.0f;
    const float worldWidth = 1600.0f;
    const float worldHeight = 800.0f;

    for (float x = 0; x <= worldWidth; x += gridSize) {
        graphics.DrawLine(&gridPen, x, 0.0f, x, worldHeight);
    }
    for (float y = 0; y <= worldHeight; y += gridSize) {
        graphics.DrawLine(&gridPen, 0.0f, y, worldWidth, y);
    }

    // 느낌표 이미지 렌더링
    INT size = 100;
    if (showExclamation && trapSpawnTimer >= 8.0f && trapSpawnTimer <= 8.4f) {
        Gdiplus::RectF rect(exclamationX - 25, exclamationY - 25, size, size);
        graphics.DrawImage(mWarnningImage, rect);
    }
    if (showExclamation && trapSpawnTimer > 8.4f && trapSpawnTimer <= 8.8f) {
        Gdiplus::RectF rect(exclamationX - 25, exclamationY - 25, size, size);
        graphics.DrawImage(mWarnningImage2, rect);
    }
    if (showExclamation && trapSpawnTimer > 8.8f && trapSpawnTimer <= 9.2f) {
        Gdiplus::RectF rect(exclamationX - 25, exclamationY - 25, size, size);
        graphics.DrawImage(mWarnningImage3, rect);
    }
    if (showExclamation && trapSpawnTimer > 9.2f && trapSpawnTimer <= 9.6f) {
        Gdiplus::RectF rect(exclamationX - 25, exclamationY - 25, size, size);
        graphics.DrawImage(mWarnningImage4, rect);
    }
    if (showExclamation && trapSpawnTimer > 9.6f && trapSpawnTimer <= 10.0f) {
        Gdiplus::RectF rect(exclamationX - 25, exclamationY - 25, size, size);
        graphics.DrawImage(mWarnningImage5, rect);
    }

    // 오브젝트 렌더링
    for (auto& p : player) {
        p.Render(graphics);
    }
    for (auto& f : foods) {
        f.Render(graphics);
    }
    for (auto& e : enemys) {
        e.Render(graphics);
    }
    for (auto& t : traps) {
        t.Render(graphics);
    }
    for (auto& j : jumbos) {
        j.Render(graphics);
    }

    // 텍스트 렌더링 (카메라 변환 해제)
    graphics.ResetTransform();
    Gdiplus::Font font(L"Arial", 25, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    if (font.GetLastStatus() != Gdiplus::Ok) {
        std::cout << "Failed to create font" << std::endl;
        return;
    }

    graphics.ResetTransform();
    if (mZoomtogleImage) {
        Gdiplus::RectF rect(1550.0f, 0, 50.0f, 50.0f); // 우측 상단
        graphics.DrawImage(mZoomtogleImage, rect);
    }

   
    Gdiplus::Font infoText(L"Arial", 15, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush infoBrush(Gdiplus::Color(255, 0, 0, 0));
    Gdiplus::StringFormat infoFormat;
    infoFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
    std::wstring toggleText = mCamType ? L"고정 탑 뷰" : L"줌 인/아웃 가능";
    graphics.DrawString(toggleText.c_str(), -1, &infoText, Gdiplus::PointF(1480.0f, 15.0f), &infoFormat, &infoBrush);


    Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255, 255));
    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentNear);

    Gdiplus::SolidBrush bgBrush(Gdiplus::Color(128, 0, 0, 0));

    int minutes = static_cast<int>(PlayTime) / 60;
    int seconds = static_cast<int>(PlayTime) % 60;

    float max = 0;
    for (auto& p : player) {
        if (p.GetRadius() > max) max = p.GetRadius();
    }

    wchar_t timeBuffer[50];
    swprintf(timeBuffer, 50, L"Play Time: %d분 %d초", minutes, seconds);
    std::wstring timeText = timeBuffer;

    wchar_t radiusBuffer[50];
    swprintf(radiusBuffer, 50, L"Player Radius: %.1f", max);
    std::wstring radiusText = radiusBuffer;

    wchar_t eatCntBuffer[50];
    swprintf(eatCntBuffer, 50, L"Player Food Cnt: %d", eatCnt);
    std::wstring eatCntText = eatCntBuffer;

    Time::Render(hdc);
    if (printFlag) {
        graphics.FillRectangle(&bgBrush, 5, 5, 300, 100);
        graphics.DrawString(timeText.c_str(), -1, &font, Gdiplus::PointF(10.0f, 10.0f), &format, &brush);
        graphics.DrawString(radiusText.c_str(), -1, &font, Gdiplus::PointF(10.0f, 40.0f), &format, &brush);
        graphics.DrawString(eatCntText.c_str(), -1, &font, Gdiplus::PointF(10.0f, 70.0f), &format, &brush);
    }
}