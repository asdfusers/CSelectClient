#pragma once
class CDlgBox
{
struct Login
{
	wchar_t ID[10];
	wchar_t password[15];
};
public:
	CDlgBox();
	~CDlgBox();
	
	void LoginDlg();
	void RoomListDlg();
	static BOOL CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp);
	static BOOL CALLBACK DialogProc2(HWND hwnd, UINT message, WPARAM wp, LPARAM lp);
	Login& getLog() { return log; }
	int getNumber();

private:
	static Login log;
	static wchar_t number[10];
};

