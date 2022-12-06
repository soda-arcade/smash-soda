#pragma once

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"

#define APPFONTS_EXTRA_LIGHT	"Fonts/Montserrat-ExtraLight.ttf"
#define APPFONTS_LIGHT			"Fonts/Montserrat-Light.ttf"
#define APPFONTS_REGULAR		"Fonts/Montserrat-Regular.ttf"
#define APPFONTS_MEDIUM			"Fonts/Montserrat-Medium.ttf"
#define APPFONTS_SEMI_BOLD		"Fonts/Montserrat-SemiBold.ttf"
#define APPFONTS_BOLD			"Fonts/Montserrat-Bold.ttf"
#define APPFONTS_EXTRA_BOLD		"Fonts/Montserrat-ExtraBold.ttf"

class AppFonts
{

public:
	static void init(ImGuiIO& io);

	static void pushLabel();
	static void pushTitle();
	static void pushInput();
	static void pushInputArrow();
	static void pushSugoiDekai();
	static void pop();

	static ImFont* label;
	static ImFont* title;
	static ImFont* input;
	static ImFont* inputArrow;
	static ImFont* sugoidekai;
};

