#include "Player.h"
#include "Input.h"
#include "Time.h"
#define MAX_SPEED 150
#define MIN_SPEED 25
using namespace Gdiplus;

Player::Player()
{
	mX = 0;
	mY = 0;
	rect = { (int)(mX - radius), (int)(mY - radius),(int)(mX + radius),(int)(rect.bottom = mY + radius) };
	color = RGB(8, 136, 248);
	radius = 30.0f;
	speed = 100.0f;
	timeSinceSplit = 0;
	isSplit = false;
	boostTime = 0.5f;
	oldSpeed = speed;
}

void Player::Update()
{
	
	if (isSplit) 
	{
		
		timeSinceSplit += Time::DeltaTime();
		if (timeSinceSplit >= boostTime) 
		{
			speed = oldSpeed;
			timeSinceSplit = 0;
			isSplit = false; 
		}
		else 
		{
			if (speed * 2.0f > MAX_SPEED) {
				speed = MAX_SPEED;
			}
			else {
				speed = speed * 2.0f;
			}
		}
	}
	else
	{
		oldSpeed = speed;
	}

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


