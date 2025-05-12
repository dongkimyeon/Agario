#include "EndScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "PlayScene.h"
#include "Time.h"
#include <cmath>
#include <iomanip> // std::fixed, std::setprecision
#include <sstream> // std::wstringstream

extern const UINT width;
extern const UINT height;

extern int endMin;
extern int endSec;
extern float maxSize;
extern int endEatCnt;

EndScene::EndScene()
    : mBackgroundImage(nullptr)
    , mGameOverimage(nullptr)
    , mGdiplusToken(0)
    , startButton({ 400, 500, 1200, 650 })
    , mCachedBitmap(nullptr)
    , mAnimationTime(0.0f)
    , mAnimationDuration(2.0f) // 애니메이션 지속 시간 (2초)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&mGdiplusToken, &gdiplusStartupInput, nullptr);
}

EndScene::~EndScene()
{
    if (mBackgroundImage != nullptr)
    {
        delete mBackgroundImage;
        mBackgroundImage = nullptr;
    }
    if (mGameOverimage != nullptr)
    {
        delete mGameOverimage;
        mGameOverimage = nullptr;
    }
    if (mCachedBitmap != nullptr)
    {
        delete mCachedBitmap;
        mCachedBitmap = nullptr;
    }
    if (mGdiplusToken != 0)
    {
        Gdiplus::GdiplusShutdown(mGdiplusToken);
        mGdiplusToken = 0;
    }
}

void EndScene::Initialize()
{
    mBackgroundImage = new Gdiplus::Image(L"resources/EndAgario.png");
    mGameOverimage = new Gdiplus::Image(L"resources/GameOver.png");

    mCachedBitmap = new Gdiplus::Bitmap(width, height);
    Gdiplus::Graphics cacheGraphics(mCachedBitmap);
    cacheGraphics.DrawImage(mBackgroundImage, 0, 0, width, height);
    cacheGraphics.DrawImage(mGameOverimage, width / 2 - 330, height / 2 - 270, 237 * 3, 114 * 3);

    // 애니메이션 초기화
    mAnimationTime = 0.0f;
}

void EndScene::Update()
{
    Scene::Update();

    // 애니메이션 시간 업데이트
    mAnimationTime += Time::DeltaTime();
    if (mAnimationTime > mAnimationDuration)
    {
        mAnimationTime = mAnimationDuration; // 애니메이션 끝
    }

    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        mAnimationTime = 0.0f; // 애니메이션 리셋
        SceneManager::LoadScene(L"TitleScene");
        maxSize = 0;
    }
}

void EndScene::LateUpdate()
{
    Scene::LateUpdate();
}

void EndScene::Render(HDC hdc)
{
    Gdiplus::Graphics graphics(hdc);
    if (graphics.GetLastStatus() != Gdiplus::Ok) {
        std::wcout << L"Graphics creation failed" << std::endl;
        return;
    }

    graphics.DrawImage(mCachedBitmap, 0, 0);

  
    Gdiplus::Font font(L"Arial", 40, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush brush(Gdiplus::Color(0, 0, 0));
    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentCenter);

    
    float speed = 800.0f; 
    float targetX = width / 2.0f; 
    float startXLeft = 0.0f;
    float startXRight = (float)width; 

    // 각 텍스트의 Y 위치 (고정)
    float baseY = height / 2.0f + 30.0f;
    float y1 = baseY;
    float y2 = baseY + 60.0f;
    float y3 = baseY + 120.0f;
    float y4 = baseY + 200.0f;

    
    float distance1 = targetX - startXLeft; // 왼쪽 -> 중앙
    float currentX1 = startXLeft + (std::min)(speed * mAnimationTime, distance1);
    float distance2 = startXRight - targetX; // 오른쪽 -> 중앙
    float currentX2 = startXRight - (std::min)(speed * mAnimationTime, distance2);
    float distance3 = targetX - startXLeft; // 왼쪽 -> 중앙
    float currentX3 = startXLeft + (std::min)(speed * mAnimationTime, distance3);
    float distance4 = startXRight - targetX; // 오른쪽 -> 중앙
    float currentX4 = startXRight - (std::min)(speed * mAnimationTime, distance4);


    std::wstringstream wss;
    wss << std::fixed << std::setprecision(2) << maxSize;
    std::wstring sizeStr = L"Highest Radius: " + wss.str();

    std::wstring timeStr = L"Play Time: " + std::to_wstring(endMin) + L"min " + std::to_wstring(endSec) + L"sec";
    std::wstring eatStr = L"Eat Count: " + std::to_wstring(endEatCnt);
    std::wstring info = L"Press Spacebar to Title";

    // 텍스트 렌더링
    graphics.DrawString(timeStr.c_str(), -1, &font, Gdiplus::PointF(currentX1, y1), &format, &brush);
    graphics.DrawString(sizeStr.c_str(), -1, &font, Gdiplus::PointF(currentX2, y2), &format, &brush);
    graphics.DrawString(eatStr.c_str(), -1, &font, Gdiplus::PointF(currentX3, y3), &format, &brush);
    graphics.DrawString(info.c_str(), -1, &font, Gdiplus::PointF(currentX4, y4), &format, &brush);
}