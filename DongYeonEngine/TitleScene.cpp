#include "TitleScene.h"
#include "SceneManager.h"
#include <cmath>

extern const UINT width;
extern const UINT height;

TitleScene::TitleScene()
    : mBackgroundImage(nullptr)
    , mGdiplusToken(0)
    , startButton({ 400, 500, 1200, 650 })
    , mAlphaTimer(0.0f)
{
    // GDI+ 초기화
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&mGdiplusToken, &gdiplusStartupInput, nullptr);
}

TitleScene::~TitleScene()
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

void TitleScene::Initialize()
{
    // 이미지 로드
    mBackgroundImage = new Gdiplus::Image(L"resources/Agario.png");
}

void TitleScene::Update()
{
    Scene::Update();

    // 스페이스 키로 PlayScene 전환
    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        SceneManager::LoadScene(L"PlayScene");
    }

    // 알파 타이머 업데이트 (페이드 효과를 위해)
    mAlphaTimer += 0.3f; // 속도 조절 (값이 클수록 빠르게 깜빡임)
    if (mAlphaTimer > 2.0f * 3.14159f) // 2π 주기로 반복
        mAlphaTimer -= 2.0f * 3.14159f;
}

void TitleScene::LateUpdate()
{
    Scene::LateUpdate();
}

void TitleScene::Render(HDC hdc)
{
    Scene::Render(hdc);
    Gdiplus::Graphics graphics(hdc);

    // 백그라운드 이미지 그리기 (창 크기에 맞게 스트레치)
    graphics.DrawImage(mBackgroundImage, 0, 0, width, height);

    // 스타트 버튼 사각형 그리기 (디버깅용, 필요 시 주석 해제)
    // Rectangle(hdc, startButton.left, startButton.top, startButton.right, startButton.bottom);

    // "PRESS TO SPACE" 텍스트 그리기
    Gdiplus::Font font(L"Arial", 24, Gdiplus::FontStyleBold);
    Gdiplus::StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter); // 가로 중앙 정렬
    stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter); // 세로 중앙 정렬

    // 투명도 계산 (0~255, 사인파로 부드럽게 변화)
    int alpha = static_cast<int>((std::sin(mAlphaTimer) + 1.0f) * 0.5f * 255);
    Gdiplus::SolidBrush brush(Gdiplus::Color(alpha, 0, 0, 0)); // 검정색, 알파 값 적용

    // 스타트 버튼 영역에 맞춰 텍스트 그리기
    Gdiplus::RectF textRect(
        static_cast<float>(startButton.left),
        static_cast<float>(startButton.top),
        static_cast<float>(startButton.right - startButton.left),
        static_cast<float>(startButton.bottom - startButton.top)
    );

    graphics.DrawString(
        L"PRESS TO SPACE",
        -1,
        &font,
        textRect,
        &stringFormat,
        &brush
    );
}