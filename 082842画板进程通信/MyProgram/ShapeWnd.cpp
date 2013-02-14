#include "stdafx.h"
#include "Resource.h"
#include "ShapeWnd.h"
LRESULT				CALLBACK CShapeWnd::WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CShapeWnd * pWnd = NULL;
	if ( message == WM_CREATE)
	{
		hwndTemp=hWnd;
		pWnd = (CShapeWnd *)(((LPCREATESTRUCT)lParam)->lpCreateParams);
		SetWindowLong(hWnd, GWL_USERDATA, (LONG) pWnd);
	}
	else
	{
		pWnd = (CShapeWnd *)::GetWindowLong(hWnd, GWL_USERDATA);
	}
	if(pWnd)
	{
		return pWnd->WindowProc(message, wParam, lParam);
	}
	else
	{
		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}
}


BOOL				CShapeWnd::ShapeWndReg(HINSTANCE hInst)
{
	LoadString(hInst, IDS_APP_TITLE, szWindowTitle, MAX_LOADSTRING);
	LoadString(hInst, IDC_MYPROGRAM, szWindowClass, MAX_LOADSTRING);
	WNDCLASSEX wcex;
	
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW|CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProcStatic;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 4;
	wcex.hInstance		= hInst;
	wcex.hIcon			= LoadIcon(hInst, MAKEINTRESOURCE(IDI_MYPROGRAM));
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MYPROGRAM);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);
	
	return TRUE;

}

