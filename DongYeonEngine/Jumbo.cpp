#include "Jumbo.h"
#include "Input.h"
#include "Time.h"

using namespace Gdiplus;

#define M_PI 3.14159265358979

Jumbo::Jumbo()
{
    std::uniform_real_distribution<float> distAngle(0.0f, 2.0f * M_PI);
    std::random_device gen;

    mX = 0;
    mY = 0;
    rect = { (int)(mX - radius), (int)(mY - radius), (int)(mX + radius), (int)(mY + radius) };
    color = RGB(255, 0, 255);
    radius = 30.0f;
    speed = 100.0f; // �⺻ �ӵ�
    fastSpeed = 200.0f; // �÷��̾� ���� �� ���� �ӵ�
    
    float angle = distAngle(gen);
    dirX = cos(angle);
    dirY = sin(angle);
    playerDetectRadius = 300.0f;
    invincibleTime = 5.0f; // ���� ������ ������ 5�ʰ� ����
    rect = { (int)(mX - radius), (int)(mY - radius), (int)(mX + radius), (int)(mY + radius) };
}

void Jumbo::Update(std::vector<Player>& players)
{
    float currentSpeed = speed; // ���� �ӵ� (�⺻ �ӵ�)
    bool playerDetected = false;

    // ���� �ð� ����
    if (invincibleTime > 0.0f)
    {
        invincibleTime -= Time::DeltaTime();
    }
    else
    {
		invincibleTime = 0.0f; // ���� �ð� ����
    }
    // �÷��̾� ���� �� �÷��̾� ������ �̵�
	if (invincibleTime <= 0.0f)
	{
        for (size_t i = 0; i < players.size(); ++i)
        {
            float dx = players[0].GetPositionX() - mX;
            float dy = players[0].GetPositionY() - mY;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance <= playerDetectRadius)
            {
                // �÷��̾� ����: ���� �ӵ��� �÷��̾� ������ �̵�
                playerDetected = true;
                currentSpeed = fastSpeed;
                dirX = dx / distance;
                dirY = dy / distance;

                float radiusSum = radius + players[0].GetRadius();
                if (distance <= radiusSum)
                {


                }
            }
        }
	}
	
   

    // �÷��̾ �������� ���� ��� ���� �̵�
    if (!playerDetected)
    {
        std::uniform_real_distribution<float> distAngle(0.0f, 2.0f * M_PI);
        std::random_device gen;

        // ȭ�� ��� �浹 �� ���� ���� ����
        bool collision = false;
        if (mX - radius < 0 || mX + radius > 1600)
        {
            collision = true;
            mX = max(radius, min(mX, 1600 - radius)); // ��� �� ��� ����
        }
        if (mY - radius < 0 || mY + radius > 800)
        {
            collision = true;
            mY = max(radius, min(mY, 800 - radius)); // ��� �� ��� ����
        }
        if (collision)
        {
            float angle = distAngle(gen);
            dirX = cos(angle);
            dirY = sin(angle);
        }
    }

    // �ð� ��� �̵�
    float deltaTime = Time::DeltaTime();
    mX += dirX * currentSpeed * deltaTime;
    mY += dirY * currentSpeed * deltaTime;

    // ��Ʈ ������Ʈ
    rect = { (int)(mX - radius), (int)(mY - radius), (int)(mX + radius), (int)(mY + radius) };
}

void Jumbo::LateUpdate()
{
}

void Jumbo::Render(Gdiplus::Graphics& graphics)
{
    
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    // �÷��̾� ���� ���� �� �׸���
    //{
    //    // �������� ������ ������ ���� ���� �ð�ȭ
    //    SolidBrush detectBrush(Color(50, 255, 0, 0)); // ARGB: 50% ����, ������
    //    Pen detectPen(Color(255, 255, 0, 0), 1); // ���� ���� �׵θ� (������ ������)
    //    float detectDiameter = playerDetectRadius * 2;
    //    graphics.FillEllipse(&detectBrush, mX - playerDetectRadius, mY - playerDetectRadius, detectDiameter, detectDiameter);
    //    graphics.DrawEllipse(&detectPen, mX - playerDetectRadius, mY - playerDetectRadius, detectDiameter, detectDiameter);
    //}

    // Jumbo ��ü (�ﰢ��) �׸���
    Color gdiBrushColor(GetRValue(color), GetGValue(color), GetBValue(color));
    SolidBrush brush(gdiBrushColor);
    Color gdiPenColor(GetRValue(color) * 0.6, GetGValue(color) * 0.6, GetBValue(color) * 0.6);
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