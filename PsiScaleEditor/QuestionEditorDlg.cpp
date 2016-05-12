
// PsiScaleEditorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PsiScaleEditor.h"
#include "QuestionEditorDlg.h"
#include "afxdialogex.h"
#include "../PsiCommon/TestManager.h"
#include "InputStringDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

CQuestionEditorDlg::CQuestionEditorDlg(shared_ptr<PsiScale> scale, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_QUESTION_EDITOR, pParent)
	, _question_text(_T(""))
	, _scale(scale)
	, _reverse_score(FALSE)
	, _current_question(-1)
{
	ASSERT(_scale);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQuestionEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_QUESTION, _question_text);
	DDX_Control(pDX, IDC_COMBO_GROUPS, _group_combo);
	DDX_Control(pDX, IDC_CHOICE_LIST, _choice_list);
	DDX_Check(pDX, IDC_CHECK_REVERSE_SCORE, _reverse_score);
	DDX_Control(pDX, IDC_DELETE_QUESTION, _delete_button);
	DDX_Control(pDX, IDC_BUTTON_PREV, _prev_button);
	DDX_Control(pDX, IDC_BUTTON_NEXT, _next_button);
	DDX_Control(pDX, IDC_BUTTON_NEW, _new_button);
	DDX_Control(pDX, IDOK, _return_button);
	DDX_Control(pDX, IDC_STATIC_CHOICE_LIST_LABEL, _choice_list_label);
}

BEGIN_MESSAGE_MAP(CQuestionEditorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ADD_QUESTION, &CQuestionEditorDlg::OnBnClickedButtonAddQuestion)
	ON_EN_CHANGE(IDC_EDIT_QUESTION, &CQuestionEditorDlg::OnEnChangeEditQuestion)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CQuestionEditorDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_PREV, &CQuestionEditorDlg::OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_DELETE_QUESTION, &CQuestionEditorDlg::OnBnClickedDeleteQuestion)
END_MESSAGE_MAP()


// CQuestionEditorDlg ��Ϣ�������

BOOL CQuestionEditorDlg::OnInitDialog()
{
	ASSERT(_scale);

	__super::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	if (_scale->IsSameChoice())
	{
		Shrink();
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CQuestionEditorDlg::Shrink()
{
	_choice_list.ShowWindow(SW_HIDE);
	_choice_list_label.ShowWindow(SW_HIDE);

	CRect label_rect;
	_choice_list_label.GetWindowRect(&label_rect);
	ScreenToClient(&label_rect);

	MoveButtonUp(_delete_button, label_rect.top);
	MoveButtonUp(_new_button, label_rect.top);
	MoveButtonUp(_prev_button, label_rect.top);
	MoveButtonUp(_next_button, label_rect.top);
	MoveButtonUp(_return_button, label_rect.top);

	CRect markrect;
	CRect dlgrect;
	CRect clientrect;

	GetClientRect(&clientrect);  // client area of the dialog
	GetWindowRect(&dlgrect);	  // rectangle of the dialog window

								  // get height of the title bar
								  //int offset = dlgrect.Width() - clientrect.right ;
	int offset = dlgrect.Height() - clientrect.bottom;

	_delete_button.GetWindowRect(&markrect);
	ScreenToClient(&markrect);

	// calculate the new rectangle of the dialog window
	//dlgrect.right = dlgrect.left + offset + (expand ? (markrect.right + 10) : markrect.left) ;
	dlgrect.bottom = dlgrect.top + offset + markrect.bottom + 10;

	MoveWindow(dlgrect.left, dlgrect.top, dlgrect.Width(), dlgrect.Height());
}

void CQuestionEditorDlg::MoveButtonUp(CButton& button, 
	unsigned int y_pos)
{
	CRect rect;
	button.GetWindowRect(rect);
	ScreenToClient(rect);

	rect.bottom = y_pos + rect.Height();
	rect.top = y_pos;
	button.MoveWindow(rect);
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CQuestionEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		__super::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CQuestionEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQuestionEditorDlg::OnBnClickedButtonAddQuestion()
{
	PsiScaleQuestion new_question;

	_scale->AddQuestion(new_question);
	_current_question = _scale->GetQuestionCount() - 1;
	UpdateUi();
	UpdateData(FALSE);
}

void CQuestionEditorDlg::OnBnClickedButtonNew()
{
}

void CQuestionEditorDlg::UpdateUi()
{
	UpdateData();

	if (!_scale)
		return;

	auto question = _scale->GetQuestion(_current_question);

	_question_text = question.GetText();
	if (!_scale->IsSameChoice())
	{
		// ���µ�ǰ�����ѡ��

	}

	UpdateData(FALSE);
}

void CQuestionEditorDlg::OnEnChangeEditQuestion()
{
	ASSERT(_scale);
	ASSERT(_current_question < int(_scale->GetQuestionCount()) && _current_question >= 0);

	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData();

	_scale->Question(_current_question).SetText(_question_text);
}


void CQuestionEditorDlg::OnBnClickedButtonNext()
{
	// TODO: Add your control notification handler code here
}


void CQuestionEditorDlg::OnBnClickedButtonPrev()
{
	// TODO: Add your control notification handler code here
}


void CQuestionEditorDlg::OnBnClickedDeleteQuestion()
{
	// TODO: Add your control notification handler code here
}
