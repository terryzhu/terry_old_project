#include "windows.h"
#include "PtrArr.h"//用于指针动态数组
#include "ShapeObject.h"//基类
#include "Line.h"//用于图形类
#include "Rectangle.h"
#include "Pencil.h"
#include "Circle.h"
#include "Ploy.h"
#include "commctrl.h"//用于工具栏
#include "commdlg.h"//用于调色板等对话框
#include "ShapeFile.h"//封装了文件句柄以及一些操作
#include "string.h"//用户宽字符的比较等
#include "StackForUndoRestore.h"//撤销还原栈
#include "FORSENDMSG.h"//用于进程间通信
#define				MAX_LOADSTRING 100
#define				NUM_BUTTONS 18
#define				NO_CUT_OR_COPY 0
#define				ISCUT 1
#define				ISCOPY 2

#define				DRAWLINE 0
#define				DRAWRECT 1
#define				DRAWPLOY 2
#define				DRAWPENCIL 3
#define				DRAWCIRCLE 5
#define				SELECT 4
//#define				SCALING 6

#define				WM_PENWIDTH (WM_USER+20)
#define				WM_CHANGECOLOR (WM_USER+30)
#define				WM_FILEOPEN (WM_USER+40)
#define				WM_INITCREATE (WM_USER+50)
#define				WM_INITFINISH (WM_USER+60)
#define MSG_NUM 17
#define CMD_MSG_NUM 18
#define TCHAR_BUF_SIZE 100
class CShapeWnd;
struct MSGMAP //消息路由
{
	UINT message;
	void (CShapeWnd::*pFn)(WPARAM,LPARAM);
};


