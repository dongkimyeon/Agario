#include "EndScene.h"
#include "SceneManager.h"
#include "Input.h"
#include <cmath>

extern const UINT width;
extern const UINT height;

extern int endMin;
extern int endSec;
extern int maxSize;
extern int endEatCnt;

EndScene::EndScene()
    : mBackgroundImage(nullptr)
    , mGdiplusToken(0)
    , startButton({ 400, 500, 1200, 650 })
    
{
    // GDI+ �ʱ�ȭ
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&mGdiplusToken, &gdiplusStartupInput, nullptr);
}

EndScene::~EndScene()
{
    if (mBackgroundImage != nullptr)
    {
        delete mBackgroundImage; // �̹��� ��ü ����
        mBackgroundImage = nullptr;
    }

    // GDI+ ����
    if (mGdiplusToken != 0)
    {
        Gdiplus::GdiplusShutdown(mGdiplusToken);
        mGdiplusToken = 0;
    }
}

void EndScene::Initialize()
{
    // �̹��� �ε�
    mBackgroundImage = new Gdiplus::Image(L"resources/EndAgario.png");
}

void EndScene::Update()
{
    Scene::Update();

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

    // ��� �̹��� ������
    if (mBackgroundImage && mBackgroundImage->GetLastStatus() == Gdiplus::Ok) {
        graphics.DrawImage(mBackgroundImage, 0, 0, width, height);
    }
    else {
        std::wcout << L"Background image not loaded" << std::endl;
    }

   
    // ��Ʈ �� �귯�� ����
    Gdiplus::Font font(L"Arial", 48, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush brush(Gdiplus::Color(0, 0, 0)); // ������ �ؽ�Ʈ
    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentCenter);

    // �ؽ�Ʈ ��ġ (���� ��ġ�� �׽�Ʈ)
    Gdiplus::PointF textPosition(width/2, height/2 - 100 );

    // ��� ���ڿ�
    std::wstring timeStr = L"Play Time: " + std::to_wstring(endMin) + L"min " + std::to_wstring(endSec) + L"sec";
    std::wstring sizeStr = L"Highest Radius: " + std::to_wstring(maxSize);
    std::wstring eatStr = L"Eat Count: " + std::to_wstring(endEatCnt);

    // �ؽ�Ʈ ������
    graphics.DrawString(timeStr.c_str(), -1, &font, textPosition, &format, &brush);
    graphics.DrawString(sizeStr.c_str(), -1, &font, Gdiplus::PointF(textPosition.X, textPosition.Y + 60), &format, &brush);
    graphics.DrawString(eatStr.c_str(), -1, &font, Gdiplus::PointF(textPosition.X, textPosition.Y + 120), &format, &brush);
}