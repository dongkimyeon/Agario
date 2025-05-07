#pragma once
#include "GameObject.h"
#include "Scene.h"

class Transform :
    public Scene
{
public:
    Transform();
    ~Transform();

    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC hdc) override;
};

