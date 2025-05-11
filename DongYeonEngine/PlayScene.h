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

    void Reset(); // ���� �޼��� ����

private:
    // ���� ��� ������
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
    bool showExclamation = false; // ����ǥ ǥ�� ����
    float exclamationX = 0.0f; // ����ǥ X ��ǥ
    float exclamationY = 0.0f; // ����ǥ Y ��ǥ
   
    //ī�޶� �κ� 
    float mZoomScale; 
    bool mCamType;
    HDC mMemDC;       // �޸� DC 
    HBITMAP mMemBitmap; // �޸� DC�� ��Ʈ��
    float mCameraX = 0.0f; // ���� ī�޶� X ��ġ
    float mCameraY = 0.0f; // ���� ī�޶� Y ��ġ

    ULONG_PTR mGdiplusToken;         // GDI+ ��ū
	bool isExclamationPositionSet = false; // ����ǥ ��ġ ���� ����
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