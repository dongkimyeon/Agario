#pragma once
#include "GameObject.h"
#include "Food.h"

class Enemy : public GameObject
{
public:
	Enemy();
	void Update(std::vector<Food>& foods);
	void LateUpdate() override;
	void Render(HDC hdc) override;

	
	void SetPosition(float x, float y) override
	{
		mX = x;
		mY = y;
	}
	void Setradius(float r) override
	{
		float oldRadius = radius;
		float deltaRadius = r - oldRadius; // 델타 값 계산

		if (deltaRadius < 0) // 반지름이 작아지면
		{
			speed += 2.5f; // 기존 로직 유지
		}
		else if (deltaRadius > 0) // 반지름이 커지면
		{
			// 델타 값에 비례하여 속도 감소 (음식 반지름에 간접적으로 비례)
			float speedReduction = deltaRadius * 1.1f; // 델타 값의 2배로 감소
			speed -= speedReduction;

			// 최소 속도 제한
			if (speed < 20.0f)
			{
				speed = 20.0f;
			}
		}

		// 반지름 업데이트
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
	float speed;
	RECT rect;
	COLORREF color;


};

