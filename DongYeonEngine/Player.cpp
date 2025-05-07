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

    // �÷��̾� �߽� ��ġ���� ���콺 ��ġ������ ���� ���� ���
    float dx = mouseX - mX;
    float dy = mouseY - mY; 

    // ������ ����(�Ÿ�) ���
    float distance = std::sqrt(dx * dx + dy * dy);

    // ���콺�� �÷��̾� ������ �Ÿ��� 1���� ũ�� �̵�
    if (distance > 1.0f)
    {
        // ���� ���� ����ȭ
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


