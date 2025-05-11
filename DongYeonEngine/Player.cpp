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
    splitTime = 0.0f;
    isBoost;
    isJumbo = false;
}

void Player::Update(float zoomScale, float cameraX, float cameraY, float clientWidth, float clientHeight)
{
    // ���� ���� ���� �ð� ����
    if (isJumbo)
    {
        jumboTime += Time::DeltaTime();
        if (jumboTime >= 5.0f)
        {
            isJumbo = false;
            jumboTime = 0.0f;
            spawnJumbo = true;
            Setradius(radius);
        }
    }
    // �п� �� �ӵ� �ν�Ʈ
    if (isSplit && isBoost)
    {
        timeSinceSplit += Time::DeltaTime();
        if (timeSinceSplit >= boostTime)
        {
            isBoost = false;
            timeSinceSplit = 0.0f;
            isSplit = false;
            Setradius(radius);
        }
        else
        {
            float boostedSpeed = (std::min)(speed * 2.0f, MAX_SPEED);
            speed = boostedSpeed;
        }
    }

    // ���콺 ��ǥ ��� (��ũ�� ��ǥ)
    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(GetActiveWindow(), &mouse); // ��ũ�� ��ǥ�� Ŭ���̾�Ʈ ��ǥ�� ��ȯ
    float mouseX = static_cast<float>(mouse.x);
    float mouseY = static_cast<float>(mouse.y);

    // ���콺 ��ǥ�� ���� ��ǥ�� ��ȯ
    float worldX = cameraX + (mouseX - clientWidth / 2.0f) / zoomScale;
    float worldY = cameraY + (mouseY - clientHeight / 2.0f) / zoomScale;

    // �÷��̾� �߽ɿ��� ���콺���� ���� ����
    float dx = worldX - mX;
    float dy = worldY - mY;
    float distance = std::sqrt(dx * dx + dy * dy);

    // �Ÿ��� 1���� ũ�� �̵�
    if (distance > 1.0f)
    {
        dx /= distance;
        dy /= distance;
        mX += dx * speed * Time::DeltaTime();
        mY += dy * speed * Time::DeltaTime();
    }

    // Ŭ���̾�Ʈ ����(1600x800) ��� ����
    const float WORLD_WIDTH = 1600.0f;
    const float WORLD_HEIGHT = 800.0f;

    // �÷��̾� �߽�(mX, mY)�� ���� ��踦 ����� �ʵ��� ����
    mX = (std::max)(radius, (std::min)(WORLD_WIDTH - radius, mX));
    mY = (std::max)(radius, (std::min)(WORLD_HEIGHT - radius, mY));

    // �浹 ���� ������Ʈ
    rect.left = (int)(mX - radius);
    rect.right = (int)(mX + radius);
    rect.top = (int)(mY - radius);
    rect.bottom = (int)(mY + radius);
}

void Player::LateUpdate()
{
}

void Player::Render(Gdiplus::Graphics& graphics)
{
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    if (!isJumbo)
    {
        Color gdiBrushColor(GetRValue(color), GetGValue(color), GetBValue(color));
        SolidBrush brush(gdiBrushColor);
        Color gdiPenColor(GetRValue(color) * 0.6, GetGValue(color) * 0.6, GetBValue(color) * 0.6);
        Pen pen(gdiPenColor, 4);

        INT ellipseWidth = rect.right - rect.left;
        INT ellipseHeight = rect.bottom - rect.top;
        graphics.FillEllipse(&brush, rect.left, rect.top, ellipseWidth, ellipseHeight);
        graphics.DrawEllipse(&pen, rect.left, rect.top, ellipseWidth, ellipseHeight);
    }
    else
    {
        Color gdiBrushColor(255, 0, 255);
        SolidBrush brush(gdiBrushColor);
        Color gdiPenColor(255 * 0.6, 0, 255 * 0.6);
        Pen pen(gdiPenColor, 4);

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