#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"
#include <cmath>

extern const UINT width;
extern const UINT height;

TitleScene::TitleScene()
    : mBackgroundImage(nullptr)
    , mGameStartImage1(nullptr)
    , mGameStartImage2(nullptr)
    , mExitImage1(nullptr)
    , mExitImage2(nullptr)
    , GameStartButtonCheck(false)
    , ExitButtonCheck(false)
    , startButton({ static_cast<LONG>(width / 2 - 100), static_cast<LONG>(height / 2 + 120), static_cast<LONG>(width / 2 + 133), static_cast<LONG>(height / 2 + 190) })
    , ExitButton({ static_cast<LONG>(width / 2 - 90), static_cast<LONG>(height / 2 + 220), static_cast<LONG>(width / 2 + 115), static_cast<LONG>(height / 2 + 290) })
    , mGdiplusToken(0)
{
    // GDI+ 초기화
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&mGdiplusToken, &gdiplusStartupInput, nullptr);
}

TitleScene::~TitleScene()
{
    if (mBackgroundImage != nullptr)
    {
        delete mBackgroundImage;
        mBackgroundImage = nullptr;
    }
    if (mGameStartImage1 != nullptr)
    {
        delete mGameStartImage1;
        mGameStartImage1 = nullptr;
    }
    if (mGameStartImage2 != nullptr)
    {
        delete mGameStartImage2;
        mGameStartImage2 = nullptr;
    }
    if (mExitImage1 != nullptr)
    {
        delete mExitImage1;
        mExitImage1 = nullptr;
    }
    if (mExitImage2 != nullptr)
    {
        delete mExitImage2;
        mExitImage2 = nullptr;
    }
    if (mLogoImage != nullptr)
    {
        delete mExitImage2;
        mLogoImage = nullptr;
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
    mBackgroundImage = new Gdiplus::Image(L"resources/EndAgario.png");
    mLogoImage = new Gdiplus::Image(L"resources/AgarioLogo.png");
    mGameStartImage1 = new Gdiplus::Image(L"resources/GameStartButton1.png");
    mGameStartImage2 = new Gdiplus::Image(L"resources/GameStartButton2.png");
    mExitImage1 = new Gdiplus::Image(L"resources/ExitButton1.png");
    mExitImage2 = new Gdiplus::Image(L"resources/ExitButton2.png");
}

void TitleScene::Update()
{
    Scene::Update();

    Vector2 mousePos = Input::GetMousePosition();

    // 마우스가 게임 시작 버튼 위에 있는지 확인
    GameStartButtonCheck = (mousePos.x >= startButton.left && mousePos.x <= startButton.right &&
        mousePos.y >= startButton.top && mousePos.y <= startButton.bottom);

    // 마우스가 종료 버튼 위에 있는지 확인
    ExitButtonCheck = (mousePos.x >= ExitButton.left && mousePos.x <= ExitButton.right &&
        mousePos.y >= ExitButton.top && mousePos.y <= ExitButton.bottom);

    // 마우스 왼쪽 버튼 클릭 처리
    if (Input::GetKeyDown(eKeyCode::LButton))
    {
        if (GameStartButtonCheck)
        {
            SceneManager::LoadScene(L"PlayScene");
        }
        else if (ExitButtonCheck)
        {
            PostQuitMessage(0); // 게임 종료
        }
    }
}

void TitleScene::LateUpdate()
{
    Scene::LateUpdate();
}

void TitleScene::Render(HDC hdc)
{
    Scene::Render(hdc);
    Gdiplus::Graphics graphics(hdc);

	Rectangle(hdc, startButton.left, startButton.top, startButton.right, startButton.bottom);
	Rectangle(hdc, ExitButton.left, ExitButton.top, ExitButton.right, ExitButton.bottom);
   
    graphics.DrawImage(mBackgroundImage, 0, 0, width, height);

    graphics.DrawImage(mLogoImage, width / 2 - 330, height / 2 -200 , 237 *3, 114*3);
    // 게임 시작 버튼 이미지 선택
    if (GameStartButtonCheck)
        graphics.DrawImage(mGameStartImage2, width / 2 - 100, height / 2 + 100, 237, 114);
    else
        graphics.DrawImage(mGameStartImage1, width / 2 - 100, height / 2 + 100, 237, 114);

    // 종료 버튼 이미지 선택
    if (ExitButtonCheck)
        graphics.DrawImage(mExitImage2, width / 2 - 100, height / 2 + 200, 237, 114);
    else
        graphics.DrawImage(mExitImage1, width / 2 - 100, height / 2 + 200, 237, 114);
}