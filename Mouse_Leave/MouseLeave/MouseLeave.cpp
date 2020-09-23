#include "stdafx.h"

#include <Commctrl.h>
#pragma comment (lib,"Comctl32")

#include <vector>

typedef struct _SSubClass
{
	HWND	_hwnd;
	WNDPROC	_ex_proc;
	bool	_isin;
	void(__stdcall *_func)(HWND hHwnd);
	TRACKMOUSEEVENT _tme;

} SSubClass,*LPSSubClass;

std::vector<LPSSubClass>subclass;

LRESULT CALLBACK MouseLeaveProc(HWND h,UINT m,WPARAM w,LPARAM l)
{
	for(UINT i = 0;i < subclass.size();i++)
		if(subclass[i]->_hwnd == h)break;

	if(i == subclass.size())return 0;

	switch(m)
	{
	case WM_MOUSEMOVE:
		if(subclass[i]->_isin)break;
		subclass[i]->_isin = true;
		_TrackMouseEvent(&subclass[i]->_tme);
		return 0;
	case WM_MOUSELEAVE:
		subclass[i]->_isin = false;
		subclass[i]->_func(h);
		return 0;
	case WM_DESTROY:
		SetWindowLong(subclass[i]->_hwnd,GWL_WNDPROC,(LONG)subclass[i]->_ex_proc);
		delete subclass[i];
		subclass.erase(subclass.begin() + i);
		return 0;
	}

	return CallWindowProc(subclass[i]->_ex_proc,h,m,w,l);
}

BOOL APIENTRY MouseLeave_AddWindow(HWND hHwnd,void(__stdcall *Func)(HWND hHwnd))
{
	if(hHwnd == NULL || Func == NULL)return FALSE;

	for(UINT i = 0;i < subclass.size();i++)
		if(subclass[i]->_hwnd == hHwnd)return FALSE;

	LPSSubClass s	= new SSubClass;
	s->_hwnd		= hHwnd;
	s->_isin		= false;
	s->_func		= Func;
	s->_tme.cbSize	= sizeof(TRACKMOUSEEVENT);
	s->_tme.dwFlags	= TME_LEAVE;
	s->_tme.hwndTrack= hHwnd;
	s->_ex_proc		= (WNDPROC)SetWindowLong(hHwnd,GWL_WNDPROC,(LONG)MouseLeaveProc);

	subclass.push_back(s);

	return TRUE;
}

VOID APIENTRY MouseLeave_Clear()
{
	for(UINT i = 0;i < subclass.size();i++)
	{
		SetWindowLong(subclass[i]->_hwnd,GWL_WNDPROC,(LONG)subclass[i]->_ex_proc);
		delete subclass[i];
	}

	subclass.clear();
}

BOOL APIENTRY DllMain( HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	if(ul_reason_for_call == DLL_PROCESS_DETACH)
		MouseLeave_Clear();

    return TRUE;
}
