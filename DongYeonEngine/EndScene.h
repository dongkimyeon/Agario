#pragma once
#include "Scene.h"

class EndScene : public Scene
{
public:
    EndScene();
    ~EndScene();

    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC hdc) override;

private:
    Gdiplus::Image* mBackgroundImage;
    Gdiplus::Image* mGameOverimage;
    RECT startButton;
    Gdiplus::Bitmap* mCachedBitmap;
    ULONG_PTR mGdiplusToken;         // GDI+ 토큰

    float mAnimationTime; // 현재 애니메이션 시간
    float mAnimationDuration; // 애니메이션 지속 시간

};