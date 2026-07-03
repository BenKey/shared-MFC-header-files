/*
Copyright © 2018 - 2026 by Benilda Key

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

#ifndef __AccessibleProxyWithIAccessible2Support_H__
#define __AccessibleProxyWithIAccessible2Support_H__

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

#pragma warning(push)
#pragma warning(disable : 28251)
#include "ia2_api_all.h"
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable : 26812)
#include <boost/numeric/conversion/cast.hpp>
#pragma warning(pop)

#include "AccessibleApplicationImpl.h"

#ifndef _ATL_NO_DEFAULT_LIBS
#  pragma comment(lib, "oleacc.lib")
#endif // _ATL_NO_DEFAULT_LIBS

#pragma pack(push, _ATL_PACKING)
namespace ATL {

#ifndef __IAccessible2Proxy_FWD_DEFINED__
#  define __IAccessible2Proxy_FWD_DEFINED__
   // NOLINTNEXTLINE(modernize-use-using)
   typedef interface IAccessible2Proxy IAccessible2Proxy;
#endif /* __IAccessible2Proxy_FWD_DEFINED__ */

#ifndef __IAccessible2Server_FWD_DEFINED__
#  define __IAccessible2Server_FWD_DEFINED__
   // NOLINTNEXTLINE(modernize-use-using)
   typedef interface IAccessible2Server IAccessible2Server;
#endif /* __IAccessible2Server_FWD_DEFINED__ */

MIDL_INTERFACE("CEAF718B-F168-4FF1-99BC-AE3E7971F709")
IAccessible2Proxy: public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetServer(_In_ IAccessible2* pAccessible2,
		_In_ IAccessible2Server* pServer) = 0;
};

MIDL_INTERFACE("383A534C-F648-457C-82F1-2C0FFB07F905")
IAccessible2Server: public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetProxy(/*[in]*/ IAccessible2Proxy* pAccessible2Proxy) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetHWND(/*[out]*/ HWND* phWnd) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetEnumVariant(/*[out]*/ IEnumVARIANT** ppEnumVariant) = 0;
};

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
    // NOLINTNEXTLINE(cert-err09-cpp,cert-err61-cpp,cppcoreguidelines-owning-memory,misc-throw-by-value-catch-by-reference)
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
IAccessible2 family of interfaces to an MFC application.

I would like to express my heartfelt gratitude to Microsoft for making it
so difficult to obtain the relevant information. As I was on my quest to
discover the arcane secrets one must understand in order to add support for
the IAccessible2 family of interfaces to an MFC application,  I fell down the
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
    if (pResult == nullptr) return E_POINTER;
    auto ret = CreateAccessibleProxyWithIAccessible2SupportHelper(wParam, lParam, this,
        static_cast<IAccessible2*>(reinterpret_cast<void*>(&m_xAccessible2)),
        static_cast<IAccessible2Server*>(reinterpret_cast<void*>(&m_xAccessible2Server)), pResult, &m_pProxy2);
    return ret;
}

Do this for each window that needs to support one of the IAccessible2 family of
interfaces. */

template<class T>
class IAccessibleProxyWithIAccessible2SupportImpl: public IAccessible2, public IAccessible2Proxy, public AccessibleApplicationImpl
{
public:
    IAccessible2Server* m_pAccessible2Server = nullptr;
    IAccessible2* m_pAccessible2 = nullptr;

