#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
    Player();
    void Update(float zoomScale, float cameraX, float cameraY, float clientWidth, float clientHeight); 
    void LateUpdate() override;
    void Render(Gdiplus::Graphics& graphics) override;

    void SetPosition(float x, float y) override { mX = x; mY = y; }
    void Setradius(float r);
    void SetSpeed(float s) override { speed = s; }
    void SetColor(COLORREF c) override { color = c; }
	void SetProtected(bool p) { isProtected = p; }
    void SetSplitTime(int s) { splitTime = s; }
    void PlusTime(float deltaTime)
    { 
       
       splitTime += deltaTime;
      
    }

	void SetJumbo(bool j) { isJumbo = j; }  
    
	float GetFoodDetectDistance() { return foodDetectDistance; } // 음식 탐지 거리 반환
	bool GetJumbo() { return isJumbo; } // 점보 상태 확인
    float GetPositionX() override { return mX; }
    float GetPositionY() override { return mY; }
    float GetSpeed() override { return speed; }
    COLORREF GetColor() override { return color; }
    float GetRadius() override { return radius; }
    RECT GetRect() override { return rect; }
    float GetSplitTime() { return splitTime; }
    bool GetIsSplit() { return isSplit; }
    void OnSplit() { isSplit = true; isBoost = true; timeSinceSplit = 0.0f; splitTime = 0.0f; }
	bool GetProtected() { return isProtected; }
    bool ShouldSpawnJumbo() const { return spawnJumbo; } // 새 점보 생성 여부
    void ResetSpawnJumbo() { spawnJumbo = false; } // 생성 플래그 리셋

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
    float splitTime;
    float boostTime; // 속도 부스트 지속 시간
	bool isProtected; // 보호 상태
    bool isJumbo;
	float foodDetectDistance = 250.0f; // 음식 탐지 거리
    float jumboTime; // 점보 상태 지속 시간
    bool spawnJumbo; // 점보 생성 플래그
    // 속도 계산용 상수
    static constexpr float BASE_RADIUS = 30.0f; // 기준 반지름
    static constexpr float BASE_SPEED = 80.0f; // 기준 속도
    static constexpr float MIN_SPEED = 25.0f; // 최소 속도
    static constexpr float MAX_SPEED = 200.0f; // 최대 속도
};