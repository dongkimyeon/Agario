#include "Trap.h"
#include "Time.h"

using namespace Gdiplus;

Trap::Trap()
{
	mX = 0;
	mY = 0;
	rect = { (int)(mX - radius), (int)(mY - radius),(int)(mX + radius),(int)(rect.bottom = mY + radius) };
	color = RGB(255, 255, 0);
	radius = 30.0f;
	speed = 100.0f;
}

void Trap::Update()
{

	rect.left = mX - radius;
	rect.right = mX + radius;
	rect.top = mY - radius;
	rect.bottom = mY + radius;


}

void Trap::LateUpdate()
{
}

void Trap::Render(HDC hdc)
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


float Trap::GetPositionX()
{
	return mX;
}

float Trap::GetPositionY()
{
	return mY;
}


