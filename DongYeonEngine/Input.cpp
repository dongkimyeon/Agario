#include "Input.h"
#include "Game.h"	

extern Game GAME;
std::vector<Input::Key> Input::Keys = {};
Vector2 Input::mMousePosition = Vector2::One;
int Input::mWheelDelta = 0;

int ASCII[(UINT)eKeyCode::End] =
{
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
	'Z', 'X', 'C', 'V', 'B', 'N', 'M',
	VK_LEFT, VK_RIGHT, VK_DOWN, VK_UP,
	VK_LBUTTON, VK_MBUTTON, VK_RBUTTON, VK_SPACE,
	0, 0 // WheelUp, WheelDown은 가상 키 코드 없음
};

void Input::Initialize()
{
	createKeys();
}

void Input::Update()
{
	updateKeys();
}

void Input::createKeys()
{
	for (size_t i = 0; i < (UINT)eKeyCode::End; i++)
	{
		Key key = {};
		key.bPressed = false;
		key.state = eKeyState::None;
		key.keyCode = (eKeyCode)i;
		Keys.push_back(key);
	}
}

void Input::updateKeys()
{
	std::for_each(Keys.begin(), Keys.end(),
		[](Key& key) -> void
		{
			updateKey(key);
		});


	if (mWheelDelta != 0)
	{
		if (mWheelDelta > 0)
		{
			Keys[(UINT)eKeyCode::WheelUp].state = eKeyState::Down;
			Keys[(UINT)eKeyCode::WheelUp].bPressed = true;
		}
		else
		{
			Keys[(UINT)eKeyCode::WheelDown].state = eKeyState::Down;
			Keys[(UINT)eKeyCode::WheelDown].bPressed = true;
		}
		mWheelDelta = 0;
	}
	else
	{
		if (Keys[(UINT)eKeyCode::WheelUp].bPressed)
		{
			Keys[(UINT)eKeyCode::WheelUp].state = eKeyState::Up;
			Keys[(UINT)eKeyCode::WheelUp].bPressed = false;
		}
		else
		{
			Keys[(UINT)eKeyCode::WheelUp].state = eKeyState::None;
		}

		if (Keys[(UINT)eKeyCode::WheelDown].bPressed)
		{
			Keys[(UINT)eKeyCode::WheelDown].state = eKeyState::Up;
			Keys[(UINT)eKeyCode::WheelDown].bPressed = false;
		}
		else
		{
			Keys[(UINT)eKeyCode::WheelDown].state = eKeyState::None;
		}
	}
}

void Input::updateKey(Input::Key& key)
{
	if (GetFocus())
	{
		if (key.keyCode != eKeyCode::WheelUp && key.keyCode != eKeyCode::WheelDown)
		{
			if (isKeyDown(key.keyCode))
				updateKeyDown(key);
			else
				updateKeyUp(key);
		}
		getMousePositionByWindow();
	}
	else
	{
		clearKeys();
	}
}

bool Input::isKeyDown(eKeyCode code)
{
	if (code == eKeyCode::WheelUp || code == eKeyCode::WheelDown)
		return false; // 휠은 GetAsyncKeyState로 처리하지 않음
	return GetAsyncKeyState(ASCII[(UINT)code]) & 0x8000;
}

void Input::updateKeyDown(Input::Key& key)
{
	if (key.bPressed == true)
		key.state = eKeyState::Pressed;
	else
		key.state = eKeyState::Down;

	key.bPressed = true;
}

void Input::updateKeyUp(Input::Key& key)
{
	if (key.bPressed == true)
		key.state = eKeyState::Up;
	else
		key.state = eKeyState::None;

	key.bPressed = false;
}

void Input::getMousePositionByWindow()
{
	POINT mousePos = {};
	if (!GetCursorPos(&mousePos))
	{
		OutputDebugStringW(L"Error: GetCursorPos failed\n");
		return;
	}

	HWND hWnd = GAME.GetHwnd();
	if (hWnd == nullptr)
	{
		OutputDebugStringW(L"Error: HWND is nullptr\n");
		return;
	}

	if (!ScreenToClient(hWnd, &mousePos))
	{
		OutputDebugStringW(L"Error: ScreenToClient failed\n");
		return;
	}

	mMousePosition.x = static_cast<float>(mousePos.x);
	mMousePosition.y = static_cast<float>(mousePos.y);
}

void Input::clearKeys()
{
	for (Key& key : Keys)
	{
		if (key.state == eKeyState::Down || key.state == eKeyState::Pressed)
			key.state = eKeyState::Up;
		else if (key.state == eKeyState::Up)
			key.state = eKeyState::None;

		key.bPressed = false;
	}
}

void Input::UpdateMouseWheel(int delta)
{
	mWheelDelta = delta; 
}