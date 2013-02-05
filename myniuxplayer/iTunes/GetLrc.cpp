// GetLrc.cpp : 实现文件
//

#include "stdafx.h"
#include "iTunes.h"
#include "GetLrc.h"
#include "afxinet.h"

// CGetLrc 对话框

IMPLEMENT_DYNAMIC(CGetLrc, CDialog)

CGetLrc::CGetLrc(CString songName,CString singerName,CString lrcpath,
				 CWnd* pParent /*=NULL*/)
	: CDialog(CGetLrc::IDD, pParent)
	, m_singer(singerName)
	, m_song(songName)
{
	this->lrcpath=lrcpath;
}

CGetLrc::~CGetLrc()
{
}

void CGetLrc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_singer);
	DDX_Text(pDX, IDC_EDIT2, m_song);
	DDX_Control(pDX, IDC_LIST1, m_listctl);
	DDX_Control(pDX, IDC_LIST2, m_list);
}


BEGIN_MESSAGE_MAP(CGetLrc, CDialog)
	ON_BN_CLICKED(IDC_BUTTONGETLRC, &CGetLrc::OnBnClickedButton1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CGetLrc::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CGetLrc::OnBnClickedButton2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CGetLrc::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// CGetLrc 消息处理程序

LONG64 CGetLrc::Conv(int i)
{
	LONG64 r = i % 4294967296;
	if (i >= 0 && r > 2147483648)
		r = r - 4294967296;
	if (i < 0 && r < 2147483648)
		r = r + 4294967296;
	return r;
}

int CGetLrc::CreateCode(TCHAR * buf,int lrcId)
{
	int length=WideCharToMultiByte(CP_UTF8,NULL,buf,-1,0,0,0,0)-1;

#if MYDEBUG
	CString s;
	s.Format(L"%d",length);
	AfxMessageBox(s);
#endif

	//int length =wcslen(buf)*3;
	char * aa=new char[length];
	WideCharToMultiByte(CP_UTF8,NULL,buf,-1,aa,length,0,0);
	int *song=new int [length];
	for (int i=0;i<length;i++)
	{
		song[i]=(unsigned char)aa[i];
	}
	int t1 = 0, t2 = 0, t3 = 0;
	t1 = (lrcId & 0x0000FF00) >> 8;
	if ((lrcId & 0x00FF0000) == 0) {
		t3 = 0x000000FF & ~t1;
	} else {
		t3 = 0x000000FF & ((lrcId & 0x00FF0000) >> 16);
	}

	t3 = t3 | ((0x000000FF & lrcId) << 8);
	t3 = t3 << 8;
	t3 = t3 | (0x000000FF & t1);
	t3 = t3 << 8;
	if ((lrcId & 0xFF000000) == 0) {
		t3 = t3 | (0x000000FF & (~lrcId));
	} else {
		t3 = t3 | (0x000000FF & (lrcId >> 24));
	}

	int j = length - 1;
	while (j >= 0) {
		int c = song[j];
		if (c >= 0x80) c = c - 0x100;

		t1 = (int)((c + t2) & 0x00000000FFFFFFFF);
		t2 = (int)((t2 << (j % 2 + 4)) & 0x00000000FFFFFFFF);
		t2 = (int)((t1 + t2) & 0x00000000FFFFFFFF);
		j -= 1;
	}
	j = 0;
	t1 = 0;
	while (j <= length - 1) {
		int c = song[j];
		if (c >= 128) c = c - 256;
		int t4 = (int)((c + t1) & 0x00000000FFFFFFFF);
		t1 = (int)((t1 << (j % 2 + 3)) & 0x00000000FFFFFFFF);
		t1 = (int)((t1 + t4) & 0x00000000FFFFFFFF);
		j += 1;
	}

	int t5 = (int)Conv(t2 ^ t3);
	t5 = (int)Conv(t5 + (t1 | lrcId));
	t5 = (int)Conv(t5 * (t1 | t3));
	t5 = (int)Conv(t5 * (t2 ^ lrcId));

	LONG64 t6 = (LONG64)t5;
	if (t6 > 2147483648)
		t5 = (int)(t6 - 4294967296);
	return t5;

}

void CGetLrc::ConvertStrToHex(CString strIn, CString & strOut)
{
	TCHAR * szSinger;//=new TCHAR[10];
	szSinger=strIn.GetBuffer(strIn.GetLength());
	BYTE *pbyte=new BYTE[strIn.GetLength()*2];
	memcpy_s(pbyte,strIn.GetLength()*2,szSinger,strIn.GetLength()*2);
	for (int i=0;i<strIn.GetLength()*2;i++)
	{
		CString str;
		if (pbyte[i]<16)
		{
			str.Format(L"0%X",pbyte[i]);
		}
		else
		{
			str.Format(L"%2X",pbyte[i]);
		}

		strOut+=str;
	}
	delete [] pbyte;
}
BOOL CGetLrc::PeekStr(char * pszIn , TCHAR * pszOut,char * cmp)
{
	char code[40]={0};
	while (*pszIn!=NULL)
	{
		//歌名转换为一个code
		if (*pszIn==cmp[0]&&*(pszIn+1)==cmp[1]&&*(pszIn+2)==cmp[2])
		{
			pszIn+=4;
			int i=0;
			while (*pszIn!='"')
			{
				i++;
				pszIn++;
			}
			pszIn-=i;
			memcpy_s(code,i,pszIn,i);//
			MultiByteToWideChar(CP_UTF8,0,code,-1,pszOut,50);
			return TRUE;
		}
		pszIn++;
	}
	return FALSE;
}

void CGetLrc::OnBnClickedButton1()
{

	UpdateData();
	m_listctl.DeleteAllItems();
	HANDLE handle=CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)GetLrcSelection,this,NULL,NULL);
	CloseHandle(handle);
	
	return;
}

