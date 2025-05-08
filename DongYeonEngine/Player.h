#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
    Player();
    void Update() override;
    void LateUpdate() override;
    void Render(HDC hdc) override;

    void SetPosition(float x, float y) override { mX = x; mY = y; }
    void Setradius(float r);
    void SetSpeed(float s) override { speed = s; }
    void SetColor(COLORREF c) override { color = c; }
    void PlusTime(float deltaTime)
    { 
       
       splitTime += deltaTime;
      
    }

    float GetPositionX() override { return mX; }
    float GetPositionY() override { return mY; }
    float GetSpeed() override { return speed; }
    COLORREF GetColor() override { return color; }
    float GetRadius() override { return radius; }
    RECT GetRect() override { return rect; }
    float GetSplitTime() { return splitTime; }
    bool GetIsSplit() { return isSplit; }
    void OnSplit() { isSplit = true; isBoost = true; timeSinceSplit = 0.0f; splitTime = 0.0f; }

private:
    float mX;
    float mY;
    float radius;
    float speed;
    RECT rect;
    COLORREF color;
    float timeSinceSplit; // 분열 후 경과 시간
    bool isSplit; // 분열 상태인지 여부
    bool isBoost;
    float splitTime = 0.0f;
    float boostTime = 0.5f; // 속도 부스트 지속 시간
   
    // 속도 계산용 상수
    static constexpr float BASE_RADIUS = 30.0f; // 기준 반지름
    static constexpr float BASE_SPEED = 80.0f; // 기준 속도
    static constexpr float MIN_SPEED = 25.0f; // 최소 속도
    static constexpr float MAX_SPEED = 200.0f; // 최대 속도
};