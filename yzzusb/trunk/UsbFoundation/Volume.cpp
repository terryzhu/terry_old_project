#include "StdAfx.h"
#include "Volume.h"

CVolume::CVolume(void)
	: m_strLabel(_T(""))
{
}

CVolume::CVolume(const CString& strLabel)
	: m_strLabel(_T(""))
{
	SetVolume(strLabel);
}

CVolume::~CVolume(void)
{
}


BOOL CVolume::IsVolumeExist()
{
	if (m_strLabel.IsEmpty()) return FALSE;
	DWORD dwVolumes = ::GetLogicalDrives();
	return dwVolumes & (0x1 << (m_strLabel.GetAt(0) - _T('A'))) ? TRUE : FALSE;
}

BOOL CVolume::IsUSBDevice()
{
	if (!IsVolumeExist()) return FALSE;

	CString strVolume = m_strLabel.Left(2);
	BOOL bRet = FALSE;
	UINT uDevType = ::GetDriveType(strVolume);
	if (uDevType == DRIVE_FIXED || uDevType == DRIVE_REMOVABLE)
	{
		CString strDevFile = _T("\\\\.\\");
		strDevFile += strVolume;
		HANDLE hDevFile = ::CreateFile(strDevFile,GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,OPEN_EXISTING,0,NULL);
		if (hDevFile == INVALID_HANDLE_VALUE) return FALSE;

		STORAGE_DEVICE_DESCRIPTOR devDesc;
		devDesc.Size = sizeof(STORAGE_DEVICE_DESCRIPTOR);
		STORAGE_PROPERTY_QUERY query;
		query.PropertyId = StorageDeviceProperty;
		query.QueryType = PropertyStandardQuery;

		DWORD dwBufferSize;
		BOOL bResult = ::DeviceIoControl(hDevFile,IOCTL_STORAGE_QUERY_PROPERTY,&query,sizeof(STORAGE_PROPERTY_QUERY),
			&devDesc,devDesc.Size,&dwBufferSize,NULL);
		if (bResult && devDesc.BusType == BusTypeUsb)
		{
			bRet = TRUE;
		}

		::CloseHandle(hDevFile);
	}

	return bRet;
}


BOOL CVolume::SetVolume(const CString& strLabel)
{
	if (ValidateLabel(strLabel))
	{
		m_strLabel = strLabel;
		NormalizeLabel(m_strLabel);
		return TRUE;
	}
	else
	{
		m_strLabel.Empty();
		return FALSE;
	}
}

BOOL CVolume::ValidateLabel(const CString& strLabel)
{
	CString strTemp = strLabel;
	strTemp.Trim();

	// Check length.
	if (strTemp.IsEmpty() || strTemp.GetLength() >3) return FALSE;

	// Check label.
	strTemp.MakeUpper();
	TCHAR chLabel = strTemp.GetAt(0);
	if (chLabel < _T('A') || chLabel > _T('Z')) return FALSE;

	// Check whole string.
	switch (strTemp.GetLength())
	{
	case 1:
		return TRUE;
	case 2:
		{
			CString strCompare;
			strCompare.Format(_T("%c:"),chLabel);
			return strTemp.CompareNoCase(strCompare) ? FALSE : TRUE;
		}
	case 3:
		{
			CString strCompare;
			strCompare.Format(_T("%c:\\"),chLabel);
			return strTemp.CompareNoCase(strCompare) ? FALSE : TRUE;
		}
	default: 
		return FALSE;
	}
}

void CVolume::NormalizeLabel(CString& strLabel)
{
	if (!ValidateLabel(strLabel)) return;

	strLabel.MakeUpper();
	if (strLabel.GetLength() == 2)
		strLabel += _T("\\");
	else if (strLabel.GetLength() == 1)
		strLabel += _T(":\\");
}


CVolumeNotify::CVolumeNotify(LPVOID lpFilter)
	: m_lpFilter(NULL)
{
	PDEV_BROADCAST_HDR pHeader = (PDEV_BROADCAST_HDR)lpFilter;
	if (!pHeader || pHeader->dbch_devicetype != DBT_DEVTYP_VOLUME) return;
	
	m_lpFilter = new BYTE[pHeader->dbch_size];
	memcpy(m_lpFilter,lpFilter,pHeader->dbch_size);
}

CVolumeNotify::~CVolumeNotify()
{
	if (m_lpFilter)
	{
		delete m_lpFilter;
		m_lpFilter = NULL;
	}
}

