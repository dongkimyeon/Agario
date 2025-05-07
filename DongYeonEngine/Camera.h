#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Player.h"

class Camera :
    public Scene
{
public:
    Vector2 CalPos(Vector2 pos) { return pos - Vector2(mX, mY); };


    Camera();
    ~Camera();
    void Initialize(GameObject* mTarget);
    void Update(GameObject* mTarget);
    void LateUpdate() override;
    void Render(HDC hdc) override;

private:
    RECT camRange;
    float mX;
    float mY;
    Vector2 mResolution;
    Vector2 mLookPos;
    GameObject* mTarget;
    Player mTargetToPlayer;
};

