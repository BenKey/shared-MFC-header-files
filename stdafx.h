/*
Copyright © 2016 - 2026 by Benilda Key

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

// Exclude rarely-used stuff from Windows headers
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#ifndef VC_EXTRALEAN
#  define VC_EXTRALEAN
#endif

#define FKG_FORCED_USAGE

#include "targetver.h"

#ifdef _DEBUG
#  define DBG 1
#else
#  define OFFICIAL_BUILD 1
#endif

// some CString constructors will be explicit
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

// MFC core and standard components
#include <afxwin.h>
// MFC extensions
#include <afxext.h>
// MFC Automation classes
#include <afxdisp.h>
// MFC Standard dialogs
#include <afxdlgs.h>
// CDialogEx class
#include <afxdialogex.h>

#ifndef _AFX_NO_OLE_SUPPORT
   // MFC OLE support
#  include <afxole.h>
   // OLE 2 Version Number Info
#  include <ole2ver.h>
   // MFC support for Internet Explorer 4 Common Controls
#  include <afxdtctl.h>
#endif

#ifndef _AFX_NO_AFXCMN_SUPPORT
   // MFC support for Windows Common Controls
#  include <afxcmn.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

// MFC Private Classes
#include <afxpriv.h>
// Windows 32-Bit Registry API
#include <winreg.h>

// MFC support for Visual Managers
#include <afxvisualmanager.h>
#include <afxvisualmanagerwindows.h>
// MFC support for the Shell Manager
#include <afxshellmanager.h>
// MFC RichEdit classes
#include <afxrich.h>
// MFC support for List Control View and Tree Control View
#include <afxcview.h>
// CWinAppEx class
#include <afxwinappex.h>

#include <Shlobj.h>
#include <shlwapi.h>

#include <cppcorecheck/warnings.h>
#include <CodeAnalysis/Warnings.h>

#include <cinttypes>
#include <climits>
#include <clocale>
#include <cstdint>
#include <cstdio>

#ifdef _WIN64
#  define SIZE_T_C(x) (x ## ULL)
#  define DWORD_PTR_C(x) (x ## ULL)
#else
#  define SIZE_T_C(x) (x ## U)
#  define DWORD_PTR_C(x) (x ## U)
#endif

#if defined(_UNICODE)
#  if defined(_M_IX86)
#    pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  elif defined(_M_X64)
#    pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  else
#    pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  endif
#endif
