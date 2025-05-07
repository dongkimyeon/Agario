#pragma once
#include "PlayScene.h"
#include "Input.h"
#include "Camera.h"

// ī�޶� ��ü�� �ʿ� �� �߰�

std::uniform_int_distribution<int> widthUid(0, 1280);
std::uniform_int_distribution<int> heightUid(0, 720);
std::random_device rd;

PlayScene::PlayScene()
{
    // ���� ��ü �ʱ�ȭ
    for (int i = 0; i < 30; ++i)
    {
        Food obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        foods.push_back(obj);
    }

    // �� ��ü �ʱ�ȭ
    for (int i = 0; i < 5; ++i)
    {
        Enemy obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        enemys.push_back(obj); // �� ����Ʈ�� �߰� (enemies ���Ͱ� �ִٰ� ����)
    }

    // Ʈ�� ��ü �ʱ�ȭ
    for (int i = 0; i < 5; ++i)
    {
        Trap obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        traps.push_back(obj); // Ʈ�� ����Ʈ�� �߰� (traps ���Ͱ� �ִٰ� ����)
    }

    // ���� ��ü �ʱ�ȭ
    for (int i = 0; i < 10; ++i)
    {
        Jumbo obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        jumbos.push_back(obj); // ���� ����Ʈ�� �߰� (jumbos ���Ͱ� �ִٰ� ����)
    }

    // �÷��̾� ��ü �ʱ�ȭ
    GameObject* obj = new Player;
    obj->SetPosition(widthUid(rd), heightUid(rd));
    player = *(static_cast<Player*>(obj)); // �÷��̾� ��ü ���� (player�� PlayScene�� ��� ������� ����)
    delete obj; // ���� �Ҵ� ����
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
    // ���� ��ü �ʱ�ȭ
    for (int i = 0; i < 30; ++i)
    {
        Food obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        foods.push_back(obj);
    }

    // �� ��ü �ʱ�ȭ
    for (int i = 0; i < 5; ++i)
    {
        Enemy obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        enemys.push_back(obj); // �� ����Ʈ�� �߰�
    }

    // Ʈ�� ��ü �ʱ�ȭ
    for (int i = 0; i < 5; ++i)
    {
        Trap obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        traps.push_back(obj); // Ʈ�� ����Ʈ�� �߰�
    }

    // ���� ��ü �ʱ�ȭ
    for (int i = 0; i < 10; ++i)
    {
        Jumbo obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        jumbos.push_back(obj); // ���� ����Ʈ�� �߰�
    }

    // �÷��̾� ��ü �ʱ�ȭ
    GameObject* obj = new Player;
    obj->SetPosition(widthUid(rd), heightUid(rd));
    player = *(static_cast<Player*>(obj)); // �÷��̾� ��ü ����
    delete obj; // ���� �Ҵ� ����

    camera.Initialize(&player);
}

void PlayScene::Update()
{
    

    //�÷��̾� <-> ���� �浹üũ
    RECT playerTempRect = player.GetRect();
    
    for (auto it = foods.begin(); it != foods.end();)
    {
        RECT foodTempRect = it->GetRect();
        //// Food ��ü�� RECT ��ǥ ���
        //std::cout << "Food Rect: left=" << foodTempRect.left
        //    << ", top=" << foodTempRect.top
        //    << ", right=" << foodTempRect.right
        //    << ", bottom=" << foodTempRect.bottom << std::endl;

        //// �����: �÷��̾� RECT ��ǥ�� �Բ� ��� (���� ����)
        //std::cout << "Player Rect: left=" << playerTempRect.left
        //    << ", top=" << playerTempRect.top
        //    << ", right=" << playerTempRect.right
        //    << ", bottom=" << playerTempRect.bottom << std::endl;

        std::cout << player.GetSpeed() << std::endl;
        RECT temp;
        if (IntersectRect(&temp, &playerTempRect, &foodTempRect))
        {
            player.Setradius(player.GetRadius() + 0.05);
            it = foods.erase(it); // �浹�� ���� ����
        }
        else
        {
            ++it;
        }
    }
    //�÷��̾� ������Ʈ
    player.Update();

    //Ǫ�� ������Ʈ
    for (auto it = foods.begin(); it != foods.end(); ++it)
    {
        it->Update();
    }

    //�� ������Ʈ
    for (auto it = enemys.begin(); it != enemys.end(); ++it)
    {
        it->Update();
    }

    //Ʈ�� ������Ʈ
    for (auto it = traps.begin(); it != traps.end(); ++it)
    {
        it->Update();
    }

    //���� ������Ʈ
    for (auto it = jumbos.begin(); it != jumbos.end(); ++it)
    {
        it->Update();
    }

    camera.Update(&player);
}

void PlayScene::LateUpdate()
{
    Scene::LateUpdate();
}

void PlayScene::Render(HDC hdc)
{
    // �÷��̾� ������
    player.Render(hdc);

    // ���� ��ü ������
    for (auto it = foods.begin(); it != foods.end(); ++it)
    {
        it->Render(hdc);
    }

    // �� ��ü ������
    for (auto it = enemys.begin(); it != enemys.end(); ++it)
    {
        it->Render(hdc);
    }

    // Ʈ�� ��ü ������
    for (auto it = traps.begin(); it != traps.end(); ++it)
    {
        it->Render(hdc);
    }

    // ���� ��ü ������
    for (auto it = jumbos.begin(); it != jumbos.end(); ++it)
    {
        it->Render(hdc);
    }
    camera.Render(hdc);

    // ���콺 ��ǥ ���
    WCHAR Text[100];
    wsprintf(Text, L"X : %d Y : %d", (int)Input::GetMousePosition().x, (int)Input::GetMousePosition().y);
    TextOut(hdc, Input::GetMousePosition().x + 10, Input::GetMousePosition().y, Text, lstrlen(Text));
}