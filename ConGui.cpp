#include "ConGui.h"

std::string login;
std::string pass;

bool auth = false;

bool mybool = false;
int myint = 40;
float myfloat = 100.f;
int mykey = 0;
std::string mystring = "INPUTTEXT";

int main()
{
	SetConsoleTitleA("ConGui Demo");
	ConGui::ApplyConsoleStyle(20, L"Consolas"); //L"Terminal" for Raster fonts
	ConGui::SetWindow(33, 9, false, true);
	ConGui::Init();
	ConGui::SetCursorState(false);
	ConGui::LastX = 2; ConGui::LastY = 1; //AutoSelect first InputText
	while (true)
	{
		ConGui::Style::ThemeBlue(); // Select theme
		Sleep(1); //Framerate
		if (!IsWindowVisible(ConGui::WindowHWND)) continue;
		ConGui::Frame();
		ConGui::InputHandle();
		if (!auth)
		{
			ConGui::Style::BoxStyle = ConGui::Style::BoxStyle_Combo2;
			ConGui::Box(0, 0, ConGui::ConsoleWidth - 1, ConGui::ConsoleHeight - 1);
			ConGui::Style::BoxStyle = ConGui::Style::BoxStyle_Combo;
			ConGui::InputText("USERNAME:", 2, 1, 18, 144, &login, true, false);
			ConGui::InputText("PASSWORD:", 2, 3, 18, 144, &pass, true, false, false, true);
			if (ConGui::Button("LOGIN", 2, 5, 28, 7, true))
			{
				Beep(100, 100);
				ConGui::SetWindow(100, 25, false, true);
				ConGui::SetCursorState(false);
				ConGui::Fill(ConGui::Style::MainColor);
				auth = true;
				
			}
		}
		else
		{
			ConGui::Text(0, 0, ("X=" + std::to_string(ConGui::CursorX) + " Y=" + std::to_string(ConGui::CursorY)).c_str(), 10);
			
			ConGui::KeySelector("INPUT KEY", 40, 18, 56, 20, &mykey);
			
			for (int i = ConGui::Style::BoxStyle_Default; i <= ConGui::Style::BoxStyle_Empty; i++)
			{
				if (i == ConGui::Style::BoxStyle_Empty)
				{
					ConGui::Style::Button = 32;
					ConGui::Style::ButtonHovered = 39;
					ConGui::Style::ButtonActive = 46;
				}
				ConGui::Style::BoxStyle = i;
				if (i > 6)
				{
					if (ConGui::Button(("BUTTON " + std::to_string(i)).c_str(), 20, 2 + 3 * i - 21, 20 +16, 4 + 3 * i - 21)) Beep(400, 45);
				}
				else
				{
					if (ConGui::Button(("BUTTON " + std::to_string(i)).c_str(), 2, 2 + 3 * i, 18, 4 + 3 * i)) Beep(400, 45);
				}
				if (i == ConGui::Style::BoxStyle_Empty)
				{
					ConGui::Style::Button = ConGui::Style::FG_LIGHTGRAY;
					ConGui::Style::ButtonHovered = ConGui::Style::FG_GREEN;
					ConGui::Style::ButtonActive = ConGui::Style::FG_LIGHTGREEN;
				}
			}
			ConGui::Style::BoxStyle = ConGui::Style::BoxStyle_Empty;
			ConGui::Style::Button = 39;
			ConGui::Style::ButtonHovered = 47;
			ConGui::Style::ButtonActive = 46;
			if(ConGui::Button("BUTTON 13", 24, 21, 32, 21)) Beep(400, 45);
			ConGui::Style::Button = ConGui::Style::FG_LIGHTGRAY;
			ConGui::Style::ButtonHovered = ConGui::Style::FG_GREEN;
			ConGui::Style::ButtonActive = ConGui::Style::FG_LIGHTGREEN;

			for (int i = ConGui::Style::CheckBoxStyle_RectX; i <= ConGui::Style::CheckBoxStyle_Rect; i++)
			{
				ConGui::Style::CheckBoxStyle = i;
				ConGui::CheckBox(("Checkbox " + std::to_string(i)).c_str(), 40, 3 + 3 * i, &mybool);
			}

			ConGui::SliderInt(60, 3, 20, &myint);
			ConGui::SliderFloat(60, 5, 20, &myfloat, 0, 200, 5.5f);

			ConGui::Style::BoxStyle = ConGui::Style::BoxStyle_Combo2;
			ConGui::Box(60, 7, 90, 9, 0, false);
			ConGui::InputText("INPUT TEXT:", 61, 8, 18, 40, &mystring, true);

			ConGui::Style::BoxStyle = ConGui::Style::BoxStyle_Block1;
			ConGui::Box(60, 11, 90, 13, 0, false);
			ConGui::InputText("INPUT TEXT:", 61, 12, 18, 40, &mystring, true);

			ConGui::InputText("INPUT TEXT:", 61, 16, 18, 40, &mystring, false, true);

			ConGui::InputText("INPUT TEXT:", 61, 20, 18, 40, &mystring, false, false);

			ConGui::Style::BoxStyle = ConGui::Style::BoxStyle_Combo;
			
		}

		ConGui::Render();
	}
}
