#pragma once
#include <Windows.h>
#include <string>
#include <vector>

namespace ConGui
{
#pragma region Globals
	static HANDLE hConsole = NULL;
	static HANDLE hInput;
	static INPUT_RECORD InputRecord;
	static HWND WindowHWND = NULL;
	static RECT WindowRect;

	static int WindowWidth = 250;
	static int WindowHeight = 250;
	static int LastWindowWidth = 250;
	static int LastWindowHeight = 250;
	static int ConsoleWidth = 18;
	static int ConsoleHeight = 18;

	static int CursorX = 0;
	static int CursorY = 0;
	static int SelectedBlock = 0;
	static int FocusedBlock = 0;

	static int LastX = 0;
	static int LastY = 0;

	static bool DontBlock = false;//Dont loop GetAsyncKey(VK_LBUTTON) when dragging sliders or smth like that
	static DWORD dwTick = 0;
	static DWORD dwBytesWritten = 0;
	static wchar_t screen [1024 * 1024];
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
			BoxStyle_RectParentheses,
			BoxStyle_RoundParentheses,
			BoxStyle_Block1,
			BoxStyle_Block2,
			BoxStyle_Block3,
			BoxStyle_Block4,
			BoxStyle_Empty,
		};
		static int BoxStyle = BoxStyle_Combo;

		static enum
		{
			CheckBoxStyle_RectX,
			CheckBoxStyle_RectRect,
			CheckBoxStyle_RoundO,
			CheckBoxStyle_RoundRect,
			CheckBoxStyle_Rect,
		};
		static int CheckBoxStyle = CheckBoxStyle_Rect;

		typedef enum FG_COLORS
		{
			FG_BLACK = 0,
			FG_BLUE = 1,
			FG_GREEN = 2,
			FG_CYAN = 3,
			FG_RED = 4,
			FG_MAGENTA = 5,
			FG_BROWN = 6,
			FG_LIGHTGRAY = 7,
			FG_GRAY = 8,
			FG_LIGHTBLUE = 9,
			FG_LIGHTGREEN = 10,
			FG_LIGHTCYAN = 11,
			FG_LIGHTRED = 12,
			FG_LIGHTMAGENTA = 13,
			FG_YELLOW = 14,
			FG_WHITE = 15
		}FG_COLORS;
		typedef enum BG_COLORS
		{
			BG_NAVYBLUE = 16,
			BG_GREEN = 32,
			BG_TEAL = 48,
			BG_MAROON = 64,
			BG_PURPLE = 80,
			BG_OLIVE = 96,
			BG_SILVER = 112,
			BG_GRAY = 128,
			BG_BLUE = 144,
			BG_LIME = 160,
			BG_CYAN = 176,
			BG_RED = 192,
			BG_MAGENTA = 208,
			BG_YELLOW = 224,
			BG_WHITE = 240
		}BG_COLORS;

		static WORD MainColor = FG_LIGHTGRAY;
		static WORD Button = FG_LIGHTGRAY;
		static WORD ButtonHovered = FG_GREEN;
		static WORD ButtonActive = FG_LIGHTGREEN;
		static bool ButtonFill = true;

		static WORD Link = FG_LIGHTGRAY;
		static WORD LinkHovered = FG_GREEN;
		static WORD LinkActive = FG_LIGHTGREEN;

		static WORD SliderGrab = BG_BLUE + FG_LIGHTBLUE;
		static WORD SliderGrabHovered = BG_TEAL + FG_MAGENTA;
		static WORD SliderGrabActive = BG_TEAL + FG_MAGENTA;

		static WORD Slider = BG_MAROON + FG_RED;
		static WORD SliderHovered = BG_RED + FG_LIGHTRED;
		static WORD SliderActive = BG_RED + FG_LIGHTRED;

		static WORD InputText = BG_WHITE + FG_BLACK;
		static WORD InputTextText = BG_GRAY + FG_LIGHTGRAY;

		void ThemeBlue()
		{
			MainColor = BG_NAVYBLUE + FG_LIGHTGRAY;
			Button = BG_NAVYBLUE + FG_LIGHTGRAY;
			ButtonHovered = BG_NAVYBLUE + FG_GRAY;
			ButtonActive = BG_NAVYBLUE + FG_YELLOW;

			Link = Button;
			LinkHovered = ButtonHovered;
			LinkActive = ButtonActive;
		}

		void ThemeBlueGray()
		{
			MainColor = BG_NAVYBLUE + FG_LIGHTGRAY;
			Button = BG_GRAY + FG_LIGHTGRAY;
			ButtonHovered = BG_SILVER + FG_GRAY;
			ButtonActive = BG_SILVER + FG_YELLOW;

			Link = MainColor;
			LinkHovered = BG_NAVYBLUE + FG_GRAY;;
			LinkActive = BG_NAVYBLUE + FG_YELLOW;
		}
		void ThemeGray()
		{
			MainColor = BG_GRAY + FG_LIGHTGRAY;
			Button = BG_GRAY + FG_LIGHTGRAY;
			ButtonHovered = BG_GRAY + FG_WHITE;
			ButtonActive = BG_GRAY + FG_YELLOW;
			InputTextText = BG_SILVER + FG_LIGHTGRAY;

			Link = Button;
			LinkHovered = ButtonHovered;
			LinkActive = ButtonActive;
		}
		void ThemeSky()
		{
			MainColor = BG_BLUE + FG_WHITE;
			Button = BG_BLUE + FG_WHITE;
			ButtonHovered = BG_BLUE + FG_WHITE;
			ButtonActive = BG_BLUE + FG_YELLOW;
			InputTextText = BG_CYAN + FG_WHITE;
			SliderGrab = BG_NAVYBLUE;

			Link = Button;
			LinkHovered = ButtonHovered;
			LinkActive = ButtonActive;
		}
	}



