#include "pch.h"
#include "framework.h"
#include "MFCApplication2.h"
#include "MFCApplication2Dlg.h"
#include "afxdialogex.h"
#include <sstream>
#include <math.h>
#include <iostream>

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Calculator::Action Calculator::get_LastInput() const
{

	return m_actions.size() <= 0 ? Action{ ActionType::None, 0.0 } : m_actions.back();
}

void Calculator::reset()
{
	m_leftExpression.reset();
	m_leftTerm.reset();
	m_actions.clear();
}

bool Calculator::is_Operation(ActionType action) const
{
		return (action == ActionType::Plus || action == ActionType::Minus ||
		action == ActionType::Multiply || action == ActionType::Divide ||
		action == ActionType::Equals   || action == ActionType::Power ||
		action == ActionType::Remainder);                                 
}

bool Calculator::is_Term(ActionType action) const
{

	return (action == ActionType::Multiply || action == ActionType::Divide || action == ActionType::Power || action == ActionType::Remainder);
}


bool Calculator::is_Expression(ActionType action) const
{

	return (action == ActionType::Plus || action == ActionType::Minus ); 
}


Calculator::ActionType Calculator::getLastOperation()
{

	for (auto op = m_actions.rbegin(); op != m_actions.rend(); ++op)
		{

		if (is_Operation(op->actionType))
			return op->actionType;
		}
	return ActionType::None;
}

double Calculator::get_CurrentResult() const

{

	return m_leftExpression.hasValue() ? m_leftExpression.getValue() : m_leftTerm.getValue();
}

bool Calculator::addInput(const Action& input)


{
	const Calculator::Action lastInput = get_LastInput();


	if (input.actionType == ActionType::Number)
	{

		if (lastInput.actionType != ActionType::Number)
			m_actions.push_back(input);
	}

	else if (is_Operation(input.actionType))

	{

		if (lastInput.actionType == ActionType::Number)
		{
			ActionType lastOperation = getLastOperation();
			switch (lastOperation)
			{
			case ActionType::Plus:

				if (is_Expression(input.actionType) || input.actionType == ActionType::Equals)
				{
					m_leftExpression.add(lastInput.value);
					m_leftTerm.reset();
				}

				else if (is_Term(input.actionType))
				{
					m_leftTerm.set(lastInput.value);
				}

				break;
			case ActionType::Minus:


			if (is_Expression(input.actionType) || input.actionType == ActionType::Equals)
				{
				m_leftExpression.add(-lastInput.value);
				m_leftTerm.reset();
				}

				else if (is_Term(input.actionType))
				{
				m_leftTerm.set(-lastInput.value);
				}

				break;
				case ActionType::Multiply:

				if (is_Expression(input.actionType) || input.actionType == ActionType::Equals)
				{
					m_leftExpression.add(m_leftTerm.getValue() * lastInput.value);
					
					m_leftTerm.reset();
				}

				else if (is_Term(input.actionType))

					m_leftTerm.multiplyBy(lastInput.value);
				break;

				case ActionType::Divide:

				if (is_Expression(input.actionType) || input.actionType == ActionType::Equals)
				{
					if (lastInput.value == 0.0)
					{
						Calculator_Exception divByZeroException("Error!! Can't Divide By Zero",
							Calculator_Exception::ExceptionType::DividedByZero);
						throw divByZeroException;
					}

					else
					{
						m_leftExpression.add(m_leftTerm.getValue() / lastInput.value);
						m_leftTerm.reset();
					}
				}

				else if (is_Term(input.actionType)) 

					// "3 / 4 x"
					m_leftTerm.multiplyBy(1.0 / lastInput.value);
					break;
				
					case ActionType::Power:

					if (is_Expression(input.actionType) || input.actionType == ActionType::Equals)
				{
					// "3 x 4 +", "3 x 4 ="

					m_leftExpression.add(pow(m_leftTerm.getValue() , lastInput.value));


					m_leftTerm.reset();
				}

					else if (is_Term(input.actionType))

					// "3 x 4 x"

					m_leftTerm.multiplyBy(lastInput.value);
					break;
					case ActionType::Remainder:

					if (is_Expression(input.actionType) || input.actionType == ActionType::Equals)
				{
					// "3 x 4 +", "3 x 4 ="

					m_leftExpression.add(int(m_leftTerm.getValue()) % int(lastInput.value));
					m_leftTerm.reset();
				}
					else if (is_Term(input.actionType))

						// "3 x 4 x"

					m_leftTerm.multiplyBy(lastInput.value);
					break;
				
					case ActionType::Equals: 

					if (is_Term(input.actionType))

					{
					m_leftExpression.reset();
					m_leftTerm.set(lastInput.value);
					}

					else if (is_Expression(input.actionType))
					{
					m_leftExpression.set(lastInput.value);
					m_leftTerm.reset();
					}

					break;

				case ActionType::None:

					if (is_Term(input.actionType))
				{
					m_leftExpression.reset();
					m_leftTerm.set(lastInput.value);
				}
					else if (is_Expression(input.actionType))
				{
					m_leftExpression.set(lastInput.value);
					m_leftTerm.reset();
				}
					break;
			}
				m_actions.push_back(input);

					return true;
		}
	}
				return false;
}



