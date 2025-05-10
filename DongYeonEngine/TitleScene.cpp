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
    // GDI+ �ʱ�ȭ
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

    // GDI+ ����
    if (mGdiplusToken != 0)
    {
        Gdiplus::GdiplusShutdown(mGdiplusToken);
        mGdiplusToken = 0;
    }
}

void TitleScene::Initialize()
{
    // �̹��� �ε�
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

    // ���콺�� ���� ���� ��ư ���� �ִ��� Ȯ��
    GameStartButtonCheck = (mousePos.x >= startButton.left && mousePos.x <= startButton.right &&
        mousePos.y >= startButton.top && mousePos.y <= startButton.bottom);

    // ���콺�� ���� ��ư ���� �ִ��� Ȯ��
    ExitButtonCheck = (mousePos.x >= ExitButton.left && mousePos.x <= ExitButton.right &&
        mousePos.y >= ExitButton.top && mousePos.y <= ExitButton.bottom);

    // ���콺 ���� ��ư Ŭ�� ó��
    if (Input::GetKeyDown(eKeyCode::LButton))
    {
        if (GameStartButtonCheck)
        {
            SceneManager::LoadScene(L"PlayScene");
        }
        else if (ExitButtonCheck)
        {
            PostQuitMessage(0); // ���� ����
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
    // ���� ���� ��ư �̹��� ����
    if (GameStartButtonCheck)
        graphics.DrawImage(mGameStartImage2, width / 2 - 100, height / 2 + 100, 237, 114);
    else
        graphics.DrawImage(mGameStartImage1, width / 2 - 100, height / 2 + 100, 237, 114);

    // ���� ��ư �̹��� ����
    if (ExitButtonCheck)
        graphics.DrawImage(mExitImage2, width / 2 - 100, height / 2 + 200, 237, 114);
    else
        graphics.DrawImage(mExitImage1, width / 2 - 100, height / 2 + 200, 237, 114);
}