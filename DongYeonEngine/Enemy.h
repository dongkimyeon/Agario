#pragma once
#include "GameObject.h"
#include "Food.h"
#include "Player.h"
class Enemy : public GameObject
{
public:
	Enemy();
	void Update(std::vector<Food>& foods, std::vector<Player>& players, std::vector<Enemy>& enemies);
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
	void SetId(int i)
	{
		id = i;
	}
	void SetLeader(bool b)
	{
		leader = b;
	}
	void PlusTime(float deltaTime)
	{
		if (!leader)
		{
			splitTime += deltaTime;
		}
		
	}

	bool GetLeaderFlag() { return leader; }
	float GetPositionX() override;
	float GetPositionY() override;
	float GetSpeed() override;
	COLORREF GetColor() override;
	float GetRadius() override;
	RECT GetRect() override;
	float GetSplitTime() { return splitTime; }
	void SetDirection(float dx, float dy) {
		// 방향 벡터를 정규화해서 저장
		float length = std::sqrt(dx * dx + dy * dy);
		if (length != 0.0f) {
			dirX = dx / length;
			dirY = dy / length;
		}
	}

	float GetDirectionX() const { return dirX; }
	float GetDirectionY() const { return dirY; }

	int GetId() const { return id; }

private:
	float mX;
	float mY;
	float radius;
	float speed;
	RECT rect;
	COLORREF color;
	float splitTime; // 분열 후 경과 시간
	bool isSplit; // 분열 상태인지 여부
	bool isBoost;
	int id;
	bool leader;
	float boostTime = 0.5f; // 속도 부스트 지속 시간

	float detectPlayerRange;
	bool PlayerDetect = false; // 플레이어 감지 여부
	float dirX = 0.0f;
	float dirY = 0.0f;

	// 속도 계산용 상수
	static constexpr float BASE_RADIUS = 30.0f; // 기준 반지름
	static constexpr float BASE_SPEED = 80.0f; // 기준 속도
	static constexpr float MIN_SPEED = 25.0f; // 최소 속도
	static constexpr float MAX_SPEED = 150.0f; // 최대 속도


};

