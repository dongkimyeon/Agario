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
    // GDI+ �ʱ�ȭ
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&mGdiplusToken, &gdiplusStartupInput, nullptr);
}

TitleScene::~TitleScene()
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

void TitleScene::Initialize()
{
    // �̹��� �ε�
    mBackgroundImage = new Gdiplus::Image(L"resources/AgarioTitle.png");
}

void TitleScene::Update()
{
    Scene::Update();

    // �����̽� Ű�� PlayScene ��ȯ
    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        SceneManager::LoadScene(L"PlayScene");
    }

    // ���� Ÿ�̸� ������Ʈ (���̵� ȿ���� ����)
    mAlphaTimer += 0.05f; // �ӵ� ���� (���� Ŭ���� ������ ������)
    if (mAlphaTimer > 2.0f * 3.14159f) // 2�� �ֱ�� �ݺ�
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

    // ��׶��� �̹��� �׸���
    float bgImgWidth = static_cast<float>(mBackgroundImage->GetWidth());
    float bgImgHeight = static_cast<float>(mBackgroundImage->GetHeight());

    // �����ϸ� ���� ��� (â�� �°� ���)
    float scaleX = static_cast<float>(width / bgImgWidth);
    float scaleY = static_cast<float>(height / bgImgHeight);
    float backGroundScale = min(scaleX, scaleY); // �� ���� ���� ���� (â �ȿ� ����)

    // �̹����� â���� ���� ��� ���� ũ�� ����
    if (backGroundScale > 1.0f) backGroundScale = 1.0f;

    // �� �̹��� ũ�� ���
    int destWidth = static_cast<int>(bgImgWidth * backGroundScale);
    int destHeight = static_cast<int>(bgImgHeight * backGroundScale);

    // �߾� ������ ���� ������
    int destX = (width - destWidth) / 2;
    int destY = (height - destHeight) / 2;

    // �̹��� ������
    graphics.DrawImage(mBackgroundImage, destX, destY, destWidth, destHeight);

    // ��ŸƮ ��ư �簢�� �׸���
   // Rectangle(hdc, startButton.left, startButton.top, startButton.right, startButton.bottom);

    // "PRESS TO SPACE" �ؽ�Ʈ �׸���
    Gdiplus::Font font(L"Arial", 24, Gdiplus::FontStyleBold);
    Gdiplus::StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter); // ���� �߾� ����
    stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter); // ���� �߾� ����

    // ���� ��� (0~255, �����ķ� �ε巴�� ��ȭ)
    int alpha = static_cast<int>((std::sin(mAlphaTimer) + 1.0f) * 0.5f * 255);
    Gdiplus::SolidBrush brush(Gdiplus::Color(alpha, 0, 0, 0)); // ������, ���� �� ����

    // ��ŸƮ ��ư ������ ���� �ؽ�Ʈ �׸���
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