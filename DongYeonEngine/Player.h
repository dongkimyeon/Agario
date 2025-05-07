#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
	void Update() override;
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
			if (speed < 15.0f)
			{
				speed = 15.0f;
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
	
	void OnSplit()
	{
		isSplit = true;
		timeSinceSplit = 0;
	}
	
private:
	float mX;
	float mY;
	float radius;
	float speed;
	RECT rect;
	COLORREF color;
	float timeSinceSplit; // 분열 후 경과 시간
	bool isSplit; // 분열 상태인지 여부
	float boostTime = 0.7f; // 속도 변화 지속 시간
	float oldSpeed; //원래 속도 저장
	


};

