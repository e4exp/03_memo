
#include "stdafx.h"

#include <windows.h>
#include "Saver.h"

Saver::Saver(){
	//AfxMessageBox(_T("constructed"));

	// sqlite3.dllをロード
	hDll = LoadLibrary("sqlite3");
	if (hDll == NULL) {
		FreeLibrary(hDll);
		AfxMessageBox(_T("dll cant be loaded."));
		return;
	}

	// sqlite3関数のアドレスを取得
	sqlite3_open = (pOpen)GetProcAddress(hDll, "sqlite3_open");
	sqlite3_exec = (pExec)GetProcAddress(hDll, "sqlite3_exec");
	sqlite3_get_table = (pGetTable)GetProcAddress(hDll, "sqlite3_get_table");
	sqlite3_free_table = (pFreeTable)GetProcAddress(hDll, "sqlite3_free_table");
	sqlite3_close = (pClose)GetProcAddress(hDll, "sqlite3_close");


	db_name= "memo.db";
	// データベースファイルを作成
	if (!sqlite3_open(db_name, &db)) {

		//TRACE("constructor::open=%d", ret);

		// テーブル作成
		CString query =
			"CREATE TABLE notes("
			"id	INTEGER PRIMARY KEY AUTOINCREMENT,"
			"title TEXT,"
			"note TEXT,"
			"created TIMESTAMP DEFAULT (DATETIME('now','localtime')),"
			"updated TIMESTAMP DEFAULT (DATETIME('now','localtime')),"
			"deleted INTEGER NOT NULL DEFAULT 0 "
			");";
		sqlite3_exec(db, query, NULL, NULL, &err);
		query.ReleaseBuffer();

		/*
		//trigger
		query =
			"CREATE TRIGGER UpdateLastTime UPDATE OF title, note, deleted ON notes "
			"BEGIN "
			"UPDATE notes SET updated = (DATETIME('now','localtime')) WHERE deleted = 0; "
			"END;";
		sqlite3_exec(db, query, NULL, NULL, &err);
		query.ReleaseBuffer();
		*/
		
		// データベースファイルを閉じる
		sqlite3_close(db);
		
	}



	
}


Saver::~Saver() {

}



void Saver::insert_note(CString title, CString body) {
	

	//TRACE("open=%d", ret);
	if (!sqlite3_open(db_name, &db)) {

		// store
		CString query;
		query.Format("INSERT INTO notes(note,title) values('%s','%s');", (LPCTSTR)body, (LPCTSTR)title);
		sqlite3_exec(db, query, NULL, NULL, &err);
		query.ReleaseBuffer();
		sqlite3_close(db);

	}
	


}


void Saver::update_note(CString title, CString body, int id) {

		
	if (!sqlite3_open(db_name, &db)) {
		

		CString query;
		query.Format("UPDATE notes SET note = '%s', title = '%s' WHERE id=%d ;", (LPCTSTR)body, (LPCTSTR)title, id); //'%s'  quotation needed for this statement.
		int ret=sqlite3_exec(db, query, NULL, NULL, &err);
		if (ret != SQLITE_OK) {
			CString est;
			est.Format("update: %s", err);
			AfxMessageBox(est);
		}
		query.ReleaseBuffer();


		sqlite3_close(db);

	}



}

void Saver::update_title(CString title, int id) {


	if (!sqlite3_open(db_name, &db)) {


		CString query;
		query.Format("UPDATE notes SET title = '%s', updated=(DATETIME('now','localtime')) WHERE id=%d ;",(LPCTSTR)title, id); //'%s'  quotation needed for this statement.
		int ret = sqlite3_exec(db, query, NULL, NULL, &err);
		if (ret != SQLITE_OK) {
			CString est;
			est.Format("update: %s", err);
			AfxMessageBox(est);
		}
		query.ReleaseBuffer();
		sqlite3_close(db);
	}
}



void Saver::update_body(CString body, int id) {


	if (!sqlite3_open(db_name, &db)) {

		CString query;
		query.Format("UPDATE notes SET note = '%s', updated=(DATETIME('now','localtime')) WHERE id=%d ;", (LPCTSTR)body, id); //'%s'  quotation needed for this statement.
		int ret = sqlite3_exec(db, query, NULL, NULL, &err);
		if (ret != SQLITE_OK) {
			CString est;
			est.Format("update: %s", err);
			AfxMessageBox(est);
		}
		query.ReleaseBuffer();
		sqlite3_close(db);
	}
}


void Saver::delete_note(int id) {
	
	if (!sqlite3_open(db_name, &db)) {

		CString query;
		query.Format("UPDATE notes SET deleted = 1,updated=(DATETIME('now','localtime')) WHERE id=%d ;", id);
		int ret = sqlite3_exec(db, query, NULL, NULL, &err);
		if (ret != SQLITE_OK) {
			CString est;
			est.Format("delete: %s", err);
			AfxMessageBox(est);
		}
		query.ReleaseBuffer();
		sqlite3_close(db);
	}
}





int Saver::load_notes() {

		
	//TRACE("open=%d", ret);
	int cnt=0;
	if (!sqlite3_open(db_name, &db)) {


		//get count
		CString query;
		query.Format("SELECT COUNT(*) FROM notes WHERE deleted=0;");
		sqlite3_get_table(db, query.GetBuffer(), &result, &row, &col, &err);
		query.ReleaseBuffer();
		cnt = _ttoi(result[1]);
		const int itr = (cnt + 1) * COLUMN_NUM;

		//get notes
		//query.Format("SELECT * FROM notes ORDER BY id DESC;");
		query.Format("SELECT id, title, note, created, updated, deleted FROM notes WHERE deleted=0 ORDER BY id DESC;");
		int ret=sqlite3_get_table(db, query.GetBuffer(), &result, &row, &col, &err);
		query.ReleaseBuffer();
		if (ret != SQLITE_OK) {
			CString est;
			est.Format("select: %s", err);
			AfxMessageBox(est);
		}

		//get whole count
		picker.whole_cnt = cnt;

		//insert holder notes, titles, ids
		picker.holder.resize(cnt);
		int idx_r;
		int idx_c;
		for (int i = COLUMN_NUM; i < itr; i++) {
			idx_r = (int)floor((i - COLUMN_NUM) / COLUMN_NUM);
			idx_c = (i - COLUMN_NUM) % COLUMN_NUM;

			picker.holder[idx_r].resize(COLUMN_NUM);
			picker.holder[idx_r][idx_c] = result[i];
			
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


