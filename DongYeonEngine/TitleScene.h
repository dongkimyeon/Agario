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
    Gdiplus::Image* mGameStartImage1;
    Gdiplus::Image* mGameStartImage2;
    Gdiplus::Image* mExitImage1;
    Gdiplus::Image* mExitImage2;
    bool GameStartButtonCheck; 
    bool ExitButtonCheck;
    RECT ExitButton;
	RECT startButton; 

    ULONG_PTR mGdiplusToken;         // GDI+ ÅäÅ«

};