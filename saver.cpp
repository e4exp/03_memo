
#include "stdafx.h"
#include <windows.h>
#include "./saver.h"

Saver::Saver(){
	//AfxMessageBox(_T("constructed"));

	// sqlite3.dll�����[�h
	hDll = LoadLibrary("sqlite3");
	if (hDll == NULL) {
		FreeLibrary(hDll);
		AfxMessageBox(_T("dll cant be loaded."));
		return;
	}

	// sqlite3�֐��̃A�h���X���擾
	sqlite3_open = (pOpen)GetProcAddress(hDll, "sqlite3_open");
	sqlite3_exec = (pExec)GetProcAddress(hDll, "sqlite3_exec");
	sqlite3_get_table = (pGetTable)GetProcAddress(hDll, "sqlite3_get_table");
	sqlite3_free_table = (pFreeTable)GetProcAddress(hDll, "sqlite3_free_table");
	sqlite3_close = (pClose)GetProcAddress(hDll, "sqlite3_close");


	db_name= "memo.db";
	// �f�[�^�x�[�X�t�@�C�����쐬
	if (!sqlite3_open(db_name, &db)) {

		//TRACE("constructor::open=%d", ret);

		// �e�[�u���쐬
		CString query =
			"CREATE TABLE notes("
			"id	INTEGER PRIMARY KEY AUTOINCREMENT,"
			"title TEXT,"
			"note TEXT,"
			"created TIMESTAMP DEFAULT (DATETIME('now','localtime')),"
			"updated TIMESTAMP DEFAULT (DATETIME('now','localtime')) "
			");";


		sqlite3_exec(db, query, NULL, NULL, &err);

		
		// �f�[�^�x�[�X�t�@�C�������
		sqlite3_close(db);
		
	}



	
}


Saver::~Saver() {

}



void Saver::store_note(CString title, CString body) {
	

	//TRACE("open=%d", ret);
	if (!sqlite3_open(db_name, &db)) {

		// store
		CString query;
		query.Format("INSERT INTO notes(note,title) values('%s','%s');", (LPCTSTR)body, (LPCTSTR)title);
		sqlite3_exec(db, query, NULL, NULL, &err);

		sqlite3_close(db);

	}
	


}


int Saver::load_notes() {

		
	//TRACE("open=%d", ret);
	int cnt;
	if (!sqlite3_open(db_name, &db)) {


		//get count
		CString query;
		query.Format("SELECT COUNT(*) FROM notes ;");
		sqlite3_get_table(db, query.GetBuffer(), &result, &row, &col, &err);
		query.ReleaseBuffer();
		cnt = _ttoi(result[1]);
		const int itr = (cnt + 1) * COLUMN_NUM;

		//get notes
		query.Format("SELECT * FROM notes");
		sqlite3_get_table(db, query.GetBuffer(), &result, &row, &col, &err);
		query.ReleaseBuffer();

		
		picker.resize(cnt);
		int idx_r;
		int idx_c;
		for (int i = COLUMN_NUM; i < itr; i++) {
			idx_r = (int)floor((i - COLUMN_NUM) / COLUMN_NUM);
			idx_c = (i - COLUMN_NUM) % COLUMN_NUM;

			picker[idx_r].resize(COLUMN_NUM);
			picker[idx_r][idx_c] = result[i];
			
			/*
			if (i == c)g_largest_list_idx = _ttoi(result[i]);
			else if (i == itr - c)g_smallest_list_idx = _ttoi(result[i]);
			*/

		}

		
		sqlite3_free_table(result);
		sqlite3_close(db);


	}

	notes_cnt = cnt;
	return cnt;

}


