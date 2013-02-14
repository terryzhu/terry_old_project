#pragma once

// 全局变量:
HINSTANCE hInst;											// 当前实例
TCHAR				szFileFilter[]=L"图形文件(*.shape)\0*.shape\0\0";
static CPtrArr		ptrArr;									//模仿MFC中的类
static CPtrArr		ptrArrTemp;								//用于在画不规则图像和多边形时储存临时对象
static BOOL			b_beginDrawPloy=TRUE;					//是否是画多边形的第一个点
static POINT		*pPt;									//用于保存在画不规则图像和多边形时的那些点
static POINT		m_pt;									//保存鼠标左键按下的坐标
static POINT		m_mousePos;								//鼠标移动时的坐标
static POINT		m_mouseLastPos;							//鼠标上次移动时的坐标,主要用于鼠标拖拽图形的时候
static POINT		m_LbtnUpPos;							//鼠标左键弹起时的坐标
static POINT		m_DbclkMousePos;						//鼠标双击时候的坐标
static RECT			rectWindow;								//窗口的矩形
static RECT			rectForSelected;						//选中图形时候的矩形
static BOOL			b_ShapeSelected;						//是否选中了图形
static BOOL			b_IsMove;								//是否图形正在移动
static BOOL			b_IsScaling;							//是否图形正在放缩
static FLOAT		g_xScale;								//用于保存放缩时候X坐标的放缩比例
static FLOAT		g_yScale;								//用于保存放缩时候Y坐标的放缩比例
static BOOL			b_IsSaved;								//是否图形已经保存了
static INT			nIndexSelected;							//选中数组中哪个图形
static BasePoint	g_EnumBasePt;							//保存放缩时基准点(左上左下右上右下)
static POINT		g_BasePt;								//保存放缩时基准点
int					ShapeType=0;							//保存的是缩放时候图形的类型
CShape				*psh;									//保存new的图形,然后添加到指针数组中去
TCHAR				buf[100];								//用于各种的输出
RECT				rect;									//画的图形的矩形区域 
RECT				rectMax;								//重绘区域的最大矩形
RECT				rectMaxForPenWidth;
COLORREF			g_color=RGB(255,0,0);
int					g_PenStyle=PS_SOLID;					//笔的类型
int					g_PenWidth=10;							//笔宽
CShapeFile			shapefile;								//操作的那个文件的句柄
int					DrawStatus=0;							//0表示画直线,1表示画矩形,2表示画多边形,3表示画随意图形
//后缀为Temp的图形为临时保存放缩时的图形
CLine				LineTemp,LineForCutOrCopy;
CRectangle			RectangleTemp,RectangleForCutOrCopy;
CCircle				CircleTemp,CircleForCutOrCopy;
CPencil				PencilTemp,PencilForCutOrCopy;
CPloy				PloyTemp,PloyForCutOrCopy;

INT					nIsCutOrCopy;							//保存剪贴板中是否有东西以及内容,0为无东西
INT					nWhichTypeCanPaste;						//剪贴板中的是什么图形
INT					nLastCutOrCopyIndex;					//用于保存上次剪切或复制的那个图形的索引

CStackForUndoRestore StackForUndoRestore;
LPUNDO_RESTORE_INFO g_lpUndo;


CHOOSECOLOR			cc={sizeof(CHOOSECOLOR)};				//用于选择颜色
COLORREF			g_rgbCustom[16]={0};					//用于选择颜色
TCHAR				szPenWidth[15]={};
HCURSOR hCursorCross;
BOOL b_IsFromOtherProcess=FALSE;
//剪贴板状态
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

#define TCHAR_BUF_SIZE 100

HWND hTool;//工具栏句柄
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

//能进行进程间通信的非COMMAND消息
UINT MsgCanPass[6]={WM_LBUTTONDOWN,WM_LBUTTONUP,WM_RBUTTONDOWN,WM_LBUTTONDBLCLK,WM_MOUSEMOVE,WM_DESTROY};
//能进行进程间通信的菜单类消息
UINT CMDMsgCanPass[12]={IDM_SELECT,IDM_CUT,IDM_COPY,IDM_PASTE,IDM_UNDO,IDM_RESTORE,
IDM_DELETE,IDM_LINE,IDM_CIRCLE,IDM_RECT,IDM_PENCIL,IDM_PLOY};
//全局绘图状态


//用于进程间通讯的结构体
typedef  struct FORSENDMSG 
{
	LPARAM lParam;
	WPARAM wParam;
	UINT msg;
	HWND hWnd;
	void GetInfo(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp)
	{
		this->hWnd=hwnd;
		this->msg=msg;
		this->wParam=wp;
		this->lParam=lp;
	}
}*LPFORSENDMSG;
FORSENDMSG ForSendMsg;

