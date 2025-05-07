#pragma once
#include "PlayScene.h"
#include "Input.h"
// 카메라 객체는 필요 시 추가

std::uniform_int_distribution<int> widthUid(0, 1280);
std::uniform_int_distribution<int> heightUid(0, 720);
std::random_device rd;

PlayScene::PlayScene()
{
    // 음식 객체 초기화
    for (int i = 0; i < 30; ++i)
    {
        Food obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        foods.push_back(obj);
    }

    // 적 객체 초기화
    for (int i = 0; i < 5; ++i)
    {
        Enemy obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        enemys.push_back(obj); // 적 리스트에 추가 (enemies 벡터가 있다고 가정)
    }

    // 트랩 객체 초기화
    for (int i = 0; i < 5; ++i)
    {
        Trap obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        traps.push_back(obj); // 트랩 리스트에 추가 (traps 벡터가 있다고 가정)
    }

    // 점보 객체 초기화
    for (int i = 0; i < 10; ++i)
    {
        Jumbo obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        jumbos.push_back(obj); // 점보 리스트에 추가 (jumbos 벡터가 있다고 가정)
    }

    // 플레이어 객체 초기화
    GameObject* obj = new Player;
    obj->SetPosition(widthUid(rd), heightUid(rd));
    player = *(static_cast<Player*>(obj)); // 플레이어 객체 설정 (player가 PlayScene의 멤버 변수라고 가정)
    delete obj; // 동적 할당 해제
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
    // 음식 객체 초기화
    for (int i = 0; i < 30; ++i)
    {
        Food obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        foods.push_back(obj);
    }

    // 적 객체 초기화
    for (int i = 0; i < 5; ++i)
    {
        Enemy obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        enemys.push_back(obj); // 적 리스트에 추가
    }

    // 트랩 객체 초기화
    for (int i = 0; i < 5; ++i)
    {
        Trap obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        traps.push_back(obj); // 트랩 리스트에 추가
    }

    // 점보 객체 초기화
    for (int i = 0; i < 10; ++i)
    {
        Jumbo obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        jumbos.push_back(obj); // 점보 리스트에 추가
    }

    // 플레이어 객체 초기화
    GameObject* obj = new Player;
    obj->SetPosition(widthUid(rd), heightUid(rd));
    player = *(static_cast<Player*>(obj)); // 플레이어 객체 설정
    delete obj; // 동적 할당 해제
}

void PlayScene::Update()
{
    

    //플레이어 <-> 먹이 충돌체크
    RECT playerTempRect = player.GetRect();
    
    for (auto it = foods.begin(); it != foods.end();)
    {
        RECT foodTempRect = it->GetRect();
        //// Food 객체의 RECT 좌표 출력
        //std::cout << "Food Rect: left=" << foodTempRect.left
        //    << ", top=" << foodTempRect.top
        //    << ", right=" << foodTempRect.right
        //    << ", bottom=" << foodTempRect.bottom << std::endl;

        //// 디버깅: 플레이어 RECT 좌표도 함께 출력 (선택 사항)
        //std::cout << "Player Rect: left=" << playerTempRect.left
        //    << ", top=" << playerTempRect.top
        //    << ", right=" << playerTempRect.right
        //    << ", bottom=" << playerTempRect.bottom << std::endl;

        std::cout << player.GetSpeed() << std::endl;
        RECT temp;
        if (IntersectRect(&temp, &playerTempRect, &foodTempRect))
        {
            player.Setradius(player.GetRadius() + 0.05);
            it = foods.erase(it); // 충돌한 음식 제거
        }
        else
        {
            ++it;
        }
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
        it->Update();
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