    // +++++++++++++++++++++++++++++++++++
    // IAccessible
    // +++++++++++++++++++++++++++++++++++
    HRESULT STDMETHODCALLTYPE get_accParent(_Outptr_ IDispatch** ppdispParent) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (ppdispParent == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_accParent(ppdispParent);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE get_accChildCount(_Out_ long* pcountChildren) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pcountChildren == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_accChildCount(pcountChildren);
    }
    HRESULT STDMETHODCALLTYPE get_accChild(VARIANT varChild, _Outptr_ IDispatch** ppdispChild) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (ppdispChild == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_accChild(varChild, ppdispChild);
    }
    HRESULT STDMETHODCALLTYPE get_accName(VARIANT varChild, _Outptr_result_z_ BSTR* pszName) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pszName == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_accName(varChild, pszName);
    }
    HRESULT STDMETHODCALLTYPE get_accValue(VARIANT varChild, _Outptr_result_z_ BSTR* pszValue) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pszValue == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_accValue(varChild, pszValue);
    }
    HRESULT STDMETHODCALLTYPE get_accDescription(VARIANT varChild, _Outptr_result_z_ BSTR* pszDescription) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pszDescription == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_accDescription(varChild, pszDescription);
    }
    HRESULT STDMETHODCALLTYPE get_accRole(VARIANT varChild, _Out_ VARIANT* pvarRole) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pvarRole == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_accRole(varChild, pvarRole);
    }
    HRESULT STDMETHODCALLTYPE get_accState(VARIANT varChild, _Out_ VARIANT* pvarState) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pvarState == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_accState(varChild, pvarState);
    }
    HRESULT STDMETHODCALLTYPE get_accHelp(VARIANT varChild, _Outptr_result_z_ BSTR* pszHelp) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pszHelp == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_accHelp(varChild, pszHelp);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE get_accHelpTopic(_Outptr_result_z_ BSTR* pszHelpFile, VARIANT varChild, _Out_ long* pidTopic) final
    {
        if (m_pAccessible2 == nullptr)
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
        return m_pAccessible2->get_accHelpTopic(pszHelpFile, varChild, pidTopic);
    }
    HRESULT STDMETHODCALLTYPE get_accKeyboardShortcut(VARIANT varChild, _Outptr_result_z_ BSTR* pszKeyboardShortcut) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pszKeyboardShortcut == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_accKeyboardShortcut(varChild, pszKeyboardShortcut);
    }
    HRESULT STDMETHODCALLTYPE get_accFocus(_Out_ VARIANT* pvarChild) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pvarChild == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_accFocus(pvarChild);
    }
    HRESULT STDMETHODCALLTYPE get_accSelection(_Out_ VARIANT* pvarChildren) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pvarChildren == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_accSelection(pvarChildren);
    }
    HRESULT STDMETHODCALLTYPE get_accDefaultAction(VARIANT varChild, _Outptr_result_z_ BSTR* pszDefaultAction) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pszDefaultAction == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_accDefaultAction(varChild, pszDefaultAction);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE accSelect(long flagsSelect, VARIANT varChild) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        return m_pAccessible2->accSelect(flagsSelect, varChild);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE accLocation(_Out_ long* pxLeft, _Out_ long* pyTop, _Out_ long* pcxWidth, _Out_ long* pcyHeight,
		VARIANT varChild) final
    {
        if (m_pAccessible2 == nullptr)
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
        return m_pAccessible2->accLocation(pxLeft, pyTop, pcxWidth, pcyHeight, varChild);
    }
	// NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE accNavigate(long navDir, VARIANT varStart, _Out_ VARIANT* pvarEndUpAt) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pvarEndUpAt == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->accNavigate(navDir, varStart, pvarEndUpAt);
    }
	// NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE accHitTest(long xLeft, long yTop, _Out_ VARIANT* pvarChild) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pvarChild == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->accHitTest(xLeft, yTop, pvarChild);
    }
    HRESULT STDMETHODCALLTYPE accDoDefaultAction(VARIANT varChild) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        return m_pAccessible2->accDoDefaultAction(varChild);
    }
    HRESULT STDMETHODCALLTYPE put_accName(VARIANT /*varChild*/, _In_z_ BSTR /*szName*/) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        return E_NOTIMPL;
    }
    HRESULT STDMETHODCALLTYPE put_accValue(VARIANT /*varChild*/, _In_z_ BSTR /*szValue*/) final
    {
        if (m_pAccessible2 == nullptr)
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
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (pctinfo == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->GetTypeInfoCount(pctinfo);
    }
    HRESULT STDMETHODCALLTYPE GetTypeInfo(unsigned int iTInfo, LCID lcid, _Outptr_ ITypeInfo** ppTInfo) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (ppTInfo == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->GetTypeInfo(iTInfo, lcid, ppTInfo);
    }
    HRESULT STDMETHODCALLTYPE GetIDsOfNames(_In_ REFIID riid, _In_reads_(cNames) _Deref_pre_z_ LPOLESTR* rgszNames,
        _In_range_(0, 16384) UINT cNames, LCID lcid, _Out_writes_(cNames) DISPID* rgDispId) final
    {
        if (m_pAccessible2 == nullptr)
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
        return m_pAccessible2->GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);
    }
    HRESULT STDMETHODCALLTYPE Invoke(_In_ DISPID dispIdMember, _In_ REFIID riid, _In_ LCID lcid, _In_ WORD wFlags,
		_In_ DISPPARAMS* pDispParams, _Out_opt_ VARIANT* pVarResult, _Out_opt_ EXCEPINFO* pExcepInfo,
		_Out_opt_ UINT* puArgErr) final
    {
        if (m_pAccessible2 == nullptr)
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
        return m_pAccessible2->Invoke(dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
    }

    // +++++++++++++++++++++++++++++++++++
    // IAccessible2Proxy
    // +++++++++++++++++++++++++++++++++++
    HRESULT STDMETHODCALLTYPE SetServer(_In_ IAccessible2* pAccessible2, _In_ IAccessible2Server* pServer) final
    {
        m_pAccessible2 = reinterpret_cast<IAccessible2*>(pAccessible2);
        m_pAccessible2Server = pServer;
        return S_OK;
    }

    // +++++++++++++++++++++++++++++++++++
    // IAccessible2
    // +++++++++++++++++++++++++++++++++++
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE get_nRelations(/*[retval][out]*/ long* nRelations) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (nRelations == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_nRelations(nRelations);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE get_relation(/*[in]*/ long relationIndex, /*[retval][out]*/ IAccessibleRelation** relation) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (relation == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_relation(relationIndex, relation);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE get_relations(/*[in]*/ long maxRelations,
        // NOLINTNEXTLINE(google-runtime-int)
		/*[length_is][size_is][out]*/ IAccessibleRelation** relations, /*[retval][out]*/ long* nRelations) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (relations == nullptr)
        {
            return E_POINTER;
        }
        if (nRelations == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_relations(maxRelations, relations, nRelations);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE role(/*[retval][out]*/ long* role) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (role == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->role(role);
    }

#pragma warning(push)
#pragma warning(disable : 26812)
    HRESULT STDMETHODCALLTYPE scrollTo(/*[in]*/ enum IA2ScrollType scrollType) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        return m_pAccessible2->scrollTo(scrollType);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE scrollToPoint(/*[in]*/ enum IA2CoordinateType coordinateType, /*[in]*/ long x, /*[in]*/ long y) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        return m_pAccessible2->scrollToPoint(coordinateType, x, y);
    }
#pragma warning(pop)

    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE get_groupPosition(/*[out]*/ long* groupLevel, /*[out]*/ long* similarItemsInGroup,
        // NOLINTNEXTLINE(google-runtime-int)
        /*[retval][out]*/ long* positionInGroup) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (groupLevel == nullptr)
        {
            return E_POINTER;
        }
        if (similarItemsInGroup == nullptr)
        {
            return E_POINTER;
        }
        if (positionInGroup == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_groupPosition(groupLevel, similarItemsInGroup, positionInGroup);
    }
    HRESULT STDMETHODCALLTYPE get_states(/*[retval][out]*/ AccessibleStates* states) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (states == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_states(states);
    }
    HRESULT STDMETHODCALLTYPE get_extendedRole(/*[retval][out]*/ BSTR* extendedRole) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (extendedRole == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_extendedRole(extendedRole);
    }
    HRESULT STDMETHODCALLTYPE get_localizedExtendedRole(/*[retval][out]*/ BSTR* localizedExtendedRole) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (localizedExtendedRole == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_localizedExtendedRole(localizedExtendedRole);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE get_nExtendedStates(/*[retval][out]*/ long* nExtendedStates) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (nExtendedStates == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_nExtendedStates(nExtendedStates);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE get_extendedStates(/*[in]*/ long maxExtendedStates,
        /*[length_is][length_is][size_is][size_is][out]*/ BSTR** extendedStates,
        // NOLINTNEXTLINE(google-runtime-int)
        /*[retval][out]*/ long* nExtendedStates) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (extendedStates == nullptr)
        {
            return E_POINTER;
        }
        if (nExtendedStates == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_extendedStates(maxExtendedStates, extendedStates, nExtendedStates);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE get_localizedExtendedStates(/*[in]*/ long maxLocalizedExtendedStates,
        /*[length_is][length_is][size_is][size_is][out]*/ BSTR** localizedExtendedStates,
        // NOLINTNEXTLINE(google-runtime-int)
        /*[retval][out]*/ long* nLocalizedExtendedStates) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (localizedExtendedStates == nullptr)
        {
            return E_POINTER;
        }
        if (nLocalizedExtendedStates == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_localizedExtendedStates(maxLocalizedExtendedStates, localizedExtendedStates,
			nLocalizedExtendedStates);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE get_uniqueID(/*[retval][out]*/ long* uniqueID) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (uniqueID == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_uniqueID(uniqueID);
    }
    HRESULT STDMETHODCALLTYPE get_windowHandle(/*[retval][out]*/ HWND* windowHandle) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (windowHandle == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_windowHandle(windowHandle);
    }
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT STDMETHODCALLTYPE get_indexInParent(/*[retval][out]*/ long* indexInParent) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (indexInParent == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_indexInParent(indexInParent);
    }
    HRESULT STDMETHODCALLTYPE get_locale(/*[retval][out]*/ IA2Locale* locale) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (locale == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_locale(locale);
    }
    HRESULT STDMETHODCALLTYPE get_attributes(/*[retval][out]*/ BSTR* attributes) final
    {
        if (m_pAccessible2 == nullptr)
        {
            return RPC_E_DISCONNECTED;
        }
        if (attributes == nullptr)
        {
            return E_POINTER;
        }
        return m_pAccessible2->get_attributes(attributes);
    }
};

class ATL_NO_VTABLE CAccessibleProxyWithIAccessible2Support:
	public CComObjectRootEx<CComSingleThreadModel>
    , public IAccessibleProxyWithIAccessible2SupportImpl<CAccessibleProxyWithIAccessible2Support>
    , public IOleWindow
    , public DebuggableIServiceProviderImpl<CAccessibleProxyWithIAccessible2Support>
{
public:
    CAccessibleProxyWithIAccessible2Support() = default;
    virtual ~CAccessibleProxyWithIAccessible2Support() = default;

    // +++++++++++++++++++++++++++++++++++
    // Disallow copy and assignment.
    // +++++++++++++++++++++++++++++++++++
    CAccessibleProxyWithIAccessible2Support(const CAccessibleProxyWithIAccessible2Support&) = delete;
    CAccessibleProxyWithIAccessible2Support(CAccessibleProxyWithIAccessible2Support&&) = delete;
    CAccessibleProxyWithIAccessible2Support& operator=(const CAccessibleProxyWithIAccessible2Support&) = delete;
    CAccessibleProxyWithIAccessible2Support& operator=(CAccessibleProxyWithIAccessible2Support&&) = delete;

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
        if (m_pAccessible2Server == nullptr)
        {
            return S_OK;
        }
        return m_pAccessible2Server->SetProxy(nullptr);
    }

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,cppcoreguidelines-pro-type-cstyle-cast,cppcoreguidelines-pro-type-vararg,hicpp-avoid-c-arrays,hicpp-use-noexcept,hicpp-vararg,modernize-avoid-c-arrays,modernize-use-noexcept)
    BEGIN_COM_MAP(CAccessibleProxyWithIAccessible2Support)
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
        COM_INTERFACE_ENTRY(IAccessible2Proxy)
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
        COM_INTERFACE_ENTRY2(IAccessible, IAccessible2)
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
        COM_INTERFACE_ENTRY(IDispatch)
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
        COM_INTERFACE_ENTRY(IOleWindow)
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
        COM_INTERFACE_ENTRY(IServiceProvider)
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
        COM_INTERFACE_ENTRY(IAccessible2)
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
        COM_INTERFACE_ENTRY(IAccessibleApplication)
    END_COM_MAP()

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,cppcoreguidelines-pro-type-cstyle-cast,cppcoreguidelines-pro-type-vararg,hicpp-avoid-c-arrays,hicpp-use-noexcept,hicpp-vararg,modernize-avoid-c-arrays,modernize-use-noexcept)
    BEGIN_SERVICE_MAP(CAccessibleProxyWithIAccessible2Support)
        SERVICE_ENTRY(__uuidof(IUnknown))
        SERVICE_ENTRY(__uuidof(IAccessible))
        SERVICE_ENTRY(__uuidof(IAccessible2))
    END_SERVICE_MAP()
};

inline HRESULT CreateAccessibleProxyWithIAccessible2SupportHelper(__in WPARAM wParam, __in LPARAM lParam, __in CWnd* pWnd,
	__in IAccessible2* pAccessible2, __in IAccessible2Server* pAccessible2Server, __out LRESULT* pResult,
    __inout IAccessible2Proxy** ppAccessible2Proxy)
{
    ASSERT(pWnd != nullptr);
    ASSERT(pAccessible2Server != nullptr);
    ASSERT(pResult != nullptr);
    ASSERT(ppAccessible2Proxy != nullptr);
    if (pWnd == nullptr)
    {
        return E_POINTER;
    }
    if (pAccessible2Server == nullptr)
    {
        return E_POINTER;
    }
    if (pResult == nullptr)
    {
        return E_POINTER;
    }
    if (ppAccessible2Proxy == nullptr)
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
    if (*ppAccessible2Proxy == nullptr)
    {
        CComObjectNoLock<CAccessibleProxyWithIAccessible2Support>* p;
        hr = AccessibilityCreateInstance(&p);
        if (FAILED(hr))
        {
            return hr;
        }
        CComPtr<IAccessible2Proxy> spProx;
        hr = p->QueryInterface(__uuidof(IAccessible2Proxy), reinterpret_cast<void**>(&spProx));
        if (FAILED(hr))
        {
            return hr;
        }
        *ppAccessible2Proxy = spProx;
        spProx->SetServer(pAccessible2, pAccessible2Server);
        *pResult = LresultFromObject(__uuidof(IAccessible), wParam, *ppAccessible2Proxy);
        hr = S_OK;
    }
    else
    {
        *pResult = LresultFromObject(__uuidof(IAccessible), wParam, *ppAccessible2Proxy);
        hr = S_OK;
    }
    return hr;
}

} // namespace ATL
#pragma pack(pop)

#endif // __AccessibleProxyWithIAccessible2Support_H__
