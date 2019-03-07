
// memoDlg.h : ヘッダー ファイル
//

#pragma once

#include "afxcmn.h"
#include "afxwin.h"

#include "Saver.h"
#include "MongoSaver.h"

// CmemoDlg ダイアログ
class CmemoDlg : public CDialogEx
{
// コンストラクション
public:
	CmemoDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedAdd();
	int init_picker();
	void init_date();
	int insert_picker_item(int);
	void load_picker();
	void update_note();
	void update_body();
	void update_title();
	void delete_note();
	int get_selected_idx();


	//Saver sv;
	MongoSaver sv;

	CListCtrl m_list_picker;
	
	afx_msg void OnNMClickListPicker(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditBody();
	afx_msg void OnEnChangeEditTitle();
//	afx_msg void OnEnUpdateEditBody();
//	afx_msg void OnEnUpdateEditTitle();
	afx_msg void OnEnSetfocusEditTitle();
	afx_msg void OnEnSetfocusEditBody();
	afx_msg void OnEnKillfocusEditTitle();
	afx_msg void OnEnKillfocusEditBody();
	afx_msg void OnBnClickedButtonDelete();
	CStatic m_static_created;
	CStatic m_static_updated;
};
