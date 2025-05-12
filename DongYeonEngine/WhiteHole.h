#pragma once
#include "GameObject.h"

class WhiteHole : public GameObject
{
public:
	WhiteHole();
	void Update() override;
	void LateUpdate() override;
	void Render(Gdiplus::Graphics& graphics) override;

	void SetPosition(float x, float y) override
	{
		mX = x;
		mY = y;
	}
	void Setradius(float r) override
	{
		radius = r;
	}
	void SetSpeed(float s) override
	{
		speed = s;
	}
	void SetColor(COLORREF c) override
	{
		color = c;
	}
	void SetDirection(float a, float b)
	{
		dirX = a;
		dirY = b;
	}
	float GetPositionX() override;
	float GetPositionY() override;
	float GetRadius() override;

private:
	float mX;
	float mY;
	float dirX;
	float dirY;
	float radius;
	float speed;
	Gdiplus::Image* mWhiteHoleImage;
	float rotationAngle; // ȸ�� ���� (�� ����)
	float rotationSpeed; // ȸ�� �ӵ� (��/��)
	RECT rect;
	COLORREF color;

};

