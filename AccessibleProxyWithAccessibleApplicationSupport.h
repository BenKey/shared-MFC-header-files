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

#ifndef __AccessibleProxyWithAccessibleApplicationSupport_H__
#define __AccessibleProxyWithAccessibleApplicationSupport_H__

#pragma once

#include <array>
#include <atldef.h>

#if !defined(_ATL_USE_WINAPI_FAMILY_DESKTOP_APP)
#  error This file is not compatible with the current WINAPI_FAMILY.
#endif

#include <oleacc.h>
#include <atliface.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlacc.h>

#include <boost/numeric/conversion/cast.hpp>

#include "AccessibleApplicationImpl.h"

#ifndef _ATL_NO_DEFAULT_LIBS
#  pragma comment(lib, "oleacc.lib")
#endif // _ATL_NO_DEFAULT_LIBS

#pragma pack(push, _ATL_PACKING)
namespace ATL {

/* Quick, tell me an easy way to determine what a function that returns as follows
 returns while debugging.

return FunctionThatIsNotWorkingProperly();

It is possible.

1. Set the breakpoint on the return line.
2. Step past the return line.
3. Look in the autos window and hope there is a FunctionThatIsNotWorkingProperly
returned entry.

There is an easier way.

auto ret = FunctionThatIsNotWorkingProperly();
return ret;

Then this can be done with a single step. Set a breakpoint on the return line.

This is an implementation of the IServiceProviderImpl class that uses the second
pattern for the return so I can easily find out why things are not working. */
template<class T>
class ATL_NO_VTABLE DebuggableIServiceProviderImpl: public IServiceProvider
{
public:
    STDMETHOD(QueryService)(_In_ REFGUID guidService, _In_ REFIID riid, _COM_Outptr_ void** ppvObject)
    {
        const auto formatString = L"QueryService returned %#010X.\r\n    guidService: %s\r\n    riid: %s\r\n";
        try
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
            ATLTRACE2(static_cast<int>(atlTraceCOM), 2U, L"IServiceProviderImpl::QueryService\n");
            auto pT = static_cast<T*>(this);
            auto ret = pT->_InternalQueryService(guidService, riid, ppvObject);
            if (SUCCEEDED(ret))
            {
                return ret;
            }
            std::array<wchar_t, MAX_PATH> guidServiceString{};
            auto getString = StringFromGUID2(guidService, guidServiceString.data(),
                boost::numeric_cast<int>(guidServiceString.size()));
            if (getString == 0)
            {
                return ret;
            }
            std::array<wchar_t, MAX_PATH> riidString{};
            getString = StringFromGUID2(riid, riidString.data(), boost::numeric_cast<int>(riidString.size()));
            if (getString == 0)
            {
                return ret;
            }
            CStringW debugMessage;
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
            debugMessage.Format(formatString, ret, guidServiceString.data(), riidString.data());
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
            ATLTRACE2(static_cast<int>(atlTraceCOM), 2U, static_cast<LPCWSTR>(debugMessage));
            return ret;
        }
        catch (boost::numeric::bad_numeric_cast&)
        {
            return E_FAIL;
        }
    }
};

template<class Base>
HRESULT WINAPI AccessibilityCreateInstance(_COM_Outptr_ CComObjectNoLock<Base>** pp) throw()
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    ATLASSERT(pp != nullptr);
    if (pp == nullptr)
    {
        return E_POINTER;
    }
    *pp = nullptr;
    HRESULT hRes = E_OUTOFMEMORY;
    CComObjectNoLock<Base>* p = nullptr;
    // NOLINTNEXTLINE(cert-err09-cpp,cert-err61-cpp,cppcoreguidelines-owning-memory)
    ATLTRY(p = _ATL_NEW CComObjectNoLock<Base>());
    if (p != nullptr)
    {
        p->SetVoid(nullptr);
        p->InternalFinalConstructAddRef();
        hRes = p->_AtlInitialConstruct();
        if (SUCCEEDED(hRes))
        {
            hRes = p->FinalConstruct();
        }
        if (SUCCEEDED(hRes))
        {
            hRes = p->_AtlFinalConstruct();
        }
        p->InternalFinalConstructRelease();
        if (hRes != S_OK)
        {
            // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
            delete p;
            p = nullptr;
        }
    }
    *pp = p;
    return hRes;
}

