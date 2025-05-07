#pragma once
#include "PlayScene.h"
#include "Input.h"
#define ENEMY_SIZE 5
#define TRAR_SIZE 7
#define JUMBO_SIZE 10
#define FOOD_SIZE 50
// 카메라 객체는 필요 시 추가

std::uniform_int_distribution<int> widthUid(0, 1280);
std::uniform_int_distribution<int> heightUid(0, 720);
std::random_device rd;

PlayScene::PlayScene()
{
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
        enemys.push_back(obj); // 적 리스트에 추가 (enemies 벡터가 있다고 가정)
    }

    // 트랩 객체 초기화
    for (int i = 0; i < TRAR_SIZE; ++i)
    {
        Trap obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        traps.push_back(obj); // 트랩 리스트에 추가 (traps 벡터가 있다고 가정)
    }

    // 점보 객체 초기화
    for (int i = 0; i < JUMBO_SIZE; ++i)
    {
        Jumbo obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        jumbos.push_back(obj); // 점보 리스트에 추가 (jumbos 벡터가 있다고 가정)
    }

    // 플레이어 객체 초기화
    player.SetPosition(1280 / 2, 720 / 2);
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
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
        enemys.push_back(obj); // 적 리스트에 추가
    }

    // 트랩 객체 초기화
    for (int i = 0; i < TRAR_SIZE; ++i)
    {
        Trap obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        traps.push_back(obj); // 트랩 리스트에 추가
    }

    // 점보 객체 초기화
    for (int i = 0; i < JUMBO_SIZE; ++i)
    {
        Jumbo obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        jumbos.push_back(obj); // 점보 리스트에 추가
    }

    // 플레이어 객체 초기화
    player.SetPosition(1280 / 2, 720 / 2);
}

void PlayScene::Update()
{
    

    // 플레이어 <-> 음식 충돌 체크
    for (auto it = foods.begin(); it != foods.end();)
    {
        // 중심점 간 거리 계산
        float dx = player.GetPositionX() - it->GetPositionX();
        float dy = player.GetPositionY() - it->GetPositionY();
        float distance = std::sqrt(dx * dx + dy * dy);

        // 반지름 합과 비교
        float radiusSum = player.GetRadius() + it->GetRadius();
        if (distance <= radiusSum)
        {
            float deltaRadius = player.GetRadius() + (it->GetRadius() / 4);
            player.Setradius(deltaRadius);
            it = foods.erase(it); // 충돌한 음식 제거
            std::cout << player.GetRadius() << std::endl;
        }
        else
        {
            ++it;
        }
    }

    //적 <-> 먹이 충돌체크 
    for (auto it = enemys.begin(); it != enemys.end();)
    {
        for (auto jt = foods.begin(); jt != foods.end();)
        {
            float dx = it->GetPositionX() - jt->GetPositionX();
            float dy = it->GetPositionY() - jt->GetPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);

            // 반지름 합과 비교
            float radiusSum = it->GetRadius() + jt->GetRadius();

            if (distance <= radiusSum)
            {
                float deltaRadius = (jt->GetRadius()/4) + it->GetRadius();
                it->Setradius(deltaRadius);
                jt = foods.erase(jt);
                std::cout << it->GetSpeed() << std::endl;
            }
            else
            {
                jt++;
            }
        }
        it++;
    }

    if (foods.size() <= FOOD_SIZE) //50개 고정
    {
        Food obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        foods.push_back(obj);
    }

    //플레이어 업데이트
    player.Update();

    //푸드 업데이트
    for (auto it = foods.begin(); it != foods.end(); ++it)
    {
        it->Update();
    }

    //적 업데이트
    for (auto it = enemys.begin(); it != enemys.end(); ++it)
    {
        it->Update(foods);
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
}

void PlayScene::LateUpdate()
{
    Scene::LateUpdate();
}

void PlayScene::Render(HDC hdc)
{
    // 플레이어 렌더링
    player.Render(hdc);

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

    // 마우스 좌표 출력
    WCHAR Text[100];
    wsprintf(Text, L"X : %d Y : %d", (int)Input::GetMousePosition().x, (int)Input::GetMousePosition().y);
    TextOut(hdc, Input::GetMousePosition().x + 10, Input::GetMousePosition().y, Text, lstrlen(Text));
}