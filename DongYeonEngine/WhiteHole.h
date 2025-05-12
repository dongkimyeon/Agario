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
	float rotationAngle; // 회전 각도 (도 단위)
	float rotationSpeed; // 회전 속도 (도/초)
	RECT rect;
	COLORREF color;

};

