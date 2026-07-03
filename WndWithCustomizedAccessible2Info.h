/*
Copyright 2018 - 2026 by Benilda Key

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

#pragma once

#include <afxpriv.h>

#include <CppCoreCheck/warnings.h>
#include <CodeAnalysis/Warnings.h>

#include "AccessibleProxyWithIAccessible2Support.h"

#pragma warning(push)
#pragma warning(disable : 4996)
#pragma warning(disable : ALL_CODE_ANALYSIS_WARNINGS)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include <array>
#include <string>
#include <boost/format.hpp>
#include <boost/numeric/conversion/cast.hpp>
#pragma warning(pop)

#pragma warning(default : ALL_CODE_ANALYSIS_WARNINGS)
#pragma warning(default : ALL_CPPCORECHECK_WARNINGS)

template<class TBase>
class CWndWithCustomizedAccessible2Info : public TBase
{

inline std::wstring loadString(UINT uID)
{
    using namespace std::string_literals;
    const size_t maxStringTableResourceLen = 4096;
    std::array<wchar_t, maxStringTableResourceLen> buffer{};
    try
    {
        int result = AfxLoadString(
            uID, buffer.data(),
            boost::numeric_cast<UINT>(buffer.size()));
        if (result == 0 || buffer.at(0) == 0)
        {
            return L""s;
        }
    }
    catch (boost::bad_numeric_cast&)
    {
        return L""s;
    }
    return buffer.data();
}

template<typename... Arguments>
inline std::wstring FormatArgs(const std::wstring& fmt, const Arguments&... args)
{
    return boost::str((boost::wformat(fmt) % ... % args));
}

template<typename... Arguments>
inline std::wstring FormatArgs(UINT fmt, const Arguments&... args)
{
    using namespace std::string_literals;
    const auto formatStr = loadString(fmt);
    if (formatStr.empty())
    {
        return L""s;
    }
    return boost::str((boost::wformat(formatStr) % ... % args));
}

public:
    using CWnd::EnableActiveAccessibility;
	using CWnd::NotifyWinEvent;
    using CWnd::GetSafeHwnd;
    using CWnd::get_accState;
    using CWnd::m_hWnd;

    // +++++++++++++++++++++++++++++++++++
    // Construction/Destruction
    // +++++++++++++++++++++++++++++++++++
    CWndWithCustomizedAccessible2Info() noexcept;
    // NOLINTNEXTLINE(hicpp-use-override,modernize-use-override)
    virtual ~CWndWithCustomizedAccessible2Info() = default;

    // +++++++++++++++++++++++++++++++++++
    // Disallow copy and assignment.
    // +++++++++++++++++++++++++++++++++++
    CWndWithCustomizedAccessible2Info(const CWndWithCustomizedAccessible2Info&) = delete;
    CWndWithCustomizedAccessible2Info(CWndWithCustomizedAccessible2Info&&) = delete;
    CWndWithCustomizedAccessible2Info& operator=(const CWndWithCustomizedAccessible2Info&) = delete;
    CWndWithCustomizedAccessible2Info& operator=(CWndWithCustomizedAccessible2Info&&) = delete;

    // +++++++++++++++++++++++++++++++++++
    // Customization: Functions that make it possible to override the behavior
    // exhibited by the default CWnd implementation of some of the functions in
    // the IAccessible interface.
    // +++++++++++++++++++++++++++++++++++
    bool SetAccessibleDescription(VARIANT varChild, LPCWSTR description);
    bool SetAccessibleDescription(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetAccessibleDescriptionF(VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto description = FormatArgs(format, args...);
        if (description.empty())
        {
            return false;
        }
        return SetAccessibleDescription(varChild, description.c_str());
    }
    template<typename... Arguments>
    bool SetAccessibleDescriptionF(VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto description = FormatArgs(format, args...);
        if (description.empty())
        {
            return false;
        }
        return SetAccessibleDescription(varChild, description.c_str());
    }
    bool SetAccessibleHelp(VARIANT varChild, LPCWSTR help);
    bool SetAccessibleHelp(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetAccessibleHelpF(VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto help = FormatArgs(format, args...);
        if (help.empty())
        {
            return false;
        }
        return SetAccessibleHelp(varChild, help.c_str());
    }
    template<typename... Arguments>
    bool SetAccessibleHelpF(VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto help = FormatArgs(format, args...);
        if (help.empty())
        {
            return false;
        }
        return SetAccessibleHelp(varChild, help.c_str());
    }
    bool SetAccessibleName(VARIANT varChild, LPCWSTR name);
    bool SetAccessibleName(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetAccessibleNameF(VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto name = FormatArgs(format, args...);
        if (name.IsEmpty())
        {
            return false;
        }
        return SetAccessibleName(varChild, name.c_str());
    }
    template<typename... Arguments>
    bool SetAccessibleNameF(VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto name = FormatArgs(format, args...);
        if (name.empty())
        {
            return false;
        }
        return SetAccessibleName(varChild, name.c_str());
    }
    bool SetAccessibleRole(VARIANT varChild, LONG role);
    bool SetAccessibleState(VARIANT varChild, LONG state, bool overrideMSAA);
    bool SetAccessibleValue(VARIANT varChild, LPCWSTR value);
    bool SetAccessibleValue(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetAccessibleValueF(VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto value = FormatArgs(format, args...);
        if (value.empty())
        {
            return false;
        }
        return SetAccessibleValue(varChild, value.c_str());
    }
    template<typename... Arguments>
    bool SetAccessibleValueF(VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto value = FormatArgs(format, args...);
        if (value.empty())
        {
            return false;
        }
        return SetAccessibleValue(varChild, value);
    }

    // +++++++++++++++++++++++++++++++++++
    // Customization: Set IAccessible2 properties for the window.
    // +++++++++++++++++++++++++++++++++++
    void SetIA2AccessibleStates(AccessibleStates states);
    void SetIA2Role(IA2Role role);

    // +++++++++++++++++++++++++++++++++++
    // CWnd method overrides.
    // +++++++++++++++++++++++++++++++++++
    // NOLINTNEXTLINE(clang-diagnostic-overloaded-virtual)
    HRESULT SetProxy(IAccessible2Proxy* pProxy);
    HRESULT CreateAccessibleProxy(WPARAM wParam, LPARAM lParam, LRESULT* pResult) override;

    // +++++++++++++++++++++++++++++++++++
    // IAccessible2
    // +++++++++++++++++++++++++++++++++++
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT get_nRelations(long* nRelations);
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT get_relation(long relationIndex, IAccessibleRelation** relation);
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT get_relations(long maxRelations, IAccessibleRelation** relations, long* nRelations);
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT role(long* role);
    HRESULT scrollTo(enum IA2ScrollType scrollType);
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT scrollToPoint(enum IA2CoordinateType coordinateType, long x, long y);
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT get_groupPosition(long* groupLevel, long* similarItemsInGroup, long* positionInGroup);
    HRESULT get_states(AccessibleStates* states);
    HRESULT get_extendedRole(BSTR* extendedRole);
    HRESULT get_localizedExtendedRole(BSTR* localizedExtendedRole);
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT get_nExtendedStates(long* nExtendedStates);
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT get_extendedStates(long maxExtendedStates, BSTR** extendedStates, long* nExtendedStates);
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT get_localizedExtendedStates(long maxLocalizedExtendedStates, BSTR** localizedExtendedStates, long* nLocalizedExtendedStates);
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT get_uniqueID(long* uniqueID);
    HRESULT get_windowHandle(HWND* windowHandle);
    // NOLINTNEXTLINE(google-runtime-int)
    HRESULT get_indexInParent(long* indexInParent);
    HRESULT get_locale(IA2Locale* locale);
    HRESULT get_attributes(BSTR* attributes);

protected:
    afx_msg void OnDestroy();

protected:
    using CWnd::m_xAccessible;
    using CWnd::m_xAccessibleServer;
    using CWnd::m_pProxy;

    bool InitAccPropServices();

    class XAccessible2
    {
    public:
        virtual ~XAccessible2() = default;
        virtual ULONG __stdcall AddRef();
        virtual ULONG __stdcall Release();
        virtual HRESULT __stdcall QueryInterface(REFIID iid, LPVOID* ppvObj);
        virtual HRESULT __stdcall Invoke(DISPID dispIdMember, REFIID refiid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams,
			VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
        virtual HRESULT __stdcall GetIDsOfNames(REFIID refiid, LPOLESTR*  rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId);
        virtual HRESULT __stdcall GetTypeInfoCount(unsigned int* pctinfo);
        virtual HRESULT __stdcall GetTypeInfo(unsigned int iTInfo, LCID lcid, ITypeInfo** ppTInfo);
        virtual HRESULT __stdcall get_accParent(IDispatch** ppdispParent);
        // NOLINTNEXTLINE(google-runtime-int)
        virtual HRESULT __stdcall get_accChildCount(long* pcountChildren);
        virtual HRESULT __stdcall get_accChild(VARIANT varChild, IDispatch** ppdispChild);
        virtual HRESULT __stdcall get_accName(VARIANT varChild, BSTR* pszName);
        virtual HRESULT __stdcall get_accValue(VARIANT varChild, BSTR* pszValue);
        virtual HRESULT __stdcall get_accDescription(VARIANT varChild, BSTR* pszDescription);
        virtual HRESULT __stdcall get_accRole(VARIANT varChild, VARIANT* pvarRole);
        virtual HRESULT __stdcall get_accState(VARIANT varChild, VARIANT* pvarState);
        virtual HRESULT __stdcall get_accHelp(VARIANT varChild, BSTR* pszHelp);
        // NOLINTNEXTLINE(google-runtime-int)
        virtual HRESULT __stdcall get_accHelpTopic(BSTR* pszHelpFile, VARIANT varChild, long* pidTopic);
        virtual HRESULT __stdcall get_accKeyboardShortcut(VARIANT varChild, BSTR* pszKeyboardShortcut);
        virtual HRESULT __stdcall get_accFocus(VARIANT* pvarChild);
        virtual HRESULT __stdcall get_accSelection(VARIANT* pvarChildren);
        virtual HRESULT __stdcall get_accDefaultAction(VARIANT varChild, BSTR* pszDefaultAction);
        // NOLINTNEXTLINE(google-runtime-int)
        virtual HRESULT __stdcall accSelect(long flagsSelect, VARIANT varChild);
        // NOLINTNEXTLINE(google-runtime-int)
        virtual HRESULT __stdcall accLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild);
        // NOLINTNEXTLINE(google-runtime-int)
        virtual HRESULT __stdcall accNavigate(long navDir, VARIANT varStart, VARIANT* pvarEndUpAt);
        // NOLINTNEXTLINE(google-runtime-int)
        virtual HRESULT __stdcall accHitTest(long xLeft, long yTop, VARIANT* pvarChild);
        virtual HRESULT __stdcall accDoDefaultAction(VARIANT varChild);
        virtual HRESULT __stdcall put_accName(VARIANT varChild, BSTR szName);
        virtual HRESULT __stdcall put_accValue(VARIANT varChild, BSTR szValue);
        // NOLINTNEXTLINE(google-runtime-int)
        virtual HRESULT __stdcall get_nRelations(long* nRelations);
        // NOLINTNEXTLINE(google-runtime-int)
        virtual HRESULT __stdcall get_relation(long relationIndex, IAccessibleRelation** relation);
        // NOLINTNEXTLINE(google-runtime-int)
        virtual HRESULT __stdcall get_relations(long maxRelations, IAccessibleRelation** relations, long* nRelations);
        // NOLINTNEXTLINE(google-runtime-int)
        virtual HRESULT __stdcall role(long* role);
        virtual HRESULT __stdcall scrollTo(enum IA2ScrollType scrollType);
        // NOLINTNEXTLINE(google-runtime-int)
        virtual HRESULT __stdcall scrollToPoint(enum IA2CoordinateType coordinateType, long x, long y);
        // NOLINTNEXTLINE(google-runtime-int)
        virtual HRESULT __stdcall get_groupPosition(long* groupLevel, long* similarItemsInGroup, long* positionInGroup);
        virtual HRESULT __stdcall get_states(AccessibleStates* states);
        virtual HRESULT __stdcall get_extendedRole(BSTR* extendedRole);
        virtual HRESULT __stdcall get_localizedExtendedRole(BSTR* localizedExtendedRole);
        // NOLINTNEXTLINE(google-runtime-int)
        virtual HRESULT __stdcall get_nExtendedStates(long* nExtendedStates);
        // NOLINTNEXTLINE(google-runtime-int)
        virtual HRESULT __stdcall get_extendedStates(long maxExtendedStates, BSTR** extendedStates, long* nExtendedStates);
        // NOLINTNEXTLINE(google-runtime-int)
        virtual HRESULT __stdcall get_localizedExtendedStates(long maxLocalizedExtendedStates, BSTR** localizedExtendedStates, long* nLocalizedExtendedStates);
        // NOLINTNEXTLINE(google-runtime-int)
        virtual HRESULT __stdcall get_uniqueID(long* uniqueID);
        virtual HRESULT __stdcall get_windowHandle(HWND* windowHandle);
        // NOLINTNEXTLINE(google-runtime-int)
        virtual HRESULT __stdcall get_indexInParent(long* indexInParent);
        virtual HRESULT __stdcall get_locale(IA2Locale* locale);
        virtual HRESULT __stdcall get_attributes(BSTR* attributes);
    };
    friend class XAccessible2;
    // NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes)
    XAccessible2 m_xAccessible2;

    class XAccessible2Server
    {
    public:
        virtual ~XAccessible2Server() = default;
        virtual ULONG __stdcall AddRef();
        virtual ULONG __stdcall Release();
        virtual HRESULT __stdcall QueryInterface(REFIID iid, LPVOID* ppvObj);
        virtual HRESULT __stdcall SetProxy(IAccessible2Proxy* pProxy);
        virtual HRESULT __stdcall GetHWND(HWND *phWnd);
        virtual HRESULT __stdcall GetEnumVariant(IEnumVARIANT **ppEnumVariant);
    };
    friend class XAccessible2Server;
    // NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes)
    XAccessible2Server m_xAccessible2Server;

private:
    CComPtr<IAccPropServices> m_pAccPropServices = nullptr;
    bool m_initAccPropServicesFailed = false;
    IAccessible2Proxy* m_pProxy2 = nullptr;
    AccessibleStates m_IA2AccessibleStates = 0;
#pragma warning(push)
#pragma warning(disable : 26812)
    IA2Role m_IA2Role = IA2_ROLE_UNKNOWN;
#pragma warning(pop)
};
