#pragma once
class CShape;
class CPtrArr
{
public:
	CPtrArr(void);
	void Add(void*);
	void* GetAt(int);
	int GetSize(void);
	void Clear();
	~CPtrArr(void);
	CPtrArr & operator=(CPtrArr&);
	CPtrArr(CPtrArr&);
	void ClearTheContent();
	void DeleteAt(int index);
	void CopyFrom(CPtrArr & ptrarr);
	void SetAt(int Index,CShape *);
private:
	void **p;
	int size;
	int Max_size;
};
