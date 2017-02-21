#pragma once
#include <vector>
#include <Windows.h>

#include "TWindow.h"
#include "DPOINT.h"
#include "TLine.h"
class TConfiguration;
class TGraph :
	public TWindow
{
private:
	TLine LineMouseX, LineMouseY;
	POINT ptMouse;
	int iPtCount;
	POINT *ptArray;
	double x_max, y_max, x_min, y_min;
	double x_len, y_len;
	TConfiguration *pConfig;
	RECT rcGraph;
	std::vector<DPOINT> dptVector;
	int iPick;
	void TGraph::OnSize(WPARAM wParam, LPARAM lParam);
	void TGraph::OnDraw(HDC hdc);
	bool TGraph::OnClose();
	void TGraph::OnMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void TGraph::OnLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void TGraph::CalcPointArray();
public:
	TGraph(TConfiguration *pConfig);
	~TGraph();
	void TGraph::InputDptVector(std::vector<DPOINT> &dptInputVector);
};