/*
A version of the IAccessibleProxyImpl class and CAccessibleProxy class that
contains the magical incantations required to add support for the
IAccessibleApplication interface to an MFC application.

I would like to express my heartfelt gratitude to Microsoft for making it
so difficult to obtain the relevant information. As I was on my quest to
discover the arcane secrets one must understand in order to add support for
the IAccessibleApplication interface to an MFC application,  I fell down the
rabbit hole (Alice's Adventures in Wonderland by Charles Lutwidge Dodgson,
a.k.a. Lewis Carroll [https://ebooks.adelaide.edu.au/c/carroll/lewis/alice/])
only to find Cthulhu (The Call of Cthulhu by H. P. Lovecraft
[https://youtu.be/vqIxCqayQok]) waiting for me.

I would also like to express my undying gratitude to Glen Gordon, CTO of VFO,
for helping me to figure out how to do this.

To use these classes, simply override the CWnd::CreateAccessibleProxy function
in your CWnd derived class as follows.

HRESULT CMyWnd::CreateAccessibleProxy(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    ASSERT(pResult != nullptr);
    if (pResult == nullptr)
    {
        return E_POINTER;
    }
    auto ret = CreateAccessibleProxyWithAccessibleApplicationSupportHelper(wParam, lParam, this,
        static_cast<IAccessible*>(reinterpret_cast<void*>(&m_xAccessible)),
        static_cast<IAccessibleServer*>(reinterpret_cast<void*>(&m_xAccessibleServer)), pResult, &m_pProxy);
    return ret;
}

Now, you may be wondering which window you need to do this for. Unfortunately, the
answer is every window that can gain focus if you want JAWS to use the
IAccessibleApplication interface to obtain the name of your application instead of
just using the file name of the executable or DLL.

This is due to what some might consider to be a bug in JAWS.

JAWS uses code like the following to obtain the name of an application via the
IAccessibleApplication interface (note, this is not the actual code since JAWS is
proprietary and I cannot share any JAWS source code with the general public).

// NOLINTNEXTLINE(google-runtime-int)
std::string GetApplicationNameUsingTheIAccessibleApplicationInterface(HWND hwnd, long idObject, long idChild)
{
    CComPtr<IAccessible> acc;
    CComVariant var;
    auto hr = AccessibleObjectFromEvent(hwnd, idObject, idChild, &acc, &var);
    if (hr != S_OK)
    {
        return L"";
    }
    if (!acc)
    {
        return L"";
    }
    CComQIPtr<IServiceProvider> serviceProvider = acc;
    if (!serviceProvider)
    {
        return L"";
    }
    CComQIPtr<IAccessibleApplication> application;
    hr = serviceProvider->QueryService(IID_IAccessible, __uuidof(IAccessibleApplication), reinterpret_cast<void**>(&application));
    if (FAILED(hr))
    {
        return L"";
    }
    if (!application)
    {
        return L"";
    }
    CComBSTR appName;
    hr = application->get_appName(&text);
    if (FAILED(hr))
    {
        return L"";
    }
    return appName.m_str;
}

Now, the key to understanding why it is necessary for any window that can gain
focus to support the IAccessibleApplication interface, you need one additional
piece of information.

This function is called in our [WinEventProc callback function](https://msdn.microsoft.com/en-us/library/windows/desktop/dd373885%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396)
in response to the EVENT_OBJECT_FOCUS event.

This works for Google Chrome, the application we originally added IAccessibleApplication for.
*/

