#pragma once

// ȫ�ֱ���:
HINSTANCE hInst;											// ��ǰʵ��
TCHAR				szFileFilter[]=L"ͼ���ļ�(*.shape)\0*.shape\0\0";
static CPtrArr		ptrArr;									//ģ��MFC�е���
static CPtrArr		ptrArrTemp;								//�����ڻ�������ͼ��Ͷ����ʱ������ʱ����
static BOOL			b_beginDrawPloy=TRUE;					//�Ƿ��ǻ�����εĵ�һ����
static POINT		*pPt;									//���ڱ����ڻ�������ͼ��Ͷ����ʱ����Щ��
static POINT		m_pt;									//�������������µ�����
static POINT		m_mousePos;								//����ƶ�ʱ������
static POINT		m_mouseLastPos;							//����ϴ��ƶ�ʱ������,��Ҫ���������קͼ�ε�ʱ��
static POINT		m_LbtnUpPos;							//����������ʱ������
static POINT		m_DbclkMousePos;						//���˫��ʱ�������
static RECT			rectWindow;								//���ڵľ���
static RECT			rectForSelected;						//ѡ��ͼ��ʱ��ľ���
static BOOL			b_ShapeSelected;						//�Ƿ�ѡ����ͼ��
static BOOL			b_IsMove;								//�Ƿ�ͼ�������ƶ�
static BOOL			b_IsScaling;							//�Ƿ�ͼ�����ڷ���
static FLOAT		g_xScale;								//���ڱ������ʱ��X����ķ�������
static FLOAT		g_yScale;								//���ڱ������ʱ��Y����ķ�������
static BOOL			b_IsSaved;								//�Ƿ�ͼ���Ѿ�������
static INT			nIndexSelected;							//ѡ���������ĸ�ͼ��
static BasePoint	g_EnumBasePt;							//�������ʱ��׼��(����������������)
static POINT		g_BasePt;								//�������ʱ��׼��
int					ShapeType=0;							//�����������ʱ��ͼ�ε�����
CShape				*psh;									//����new��ͼ��,Ȼ����ӵ�ָ��������ȥ
TCHAR				buf[100];								//���ڸ��ֵ����
RECT				rect;									//����ͼ�εľ������� 
RECT				rectMax;								//�ػ������������
RECT				rectMaxForPenWidth;
COLORREF			g_color=RGB(255,0,0);
int					g_PenStyle=PS_SOLID;					//�ʵ�����
int					g_PenWidth=10;							//�ʿ�
CShapeFile			shapefile;								//�������Ǹ��ļ��ľ��
int					DrawStatus=0;							//0��ʾ��ֱ��,1��ʾ������,2��ʾ�������,3��ʾ������ͼ��
//��׺ΪTemp��ͼ��Ϊ��ʱ�������ʱ��ͼ��
CLine				LineTemp,LineForCutOrCopy;
CRectangle			RectangleTemp,RectangleForCutOrCopy;
CCircle				CircleTemp,CircleForCutOrCopy;
CPencil				PencilTemp,PencilForCutOrCopy;
CPloy				PloyTemp,PloyForCutOrCopy;

INT					nIsCutOrCopy;							//������������Ƿ��ж����Լ�����,0Ϊ�޶���
INT					nWhichTypeCanPaste;						//�������е���ʲôͼ��
INT					nLastCutOrCopyIndex;					//���ڱ����ϴμ��л��Ƶ��Ǹ�ͼ�ε�����

CStackForUndoRestore StackForUndoRestore;
LPUNDO_RESTORE_INFO g_lpUndo;


CHOOSECOLOR			cc={sizeof(CHOOSECOLOR)};				//����ѡ����ɫ
COLORREF			g_rgbCustom[16]={0};					//����ѡ����ɫ
TCHAR				szPenWidth[15]={};
HCURSOR hCursorCross;
BOOL b_IsFromOtherProcess=FALSE;
//������״̬
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

HWND hTool;//���������
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

//�ܽ��н��̼�ͨ�ŵķ�COMMAND��Ϣ
UINT MsgCanPass[6]={WM_LBUTTONDOWN,WM_LBUTTONUP,WM_RBUTTONDOWN,WM_LBUTTONDBLCLK,WM_MOUSEMOVE,WM_DESTROY};
//�ܽ��н��̼�ͨ�ŵĲ˵�����Ϣ
UINT CMDMsgCanPass[12]={IDM_SELECT,IDM_CUT,IDM_COPY,IDM_PASTE,IDM_UNDO,IDM_RESTORE,
IDM_DELETE,IDM_LINE,IDM_CIRCLE,IDM_RECT,IDM_PENCIL,IDM_PLOY};
//ȫ�ֻ�ͼ״̬


//���ڽ��̼�ͨѶ�Ľṹ��
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

