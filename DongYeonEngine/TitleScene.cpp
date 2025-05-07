#include "TitleScene.h"
#include "SceneManager.h"
#include <cmath>

extern const UINT width;
extern const UINT height;

TitleScene::TitleScene()
    : mBackgroundImage(nullptr)
    , mGdiplusToken(0)
    , startButton({ 320, 500, 960, 650 })
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
    mBackgroundImage = new Gdiplus::Image(L"resources/AgarioTitle.png");
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
    mAlphaTimer += 0.05f; // 속도 조절 (값이 클수록 빠르게 깜빡임)
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

    // 백그라운드 이미지 그리기
    float bgImgWidth = static_cast<float>(mBackgroundImage->GetWidth());
    float bgImgHeight = static_cast<float>(mBackgroundImage->GetHeight());

    // 스케일링 비율 계산 (창에 맞게 축소)
    float scaleX = static_cast<float>(width / bgImgWidth);
    float scaleY = static_cast<float>(height / bgImgHeight);
    float backGroundScale = min(scaleX, scaleY); // 더 작은 비율 선택 (창 안에 들어가게)

    // 이미지가 창보다 작을 경우 원본 크기 유지
    if (backGroundScale > 1.0f) backGroundScale = 1.0f;

    // 새 이미지 크기 계산
    int destWidth = static_cast<int>(bgImgWidth * backGroundScale);
    int destHeight = static_cast<int>(bgImgHeight * backGroundScale);

    // 중앙 정렬을 위한 오프셋
    int destX = (width - destWidth) / 2;
    int destY = (height - destHeight) / 2;

    // 이미지 렌더링
    graphics.DrawImage(mBackgroundImage, destX, destY, destWidth, destHeight);

    // 스타트 버튼 사각형 그리기

    // "PRESS TO SPACE" 텍스트 그리기
    Gdiplus::Font font(L"Arial", 30, Gdiplus::FontStyleBold);
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