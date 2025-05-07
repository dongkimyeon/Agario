#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
	void Update() override;
	void LateUpdate() override;
	void Render(HDC hdc) override;

	void SetPosition(float x, float y) override;
	
	float GetPositionX() override;
	float GetPositionY() override;
	float GetSpeed() override;
	COLORREF GetColor() override;
	float GetRadius() override;
	RECT GetRect() override;
	

private:
	float mX;
	float mY;
	Vector2 position;
	float radius;
	float speed;
	RECT rect;
	COLORREF color;

};

