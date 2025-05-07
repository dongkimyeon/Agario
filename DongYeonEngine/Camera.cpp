#include "Camera.h"
#include "GameObject.h"

using namespace Gdiplus;

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::Initialize(GameObject* mTarget)
{
    if (mTarget)
    {
        mLookPos.x = mTarget->GetPositionX();
        mLookPos.y = mTarget->GetPositionY();
        mX = mLookPos.x - (mResolution.x / 2.0f);
        mY = mLookPos.y - (mResolution.y / 2.0f);

        camRange = { static_cast<int>(mX) - 400, static_cast<int>(mY) - 400,
            static_cast<int>(mX) + 400, static_cast<int>(mY) + 400 };
    }
}

void Camera::Update(GameObject* mTarget)
{
    if (mTarget)
    {
        mLookPos.x = mTarget->GetPositionX();
        mLookPos.y = mTarget->GetPositionY();
        mX = mLookPos.x - (mResolution.x / 2.0f);
        mY = mLookPos.y - (mResolution.y / 2.0f);

        camRange = { static_cast<int>(mX) - 400, static_cast<int>(mY) - 400,
            static_cast<int>(mX) + 400, static_cast<int>(mY) + 400 };
    }
}

void Camera::LateUpdate()
{

}

void Camera::Render(HDC hdc)
{
    FrameRect(hdc, &camRange, NULL);
}