#include "Player.h"
#include "Input.h"
#include "Time.h"

using namespace Gdiplus;

Player::Player()
{
	mX = 0;
	mY = 0;
	rect = { (int)(mX - radius), (int)(mY - radius),(int)(mX + radius),(int)(rect.bottom = mY + radius) };
	color = RGB(0, 0, 255);
	radius = 30.0f;
	speed = 100.0f;
}

void Player::Update()
{
    int mouseX = (int)Input::GetMousePosition().x;
    int mouseY = (int)Input::GetMousePosition().y;

    // 플레이어 중심 위치에서 마우스 위치까지의 방향 벡터 계산
    float dx = mouseX - mX;
    float dy = mouseY - mY; 

    // 벡터의 길이(거리) 계산
    float distance = std::sqrt(dx * dx + dy * dy);

    // 마우스와 플레이어 사이의 거리가 1보다 크면 이동
    if (distance > 1.0f)
    {
        // 방향 벡터 정규화
        dx /= distance;
        dy /= distance;

        mX += dx * speed * Time::DeltaTime();
        mY += dy * speed * Time::DeltaTime();
    }

   
    rect.left = (int)(mX - radius);
    rect.right = (int)(mX + radius);
    rect.top = (int)(mY - radius);
    rect.bottom = (int)(mY + radius);
}

void Player::LateUpdate()
{
}

void Player::Render(HDC hdc)
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

void Player::SetPosition(float x, float y)
{
	mX = x;
	mY = y;
	position.x = x;
	position.y = y;

}

float Player::GetPositionX()
{
	return mX;
}

float Player::GetPositionY()
{
	return mY;
}

float Player::GetSpeed()
{
	return speed;
}

COLORREF Player::GetColor()
{
	return color;
}

float Player::GetRadius()
{
	return radius;
}

RECT Player::GetRect()
{
	return rect;
}


