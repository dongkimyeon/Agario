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
	PlayTime = 0.0f;
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
    // �÷���Ÿ�� ����
    PlayTime += Time::DeltaTime();

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
                float deltaRadius = it->GetRadius() + (jt->GetRadius()/4);
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
                float deltaRadius = it->GetRadius() + (jt->GetRadius()/4);
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

            // �ּ� �п� ������ 25
            if (radius < 25.0f) {
                continue;
            }

            float newRadius = radius / 2.0f; // ������ ������
            it->Setradius(newRadius); // ���� �÷��̾� ������ ���� (�ӵ� �ڵ� ���)

            Player newPlayer;
            newPlayer.Setradius(newRadius); // �� �÷��̾� ������ ���� (�ӵ� �ڵ� ���)
            newPlayer.SetColor(it->GetColor()); // ���� ����
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
    // �� �п�
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

            // ���� ���� ���� ���� ���
            float dirX = it->GetDirectionX();  // Enemy Ŭ������ �־�� ��
            float dirY = it->GetDirectionY();

            // ���� ���� ����ȭ (�� �Ǿ� ������)
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

    //�÷��̾� <-> �÷��̾� �浹ó��
    for (int i = 0; i < player.size(); ++i) {
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
    int cnt = 0;
    for (auto it = player.begin(); it != player.end(); ++it)
    {
        it->Update();
        std::cout << cnt << "��° :  " << it->GetSpeed() << std::endl;
        cnt++;
    }
    cnt = 0;
    //Ǫ�� ������Ʈ
    for (auto it = foods.begin(); it != foods.end(); ++it)
    {
        it->Update();
    }

    //�� ������Ʈ
    for (auto it = enemys.begin(); it != enemys.end(); ++it)
    {
        it->Update(foods ,player);
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
   

    // ���콺 ��ǥ ���
    WCHAR Text[100];
    wsprintf(Text, L"X : %d Y : %d", (int)Input::GetMousePosition().x, (int)Input::GetMousePosition().y);
    TextOut(hdc, Input::GetMousePosition().x + 10, Input::GetMousePosition().y, Text, lstrlen(Text));

    WCHAR TimeText[100];
    int minutes = static_cast<int>(PlayTime) / 60;
    int seconds = static_cast<int>(PlayTime) % 60;

    wsprintf(TimeText, L"Play Time: %02d�� : %02d��", minutes, seconds);
    TextOut(hdc, 10, 10, TimeText, lstrlen(TimeText));
}