template<class T>
class IAccessibleProxyWithAccessibleApplicationSupportImpl:
    public IAccessible, public IAccessibleProxy, public CAccessibleApplicationImpl
{
public:
    IAccessible* m_pAccessible = nullptr;
    IAccessibleServer* m_pAccessibleServer = nullptr;

    // +++++++++++++++++++++++++++++++++++
    // IAccessible
    // +++++++++++++++++++++++++++++++++++
    HRESULT STDMETHODCALLTYPE get_accParent(_Outptr_ IDispatch** ppdispParent) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (ppdispParent == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->get_accParent(ppdispParent);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE get_accChildCount(_Out_ long* pcountChildren) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pcountChildren == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->get_accChildCount(pcountChildren);
    }
    HRESULT STDMETHODCALLTYPE get_accChild(VARIANT varChild, _Outptr_ IDispatch** ppdispChild) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (ppdispChild == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->get_accChild(varChild, ppdispChild);
    }
    HRESULT STDMETHODCALLTYPE get_accName(VARIANT varChild, _Outptr_result_z_ BSTR* pszName) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pszName == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->get_accName(varChild, pszName);
    }
    HRESULT STDMETHODCALLTYPE get_accValue(VARIANT varChild, _Outptr_result_z_ BSTR* pszValue) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pszValue == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->get_accValue(varChild, pszValue);
    }
    HRESULT STDMETHODCALLTYPE get_accDescription(VARIANT varChild, _Outptr_result_z_ BSTR* pszDescription) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pszDescription == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->get_accDescription(varChild, pszDescription);
    }
    HRESULT STDMETHODCALLTYPE get_accRole(VARIANT varChild, _Out_ VARIANT* pvarRole) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pvarRole == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->get_accRole(varChild, pvarRole);
    }
    HRESULT STDMETHODCALLTYPE get_accState(VARIANT varChild, _Out_ VARIANT* pvarState) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pvarState == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->get_accState(varChild, pvarState);
    }
    HRESULT STDMETHODCALLTYPE get_accHelp(VARIANT varChild, _Outptr_result_z_ BSTR* pszHelp) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pszHelp == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->get_accHelp(varChild, pszHelp);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE get_accHelpTopic(_Outptr_result_z_ BSTR* pszHelpFile, VARIANT varChild, _Out_ long* pidTopic) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pszHelpFile == nullptr)
        {
            return E_POINTER;
        }
        if (pidTopic == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->get_accHelpTopic(pszHelpFile, varChild, pidTopic);
    }
    HRESULT STDMETHODCALLTYPE get_accKeyboardShortcut(VARIANT varChild, _Outptr_result_z_ BSTR* pszKeyboardShortcut) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pszKeyboardShortcut == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->get_accKeyboardShortcut(varChild, pszKeyboardShortcut);
    }
    HRESULT STDMETHODCALLTYPE get_accFocus(_Out_ VARIANT* pvarChild) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pvarChild == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->get_accFocus(pvarChild);
    }
    HRESULT STDMETHODCALLTYPE get_accSelection(_Out_ VARIANT* pvarChildren) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pvarChildren == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->get_accSelection(pvarChildren);
    }
    HRESULT STDMETHODCALLTYPE get_accDefaultAction(VARIANT varChild, _Outptr_result_z_ BSTR* pszDefaultAction) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pszDefaultAction == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->get_accDefaultAction(varChild, pszDefaultAction);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE accSelect(long flagsSelect, VARIANT varChild) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        return m_pAccessible->accSelect(flagsSelect, varChild);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE accLocation(_Out_ long* pxLeft, _Out_ long* pyTop, _Out_ long* pcxWidth, _Out_ long* pcyHeight,
        VARIANT varChild) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pxLeft == nullptr)
        {
            return E_POINTER;
        }
        if (pyTop == nullptr)
        {
            return E_POINTER;
        }
        if (pcxWidth == nullptr)
        {
            return E_POINTER;
        }
        if (pcyHeight == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->accLocation(pxLeft, pyTop, pcxWidth, pcyHeight, varChild);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE accNavigate(long navDir, VARIANT varStart, _Out_ VARIANT* pvarEndUpAt) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pvarEndUpAt == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->accNavigate(navDir, varStart, pvarEndUpAt);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE accHitTest(long xLeft, long yTop, _Out_ VARIANT* pvarChild) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pvarChild == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->accHitTest(xLeft, yTop, pvarChild);
    }
    HRESULT STDMETHODCALLTYPE accDoDefaultAction(VARIANT varChild) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        return m_pAccessible->accDoDefaultAction(varChild);
    }
    HRESULT STDMETHODCALLTYPE put_accName(VARIANT /*varChild*/, _In_z_ BSTR /*szName*/) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        return E_NOTIMPL;
    }
    HRESULT STDMETHODCALLTYPE put_accValue(VARIANT /*varChild*/, _In_z_ BSTR /*szValue*/) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        return E_NOTIMPL;
    }

    // +++++++++++++++++++++++++++++++++++
    // IDispatch, through IAccessible
    // +++++++++++++++++++++++++++++++++++
    HRESULT STDMETHODCALLTYPE GetTypeInfoCount(_Out_ unsigned int* pctinfo) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pctinfo == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->GetTypeInfoCount(pctinfo);
    }
    HRESULT STDMETHODCALLTYPE GetTypeInfo(unsigned int iTInfo, LCID lcid, _Outptr_ ITypeInfo** ppTInfo) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (ppTInfo == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->GetTypeInfo(iTInfo, lcid, ppTInfo);
    }
    HRESULT STDMETHODCALLTYPE GetIDsOfNames(_In_ REFIID riid, _In_reads_(cNames) _Deref_pre_z_ LPOLESTR* rgszNames,
        _In_range_(0, 16384) UINT cNames, LCID lcid, _Out_writes_(cNames) DISPID* rgDispId) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (rgszNames == nullptr)
        {
            return E_POINTER;
        }
        if (rgDispId == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);
    }
    HRESULT STDMETHODCALLTYPE Invoke(_In_ DISPID dispIdMember, _In_ REFIID riid, _In_ LCID lcid, _In_ WORD wFlags,
        _In_ DISPPARAMS* pDispParams, _Out_opt_ VARIANT* pVarResult, _Out_opt_ EXCEPINFO* pExcepInfo,
        _Out_opt_ UINT* puArgErr) final
    {
        if (m_pAccessible == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pDispParams == nullptr)
        {
            return E_POINTER;
        }
        if (pVarResult == nullptr)
        {
            return E_POINTER;
        }
        if (pExcepInfo == nullptr)
        {
            return E_POINTER;
        }
        if (puArgErr == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible->Invoke(dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
    }

    // +++++++++++++++++++++++++++++++++++
    // IAccessibleProxy
    // +++++++++++++++++++++++++++++++++++
    HRESULT STDMETHODCALLTYPE SetServer(_In_ IAccessible* pAccessible, _In_ IAccessibleServer* pServer) final
    {
        // hold a weak reference to the server
        m_pAccessible = pAccessible;
        m_pAccessibleServer = pServer;
        return S_OK;
    }
};

class ATL_NO_VTABLE CAccessibleProxyWithAccessibleApplicationSupport:
    public CComObjectRootEx<CComSingleThreadModel>
    , public IAccessibleProxyWithAccessibleApplicationSupportImpl<CAccessibleProxyWithAccessibleApplicationSupport>
    , public IOleWindow
    , public DebuggableIServiceProviderImpl<CAccessibleProxyWithAccessibleApplicationSupport>
{
public:
    CAccessibleProxyWithAccessibleApplicationSupport() = default;
    virtual ~CAccessibleProxyWithAccessibleApplicationSupport() = default;

    CAccessibleProxyWithAccessibleApplicationSupport(const CAccessibleProxyWithAccessibleApplicationSupport&) = delete;
    CAccessibleProxyWithAccessibleApplicationSupport(CAccessibleProxyWithAccessibleApplicationSupport&&) = delete;
    CAccessibleProxyWithAccessibleApplicationSupport& operator=(const CAccessibleProxyWithAccessibleApplicationSupport&) = delete;
    CAccessibleProxyWithAccessibleApplicationSupport& operator=(CAccessibleProxyWithAccessibleApplicationSupport&&) = delete;

    // +++++++++++++++++++++++++++++++++++
    // IOleWindow
    // +++++++++++++++++++++++++++++++++++
    HRESULT STDMETHODCALLTYPE GetWindow(_In_opt_ HWND* /*phwnd*/) final
    {
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL /*fEnterMode*/) final
    {
        return E_NOTIMPL;
    }

    // +++++++++++++++++++++++++++++++++++
    // CComObjectRootEx overrides
    // +++++++++++++++++++++++++++++++++++
    HRESULT FinalRelease()
    {
        if (m_pAccessibleServer == nullptr)
        {
            return S_OK;
        }
        return m_pAccessibleServer->SetProxy(nullptr);
    }

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,cppcoreguidelines-pro-type-cstyle-cast,cppcoreguidelines-pro-type-vararg,hicpp-avoid-c-arrays,hicpp-use-noexcept,hicpp-vararg,modernize-avoid-c-arrays,modernize-use-noexcept)
    BEGIN_COM_MAP(CAccessibleProxyWithAccessibleApplicationSupport)
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
        COM_INTERFACE_ENTRY(IAccessibleProxy)
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
        COM_INTERFACE_ENTRY(IAccessible)
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
        COM_INTERFACE_ENTRY(IDispatch)
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
        COM_INTERFACE_ENTRY(IOleWindow)
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
        COM_INTERFACE_ENTRY(IServiceProvider)
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
        COM_INTERFACE_ENTRY(IAccessibleApplication)
    END_COM_MAP()

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    BEGIN_SERVICE_MAP(CAccessibleProxyWithAccessibleApplicationSupport)
        SERVICE_ENTRY(__uuidof(IUnknown))
        SERVICE_ENTRY(__uuidof(IAccessible))
    END_SERVICE_MAP()
};

