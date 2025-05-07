#pragma once
#include "PlayScene.h"
#include "Input.h"
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
    player.SetPosition(1280 / 2, 720 / 2);
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
    player.SetPosition(1280 / 2, 720 / 2);
}

void PlayScene::Update()
{
    

    // �÷��̾� <-> ���� �浹 üũ (�� ���)
    for (auto it = foods.begin(); it != foods.end();)
    {
        // �߽��� �� �Ÿ� ���
        float dx = player.GetPositionX() - it->GetPositionX();
        float dy = player.GetPositionY() - it->GetPositionY();
        float distance = std::sqrt(dx * dx + dy * dy);

        // ������ �հ� ��
        float radiusSum = player.GetRadius() + it->GetRadius();
        if (distance <= radiusSum)
        {
            float deltaRadius = player.GetRadius() + (it->GetRadius() / 4);
            player.Setradius(deltaRadius);
            it = foods.erase(it); // �浹�� ���� ����
            std::cout << player.GetRadius() << std::endl;
        }
        else
        {
            ++it;
        }
    }

    //�� <-> ���� �浹üũ 
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
        it->Update(foods);
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

    // ���콺 ��ǥ ���
    WCHAR Text[100];
    wsprintf(Text, L"X : %d Y : %d", (int)Input::GetMousePosition().x, (int)Input::GetMousePosition().y);
    TextOut(hdc, Input::GetMousePosition().x + 10, Input::GetMousePosition().y, Text, lstrlen(Text));
}