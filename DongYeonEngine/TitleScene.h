#pragma once
#include "Scene.h"

class TitleScene : public Scene
{
public:
    TitleScene();
    ~TitleScene();

    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC hdc) override;

private:
    Gdiplus::Image* mBackgroundImage;
    Gdiplus::Image* mLogoImage;
    ULONG_PTR mGdiplusToken;         // GDI+ ÅäÅ«

};