inline HRESULT CreateAccessibleProxyWithAccessibleApplicationSupportHelper(__in WPARAM wParam, __in LPARAM lParam,
    __in CWnd* pWnd, __in IAccessible* pAccessible, __in IAccessibleServer* pAccessibleServer, __out LRESULT* pResult,
    __inout IAccessibleProxy** ppAccessibleProxy)
{
    ASSERT(pWnd != nullptr);
    ASSERT(pAccessible != nullptr);
    ASSERT(pAccessibleServer != nullptr);
    ASSERT(pResult != nullptr);
    ASSERT(ppAccessibleProxy != nullptr);
    if (pWnd == nullptr)
    {
        return E_POINTER;
    }
    if (pAccessible == nullptr)
    {
        return E_POINTER;
    }
    if (pAccessibleServer == nullptr)
    {
        return E_POINTER;
    }
    if (pResult == nullptr)
    {
        return E_POINTER;
    }
    if (ppAccessibleProxy == nullptr)
    {
        return E_POINTER;
    }
    auto dwObjId = static_cast<DWORD>(lParam);
    if (dwObjId != OBJID_CLIENT)
    {
        return E_FAIL;
    }
    auto hr = E_FAIL;
    hr = pWnd->EnsureStdObj();
    if (FAILED(hr))
    {
        return hr;
    }
    if (*ppAccessibleProxy == nullptr)
    {
        CComObjectNoLock<CAccessibleProxyWithAccessibleApplicationSupport>* p;
        hr = AccessibilityCreateInstance(&p);
        if (FAILED(hr))
        {
            return hr;
        }
        CComPtr<IAccessibleProxy> spProx;
        hr = p->QueryInterface(__uuidof(IAccessibleProxy), reinterpret_cast<void**>(&spProx));
        if (FAILED(hr))
        {
            return hr;
        }
        *ppAccessibleProxy = spProx;
        spProx->SetServer(pAccessible, pAccessibleServer);
        *pResult = LresultFromObject(__uuidof(IAccessible), wParam, *ppAccessibleProxy);
        hr = S_OK;
    }
    else
    {
        *pResult = LresultFromObject(__uuidof(IAccessible), wParam, *ppAccessibleProxy);
        hr = S_OK;
    }
    return hr;
}

} // namespace ATL
#pragma pack(pop)

#endif // __AccessibleProxyWithAccessibleApplicationSupport_H__
