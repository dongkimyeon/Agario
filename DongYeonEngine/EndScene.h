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
    ULONG_PTR mGdiplusToken;         // GDI+ ÅäÅ«



};