INT_PTR CALLBACK	CShapeWnd::ChangePenWidth(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK )
		{
			TCHAR buf[10];
			HWND hEdit=GetWindow(hDlg,GW_CHILD);
			GetWindowText(hEdit,buf,4);
			if (buf[0]==L'\0')
			{
				MessageBox(hDlg,L"输入出错,请重新输入",L"ERROR",MB_OK);
				return (INT_PTR)TRUE;
			}
			g_PenWidth=_wtoi(buf);
			if (g_PenWidth>15)
			{
				MessageBox(hDlg,L"笔宽过大,请重新输入",L"ERROR",MB_OK);
				g_PenWidth=10;
				break;
			}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK	CShapeWnd::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
BOOL CALLBACK		CShapeWnd::lpEnumFunc(HWND hwnd, LPARAM lParam)
{
	TCHAR buf[TCHAR_BUF_SIZE];
	GetClassName(hwnd,buf,TCHAR_BUF_SIZE);
	LPFORSENDMSG pfsm;
	pfsm=(LPFORSENDMSG)lParam;
	if(0==wcscmp(buf,szWindowClass))
	{

		if (pfsm->hWnd!=hwnd)//不是操作的主窗口
		{
			if (pfsm->msg==WM_FILEOPEN)
			{
				COPYDATASTRUCT cp;
				cp.dwData=WM_FILEOPEN;
				cp.cbData=pfsm->wParam;
				cp.lpData=(PVOID)pfsm->lParam;
				SendMessage(hwnd,WM_COPYDATA,0,(LPARAM)(LPVOID)&cp);
			}
			else
			{
				PostMessage(hwnd,((pfsm->msg)|0x1000),pfsm->wParam,pfsm->lParam);
			}

		}
	}
	return TRUE;
}
BOOL CALLBACK		CShapeWnd::lpEnumFuncForCreate(HWND hwnd, LPARAM lParam)
{
	TCHAR buf[TCHAR_BUF_SIZE];
	GetClassName(hwnd,buf,TCHAR_BUF_SIZE);
	if(0==wcscmp(buf,szWindowClass))
	{
		if ((HWND)lParam!=hwnd)
		{
			PostMessage(hwnd,WM_INITCREATE,0,(LPARAM)hwndTemp);
			return FALSE;
		}

	}
	return TRUE;
}
BOOL				CShapeWnd::CreatShapeWindow(HINSTANCE hInst)
{
	m_hInst=hInst;
	m_hWnd=CreateWindow(szWindowClass,szWindowTitle, WS_OVERLAPPEDWINDOW&~WS_SIZEBOX,
		0, 0, 600, 600, NULL, NULL, hInst,(LPVOID)this);

	if (!m_hWnd)
	{
		return FALSE;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	return TRUE;
}
LRESULT				CShapeWnd::WindowProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	//======================================用于过滤消息
	if ((message&0x1000)==0x1000)//表示是从其他进程里面发送过来的
	{
		message=(message&0x0fff);
		b_IsFromOtherProcess=TRUE;
	}
	else//表示是本进程的消息
	{
		b_IsFromOtherProcess=FALSE;
		//开始过滤消息
		if (AfxMsgCanPass(message,MsgCanPass,sizeof(MsgCanPass)>>2))
		{
			ForSendMsg.GetInfo(m_hWnd,message,wParam,lParam);
			EnumWindows(lpEnumFunc,(LPARAM)&ForSendMsg);
		}
		if (message==WM_COMMAND)
		{
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			if (AfxMsgCanPass(wmId,CMDMsgCanPass,sizeof(CMDMsgCanPass)>>2))
			{
				ForSendMsg.GetInfo(m_hWnd,message,wParam,lParam);
				EnumWindows(lpEnumFunc,(LPARAM)&ForSendMsg);
			}

		}
	}
	if (message==WM_COMMAND)//CMD消息路由
	{
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		for (int i=0;i<CMD_MSG_NUM;i++)
		{
			if (wmId==cmdmsgmap[i].message)
			{
				(this->*(cmdmsgmap[i].pFn))(wParam,lParam);
				return 0;
			}
		}
		return DefWindowProc(m_hWnd, message, wParam, lParam);
	}
	for (int i=0;i<MSG_NUM;i++)//非CMD消息路由
	{
		if (message==msgmap[i].message)
		{
			(this->*msgmap[i].pFn)(wParam,lParam);
			return 0;
		}
	}
	return DefWindowProc(m_hWnd, message, wParam, lParam);
}


					CShapeWnd::CShapeWnd()
{
	TCHAR tempfilter[MAX_LOADSTRING]=L"图形文件(*.shape)\0*.shape\0\0";
	memcpy(szFileFilter,tempfilter,MAX_LOADSTRING*sizeof(TCHAR));
	b_beginDrawPloy=TRUE;
	ShapeType=0;
	g_color=RGB(255,0,0);
	g_PenStyle=PS_SOLID;
	g_PenWidth=10;
	DrawStatus=DRAWLINE;
	ZeroMemory(&cc,sizeof(CHOOSECOLOR));
	cc.lStructSize=sizeof(CHOOSECOLOR);
	ZeroMemory(g_rgbCustom,sizeof(COLORREF)*16);
	ZeroMemory(szPenWidth,30);
	b_IsFromOtherProcess=FALSE;

	UINT tempMsgCanPass[]={WM_LBUTTONDOWN,WM_LBUTTONUP,WM_RBUTTONDOWN,WM_LBUTTONDBLCLK,WM_MOUSEMOVE,WM_DESTROY};
	memcpy(MsgCanPass,tempMsgCanPass,sizeof(UINT)*6);

	UINT tempCMDMsgCanPass[]={IDM_SELECT,IDM_CUT,IDM_COPY,IDM_PASTE,IDM_UNDO,IDM_RESTORE,
		IDM_DELETE,IDM_LINE,IDM_CIRCLE,IDM_RECT,IDM_PENCIL,IDM_PLOY};
	memcpy(CMDMsgCanPass,tempCMDMsgCanPass,12*sizeof(UINT));

	MSGMAP msgmapTemp[MSG_NUM]=
	{
		{WM_CREATE,&CShapeWnd::OnCreate},
		{WM_CHANGECOLOR,&CShapeWnd::OnChangeColor},
		{WM_PENWIDTH,&CShapeWnd::OnPenWidth},
		{WM_COPYDATA,&CShapeWnd::OnCopyData},
		{WM_ERASEBKGND,&CShapeWnd::OnEraseBkGnd},
		{WM_SIZE,&CShapeWnd::OnSize},
		{WM_MOVE,&CShapeWnd::OnMove},
		{WM_LBUTTONDBLCLK,&CShapeWnd::OnLButtonDbclk},
		{WM_LBUTTONDOWN,&CShapeWnd::OnLButtonDown},
		{WM_LBUTTONUP,&CShapeWnd::OnLButtonUp},
		{WM_MOUSEMOVE,&CShapeWnd::OnMouseMove},
		{WM_RBUTTONDOWN,&CShapeWnd::OnRButtonDown},
		{WM_PAINT,&CShapeWnd::OnPaint},
		{WM_CLOSE,&CShapeWnd::OnClose},
		{WM_DESTROY,&CShapeWnd::OnDestory},
		{WM_INITCREATE,&CShapeWnd::OnInitCreate},
		{WM_INITFINISH,&CShapeWnd::OnInitCreateFinish},
	};
	memcpy(msgmap,msgmapTemp,sizeof(MSGMAP)*MSG_NUM);
	MSGMAP cmdmsgmapTemp[CMD_MSG_NUM]=
	{
		{IDM_LINE,&CShapeWnd::OnLine},
		{IDM_RECT,&CShapeWnd::OnRect},
		{IDM_PLOY,&CShapeWnd::OnPloy},
		{IDM_PENCIL,&CShapeWnd::OnPencil},
		{IDM_CIRCLE,&CShapeWnd::OnCircle},
		{IDM_SAVE,&CShapeWnd::OnSave},
		{IDM_SAVEAS,&CShapeWnd::OnSaveAs},
		{IDM_OPEN,&CShapeWnd::OnOpen},
		{IDM_SELECT,&CShapeWnd::OnSelect},
		{IDM_CUT,&CShapeWnd::OnCut},
		{IDM_COPY,&CShapeWnd::OnCopy},
		{IDM_PASTE,&CShapeWnd::OnPaste},
		{IDM_DELETE,&CShapeWnd::OnDelete},
		{IDM_UNDO,&CShapeWnd::OnUndo},
		{IDM_RESTORE,&CShapeWnd::OnRestore},
		{IDM_PENWIDTH,&CShapeWnd::OnChangePenWidth},
		{IDM_CHOOSECOLOR,&CShapeWnd::OnChangeColor},
		{IDM_ABOUT,&CShapeWnd::OnAbout},
	};
	memcpy(cmdmsgmap,cmdmsgmapTemp,sizeof(MSGMAP)*CMD_MSG_NUM);

}
VOID				CShapeWnd::OnCreateToolBar()
{
	TBBUTTON tb[NUM_BUTTONS]={
		{0,IDM_OPEN,TBSTATE_ENABLED,BTNS_BUTTON,0,0},
		{1,IDM_SAVE,TBSTATE_ENABLED,BTNS_BUTTON,0,0},
		{0,0,TBSTATE_ENABLED, TBSTYLE_SEP,   0L, -1},
		{2,IDM_CUT,TBSTATE_ENABLED,BTNS_BUTTON,0,0},
		{3,IDM_COPY,TBSTATE_ENABLED,BTNS_BUTTON,0,0},
		{4,IDM_PASTE,TBSTATE_ENABLED,BTNS_BUTTON,0,0},
		{0,0,TBSTATE_ENABLED, TBSTYLE_SEP,   0L, -1},
		{5,IDM_SELECT,TBSTATE_ENABLED,BTNS_BUTTON,0,0},
		//	{6,IDM_SCALING,TBSTATE_ENABLED,BTNS_BUTTON,0,0},
		{7,IDM_DELETE,TBSTATE_ENABLED,BTNS_BUTTON,0,0},
		{0,0,TBSTATE_ENABLED, TBSTYLE_SEP,   0L, -1},
		{8,IDM_UNDO,TBSTATE_ENABLED,BTNS_BUTTON,0,0},
		{9,IDM_RESTORE,TBSTATE_ENABLED,BTNS_BUTTON,0,0},    //12
		{0,0,TBSTATE_ENABLED, TBSTYLE_SEP,   0L, -1},
		{10,IDM_LINE,TBSTATE_ENABLED,BTNS_BUTTON,0,0},
		{11,IDM_RECT,TBSTATE_ENABLED,BTNS_BUTTON,0,0},
		{12,IDM_CIRCLE,TBSTATE_ENABLED,BTNS_BUTTON,0,0},
		{13,IDM_PLOY,TBSTATE_ENABLED,BTNS_BUTTON,0,0},
		{14,IDM_PENCIL,TBSTATE_ENABLED,BTNS_BUTTON,0,0},
	};
	//==========================================================
	hTool = CreateWindow(_T("ToolBarWindow32"), NULL, WS_CHILD | 
		WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS|WS_BORDER |   
		CCS_ADJUSTABLE, 0, 0, 0, 0, m_hWnd, NULL, m_hInst, NULL);
//	ShowWindow(hTool,SW_SHOW);
//	UpdateWindow(hTool);
	TBADDBITMAP   tbab;   
	tbab.hInst=m_hInst;   
	tbab.nID=IDB_BITMAP1;
	SendMessage(hTool,   TB_BUTTONSTRUCTSIZE,   sizeof(TBBUTTON),   0);   
	SendMessage(hTool,   TB_ADDBITMAP,   NUM_BUTTONS,   (LPARAM)&tbab); 
	SendMessage(hTool,   TB_BUTTONSTRUCTSIZE,   (WPARAM)   sizeof(TBBUTTON),   0);       
	SendMessage(hTool,   TB_ADDBUTTONS,   (WPARAM)   NUM_BUTTONS,   (LPARAM)   (LPTBBUTTON)   &tb);
	ShowWindow(hTool,   SW_SHOW);     
	UpdateWindow(hTool);
	//==========================================================
	m_bLBtnIsDown=FALSE;
	rect.top=rect.left=0;
	rect.right=rect.bottom=800;
	rectMax.bottom=rectMax.left=rectMax.right=rectMax.top=0;
	b_ShapeSelected=FALSE;
	b_IsMove=FALSE;
	b_IsSaved=FALSE;
	b_IsScaling=FALSE;
	hCursorCross=LoadCursor(NULL,IDC_CROSS);
	SetCursor(LoadCursor(m_hInst,MAKEINTRESOURCE(IDC_CURSOR1)));
	AfxAddStringStar(szWindowTitle,szWindowTitleStar);
}
VOID				CShapeWnd::OnColor(WPARAM wParam,LPARAM lParam)
{
	g_color=wParam;
}
VOID				CShapeWnd::OnPenWidth(WPARAM wParam,LPARAM lParam)
{
	g_PenWidth=wParam;
}
VOID				CShapeWnd::OnCopyData(WPARAM wParam,LPARAM lParam)
{
	ptrArr.Clear();
	InvalidateRect(m_hWnd,NULL,TRUE);
	PCOPYDATASTRUCT pMyCDS = (PCOPYDATASTRUCT) lParam;
	shapefile.DeSerialize((TCHAR*)pMyCDS->lpData,ptrArr);
	StackForUndoRestore.Clear();
}
VOID				CShapeWnd::OnEraseBkGnd(WPARAM wParam,LPARAM lParam)
{

}
VOID				CShapeWnd::OnSize(WPARAM wParam,LPARAM lParam)
{
	MoveWindow(hTool, 0, 0, 0, 0, TRUE);//用于工具栏
	GetWindowRect(m_hWnd,&rectWindow);
}
VOID				CShapeWnd::OnMove(WPARAM wParam,LPARAM lParam)
{
	GetWindowRect(m_hWnd,&rectWindow);
}

VOID				CShapeWnd::OnLButtonDbclk(WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	m_DbclkMousePos.x=LOWORD(lParam);
	m_DbclkMousePos.y=HIWORD(lParam);
	hdc=GetDC(m_hWnd);
	if (DrawStatus!=SELECT)
	{
		return;
	}
	if (b_ShapeSelected==TRUE)
	{
		AfxEnlargeRect(rectForSelected);
		InvalidateRect(m_hWnd,&rectForSelected, TRUE);
	}
	b_ShapeSelected=FALSE;

	for (int i=0;i<ptrArr.GetSize();i++)
	{
		if (ptrArr.GetAt(i)==NULL)
		{
			continue;
		}
		if (((CShape*)ptrArr.GetAt(i))->IsSelected(m_DbclkMousePos))
		{
			rectForSelected=((CShape*)ptrArr.GetAt(i))->GetItsRect();
			AfxDrawSelectedRect(hdc,rectForSelected);
			nIndexSelected=i;
			b_ShapeSelected=TRUE;
			break;
		}
	}
	InvalidateRect(m_hWnd,NULL,TRUE);
	ReleaseDC(m_hWnd,hdc);
}
VOID				CShapeWnd::OnLButtonDown(WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	hdc=GetDC(m_hWnd);


	m_pt.x=LOWORD(lParam);
	m_pt.y=HIWORD(lParam);
	m_bLBtnIsDown=TRUE;
	OutputDebugString(L"LBUTTONDOWN");
	//********最小化重绘矩形
	{
		rectMax.left=rectMax.right=m_pt.x;
		rectMax.top=rectMax.bottom=m_pt.y;
	}

	if (b_IsFromOtherProcess==FALSE)
	{
		RECT rect;
		GetWindowRect(m_hWnd,&rectWindow);
		rect=rectWindow;
		rect.top+=80;
		rect.bottom-=20;
		rect.left+=20;
		rect.right-=20;
		ClipCursor(&rect);
	}


	if (DrawStatus==SELECT)//////////////////////////////////////////////////////
	{
		if (!b_ShapeSelected)//如果此时没有选择任何图形
		{
			ReleaseDC(m_hWnd,hdc);
			return;
		}
		POINT pt1,pt2,pt3,pt4;

		pt4.x=pt1.x=rectForSelected.left;
		pt1.y=pt2.y=rectForSelected.top;
		pt2.x=pt3.x=rectForSelected.right;
		pt3.y=pt4.y=rectForSelected.bottom;
		BOOL canScaling=FALSE;
		if (AfxTwoPointDistance(m_pt,pt1)<(SELECTDISTANCE>>1))
		{
			g_EnumBasePt=RIGHTBOTTOM;
			g_BasePt.x=rectForSelected.right;
			g_BasePt.y=rectForSelected.bottom;
			canScaling=TRUE;
		}
		else if (AfxTwoPointDistance(m_pt,pt2)<(SELECTDISTANCE>>1))
		{
			g_EnumBasePt=LEFTBOTTOM;
			g_BasePt.x=rectForSelected.left;
			g_BasePt.y=rectForSelected.bottom;
			canScaling=TRUE;
		}
		else if (AfxTwoPointDistance(m_pt,pt3)<(SELECTDISTANCE>>1))
		{
			g_EnumBasePt=LEFTTOP;
			g_BasePt.x=rectForSelected.left;
			g_BasePt.y=rectForSelected.top;
			canScaling=TRUE;
		}
		else if (AfxTwoPointDistance(m_pt,pt4)<(SELECTDISTANCE>>1))
		{
			g_EnumBasePt=RIGHTTOP;
			g_BasePt.x=rectForSelected.right;
			g_BasePt.y=rectForSelected.top;
			canScaling=TRUE;
		}
		if (canScaling==TRUE)
		{
			b_IsScaling=TRUE;
			ShapeType=((CShape*)ptrArr.GetAt(nIndexSelected))->What_Am_I();

			OutputDebugString(L"IsScaling");
		}


	}
	if (DrawStatus==SELECT&&b_IsScaling==FALSE)
	{
		if (b_ShapeSelected==TRUE)
		{
			if (AfxPointIsInRect(m_pt,rectForSelected))
			{

				b_IsMove=TRUE;
				g_lpUndo=new UNDO_RESTORE_INFO;
				g_lpUndo->pSh=(CShape*)ptrArr.GetAt(nIndexSelected);
			}
			else
			{
				b_ShapeSelected=FALSE;
				b_IsMove=FALSE;
				InvalidateRect(m_hWnd,NULL,TRUE);
			}
		}
	}

	if (DrawStatus==DRAWPENCIL)
	{
		pPt=new POINT;
		pPt->x=m_pt.x;
		pPt->y=m_pt.y;
		ptrArrTemp.Add(pPt);

	}
	if (DrawStatus==DRAWPLOY)
	{
		if (b_beginDrawPloy==TRUE)
		{
			pPt=new POINT;
			pPt->x=m_pt.x;
			pPt->y=m_pt.y;
			ptrArrTemp.Add(pPt);

		}
		else
		{
			AfxDrawLine(hdc,m_LbtnUpPos,m_pt,g_PenStyle,g_PenWidth,g_color);
			AfxTwoPointsToRect(rect,m_LbtnUpPos,m_pt);
			AfxFixMaxRect(rectMax,rect);

		}
	}
	ReleaseDC(m_hWnd,hdc);
}
VOID				CShapeWnd::OnMouseMove(WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	m_mousePos.x=LOWORD(lParam);
	m_mousePos.y=HIWORD(lParam);
	hdc=GetDC(m_hWnd);
	if (DrawStatus==DRAWCIRCLE||DrawStatus==DRAWPLOY||DrawStatus==DRAWLINE||
		DrawStatus==DRAWRECT||DrawStatus==DRAWPENCIL)
	{
		SetCursor(LoadCursor(m_hInst,MAKEINTRESOURCE(IDC_CURSOR1)));
	}
	else
	{
		SetCursor(LoadCursor(NULL,IDC_ARROW));
	}

	if (b_IsScaling==FALSE&&b_ShapeSelected==TRUE&&DrawStatus==SELECT)/////////////////////////////////
	{
		POINT pt1,pt2,pt3,pt4;

		pt4.x=pt1.x=rectForSelected.left;
		pt1.y=pt2.y=rectForSelected.top;
		pt2.x=pt3.x=rectForSelected.right;
		pt3.y=pt4.y=rectForSelected.bottom;

		if (AfxTwoPointDistance(m_mousePos,pt1)<(SELECTDISTANCE>>1)||
			AfxTwoPointDistance(m_mousePos,pt3)<(SELECTDISTANCE>>1)
			)
		{
			SetCursor(LoadCursor(NULL,IDC_SIZENWSE));
		}
		else if (AfxTwoPointDistance(m_mousePos,pt2)<(SELECTDISTANCE>>1)||
			AfxTwoPointDistance(m_mousePos,pt4)<(SELECTDISTANCE>>1))
		{
			SetCursor(LoadCursor(NULL,IDC_SIZENESW));
		}
	}
	if (m_bLBtnIsDown==TRUE)
	{
		switch (DrawStatus)
		{
		case DRAWLINE:
		case DRAWRECT:
		case DRAWCIRCLE:
			//确定此次的矩形区域
			AfxTwoPointsToRect(rect,m_pt,m_mousePos);
			//确定最大重绘矩形区域,为了防止先画出大矩形后再画小矩形导致外部的一部分没有重绘
			AfxFixMaxRect(rectMax,rect);
			rectMaxForPenWidth=rectMax;
			AfxEnlargeRect(rectMaxForPenWidth,g_PenWidth/2+1);
			InvalidateRect(m_hWnd,&rectMaxForPenWidth,TRUE);
			break;
		case DRAWPLOY:
			if(b_beginDrawPloy==TRUE)
			{//确定此次的矩形区域
				AfxTwoPointsToRect(rect,m_pt,m_mousePos);
				AfxFixMaxRect(rectMax,rect);//确定最大重绘矩形区域,为了防止先画出大矩形后再画小矩形导致外部的一部分没有重绘
			}
			else
			{
				AfxTwoPointsToRect(rect,m_mousePos,m_LbtnUpPos);
				AfxFixMaxRect(rectMax,rect);
			}

			{
				RECT rect=rectMax;
				AfxEnlargeRect(rect,15);
				InvalidateRect(m_hWnd,&rect,TRUE);
			}

			break;
		case DRAWPENCIL:
			AfxDrawLine(hdc,*((POINT*)ptrArrTemp.GetAt(ptrArrTemp.GetSize()-1)),
				m_mousePos,g_PenStyle,g_PenWidth,g_color);
			pPt=new POINT;
			pPt->x=m_mousePos.x;
			pPt->y=m_mousePos.y;
			ptrArrTemp.Add(pPt);
			break;
		case SELECT:
			if (b_IsMove==TRUE&&b_ShapeSelected==TRUE)
			{
				SetCursor(hCursorCross);
				((CShape*)ptrArr.GetAt(nIndexSelected))->Move(m_mousePos.x-m_mouseLastPos.x,m_mousePos.y-m_mouseLastPos.y);
				RECT rect=rectForSelected;
				AfxFixMaxRect(rect,rectForSelected);
				AfxRectMove(&rectForSelected,m_mousePos.x-m_mouseLastPos.x,m_mousePos.y-m_mouseLastPos.y);	
				AfxEnlargeRect(rect,15);
				InvalidateRect(m_hWnd,&rect,TRUE);
			}
			//break;



			if (b_IsScaling==TRUE&&b_ShapeSelected==TRUE)
			{
				g_xScale=(float(m_mousePos.x-g_BasePt.x))/(m_pt.x-g_BasePt.x);
				g_yScale=(float(m_mousePos.y-g_BasePt.y))/(m_pt.y-g_BasePt.y);
				switch (ShapeType)
				{
				case ISCLINE:

					LineTemp=*((CLine*)ptrArr.GetAt(nIndexSelected));
					LineTemp.Scaling(g_EnumBasePt,g_xScale,g_yScale);
					rectForSelected=LineTemp.GetItsRect();
					break;
				case ISCCIRCLE:
					CircleTemp=*((CCircle*)ptrArr.GetAt(nIndexSelected));
					CircleTemp.Scaling(g_EnumBasePt,g_xScale,g_yScale);
					rectForSelected=CircleTemp.GetItsRect();
					break;
				case ISCRECTANGLE:
					RectangleTemp=*((CRectangle*)ptrArr.GetAt(nIndexSelected));
					RectangleTemp.Scaling(g_EnumBasePt,g_xScale,g_yScale);
					rectForSelected=RectangleTemp.GetItsRect();
					break;
				case ISCPENCIL:
					PencilTemp=*((CPencil*)ptrArr.GetAt(nIndexSelected));
					PencilTemp.Scaling(g_EnumBasePt,g_xScale,g_yScale);
					rectForSelected=PencilTemp.GetItsRect();
					break;
				case ISCPLOY:
					PloyTemp=*((CPloy*)ptrArr.GetAt(nIndexSelected));
					PloyTemp.Scaling(g_EnumBasePt,g_xScale,g_yScale);
					rectForSelected=PloyTemp.GetItsRect();
					break;
				}


				AfxTwoPointsToRect(rect,g_BasePt,m_mousePos);
				AfxFixMaxRect(rectMax,rect);

				rectMaxForPenWidth=rectMax;
				AfxEnlargeRect(rectMaxForPenWidth,g_PenWidth/2+1);
				InvalidateRect(m_hWnd,&rectMaxForPenWidth,TRUE);
			}
			ReleaseDC(m_hWnd,hdc);break;
		}
	}
	m_mouseLastPos.x=LOWORD(lParam);
	m_mouseLastPos.y=HIWORD(lParam);
	ReleaseDC(m_hWnd,hdc);
}
VOID				CShapeWnd::OnLButtonUp(WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	hdc=GetDC(m_hWnd);

	if (DrawStatus==DRAWCIRCLE||DrawStatus==DRAWPLOY||DrawStatus==DRAWLINE||
		DrawStatus==DRAWRECT||DrawStatus==DRAWPENCIL)
	{
		SetCursor(LoadCursor(m_hInst,MAKEINTRESOURCE(IDC_CURSOR1)));
	}
	else
	{
		SetCursor(LoadCursor(NULL,IDC_ARROW));
	}
	m_LbtnUpPos.x=LOWORD(lParam);
	m_LbtnUpPos.y=HIWORD(lParam);
	if (DrawStatus!=DRAWPLOY)
	{
		ClipCursor(NULL);
	}
	//********
	rectMax.bottom=rectMax.left=rectMax.right=rectMax.top=0;
	//********
	switch (DrawStatus)
	{
	case DRAWLINE:
		psh=new CLine(m_pt,m_LbtnUpPos,g_PenStyle,g_PenWidth,g_color);
		psh->Draw(hdc);
		break;
	case DRAWRECT:
		psh=new CRectangle(m_pt,m_LbtnUpPos,TRUE,RGB(0,0,0),g_color,g_PenWidth);
		psh->Draw(hdc);
		break;
	case DRAWPENCIL:
		{
			POINT *pPt=new POINT;
			pPt->x=m_LbtnUpPos.x;
			pPt->y=m_LbtnUpPos.y;
			ptrArrTemp.Add(pPt);
		}
		psh=new CPencil(g_PenStyle,g_PenWidth,g_color,ptrArrTemp);
		psh->Draw(hdc);
		ptrArrTemp.Clear();
		break;
	case DRAWCIRCLE:
		{
			RECT rectTemp;
			AfxTwoPointsToRect(rectTemp,m_pt,m_LbtnUpPos);
			int len=abs((m_pt.x-m_LbtnUpPos.x)>>1);
			psh=new CCircle(rectTemp,g_PenStyle,g_PenWidth,g_color);
			psh->Draw(hdc);
		}

		break;
	case DRAWPLOY:
		pPt=new POINT;
		pPt->x=m_LbtnUpPos.x;
		pPt->y=m_LbtnUpPos.y;
		ptrArrTemp.Add(pPt);
		b_beginDrawPloy=FALSE;InvalidateRect(m_hWnd,NULL,TRUE);
		break;

	case SELECT:
		if (b_ShapeSelected==TRUE&&b_IsMove==TRUE)
		{
			((CShape*)ptrArr.GetAt(nIndexSelected))->Move(m_LbtnUpPos.x-m_mousePos.x,m_LbtnUpPos.y-m_mousePos.y);

			b_IsMove=FALSE;
			AfxRectMove(&rectForSelected,m_LbtnUpPos.x-m_mousePos.x,m_LbtnUpPos.y-m_mousePos.y);
			AfxDrawSelectedRect(hdc,rectForSelected);
			InvalidateRect(m_hWnd,NULL,TRUE);
			g_lpUndo->nFirParam=m_LbtnUpPos.x-m_pt.x;
			g_lpUndo->nSecParam=m_LbtnUpPos.y-m_pt.y;
			g_lpUndo->nWhichChange=SHAPE_MOVE;
			g_lpUndo->Index=nIndexSelected;
			StackForUndoRestore.Push(g_lpUndo);
		}

		if (!b_IsScaling)
		{
			break;
		}
		//用于撤销

		g_xScale=(float(m_LbtnUpPos.x-g_BasePt.x))/(m_pt.x-g_BasePt.x);
		g_yScale=(float(m_LbtnUpPos.y-g_BasePt.y))/(m_pt.y-g_BasePt.y);
		((CShape*)ptrArr.GetAt(nIndexSelected))->Scaling(g_EnumBasePt,g_xScale,g_yScale);
		{
			g_lpUndo=new UNDO_RESTORE_INFO;
			g_lpUndo->Index=nIndexSelected;
			g_lpUndo->Bp=g_EnumBasePt;
			g_lpUndo->fFirParam=g_xScale;
			g_lpUndo->nWhichChange=SHAPE_SCALING;
			g_lpUndo->fSecParam=g_yScale;
			g_lpUndo->pSh=(CShape*)ptrArr.GetAt(nIndexSelected);
			StackForUndoRestore.Push(g_lpUndo);
		}
		InvalidateRect(m_hWnd,NULL,TRUE);
		rectForSelected=((CShape*)ptrArr.GetAt(nIndexSelected))->GetItsRect();
		b_IsScaling=FALSE;
		break;
	}
	m_bLBtnIsDown=FALSE;
	if (DrawStatus==DRAWLINE||DrawStatus==DRAWRECT||DrawStatus==DRAWPENCIL||DrawStatus==DRAWCIRCLE)
	{
		ptrArr.Add(psh);
		b_IsSaved=FALSE;
		SetWindowText(m_hWnd,szWindowTitleStar);

		//用于撤消操作
		{
			g_lpUndo=new UNDO_RESTORE_INFO;
			g_lpUndo->pSh=psh;
			g_lpUndo->Index=ptrArr.GetSize()-1;
			g_lpUndo->nWhichChange=SHAPE_CREATE;
			StackForUndoRestore.Push(g_lpUndo);
		}
	}	
	ReleaseDC(m_hWnd,hdc);
}
VOID				CShapeWnd::OnRButtonDown(WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	hdc=GetDC(m_hWnd);
	if (DrawStatus==DRAWPLOY)
	{
		if (ptrArrTemp.GetSize()==0)
		{
			ReleaseDC(m_hWnd,hdc);
			return ;
		}
		b_beginDrawPloy=TRUE;
		psh=new CPloy(g_PenStyle,g_PenWidth,g_color,ptrArrTemp);
		ptrArr.Add(psh);

		b_IsSaved=FALSE;
		SetWindowText(m_hWnd,szWindowTitleStar);
		psh->Draw(hdc);
		ptrArrTemp.Clear();
		ClipCursor(NULL);
		//用于撤消操作
		{
			g_lpUndo=new UNDO_RESTORE_INFO;
			g_lpUndo->pSh=psh;
			g_lpUndo->Index=ptrArr.GetSize()-1;
			g_lpUndo->nWhichChange=SHAPE_CREATE;
			StackForUndoRestore.Push(g_lpUndo);
		}
	}
	ReleaseDC(m_hWnd,hdc);
}
VOID				CShapeWnd::OnPaint(WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(m_hWnd, &ps);

	{
		HDC hdcMem;  // 内存设备描述表
		HBITMAP hBitmap; // 缓冲位图
		HGDIOBJ hOldObject; // 内存设备描述表原位图
		RECT rcClient; // 客户区矩形
		GetClientRect(m_hWnd,&rcClient);
		hdcMem = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc,rcClient.right-rcClient.left,rcClient.bottom-rcClient.top);
		hOldObject = SelectObject(hdcMem,hBitmap);
		FillRect(hdcMem,&rcClient,(HBRUSH) GetStockObject(WHITE_BRUSH));

		if (m_bLBtnIsDown==TRUE)
		{
			switch (DrawStatus)
			{
			case DRAWLINE:
				AfxDrawLine(hdcMem,m_pt,m_mousePos,g_PenStyle,g_PenWidth,g_color);
				break;
			case DRAWRECT:
				AfxDrawRect(hdcMem,m_pt,m_mousePos,TRUE,RGB(0,0,0),g_color,g_PenWidth);
				break;
			case DRAWCIRCLE:
				{
					RECT rect;
					AfxTwoPointsToRect(rect,m_pt,m_mousePos);
					AfxDrawEllipse(hdcMem,rect,g_PenStyle,g_PenWidth,g_color);
				}

				break;
			case DRAWPLOY:
				if (b_beginDrawPloy==TRUE)
				{
					AfxDrawLine(hdcMem,m_pt,m_mousePos,g_PenStyle,g_PenWidth,g_color);
				}
				else
				{
					AfxDrawLine(hdcMem,m_LbtnUpPos,m_mousePos,g_PenStyle,g_PenWidth,g_color);
					AfxDrawPloy(hdcMem,g_PenStyle,g_PenWidth,g_color,ptrArrTemp);
				}
				break;
			case SELECT:
				if (b_ShapeSelected==TRUE)
				{
					AfxDrawSelectedRect(hdcMem,rectForSelected);
				} 

				if (b_ShapeSelected==TRUE&&b_IsScaling==TRUE)
				{
					switch (ShapeType)
					{
					case ISCLINE:
						LineTemp.Draw(hdcMem);
						break;
					case ISCRECTANGLE:
						RectangleTemp.Draw(hdcMem);
						break;
					case ISCCIRCLE:
						CircleTemp.Draw(hdcMem);
						break;
					case ISCPLOY:
						PloyTemp.Draw(hdcMem);
						break;
					case ISCPENCIL:
						PencilTemp.Draw(hdcMem);
						break;
					}
				}
				break;

			}	
		}
		if (DrawStatus==SELECT&&b_ShapeSelected==TRUE)
		{
			AfxDrawSelectedRect(hdcMem,rectForSelected);
		}

		if (DrawStatus==DRAWPLOY&&ptrArrTemp.GetSize()>0)
		{
			AfxDrawPloy(hdcMem,g_PenStyle,g_PenWidth,g_color,ptrArrTemp);
		}
		for (int i=0;i<ptrArr.GetSize();i++)
		{
			if (ptrArr.GetAt(i)==NULL)
			{
				continue;
			}
			if (i==nIndexSelected&&b_IsScaling==TRUE)
			{
				continue;
			}

			((CShape*)ptrArr.GetAt(i))->Draw(hdcMem);
		}

		BitBlt(hdc,rcClient.left,rcClient.top,rcClient.right-rcClient.left,
			rcClient.bottom-rcClient.top,hdcMem,0,0,SRCCOPY);
		// 释放资源
		SelectObject(hdcMem,hOldObject);
		DeleteObject(hBitmap);
		DeleteDC(hdcMem);
	}
	EndPaint(m_hWnd, &ps);
}
VOID				CShapeWnd::OnClose(WPARAM wParam,LPARAM lParam)
{
	if(b_IsFromOtherProcess==FALSE&&MessageBox(m_hWnd,L"确定要关闭?",NULL,MB_OKCANCEL)==IDOK)
	{
		PostMessage(m_hWnd,WM_DESTROY,0,0);

	}
}
VOID				CShapeWnd::OnDestory(WPARAM wParam,LPARAM lParam)
{
	PostQuitMessage(0);
}
VOID				CShapeWnd::OnLine(WPARAM,LPARAM)
{
	b_ShapeSelected=FALSE;
	InvalidateRect(m_hWnd,NULL,TRUE);
	DrawStatus=DRAWLINE;
}
VOID				CShapeWnd::OnRect(WPARAM,LPARAM)
{
	DrawStatus=DRAWRECT;
	b_ShapeSelected=FALSE;
	InvalidateRect(m_hWnd,NULL,TRUE);
}
VOID				CShapeWnd::OnPloy(WPARAM,LPARAM)
{
	DrawStatus=DRAWPLOY;	
	b_ShapeSelected=FALSE;
	InvalidateRect(m_hWnd,NULL,TRUE);
}
VOID				CShapeWnd::OnPencil(WPARAM,LPARAM)
{
	DrawStatus=DRAWPENCIL;
	b_ShapeSelected=FALSE;
	InvalidateRect(m_hWnd,NULL,TRUE);
}
VOID				CShapeWnd::OnCircle(WPARAM,LPARAM)
{
	DrawStatus=DRAWCIRCLE;
	b_ShapeSelected=FALSE;
	InvalidateRect(m_hWnd,NULL,TRUE);
}
VOID				CShapeWnd::OnSave(WPARAM,LPARAM)
{
	if (wcscmp(szWindowTitle,L"MyProgram")!=0)//表示不匹配
	{
		shapefile.SaveToShapeFile(szWindowTitle,ptrArr);
		b_IsSaved=TRUE;
		SetWindowText(m_hWnd,szWindowTitle);
		StackForUndoRestore.Clear();
		AfxAddStringStar(szWindowTitle,szWindowTitleStar);
	}
	else
	{
		OnSaveAs(NULL,NULL);
	}
}
VOID				CShapeWnd::OnSaveAs(WPARAM,LPARAM)
{
	if (b_IsFromOtherProcess==TRUE)
	{
		return;
	}

	OPENFILENAME ofn;
	AfxInit_OPENFILENAME(ofn,m_hWnd,szWindowTitle,60,szFileFilter);
	//		ofn.lpstrFile=szInitFileName;
	ofn.lpstrDefExt=L"shape";
	if (GetSaveFileName(&ofn)==TRUE)
	{
		shapefile.SaveToShapeFile(szWindowTitle,ptrArr);
		b_IsSaved=TRUE;
		StackForUndoRestore.Clear();
		SetWindowText(m_hWnd,szWindowTitle);
		AfxAddStringStar(szWindowTitle,szWindowTitleStar);
	}

}
VOID				CShapeWnd::OnOpen(WPARAM,LPARAM)
{
	if (b_IsFromOtherProcess==TRUE)
	{
		return ;
	}
	TCHAR p[MAX_LOADSTRING];
	GetWindowText(m_hWnd,p,MAX_LOADSTRING);
	if (AfxLastCharIsStar(p)&&MessageBox(m_hWnd,L"当前文件未保存,是否仍要打开新文件?",L"警告",MB_YESNO)==IDNO)
	{
		return ;
	}

	InvalidateRect(m_hWnd,NULL,TRUE);

	//打开文件
	OPENFILENAME ofn;
	TCHAR temp[TCHAR_BUF_SIZE];
	memcpy(temp,szWindowTitle,sizeof(TCHAR)*TCHAR_BUF_SIZE);
	szWindowTitle[0]=L'\0';

	AfxInit_OPENFILENAME(ofn,m_hWnd,szWindowTitle,60,szFileFilter);
	if (GetOpenFileName(&ofn)==TRUE)//为选择打开的文件
	{
		StackForUndoRestore.Clear();
		ptrArr.Clear();	
	}
	else
	{
		memcpy(szWindowTitle,temp,sizeof(TCHAR)*TCHAR_BUF_SIZE);
		return ;
	}			
	if (shapefile.DeSerialize(szWindowTitle,ptrArr)==FALSE)
	{
		MessageBox(m_hWnd,L"打开出错",L"ERROR",MB_OK);
	}
	else
	{
		SetWindowText(m_hWnd,szWindowTitle);
		AfxAddStringStar(szWindowTitle,szWindowTitleStar);
		ForSendMsg.GetInfo(m_hWnd,WM_FILEOPEN,2*(wcslen(szWindowTitle)+1),(LPARAM)szWindowTitle);
		EnumWindows(lpEnumFunc,(LPARAM)&ForSendMsg);
		b_IsSaved=TRUE;
	}
	InvalidateRect(m_hWnd,NULL,TRUE);

}
VOID				CShapeWnd::OnSelect(WPARAM,LPARAM)
{
	DrawStatus=SELECT;
}
VOID				CShapeWnd::OnCut(WPARAM,LPARAM)
{
	if (!b_ShapeSelected)
	{
		if (b_IsFromOtherProcess==FALSE)
		{
			MessageBox(m_hWnd,L"没有选中的图形",L"ERROR",MB_OK);
		}

		return ;
	}
	nWhichTypeCanPaste=((CShape*)ptrArr.GetAt(nIndexSelected))->What_Am_I();
	switch (nWhichTypeCanPaste)
	{
	case ISCLINE:
		LineForCutOrCopy=*((CLine*)ptrArr.GetAt(nIndexSelected));
		break;
	case ISCRECTANGLE:
		RectangleForCutOrCopy=*((CRectangle*)ptrArr.GetAt(nIndexSelected));
		break;
	case ISCCIRCLE:
		CircleForCutOrCopy=*((CCircle*)ptrArr.GetAt(nIndexSelected));
		break;
	case ISCPLOY:
		PloyForCutOrCopy=*((CPloy*)ptrArr.GetAt(nIndexSelected));
		break;
	case ISCPENCIL:
		PencilForCutOrCopy=*((CPencil*)ptrArr.GetAt(nIndexSelected));
		break;
	}

	{
		g_lpUndo=new UNDO_RESTORE_INFO;
		g_lpUndo->Index=nIndexSelected;
		g_lpUndo->pSh=(CShape*)ptrArr.GetAt(nIndexSelected);
		g_lpUndo->nWhichChange=SHAPE_CUT;
		StackForUndoRestore.Push(g_lpUndo);
	}
	nLastCutOrCopyIndex=nIndexSelected;
	nIsCutOrCopy=ISCUT;
	b_ShapeSelected=FALSE;
	ptrArr.DeleteAt(nIndexSelected);
	nIndexSelected=-1;
	InvalidateRect(m_hWnd,NULL,TRUE);
	b_IsSaved=FALSE;
	SetWindowText(m_hWnd,szWindowTitleStar);
}
VOID				CShapeWnd::OnCopy(WPARAM,LPARAM)
{
	if (!b_ShapeSelected)
	{
		if (b_IsFromOtherProcess==FALSE)
		{
			MessageBox(m_hWnd,L"没有选中的图形",L"ERROR",MB_OK);
		}

		return ;
	}
	nWhichTypeCanPaste=((CShape*)ptrArr.GetAt(nIndexSelected))->What_Am_I();
	switch (nWhichTypeCanPaste)
	{
	case ISCLINE:
		LineForCutOrCopy=*((CLine*)ptrArr.GetAt(nIndexSelected));
		break;
	case ISCRECTANGLE:
		RectangleForCutOrCopy=*((CRectangle*)ptrArr.GetAt(nIndexSelected));
		break;
	case ISCCIRCLE:
		CircleForCutOrCopy=*((CCircle*)ptrArr.GetAt(nIndexSelected));
		break;
	case ISCPLOY:
		PloyForCutOrCopy=*((CPloy*)ptrArr.GetAt(nIndexSelected));
		break;
	case ISCPENCIL:
		PencilForCutOrCopy=*((CPencil*)ptrArr.GetAt(nIndexSelected));
		break;
	}
	nLastCutOrCopyIndex=nIndexSelected;
	nIsCutOrCopy=ISCOPY;

}
VOID				CShapeWnd::OnPaste(WPARAM,LPARAM)
{
	if (nIsCutOrCopy==NO_CUT_OR_COPY)
	{
		if (b_IsFromOtherProcess==FALSE)
		{
			MessageBox(m_hWnd,L"剪贴板里无数据",L"ERROR",NULL);
		}

		return ;
	}
	switch (nWhichTypeCanPaste)
	{
	case ISCLINE:
		psh=new CLine(LineForCutOrCopy);
		ptrArr.Add(psh);
		break;
	case ISCRECTANGLE:
		psh=new CRectangle(RectangleForCutOrCopy);
		ptrArr.Add(psh);
		break;
	case ISCCIRCLE:
		psh=new CCircle(CircleForCutOrCopy);
		ptrArr.Add(psh);
		break;
	case ISCPLOY:
		psh=new CPloy(PloyForCutOrCopy);
		ptrArr.Add(psh);
		break;
	case ISCPENCIL:
		psh=new CPencil(PencilForCutOrCopy);
		ptrArr.Add(psh);
		break;
	}
	if (nIsCutOrCopy==ISCOPY)
	{
		psh->Move(20,20);
	}
	//用于撤销
	{
		g_lpUndo=new UNDO_RESTORE_INFO;
		g_lpUndo->nWhichChange=SHAPE_PASTE;
		g_lpUndo->Index=ptrArr.GetSize()-1;
		g_lpUndo->pSh=psh;
		StackForUndoRestore.Push(g_lpUndo);

	}
	nIndexSelected=ptrArr.GetSize()-1;
	rectForSelected=((CShape*)ptrArr.GetAt(nIndexSelected))->GetItsRect();
	b_ShapeSelected=TRUE;
	DrawStatus=SELECT;
	InvalidateRect(m_hWnd,NULL,TRUE);

	if (nIsCutOrCopy==ISCUT)
	{
		nIsCutOrCopy=NO_CUT_OR_COPY;
		nWhichTypeCanPaste=-1;
	}
	b_IsSaved=FALSE;
	SetWindowText(m_hWnd,szWindowTitleStar);
}
VOID				CShapeWnd::OnDelete(WPARAM,LPARAM)
{
	if (!b_ShapeSelected)
	{
		if (b_IsFromOtherProcess==FALSE)
		{
			MessageBox(m_hWnd,L"没有选中的图形",L"ERROR",MB_OK);
		}
		DrawStatus=SELECT;
		return ;
	}
	b_ShapeSelected=FALSE;
	{
		g_lpUndo=new UNDO_RESTORE_INFO;
		g_lpUndo->Index=nIndexSelected;
		g_lpUndo->pSh=(CShape*)ptrArr.GetAt(nIndexSelected);
		g_lpUndo->nWhichChange=SHAPE_DELETE;
		StackForUndoRestore.Push(g_lpUndo);

	}
	ptrArr.DeleteAt(nIndexSelected);
	nIndexSelected=-1;
	InvalidateRect(m_hWnd,NULL,TRUE);
	b_IsSaved=FALSE;
	SetWindowText(m_hWnd,szWindowTitleStar);
}
VOID				CShapeWnd::OnUndo(WPARAM,LPARAM)
{
	LPUNDO_RESTORE_INFO lp=StackForUndoRestore.Get();
	if (lp==NULL)
	{
		if (b_IsFromOtherProcess==FALSE)
		{
			MessageBox(m_hWnd,L"无法撤销",L"ERROR",MB_OK);
		}

		return ;
	}
	switch (lp->nWhichChange)
	{
	case SHAPE_CREATE:
		StackForUndoRestore.Pop();
		ptrArr.DeleteAt(lp->Index);	
		b_ShapeSelected=FALSE;
		DrawStatus=SELECT;
		break;
	case SHAPE_MOVE:
		StackForUndoRestore.Pop();
		((CShape*)ptrArr.GetAt(lp->Index))->Move(-lp->nFirParam,-lp->nSecParam);
		rectForSelected=((CShape*)ptrArr.GetAt(lp->Index))->GetItsRect();
		DrawStatus=SELECT;
		break;
	case SHAPE_DELETE:
		StackForUndoRestore.Pop();
		ptrArr.SetAt(lp->Index,lp->pSh);
		rectForSelected=lp->pSh->GetItsRect();
		nIndexSelected=lp->Index;
		b_ShapeSelected=TRUE;
		DrawStatus=SELECT;
		break;
	case SHAPE_CUT:
		StackForUndoRestore.Pop();
		ptrArr.SetAt(lp->Index,lp->pSh);
		rectForSelected=lp->pSh->GetItsRect();
		nIndexSelected=lp->Index;
		b_ShapeSelected=TRUE;
		DrawStatus=SELECT;
		break;
	case SHAPE_PASTE:
		StackForUndoRestore.Pop();
		ptrArr.DeleteAt(lp->Index);
		b_ShapeSelected=FALSE;
		nIndexSelected=-1;
		break;
	case SHAPE_SCALING:
		StackForUndoRestore.Pop();
		((CShape*)ptrArr.GetAt(lp->Index))->Scaling(lp->Bp,1/lp->fFirParam,1/lp->fSecParam);
		b_ShapeSelected=TRUE;
		nIndexSelected=lp->Index;
		rectForSelected=lp->pSh->GetItsRect();
		DrawStatus=SELECT;
		break;
	}
	InvalidateRect(m_hWnd,NULL,TRUE);
}
VOID				CShapeWnd::OnRestore(WPARAM,LPARAM)
{
	LPUNDO_RESTORE_INFO lp=StackForUndoRestore.Restore();
	if (lp==NULL)
	{
		if (b_IsFromOtherProcess==FALSE)
		{
			MessageBox(m_hWnd,L"无法还原",L"ERROR",MB_OK);
		}

		return ;
	}
	switch (lp->nWhichChange)
	{
	case SHAPE_CREATE:
		ptrArr.SetAt(lp->Index,lp->pSh);
		nIndexSelected=lp->Index;
		b_ShapeSelected=TRUE;
		DrawStatus=SELECT;
		rectForSelected=lp->pSh->GetItsRect();
		break;
	case SHAPE_MOVE:
		((CShape*)ptrArr.GetAt(lp->Index))->Move(lp->nFirParam,lp->nSecParam);
		rectForSelected=((CShape*)ptrArr.GetAt(lp->Index))->GetItsRect();
		DrawStatus=SELECT;
		break;
	case SHAPE_DELETE:
		ptrArr.DeleteAt(lp->Index);
		nIndexSelected=-1;
		b_ShapeSelected=FALSE;
		DrawStatus=SELECT;
		break; 
	case SHAPE_CUT:
		nIsCutOrCopy=ISCUT;
		b_ShapeSelected=FALSE;
		ptrArr.DeleteAt(lp->Index);
		nIndexSelected=-1;
		DrawStatus=SELECT;
		break;
	case SHAPE_PASTE:
		ptrArr.SetAt(lp->Index,psh);
		b_ShapeSelected=TRUE;
		nIndexSelected=lp->Index;
		rectForSelected=lp->pSh->GetItsRect();
		break;
	case SHAPE_SCALING:
		((CShape*)ptrArr.GetAt(lp->Index))->Scaling(lp->Bp,lp->fFirParam,lp->fSecParam);
		b_ShapeSelected=TRUE;
		nIndexSelected=lp->Index;
		rectForSelected=lp->pSh->GetItsRect();
		DrawStatus=SELECT;
		break;
	}
	InvalidateRect(m_hWnd,NULL,TRUE);
}
VOID				CShapeWnd::OnChangePenWidth(WPARAM,LPARAM)
{
	memset(szPenWidth,0,15);
	DialogBox(m_hInst, MAKEINTRESOURCE(IDD_PENWIDTH), m_hWnd, ChangePenWidth); 
	ForSendMsg.GetInfo(m_hWnd,WM_PENWIDTH,g_PenWidth,0);
	EnumWindows(lpEnumFunc,(LPARAM)&ForSendMsg);
}
VOID				CShapeWnd::OnChangeColor(WPARAM wParam,LPARAM)
{
	if (b_IsFromOtherProcess==TRUE)
	{
		g_color=wParam;
		return ;
	}
	cc.Flags   =   CC_RGBINIT   |   CC_FULLOPEN   |   CC_ANYCOLOR;   
	cc.hwndOwner   =  m_hWnd;   
	cc.rgbResult   =RGB(0,0,0);
	cc.lpCustColors   =   g_rgbCustom; 
	if (ChooseColor(&cc)==FALSE)
	{
		return ;
	}
	if (cc.rgbResult==RGB(255,255,255))
	{
		MessageBox(m_hWnd,L"无法选择白色作为画笔颜色",L"ERROR",MB_OK);
		return ;
	}
	else
	{
		g_color=cc.rgbResult;
		ForSendMsg.GetInfo(m_hWnd,WM_CHANGECOLOR,g_color,0);
		EnumWindows(lpEnumFunc,(LPARAM)&ForSendMsg);
	}
}
VOID				CShapeWnd::OnAbout(WPARAM,LPARAM)
{
	DialogBox(m_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), m_hWnd, About);
}
VOID				CShapeWnd::OnCreate(WPARAM wParam,LPARAM lParam)
{

	EnumWindows(lpEnumFuncForCreate,(LPARAM)hwndTemp);
	
}

VOID				CShapeWnd::OnInitCreate(WPARAM wParam,LPARAM lParam)
{
	CShapeFile savetemp;
	savetemp.SaveToShapeFile(L"D:\\a.tmp",this->ptrArr);
	LONG l=0+g_PenWidth+100*DrawStatus;
	PostMessage((HWND)lParam,WM_INITFINISH,this->g_color,l);
}
VOID				CShapeWnd::OnInitCreateFinish(WPARAM wParam,LPARAM lParam)
{
	this->ptrArr.Clear();
	LONG l=(LONG)lParam;
	this->DrawStatus=l/100;
	this->g_PenWidth=l%100;
	this->g_color=(COLORREF)wParam;
	CShapeFile filetemp;
	filetemp.DeSerialize(L"D:\\a.tmp",this->ptrArr);
	InvalidateRect(m_hWnd,NULL,TRUE);
	DeleteFile(L"D:\\a.tmp");
	
	
}