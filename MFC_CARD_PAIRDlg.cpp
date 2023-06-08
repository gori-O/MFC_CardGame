
// MFC_CARD_PAIRDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFC_CARD_PAIR.h"
#include "MFC_CARD_PAIRDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCCARDPAIRDlg 대화 상자



CMFCCARDPAIRDlg::CMFCCARDPAIRDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_CARD_PAIR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCCARDPAIRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_time_progress);
}

BEGIN_MESSAGE_MAP(CMFCCARDPAIRDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_HINT_BTN, &CMFCCARDPAIRDlg::OnBnClickedHintBtn)
END_MESSAGE_MAP()


// CMFCCARDPAIRDlg 메시지 처리기

BOOL CMFCCARDPAIRDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	CString str;
	for (int i = 0; i < 19; i++)
	{
		str.Format(L".\\card_image\\%03d.bmp", i);
		m_card_img[i].Load(str);
	}
	
	StartGame();
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCCARDPAIRDlg::OnPaint()
{

	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CString str;
		int card_idx = 0;
		for (int i = 0; i < 36; i++) {
			if (m_table[i] == 0) continue;
			if (m_view_flag == 1) card_idx = m_table[i];

			m_card_img[card_idx].Draw(dc, (i % 6) * 36, (i / 6) * 56);
			//
			str.Format(L"%d", m_table[i]);
			//dc.TextOutW(5 + (i % 6) * 36, 5 + (i / 6) * 56, str);				// 화투 번호 표시 ; 개발테스트용 ; 나중에 없애야함.
			// 테스트코드
		}
		//CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCCARDPAIRDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCCARDPAIRDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1) {
		KillTimer(1);
		m_view_flag = 0;
		Invalidate();
	}
	else if (nIDEvent == 2) {
		KillTimer(2);
		m_view_flag = 0;
		Invalidate();
	}
	else if (nIDEvent == 10) {
		int num = m_time_progress.GetPos() - 1;
		if (num > 0) { m_time_progress.SetPos(num); }
		else {
			EndOfGame(L"시간초과! 게임이 종료되었습니다.");
		}
	}
	else
		CDialogEx::OnTimer(nIDEvent);
}


void CMFCCARDPAIRDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_view_flag != 0) return;
	int x = point.x / 36;
	int y = point.y / 56;

	if (x < 6 && y < 6) {
		int sel_pos = y * 6 + x;		// 0 ~ 35
		if (m_table[sel_pos] == 0) return;					// 사라진 카드 선택 안되도록 막는다.

		if (m_first_pos == -1) m_first_pos = sel_pos;		// 첫번째 카드 선택
		else {
			if (m_first_pos == sel_pos) return;				// 같은 위치 카드 선택 안되도록 막는다.

			if (m_table[m_first_pos] == m_table[sel_pos]) {
				m_table[m_first_pos] = m_table[sel_pos] = 0;
				Invalidate();
				
				m_clr_count++;
				if (m_clr_count >= 18) {
					// game clear!!
					EndOfGame(L"성공! 축하합니다~~!");
				}
			}
			else {
				m_view_flag = 2;
				SetTimer(2, 1000, NULL);
			}
			m_first_pos = -1;
		}
		CClientDC dc(this);
		m_card_img[m_table[sel_pos]].Draw(dc, x * 36, y * 56);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMFCCARDPAIRDlg::OnBnClickedHintBtn()
{
	int num = GetDlgItemInt(IDC_HINT_BTN);
	if (num > 0) {
		SetDlgItemInt(IDC_HINT_BTN, num - 1);
		m_view_flag = 1;
		Invalidate();
		SetTimer(1, 3000, NULL);
	}
}


void CMFCCARDPAIRDlg::EndOfGame(const wchar_t* ap_ment)
{
	KillTimer(10);
	if (IDOK == MessageBox(L"다시 게임을 하시겠습니까?", ap_ment, MB_OKCANCEL | MB_ICONQUESTION)) {
		StartGame();
		Invalidate();
	}
	else {
		EndDialog(IDOK);
	}
}

void CMFCCARDPAIRDlg::StartGame()
{
	srand((unsigned int)time(NULL));
	m_time_progress.SetRange(0, 60);
	m_time_progress.SetPos(60);

	SetDlgItemInt(IDC_HINT_BTN, 3);
	for (int i = 0; i < 18; i++) {
		m_table[i] = i + 1;				// 1 ~ 18
		m_table[18 + i] = i + 1;		// 1 ~ 18
	}

	// 카드 랜덤으로 섞기
	int first, second, tmp;
	for (int i = 0; i < 100; i++) {
		first = rand() % 36;
		second = rand() % 36;
		if (first != second) {
			tmp = m_table[first];
			m_table[first] = m_table[second];
			m_table[second] = tmp;
		}
	}

	SetTimer(1, 3000, NULL);
	SetTimer(10, 1000, NULL);
}