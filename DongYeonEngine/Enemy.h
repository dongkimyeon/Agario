#pragma once
#include "GameObject.h"
#include "Food.h"
#include "Player.h"

class Enemy : public GameObject
{
public:
    Enemy();
    void Update(std::vector<Food>& foods, std::vector<Player>& players, std::vector<Enemy>& enemies);
    void LateUpdate() override;
    void Render(HDC hdc) override;

    void SetPosition(float x, float y) override
    {
        mX = x;
        mY = y;
    }
    void Setradius(float r) override;
    void SetSpeed(float s) override
    {
        speed = s;
    }
    void SetColor(COLORREF c) override
    {
        color = c;
    }
    void SetId(int i)
    {
        id = i;
    }
    void SetLeader(bool b)
    {
        leader = b;
    }
    void PlusTime(float deltaTime)
    {
        splitTime += deltaTime;
    }
    void OnSplit(float splitDx = 0.0f, float splitDy = 0.0f) // 분열 방향 전달
    {
        splitTime = 0.0f;
        isSplit = true;
        isBoost = true;
        timeSinceSplit = 0.0f;
        splitDirX = splitDx; // 분열 방향 저장
        splitDirY = splitDy;
    }

    bool GetLeaderFlag() { return leader; }
    float GetPositionX() override;
    float GetPositionY() override;
    float GetSpeed() override;
    COLORREF GetColor() override;
    float GetRadius() override;
    RECT GetRect() override;
    float GetSplitTime() { return splitTime; }
    void SetDirection(float dx, float dy)
    {
        float length = std::sqrt(dx * dx + dy * dy);
        if (length != 0.0f) {
            dirX = dx / length;
            dirY = dy / length;
        }
    }

    float GetDirectionX() const { return dirX; }
    float GetDirectionY() const { return dirY; }
    int GetId() const { return id; }

private:
    float mX;
    float mY;
    float radius;
    float speed;
    RECT rect;
    COLORREF color;
    float splitTime;
    bool isSplit;
    bool isBoost;
    float timeSinceSplit;
    float boostTime = 0.5f;
    int id;
    bool leader;

    float detectPlayerRange;
    bool PlayerDetect = false;
    float dirX = 0.0f;
    float dirY = 0.0f;
    float splitDirX = 0.0f; // 분열 방향 X
    float splitDirY = 0.0f; // 분열 방향 Y

    static constexpr float BASE_RADIUS = 30.0f;
    static constexpr float BASE_SPEED = 80.0f;
    static constexpr float MIN_SPEED = 25.0f;
    static constexpr float MAX_SPEED = 150.0f;
};