#include "EndScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "PlayScene.h"
#include <cmath>

extern const UINT width;
extern const UINT height;

extern int endMin;
extern int endSec;
extern float maxSize;
extern int endEatCnt;

EndScene::EndScene()
    : mBackgroundImage(nullptr)
    , mGdiplusToken(0)
    , startButton({ 400, 500, 1200, 650 })
    
{
    // GDI+ 초기화
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&mGdiplusToken, &gdiplusStartupInput, nullptr);
}

EndScene::~EndScene()
{
    if (mBackgroundImage != nullptr)
    {
        delete mBackgroundImage; // 이미지 객체 해제
        mBackgroundImage = nullptr;
    }

    // GDI+ 종료
    if (mGdiplusToken != 0)
    {
        Gdiplus::GdiplusShutdown(mGdiplusToken);
        mGdiplusToken = 0;
    }

}

void EndScene::Initialize()
{
    // 이미지 로드
    mBackgroundImage = new Gdiplus::Image(L"resources/EndAgario.png");
    mGameOverimage = new Gdiplus::Image(L"resources/GameOver.png");
}

void EndScene::Update()
{
    Scene::Update();
    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
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
    Scene::Render(hdc);
    Gdiplus::Graphics graphics(hdc);
    if (graphics.GetLastStatus() != Gdiplus::Ok) {
        std::wcout << L"Graphics creation failed" << std::endl;
        return;
    }

    // 배경 이미지 렌더링
    if (mBackgroundImage && mBackgroundImage->GetLastStatus() == Gdiplus::Ok) {
        graphics.DrawImage(mBackgroundImage, 0, 0, width, height);
    }
    else {
        std::wcout << L"Background image not loaded" << std::endl;
    }

    graphics.DrawImage(mGameOverimage, width / 2 - 330, height / 2 - 270, 237 * 3, 114 * 3);
   
    // 폰트 및 브러시 설정
    Gdiplus::Font font(L"Arial", 40, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush brush(Gdiplus::Color(0, 0, 0)); // 빨간색 텍스트
    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentCenter);

    // 텍스트 위치 (고정 위치로 테스트)
    Gdiplus::PointF textPosition(width/2, height/2 + 30);

    // 결과 문자열
    std::wstring timeStr = L"Play Time: " + std::to_wstring(endMin) + L"min " + std::to_wstring(endSec) + L"sec";
    std::wstring sizeStr = L"Highest Radius: " + std::to_wstring(maxSize);
    std::wstring eatStr = L"Eat Count: " + std::to_wstring(endEatCnt);
    std::wstring info = L"Press Spacebar to Title";
    // 텍스트 렌더링
    graphics.DrawString(timeStr.c_str(), -1, &font, textPosition, &format, &brush);
    graphics.DrawString(sizeStr.c_str(), -1, &font, Gdiplus::PointF(textPosition.X, textPosition.Y + 60), &format, &brush);
    graphics.DrawString(eatStr.c_str(), -1, &font, Gdiplus::PointF(textPosition.X, textPosition.Y + 120), &format, &brush);
    graphics.DrawString(info.c_str(), -1, &font, Gdiplus::PointF(textPosition.X, textPosition.Y + 200), &format, &brush);
}