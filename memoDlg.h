
// memoDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "saver.h"
#include "afxcmn.h"

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
	afx_msg void OnBnClickedadd();
	int init_picker();
	int insert_picker_item(int);


	Saver sv;

	CListCtrl m_list_picker;
	afx_msg void OnHdnItemclickListPicker(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListPicker(NMHDR *pNMHDR, LRESULT *pResult);
};
