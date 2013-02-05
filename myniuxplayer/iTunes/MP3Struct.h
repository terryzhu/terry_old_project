////////////////////////////////////////////////////////////
/********************** MP3 Info Struct *******************/
//   #include <sqltypes.h>  ------------>for DATE_STRUCT

#pragma once
#include <sqltypes.h>
#ifndef _MP3INFO
#define _MP3INFO



typedef struct tagMP3_INFO
{
	CString song_name;
	CString singer_name;
	CString album_name;
	CString	style;
	CString file_name;
	UINT	lasting_time;
	UINT	play_times;
	UINT	issue_year;
	DATE_STRUCT add_time;
} MP3_INFO;


#endif