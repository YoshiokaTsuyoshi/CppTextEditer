#pragma once

#include <Windows.h>
#include <cstdint>

class App {
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	uint32_t	m_Width;
	uint32_t	m_Height;
public:
	App(uint32_t width, uint32_t height);
	~App();
	void Run();
private:
	bool InitApp();
	void TermApp();
	bool InitWnd();
	void TermWnd();
	void MainLoop();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
};