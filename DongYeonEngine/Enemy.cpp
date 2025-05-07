#include "Enemy.h"
#include "Time.h"

using namespace Gdiplus;

Enemy::Enemy()
{
	mX = 0;
	mY = 0;
	rect = { (int)(mX - radius), (int)(mY - radius),(int)(mX + radius),(int)(rect.bottom = mY + radius) };
	color = RGB(255, 0, 0);
    detectPlayerRange = radius * 2.5f;
	PlayerDetect = false; 
	radius = 30.0f;
	speed = 100.0f;
}

void Enemy::Update(std::vector<Food>& foods, std::vector<Player>& players)
{
    // Update the collision rectangle
    rect.left = mX - radius;
    rect.right = mX + radius;
    rect.top = mY - radius;
    rect.bottom = mY + radius;

    if (foods.empty()) return; // No food to chase

    bool PlayerDetect = false;
    Player* closestPlayer = nullptr;
    float closestPlayerDistance = FLT_MAX;

    for (auto it = players.begin(); it != players.end();)
    {
        // 중심점 간 거리 계산
        float dx = it->GetPositionX() - mX;
        float dy = it->GetPositionY() - mY;
        float distance = std::sqrt(dx * dx + dy * dy);

        // 반지름 합과 비교
        float radiusSum = it->GetRadius() + detectPlayerRange;
        if (distance <= radiusSum) //안에 있어 그러면 플레이어를 추적
        {
            PlayerDetect = true;
            if (distance < closestPlayerDistance)
            {
                closestPlayerDistance = distance;
                closestPlayer = &(*it);
            }
        }
        it++;
    }

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

    if (PlayerDetect == false)
    {
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

                SetDirection(directionX, directionY);

                // Move enemy based on speed and time delta
                mX += directionX * speed * Time::DeltaTime();
                mY += directionY * speed * Time::DeltaTime();
            }
        }
    }
    else
    {
        //플레이어 쪽으로 이동 
        if (closestPlayer)
        {
            float dx = closestPlayer->GetPositionX() - mX;
            float dy = closestPlayer->GetPositionY() - mY;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance > 0) // 0으로 나누기 방지
            {
                float directionX = dx / distance;
                float directionY = dy / distance;

                SetDirection(directionX, directionY);

                // 속도와 시간 델타를 기반으로 적 이동
                mX += directionX * speed * Time::DeltaTime();
                mY += directionY * speed * Time::DeltaTime();
            }
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
    graphics.SetPixelOffsetMode(PixelOffsetModeHalf);
	// GDI+ 색상 객체 생성
    Color gdiBrushColor(GetRValue(color), GetGValue(color), GetBValue(color));
    SolidBrush brush(gdiBrushColor);
    Color gdiPenColor(GetRValue(color) * 0.6, GetGValue(color) * 0.6, GetBValue(color) * 0.6);
    Pen pen(gdiPenColor, 4);
	// 디버깅용 사각형(충돌영역)
	//Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

	// 사각형 안에 원 그리기
	INT ellipseWidth = rect.right - rect.left; //너비
	INT ellipseHeight = rect.bottom - rect.top; //높이
	graphics.FillEllipse(&brush, rect.left, rect.top, ellipseWidth, ellipseHeight);
    graphics.DrawEllipse(&pen, rect.left, rect.top, ellipseWidth, ellipseHeight);

    // 플레이어 감지 범위 렌더링 (빨간 선)
    Pen detectPen(Color(255, 0, 0), 2); // 빨간색, 두께 2
    float detectDiameter = detectPlayerRange * 2.0f;
    float detectLeft = mX - detectPlayerRange;
    float detectTop = mY - detectPlayerRange;
    graphics.DrawEllipse(&detectPen, detectLeft, detectTop, detectDiameter, detectDiameter);

}

void Enemy::Setradius(float r)
{
    radius = r;

    float scale = BASE_RADIUS / radius; // 반지름 비율
    speed = BASE_SPEED * scale;
	detectPlayerRange = radius * 2.5f;
    // 속도 제한
    speed = (std::max)(MIN_SPEED, (std::min)(MAX_SPEED, speed));

    // 충돌 영역 업데이트
    rect.left = (int)(mX - radius);
    rect.right = (int)(mX + radius);
    rect.top = (int)(mY - radius);
    rect.bottom = (int)(mY + radius);
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