CVolumeNotify CVolumeNotify::VolumeNotifyFilter()
{
	CVolumeNotify ret;
	DEV_BROADCAST_DEVICEINTERFACE* pFilter = new DEV_BROADCAST_DEVICEINTERFACE;
	pFilter->dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	pFilter->dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	pFilter->dbcc_reserved = 0;
	pFilter->dbcc_classguid = GUID_DEVINTERFACE_VOLUME;
	pFilter->dbcc_name[0] = _T('\0');
	ret.m_lpFilter = pFilter;
	return ret;
}

CVolumeNotify::CVolumeNotify()
	: m_lpFilter(NULL)
{
}

CVolumeNotify::CVolumeNotify(const CVolumeNotify& other)
	: m_lpFilter(NULL)
{
	if (other.m_lpFilter)
	{
		PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)other.m_lpFilter;
		m_lpFilter = new BYTE[pHdr->dbch_size];
		memcpy(m_lpFilter,pHdr,pHdr->dbch_size);
	}
}


CVolumeCollection::CVolumeCollection()
{
	m_dwVolumes = ::GetLogicalDrives();
}

CVolumeCollection::CVolumeCollection(const CVolumeNotify& filter)
{
	m_dwVolumes = 0;
	PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)filter.Filter();
	if (pHdr && pHdr->dbch_devicetype == DBT_DEVTYP_VOLUME)
	{
		DEV_BROADCAST_VOLUME* pVolumeFilter = (DEV_BROADCAST_VOLUME*)pHdr;
		if (pVolumeFilter->dbcv_flags == DBTF_MEDIA)
			m_dwVolumes = pVolumeFilter->dbcv_unitmask;
	}
}

CVolumeCollection::~CVolumeCollection()
{
}



BOOL CVolumeCollection::IsVolumeExist(const CString& strLabel)
{
	if (!CVolume::ValidateLabel(strLabel)) return FALSE;

	CVolume::ValidateLabel(strLabel);
	return m_dwVolumes & (0x1 << (strLabel.GetAt(0) - _T('A'))) ? TRUE : FALSE;
}

CVolumeCollection::iterator CVolumeCollection::begin() const
{
	iterator it;
	it.m_dwVolumes = m_dwVolumes;
	for (it.m_nIndex = 0;it.m_nIndex < 26;it.m_nIndex++)
	{
		if (m_dwVolumes & (0x1 << it.m_nIndex))
			break;
	}
	return it;
}

CVolumeCollection::iterator CVolumeCollection::end() const
{
	iterator it;
	it.m_dwVolumes = m_dwVolumes;
	it.m_nIndex = 26;
	return it;
}


CVolumeCollection::iterator::iterator()
	: m_nIndex(0)
	, m_dwVolumes(0)
{
}

CVolumeCollection::iterator::~iterator()
{
}

CVolumeCollection::iterator::iterator(const CVolumeCollection::iterator& other)
{
	this->m_dwVolumes = other.m_dwVolumes;
	this->m_nIndex = other.m_nIndex;
}

CVolumeCollection::iterator& CVolumeCollection::iterator::operator= (const CVolumeCollection::iterator& other)
{
	if (this == &other) return *this;
	
	this->m_dwVolumes = other.m_dwVolumes;
	this->m_nIndex = other.m_nIndex;
	return *this;
}

bool CVolumeCollection::iterator::operator== (const CVolumeCollection::iterator& other) const
{
	return this->m_dwVolumes == other.m_dwVolumes && this->m_nIndex == other.m_nIndex;
}

bool CVolumeCollection::iterator::operator!= (const CVolumeCollection::iterator& other) const
{
	return this->m_dwVolumes != other.m_dwVolumes || this->m_nIndex != other.m_nIndex;
}

CVolumeCollection::iterator& CVolumeCollection::iterator::operator++ ()
{
	do 
	{
		this->m_nIndex++;
	} while (!(m_dwVolumes & (0x1 << m_nIndex)) && m_nIndex < 26);
	return *this;
}

CVolumeCollection::iterator CVolumeCollection::iterator::operator++ (int i)
{
	iterator it = *this;
	do 
	{
		this->m_nIndex++;
	} while (!(m_dwVolumes & (0x1 << m_nIndex)) && m_nIndex < 26);
	
	return it;
}

CVolume CVolumeCollection::iterator::operator* () const
{
	CString strLabel;
	strLabel.Format(_T("%c"),_T('A') + m_nIndex);
	return CVolume(strLabel);
}