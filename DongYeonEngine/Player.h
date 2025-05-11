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
    
	float GetFoodDetectDistance() { return foodDetectDistance; } // ���� Ž�� �Ÿ� ��ȯ
	bool GetJumbo() { return isJumbo; } // ���� ���� Ȯ��
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
    bool ShouldSpawnJumbo() const { return spawnJumbo; } // �� ���� ���� ����
    void ResetSpawnJumbo() { spawnJumbo = false; } // ���� �÷��� ����

private:
    float mX;
    float mY;
    float radius;
    float speed;
    RECT rect;
    COLORREF color;
    float timeSinceSplit; // �п� �� ��� �ð�
    bool isSplit; // �п� �������� ����
    bool isBoost;
    float splitTime;
    float boostTime; // �ӵ� �ν�Ʈ ���� �ð�
	bool isProtected; // ��ȣ ����
    bool isJumbo;
	float foodDetectDistance = 250.0f; // ���� Ž�� �Ÿ�
    float jumboTime; // ���� ���� ���� �ð�
    bool spawnJumbo; // ���� ���� �÷���
    // �ӵ� ���� ���
    static constexpr float BASE_RADIUS = 30.0f; // ���� ������
    static constexpr float BASE_SPEED = 80.0f; // ���� �ӵ�
    static constexpr float MIN_SPEED = 25.0f; // �ּ� �ӵ�
    static constexpr float MAX_SPEED = 200.0f; // �ִ� �ӵ�
};