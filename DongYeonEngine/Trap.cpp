#include "Trap.h"
#include "Time.h"

using namespace Gdiplus;
#define M_PI 3.14159265358979


Trap::Trap()
{
    std::uniform_real_distribution<float> distAngle(0.0f, 2.0f * M_PI);
    std::uniform_real_distribution<float> distRadius(30.0f, 70.0f);
    std::random_device gen;


	mX = 0;
	mY = 0;
	rect = { (int)(mX - radius), (int)(mY - radius),(int)(mX + radius),(int)(rect.bottom = mY + radius) };
	color = RGB(255, 255, 0);
	radius = (float)distRadius(gen);
	speed = 100.0f;
    // 초기 랜덤 각도 설정
    float angle = distAngle(gen);
    dirX = cos(angle);
    dirY = sin(angle);

    rect = { (int)(mX - radius), (int)(mY - radius), (int)(mX + radius), (int)(mY + radius) };

}

void Trap::Update()
{
    std::uniform_real_distribution<float> distAngle(0.0f, 2.0f * M_PI);
    std::random_device gen;


    float deltaTime = Time::DeltaTime(); // 시간 기반 이동
    mX += dirX * speed * deltaTime;
    mY += dirY * speed * deltaTime;

    // 화면 경계 충돌 처리
    bool collision = false;
    if (mX - radius < 0 || mX + radius > 1600)
    {
        collision = true;
        mX = max(radius, min(mX, 1600 - radius)); // 경계 밖 벗어남 방지
    }

    if (mY - radius < 0 || mY + radius > 800)
    {
        collision = true;
        mY = max(radius, min(mY, 800 - radius)); // 경계 밖 벗어남 방지
    }

    // 충돌 시 랜덤 각도로 방향 설정
    if (collision)
    {
        // 0 ~ 2π 사이의 랜덤 각도 생성
       
        float angle = distAngle(gen);

        // 방향 벡터 계산
        dirX = cos(angle);
        dirY = sin(angle);
    }
    // 렉트 업데이트
    rect = { (int)(mX - radius), (int)(mY - radius), (int)(mX + radius), (int)(mY + radius) };
}

void Trap::LateUpdate()
{
}

void Trap::Render(HDC hdc)
{
    Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetPixelOffsetMode(PixelOffsetModeHalf);

    // 색상 설정
    Color gdiBrushColor(GetRValue(color), GetGValue(color), GetBValue(color));
    SolidBrush brush(gdiBrushColor);
    Color gdiPenColor(GetRValue(color) * 0.6, GetGValue(color) * 0.6, GetBValue(color) * 0.6);
    Pen pen(gdiPenColor, 2);

    // 중심 좌표 및 반지름
    float centerX = mX;
    float centerY = mY;
    float baseRadius = radius;
    float spikeLength = 3.0f; // 뾰족한 부분의 길이
    int numSpikes = 30; // 스파이크 개수

    // 경로 객체 생성
    GraphicsPath path;
    for (int i = 0; i < numSpikes; ++i)
    {
        float angle1 = (float)(i * 2 * M_PI / numSpikes);
        float angle2 = (float)((i + 0.5f) * 2 * M_PI / numSpikes);

        // 바깥쪽 뾰족한 점
        float x1 = centerX + cos(angle1) * baseRadius;
        float y1 = centerY + sin(angle1) * baseRadius;

        float x2 = centerX + cos(angle2) * (baseRadius + spikeLength);
        float y2 = centerY + sin(angle2) * (baseRadius + spikeLength);

        float angle3 = (float)((i + 1) * 2 * M_PI / numSpikes);
        float x3 = centerX + cos(angle3) * baseRadius;
        float y3 = centerY + sin(angle3) * baseRadius;

        // 삼각형으로 스파이크 그리기
        PointF spikePoints[3] = { PointF(x1, y1), PointF(x2, y2), PointF(x3, y3) };
        path.AddPolygon(spikePoints, 3);
    }

  
    // 내부 원
    graphics.FillEllipse(&brush, centerX - radius, centerY - radius, radius * 2, radius * 2);
    graphics.DrawEllipse(&pen, centerX - radius, centerY - radius, radius * 2, radius * 2);

    // 스파이크 테두리 그리기
    graphics.DrawPath(&pen, &path);
}


float Trap::GetPositionX()
{
	return mX;
}

float Trap::GetPositionY()
{
	return mY;
}

float Trap::GetRadius()
{
    return radius;
}