class CShapeWnd :CShapeObject
{
public:
	CShapeWnd();
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL ShapeWndReg(HINSTANCE hInst);
	BOOL CreatShapeWindow(HINSTANCE hInst);
	static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK ChangePenWidth(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK lpEnumFunc(HWND hwnd, LPARAM lParam);//用于进程间通信
	static BOOL CALLBACK lpEnumFuncForCreate(HWND hwnd, LPARAM lParam);//用于进程间通信
	//message
	VOID OnCreateToolBar();
	VOID OnCreate(WPARAM wParam,LPARAM lParam);
	VOID OnColor(WPARAM wParam,LPARAM lParam);
	VOID OnPenWidth(WPARAM wParam,LPARAM lParam);
	VOID OnCopyData(WPARAM wParam,LPARAM lParam);
	VOID OnEraseBkGnd(WPARAM wParam,LPARAM lParam);
	VOID OnSize(WPARAM wParam,LPARAM lParam);
	VOID OnMove(WPARAM wParam,LPARAM lParam);
	VOID OnLButtonDbclk(WPARAM wParam,LPARAM lParam);
	VOID OnLButtonDown(WPARAM wParam,LPARAM lParam);
	VOID OnMouseMove(WPARAM wParam,LPARAM lParam);
	VOID OnLButtonUp(WPARAM wParam,LPARAM lParam);
	VOID OnRButtonDown(WPARAM wParam,LPARAM lParam);
	VOID OnPaint(WPARAM wParam,LPARAM lParam);
	VOID OnClose(WPARAM wParam,LPARAM lParam);
	VOID OnDestory(WPARAM wParam,LPARAM lParam);
	VOID OnInitCreate(WPARAM wParam,LPARAM lParam);
	VOID OnInitCreateFinish(WPARAM wParam,LPARAM lParam);
	//菜单项
	VOID OnLine(WPARAM,LPARAM);
	VOID OnRect(WPARAM,LPARAM);
	VOID OnPloy(WPARAM,LPARAM);
	VOID OnPencil(WPARAM,LPARAM);
	VOID OnCircle(WPARAM,LPARAM);

	VOID OnSave(WPARAM,LPARAM);
	VOID OnSaveAs(WPARAM,LPARAM);
	VOID OnOpen(WPARAM,LPARAM);

	VOID OnSelect(WPARAM,LPARAM);
	VOID OnCut(WPARAM,LPARAM);
	VOID OnCopy(WPARAM,LPARAM);
	VOID OnPaste(WPARAM,LPARAM);
	VOID OnDelete(WPARAM,LPARAM);
	VOID OnUndo(WPARAM,LPARAM);
	VOID OnRestore(WPARAM,LPARAM);

	VOID OnChangePenWidth(WPARAM,LPARAM);
	VOID OnChangeColor(WPARAM,LPARAM);
	VOID OnAbout(WPARAM,LPARAM);
private:
	HWND m_hWnd;
	HINSTANCE m_hInst;
	HWND hTool;
	static HWND hwndTemp;
	static TCHAR	szWindowClass[MAX_LOADSTRING];
	TCHAR			szWindowTitle[MAX_LOADSTRING];
	TCHAR			szWindowTitleStar[MAX_LOADSTRING];
	TCHAR			szFileFilter[MAX_LOADSTRING];
	CPtrArr			ptrArr;									//模仿MFC中的类
	CPtrArr			ptrArrTemp;							//用于在画不规则图像和多边形时储存临时对象
	BOOL			b_beginDrawPloy;					//是否是画多边形的第一个点
	POINT			*pPt;										//用于保存在画不规则图像和多边形时的那些点
	POINT			m_pt;										//保存鼠标左键按下的坐标
	POINT			m_mousePos;							//鼠标移动时的坐标
	POINT			m_mouseLastPos;					//鼠标上次移动时的坐标,主要用于鼠标拖拽图形的时候
	POINT			m_LbtnUpPos;						//鼠标左键弹起时的坐标
	POINT			m_DbclkMousePos;				//鼠标双击时候的坐标
	BOOL			m_bLBtnIsDown;						//鼠标左键是否按下
	RECT				rectWindow;							//窗口的矩形
	RECT				rectForSelected;						//选中图形时候的矩形
	BOOL			b_ShapeSelected;					//是否选中了图形
	BOOL			b_IsMove;								//是否图形正在移动
	BOOL			b_IsScaling;							//是否图形正在放缩
	FLOAT			g_xScale;								//用于保存放缩时候X坐标的放缩比例
	FLOAT			g_yScale;								//用于保存放缩时候Y坐标的放缩比例
	BOOL			b_IsSaved;								//是否图形已经保存了
	INT				nIndexSelected;						//选中数组中哪个图形
	BasePoint		g_EnumBasePt;						//保存放缩时基准点(左上左下右上右下)
	POINT			g_BasePt;								//保存放缩时基准点
	INT				ShapeType;							//保存的是缩放时候图形的类型
	CShape			*psh;										//保存new的图形,然后添加到指针数组中去
	TCHAR			buf[MAX_LOADSTRING];			//用于各种的输出
	RECT			rect;											//画的图形的矩形区域 
	RECT			rectMax;									//重绘区域的最大矩形
	RECT			rectMaxForPenWidth;
	COLORREF		g_color;
	INT				g_PenStyle;							//笔的类型
	static INT		g_PenWidth;							//笔宽
	CShapeFile		shapefile;								//操作的那个文件的句柄
	INT				DrawStatus;							//0表示画直线,1表示画矩形,2表示画多边形,3表示画随意图形
	//后缀为Temp的图形为临时保存放缩时的图形
	CLine			LineTemp,LineForCutOrCopy;
	CRectangle		RectangleTemp,RectangleForCutOrCopy;
	CCircle			CircleTemp,CircleForCutOrCopy;
	CPencil			PencilTemp,PencilForCutOrCopy;
	CPloy			PloyTemp,PloyForCutOrCopy;

	INT				nIsCutOrCopy;						//保存剪贴板中是否有东西以及内容,0为无东西
	INT				nWhichTypeCanPaste;				//剪贴板中的是什么图形
	INT				nLastCutOrCopyIndex;			//用于保存上次剪切或复制的那个图形的索引

	CStackForUndoRestore StackForUndoRestore;
	LPUNDO_RESTORE_INFO g_lpUndo;


	CHOOSECOLOR	cc;										//用于选择颜色
	COLORREF		g_rgbCustom[16];					//用于选择颜色
	TCHAR			szPenWidth[15];
	HCURSOR			hCursorCross;
	BOOL			b_IsFromOtherProcess;
	FORSENDMSG		ForSendMsg;
	UINT			MsgCanPass[6];							//能进行进程间通信的非COMMAND消息
	UINT			CMDMsgCanPass[12];				//能进行进程间通信的菜单类消息
	MSGMAP			 msgmap[MSG_NUM];		//非WM_COMMAND消息路由
	MSGMAP			cmdmsgmap[CMD_MSG_NUM];//WM_COMMAND消息路由

};