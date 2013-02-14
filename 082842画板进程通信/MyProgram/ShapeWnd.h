#include "windows.h"
#include "PtrArr.h"//����ָ�붯̬����
#include "ShapeObject.h"//����
#include "Line.h"//����ͼ����
#include "Rectangle.h"
#include "Pencil.h"
#include "Circle.h"
#include "Ploy.h"
#include "commctrl.h"//���ڹ�����
#include "commdlg.h"//���ڵ�ɫ��ȶԻ���
#include "ShapeFile.h"//��װ���ļ�����Լ�һЩ����
#include "string.h"//�û����ַ��ıȽϵ�
#include "StackForUndoRestore.h"//������ԭջ
#include "FORSENDMSG.h"//���ڽ��̼�ͨ��
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
struct MSGMAP //��Ϣ·��
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
	static BOOL CALLBACK lpEnumFunc(HWND hwnd, LPARAM lParam);//���ڽ��̼�ͨ��
	static BOOL CALLBACK lpEnumFuncForCreate(HWND hwnd, LPARAM lParam);//���ڽ��̼�ͨ��
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
	//�˵���
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
	CPtrArr			ptrArr;									//ģ��MFC�е���
	CPtrArr			ptrArrTemp;							//�����ڻ�������ͼ��Ͷ����ʱ������ʱ����
	BOOL			b_beginDrawPloy;					//�Ƿ��ǻ�����εĵ�һ����
	POINT			*pPt;										//���ڱ����ڻ�������ͼ��Ͷ����ʱ����Щ��
	POINT			m_pt;										//�������������µ�����
	POINT			m_mousePos;							//����ƶ�ʱ������
	POINT			m_mouseLastPos;					//����ϴ��ƶ�ʱ������,��Ҫ���������קͼ�ε�ʱ��
	POINT			m_LbtnUpPos;						//����������ʱ������
	POINT			m_DbclkMousePos;				//���˫��ʱ�������
	BOOL			m_bLBtnIsDown;						//�������Ƿ���
	RECT				rectWindow;							//���ڵľ���
	RECT				rectForSelected;						//ѡ��ͼ��ʱ��ľ���
	BOOL			b_ShapeSelected;					//�Ƿ�ѡ����ͼ��
	BOOL			b_IsMove;								//�Ƿ�ͼ�������ƶ�
	BOOL			b_IsScaling;							//�Ƿ�ͼ�����ڷ���
	FLOAT			g_xScale;								//���ڱ������ʱ��X����ķ�������
	FLOAT			g_yScale;								//���ڱ������ʱ��Y����ķ�������
	BOOL			b_IsSaved;								//�Ƿ�ͼ���Ѿ�������
	INT				nIndexSelected;						//ѡ���������ĸ�ͼ��
	BasePoint		g_EnumBasePt;						//�������ʱ��׼��(����������������)
	POINT			g_BasePt;								//�������ʱ��׼��
	INT				ShapeType;							//�����������ʱ��ͼ�ε�����
	CShape			*psh;										//����new��ͼ��,Ȼ����ӵ�ָ��������ȥ
	TCHAR			buf[MAX_LOADSTRING];			//���ڸ��ֵ����
	RECT			rect;											//����ͼ�εľ������� 
	RECT			rectMax;									//�ػ������������
	RECT			rectMaxForPenWidth;
	COLORREF		g_color;
	INT				g_PenStyle;							//�ʵ�����
	static INT		g_PenWidth;							//�ʿ�
	CShapeFile		shapefile;								//�������Ǹ��ļ��ľ��
	INT				DrawStatus;							//0��ʾ��ֱ��,1��ʾ������,2��ʾ�������,3��ʾ������ͼ��
	//��׺ΪTemp��ͼ��Ϊ��ʱ�������ʱ��ͼ��
	CLine			LineTemp,LineForCutOrCopy;
	CRectangle		RectangleTemp,RectangleForCutOrCopy;
	CCircle			CircleTemp,CircleForCutOrCopy;
	CPencil			PencilTemp,PencilForCutOrCopy;
	CPloy			PloyTemp,PloyForCutOrCopy;

	INT				nIsCutOrCopy;						//������������Ƿ��ж����Լ�����,0Ϊ�޶���
	INT				nWhichTypeCanPaste;				//�������е���ʲôͼ��
	INT				nLastCutOrCopyIndex;			//���ڱ����ϴμ��л��Ƶ��Ǹ�ͼ�ε�����

	CStackForUndoRestore StackForUndoRestore;
	LPUNDO_RESTORE_INFO g_lpUndo;


	CHOOSECOLOR	cc;										//����ѡ����ɫ
	COLORREF		g_rgbCustom[16];					//����ѡ����ɫ
	TCHAR			szPenWidth[15];
	HCURSOR			hCursorCross;
	BOOL			b_IsFromOtherProcess;
	FORSENDMSG		ForSendMsg;
	UINT			MsgCanPass[6];							//�ܽ��н��̼�ͨ�ŵķ�COMMAND��Ϣ
	UINT			CMDMsgCanPass[12];				//�ܽ��н��̼�ͨ�ŵĲ˵�����Ϣ
	MSGMAP			 msgmap[MSG_NUM];		//��WM_COMMAND��Ϣ·��
	MSGMAP			cmdmsgmap[CMD_MSG_NUM];//WM_COMMAND��Ϣ·��

};