#include "SceneManager.h"
#include "Input.h"

std::map<std::wstring, Scene*> SceneManager::mScene = {};
Scene* SceneManager::mActiveScene = nullptr;
Gdiplus::Image* SceneManager::mMouseCursorImage = nullptr; 
static ULONG_PTR gdiplusToken = 0; // GDI+ 토큰
void SceneManager::Initialize()
{
    // GDI+ 초기화
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // 마우스 커서 이미지 초기화
    mMouseCursorImage = new Gdiplus::Image(L"resources/MouseCursor.png");
  
}

void SceneManager::Update()
{
	mActiveScene->Update();
}

void SceneManager::LateUpdate()
{
	mActiveScene->LateUpdate();
}

void SceneManager::Render(HDC hdc)
{
    mActiveScene->Render(hdc);

 
    Gdiplus::Graphics graphics(hdc);
    if (mMouseCursorImage && graphics.GetLastStatus() == Gdiplus::Ok)
    {
        // 원본 이미지 크기 사용
       
        float width = static_cast<float>(mMouseCursorImage->GetWidth()) + 10.0f;
        float height = static_cast<float>(mMouseCursorImage->GetHeight()) + 10.0f;
        Gdiplus::RectF cursorRect(
            static_cast<float>(Input::GetMousePosition().x - 20),
            static_cast<float>(Input::GetMousePosition().y - 20),
            width, height);
        graphics.DrawImage(mMouseCursorImage, cursorRect);
    }
}