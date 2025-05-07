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
}

void Food::Update()
{

	rect.left = mX - radius;
	rect.right = mX + radius;
	rect.top = mY - radius;
	rect.bottom = mY + radius;

}

void Food::LateUpdate()
{
}

void Food::Render(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	// GDI+ ���� ��ü ����
	Color gdiColor(GetRValue(color), GetGValue(color), GetBValue(color));
	SolidBrush brush(gdiColor);

	// ������ �簢��(�浹����)
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

	// �簢�� �ȿ� �� �׸���
	INT ellipseWidth = rect.right - rect.left; //�ʺ�
	INT ellipseHeight = rect.bottom - rect.top; //����
	graphics.FillEllipse(&brush, rect.left, rect.top, ellipseWidth, ellipseHeight);
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

