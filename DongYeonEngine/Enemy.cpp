#include "Enemy.h"
#include "Time.h"

using namespace Gdiplus;

Enemy::Enemy()
{
	mX = 0;
	mY = 0;
	rect = { (int)(mX - radius), (int)(mY - radius),(int)(mX + radius),(int)(rect.bottom = mY + radius) };
	color = RGB(255, 0, 0);
	radius = 30.0f;
	speed = 100.0f;
}

void Enemy::Update(std::vector<Food>& foods)
{
    // Update the collision rectangle
    rect.left = mX - radius;
    rect.right = mX + radius;
    rect.top = mY - radius;
    rect.bottom = mY + radius;

    if (foods.empty()) return; // No food to chase

    // Find the closest food
    float closestDistance = FLT_MAX;
    Food* closestFood = nullptr;

    for (Food& food : foods)
    {
        float dx = food.GetPositionX() - mX;
        float dy = food.GetPositionY() - mY;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance < closestDistance)
        {
            closestDistance = distance;
            closestFood = &food;
        }
    }

    if (closestFood)
    {
        // Calculate direction to the closest food
        float dx = closestFood->GetPositionX() - mX;
        float dy = closestFood->GetPositionY() - mY;
        float distance = sqrt(dx * dx + dy * dy);

        // Normalize direction and move enemy
        if (distance > 0) // Prevent division by zero
        {
            float directionX = dx / distance;
            float directionY = dy / distance;

            // Move enemy based on speed and time delta
            mX += directionX * speed * Time::DeltaTime();
            mY += directionY * speed * Time::DeltaTime();
        }
    }
}
void Enemy::LateUpdate()
{
}

void Enemy::Render(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	// GDI+ 색상 객체 생성
	Color gdiColor(GetRValue(color), GetGValue(color), GetBValue(color));
	SolidBrush brush(gdiColor);

	// 디버깅용 사각형(충돌영역)
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

	// 사각형 안에 원 그리기
	INT ellipseWidth = rect.right - rect.left; //너비
	INT ellipseHeight = rect.bottom - rect.top; //높이
	graphics.FillEllipse(&brush, rect.left, rect.top, ellipseWidth, ellipseHeight);
}


float Enemy::GetPositionX()
{
	return mX;
}

float Enemy::GetPositionY()
{
	return mY;
}

float Enemy::GetSpeed()
{
	return speed;
}

COLORREF Enemy::GetColor()
{
	return color;
}

float Enemy::GetRadius()
{
	return radius;
}

RECT Enemy::GetRect()
{
	return rect;
}

