#pragma once
#include "GameObject.h"
#include "Player.h"

class Jumbo : public GameObject
{
public:
    Jumbo();
    void Update(std::vector<Player>& players);
    void LateUpdate() override;
    void Render(Gdiplus::Graphics& graphics) override;

    void SetPosition(float x, float y) override;
    void SetDirection(float a, float b) { dirX = a; dirY = b; }
    float GetPositionX() override;
    float GetPositionY() override;
    float GetSpeed() override;
    COLORREF GetColor() override;
    float GetRadius() override;
    RECT GetRect() override;
    bool IsInvincible() const { return invincibleTime > 0.0f; } // 무적 상태 확인

private:
    float mX;
    float mY;
    float dirX;
    float dirY;
    float radius;
    float speed;
    float fastSpeed;
    float playerDetectRadius;
    float invincibleTime = 0.0f; // 무적 지속 시간
    RECT rect;
    COLORREF color;
};