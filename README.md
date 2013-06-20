xGui
====
Simple windows GUI framework for make some base standard controls in pure C/Win32API.


Example
====

	int main(int argc, char ** argv)
	{
	    struct xGui * wnd, * btn;
	    wnd = xGui_New(-1, -1, 600, 400, "Window Title");
	    btn = xGui_Add_Butn(GBox_PHP, 50, 50, 80, 40, _T("Button A"));
	    //Butn_On_Click(btn, Butn_A_On_Click);
	
	    return xGui_Msg_Loop();
	}