#pragma endregion

	void SetColor(short x, short y, int len, WORD color)
	{
		DWORD written;
		std::vector<WORD> attributes(len, color);
		WriteConsoleOutputAttribute(hConsole, &attributes[0], len, { x,y }, &written);
	}

	void Fill(WORD color)
	{
		DWORD written;
		int size = sizeof(screen);//ConGui::ConsoleHeight * ConGui::ConsoleWidth * 2;
		std::vector<WORD> attributes(size, color);
		WriteConsoleOutputAttribute(ConGui::hConsole, &attributes[0], size, { 0,0 }, &written);
	}
#pragma region Initialization
	static void ApplyConsoleStyle(short font_size, const WCHAR font_name[LF_FACESIZE])
	{
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;                   // Width of each character in the font
		cfi.dwFontSize.Y = font_size;                  // Height
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_BOLD;
		wcscpy_s(cfi.FaceName, LF_FACESIZE, font_name);
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	}
	static void SetWindow(int w, int h, bool disable_resize = true, bool always_on_top = false)
	{
		WindowHWND = GetConsoleWindow();
		COORD font_size = GetConsoleFontSize(GetStdHandle(STD_OUTPUT_HANDLE), 0);
		DWORD dwStyle = (DWORD)GetWindowLong(WindowHWND, GWL_STYLE);
		RECT rect = { 0, 0, w * font_size.X, h * font_size.Y };
		AdjustWindowRect(&rect, dwStyle, false);
		WindowWidth = rect.right - rect.left;//w * font_size.X;
		WindowHeight = rect.bottom - rect.top;// h* font_size.Y + 21;
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		MoveWindow(WindowHWND, (desktop.right / 2) - (WindowWidth / 2), (desktop.bottom / 2) - (WindowHeight / 2), WindowWidth, WindowHeight, TRUE); //Move to center and ReSize
		if(always_on_top) SetWindowPos(WindowHWND, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		if(disable_resize) SetWindowLong(WindowHWND, GWL_STYLE, GetWindowLong(WindowHWND, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	}
	static void Init()
	{
		hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleActiveScreenBuffer(hConsole);
		DWORD prev_mode;
		hInput = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(hInput, &prev_mode);
		prev_mode |= ENABLE_QUICK_EDIT_MODE;
		prev_mode &= ENABLE_EXTENDED_FLAGS;
		prev_mode |= ENABLE_MOUSE_INPUT;
		SetConsoleMode(hInput, prev_mode);
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
		if (LastWindowWidth != WindowWidth || LastWindowHeight != WindowHeight)
		{
			LastWindowWidth = WindowWidth;
			LastWindowHeight = WindowHeight;
			Fill(Style::MainColor);
		}

		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
		GetConsoleScreenBufferInfo(hConsole, &csbiInfo);

		ConsoleWidth = csbiInfo.dwMaximumWindowSize.X;
		ConsoleHeight = csbiInfo.dwMaximumWindowSize.Y;

		//screen = new wchar_t[ConsoleWidth * ConsoleHeight];
		for (int i = 0; i < ConsoleWidth * ConsoleHeight; i++) screen[i] = ' ';
	}
	
	static void InputHandle()
	{
		DWORD numberOfInputs;
		GetNumberOfConsoleInputEvents(hInput, &numberOfInputs);
		if (numberOfInputs)
		{
			DWORD Events;
			ReadConsoleInput(hInput, &InputRecord, 1, &Events);
			if (InputRecord.EventType == MOUSE_EVENT)
			{
				CursorX = InputRecord.Event.MouseEvent.dwMousePosition.X;
				CursorY = InputRecord.Event.MouseEvent.dwMousePosition.Y;
				FocusedBlock = CursorY * ConsoleWidth + CursorX;
				SelectedBlock = 0;
				if (CursorX < 0 || CursorY < 0) return;
				if (DontBlock && GetAsyncKeyState(VK_LBUTTON))
				{
					SelectedBlock = CursorY * ConsoleWidth + CursorX;
					FocusedBlock = 0;
					return;
				}
				while (GetAsyncKeyState(VK_LBUTTON)/* == -32767 */&& GetForegroundWindow() == WindowHWND)
				{
					SelectedBlock = CursorY * ConsoleWidth + CursorX;
					FocusedBlock = 0;
				}
				DontBlock = false;
			}
		}
	}
	static void Render()
	{
		int screenlen = wcslen(screen);
		screen[ConsoleWidth * ConsoleHeight] = '\0';
		WriteConsoleOutputCharacterW(hConsole, screen, screenlen, { 0,0 }, &dwBytesWritten);
	}

#pragma endregion

#pragma region UIElements

	static void Text(short x, short y, const char* text, WORD color = 0)
	{
		for (int i = 0; i < strlen(text); i++)
		{
			if (color > 0)
			{
				SetColor(x, y, strlen(text), color);
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
				SetColor(x, y, wcslen(text), color);
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
		else if (Style::BoxStyle == Style::BoxStyle_Block1)
		{
			LeftUpCorner = L'░';
			LeftDownCorner = L'░';
			RightUpCorner = L'░';
			RightDownCorner = L'░';
			Vertical = L'░';
			Horizontal = L'░';
		}
		else if (Style::BoxStyle == Style::BoxStyle_Block2)
		{
			LeftUpCorner = L'▒';
			LeftDownCorner = L'▒';
			RightUpCorner = L'▒';
			RightDownCorner = L'▒';
			Vertical = L'▒';
			Horizontal = L'▒';
		}
		else if (Style::BoxStyle == Style::BoxStyle_Block3)
		{
			LeftUpCorner = L'▓';
			LeftDownCorner = L'▓';
			RightUpCorner = L'▓';
			RightDownCorner = L'▓';
			Vertical = L'▓';
			Horizontal = L'▓';
		}
		else if (Style::BoxStyle == Style::BoxStyle_Block4)
		{
			LeftUpCorner = L'█';
			LeftDownCorner = L'█';
			RightUpCorner = L'█';
			RightDownCorner = L'█';
			Vertical = L'█';
			Horizontal = L'█';
		}
		else if (Style::BoxStyle == Style::BoxStyle_RectParentheses)
		{
			LeftUpCorner = L' ';
			LeftDownCorner = L' ';
			RightUpCorner = L' ';
			RightDownCorner = L' ';
			Vertical = L'[';
			Horizontal = L' ';
		}
		else if (Style::BoxStyle == Style::BoxStyle_RoundParentheses)
		{
			LeftUpCorner = L' ';
			LeftDownCorner = L' ';
			RightUpCorner = L' ';
			RightDownCorner = L' ';
			Vertical = L'(';
			Horizontal = L' ';
		}
		else if (Style::BoxStyle == Style::BoxStyle_Empty)
		{
			LeftUpCorner = L' ';
			LeftDownCorner = L' ';
			RightUpCorner = L' ';
			RightDownCorner = L' ';
			Vertical = L' ';
			Horizontal = L' ';
		}

		{
			//Horiozntal
			for (int i = 1; i < w; i++)
			{
				if (color > 0) SetColor(x + i, y, 1, color);
				screen[y * ConsoleWidth + x + i] = Horizontal;
			}
			for (int i = 1; i < w; i++)
			{
				if (color > 0) SetColor(x + i, y + h, 1, color);
				screen[(y + h) * ConsoleWidth + x + i] = Horizontal;
			}

			//Vertical
			for (int i = 1; i < h; i++)
			{
				if (color > 0) SetColor(x, y + i, 1, color);
				screen[(y + i) * ConsoleWidth + x] = Vertical;
			}
			if (Style::BoxStyle == Style::BoxStyle_RectParentheses) Vertical = ']';
			if (Style::BoxStyle == Style::BoxStyle_RoundParentheses) Vertical = ')';
			for (int i = 1; i < h; i++)
			{
				if (color > 0) SetColor(x + w, y + i, 1, color);
				screen[(y + i) * ConsoleWidth + x + w] = Vertical;
			}
		
			//Corners
			screen[y * ConsoleWidth + x] = LeftUpCorner;
			screen[y * ConsoleWidth + x + w] = RightUpCorner;
			screen[(y + h) * ConsoleWidth + x] = LeftDownCorner;
			screen[(y + h) * ConsoleWidth + x + w] = RightDownCorner;
		}

		if (color > 0)
		{
			SetColor(x, y, 1, color);
			SetColor(x + w, y, 1, color);
			SetColor(x, y + h, 1, color);
			SetColor(x + w, y + h, 1, color);
			if (fill)
			{
				for (int i = 1; i < h; i++) SetColor(x, y + i, w, color);
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
	static bool Button(const char* text, short x, short y, short x2, short y2, bool return_true_on_enter = false)
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
		if (return_true_on_enter && GetAsyncKeyState(VK_RETURN) && GetForegroundWindow() == WindowHWND) return true;
		return pressed;
	}


	static const int KeysLength = 170;
	static const char* Keys[] =
	{
		"NONE", "MOUSE1", "MOUSE2", "CANCEL", "MOUSE 3", "MOUSE 4", "MOUSE 5", "None", "BACKSPACE",
		"TAB", "None", "None", "CLEAR", "ENTER", "None",  "None",  "SHIFT",  "CTRL",  "ALT",  "PAUSE",
		"CAPS LOCK",  "KANA",  "HANGUEL",  "HANGUL",  "JUNIA",  "FINAL",  "HANJA",
		"ESC",  "CONVERT",  "NONCONVERT",  "ACCEPT",  "MODE",  "SPACE",  "PAGE UP",  "PAGE DOWN",
		"END", "HOME", "LEFT ARROW", "UP ARROW", "RIGHT ARROW", "DOWN ARROW", "SELECT", "PRINT", "EXECUTE",
		"PRINT SCREEN", "INSERT", "DELETE", "HELP", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
		"NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "a", "b", "c", "d", "e", "f", "g", "h",
		"i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
		"WIN", "WIN", "APPS", "NONE", "SLEEP", "KP_INS", "KP_END", "KP_DOWNARROW", "KP_PGDN", "KP_LEFTARROW",
		"KP_5", "KP_RIGHTARROW", "KP_HOME", "KP_UPARROW", "KP_PGUP", "kp_multiply", "kp_plus", "kp_slash", "kp_minus",
		"kp_del", "kp_slash", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
		"F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24",
		"NUM LOCK", "SCROLL LOCK", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
		"NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE",
		"LSHIFT", "RSHIFT", "LCTRL", "RCTRL", "LALT", "RALT", "BACK", "NONE", "MWHEELUP", "MWHEELDOWN"
	};
	
	static void KeySelector(const char* text, short x, short y, short x2, short y2, int* variable)
	{
		if (*variable > KeysLength) *variable = 0;
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
						LastX = x;
						LastY = y;
						*variable = 0;
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
			for (int i = 0; i < KeysLength; i++)
			{
				if (GetAsyncKeyState(i) == -32767)
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
			if (LastX == x && LastY == y)
			{
				int centerX = ((x2 + x) / 2) - (3 / 2);
				int centerY = (y2 + y) / 2;
				Text(centerX, centerY, "...");
			}
			else
			{
				int centerX = ((x2 + x) / 2) - (strlen(text) / 2);
				int centerY = (y2 + y) / 2;
				Text(centerX, centerY, text);
			}
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
			if (Style::CheckBoxStyle == Style::CheckBoxStyle_RectX)
			{
				if (Link((std::string("[x] ") + text).c_str(), x, y))*variable = !(*variable);
			}
			else if (Style::CheckBoxStyle == Style::CheckBoxStyle_RectRect)
			{
				if (Link(L"[■]", x, y)) *variable = !(*variable);
				if (Link(text, x + 4, y)) *variable = !(*variable);
			}
			else if (Style::CheckBoxStyle == Style::CheckBoxStyle_RoundO)
			{
				if (Link((std::string("(o) ") + text).c_str(), x, y))*variable = !(*variable);
			}
			else if (Style::CheckBoxStyle == Style::CheckBoxStyle_RoundRect)
			{
				if (Link(L"(■)", x, y)) *variable = !(*variable);
				if (Link(text, x + 4, y)) *variable = !(*variable);
			}
			else if (Style::CheckBoxStyle == Style::CheckBoxStyle_Rect)
			{
				if (Link(L"■", x, y)) *variable = !(*variable);
				if (Link(text, x + 2, y)) *variable = !(*variable);
			}
		}
		else
		{
			if (Style::CheckBoxStyle == Style::CheckBoxStyle_RectX)
			{
				if (Link((std::string("[ ] ") + text).c_str(), x, y))*variable = !(*variable);
			}
			else if (Style::CheckBoxStyle == Style::CheckBoxStyle_RectRect)
			{
				if (Link(L"[ ]", x, y)) *variable = !(*variable);
				if (Link(text, x + 4, y)) *variable = !(*variable);
			}
			else if (Style::CheckBoxStyle == Style::CheckBoxStyle_RoundO)
			{
				if (Link((std::string("( ) ") + text).c_str(), x, y))*variable = !(*variable);
			}
			else if (Style::CheckBoxStyle == Style::CheckBoxStyle_RoundRect)
			{
				if (Link(L"( )", x, y)) *variable = !(*variable);
				if (Link(text, x + 4, y)) *variable = !(*variable);
			}
			else if (Style::CheckBoxStyle == Style::CheckBoxStyle_Rect)
			{
				if (Link(L"□", x, y)) *variable = !(*variable);
				if (Link(text, x + 2, y)) *variable = !(*variable);
			}
		}
		Style::BoxStyle = SavedBoxStyle;
	}
	
	static void SliderFloat(short x, short y, short w, float* variable, float min = 0, float max = 100, float stepsize = 2)
	{
		if (CursorX > x + 1 && CursorY == y && CursorX <= x + w) DontBlock = true;
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
	static void SliderInt(short x, short y, short w, int* variable, int min = 0, int max = 100, int stepsize = 2)
	{
		float val = (float)*variable;
		SliderFloat(x, y, w, &val, min, max, stepsize);
		*variable = (int)val;
	}
	struct INPUT_TEXT_INFO
	{
		const char* title = "";
		short x = -1;
		short y = -1;
	};
	INPUT_TEXT_INFO InputTexts[10];
	static bool InputText(const char* title, short x, short y, short w, short len, std::string* variable, bool same_line = false, bool center_title = true, bool return_true_on_enter = false, bool encrypted = false)
	{
		#define GetKey(keycode, str, str2) if (GetAsyncKeyState(keycode) == -32767) { if (GetAsyncKeyState(VK_SHIFT)) *variable = *variable + str2; else *variable = *variable + str; }
		int found = -1;
		for (int i = 0; i < 10; i++)
		{
			if(InputTexts[i].x == x && InputTexts[i].y == y)
			{
				found = i;
				break;
			}
		}
		if (found == -1)
		{
			for (int i = 0; i < 10; i++)
			{
				if (InputTexts[i].x == -1 && InputTexts[i].y == -1)
				{
					InputTexts[i].title = title;
					InputTexts[i].x = x;
					InputTexts[i].y = y;
					found = i;
					break;
				}
			}
		}

		short title_x = x;
		int var_pos_x = x;
		int var_pos_y = y + 1;
		if (same_line)
		{
			var_pos_x = x + strlen(title);
			var_pos_y = y;
		}
		else if (center_title) title_x = x + w / 2 - strlen(title) / 2;

		if (LastX == x && LastY == y && GetForegroundWindow() == WindowHWND)
		{
			if (GetAsyncKeyState(VK_TAB) || (!return_true_on_enter && GetAsyncKeyState(VK_RETURN)))
			{
				if (found > -1)
				{
					for (int i = 1; i < 10; i++)
					{
						int index = found + i;
						if (index >= 10) index = index - 10;
						if (InputTexts[index].x != -1 || InputTexts[index].y != -1)
						{
							LastX = InputTexts[index].x;
							LastY = InputTexts[index].y;
							break;
						}
					}
				}
				while (GetAsyncKeyState(VK_TAB)) Sleep(1);
				return false;
			}
			if (return_true_on_enter && GetAsyncKeyState(VK_RETURN))
			{
				while (GetAsyncKeyState(VK_RETURN)) Sleep(1);
				return true;
			}
			if (GetAsyncKeyState(VK_CONTROL))
			{
				if (GetAsyncKeyState(0x56) == -32767) // V KEY
				{
					std::string clipboard = "";
					if (OpenClipboard(NULL))
					{
						HANDLE h = GetClipboardData(CF_TEXT);
						if (h && h != INVALID_HANDLE_VALUE) clipboard = (char*)h;
						CloseClipboard();
					}
					for(int i = 0; i < clipboard.size(); i++) *variable = *variable + clipboard[i];
					if (variable->size() > w) variable->resize(w);
				}
				if (GetAsyncKeyState(0x43) == -32767 || GetAsyncKeyState(0x58) == -32767) // C OR X key
				{
					OpenClipboard(NULL);
					EmptyClipboard();
					HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE,  (*variable).size() + 1);
					if (!hg)
					{
						CloseClipboard();
						return false;
					}
					memcpy(GlobalLock(hg), (*variable).c_str(), (*variable).size() + 1);
					GlobalUnlock(hg);
					SetClipboardData(CF_TEXT, hg);
					CloseClipboard();
					GlobalFree(hg);

					if(GetAsyncKeyState(0x58) == -32767) (*variable) = "";
				}
			}
			if (variable->size() < len/*w - 1*/) // -1 for cursor
			{
				GetKey(0x30, "0", ")");
				GetKey(0x31, "1", "!");
				GetKey(0x32, "2", "@");
				GetKey(0x33, "3", "#");
				GetKey(0x34, "4", "$");
				GetKey(0x35, "5", "%");
				GetKey(0x36, "6", "^");
				GetKey(0x37, "7", "&");
				GetKey(0x38, "8", "*");
				GetKey(0x39, "9", "(");
				for (int i = 0x3A; i <= 0x40; i++) GetKey(i, "-", "_");
				for (int i = 0x41; i <= 0x5A; i++) GetKey(i, (char)tolower(i), (char)(i)); // a-z
				for (int i = VK_NUMPAD0; i <= VK_NUMPAD9; i++) GetKey(i, (char)(i - 0x30), (char)(i - 0x30));// numpad 0-9
				GetKey(VK_OEM_1, ";", ":");
				GetKey(VK_OEM_2, "/", "?");
				GetKey(VK_OEM_3, "`", "~");
				GetKey(VK_OEM_4, "[", "{");
				GetKey(VK_OEM_5, "\\", "|");
				GetKey(VK_OEM_6, "{", "}");
				GetKey(VK_OEM_7, "'", "\"");
				GetKey(VK_OEM_MINUS, "-", "_");
				GetKey(VK_OEM_PLUS, "=", "+");
				GetKey(VK_OEM_PERIOD, ".", ">");
				GetKey(VK_OEM_COMMA, ",", "<");
				GetKey(VK_SPACE, " ", " ");
			}
			if (variable->size() > 0 && GetAsyncKeyState(VK_BACK) == -32767 || GetAsyncKeyState(VK_DELETE) == -32767) variable->pop_back();

			
			Text(title_x, y, title, Style::InputText);
			std::string tmp_str = *variable;
			if (encrypted)
			{
				tmp_str = ""; tmp_str.resize(variable->size(), '*');
			}
			if (dwTick + 500 < GetTickCount()) dwTick = GetTickCount();
			if (dwTick + 250 > GetTickCount()) tmp_str = tmp_str + "_";
			else tmp_str = tmp_str + " ";
			if (tmp_str.size() > w) tmp_str = tmp_str.substr(tmp_str.size() - w, tmp_str.size() - (tmp_str.size() - w));
			Text(var_pos_x, var_pos_y, tmp_str.c_str(), Style::InputTextText);
		}
		else
		{
			Text(title_x, y, title, Style::InputText);
			if (variable->size() == 0)
			{
				std::string placeholder = "";
				placeholder.resize(variable->size(), 'x');
				Text(var_pos_x + strlen(title), var_pos_y, placeholder.c_str(), Style::InputTextText);
			}
			else
			{
				if (encrypted)
				{
					std::string pass = "";
					pass.resize(variable->size(), '*');
					Text(var_pos_x, var_pos_y, (pass).c_str(), Style::InputTextText);
				}
				else Text(var_pos_x, var_pos_y, (*variable).c_str(), Style::InputTextText);
			}
		}
		SetColor(var_pos_x, var_pos_y, w, Style::InputTextText);
		if(!same_line) SetColor(x, y, w, Style::InputText);

		if (InvisibleButton(x, y, var_pos_x + w , y + 2))
		{
			LastX = x;
			LastY = y;
		}

		return false;
	}


#pragma endregion

}