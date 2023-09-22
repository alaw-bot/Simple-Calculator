
#include "afxwin.h"
#include <memory>
#include <vector>
#include <exception>
#include <string>
#include <math.h>

class Calculator_Exception : public std::exception
{
public:
	enum class ExceptionType : char { None = 0, DividedByZero };

	Calculator_Exception(std::string textWhat, ExceptionType exceptionType) :
		m_textWhat(textWhat), m_exceptionType(exceptionType)
	{
	}
	virtual const char* what() const throw()
	{
		return m_textWhat.c_str();
	}
private:
	std::string m_textWhat;

	ExceptionType m_exceptionType{ ExceptionType::None };
};

class Calculator
{
public:
	enum class ActionType : char { Number,  Plus, Minus, Multiply, Divide, Equals, Power, Remainder, None };
	struct Action
	{
		ActionType actionType;
		double value;
	};

	void reset();
	bool addInput(const Action& input);
	Action get_LastInput() const;
	double get_CurrentResult() const;
	bool has_LeftTermValue() const { return m_leftTerm.hasValue(); }
	bool has_LeftExpressionValue() const { return m_leftExpression.hasValue(); }
	bool is_Operation(ActionType action) const;
	int get_ActionsSize() { return static_cast<int>(m_actions.size()); }
	const Action& get_Action(int i) { return m_actions.at(i); }
private:
	bool is_Term(ActionType action) const;
	bool is_Expression(ActionType action) const;
	ActionType getLastOperation();

	class LeftExpression

	{

		public:
		void reset();
		void set(double value);
		void add(double value);
		double getValue() const { return m_value; }
		bool hasValue() const { return m_hasValue; }
		private:
		bool m_hasValue = false;
		double m_value = 0.0;
	};

		class LeftTerm
	{
		public:
		void reset();
		void set(double value);
		void multiplyBy(double value);
		double getValue() const { return m_value; }
		bool hasValue() const { return m_hasValue; }
		private:
		bool m_hasValue = false;
		double m_value;
	};

		std::vector<Action> m_actions; 
		LeftExpression m_leftExpression;
		LeftTerm m_leftTerm;
	};

inline void Calculator::LeftExpression::reset()
{
	m_hasValue = false;
	m_value = 0.0;
}

inline void Calculator::LeftExpression::set(double value)
{
	m_hasValue = true;
	m_value = value;
}

inline void Calculator::LeftExpression::add(double value)
{
	set(m_value + value);
}

inline void Calculator::LeftTerm::reset()
{
	m_hasValue = false;
	m_value = 0.0;
}

inline void Calculator::LeftTerm::set(double value)
{
	m_hasValue = true;
	m_value = value;
}

inline void Calculator::LeftTerm::multiplyBy(double value)
{
	set(m_value * value);
}

class CMFCApplication2Dlg : public CDialogEx
{
public:
	CMFCApplication2Dlg(CWnd* pParent = nullptr);	

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:

	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton0();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButtonPlus();
	afx_msg void OnBnClickedButtonMinus();
	afx_msg void OnBnClickedButtonDivide();
	afx_msg void OnBnClickedButtonMultiply();
	afx_msg void OnBnClickedButtonC();
	afx_msg void OnBnClickedButtonEqual();
	afx_msg void OnBnClickedButtonPower();  
	afx_msg void OnBnClickedButtonRemainder(); 
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);



private:
	bool m_errorInput = false;
	const CString m_outputResetString{ "0" };
	void reset_Output();
	void reset();
	void addDigit(char digit);
	void do_Operation(Calculator::ActionType operation, bool handleNumber = true);
	void createHistoryText();
	Calculator m_calculator;
	CString m_output;
	CFont m_font;
	CFont m_historyFont;
	BOOL m_firstDigitEntered = FALSE;
	CEdit m_editResult;
	CButton m_button0;
	CButton m_button1;
	CButton m_button2;
	CButton m_button3;
	CButton m_button4;
	CButton m_button5;
	CButton m_button6;
	CButton m_button7;
	CButton m_button8;
	CButton m_button9;
	CButton m_buttonPlus;
	CButton m_buttonEqual;
	CButton m_buttonC;
	CButton m_buttonMinus;
	CButton m_buttonMultiply;
	CButton m_buttonDivide;
	CButton m_buttonPower;
	CButton m_buttonRemainder;
	CEdit m_editHistory;
	CString m_historyText;
	std::unique_ptr<CBrush> m_historyBkBrush;
	COLORREF m_historyBkColor;

	public:

	afx_msg void OnEnChangeEditHistory();
	afx_msg void OnBnClickedButton10();
};