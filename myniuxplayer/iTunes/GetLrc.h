#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CGetLrc �Ի���
struct DOWNLOADLRC 
{
	CString m_strSingerName;
	CString m_strSongName;
	CString MusicLrcPath;
	int songCode;
};
class CGetLrc : public CDialog
{
	DECLARE_DYNAMIC(CGetLrc)

public:
	CGetLrc(CString songName=NULL,CString singerName=NULL,CString lrcpath=NULL,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGetLrc();

// �Ի�������
	enum { IDD = IDD_GETLRC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_singer;
	CString m_song;
	CListCtrl m_listctl;
	CListBox m_list;
	CString lrcpath;
	static int CreateCode(TCHAR * buf,int lrcId);
	static void ConvertStrToHex(CString strIn, CString & strOut);
	static BOOL PeekStr(char * pszIn , TCHAR * pszOut,char * cmp);
	static LONG64 Conv(int i);
	static DWORD GetLrcSelection(LPVOID lp);
	static DWORD DownloadLrc(LPVOID lp);
	int songCode[15];

	static UINT GetLrcUntilOK(LPVOID lp);
	// ��˫�����л�ȡ�ַ�
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
};
