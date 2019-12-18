#include "ConGui.h"

bool mybool = false;
int myint = 40;
float myfloat = 100.f;
std::string mystring = "INPUTTEXT";
int main()
{
	ConGui::ApplyConsoleStyle();
	ConGui::PrepareFrame();
	ConGui::Init();
	ConGui::SetCursorState(false);
	
	while (true)
	{
		Sleep(30);
		if (!IsWindowVisible(ConGui::WindowHWND)) continue;

		ConGui::Frame();
		ConGui::InputHandle();

		ConGui::Text(0, 0, ("X=" + std::to_string(ConGui::CursorX) + " Y=" + std::to_string(ConGui::CursorY) + " S=" + std::to_string(ConGui::SelectedBlock) + " X=" + std::to_string(ConGui::LastX)).c_str(), 10);
		ConGui::Style::BoxStyle = ConGui::Style::BoxStyle_Default;
		if (ConGui::Button("BUTTON 1", 2, 2, 20, 4)) Beep(100, 100);
		ConGui::Style::BoxStyle = ConGui::Style::BoxStyle_Single;
		if (ConGui::Button("BUTTON 2", 2, 5, 20, 7)) Beep(200, 200);
		ConGui::Style::BoxStyle = ConGui::Style::BoxStyle_Corners;
		if (ConGui::Button("BUTTON 3", 2, 8, 20, 10)) Beep(300, 300);
		ConGui::Style::BoxStyle = ConGui::Style::BoxStyle_Corners2;
		if (ConGui::Button("BUTTON 4", 22, 2, 44, 4)) Beep(400, 400);
		ConGui::Style::BoxStyle = ConGui::Style::BoxStyle_Combo;
		if (ConGui::Button("BUTTON 5", 22, 5, 44, 7)) Beep(500, 500);
		ConGui::Style::BoxStyle = ConGui::Style::BoxStyle_Combo2;
		if (ConGui::Button("BUTTON 6", 22, 8, 44, 10)) Beep(500, 600);

		ConGui::Style::CheckBoxStyle = ConGui::Style::CheckBoxStyle_Round;
		ConGui::CheckBox("Checkbox1", 2, 12, &mybool); 
		ConGui::Style::CheckBoxStyle = 0;
		ConGui::CheckBox("Checkbox2", 16, 12, &mybool);
		ConGui::Style::CheckBoxStyle = ConGui::Style::CheckBoxStyle_BigBox;
		ConGui::CheckBox("Checkbox3", 30, 11, &mybool);
		ConGui::Style::CheckBoxStyle = ConGui::Style::CheckBoxStyle_Rect;
		ConGui::CheckBox("Checkbox4", 46, 12, &mybool);

		ConGui::SliderInt(2, 14,20, &myint);
		ConGui::SliderFloat(2, 16, 20, &myfloat,0,200,5.5f);
		ConGui::InputText("INPUT TEXT:", 46, 2, 17, &mystring);

		ConGui::Render();
	}
}
