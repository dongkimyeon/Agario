#include "Food.h"
#include "Input.h"
#include "Time.h"

using namespace Gdiplus;

Food::Food()
{
	mX = 0;
	mY = 0;
	rect = { (int)(mX - radius), (int)(mY - radius),(int)(mX + radius),(int)(rect.bottom = mY + radius) };
	color = RGB(0, 255, 0);
	radius = 10.0f;
	speed = 0.0f;
	dirX = 0.0f;
	dirY = 0.0f;

}

void Food::Update()
{
	// 방향과 속도가 설정되어 있으면 이동
	if (speed > 0.0f)
	{
		mX += dirX * speed * Time::DeltaTime();
		mY += dirY * speed * Time::DeltaTime();

		// 충돌 영역 업데이트
		rect.left = (int)(mX - radius);
		rect.right = (int)(mX + radius);
		rect.top = (int)(mY - radius);
		rect.bottom = (int)(mY + radius);
	}
	else
	{
		rect.left = (int)(mX - radius);
		rect.right = (int)(mX + radius);
		rect.top = (int)(mY - radius);
		rect.bottom = (int)(mY + radius);
	}
}

void Food::LateUpdate()
{
}

void Food::Render(HDC hdc)
{

	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetPixelOffsetMode(PixelOffsetModeHalf);
	
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

void Food::SetPosition(float x, float y)
{
	mX = x;
	mY = y;

}

float Food::GetPositionX()
{
	return mX;
}

float Food::GetPositionY()
{
	return mY;
}

float Food::GetSpeed()
{
	return speed;
}

COLORREF Food::GetColor()
{
	return color;
}

float Food::GetRadius()
{
	return radius;
}

RECT Food::GetRect()
{
	return rect;
}

