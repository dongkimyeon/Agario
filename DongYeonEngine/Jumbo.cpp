#include "Jumbo.h"
#include "Input.h"
#include "Time.h"

using namespace Gdiplus;

Jumbo::Jumbo()
{
	mX = 0;
	mY = 0;
	rect = { (int)(mX - radius), (int)(mY - radius),(int)(mX + radius),(int)(rect.bottom = mY + radius) };
	color = RGB(0, 122, 122);
	radius = 10.0f;
}

void Jumbo::Update()
{

	rect.left = mX - radius;
	rect.right = mX + radius;
	rect.top = mY - radius;
	rect.bottom = mY + radius;

}

void Jumbo::LateUpdate()
{
}

void Jumbo::Render(HDC hdc)
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


