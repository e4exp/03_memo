
// memoDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "memo.h"
#include "memoDlg.h"
#include "afxdialogex.h"

#include "Saver.h"
#include "MongoSaver.h"
#include "Utils.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
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


// CmemoDlg ダイアログ



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


// CmemoDlg メッセージ ハンドラー

BOOL CmemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
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

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	init_picker();
	//load titles from db to listview
	int notes_num = sv.load_notes();
	insert_picker_item(notes_num);
	init_date();
	



	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
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

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CmemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CmemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CmemoDlg::OnBnClickedAdd()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	//get text
	CString str_title, str_body;
	str_title = "";
	str_body = "";

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
{	// リストコントロール初期化

	LVCOLUMN    lvc;
	int         i;
	TCHAR       caption[][32] = { _T("ID"), _T("title"),_T("body") };
	const int   clmNum = sizeof caption / sizeof caption[0];
	int         err = 0;
	m_list_picker.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // 有効フラグ
	for (i = 0; i < clmNum; i++)
	{
		lvc.iSubItem = i;            // サブアイテム番号
		lvc.pszText = caption[i];   // 見出しテキスト
		lvc.cx = 20;          // 横幅
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


// リストアイテム挿入
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
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
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
	//sv.picker.selected_idx = _ttoi(sv.picker.holder[list_idx][0]);
	sv.picker.selected_id = sv.picker.holder[list_idx][0];

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
	CString idx= sv.picker.selected_id;
	sv.update_title(str_title, idx);
	sv.update_body(str_body, idx);
	load_picker();


}


void CmemoDlg::update_body() {
	if (!sv.save_body)return;

	
	//get text
	CString str_body;
	((CEdit*)GetDlgItem(IDC_EDIT_BODY))->GetWindowTextA(str_body);
	CString idx = sv.picker.selected_id;

	sv.update_body(str_body, idx);	
	load_picker();


}


void CmemoDlg::update_title() {
	if (!sv.save_title)return;


	CString str_title;
	((CEdit*)GetDlgItem(IDC_EDIT_TITLE))->GetWindowTextA(str_title);
	//get selected idx
	CString idx = sv.picker.selected_id;
		
	sv.update_title(str_title, idx);
	load_picker();


}


void CmemoDlg::delete_note() {
	
	if (sv.picker.whole_cnt == 0)return;

	CString idx = sv.picker.selected_id;
	sv.delete_note (idx);
	load_picker();

}

int CmemoDlg::get_text_length_in_ctrl(int ctrl) {

	CString str_body;
	((CEdit*)GetDlgItem(ctrl))->GetWindowTextA(str_body);
	int len = str_body.GetLength();

	return len;
}

void CmemoDlg::OnEnChangeEditBody()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
	/*
	int body_count = get_text_length_in_ctrl(IDC_EDIT_BODY);
	if (abs(body_count-sv.picker.old_body_count) > sv.picker.BODY_LOAD_LIMIT) {
		update_body();
	}

	sv.picker.old_body_count = body_count;
	*/
	if (sv.picker.update_body_count > sv.picker.UPDATE_BODY_LIMIT) {
		update_body();
		sv.picker.update_body_count = 0;
	}
	else {
		sv.picker.update_body_count++;
	}
	
	
}


void CmemoDlg::OnEnChangeEditTitle()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。

	//date_title();
	if (sv.picker.update_title_count > sv.picker.UPDATE_TITLE_LIMIT) {
		update_title();
		sv.picker.update_title_count = 0;
	}
	else {
		sv.picker.update_title_count++;
	}


}


//save flag title
void CmemoDlg::OnEnSetfocusEditTitle()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	sv.save_title = true;

}

//save flag body
void CmemoDlg::OnEnSetfocusEditBody()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	sv.save_body = true;
}

//save flag title
void CmemoDlg::OnEnKillfocusEditTitle()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	update_title();
	sv.save_title = false;
}

//save flag body
void CmemoDlg::OnEnKillfocusEditBody()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	update_body();
	sv.save_body = false;
}

//delete button
void CmemoDlg::OnBnClickedButtonDelete()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	delete_note();


}
