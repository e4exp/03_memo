#ifndef INCLUDE_GUARD_SAVER_H
#define INCLUDE_GUARD_SAVER_H


#include "stdafx.h"
#include "./sqlite3.h"
#include "Picker.h"

// sqlite3.dll���֐��|�C���^�^��`
typedef int(__cdecl * pOpen)(char *, sqlite3 **);
typedef int(__cdecl * pExec)(sqlite3 *, const char *, sqlite3_callback, void *, char **);
typedef int(__cdecl * pGetTable)(sqlite3 *, const char *, char ***, int *, int *, char **);
typedef int(__cdecl * pFreeTable)(char **);
typedef int(__cdecl * pClose)(sqlite3 *);



class Saver {



public:
	Saver();
	~Saver();

	void insert_note(CString, CString);
	void update_note(CString, CString, int);
	void update_title(CString, int);
	void update_body(CString, int);
	int load_notes();
	void delete_note(int);
	

public:

	// �֐��|�C���^�ϐ�
	pOpen sqlite3_open;
	pExec sqlite3_exec;
	pGetTable sqlite3_get_table;
	pFreeTable sqlite3_free_table;
	pClose sqlite3_close;


	HINSTANCE hDll;
	sqlite3 *db;
	char *err;
	char **result;
	int row, col;
	
	char* db_name;

	static const int COLUMN_NUM = 6; //all columns
	int notes_cnt;
	//CString **picker; // [][COLUMN_NUM];
	//std::vector<std::vector<CString>> picker; //buffer for storing all notes
	Picker picker;

	bool save_title=false;
	bool save_body=false;


};

#endif // INCLUDE_GUARD_SAVER_H
