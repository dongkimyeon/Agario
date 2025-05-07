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
		float deltaRadius = r - oldRadius; // ��Ÿ �� ���

		if (deltaRadius < 0) // �������� �۾�����
		{
			speed += 2.5f; // ���� ���� ����
		}
		else if (deltaRadius > 0) // �������� Ŀ����
		{
			// ��Ÿ ���� ����Ͽ� �ӵ� ���� (���� �������� ���������� ���)
			float speedReduction = deltaRadius * 1.1f; // ��Ÿ ���� 2��� ����
			speed -= speedReduction;

			// �ּ� �ӵ� ����
			if (speed < 15.0f)
			{
				speed = 15.0f;
			}
		}

		// ������ ������Ʈ
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
	float timeSinceSplit; // �п� �� ��� �ð�
	bool isSplit; // �п� �������� ����
	float boostTime = 0.7f; // �ӵ� ��ȭ ���� �ð�
	float oldSpeed; //���� �ӵ� ����
	


};

