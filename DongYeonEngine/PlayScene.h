#pragma once
#include "Scene.h"
#include "Player.h"
#include "Food.h"
#include "Enemy.h"
#include "Trap.h"
#include "Jumbo.h"
#include "Camera.h"
#include "SceneManager.h"

class PlayScene : public Scene
{
public:
    PlayScene();
    ~PlayScene();

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(HDC hdc) override;

    void Reset(); // 리셋 메서드 선언

private:
    // 기존 멤버 변수들
    std::vector<Food> foods;
    std::vector<Enemy> enemys;
    std::vector<Trap> traps;
    std::vector<Jumbo> jumbos;
    std::vector<Player> player;

	Gdiplus::Image* mWarnningImage;
    Gdiplus::Image* mWarnningImage2;
	Gdiplus::Image* mWarnningImage3;
	Gdiplus::Image* mWarnningImage4;
	Gdiplus::Image* mWarnningImage5;
    Gdiplus::Image* mZoomtogleImage;
    bool showExclamation = false; // 느낌표 표시 여부
    float exclamationX = 0.0f; // 느낌표 X 좌표
    float exclamationY = 0.0f; // 느낌표 Y 좌표
   
    //카메라 부분 
    float mZoomScale; 
    bool mCamType;
    HDC mMemDC;       // 메모리 DC 
    HBITMAP mMemBitmap; // 메모리 DC용 비트맵
    float mCameraX = 0.0f; // 현재 카메라 X 위치
    float mCameraY = 0.0f; // 현재 카메라 Y 위치

    ULONG_PTR mGdiplusToken;         // GDI+ 토큰
	bool isExclamationPositionSet = false; // 느낌표 위치 설정 여부
    float PlayTime;
    float foodSpawnTimer;
    float enemySpawnTimer;
    float trapSpawnTimer;
    float enemySplitTimer; 
	float jumboDetectTimer;
    float stopTimer;
    int trapCnt;
    int enemyCnt;
    int eatCnt;
    bool printFlag = true;
    bool stopFlag = false;
    bool allowEnemySplit = false;
  
};