class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

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

CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent )
	: CDialogEx(IDD_MFCAPPLICATION2_DIALOG, pParent)
	, m_output(_T(""))
{

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_historyBkColor = RGB(255, 255, 255);
	m_historyBkBrush = std::make_unique<CBrush>(m_historyBkColor);

}

void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_output);
	DDX_Control(pDX, IDC_EDIT1, m_editResult);
	DDX_Control(pDX, IDC_BUTTON_0, m_button0);
	DDX_Control(pDX, IDC_BUTTON_1, m_button1);
	DDX_Control(pDX, IDC_BUTTON_2, m_button2);
	DDX_Control(pDX, IDC_BUTTON_3, m_button3);
	DDX_Control(pDX, IDC_BUTTON_4, m_button4);
	DDX_Control(pDX, IDC_BUTTON_5, m_button5);
	DDX_Control(pDX, IDC_BUTTON_6, m_button6);
	DDX_Control(pDX, IDC_BUTTON_7, m_button7);
	DDX_Control(pDX, IDC_BUTTON_8, m_button8);
	DDX_Control(pDX, IDC_BUTTON_9, m_button9);
	DDX_Control(pDX, IDC_BUTTON_PLUS, m_buttonPlus);
	DDX_Control(pDX, IDC_BUTTON_MINUS, m_buttonMinus);
	DDX_Control(pDX, IDC_BUTTON_MULTIPLY, m_buttonMultiply);
	DDX_Control(pDX, IDC_BUTTON_DIVIDE, m_buttonDivide);
	DDX_Control(pDX, IDC_BUTTON_C, m_buttonC);
	DDX_Control(pDX, IDC_BUTTON_EQUAL, m_buttonEqual);
	DDX_Control(pDX, IDC_BUTTON_POWER, m_buttonPower);
	DDX_Control(pDX, IDC_BUTTON_REMAINDER, m_buttonRemainder);
	DDX_Text(pDX, IDC_EDIT_HISTORY, m_historyText);
	DDX_Control(pDX, IDC_EDIT_HISTORY, m_editHistory);
}

BEGIN_MESSAGE_MAP(CMFCApplication2Dlg, CDialogEx)

	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_0, &CMFCApplication2Dlg::OnBnClickedButton0)
	ON_BN_CLICKED(IDC_BUTTON_1, &CMFCApplication2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, &CMFCApplication2Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, &CMFCApplication2Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, &CMFCApplication2Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_5, &CMFCApplication2Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_6, &CMFCApplication2Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_7, &CMFCApplication2Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON_8, &CMFCApplication2Dlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON_9, &CMFCApplication2Dlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON_PLUS, &CMFCApplication2Dlg::OnBnClickedButtonPlus)
	ON_BN_CLICKED(IDC_BUTTON_MINUS, &CMFCApplication2Dlg::OnBnClickedButtonMinus)
	ON_BN_CLICKED(IDC_BUTTON_MULTIPLY, &CMFCApplication2Dlg::OnBnClickedButtonMultiply)
	ON_BN_CLICKED(IDC_BUTTON_DIVIDE, &CMFCApplication2Dlg::OnBnClickedButtonDivide)
	ON_BN_CLICKED(IDC_BUTTON_C, &CMFCApplication2Dlg::OnBnClickedButtonC)
	ON_BN_CLICKED(IDC_BUTTON_EQUAL, &CMFCApplication2Dlg::OnBnClickedButtonEqual)
	ON_BN_CLICKED(IDC_BUTTON_POWER, &CMFCApplication2Dlg::OnBnClickedButtonPower)
	ON_BN_CLICKED(IDC_BUTTON_REMAINDER, &CMFCApplication2Dlg::OnBnClickedButtonRemainder)
	ON_WM_CTLCOLOR()
	
	ON_EN_CHANGE(IDC_EDIT_HISTORY, &CMFCApplication2Dlg::OnEnChangeEditHistory)
	ON_BN_CLICKED(IDC_BUTTON_10, &CMFCApplication2Dlg::OnBnClickedButton10)
