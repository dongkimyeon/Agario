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
	graphics.SetPixelOffsetMode(PixelOffsetModeHalf);
	// GDI+ ���� ��ü ����
	Color gdiBrushColor(GetRValue(color), GetGValue(color), GetBValue(color));
	SolidBrush brush(gdiBrushColor);
	Color gdiPenColor(GetRValue(color) * 0.6, GetGValue(color) * 0.6, GetBValue(color) * 0.6);
	Pen pen(gdiPenColor, 4);
	// ������ �簢��(�浹����)
	//Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

	// �簢�� �ȿ� �� �׸���
	INT ellipseWidth = rect.right - rect.left; //�ʺ�
	INT ellipseHeight = rect.bottom - rect.top; //����
	graphics.FillEllipse(&brush, rect.left, rect.top, ellipseWidth, ellipseHeight);
	graphics.DrawEllipse(&pen, rect.left, rect.top, ellipseWidth, ellipseHeight);
}


float Trap::GetPositionX()
{
	return mX;
}

float Trap::GetPositionY()
{
	return mY;
}


