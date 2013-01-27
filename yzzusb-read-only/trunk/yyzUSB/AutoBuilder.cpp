#include "StdAfx.h"
#include "AutoBuilder.h"
#include <fstream>
#include <iostream>

static const wstring title = _T("[Autorun]\r\n");
static const wstring open = _T("OPEN=");
static const wstring icon = _T("Icon=");

CAutoBuilder::CAutoBuilder(void)
{
}


CAutoBuilder::~CAutoBuilder(void)
{
}

wstring CAutoBuilder::getAutorunContext()
{
	wstring str = title;
	str.append(open);
	str.append(this->m_strExecPath);
	str.append(_T("\r\n"));
	return str;
}

void CAutoBuilder::setExec(wstring path)
{
	this->m_strExecPath = path;
}

int CAutoBuilder::buildAutorun(wstring path)
{
	wstring m_strAutoFileName;
	wofstream o_file;

	m_strAutoFileName.append(path);
	m_strAutoFileName.append(_T("\\Autorun.inf"));
	o_file.open(m_strAutoFileName.c_str(), ofstream::out | ofstream::trunc);
	if (!o_file.is_open())
	{
		cerr << "error: unable to open the autorun file!"<<endl;
		return -1;
	}
	
	o_file << this->getAutorunContext();

	o_file.close();

	char tarPath[256];

	memset(tarPath, 0, 256);
	memcpy(tarPath, m_strAutoFileName.c_str(), 256);

	BOOL result = SetFileAttributes(m_strAutoFileName.c_str(), 
		FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN);

	return 0;
}

int CAutoBuilder::buildAutorun(TCHAR diskLabel)
{
	wstring vol;
	
	vol.push_back(diskLabel);
	vol.push_back(':');

	buildAutorun(vol);

	return 0;
}