#pragma once
#include "GameObject.h"

class Food : public GameObject
{
public:
	Food();
	void Update() override;
	void LateUpdate() override;
	void Render(Gdiplus::Graphics& graphics) override;

	void SetPosition(float x, float y) override;
	void SetDirection(float dx, float dy) { dirX = dx; dirY = dy; }
	void SetSpeed(float s)  { speed = s; }
	float GetPositionX() override;
	float GetPositionY() override;
	float GetSpeed() override;
	COLORREF GetColor() override;
	float GetRadius() override;
	RECT GetRect() override;
private:
	float mX;
	float mY;
	float radius;
	float dirX = 0.0f;
	float dirY = 0.0f; 
	float speed;
	RECT rect;
	COLORREF color;

};