END_MESSAGE_MAP()

BOOL CMFCApplication2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Adding "About..." list 

	
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		


	m_font.CreateFont(32, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET,
		0, 0, 0, 0, _T("Microsoft Sans Serif"));

	m_historyFont.CreateFont(18, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET,
		0, 0, 0, 0, _T("Microsoft Sans Serif"));

	m_editResult.SetFont(&m_font);
	m_button0.SetFont(&m_font);
	m_button1.SetFont(&m_font);
	m_button2.SetFont(&m_font);
	m_button3.SetFont(&m_font);
	m_button4.SetFont(&m_font);
	m_button5.SetFont(&m_font);
	m_button6.SetFont(&m_font);
	m_button7.SetFont(&m_font);
	m_button8.SetFont(&m_font);
	m_button9.SetFont(&m_font);
	m_buttonPlus.SetFont(&m_font);
	m_buttonMinus.SetFont(&m_font);
	m_buttonMultiply.SetFont(&m_font);
	m_buttonDivide.SetFont(&m_font);
	m_buttonC.SetFont(&m_font);
	m_buttonEqual.SetFont(&m_font);
	m_buttonPower.SetFont(&m_font);
	m_buttonRemainder.SetFont(&m_font);
	

	return TRUE;  
}

void CMFCApplication2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CMFCApplication2Dlg::OnPaint()
{
		if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}

		else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CMFCApplication2Dlg::OnQueryDragIcon()
{

	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCApplication2Dlg::reset()
{

	m_calculator.reset();
	m_errorInput = false;
	m_firstDigitEntered = FALSE;
	reset_Output();
	m_historyText = "";
	UpdateData(FALSE);
}

void CMFCApplication2Dlg::reset_Output()
{

	m_output = m_outputResetString;
}

void CMFCApplication2Dlg::addDigit(char digit)
{
	
	if (m_errorInput)
		return;
	
	

	UpdateData();
	
	if (m_calculator.is_Operation(m_calculator.get_LastInput().actionType) &&
		!m_firstDigitEntered)
	{
		m_output = digit;
	}
	else
	{
		if (m_output == m_outputResetString && digit == '0')
			return; 
		else if (m_output == m_outputResetString)
			m_output = digit; 
		else
			m_output += digit; 
	}
	
	m_firstDigitEntered = TRUE;
	UpdateData(FALSE);
}


void CMFCApplication2Dlg::OnBnClickedButton0()
{
	addDigit('0');
}

void CMFCApplication2Dlg::OnBnClickedButton1()
{
	addDigit('1');
}

void CMFCApplication2Dlg::OnBnClickedButton2()
{
	addDigit('2');
}
void CMFCApplication2Dlg::OnBnClickedButton10()
{
	addDigit('.');
}



void CMFCApplication2Dlg::OnBnClickedButton4()
{
	addDigit('4');
}

void CMFCApplication2Dlg::OnBnClickedButton5()
{
	addDigit('5');
}

void CMFCApplication2Dlg::OnBnClickedButton6()
{
	addDigit('6');
}

void CMFCApplication2Dlg::OnBnClickedButton7()
{
	addDigit('7');
}

void CMFCApplication2Dlg::OnBnClickedButton8()
{
	addDigit('8');
}

void CMFCApplication2Dlg::OnBnClickedButton9()
{
	addDigit('9');
}
void CMFCApplication2Dlg::OnBnClickedButton3()
{
	addDigit('3');
}

void CMFCApplication2Dlg::createHistoryText()
{

	m_historyText = "";
	int historySize = m_calculator.get_ActionsSize();

	for (int i = 0; i < historySize; ++i)

	{
			switch (m_calculator.get_Action(i).actionType)
		{
			case Calculator::ActionType::Number:
		{
			CString strNumber;
			std::stringstream ss;
			ss << m_calculator.get_Action(i).value;
			std::string stdNum = ss.str();

			strNumber = stdNum.c_str();

				if (m_historyText.IsEmpty())
				m_historyText += strNumber;

				else
				m_historyText += (CString(" ") + strNumber);
		}
			break;

			case Calculator::ActionType::Plus:
			m_historyText += " +";

			break;

			case Calculator::ActionType::Divide:
			m_historyText += " /";

			break;

			case Calculator::ActionType::Minus:
			m_historyText += " -";

			break;

			case Calculator::ActionType::Multiply:
			m_historyText += " x";

			break;

			case Calculator::ActionType::Equals:
			m_historyText += " =";

			break;

			case Calculator::ActionType::Power:
			m_historyText += " pow";

			break;

			case Calculator::ActionType::Remainder:
			m_historyText += " Remainder";

			break;

			case Calculator::ActionType::None:

			break;
		}
	}
}



void CMFCApplication2Dlg::do_Operation(Calculator::ActionType operation, bool handleNumber)
{
		if (m_errorInput)
		return;

	UpdateData();
	Calculator::Action input;
		if (handleNumber)
	{
		input.actionType = Calculator::ActionType::Number;
		input.value = _wtof(m_output);
		m_calculator.addInput(input);
	}
	input.actionType = operation;
	m_errorInput = false;
		try
	{
			if (m_calculator.addInput(input))
		{
				if (!m_calculator.has_LeftTermValue() || !m_calculator.has_LeftExpressionValue())
			{
				std::stringstream ss;
				ss << m_calculator.get_CurrentResult();
				std::string curResult = ss.str();

				m_output = "";
				m_output += curResult.c_str();
				UpdateData(FALSE);
			}
		}
				else 
				AfxMessageBox(_T("Error: An unknown operation."));
	}
				catch (std::exception& e)
				{
				m_output = e.what();
				m_firstDigitEntered = FALSE;
				m_errorInput = true;
	}
	createHistoryText();

	if (operation == Calculator::ActionType::Equals)
	{

		if (m_errorInput)
			m_historyText += " ";
		m_historyText += m_output;
	}
	UpdateData(FALSE);
	m_firstDigitEntered = FALSE;
}

void CMFCApplication2Dlg::OnBnClickedButtonPlus()
{
	do_Operation(Calculator::ActionType::Plus);
}

void CMFCApplication2Dlg::OnBnClickedButtonMinus()
{
	do_Operation(Calculator::ActionType::Minus);
}

void CMFCApplication2Dlg::OnBnClickedButtonEqual()
{
	
	do_Operation(Calculator::ActionType::Equals);
}

void CMFCApplication2Dlg::OnBnClickedButtonC()
{
	reset();
}

void CMFCApplication2Dlg::OnBnClickedButtonDivide()
{
	do_Operation(Calculator::ActionType::Divide);
}

void CMFCApplication2Dlg::OnBnClickedButtonMultiply()
{
	do_Operation(Calculator::ActionType::Multiply);
}

void CMFCApplication2Dlg::OnBnClickedButtonPower()
{
	do_Operation(Calculator::ActionType::Power);
}

void CMFCApplication2Dlg::OnBnClickedButtonRemainder()
{
	do_Operation(Calculator::ActionType::Remainder);
}

HBRUSH CMFCApplication2Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	
	switch (nCtlColor) 
	{
	case CTLCOLOR_EDIT:

		if (pWnd->GetDlgCtrlID() == IDC_EDIT_HISTORY)
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkColor(m_historyBkColor);
			return (HBRUSH)(m_historyBkBrush->GetSafeHandle());
		}
	}

	return hbr;
}

void CMFCApplication2Dlg::OnEnChangeEditHistory()
{

}



