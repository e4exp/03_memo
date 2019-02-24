
// memoDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "memo.h"
#include "memoDlg.h"
#include "afxdialogex.h"

#include "./saver.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CmemoDlg �_�C�A���O



CmemoDlg::CmemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PICKER, m_list_picker);
}

BEGIN_MESSAGE_MAP(CmemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(button_add, &CmemoDlg::OnBnClickedadd)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PICKER, &CmemoDlg::OnNMClickListPicker)
END_MESSAGE_MAP()


// CmemoDlg ���b�Z�[�W �n���h���[

BOOL CmemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B

	init_picker();
	//load titles from db to listview
	int notes_num = sv.load_notes();
	insert_picker_item(notes_num);







	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

void CmemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CmemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CmemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CmemoDlg::OnBnClickedadd()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B

	//get text
	CString str_title, str_body;
	((CEdit*)GetDlgItem(IDC_EDIT_TITLE))->GetWindowTextA(str_title);
	((CEdit*)GetDlgItem(IDC_EDIT_BODY))->GetWindowTextA(str_body);

	//store in db
	sv.store_note(str_title, str_body);

	//delete text
	((CEdit*)GetDlgItem(IDC_EDIT_BODY))->SetWindowTextA("");
	((CEdit*)GetDlgItem(IDC_EDIT_TITLE))->SetWindowTextA("");

	//load titles from db to listview

	/*
	for (int i = 0; i < sv.notes_cnt; i++) {
		sv.picker[i].clear();

	}
	sv.picker.clear();
	*/

	m_list_picker.DeleteAllItems();

	int notes_num=sv.load_notes();
	insert_picker_item(notes_num);

}



int  CmemoDlg::init_picker()
{	// ���X�g�R���g���[��������

	LVCOLUMN    lvc;
	int         i;
	TCHAR       caption[][32] = { _T("ID"), _T("title"),_T("body") };
	const int   clmNum = sizeof caption / sizeof caption[0];
	int         err = 0;
	m_list_picker.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // �L���t���O
	for (i = 0; i < clmNum; i++)
	{
		lvc.iSubItem = i;            // �T�u�A�C�e���ԍ�
		lvc.pszText = caption[i];   // ���o���e�L�X�g
		lvc.cx = 20;          // ����
		if (i == 1) {
			lvc.cx =60;
		}
		if (i == 2) {
			lvc.cx = 80;
		}
		
		if (m_list_picker.InsertColumn(i, &lvc) == -1) { err = 1; break; }

	}

	return err;
}


// ���X�g�A�C�e���}��
int CmemoDlg::insert_picker_item(int cnt) {
	
	const int col = 3;
	const int    itemNum = cnt * col;
	LVITEM       lvi;
	CString      str;
	int          i, index = 0;
	int          err = 0;

	for (i = 0; i < itemNum; i++)
	{

		int idx_r = (int)floor(i / col);
		int idx_c = (i % col);
		int idx_c_real = idx_c;
				
		lvi.mask = LVIF_TEXT;
		// ID
		if (!err)
		{
			lvi.iItem = idx_r;
			lvi.iSubItem = idx_c;

			
			lvi.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(sv.picker[idx_r][idx_c_real]));
			if (idx_c == 0) {
				if ((index = m_list_picker.InsertItem(&lvi)) == -1) err = 1;
			}
			else
			{
				if ((index = m_list_picker.SetItem(&lvi)) == -1) err = 1;
			}
		}


		if (err) {
			AfxMessageBox(_T("error occured when inserting item"));
			break;

		}
	}

	return err;
}



void CmemoDlg::OnNMClickListPicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	*pResult = 0;


	//get selected idx
	int idx = -1;
	int list_idx;
	while ((idx = m_list_picker.GetNextItem(idx, LVNI_ALL | LVNI_SELECTED)) != -1) {
		list_idx = idx;
	}

	//set texts
	CString str_title, str_body;
	str_title = sv.picker[list_idx][1];
	str_body = sv.picker[list_idx][2];
	((CEdit*)GetDlgItem(IDC_EDIT_TITLE))->SetWindowTextA(str_title);
	((CEdit*)GetDlgItem(IDC_EDIT_BODY))->SetWindowTextA(str_body);

	
}
