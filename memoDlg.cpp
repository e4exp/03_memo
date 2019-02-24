
// memoDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "memo.h"
#include "memoDlg.h"
#include "afxdialogex.h"

#include "./saver.h"




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
}

BEGIN_MESSAGE_MAP(CmemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(button_add, &CmemoDlg::OnBnClickedadd)
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


void CmemoDlg::OnBnClickedadd()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	//get text
	CString str;
	((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowTextA(str);

	//store in db
	sv.store_note(str);

	//delete text
	((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowTextA("");

	//load titles from db to listview
	int notes_num=sv.load_notes();
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
			lvc.cx = 30;
		}
		if (i == 2) {
			lvc.cx = 40;
		}
		
		if (m_list_picker.InsertColumn(i, &lvc) == -1) { err = 1; break; }

	}

	return err;
}


// リストアイテム挿入
int CmemoDlg::insert_picker_item(int cnt) {
	const int col = 2;
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
		//if (idx_c >= 3)idx_c_real++;
				
		lvi.mask = LVIF_TEXT;
		// ID
		if (!err)
		{
			lvi.iItem = idx_r;
			lvi.iSubItem = idx_c;

			//lvi.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(sv.picker[idx_r][idx_c_real]));
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