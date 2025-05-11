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
    speed = BASE_SPEED; // 기준 반지름에 맞는 속도
    rect = { (int)(mX - radius), (int)(mY - radius), (int)(mX + radius), (int)(mY + radius) };
    color = RGB(8, 136, 248);
    timeSinceSplit = 0.0f;
    isSplit = false;
    boostTime = 0.5f;
    splitTime = 0.0f;
    isBoost;
    isJumbo = false;
}

void Player::Update()
{
    // 점보 상태 지속 시간 관리
    if (isJumbo)
    {
        jumboTime += Time::DeltaTime();
        if (jumboTime >= 5.0f) // 5초 후 점보 상태 해제
        {
            isJumbo = false;
            jumboTime = 0.0f;
            spawnJumbo = true; // 점보 생성 플래그 설정
            Setradius(radius); // 반지름 초기화
        }
    }
    // 분열 중 속도 부스트
    if (isSplit && isBoost)
    {
        timeSinceSplit += Time::DeltaTime();
        if (timeSinceSplit >= boostTime)
        {
            isBoost = false;
            timeSinceSplit = 0.0f;
            isSplit = false;
            Setradius(radius); // 반지름 기반으로 속도 재설정
        }
        else
        {
            // 부스트 속도: 기본 속도의 2배, 최대 속도 제한
            float boostedSpeed = (std::min)(speed * 2.0f, MAX_SPEED);
            speed = boostedSpeed;
        }
    }

    int mouseX = (int)Input::GetMousePosition().x;
    int mouseY = (int)Input::GetMousePosition().y;

    // 플레이어 중심에서 마우스까지 방향 벡터
    float dx = mouseX - mX;
    float dy = mouseY - mY;
    float distance = std::sqrt(dx * dx + dy * dy);

    // 거리가 1보다 크면 이동
    if (distance > 1.0f)
    {
        dx /= distance;
        dy /= distance;
        mX += dx * speed * Time::DeltaTime();
        mY += dy * speed * Time::DeltaTime();
    }
    // 클라이언트 영역(1600x800) 경계 제한
    const int CLIENT_WIDTH = 1600;
    const int CLIENT_HEIGHT = 800;

    // 플레이어 중심(mX, mY)이 경계를 벗어나지 않도록 제한
    mX = (std::max)(radius, (std::min)((float)CLIENT_WIDTH - radius, mX));
    mY = (std::max)(radius, (std::min)((float)CLIENT_HEIGHT - radius, mY));

    // 충돌 영역 업데이트
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

        // Jumbo 본체 (삼각형) 그리기
        Color gdiBrushColor(255, 0, 255);
        SolidBrush brush(gdiBrushColor);
        Color gdiPenColor(255 * 0.6, GetGValue(color) * 0.6, 255 * 0.6);
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

    
    float scale = BASE_RADIUS / radius; // 반지름 비율
    speed = BASE_SPEED * scale;

    // 속도 제한
    speed = (std::max)(MIN_SPEED, (std::min)(MAX_SPEED, speed));

    // 충돌 영역 업데이트
    rect.left = (int)(mX - radius);
    rect.right = (int)(mX + radius);
    rect.top = (int)(mY - radius);
    rect.bottom = (int)(mY + radius);
}