DWORD CGetLrc::GetLrcSelection(LPVOID lp)
{
	CGetLrc *p=(CGetLrc*)lp;
	CInternetSession   sess;   
	CHttpFile*   fileGet;
	TCHAR buf[500];
	TCHAR ss[500];
	TCHAR szArtist[50]={0};
	TCHAR szTitle[50]={0};
	CString strArtist;
	CString strTitle;
	ConvertStrToHex(p->m_singer,strArtist);
	ConvertStrToHex(p->m_song,strTitle);

	try   
	{
		fileGet=(CHttpFile*)sess.OpenURL(L"http://ttlrcct2.qianqian.com/dll/lyricsvr.dll?sh?Artist="
			+strArtist+L"&Title="+strTitle+L"&Flags=0");
	}   
	catch(...)   
	{   
		::AfxMessageBox(_T("链接无效!"));
		sess.Close();   
		return  0 ;   
	}
	//char code[15]={0};
	int index=0;
	ZeroMemory(p->songCode,15*sizeof(int));
	while(fileGet->ReadString(buf,500))   
	{
		char *pc=(char*)buf;
		if (PeekStr(pc,ss,"id="))
		{
			PeekStr(pc,szArtist,"st=");
			PeekStr(pc,szTitle,"le=");
			p->m_listctl.InsertItem(0,szArtist);
			p->m_listctl.SetItemText(0,1,szTitle);
			int codeId=wcstol(ss,NULL,10);
			p->songCode[index]=codeId;
			//	MessageBox(ss);
			index++;
		}
	} 
	fileGet->Close();
	sess.Close(); 
	return 0;
}
DWORD CGetLrc::DownloadLrc(LPVOID lp)
{
	CGetLrc * pglc=(CGetLrc*)lp;
	POSITION pos=pglc->m_listctl.GetFirstSelectedItemPosition();
	int index=pglc->m_listctl.GetNextSelectedItem(pos);
	if (index==-1)
	{
		return 0;
	}
	pglc->m_list.ResetContent();
	pglc->m_singer=pglc->m_listctl.GetItemText(index,0);
	int codeId=pglc->songCode[pglc->m_listctl.GetItemCount()-1-index];
	pglc->m_song=pglc->m_listctl.GetItemText(index,1);
	pglc->m_singer+=pglc->m_song;
	TCHAR * sz=pglc->m_singer.GetBuffer(pglc->m_singer.GetLength());
	int result=CreateCode(sz,codeId);
	CString str;
	str.Format(L"%d",result);
	CString songid;
	songid.Format(L"%d",codeId);
	//保存到歌词的字符串
	CString strLrcFile;

	CInternetSession   sess;   
	CHttpFile*   fileGet;   
	fileGet=(CHttpFile*)sess.OpenURL(L"http://ttlrcct2.qianqian.com/dll/lyricsvr.dll?dl?Id="
		+songid+L"&Code="+str);
	TCHAR szlrcTemp[50]={0};
	TCHAR buf[500]={0};
	while(fileGet->ReadString(buf,500))
	{
		char *p=(char*)buf;
		MultiByteToWideChar(CP_UTF8,0,p,-1,szlrcTemp,50);
		strLrcFile+=szlrcTemp;
		pglc->m_list.AddString(szlrcTemp);
		strLrcFile+=L"\r\n";
	}
	int *xxx=new int[200];
	//把Unicode转换为ANSI
	TCHAR *p=strLrcFile.GetBuffer(strLrcFile.GetLength());
	int size=WideCharToMultiByte(CP_ACP,NULL,p,-1,0,0,0,0);
	char  *b=new char[size];
	WideCharToMultiByte(CP_ACP,NULL,p,-1,b,size,0,0);

	//保存到lrc文件
	if (pglc->lrcpath.GetLength()<4)
	{
		pglc->lrcpath=L"lrc.txt";
	}
	CFile   file(pglc->lrcpath,   CFile::modeCreate|CFile::modeReadWrite);
	
	file.Write(b,size);
	delete [] b;
	sess.Close();   
	file.Close();
	fileGet->Close();
	
	return 0;
}
BOOL CGetLrc::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listctl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listctl.InsertColumn(0,L"歌手", LVCFMT_CENTER,100);
	m_listctl.InsertColumn(1,L"歌名", LVCFMT_CENTER,100);
	UpdateData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CGetLrc::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	HANDLE handle=CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)DownloadLrc,this,NULL,NULL);
	CloseHandle(handle);
}

