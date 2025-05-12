#include "WhiteHole.h"
#include "Time.h"

using namespace Gdiplus;
#define M_PI 3.14159265358979

// 블랙홀 생성자
WhiteHole::WhiteHole()
{
    std::uniform_real_distribution<float> distAngle(0.0f, 2.0f * M_PI);

    std::random_device gen;


    mWhiteHoleImage = new Gdiplus::Image(L"resources/whitehole.png");


    mX = 0;
    mY = 0;
    radius = 50.0f;
    speed = 70.0f;
    rotationAngle = 0.0f;
    rotationSpeed = 120.0f; // 초당 90도 회전


    float angle = distAngle(gen);
    dirX = cos(angle);
    dirY = sin(angle);

    // 렉트 초기화
    rect = { (int)(mX - radius), (int)(mY - radius), (int)(mX + radius), (int)(mY + radius) };
    color = RGB(255, 255, 0);
}

// 블랙홀 업데이트
void WhiteHole::Update()
{
    std::uniform_real_distribution<float> distAngle(0.0f, 2.0f * M_PI);
    std::random_device gen;

    float deltaTime = Time::DeltaTime();


    mX += dirX * speed * deltaTime;
    mY += dirY * speed * deltaTime;

    rotationAngle += rotationSpeed * deltaTime;
    if (rotationAngle >= 360.0f)
        rotationAngle -= 360.0f;

    bool collision = false;
    if (mX - radius < 0 || mX + radius > 1600)
    {
        collision = true;
        mX = max(radius, min(mX, 1600 - radius));
    }

    if (mY - radius < 0 || mY + radius > 800)
    {
        collision = true;
        mY = max(radius, min(mY, 800 - radius));
    }


    if (collision)
    {
        float angle = distAngle(gen);
        dirX = cos(angle);
        dirY = sin(angle);
    }


    rect = { (int)(mX - radius), (int)(mY - radius), (int)(mX + radius), (int)(mY + radius) };
}


void WhiteHole::LateUpdate()
{
}


void WhiteHole::Render(Gdiplus::Graphics& graphics)
{
    if (mWhiteHoleImage == nullptr)
        return;

    
    float imageWidth = 2.0f * radius;
    float imageHeight = 2.0f * radius;

   
    Bitmap rotatedBitmap((INT)imageWidth, (INT)imageHeight);
    Graphics bitmapGraphics(&rotatedBitmap);
    bitmapGraphics.SetInterpolationMode(InterpolationModeHighQuality); // 고품질 렌더링

    Matrix rotationMatrix;
    rotationMatrix.RotateAt(rotationAngle, PointF(radius, radius)); // 비트맵 중심 (radius, radius)
    bitmapGraphics.SetTransform(&rotationMatrix);


    bitmapGraphics.DrawImage( mWhiteHoleImage,RectF(0, 0, imageWidth, imageHeight));

   
    graphics.DrawImage( &rotatedBitmap,RectF(mX - radius, mY - radius, imageWidth, imageHeight) );
}


float WhiteHole::GetPositionX()
{
    return mX;
}

float WhiteHole::GetPositionY()
{
    return mY;
}


float WhiteHole::GetRadius()
{
    return radius;
}