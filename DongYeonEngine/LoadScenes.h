#pragma once
#include "SceneManager.h"
#include "PlayScene.h"
#include "TitleScene.h"
#include "EndScene.h"


void LoadScenes()
{
	SceneManager::CreateScene<PlayScene>(L"PlayScene");
	//SceneManager::CreateScene<EndScene>(L"EndScene");
	SceneManager::CreateScene<TitleScene>(L"TitleScene");
	SceneManager::CreateScene<EndScene>(L"EndScene");

	SceneManager::LoadScene(L"PlayScene");
}