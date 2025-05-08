#pragma once
#include "GameObject.h"

class Jumbo : public GameObject
{
public:
	Jumbo();
	void Update() override;
	void LateUpdate() override;
	void Render(HDC hdc) override;

	void SetPosition(float x, float y) override;
	void SetDirection(float a, float b)
	{
		dirX = a;
		dirY = b;
	}
	float GetPositionX() override;
	float GetPositionY() override;
	float GetSpeed() override;
	COLORREF GetColor() override;
	float GetRadius() override;
	RECT GetRect() override;
private:
	float mX;
	float mY;
	float dirX;
	float dirY;
	float radius;
	float speed;

	RECT rect;
	COLORREF color;

};

