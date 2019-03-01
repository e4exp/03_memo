
// memoDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "saver.h"
#include "afxcmn.h"
#include "afxwin.h"

// CmemoDlg �_�C�A���O
class CmemoDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CmemoDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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


	Saver sv;

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
