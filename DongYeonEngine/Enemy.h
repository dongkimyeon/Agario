#pragma once
#include "GameObject.h"
#include "Food.h"
#include "Player.h"
class Enemy : public GameObject
{
public:
	Enemy();
	void Update(std::vector<Food>& foods , std::vector<Player>& players);
	void LateUpdate() override;
	void Render(HDC hdc) override;

	void SetPosition(float x, float y) override
	{
		mX = x;
		mY = y;
	}
	void Setradius(float r) override;
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
	void OnSplit() { isSplit = true; timeSinceSplit = 0.0f; }
	void SetDirection(float dx, float dy) {
		// ���� ���͸� ����ȭ�ؼ� ����
		float length = std::sqrt(dx * dx + dy * dy);
		if (length != 0.0f) {
			dirX = dx / length;
			dirY = dy / length;
		}
	}

	float GetDirectionX() const { return dirX; }
	float GetDirectionY() const { return dirY; }

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
	float detectPlayerRange;
	bool PlayerDetect = false; // �÷��̾� ���� ����
	float dirX = 0.0f;
	float dirY = 0.0f;

	// �ӵ� ���� ���
	static constexpr float BASE_RADIUS = 30.0f; // ���� ������
	static constexpr float BASE_SPEED = 80.0f; // ���� �ӵ�
	static constexpr float MIN_SPEED = 25.0f; // �ּ� �ӵ�
	static constexpr float MAX_SPEED = 150.0f; // �ִ� �ӵ�


};

