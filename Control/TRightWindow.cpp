#pragma once
#include "TRightWindow.h"
#include <CommCtrl.h>
#include "..\Common\tchar_head.h"
#include <stdio.h>
#pragma comment(lib, "comctl32.lib")

#include "..\resource.h"
#include "..\Control\TEdit.h"
#include "..\Draw\TDraw.h"
#include "..\Tool\TSelectTool.h"

#include "..\Control\TMainWindow.h"

extern TMainWindow win;

TRightWindow::TRightWindow()
{
	margin = 6;
	TitleHeight = 20;
}


TRightWindow::~TRightWindow()
{
}

void TRightWindow::OnDraw(HDC hdc)
{
	SetBkMode(hdc, TRANSPARENT);

	TDraw::FillRect(hdc, &ClientRect, RGB(240, 240, 240));

	TDraw::FillRect(hdc, &rcTitle1, RGB(220, 220, 220));
	TDraw::FillRect(hdc, &rcTitle2, RGB(220, 220, 220));


	LOGPEN logpenMargin;
	logpenMargin.lopnStyle = PS_SOLID;
	logpenMargin.lopnWidth = { 1, 1 };
	logpenMargin.lopnColor = RGB(180, 180, 180);
	TDraw::DrawRect(hdc, rcRegion1, logpenMargin);
	TDraw::DrawRect(hdc, rcRegion2, logpenMargin);

	TDraw::DrawSystemFontText(hdc, TEXT("����"), TDraw::GetMarginRect(rcTitle1, 4), RGB(50, 50, 50), DT_VCENTER | DT_SINGLELINE);
	TDraw::DrawSystemFontText(hdc, TEXT("����"), TDraw::GetMarginRect(rcTitle2, 4), RGB(50, 50, 50), DT_VCENTER | DT_SINGLELINE);
}

void TRightWindow::OnLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//ShowMessage(TEXT("%d"), ListView_GetItemCount(ListView.m_hWnd));
}

void TRightWindow::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//��ʱm_hWnd��δ���£�����ʹ�á�ֻ����hWnd��
	TreeViewContent.CreateTreeViewEx(hWnd, IDC_TREEVIEW, m_hInst);
	TreeViewContent.Initial();
	SetTreeViewPos();

	ListView.CreateListViewEx(hWnd, IDC_LISTVIEW, m_hInst);
	SetListViewPos();
	ListView.AddColumn(TEXT("��Ŀ"), 60);
	ListView.AddColumn(TEXT("ֵ"), 108);

}

void TRightWindow::OnNotify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	int wmEvent = HIWORD(wParam);
	if (wmId == IDC_TREEVIEW)
	{
		LPNMHDR lpnmhdr = (LPNMHDR)lParam;
		switch (lpnmhdr->code)
		{
		case NM_RCLICK:
		{
			static POINT pt;
			GetCursorPos(&pt);

			ScreenToClient(lpnmhdr->hwndFrom, &pt);

			HTREEITEM hTreeItem=TreeView_HitTest(lpnmhdr->hwndFrom, &pt);

			//�õ���ѡ���id
			int id = TreeViewContent.GetIdFromHTreeView(hTreeItem);

			if (id != -1)
			{

				//������ǰ���ߣ�ʹ��ѡ�񹤾�
				if (win.m_ManageTool.m_uiCurActiveTool != ID_SELECT &&
					win.m_ManageTool.m_uiCurActiveTool != ID_DRAG)
					win.m_ManageTool.SetCurActiveTool(ID_SELECT);

				//����ѡ�񹤾�
				((TSelectTool *)win.m_ManageTool.m_pCurrentTool)->SelectById(id,false,true);

				////ˢ����ʾ
				//win.Canvas.Invalidate();
				
				//�����Ҽ��˵�
				HMENU hMenu = LoadMenu(m_hInst, MAKEINTRESOURCE(IDR_MENU_RIGHT));
				hMenu = GetSubMenu(hMenu, 0);
				ClientToScreen(lpnmhdr->hwndFrom, &pt);

				TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, m_hParent, NULL);
			}

			break;
		}
		case TVN_SELCHANGED://�ı�ѡ��
		{
			LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lParam;
			if (pnmtv->itemNew.hItem == NULL)//û��ѡ��
				break;

			//�õ���ѡ���id
			int id = TreeViewContent.GetIdFromHTreeView(pnmtv->itemNew.hItem);

			//������ǰ���ߣ�ʹ��ѡ�񹤾�
			if (win.m_ManageTool.m_uiCurActiveTool != ID_SELECT &&
				win.m_ManageTool.m_uiCurActiveTool != ID_DRAG)
				win.m_ManageTool.SetCurActiveTool(ID_SELECT);

			//����ѡ�񹤾�
			((TSelectTool *)win.m_ManageTool.m_pCurrentTool)->SelectById(id,false,true);

			////ˢ��ListView
			//if (id != -1)
			//	win.m_Shape.GetElementById(id)->NoticeListView(&ListView);

			////ˢ����ʾ
			//win.Canvas.Invalidate();

			break;
		}
		}
	}
}

void TRightWindow::SetTreeViewPos()
{
	rcRegion1.left = ClientRect.left + margin;
	rcRegion1.right = ClientRect.right - margin;
	rcRegion1.top = ClientRect.top;//+ margin
	rcRegion1.bottom = ClientRect.bottom / 2 - margin / 2;
	rcTitle1 = rcRegion1;
	rcTitle1.bottom = rcTitle1.top + TitleHeight;

	TreeViewContent.SetRect(
		rcRegion1.left + 1,
		rcRegion1.top + TitleHeight + 1,
		rcRegion1.right - 1,
		rcRegion1.bottom - 1);
}

void TRightWindow::SetListViewPos()
{
	rcRegion2.left = ClientRect.left + margin;
	rcRegion2.right = ClientRect.right - margin;
	rcRegion2.top = ClientRect.bottom / 2 + margin / 2;
	rcRegion2.bottom = ClientRect.bottom;// -margin
	rcTitle2 = rcRegion2;
	rcTitle2.bottom = rcTitle2.top + TitleHeight;

	ListView.SetRect(
		rcRegion2.left + 1,
		rcRegion2.top + TitleHeight + 1,
		rcRegion2.right - 1,
		rcRegion2.bottom - 1);
}


void TRightWindow::OnSize(WPARAM wParam, LPARAM lParam)
{
	SetTreeViewPos();
	SetListViewPos();
	Invalidate();
}