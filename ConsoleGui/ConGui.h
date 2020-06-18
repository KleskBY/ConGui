#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
using namespace std;

/*
	ConGui v0.3
	Author: KleskBY
	Date: 6/18/2020
	Release date: 12/12/2019
*/

namespace ConGui
{

#pragma region Globals
	static HANDLE hConsole = NULL;
	static HWND WindowHWND = NULL;
	static RECT WindowRect;

	static int WindowWidth = 600;
	static int WindowHeight = 400;
	static int ConsoleWidth = 18;
	static int ConsoleHeight = 18;

	static int CursorX = 0;
	static int CursorY = 0;
	static int SelectedBlock = 0;
	static int FocusedBlock = 0;

	static int LastX = 0;
	static int LastY = 0;

	static DWORD dwTick = 0;
	static DWORD dwBytesWritten = 0;
	static wchar_t* screen;
#pragma endregion

#pragma region Style

	namespace Style
	{
		static enum
		{
			BoxStyle_Default,
			BoxStyle_Single,
			BoxStyle_Corners,
			BoxStyle_Corners2,
			BoxStyle_Combo,
			BoxStyle_Combo2,
		};
		static int BoxStyle = BoxStyle_Default;

		static enum
		{
			CheckBoxStyle_ShortBox,
			CheckBoxStyle_FullBox,
			CheckBoxStyle_BigBox,
			CheckBoxStyle_Round,
			CheckBoxStyle_Rect
		};
		static int CheckBoxStyle = CheckBoxStyle_Rect;

		static WORD Button = 7;
		static WORD ButtonHovered = 2;
		static WORD ButtonActive = 10;//42;
		static bool ButtonFill = true;

		static WORD Link = 7;
		static WORD LinkHovered = 2;
		static WORD LinkActive = 10;

		static WORD SliderGrab = 153;
		static WORD SliderGrabHovered = 53;
		static WORD SliderGrabActive = 53;

		static WORD Slider = 68;
		static WORD SliderHovered = 204;
		static WORD SliderActive = 204;

		static WORD InputText = 240;
		static WORD InputTextText = 0;
	}

#pragma endregion

#pragma region Initialization
	static void ApplyConsoleStyle()
	{
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;                   // Width of each character in the font
		cfi.dwFontSize.Y = 20;                  // Height
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_BOLD;
		wcscpy_s(cfi.FaceName, LF_FACESIZE, L"Consolas");
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	}
	static void PrepareFrame()
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);

		WindowHWND = GetConsoleWindow();
		MoveWindow(WindowHWND, (desktop.right / 2) - (WindowWidth / 2), (desktop.bottom / 2) - (WindowHeight / 2), WindowWidth, WindowHeight, TRUE); //Move to center and ReSize
		SetWindowPos(WindowHWND, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); //Always on TOP
		SetWindowLong(WindowHWND, GWL_STYLE, GetWindowLong(WindowHWND, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX); //Disable ReSize

	}
	static void Init()
	{
		hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleActiveScreenBuffer(hConsole);
		SetConsoleMode(hConsole, ENABLE_EXTENDED_FLAGS | ~ENABLE_QUICK_EDIT_MODE);
	}
	static void SetCursorState(bool enabled)
	{
		CONSOLE_CURSOR_INFO     cursorInfo;
		GetConsoleCursorInfo(hConsole, &cursorInfo);
		cursorInfo.bVisible = enabled;
		SetConsoleCursorInfo(hConsole, &cursorInfo);
	}
	static void Frame()
	{
		GetWindowRect(WindowHWND, &WindowRect);
		WindowWidth = WindowRect.right - WindowRect.left;
		WindowHeight = WindowRect.bottom - WindowRect.top;

		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
		GetConsoleScreenBufferInfo(hConsole, &csbiInfo);

		ConsoleWidth = csbiInfo.dwMaximumWindowSize.X;
		ConsoleHeight = csbiInfo.dwMaximumWindowSize.Y;

		screen = new wchar_t[ConsoleWidth * ConsoleHeight];
		for (int i = 0; i < ConsoleWidth * ConsoleHeight; i++) screen[i] = ' ';
	}
	static void InputHandle()
	{
		POINT pt;
		GetCursorPos(&pt);
		int mouseX = (pt.x - WindowRect.left);
		int mouseY = (pt.y - WindowRect.top) * 0.98f;

		int CoefX = WindowWidth / ConsoleWidth;
		int CoefY = WindowHeight / ConsoleHeight;

		if (CoefX == 0 || CoefY == 0) return;
		CursorX = mouseX / CoefX;
		CursorY = mouseY / CoefY;

		FocusedBlock = CursorY * ConsoleWidth + CursorX;
		SelectedBlock = 0;
		while (GetAsyncKeyState(VK_LBUTTON) && GetForegroundWindow() == WindowHWND)
		{
			if (CursorX < 0 || CursorY < 0) return;
			SelectedBlock = CursorY * ConsoleWidth + CursorX;
			FocusedBlock = 0;
		}
	}
	static void Render()
	{
		int screenlen = wcslen(screen);
		screen[ConsoleWidth * ConsoleHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, screenlen, { 0,0 }, &dwBytesWritten);
	}

