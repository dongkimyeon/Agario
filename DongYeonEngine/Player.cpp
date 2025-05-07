#include "Player.h"
#include "Input.h"
#include "Time.h"
#include <algorithm>
using namespace Gdiplus;

Player::Player()
{
    mX = 0;
    mY = 0;
    radius = BASE_RADIUS;
    speed = BASE_SPEED; // ���� �������� �´� �ӵ�
    rect = { (int)(mX - radius), (int)(mY - radius), (int)(mX + radius), (int)(mY + radius) };
    color = RGB(8, 136, 248);
    timeSinceSplit = 0.0f;
    isSplit = false;
    boostTime = 0.5f;
}

void Player::Update()
{
    // �п� �� �ӵ� �ν�Ʈ
    if (isSplit)
    {
        timeSinceSplit += Time::DeltaTime();
        if (timeSinceSplit >= boostTime)
        {
            isSplit = false;
            timeSinceSplit = 0.0f;
            Setradius(radius); // ������ ������� �ӵ� �缳��
        }
        else
        {
            // �ν�Ʈ �ӵ�: �⺻ �ӵ��� 2��, �ִ� �ӵ� ����
            float boostedSpeed = (std::min)(speed * 2.0f, MAX_SPEED);
            speed = boostedSpeed;
        }
    }

    int mouseX = (int)Input::GetMousePosition().x;
    int mouseY = (int)Input::GetMousePosition().y;

    // �÷��̾� �߽ɿ��� ���콺���� ���� ����
    float dx = mouseX - mX;
    float dy = mouseY - mY;
    float distance = std::sqrt(dx * dx + dy * dy);

    // �Ÿ��� 1���� ũ�� �̵�
    if (distance > 1.0f)
    {
        dx /= distance;
        dy /= distance;
        mX += dx * speed * Time::DeltaTime();
        mY += dy * speed * Time::DeltaTime();
    }

    // �浹 ���� ������Ʈ
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
    Color gdiBrushColor(GetRValue(color), GetGValue(color), GetBValue(color));
    SolidBrush brush(gdiBrushColor);
    Color gdiPenColor(GetRValue(color) * 0.6, GetGValue(color) * 0.6, GetBValue(color) * 0.6);
    Pen pen(gdiPenColor, 4);

    INT ellipseWidth = rect.right - rect.left;
    INT ellipseHeight = rect.bottom - rect.top;
    graphics.FillEllipse(&brush, rect.left, rect.top, ellipseWidth, ellipseHeight);
    graphics.DrawEllipse(&pen, rect.left, rect.top, ellipseWidth, ellipseHeight);
}

void Player::Setradius(float r)
{
    radius = r;

    
    float scale = BASE_RADIUS / radius; // ������ ����
    speed = BASE_SPEED * scale;

    // �ӵ� ����
    speed = (std::max)(MIN_SPEED, (std::min)(MAX_SPEED, speed));

    // �浹 ���� ������Ʈ
    rect.left = (int)(mX - radius);
    rect.right = (int)(mX + radius);
    rect.top = (int)(mY - radius);
    rect.bottom = (int)(mY + radius);
}