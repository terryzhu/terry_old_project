#pragma once
class CVolume
{
public:
	CVolume(void);
	CVolume(const CString& strLabel);
	~CVolume(void);

	BOOL SetVolume(const CString& strLabel);
	CString Label() {return m_strLabel;}
	BOOL IsVolumeExist();
	BOOL IsUSBDevice();

public:
	static void NormalizeLabel(CString& strLabel);
	static BOOL ValidateLabel(const CString& strLabel);

private:
	CString m_strLabel;
};


class CVolumeNotify
{
public:
	CVolumeNotify(LPVOID lpFilter);
	static CVolumeNotify VolumeNotifyFilter();
	~CVolumeNotify();

	LPVOID Filter() const {return m_lpFilter;}

private:
	CVolumeNotify();
	CVolumeNotify(const CVolumeNotify&);
	CVolumeNotify& operator= (const CVolumeNotify&);
	
private:
	LPVOID m_lpFilter;
};


class CVolumeCollection
{
public:
	class iterator
	{
		friend CVolumeCollection;
	public:
		iterator();
		~iterator();
		iterator(const iterator& other);
		iterator& operator= (const iterator& other);

	public:
		bool operator== (const iterator& other) const;
		bool operator!= (const iterator& other) const;
		iterator& operator++ ();
		iterator operator++ (int);
		CVolume operator* () const;

	private:
		int m_nIndex;
		DWORD m_dwVolumes;
	};

public:
	friend iterator;
	CVolumeCollection(void);
	CVolumeCollection(const CVolumeNotify& filter);
	~CVolumeCollection(void);

	BOOL IsVolumeExist(const CString& strLabel);
	
	iterator begin() const;
	iterator end() const;

private:
	DWORD m_dwVolumes;
};

