#include "Enemy.h"
#include "Time.h"

using namespace Gdiplus;

Enemy::Enemy()
{
	mX = 0;
	mY = 0;
	rect = { (int)(mX - radius), (int)(mY - radius),(int)(mX + radius),(int)(rect.bottom = mY + radius) };
	color = RGB(255, 0, 0);
    radius = 30.0f;
    detectPlayerRange = radius * 2.5f;
	PlayerDetect = false;
    leader = false;
	speed = 100.0f;
	splitTime = 0.0f;
}

void Enemy::Update(std::vector<Food>& foods, std::vector<Player>& players, std::vector<Enemy>& enemies)
{
    // 충돌 사각형 업데이트
    rect.left = mX - radius;
    rect.right = mX + radius;
    rect.top = mY - radius;
    rect.bottom = mY + radius;

    if (foods.empty()) return; // 추적할 음식 없음

    bool playerDetected = false;
    Player* closestPlayer = nullptr;
    float closestPlayerDistance = FLT_MAX;

    // 모든 적(리더, 비리더 상관없이) 플레이어 감지
    for (auto& player : players)
    {
        float dx = player.GetPositionX() - mX;
        float dy = player.GetPositionY() - mY;
        float distance = std::sqrt(dx * dx + dy * dy);

        float radiusSum = player.GetRadius() + detectPlayerRange;
        if (distance <= radiusSum)
        {
            playerDetected = true;
            if (distance < closestPlayerDistance)
            {
                closestPlayerDistance = distance;
                closestPlayer = &player;
            }
        }
    }

    // 비리더가 플레이어를 감지했을 경우 리더 변경
    if (playerDetected && !leader && closestPlayer)
    {
        // 기존 리더 찾아서 리더 상태 해제
        for (auto& enemy : enemies)
        {
            if (enemy.GetId() == id && enemy.GetLeaderFlag())
            {
                enemy.SetLeader(false); // 기존 리더의 리더 상태 제거
                break;
            }
        }
        // 현재 객체를 리더로 설정
        SetLeader(true);
    }

    if (leader)
    {
        // 가장 가까운 음식 찾기
        float closestDistance = FLT_MAX;
        Food* closestFood = nullptr;
        for (Food& food : foods)
        {
            float dx = food.GetPositionX() - mX;
            float dy = food.GetPositionY() - mY;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance < closestDistance)
            {
                closestDistance = distance;
                closestFood = &food;
            }
        }

        // 방향 설정 
        if (playerDetected && closestPlayer)
        {
            float dx = closestPlayer->GetPositionX() - mX;
            float dy = closestPlayer->GetPositionY() - mY;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance > 0.1f) // 0으로 나누기 방지
            {
                float directionX = dx / distance;
                float directionY = dy / distance;
                SetDirection(directionX, directionY);
            }
        }
        else if (closestFood)
        {
            float dx = closestFood->GetPositionX() - mX;
            float dy = closestFood->GetPositionY() - mY;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance > 0.1f)
            {
                float directionX = dx / distance;
                float directionY = dy / distance;
                SetDirection(directionX, directionY);
            }
        }
    }
    else // 비리더: 같은 ID를 가진 리더 따라가기
    {
        Enemy* leaderEnemy = nullptr;
        for (auto& enemy : enemies)
        {
            if (enemy.GetId() == id && enemy.GetLeaderFlag())
            {
                leaderEnemy = &enemy;
                break;
            }
        }

        if (leaderEnemy)
        {
            float dx = leaderEnemy->GetPositionX() - mX;
            float dy = leaderEnemy->GetPositionY() - mY;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance > 0.1f) // 0으로 나누기 방지
            {
                float directionX = dx / distance;
                float directionY = dy / distance;
                SetDirection(directionX, directionY);
            }
        }
    }

    // 위치 업데이트 (리더와 비리더 모두)
    float moveSpeed = speed * Time::DeltaTime();
    mX += dirX * moveSpeed;
    mY += dirY * moveSpeed;

    // 클라이언트 영역(1600x800) 경계 제한
    const int CLIENT_WIDTH = 1600;
    const int CLIENT_HEIGHT = 800;

    // 적 중심(mX, mY)이 경계를 벗어나지 않도록 제한
    mX = (std::max)(radius, (std::min)((float)CLIENT_WIDTH - radius, mX));
    mY = (std::max)(radius, (std::min)((float)CLIENT_HEIGHT - radius, mY));

    // 충돌 사각형 업데이트
    rect.left = (int)(mX - radius);
    rect.right = (int)(mX + radius);
    rect.top = (int)(mY - radius);
    rect.bottom = (int)(mY + radius);
}

void Enemy::LateUpdate()
{

}

void Enemy::Render(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetPixelOffsetMode(PixelOffsetModeHalf);
	// GDI+ 색상 객체 생성
    Color gdiBrushColor(GetRValue(color), GetGValue(color), GetBValue(color));
    SolidBrush brush(gdiBrushColor);
    Color gdiPenColor(GetRValue(color) * 0.6, GetGValue(color) * 0.6, GetBValue(color) * 0.6);
    Pen pen(gdiPenColor, 4);
	// 디버깅용 사각형(충돌영역)
	//Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

	// 사각형 안에 원 그리기
	INT ellipseWidth = rect.right - rect.left; //너비
	INT ellipseHeight = rect.bottom - rect.top; //높이
	graphics.FillEllipse(&brush, rect.left, rect.top, ellipseWidth, ellipseHeight);
    graphics.DrawEllipse(&pen, rect.left, rect.top, ellipseWidth, ellipseHeight);

    // 플레이어 감지 범위 렌더링 (빨간 선)
    Pen detectPen(Color(255, 0, 0), 2); // 빨간색, 두께 2
    float detectDiameter = detectPlayerRange * 2.0f;
    float detectLeft = mX - detectPlayerRange;
    float detectTop = mY - detectPlayerRange;
    graphics.DrawEllipse(&detectPen, detectLeft, detectTop, detectDiameter, detectDiameter);

}

void Enemy::Setradius(float r)
{
    radius = r;

    float scale = BASE_RADIUS / radius; // 반지름 비율
    speed = BASE_SPEED * scale;
	detectPlayerRange = radius * 2.5f;
    // 속도 제한
    speed = (std::max)(MIN_SPEED, (std::min)(MAX_SPEED, speed));

    // 충돌 영역 업데이트
    rect.left = (int)(mX - radius);
    rect.right = (int)(mX + radius);
    rect.top = (int)(mY - radius);
    rect.bottom = (int)(mY + radius);
}



float Enemy::GetPositionX()
{
	return mX;
}

float Enemy::GetPositionY()
{
	return mY;
}

float Enemy::GetSpeed()
{
	return speed;
}

COLORREF Enemy::GetColor()
{
	return color;
}

float Enemy::GetRadius()
{
	return radius;
}

RECT Enemy::GetRect()
{
	return rect;
}