#pragma endregion

#pragma region UIElements

	static void Text(short x, short y, const char* text, WORD color = 0)
	{
		for (int i = 0; i < strlen(text); i++)
		{
			if (color > 0)
			{
				DWORD written;
				std::vector<WORD> attributes(strlen(text), color);
				WriteConsoleOutputAttribute(hConsole, &attributes[0], strlen(text), { x,y }, &written);
			}
			screen[y * ConsoleWidth + x + i] = text[i];
		}
	}
	static void Text(short x, short y, const wchar_t* text, WORD color = 0)
	{
		for (int i = 0; i < wcslen(text); i++)
		{
			if (color > 0)
			{
				DWORD written;
				std::vector<WORD> attributes(wcslen(text), color);
				WriteConsoleOutputAttribute(hConsole, &attributes[0], wcslen(text), { x,y }, &written);
			}
			screen[y * ConsoleWidth + x + i] = text[i];
		}
	}

	static void Box(short x, short y, short x2, short y2, WORD color = 0, bool fill = true)
	{
		int w = abs(x2 - x);
		int h = abs(y2 - y);

		int LeftUpCorner = 0x2554;
		int RightUpCorner = 0x2557;
		int LeftDownCorner = 0x255A;
		int RightDownCorner = 0x255D;
		int Horizontal = 0x2550;
		int Vertical = 0x2551;
		if (Style::BoxStyle == Style::BoxStyle_Single)
		{
			LeftUpCorner = 0x250C; 
			LeftDownCorner = 0x2514;
			RightUpCorner = 0x2510;
			RightDownCorner = 0x2518;
			Vertical = 0x2502;
			Horizontal = 0x2500;
		}
		else if (Style::BoxStyle == Style::BoxStyle_Corners)
		{
			LeftUpCorner = 0x250C;
			LeftDownCorner = 0x2514;
			RightUpCorner = 0x2510;
			RightDownCorner = 0x2518;
			Vertical = ' ';
			Horizontal = ' ';
		}
		else if (Style::BoxStyle == Style::BoxStyle_Corners2)
		{
			Vertical = ' ';
			Horizontal = ' ';
		}
		else if (Style::BoxStyle == Style::BoxStyle_Combo)
		{
			Horizontal = 0x2500;
		}
		else if (Style::BoxStyle == Style::BoxStyle_Combo2)
		{
			Vertical = 0x2502;
			Horizontal = 0x2500;
		}


		//Horiozntal
		for (int i = 1; i < w; i++)
		{
			if (color > 0)
			{
				DWORD written;
				std::vector<WORD> attributes(1, color);
				WriteConsoleOutputAttribute(hConsole, &attributes[0], 1, { short(x + i),y }, &written);
			}
			screen[y * ConsoleWidth + x + i] = Horizontal;
		}
		for (int i = 1; i < w; i++)
		{
			if (color > 0)
			{
				DWORD written;
				std::vector<WORD> attributes(1, color);
				WriteConsoleOutputAttribute(hConsole, &attributes[0], 1, { short(x + i),short(y + h) }, &written);
			}
			screen[(y + h) * ConsoleWidth + x + i] = Horizontal;
		}

		//Vertical
		for (int i = 1; i < h; i++)
		{
			if (color > 0)
			{
				DWORD written;
				std::vector<WORD> attributes(1, color);
				WriteConsoleOutputAttribute(hConsole, &attributes[0], 1, { x,short(y + i) }, &written);
			}
			screen[(y + i) * ConsoleWidth + x] = Vertical;
		}
		for (int i = 1; i < h; i++)
		{
			if (color > 0)
			{
				DWORD written;
				std::vector<WORD> attributes(1, color);
				WriteConsoleOutputAttribute(hConsole, &attributes[0], 1, { short(x + w),short(y + i) }, &written);
			}
			screen[(y + i) * ConsoleWidth + x + w] = Vertical;
		}

		//Corners
		screen[y * ConsoleWidth + x] = LeftUpCorner;
		screen[y * ConsoleWidth + x + w] = RightUpCorner;
		screen[(y + h) * ConsoleWidth + x] = LeftDownCorner;
		screen[(y + h) * ConsoleWidth + x + w] = RightDownCorner;

		if (color > 0)
		{
			DWORD written;
			std::vector<WORD> attributes(1, color);
			WriteConsoleOutputAttribute(hConsole, &attributes[0], 1, { short(x), short(y) }, &written);
			WriteConsoleOutputAttribute(hConsole, &attributes[0], 1, { short(x + w), short(y) }, &written);
			WriteConsoleOutputAttribute(hConsole, &attributes[0], 1, { short(x), short(y + h) }, &written);
			WriteConsoleOutputAttribute(hConsole, &attributes[0], 1, { short(x + w), short(y + h) }, &written);

			if (fill)
			{
				std::vector<WORD> attributes2(w, color);
				for (int i = 1; i < h; i++)
				{
					WriteConsoleOutputAttribute(hConsole, &attributes2[0], w, { short(x), short(y + i) }, &written);
				}
			}
		}
	}

	static bool Link(const char* text, short x, short y)
	{
		bool pressed = false;
		WORD NewColor = Style::Link;
		for (int winx = 0; winx < ConsoleWidth; winx++)
		{
			for (int winy = 0; winy < ConsoleHeight; winy++)
			{
				if (winx >= x && winx <= (x + strlen(text)) && winy == y)
				{
					if (SelectedBlock == (winy * ConsoleWidth + winx))
					{
						//SetForegroundWindow(GetDesktopWindow());
						LastX = x;
						LastY = y;
						pressed = true;
						NewColor = Style::LinkActive;
					}
					else if (FocusedBlock == (winy * ConsoleWidth + winx))
					{
						NewColor = Style::LinkHovered;
					}
				}
			}
		}
		Text(x, y, text, NewColor);
		return pressed;
	}
	static bool Link(const wchar_t* text, short x, short y)
	{
		bool pressed = false;
		WORD NewColor = Style::Link;
		for (int winx = 0; winx < ConsoleWidth; winx++)
		{
			for (int winy = 0; winy < ConsoleHeight; winy++)
			{
				if (winx >= x && winx <= (x + wcslen(text)) && winy == y)
				{
					if (SelectedBlock == (winy * ConsoleWidth + winx))
					{
						LastX = x;
						LastY = y;
						pressed = true;
						NewColor = Style::LinkActive;
					}
					else if (FocusedBlock == (winy * ConsoleWidth + winx))
					{
						NewColor = Style::LinkHovered;
					}
				}
			}
		}
		Text(x, y, text, NewColor);
		return pressed;
	}
	static bool InvisibleButton(short x, short y, short x2, short y2)
	{
		bool pressed = false;
		for (int winx = 0; winx < ConsoleWidth; winx++)
		{
			for (int winy = 0; winy < ConsoleHeight; winy++)
			{
				if (winx >= x && winx <= x2 && winy >= y && winy <= y2)
				{
					if (SelectedBlock == (winy * ConsoleWidth + winx))
					{
						pressed = true;
					}
				}
			}
		}
		return pressed;
	}
	static bool Button(const char* text, short x, short y, short x2, short y2)
	{
		bool pressed = false;
		WORD NewColor = Style::Button;
		//Controls
		for (int winx = 0; winx < ConsoleWidth; winx++)
		{
			for (int winy = 0; winy < ConsoleHeight; winy++)
			{
				if (winx >= x && winx <= x2 && winy >= y && winy <= y2)
				{
					if (SelectedBlock == (winy * ConsoleWidth + winx))
					{
						//SetForegroundWindow(GetDesktopWindow());
						LastX = x;
						LastY = y;
						pressed = true;
						NewColor = Style::ButtonActive;
					}
					else if (FocusedBlock == (winy * ConsoleWidth + winx)) NewColor = Style::ButtonHovered;
				}
			}
		}
		Box(x, y, x2, y2, NewColor, Style::ButtonFill);
		int centerX = ((x2 + x) / 2) - (strlen(text) / 2);
		int centerY = (y2 + y) / 2;
		Text(centerX, centerY, text);
		return pressed;
	}

	static const char* Keys[] =
	{
		"none", "MOUSE 1", "MOUSE 2", "CANCEL", "MOUSE 3", "MOUSE 4", "MOUSE 5", "None", "BACKSPACE",
		"TAB", "None", "None", "CLEAR", "ENTER", "None",  "None",  "SHIFT",  "CTRL",  "ALT",  "PAUSE",
		 "CAPS LOCK",  "KANA",  "HANGUEL",  "HANGUL",  "JUNIA",  "FINAL",  "HANJA",
			"ESC",  "CONVERT",  "NONCONVERT",  "ACCEPT",  "MODE",  "SPACE",  "PAGE UP",  "PAGE DOWN",
		   "END", "HOME", "LEFT ARROW", "UP ARROW", "RIGHT ARROW", "DOWN ARROW", "SELECT", "PRINT", "EXECUTE",
		   "PRINT SCREEN", "INSERT", "DELETE", "HELP", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
		   "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "A", "B", "C", "D", "E", "F", "G", "H",
		   "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
		   "WIN", "WIN", "APPS", "NONE", "SLEEP", "NUMPAD 0", "NUMPAD 1", "NUMPAD 2", "NUMPAD 3", "NUMPAD 4",
		   "NUMPAD 5", "NUMPAD 6", "NUMPAD 7", "NUMPAD 8", "NUMPAD 9", "MULTIPLY", "ADD", "SEPARATOR", "SUBTRACT",
		   "DECIMIAL", "DIVIDE", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
		   "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24",
		   "NUM LOCK", "SCROLL LOCK", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
		   "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
		   "LSHIFT", "RSHIFT", "LCTRL", "RCTRL", "LALT", "RALT", "BACK", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", 
		   "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
		   "';:' key", "'+' key", "',' key", "'-' key","'.' key","'/?' key","'`' key",
	};

	static void KeySelector(const char* text, short x, short y, short x2, short y2, int* variable)
	{
		bool pressed = false;
		WORD NewColor = Style::Button;
		//Controls
		for (int winx = 0; winx < ConsoleWidth; winx++)
		{
			for (int winy = 0; winy < ConsoleHeight; winy++)
			{
				if (winx >= x && winx <= x2 && winy >= y && winy <= y2)
				{
					if (SelectedBlock == (winy * ConsoleWidth + winx))
					{
						//SetForegroundWindow(GetDesktopWindow());
						LastX = x;
						LastY = y;
						pressed = true;
						NewColor = Style::ButtonActive;
					}
					else if (FocusedBlock == (winy * ConsoleWidth + winx))
					{
						NewColor = Style::ButtonHovered;
					}
				}
			}
		}
		Box(x, y, x2, y2, NewColor, Style::ButtonFill);

		if (LastX == x && LastY == y)
		{
			for (int i = 0; i < sizeof(Keys); i++)
			{
				if (GetAsyncKeyState(i))
				{
					pressed = false;
					*variable = i;
					LastX = 9999;
					break;
				}
			}
		}

		if (*variable == 0)
		{
			int centerX = ((x2 + x) / 2) - (strlen(text) / 2);
			int centerY = (y2 + y) / 2;
			Text(centerX, centerY, text);
		}
		else
		{
			int centerX = ((x2 + x) / 2) - (strlen(Keys[*variable]) / 2);
			int centerY = (y2 + y) / 2;
			Text(centerX, centerY, Keys[*variable]);
		}
	}
	static void CheckBox(const char* text, short x, short y, bool* variable)
	{
		int SavedBoxStyle = Style::BoxStyle;
		Style::BoxStyle = Style::BoxStyle_Default;
		if (*variable)
		{
			if (Style::CheckBoxStyle == Style::CheckBoxStyle_ShortBox)
			{
				if (Button("X", x, y, x + 2, y + 1)) *variable = !(*variable);
				if (Link(text, x + 4, y)) *variable = !(*variable);
			}
			else if (Style::CheckBoxStyle == Style::CheckBoxStyle_FullBox)
			{
				if (Button("X", x, y, x + 2, y + 2)) *variable = !(*variable);
				if (Link(text, x + 4, y + 1)) *variable = !(*variable);
			}
			else if (Style::CheckBoxStyle == Style::CheckBoxStyle_BigBox)
			{
				if (Button("X", x, y, x + 4, y + 2)) *variable = !(*variable);
				if (Link(text, x + 6, y + 1)) *variable = !(*variable);
			}
			else if (Style::CheckBoxStyle == Style::CheckBoxStyle_Round)
			{
				if(Link((std::string("(o) ") + text).c_str(), x, y))*variable = !(*variable);
			}
			else if (Style::CheckBoxStyle == Style::CheckBoxStyle_Rect)
			{
				if (Link(L"■", x, y)) *variable = !(*variable);
				if (Link(text, x + 1, y)) *variable = !(*variable);
			}
		}
		else
		{
			if (Style::CheckBoxStyle == Style::CheckBoxStyle_ShortBox)
			{
				if (Button("", x, y, x + 2, y + 1)) *variable = !(*variable);
				if(Link(text,x + 4, y)) *variable = !(*variable);
			}
			else if (Style::CheckBoxStyle == Style::CheckBoxStyle_FullBox)
			{
				if (Button("", x, y, x + 2, y + 2)) *variable = !(*variable);
				if (Link(text, x + 4, y+1)) *variable = !(*variable);
			}
			else if (Style::CheckBoxStyle == Style::CheckBoxStyle_BigBox)
			{
				if (Button("", x, y, x + 4, y + 2)) *variable = !(*variable);
				if (Link(text, x + 6, y + 1)) *variable = !(*variable);
			}
			else if (Style::CheckBoxStyle == Style::CheckBoxStyle_Round)
			{
				if (Link((std::string("( ) ") + text).c_str(), x, y))*variable = !(*variable);
			}
			else if (Style::CheckBoxStyle == Style::CheckBoxStyle_Rect)
			{
				if (Link(L"□", x, y)) *variable = !(*variable);
				if (Link(text, x+1, y)) *variable = !(*variable);
			}
		}
		Style::BoxStyle = SavedBoxStyle;
	}
	static void SliderInt(short x, short y, short w, int* variable, int min=0, int max=100, int stepsize=2)
	{
		if (Link(" -", x - 1, y))
		{
			if (GetAsyncKeyState(VK_CONTROL))
			{
				*variable = *variable - stepsize*4;
				if (*variable < min) *variable = min;
			}
			else
			{
				*variable = *variable - stepsize;
				if (*variable < min) *variable = min;
			}
		}


		WORD Backup1 = Style::Link;
		WORD Backup2 = Style::LinkHovered;
		WORD Backup3 = Style::LinkActive;

		Style::Link = Style::SliderGrab;
		Style::LinkHovered = Style::SliderGrabHovered;
		Style::LinkActive = Style::SliderGrabActive;

		int coef = (max-min) / w;
		int blocksAmount = ((*variable - min) / coef);
		for (int i = 1; i < w; i++)
		{
			if (i <= blocksAmount)
			{
				if(Link(" ", x + i, y)) *variable = (i * coef) + min;
			}
			else
			{
				Style::Link = Style::Slider;
				Style::LinkHovered = Style::SliderHovered;
				Style::LinkActive = Style::SliderActive;
				if (Link(" ", x + i, y)) *variable = (i * coef) +min;
			}
		}

		Style::Link = Backup1;
		Style::LinkHovered = Backup2;
		Style::LinkActive = Backup3;

		std::string variableText = std::to_string(*variable);
		if (*variable < 10 && *variable >= 0) variableText = "00" + variableText;
		else if (*variable < 100 && *variable >= 0) variableText = "0" + variableText;

		Text(x + w, y, variableText.c_str());
		if (Link("+ ", x + w + variableText.size(), y))
		{
			if (GetAsyncKeyState(VK_CONTROL))
			{
				*variable = *variable + stepsize * 4;
				if (*variable > max) *variable = max;
			}
			else
			{
				*variable = *variable + stepsize;
				if (*variable > max) *variable = max;
			}
		}
	}
	static void SliderFloat(short x, short y, short w, float* variable, float min = 0, float max = 100, float stepsize = 2)
	{
		if (Link(" -", x - 1, y))
		{
			if (GetAsyncKeyState(VK_CONTROL))
			{
				*variable = *variable - stepsize * 4;
				if (*variable < min) *variable = min;
			}
			else
			{
				*variable = *variable - stepsize;
				if (*variable < min) *variable = min;
			}
		}

		WORD Backup1 = Style::Link;
		WORD Backup2 = Style::LinkHovered;
		WORD Backup3 = Style::LinkActive;

		Style::Link = Style::SliderGrab;
		Style::LinkHovered = Style::SliderGrabHovered;
		Style::LinkActive = Style::SliderGrabActive;

		int coef = (max - min) / w;
		int blocksAmount = ((*variable - min) / coef);
		for (int i = 1; i < w; i++)
		{
			if (i <= blocksAmount)
			{
				if (Link(" ", x + i, y)) *variable = (i * coef) + min;
			}
			else
			{
				Style::Link = Style::Slider;
				Style::LinkHovered = Style::SliderHovered;
				Style::LinkActive = Style::SliderActive;
				if (Link(" ", x + i, y)) *variable = (i * coef) + min;
			}
		}

		Style::Link = Backup1;
		Style::LinkHovered = Backup2;
		Style::LinkActive = Backup3;

		std::string variableText = std::to_string(*variable);
		variableText.erase(variableText.size() - 5, 5);

		Text(x + w, y, variableText.c_str());
		if (Link("+ ", x + w + variableText.size(), y))
		{
			if (GetAsyncKeyState(VK_CONTROL))
			{
				*variable = *variable + stepsize * 4;
				if (*variable > max) *variable = max;
			}
			else
			{
				*variable = *variable + stepsize;
				if (*variable > max) *variable = max;
			}
		}
	}
	static void InputText(const char* text, short x, short y, short w, std::string* variable, bool encrypted = false)
	{
		if (LastX == x && LastY == y && GetForegroundWindow() == WindowHWND)
		{
			if (variable->size() < w)
			{
				if (GetAsyncKeyState(VK_SHIFT))
				{
					for (int i = 65; i < 91; i++)
					{
						if (GetAsyncKeyState(i))
						{
							*variable = *variable + (char)i;
							while (GetAsyncKeyState(i)) Sleep(10);
						}
					}
					if (GetAsyncKeyState(VK_OEM_PLUS))
					{
						*variable = *variable + "=";
						while (GetAsyncKeyState(VK_OEM_PLUS)) Sleep(10);
					}
					if (GetAsyncKeyState(48))
					{
						*variable = *variable + ")";
						while (GetAsyncKeyState(48)) Sleep(10);
					}
					if (GetAsyncKeyState(49))
					{
						*variable = *variable + "!";
						while (GetAsyncKeyState(49)) Sleep(10);
					}
					if (GetAsyncKeyState(50))
					{
						*variable = *variable + "@";
						while (GetAsyncKeyState(50)) Sleep(10);
					}
					if (GetAsyncKeyState(51))
					{
						*variable = *variable + "#";
						while (GetAsyncKeyState(51)) Sleep(10);
					}
					if (GetAsyncKeyState(52))
					{
						*variable = *variable + "$";
						while (GetAsyncKeyState(52)) Sleep(10);
					}
					if (GetAsyncKeyState(53))
					{
						*variable = *variable + "%";
						while (GetAsyncKeyState(53)) Sleep(10);
					}
					if (GetAsyncKeyState(54))
					{
						*variable = *variable + "^";
						while (GetAsyncKeyState(54)) Sleep(10);
					}
					if (GetAsyncKeyState(55))
					{
						*variable = *variable + "&";
						while (GetAsyncKeyState(55)) Sleep(10);
					}
					if (GetAsyncKeyState(56))
					{
						*variable = *variable + "*";
						while (GetAsyncKeyState(56)) Sleep(10);
					}
					if (GetAsyncKeyState(57))
					{
						*variable = *variable + "(";
						while (GetAsyncKeyState(57)) Sleep(10);
					}
					if (GetAsyncKeyState(VK_OEM_PLUS))
					{
						*variable = *variable + "+";
						while (GetAsyncKeyState(VK_OEM_PLUS)) Sleep(10);
					}
					if (GetAsyncKeyState(VK_OEM_MINUS))
					{
						*variable = *variable + "_";
						while (GetAsyncKeyState(VK_OEM_MINUS)) Sleep(10);
					}
					if (GetAsyncKeyState(VK_OEM_COMMA))
					{
						*variable = *variable + "<";
						while (GetAsyncKeyState(VK_OEM_COMMA)) Sleep(10);
					}
					if (GetAsyncKeyState(VK_OEM_PERIOD))
					{
						*variable = *variable + ">";
						while (GetAsyncKeyState(VK_OEM_PERIOD)) Sleep(10);
					}
					if (GetAsyncKeyState(VK_OEM_2))
					{
						*variable = *variable + "?";
						while (GetAsyncKeyState(VK_OEM_2)) Sleep(10);
					}
				}
				else
				{
					for (int i = 48; i < 58; i++)
					{
						if (GetAsyncKeyState(i))
						{
							*variable = *variable + (char)tolower(i);
							while (GetAsyncKeyState(i)) Sleep(10);
						}
					}
					for (int i = 65; i < 91; i++)
					{
						if (GetAsyncKeyState(i))
						{
							*variable = *variable + (char)tolower(i);
							while (GetAsyncKeyState(i)) Sleep(10);
						}
					}
					if (GetAsyncKeyState(VK_OEM_PLUS))
					{
						*variable = *variable + "+";
						while (GetAsyncKeyState(VK_OEM_PLUS)) Sleep(10);
					}
					if (GetAsyncKeyState(VK_OEM_MINUS))
					{
						*variable = *variable + "-";
						while (GetAsyncKeyState(VK_OEM_MINUS)) Sleep(10);
					}
					if (GetAsyncKeyState(VK_OEM_COMMA))
					{
						*variable = *variable + ",";
						while (GetAsyncKeyState(VK_OEM_COMMA)) Sleep(10);
					}
					if (GetAsyncKeyState(VK_OEM_PERIOD))
					{
						*variable = *variable + ".";
						while (GetAsyncKeyState(VK_OEM_PERIOD)) Sleep(10);
					}
					if (GetAsyncKeyState(VK_OEM_2))
					{
						*variable = *variable + "/";
						while (GetAsyncKeyState(VK_OEM_2)) Sleep(10);
					}
				}
				if (GetAsyncKeyState(VK_SPACE))
				{
					*variable = *variable + " ";
					while (GetAsyncKeyState(VK_SPACE)) Sleep(10);
				}
			}
			if (GetAsyncKeyState(VK_BACK))
			{
				Sleep(85);
				*variable = variable->substr(0, variable->size() - 1);
			}
			Text(x, y, text, Style::InputText);
			if (encrypted)
			{
				if (dwTick + 500 < GetTickCount())dwTick = GetTickCount();
				std::string pass = "";
				pass.resize(variable->size(), '*');
				if (dwTick + 250 < GetTickCount()) Text(x, y + 1, (pass + "_").c_str(), Style::InputTextText);
				else Text(x, y + 1, (pass).c_str(), Style::InputTextText);
			}
			else
			{
				if (dwTick + 500 < GetTickCount())dwTick = GetTickCount();
				if (dwTick + 250 < GetTickCount()) Text(x, y + 1, (*variable + "_").c_str(), Style::InputTextText);
				else Text(x, y + 1, (*variable).c_str(), Style::InputTextText);
			}
		}
		else
		{
			Text(x, y, text, Style::InputText);
			if (encrypted)
			{
				std::string pass = "";
				pass.resize(variable->size(), '*');
				Text(x, y + 1, (pass).c_str(), Style::InputTextText);
			}
			else Text(x, y + 1, (*variable).c_str(), Style::InputTextText);
		}

		if (InvisibleButton(x, y, x + w, y + 2))
		{
			LastX = x;
			LastY = y;
		}
	}


#pragma endregion

}