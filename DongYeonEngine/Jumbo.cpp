#include "Jumbo.h"
#include "Input.h"
#include "Time.h"

using namespace Gdiplus;

#define M_PI 3.14159265358979


Jumbo::Jumbo()
{
    std::uniform_real_distribution<float> distAngle(0.0f, 2.0f * M_PI);
    std::random_device gen;

	mX = 0;

	mY = 0;
	rect = { (int)(mX - radius), (int)(mY - radius),(int)(mX + radius),(int)(rect.bottom = mY + radius) };
	color = RGB(255, 0, 255);
	radius = 30.0f;
	speed = 150.0f;
	// 초기 랜덤 각도 설정
	float angle = distAngle(gen);
	dirX = cos(angle);
	dirY = sin(angle);

	rect = { (int)(mX - radius), (int)(mY - radius), (int)(mX + radius), (int)(mY + radius) };

}

void Jumbo::Update()
{
    std::uniform_real_distribution<float> distAngle(0.0f, 2.0f * M_PI);
    std::random_device gen;

    float deltaTime = Time::DeltaTime(); // 시간 기반 이동
    mX += dirX * speed * deltaTime;
    mY += dirY * speed * deltaTime;

    // 화면 경계 충돌 처리
    bool collision = false;
    if (mX - radius < 0 || mX + radius > 1600)
    {
        collision = true;
        mX = max(radius, min(mX, 1600 - radius)); // 경계 밖 벗어남 방지
    }

    if (mY - radius < 0 || mY + radius > 800)
    {
        collision = true;
        mY = max(radius, min(mY, 800 - radius)); // 경계 밖 벗어남 방지
    }

    // 충돌 시 랜덤 각도로 방향 설정
    if (collision)
    {
        // 0 ~ 2π 사이의 랜덤 각도 생성

        float angle = distAngle(gen);

        // 방향 벡터 계산
        dirX = cos(angle);
        dirY = sin(angle);
    }
    // 렉트 업데이트
    rect = { (int)(mX - radius), (int)(mY - radius), (int)(mX + radius), (int)(mY + radius) };
}

void Jumbo::LateUpdate()
{
}
void Jumbo::Render(HDC hdc)
{
    Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

   
	Color gdiBrushColor(GetRValue(color), GetGValue(color), GetBValue(color));
	SolidBrush brush(gdiBrushColor);
	Color gdiPenColor(GetRValue(color) * 0.6, GetGValue(color) * 0.6, GetBValue(color) * 0.6);
	Pen pen(gdiPenColor, 4);
    //디버깅용
    //Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

    
    POINT points[3];
    float centerX = mX;
    float centerY = mY;
    float r = radius;

    points[0].x = static_cast<LONG>(centerX);
    points[0].y = static_cast<LONG>(centerY - r);

    points[1].x = static_cast<LONG>(centerX - r * 0.866f);
    points[1].y = static_cast<LONG>(centerY + r * 0.5f);

    points[2].x = static_cast<LONG>(centerX + r * 0.866f);
    points[2].y = static_cast<LONG>(centerY + r * 0.5f);

    graphics.FillPolygon(&brush, (Gdiplus::Point*)points, 3);
    graphics.DrawPolygon(&pen, (Gdiplus::Point*)points, 3);
}

void Jumbo::SetPosition(float x, float y)
{
	mX = x;
	mY = y;

}



float Jumbo::GetPositionX()
{
	return mX;
}

float Jumbo::GetPositionY()
{
	return mY;
}

float Jumbo::GetSpeed()
{
	return speed;
}

COLORREF Jumbo::GetColor()
{
	return color;
}

float Jumbo::GetRadius()
{
	return radius;
}

RECT Jumbo::GetRect()
{
	return rect;
}


