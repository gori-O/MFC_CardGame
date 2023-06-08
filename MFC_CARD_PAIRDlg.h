
// MFC_CARD_PAIRDlg.h: 헤더 파일
//

#pragma once


// CMFCCARDPAIRDlg 대화 상자
class CMFCCARDPAIRDlg : public CDialogEx
{
private:
	CImage m_card_img[19];	// 0 : 뒷면, 1 ~ 18 카드면
	char m_table[36];
	char m_view_flag = 1;
	char m_first_pos = -1;
	int m_clr_count;
// 생성입니다.
public:
	CMFCCARDPAIRDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	void CMFCCARDPAIRDlg::EndOfGame(const wchar_t* ap_ment);
	void CMFCCARDPAIRDlg::StartGame();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_CARD_PAIR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedHintBtn();
	CProgressCtrl m_time_progress;
};
