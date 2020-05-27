#ifndef _KEYCONV_
#define _KEYCONV_

#ifndef _WINDOWS_
#include <Windows.h>
#endif

/*
 resources:
	 https://docs.microsoft.com/en-us/windows/win32/learnwin32/keyboard-input
	 https://docs.microsoft.com/it-it/windows/win32/inputdev/virtual-key-codes
*/

// converts a virtual key in a valid unicode char
CONST WCHAR* vkConvert(UINT vk, UINT sc){

	//special key
	switch (vk) {
	case VK_SPACE:  return L" ";
	case VK_RETURN: return L"\n";
	case VK_ESCAPE: return L"<esc>";
	case VK_BACK:	return L"<canc>";	
	default:
		break;
	}

	GetKeyState(VK_CAPITAL);
	GetAsyncKeyState(VK_LSHIFT);
	GetAsyncKeyState(VK_RSHIFT);

	HKL layout = GetKeyboardLayout(0);
	BYTE kbState[256];
	static WCHAR key[2];

	GetKeyboardState(kbState);
	int r  = ToUnicodeEx(vk, sc, kbState, key, 1, 0, layout);

	key[1] = '\0';
	return r > 0 ? key : "";
}


#endif
