#pragma once
#include "PlayScene.h"
#include "Input.h"
#include "Time.h"

#define FOOD_SIZE 50
#define ENEMY_SIZE 2
#define JUMBO_SIZE 5
#define TRAP_SIZE 5

// ī�޶� ��ü�� �ʿ� �� �߰�

std::uniform_int_distribution<int> widthUid(0, 1280);
std::uniform_int_distribution<int> heightUid(0, 720);
std::random_device rd;

PlayScene::PlayScene()
{

    // ���� ��ü �ʱ�ȭ
    for (int i = 0; i < FOOD_SIZE; ++i)
    {
        Food obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        foods.push_back(obj);
    }

    // �� ��ü �ʱ�ȭ
    for (int i = 0; i < ENEMY_SIZE; ++i)
    {
        Enemy obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        enemys.push_back(obj);
    }

    // Ʈ�� ��ü �ʱ�ȭ
    for (int i = 0; i < TRAP_SIZE; ++i)
    {
        Trap obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        traps.push_back(obj);
    }

    // ���� ��ü �ʱ�ȭ
    for (int i = 0; i < JUMBO_SIZE; ++i)
    {
        Jumbo obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        jumbos.push_back(obj);
    }


    // �÷��̾� ��ü �ʱ�ȭ
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
    static std::vector<std::tuple<size_t, float, float>> splitPlayers;

    // �÷��̾� <-> ���� �浹 üũ (�� ���)
    for (auto it = player.begin(); it != player.end();)
    {
        for (auto jt = foods.begin(); jt != foods.end();)
        {
            // �߽��� �� �Ÿ� ���
            float dx = jt->GetPositionX() - it->GetPositionX();
            float dy = jt->GetPositionY() - it->GetPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);

            // ������ �հ� ��
            float radiusSum = jt->GetRadius() + it->GetRadius();
            if (distance <= radiusSum)
            {
                float deltaRadius = it->GetRadius() + (jt->GetRadius() / 4);
                it->Setradius(deltaRadius);
                jt = foods.erase(jt); // �浹�� ���� ����

            }
            else
            {
                ++jt;
            }
        }
        it++;

    }


    //�� <-> ���� �浹üũ 
    for (auto it = enemys.begin(); it != enemys.end();)
    {
        for (auto jt = foods.begin(); jt != foods.end();)
        {
            // �߽��� �� �Ÿ� ���
            float dx = jt->GetPositionX() - it->GetPositionX();
            float dy = jt->GetPositionY() - it->GetPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);

            // ������ �հ� ��
            float radiusSum = jt->GetRadius() + it->GetRadius();
            if (distance <= radiusSum)
            {
                float deltaRadius = it->GetRadius() + (jt->GetRadius() / 4);
                it->Setradius(deltaRadius);
                jt = foods.erase(jt); // �浹�� ���� ����
               
            }
            else
            {
                ++jt;
            }
        }
        it++;
        
    }

    //50���� ��� ������Ʈ
    if (foods.size() <= FOOD_SIZE)
    {
        Food obj;
        obj.SetPosition(widthUid(rd), heightUid(rd));
        foods.push_back(obj);
    }

    // �÷��̾� �п�
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
  
            newPlayer.OnSplit(); // �п� ���� Ȱ��ȭ

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

    //�÷��̾� <-> �÷��̾� �浹ó��
    for (size_t i = 0; i < player.size(); ++i) {
        for (size_t j = i + 1; j < player.size(); ++j) {
            float dx = player[j].GetPositionX() - player[i].GetPositionX();
            float dy = player[j].GetPositionY() - player[i].GetPositionY();
            float distance = std::sqrt(dx * dx + dy * dy);
            float radiusSum = player[i].GetRadius() + player[j].GetRadius();

            if (distance < radiusSum && distance > 0.0f) {
                float overlap = radiusSum - distance;

                // ����ȭ
                float nx = dx / distance;
                float ny = dy / distance;

                // �о�� - �� �÷��̾ �ݾ� �̵�
                float pushX = nx * (overlap / 2.0f);
                float pushY = ny * (overlap / 2.0f);

                player[i].SetPosition(player[i].GetPositionX() - pushX, player[i].GetPositionY() - pushY);
                player[j].SetPosition(player[j].GetPositionX() + pushX, player[j].GetPositionY() + pushY);
            }
        }
    }
    


    //�÷��̾� ������Ʈ
    for (auto it = player.begin(); it != player.end(); ++it)
    {
        it->Update();
        
    }

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
    for (auto it = player.begin(); it != player.end(); ++it)
    {
        it->Render(hdc);
    }

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