void CGetLrc::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	

}

void CGetLrc::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

UINT CGetLrc::GetLrcUntilOK(LPVOID lp)
{
	DOWNLOADLRC *p=(DOWNLOADLRC*)lp;
	CInternetSession   sess;   
	CHttpFile*   fileGet;
	TCHAR buf[500];
	TCHAR ss[500];
	TCHAR szArtist[50]={0};
	TCHAR szTitle[50]={0};
	CString strArtist;
	CString strTitle;
	ConvertStrToHex(p->m_strSingerName,strArtist);
	ConvertStrToHex(p->m_strSongName,strTitle);
	try   
	{
		fileGet=(CHttpFile*)sess.OpenURL(L"http://ttlrcct2.qianqian.com/dll/lyricsvr.dll?sh?Artist="
			+strArtist+L"&Title="+strTitle+L"&Flags=0");
	}   
	catch(...)   
	{   
		::AfxMessageBox(_T("链接无效!"));
		sess.Close();   
		return  0 ;   
	}
	int index=0;
	ZeroMemory(&p->songCode,sizeof(int));
	while(fileGet->ReadString(buf,500))   
	{
		char *pc=(char*)buf;
		if (PeekStr(pc,ss,"id="))
		{
			PeekStr(pc,szArtist,"st=");
			PeekStr(pc,szTitle,"le=");
		//	p->m_listctl.InsertItem(0,szArtist);
		//	p->m_listctl.SetItemText(0,1,szTitle);
			int codeId=wcstol(ss,NULL,10);
			p->songCode=codeId;
			index++;
			break;
		}
	}
	if (index==0)
	{
		return 0;
	}

	fileGet->Close();
	sess.Close(); 

	CString strSinger=szArtist;
	CString strSong=szTitle;



	int codeId=p->songCode;
	strSinger+=strSong;
	TCHAR * sz=strSinger.GetBuffer(strSinger.GetLength());
	int result=CreateCode(sz,codeId);
	CString str;
	str.Format(L"%d",result);
	CString songid;
	songid.Format(L"%d",codeId);
	//保存到歌词的字符串
	CString strLrcFile;
	fileGet=NULL;
	CInternetSession   sess2;   
	//CHttpFile*   fileGet;   
	
	fileGet=(CHttpFile*)sess2.OpenURL(L"http://ttlrcct2.qianqian.com/dll/lyricsvr.dll?dl?Id="
		+songid+L"&Code="+str);
	TCHAR szlrcTemp[50]={0};
	while(fileGet->ReadString(buf,500))   
	{
		char *p=(char*)buf;
		MultiByteToWideChar(CP_UTF8,0,p,-1,szlrcTemp,50);
		strLrcFile+=szlrcTemp;
		//pglc->m_list.AddString(szlrcTemp);
		strLrcFile+=L"\r\n";
	}
	int *xxx=new int[200];
	//把Unicode转换为ANSI
	TCHAR *pLrcFile=strLrcFile.GetBuffer(strLrcFile.GetLength());
	int size=WideCharToMultiByte(CP_ACP,NULL,pLrcFile,-1,0,0,0,0);
	char  *b=new char[size];
	WideCharToMultiByte(CP_ACP,NULL,pLrcFile,-1,b,size,0,0);

	//保存到lrc文件
	CString filePath(p->MusicLrcPath);
	//filePath.Format(L"I:\\Program Files\\TTPlayer\\Lyrics%s%s.lrc",szArtist,szTitle);
	CFile   file(filePath,CFile::modeCreate|CFile::modeWrite);
	file.Write(b,size);
	delete [] b;
	sess2.Close();   
	file.Close();
	return 0;
}