#pragma once
#include "GameObject.h"

class Trap : public GameObject
{
public:
	Trap();
	void Update() override;
	void LateUpdate() override;
	void Render(HDC hdc) override;

	void SetPosition(float x, float y) override;

	float GetPositionX() override;
	float GetPositionY() override;
	
private:
	float mX;
	float mY;
	float radius;
	float speed;
	RECT rect;
	COLORREF color;

};

