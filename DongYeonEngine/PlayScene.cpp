#pragma once
#include "PlayScene.h"
#include "Input.h"
#include "Time.h"

#define FOOD_SIZE 50
#define ENEMY_SIZE 2
#define JUMBO_SIZE 5
#define TRAP_SIZE 5

// 카메라 객체는 필요 시 추가

std::uniform_int_distribution<int> widthUid(0, 1280);
std::uniform_int_distribution<int> heightUid(0, 720);
std::random_device rd;

PlayScene::PlayScene()
{
	PlayTime = 0.0f;
    // 음식 객체 초기화
    for (int i = 0; i < FOOD_SIZE; ++i)
    {
        Food obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        foods.push_back(obj);
    }

    // 적 객체 초기화
    for (int i = 0; i < ENEMY_SIZE; ++i)
    {
        Enemy obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        enemys.push_back(obj);
    }

    // 트랩 객체 초기화
    for (int i = 0; i < TRAP_SIZE; ++i)
    {
        Trap obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        traps.push_back(obj);
    }

    // 점보 객체 초기화
    for (int i = 0; i < JUMBO_SIZE; ++i)
    {
        Jumbo obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        jumbos.push_back(obj);
    }


    // 플레이어 객체 초기화
    Player obj;
    obj.SetPosition(1280/2, 720/2);
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
    //분열 타임 갱신
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
            // 중심점 간 거리 계산
            float dx = jt->GetPositionX() - it->GetPositionX();
            float dy = jt->GetPositionY() - it->GetPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);

            // 반지름 합과 비교
            float radiusSum = jt->GetRadius() + it->GetRadius();
            if (distance <= radiusSum)
            {
                float deltaRadius = it->GetRadius() + (jt->GetRadius()/4);
                it->Setradius(deltaRadius);
                jt = foods.erase(jt); // 충돌한 음식 제거

            }
            else
            {
                ++jt;
            }
        }
        it++;

    }

    //적 <-> 먹이 충돌체크 
    for (auto it = enemys.begin(); it != enemys.end();)
    {
        for (auto jt = foods.begin(); jt != foods.end();)
        {
            // 중심점 간 거리 계산
            float dx = jt->GetPositionX() - it->GetPositionX();
            float dy = jt->GetPositionY() - it->GetPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);

            // 반지름 합과 비교
            float radiusSum = jt->GetRadius() + it->GetRadius();
            if (distance <= radiusSum)
            {
                float deltaRadius = it->GetRadius() + (jt->GetRadius()/4);
                it->Setradius(deltaRadius);
                jt = foods.erase(jt); // 충돌한 음식 제거
               
            }
            else
            {
                ++jt;
            }
        }
        it++;
        
    }

    //50개로 계속 업데이트
    if (foods.size() <= FOOD_SIZE)
    {
        Food obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        foods.push_back(obj);
    }

    
    // 플레이어 분열
    if (Input::GetKeyDown(eKeyCode::LButton) && !player.empty()) {
        POINT mousePos;
        mousePos.x = Input::GetMousePosition().x;
        mousePos.y = Input::GetMousePosition().y;

        std::vector<Player> newPlayers;

        for (auto it = player.begin(); it != player.end(); ++it) {
            float radius = it->GetRadius();

            // 최소 분열 반지름 25
            if (radius < 25.0f) {
                continue;
            }

            float newRadius = radius / 2.0f; // 반지름 반으로
            it->Setradius(newRadius); // 기존 플레이어 반지름 설정 (속도 자동 계산)

            Player newPlayer;
            newPlayer.Setradius(newRadius); // 새 플레이어 반지름 설정 (속도 자동 계산)
            newPlayer.SetColor(it->GetColor()); // 색상 복사
            newPlayer.SetSpeed(it->GetSpeed());
            newPlayer.OnSplit(); // 분열 상태 활성화

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

    //플레이어 합치기
    for (int i = 1; i < player.size(); ++i)
    {
        std::cout << (int)player[i].GetSplitTime() << std::endl;

        if ((int)player[i].GetSplitTime() >= 3 && player.size() > 1) // 2초마다, 플레이어가 2개 이상일 때
        {
            auto basePlayer = player.begin(); // 기준 플레이어 (첫 번째 플레이어)
            for (auto it = std::next(player.begin()); it != player.end();)
            {
                // 기준 플레이어 방향으로 이동
                float dx = basePlayer->GetPositionX() - it->GetPositionX();
                float dy = basePlayer->GetPositionY() - it->GetPositionY();
                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance < 0.1f) // 너무 가까우면 이동 생략
                {
                    ++it;
                    continue;
                }

                // 방향 벡터 정규화
                dx /= distance;
                dy /= distance;

                // 플레이어 속도 기반으로 이동
                float moveSpeed = it->GetSpeed() * Time::DeltaTime();
                it->SetPosition(
                    it->GetPositionX() + dx * moveSpeed,
                    it->GetPositionY() + dy * moveSpeed
                );

                // 충돌 체크
                float radiusSum = basePlayer->GetRadius() + it->GetRadius();
                if (distance <= radiusSum)
                {
                    // 반지름 합치기 (면적 기반 계산)
                    float area1 = basePlayer->GetRadius() * basePlayer->GetRadius();
                    float area2 = it->GetRadius() * it->GetRadius();
                    float newRadius = std::sqrt(area1 + area2);
                    basePlayer->Setradius(newRadius);

                    // 합쳐진 플레이어 제거
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
        if (it->GetRadius() > 70.0f)
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

            float x = it->GetPositionX();
            float y = it->GetPositionY();

            // 기존 적의 방향 벡터 사용
            float dirX = it->GetDirectionX();  // Enemy 클래스에 있어야 함
            float dirY = it->GetDirectionY();

            // 방향 벡터 정규화 (안 되어 있으면)
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

    //플레이어 <-> 플레이어 충돌처리
    for (int i = 0; i < player.size(); ++i) {
        for (int j = i + 1; j < player.size(); ++j) 
        {
            float dx = player[j].GetPositionX() - player[i].GetPositionX();
            float dy = player[j].GetPositionY() - player[i].GetPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);
            float radiusSum = player[i].GetRadius() + player[j].GetRadius();

            if (distance < radiusSum && distance > 0.0f) {
                float overlap = radiusSum - distance;

                // 정규화
                float nx = dx / distance;
                float ny = dy / distance;

                // 밀어내기 - 각 플레이어를 반씩 이동
                float pushX = nx * (overlap / 2.0f);
                float pushY = ny * (overlap / 2.0f);
                
              

                player[i].SetPosition(player[i].GetPositionX() - pushX, player[i].GetPositionY() - pushY);
                player[j].SetPosition(player[j].GetPositionX() + pushX, player[j].GetPositionY() + pushY);
            }
        }
    }
    
    //플레이어 합치기 
     

    //플레이어 업데이트
    int cnt = 0;
    for (auto it = player.begin(); it != player.end(); ++it)
    {
        it->Update();
        std::cout << cnt << "번째 :  " << it->GetSpeed() << std::endl;
        cnt++;
    }
    cnt = 0;
    //푸드 업데이트
    for (auto it = foods.begin(); it != foods.end(); ++it)
    {
        it->Update();
    }

    //적 업데이트
    for (auto it = enemys.begin(); it != enemys.end(); ++it)
    {
        it->Update(foods ,player);
    }

    //트랩 업데이트
    for (auto it = traps.begin(); it != traps.end(); ++it)
    {
        it->Update();
    }

    //점보 업데이트
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
	camera.Render(hdc); // 카메라 렌더링
    

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