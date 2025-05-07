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
    float GetPositionX() override { return mX; }
    float GetPositionY() override { return mY; }
    float GetSpeed() override { return speed; }
    COLORREF GetColor() override { return color; }
    float GetRadius() override { return radius; }
    RECT GetRect() override { return rect; }

    void OnSplit() { isSplit = true; timeSinceSplit = 0.0f; }

private:
    float mX;
    float mY;
    float radius;
    float speed;
    RECT rect;
    COLORREF color;
    float timeSinceSplit; // �п� �� ��� �ð�
    bool isSplit; // �п� �������� ����
    float boostTime = 0.5f; // �ӵ� �ν�Ʈ ���� �ð�

    // �ӵ� ���� ���
    static constexpr float BASE_RADIUS = 30.0f; // ���� ������
    static constexpr float BASE_SPEED = 80.0f; // ���� �ӵ�
    static constexpr float MIN_SPEED = 25.0f; // �ּ� �ӵ�
    static constexpr float MAX_SPEED = 150.0f; // �ִ� �ӵ�
};