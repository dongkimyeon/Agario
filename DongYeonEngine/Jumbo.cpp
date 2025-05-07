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
	radius = 30.0f;
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

   
	Color gdiBrushColor(GetRValue(color), GetGValue(color), GetBValue(color));
	SolidBrush brush(gdiBrushColor);
	Color gdiPenColor(GetRValue(color) * 0.6, GetGValue(color) * 0.6, GetBValue(color) * 0.6);
	Pen pen(gdiPenColor, 4);
    //µð¹ö±ë¿ë
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


