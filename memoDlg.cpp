
// memoDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "memo.h"
#include "memoDlg.h"
#include "afxdialogex.h"

#include "Saver.h"
#include "MongoSaver.h"



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
	DDX_Control(pDX, IDC_STATIC_CREATED, m_static_created);
	DDX_Control(pDX, IDC_STATIC_UPDATED, m_static_updated);
}

BEGIN_MESSAGE_MAP(CmemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CmemoDlg::OnBnClickedAdd)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PICKER, &CmemoDlg::OnNMClickListPicker)
	ON_EN_CHANGE(IDC_EDIT_BODY, &CmemoDlg::OnEnChangeEditBody)
	ON_EN_CHANGE(IDC_EDIT_TITLE, &CmemoDlg::OnEnChangeEditTitle)
	ON_EN_SETFOCUS(IDC_EDIT_TITLE, &CmemoDlg::OnEnSetfocusEditTitle)
	ON_EN_SETFOCUS(IDC_EDIT_BODY, &CmemoDlg::OnEnSetfocusEditBody)
	ON_EN_KILLFOCUS(IDC_EDIT_TITLE, &CmemoDlg::OnEnKillfocusEditTitle)
	ON_EN_KILLFOCUS(IDC_EDIT_BODY, &CmemoDlg::OnEnKillfocusEditBody)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CmemoDlg::OnBnClickedButtonDelete)
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
	init_date();
	



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


void CmemoDlg::OnBnClickedAdd()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B

	//get text
	CString str_title, str_body;

	//delete text
	((CEdit*)GetDlgItem(IDC_EDIT_BODY))->SetWindowTextA("");
	((CEdit*)GetDlgItem(IDC_EDIT_TITLE))->SetWindowTextA("");

	/*
	((CEdit*)GetDlgItem(IDC_EDIT_TITLE))->GetWindowTextA(str_title);
	((CEdit*)GetDlgItem(IDC_EDIT_BODY))->GetWindowTextA(str_body);
	*/

	//store in db
	sv.insert_note(str_title, str_body);

	
	load_picker();

}

void CmemoDlg::load_picker() {
	m_list_picker.DeleteAllItems();
	int notes_num = sv.load_notes();
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

		if (idx_c == col)continue;
				
		lvi.mask = LVIF_TEXT;
		// ID
		if (!err)
		{
			lvi.iItem = idx_r;
			lvi.iSubItem = idx_c;

			
			lvi.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(sv.picker.holder[idx_r][idx_c]));
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


int CmemoDlg::get_selected_idx() {

	int idx = -1;
	int list_idx = 0;
	while ((idx = m_list_picker.GetNextItem(idx, LVNI_ALL | LVNI_SELECTED)) != -1) {
		list_idx = idx;

	}

	return list_idx;

}

void CmemoDlg::init_date() {
	m_static_created.SetWindowTextA("created: ");
	m_static_updated.SetWindowTextA("updated: ");
}


void CmemoDlg::OnNMClickListPicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	*pResult = 0;

	if (sv.picker.whole_cnt == 0)return;

	//get selected idx
	int list_idx=get_selected_idx();
	
	//set texts
	CString str_title, str_body;
	str_title = sv.picker.holder[list_idx][1];
	str_body = sv.picker.holder[list_idx][2];
	((CEdit*)GetDlgItem(IDC_EDIT_TITLE))->SetWindowTextA(str_title);
	((CEdit*)GetDlgItem(IDC_EDIT_BODY))->SetWindowTextA(str_body);

	/*
	//hold
	sv.picker.old_title = str_title;
	sv.picker.old_body = str_body;
	*/


	//hold index
	sv.picker.selected_idx = _ttoi(sv.picker.holder[list_idx][0]);

	CString create="created: "+sv.picker.holder[list_idx][3];
	CString update = "updated: " + sv.picker.holder[list_idx][4];
	m_static_created.SetWindowTextA(create);
	m_static_updated.SetWindowTextA(update);

}


void CmemoDlg::update_note() {

	//update current note
	//get text
	CString str_body;
	((CEdit*)GetDlgItem(IDC_EDIT_BODY))->GetWindowTextA(str_body);
	CString str_title;
	((CEdit*)GetDlgItem(IDC_EDIT_TITLE))->GetWindowTextA(str_title);
	//get selected idx
	int idx= sv.picker.selected_idx;
	sv.update_title(str_title, idx);
	sv.update_body(str_body, idx);
	load_picker();


}


void CmemoDlg::update_body() {
	if (!sv.save_body)return;

	
	//get text
	CString str_body;
	((CEdit*)GetDlgItem(IDC_EDIT_BODY))->GetWindowTextA(str_body);
	int idx = sv.picker.selected_idx;

	sv.update_body(str_body, idx);	
	load_picker();


}


void CmemoDlg::update_title() {
	if (!sv.save_title)return;


	CString str_title;
	((CEdit*)GetDlgItem(IDC_EDIT_TITLE))->GetWindowTextA(str_title);
	//get selected idx
	int idx = sv.picker.selected_idx;
		
	sv.update_title(str_title, idx);
	load_picker();


}


void CmemoDlg::delete_note() {
	
	if (sv.picker.whole_cnt == 0)return;

	int idx = sv.picker.selected_idx;
	sv.delete_note (idx);
	load_picker();

}

void CmemoDlg::OnEnChangeEditBody()
{
	// TODO: ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A���̃R���g���[����
	// ���̒ʒm�𑗐M����ɂ́ACDialogEx::OnInitDialog() �֐����I�[�o�[���C�h���A
	// CRichEditCtrl().SetEventMask() ��
	// OR ��Ԃ� ENM_CHANGE �t���O���}�X�N�ɓ���ČĂяo���K�v������܂��B

	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����Ă��������B

	
	update_body();


}


void CmemoDlg::OnEnChangeEditTitle()
{
	// TODO: ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A���̃R���g���[����
	// ���̒ʒm�𑗐M����ɂ́ACDialogEx::OnInitDialog() �֐����I�[�o�[���C�h���A
	// CRichEditCtrl().SetEventMask() ��
	// OR ��Ԃ� ENM_CHANGE �t���O���}�X�N�ɓ���ČĂяo���K�v������܂��B

	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����Ă��������B

	update_title();


}


//save flag title
void CmemoDlg::OnEnSetfocusEditTitle()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	sv.save_title = true;

}

//save flag body
void CmemoDlg::OnEnSetfocusEditBody()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	sv.save_body = true;
}

//save flag title
void CmemoDlg::OnEnKillfocusEditTitle()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	sv.save_title = false;
}

//save flag body
void CmemoDlg::OnEnKillfocusEditBody()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	sv.save_body = false;
}

//delete button
void CmemoDlg::OnBnClickedButtonDelete()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	delete_note();


}
