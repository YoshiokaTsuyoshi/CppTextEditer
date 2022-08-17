#include <iostream>
#include "App.h"

namespace {
	const auto ClassName = TEXT("SampleWindowClass");
}

App::App(uint32_t width, uint32_t height) : m_hInst(nullptr), m_hWnd(nullptr), m_Width(width), m_Height(height), m_text(L"") {
	
}

App::~App() {
	
}

void App::Run() {
	if (InitApp()) { MainLoop(); }
	TermApp();
}

bool App::InitApp() {
	if (!InitWnd()) { return false; }
	return true;
}

void App::TermApp() {
	TermWnd();
}

bool App::InitWnd() {
	auto hInst = GetModuleHandle(nullptr);
	if (hInst == nullptr) { return false; }
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wc.hCursor = LoadCursor(hInst, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = ClassName;
	wc.hIconSm = LoadIcon(hInst, IDI_APPLICATION);
	if (!RegisterClassEx(&wc)) { return false; }
	m_hInst = hInst;
	RECT rc = {};
	rc.right = static_cast<LONG>(m_Width);
	rc.bottom = static_cast<LONG>(m_Height);
	auto style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&rc, style, FALSE);
	m_hWnd = CreateWindowEx(0, ClassName, TEXT("Sample"), style, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, m_hInst, nullptr);
	if (m_hWnd == nullptr) { return false; }
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);
	SetFocus(m_hWnd);
	return true;
}

void App::TermWnd() {
	if (m_hInst != nullptr) { UnregisterClass(ClassName, m_hInst); }
	m_hInst = nullptr;
	m_hWnd = nullptr;
}

void App::MainLoop() {
	MSG msg = {};
	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

LRESULT CALLBACK App::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	static HDC hDC;
	static HRGN hRgn;
	static HFONT hFontOrigin, hFont;
	static int vPosText = 0;
	App* mainApp;
	switch (msg) {
	case WM_CREATE:
	{
		hDC = GetDC(hWnd);
		hFont = CreateFont(36, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DRAFT_QUALITY, FIXED_PITCH, TEXT("MS ÉSÉVÉbÉN"));
		hFontOrigin = (HFONT)SelectObject(hDC, hFont);
		//float dpi = GetDpiForWindow(hWnd);
		//SetWindowPos(hWnd, NULL, NULL, NULL, static_cast<int>(ceil(640.f * dpi / 96.f)), static_cast<int>(ceil(480.f * dpi / 96.f)), SWP_NOMOVE);
		DEVMODE deviceMode = {};
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &deviceMode);
		//std::cout << deviceMode.dmLogPixels << std::endl;
		//std::cout << GetDpiForWindow(hWnd) << std::endl;

		/*
		mainApp = (App*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		RECT tempRect;
		GetClientRect(hWnd, &tempRect);
		mainApp->m_Height = tempRect.bottom;
		mainApp->m_Width = tempRect.right;
		hRgn = CreateRectRgn(0, vPosText, mainApp->m_Width, vPosText + 36);
		SelectClipRgn(hDC, hRgn);
		*/
	}
	return msg;
	case WM_SHOWWINDOW:
	{
		mainApp = (App*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		RECT tempRect;
		GetClientRect(hWnd, &tempRect);
		mainApp->m_Height = tempRect.bottom;
		mainApp->m_Width = tempRect.right;
		hRgn = CreateRectRgn(0, vPosText, mainApp->m_Width, vPosText + 36);
		SelectClipRgn(hDC, hRgn);
		SetTextAlign(hDC, TA_LEFT | TA_BOTTOM);
	}
	return msg;
	case WM_DESTROY:
	{
		mainApp = (App*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		for (int i = 0; i < mainApp->m_text.size(); i++) std::wcout << mainApp->m_text.c_str()[i] << std::flush;
		
		SelectClipRgn(hDC, NULL);
		DeleteObject(hRgn);

		SelectObject(hDC, hFontOrigin);
		DeleteObject(hFont);

		ReleaseDC(hWnd, hDC);
		PostQuitMessage(0);
	}
	return msg;
	case WM_KEYDOWN:
	{
		switch (wp) {
		case VK_RETURN:
			//std::cout << "return" << std::endl;
			//SetTextAlign(hDC, TA_BOTTOM);
			//TextOut(hDC, 0, 0, L"\n", 1);
			break;
		}
	}
	return msg;
	case WM_CHAR:
	{
		mainApp = (App*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		switch (wp) {
		case 13:
			std::cout << "Return" << std::endl;
			vPosText += 36;

			SelectClipRgn(hDC, NULL);
			DeleteObject(hRgn);
			//SelectObject(hDC, hFontOrigin);

			//ReleaseDC(hWnd, hDC);
			//hDC = GetDC(hWnd);

			hRgn = CreateRectRgn(0, vPosText, mainApp->m_Width, vPosText + 36);
			SelectClipRgn(hDC, hRgn);

			//hFontOrigin = (HFONT)SelectObject(hDC, hFont);

			mainApp->m_text += L'\n';
			break;
		default:
			WCHAR str = (WCHAR)wp;
			TextOut(hDC, 0, 0, (LPCWSTR)&str, 1);
			//if (GetTextAlign(hDC) != TA_UPDATECP) SetTextAlign(hDC, TA_UPDATECP);
			mainApp->m_text += str;
		}
	}
	return msg;
	case WM_PAINT:
	{
		mainApp = (App*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);

		RECT tempRect;
		GetClientRect(hWnd, &tempRect);
		mainApp->m_Height = tempRect.bottom;
		mainApp->m_Width = tempRect.right;
		SelectClipRgn(hDC, NULL);
		DeleteObject(hRgn);
		hRgn = CreateRectRgn(0, vPosText, mainApp->m_Width, vPosText + 36);
		SelectClipRgn(hDC, hRgn);

		SetTextAlign(hDC, TA_LEFT);
		TextOut(hDC, 0, 0, mainApp->m_text.c_str(), mainApp->m_text.size());
		SetTextAlign(hDC, TA_LEFT | TA_BOTTOM);
	}
	return msg;
	default:
	{}
	break;
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}