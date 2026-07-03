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

// NOLINTBEGIN(clang-analyzer-optin.core.EnumCastOutOfRange)
#include "stdafx.h"
#include "WndWithCustomizedAccessible2Info.h"
#include "AccessibleProxyWithIAccessible2Support.h"

#ifdef _DEBUG
   // NOLINTNEXTLINE(clang-diagnostic-keyword-macro)
#  define new DEBUG_NEW
#endif

// +++++++++++++++++++++++++++++++++++
// Construction/Destruction
// +++++++++++++++++++++++++++++++++++

template<class TBase>
CWndWithCustomizedAccessible2Info<TBase>::CWndWithCustomizedAccessible2Info() noexcept:
    TBase()
{
    EnableActiveAccessibility();
}

// +++++++++++++++++++++++++++++++++++
// Customization: Functions that make it possible to override the behavior
// exhibited by the default CWnd implementation of some of the functions in
// the IAccessible interface.
// +++++++++++++++++++++++++++++++++++

template<class TBase>
bool CWndWithCustomizedAccessible2Info<TBase>::SetAccessibleDescription(VARIANT varChild, LPCWSTR description)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    if (varChild.vt != VT_I4)
    {
        return false;
    }
    if (!InitAccPropServices())
    {
        return false;
    }
    if (description == nullptr || description[0] == 0)
    {
        return false;
    }
    CComVariant var(description);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,cppcoreguidelines-pro-type-union-access)
    auto hr = m_pAccPropServices->SetHwndProp(GetSafeHwnd(), static_cast<DWORD>(OBJID_CLIENT), varChild.ulVal,
		PROPID_ACC_DESCRIPTION, var);
    return SUCCEEDED(hr);
}

template<class TBase>
bool CWndWithCustomizedAccessible2Info<TBase>::SetAccessibleDescription(VARIANT varChild, UINT nID)
{
    if (nID == 0)
    {
        return false;
    }
    CStringW description;
    const auto loadStringResult = description.LoadString(nID);
    if (loadStringResult == FALSE)
    {
        return false;
    }
    if (description.IsEmpty())
    {
        return false;
    }
    return SetAccessibleDescription(varChild, description);
}

template<class TBase>
bool CWndWithCustomizedAccessible2Info<TBase>::SetAccessibleHelp(VARIANT varChild, LPCWSTR help)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    if (varChild.vt != VT_I4)
    {
        return false;
    }
    if (!InitAccPropServices())
    {
        return false;
    }
    if (help == nullptr || help[0] == 0)
    {
        return false;
    }
    CComVariant var(help);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,cppcoreguidelines-pro-type-union-access)
    auto hr = m_pAccPropServices->SetHwndProp(GetSafeHwnd(), static_cast<DWORD>(OBJID_CLIENT), varChild.ulVal,
		PROPID_ACC_HELP, var);
    return SUCCEEDED(hr);
}

template<class TBase>
bool CWndWithCustomizedAccessible2Info<TBase>::SetAccessibleHelp(VARIANT varChild, UINT nID)
{
    if (nID == 0)
    {
        return false;
    }
    CStringW help;
    const auto loadStringResult = help.LoadString(nID);
    if (loadStringResult == FALSE)
    {
        return false;
    }
    if (help.IsEmpty())
    {
        return false;
    }
    return SetAccessibleHelp(varChild, help);
}

template<class TBase>
bool CWndWithCustomizedAccessible2Info<TBase>::SetAccessibleName(VARIANT varChild, LPCWSTR name)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    if (varChild.vt != VT_I4)
    {
        return false;
    }
    if (!InitAccPropServices())
    {
        return false;
    }
    if (name == nullptr || name[0] == 0)
    {
        return false;
    }
    CComVariant var(name);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,cppcoreguidelines-pro-type-union-access)
    auto hr = m_pAccPropServices->SetHwndProp(GetSafeHwnd(), static_cast<DWORD>(OBJID_CLIENT), varChild.ulVal,
		PROPID_ACC_NAME, var);
    return SUCCEEDED(hr);
}

template<class TBase>
bool CWndWithCustomizedAccessible2Info<TBase>::SetAccessibleName(VARIANT varChild, UINT nID)
{
    if (nID == 0)
    {
        return false;
    }
    CStringW name;
    const auto loadStringResult = name.LoadString(nID);
    if (loadStringResult == FALSE)
    {
        return false;
    }
    if (name.IsEmpty())
    {
        return false;
    }
    return SetAccessibleName(varChild, name);
}

template<class TBase>
bool CWndWithCustomizedAccessible2Info<TBase>::SetAccessibleRole(VARIANT varChild, LONG role)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    if (varChild.vt != VT_I4)
    {
        return false;
    }
    if (!InitAccPropServices())
    {
        return false;
    }
    if (role == 0)
    {
        return false;
    }
    CComVariant var(role);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,cppcoreguidelines-pro-type-union-access)
    auto hr = m_pAccPropServices->SetHwndProp(GetSafeHwnd(), static_cast<DWORD>(OBJID_CLIENT), varChild.ulVal,
        PROPID_ACC_ROLE, var);
    return SUCCEEDED(hr);
}

template<class TBase>
bool CWndWithCustomizedAccessible2Info<TBase>::SetAccessibleState(VARIANT varChild, LONG state, bool overrideMSAA)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    if (varChild.vt != VT_I4)
    {
        return false;
    }
    if (!InitAccPropServices())
    {
        return false;
    }
    if (state == 0)
    {
        return false;
    }
    CComVariant var;
    if (overrideMSAA)
    {
        var = state;
    }
    else
    {
        auto hrGetState = get_accState(varChild, &var);
        if (FAILED(hrGetState))
        {
            return false;
        }
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access,hicpp-signed-bitwise)
        var.lVal |= state;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,cppcoreguidelines-pro-type-union-access)
    auto hr = m_pAccPropServices->SetHwndProp(GetSafeHwnd(), static_cast<DWORD>(OBJID_CLIENT), varChild.ulVal,
        PROPID_ACC_STATE, var);
    return SUCCEEDED(hr);
}

template<class TBase>
bool CWndWithCustomizedAccessible2Info<TBase>::SetAccessibleValue(VARIANT varChild, LPCWSTR value)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    if (varChild.vt != VT_I4)
    {
        return false;
    }
    if (!InitAccPropServices())
    {
        return false;
    }
    if (value == nullptr || value[0] == 0)
    {
        return false;
    }
    CComVariant var(value);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,cppcoreguidelines-pro-type-union-access)
    auto hr = m_pAccPropServices->SetHwndProp(GetSafeHwnd(), static_cast<DWORD>(OBJID_CLIENT), varChild.ulVal,
        PROPID_ACC_VALUE, var);
    return SUCCEEDED(hr);
}

template<class TBase>
bool CWndWithCustomizedAccessible2Info<TBase>::SetAccessibleValue(VARIANT varChild, UINT nID)
{
    if (nID == 0)
    {
        return false;
    }
    CStringW value;
    const auto loadStringResult = value.LoadString(nID);
    if (loadStringResult == FALSE)
    {
        return false;
    }
    if (value.IsEmpty())
    {
        return false;
    }
    return SetAccessibleValue(varChild, value);
}

// +++++++++++++++++++++++++++++++++++
// Customization: Set IAccessible2 properties for the window.
// +++++++++++++++++++++++++++++++++++
template<class TBase>
void CWndWithCustomizedAccessible2Info<TBase>::SetIA2AccessibleStates(AccessibleStates states)
{
    m_IA2AccessibleStates = states;
}
template<class TBase>
void CWndWithCustomizedAccessible2Info<TBase>::SetIA2Role(IA2Role role)
{
    m_IA2Role = role;
}

// +++++++++++++++++++++++++++++++++++
// CWnd method overrides.
// +++++++++++++++++++++++++++++++++++
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::SetProxy(IAccessible2Proxy* pProxy)
{
    m_pProxy2 = pProxy;
    return S_OK;
}

template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::CreateAccessibleProxy(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    ASSERT(pResult != nullptr);
    if (pResult == nullptr)
    {
        return E_POINTER;
    }
    auto ret = CreateAccessibleProxyWithIAccessible2SupportHelper(wParam, lParam, this,
        static_cast<IAccessible2*>(reinterpret_cast<void*>(&m_xAccessible2)),
        static_cast<IAccessible2Server*>(reinterpret_cast<void*>(&m_xAccessible2Server)), pResult, &m_pProxy2);
    return ret;
}

#pragma warning(push)
#pragma warning(disable : 6387)
template<class TBase>
void CWndWithCustomizedAccessible2Info<TBase>::OnDestroy()
{
    if (m_pProxy2 != nullptr)
    {
        m_pProxy2->SetServer(nullptr, nullptr);
    }
    TBase::OnDestroy();
}
#pragma warning(pop)

// +++++++++++++++++++++++++++++++++++
// IAccessible2
// +++++++++++++++++++++++++++++++++++
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::get_nRelations(long* nRelations)
{
    if (nRelations == nullptr)
    {
        return E_POINTER;
    }
    *nRelations = 0;
    return E_NOTIMPL;
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::get_relation(long relationIndex, IAccessibleRelation** relation)
{
    if (relation == nullptr)
    {
        return E_POINTER;
    }
    return E_NOTIMPL;
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::get_relations(long maxRelations, IAccessibleRelation** relations, long* nRelations)
{
    if (relations == nullptr)
    {
        return E_POINTER;
    }
    *relations = nullptr;
    if (nRelations == nullptr)
    {
        return E_POINTER;
    }
    *nRelations = 0;
    return E_NOTIMPL;
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::role(long* role)
{
    if (role == nullptr)
    {
        return E_POINTER;
    }
    *role = m_IA2Role;
    if (m_IA2Role == IA2_ROLE_UNKNOWN)
    {
        return S_FALSE;
    }
    return S_OK;
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::scrollTo(enum IA2ScrollType scrollType)
{
    return E_NOTIMPL;
}
template<class TBase>
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters,google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::scrollToPoint(enum IA2CoordinateType coordinateType, long x, long y)
{
    return E_NOTIMPL;
}
template<class TBase>
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters,google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::get_groupPosition(long* groupLevel, long* similarItemsInGroup, long* positionInGroup)
{
    if (groupLevel == nullptr)
    {
        return E_POINTER;
    }
    *groupLevel = 0;
    if (similarItemsInGroup == nullptr)
    {
        return E_POINTER;
    }
    *similarItemsInGroup = 0;
    if (positionInGroup == nullptr)
    {
        return E_POINTER;
    }
    *positionInGroup = 0;
    return E_NOTIMPL;
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::get_states(AccessibleStates* states)
{
    if (states == nullptr)
    {
        return E_POINTER;
    }
    *states = m_IA2AccessibleStates;
    if (m_IA2AccessibleStates == 0)
    {
        return S_FALSE;
    }
    return S_OK;
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::get_extendedRole(BSTR* extendedRole)
{
    if (extendedRole == nullptr)
    {
        return E_POINTER;
    }
    *extendedRole = nullptr;
    return E_NOTIMPL;
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::get_localizedExtendedRole(BSTR* localizedExtendedRole)
{
    if (localizedExtendedRole == nullptr)
    {
        return E_POINTER;
    }
    *localizedExtendedRole = nullptr;
    return E_NOTIMPL;
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::get_nExtendedStates(long* nExtendedStates)
{
    if (nExtendedStates == nullptr)
    {
        return E_POINTER;
    }
    *nExtendedStates = 0;
    return E_NOTIMPL;
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::get_extendedStates(long maxExtendedStates, BSTR** extendedStates, long* nExtendedStates)
{
    if (extendedStates == nullptr)
    {
        return E_POINTER;
    }
    *extendedStates = nullptr;
    if (nExtendedStates == nullptr)
    {
        return E_POINTER;
    }
    *nExtendedStates = 0;
    return E_NOTIMPL;
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::get_localizedExtendedStates(long maxLocalizedExtendedStates,
    // NOLINTNEXTLINE(google-runtime-int)
	BSTR** localizedExtendedStates, long* nLocalizedExtendedStates)
{
    if (localizedExtendedStates == nullptr)
    {
        return E_POINTER;
    }
    *localizedExtendedStates = nullptr;
    if (nLocalizedExtendedStates == nullptr)
    {
        return E_POINTER;
    }
    *nLocalizedExtendedStates = 0;
    return E_NOTIMPL;
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::get_uniqueID(long* uniqueID)
{
    if (uniqueID == nullptr)
    {
        return E_POINTER;
    }
    *uniqueID = 0;
    return E_NOTIMPL;
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::get_windowHandle(HWND* windowHandle)
{
    if (windowHandle == nullptr)
    {
        return E_POINTER;
    }
    *windowHandle = GetSafeHwnd();
    return S_OK;
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::get_indexInParent(long* indexInParent)
{
    if (indexInParent == nullptr)
    {
        return E_POINTER;
    }
    *indexInParent = 0;
    return E_NOTIMPL;
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::get_locale(IA2Locale* locale)
{
    if (locale == nullptr)
    {
        return E_POINTER;
    }
    locale->country = nullptr;
    locale->language = nullptr;
    locale->variant = nullptr;
    return E_NOTIMPL;
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::get_attributes(BSTR* attributes)
{
    if (attributes == nullptr)
    {
        return E_POINTER;
    }
    *attributes = nullptr;
    return E_NOTIMPL;
}

// +++++++++++++++++++++++++++++++++++
// Protected methods.
// +++++++++++++++++++++++++++++++++++
template<class TBase>
bool CWndWithCustomizedAccessible2Info<TBase>::InitAccPropServices()
{
    if (m_pAccPropServices != nullptr)
    {
        return true;
    }
    if (m_initAccPropServicesFailed)
    {
        return false;
    }
    // NOLINTNEXTLINE(hicpp-signed-bitwise)
    auto hr = m_pAccPropServices.CoCreateInstance(CLSID_AccPropServices, nullptr, CLSCTX_SERVER);
    if (FAILED(hr) || !m_pAccPropServices)
    {
        m_initAccPropServicesFailed = false;
        return false;
    }
    return true;
}

// +++++++++++++++++++++++++++++++++++
// XAccessible2 class.
// +++++++++++++++++++++++++++++++++++
template<class TBase>
ULONG FAR EXPORT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::AddRef()
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->ExternalAddRef();
}
template<class TBase>
ULONG FAR EXPORT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::Release()
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->ExternalRelease();
}
template<class TBase>
HRESULT FAR EXPORT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::QueryInterface(REFIID iid, LPVOID* ppvObj)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return static_cast<HRESULT>(pThis->ExternalQueryInterface(&iid, ppvObj));
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::Invoke(/*[in]*/ DISPID dispIdMember, /*[in]*/ REFIID refiid,
    /*[in]*/ LCID lcid, /*[in]*/ WORD wFlags, /*[out][in]*/ DISPPARAMS* pDispParams, /*[out]*/ VARIANT* pVarResult,
    /*[out]*/ EXCEPINFO* pExcepInfo, /*[out]*/ UINT* puArgErr)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return AtlIAccessibleInvokeHelper(reinterpret_cast<IAccessible2*>(reinterpret_cast<void*>(this)), dispIdMember, refiid,
		lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::GetIDsOfNames(REFIID refiid, LPOLESTR* rgszNames, UINT cNames,
    LCID lcid, DISPID* rgDispId)
{
    return AtlIAccessibleGetIDsOfNamesHelper(refiid, rgszNames, cNames, lcid, rgDispId);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::GetTypeInfoCount(unsigned int* pctinfo)
{
    if (pctinfo == nullptr)
    {
        return E_POINTER;
    }
    *pctinfo = 1;
    return S_OK;
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::GetTypeInfo(unsigned int /*iTInfo*/, LCID /*lcid*/,
	ITypeInfo** /*ppTInfo*/)
{
    return E_NOTIMPL;
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_accParent(IDispatch** ppdispParent)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_accParent(ppdispParent);
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_accChildCount(long* pcountChildren)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_accChildCount(pcountChildren);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_accChild(VARIANT varChild, IDispatch** ppdispChild)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_accChild(varChild, ppdispChild);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_accName(VARIANT varChild, BSTR* pszName)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_accName(varChild, pszName);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_accValue(VARIANT varChild, BSTR* pszValue)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_accValue(varChild, pszValue);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_accDescription(VARIANT varChild, BSTR* pszDescription)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_accDescription(varChild, pszDescription);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_accRole(VARIANT varChild, VARIANT* pvarRole)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_accRole(varChild, pvarRole);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_accState(VARIANT varChild, VARIANT* pvarState)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_accState(varChild, pvarState);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_accHelp(VARIANT varChild, BSTR* pszHelp)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_accHelp(varChild, pszHelp);
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_accHelpTopic(BSTR* pszHelpFile, VARIANT varChild, long* pidTopic)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_accHelpTopic(pszHelpFile, varChild, pidTopic);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_accKeyboardShortcut(VARIANT varChild, BSTR* pszKeyboardShortcut)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_accKeyboardShortcut(varChild, pszKeyboardShortcut);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_accFocus(VARIANT* pvarChild)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_accFocus(pvarChild);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_accSelection(VARIANT* pvarChildren)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_accSelection(pvarChildren);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_accDefaultAction(VARIANT varChild, BSTR* pszDefaultAction)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_accDefaultAction(varChild, pszDefaultAction);
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::accSelect(long flagsSelect, VARIANT varChild)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->accSelect(flagsSelect, varChild);
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::accLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->accLocation(pxLeft, pyTop, pcxWidth, pcyHeight, varChild);
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::accNavigate(long navDir, VARIANT varStart, VARIANT* pvarEndUpAt)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->accNavigate(navDir, varStart, pvarEndUpAt);
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::accHitTest(long xLeft, long yTop, VARIANT* pvarChild)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->accHitTest(xLeft, yTop, pvarChild);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::accDoDefaultAction(VARIANT varChild)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->accDoDefaultAction(varChild);
}
// Obsolete
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::put_accName(VARIANT varChild, BSTR szName)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->put_accName(varChild, szName);
}
// Obsolete
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::put_accValue(VARIANT varChild, BSTR szValue)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->put_accValue(varChild, szValue);
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_nRelations(long* nRelations)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_nRelations(nRelations);
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_relation(long relationIndex, IAccessibleRelation** relation)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_relation(relationIndex, relation);
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_relations(long maxRelations, IAccessibleRelation** relations, long* nRelations)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_relations(maxRelations, relations, nRelations);
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::role(long* role)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->role(role);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::scrollTo(enum IA2ScrollType scrollType)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->scrollTo(scrollType);
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::scrollToPoint(enum IA2CoordinateType coordinateType, long x, long y)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->scrollToPoint(coordinateType, x, y);
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_groupPosition(long* groupLevel, long* similarItemsInGroup, long* positionInGroup)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_groupPosition(groupLevel, similarItemsInGroup, positionInGroup);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_states(AccessibleStates* states)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_states(states);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_extendedRole(BSTR* extendedRole)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_extendedRole(extendedRole);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_localizedExtendedRole(BSTR* localizedExtendedRole)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_localizedExtendedRole(localizedExtendedRole);
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_nExtendedStates(long* nExtendedStates)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_nExtendedStates(nExtendedStates);
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_extendedStates(long maxExtendedStates, BSTR** extendedStates, long* nExtendedStates)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_extendedStates(maxExtendedStates, extendedStates, nExtendedStates);
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_localizedExtendedStates(long maxLocalizedExtendedStates,
    // NOLINTNEXTLINE(google-runtime-int)
	BSTR** localizedExtendedStates, long* nLocalizedExtendedStates)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_localizedExtendedStates(
        maxLocalizedExtendedStates, localizedExtendedStates, nLocalizedExtendedStates);
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_uniqueID(long* uniqueID)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_uniqueID(uniqueID);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_windowHandle(HWND* windowHandle)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_windowHandle(windowHandle);
}
template<class TBase>
// NOLINTNEXTLINE(google-runtime-int)
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_indexInParent(long* indexInParent)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_indexInParent(indexInParent);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_locale(IA2Locale* locale)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_locale(locale);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2::get_attributes(BSTR* attributes)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2)
    return pThis->get_attributes(attributes);
}

// +++++++++++++++++++++++++++++++++++
// XAccessible2Server class.
// +++++++++++++++++++++++++++++++++++
template<class TBase>
ULONG FAR EXPORT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2Server::AddRef()
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2Server)
    return pThis->ExternalAddRef();
}
template<class TBase>
ULONG FAR EXPORT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2Server::Release()
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2Server)
    return pThis->ExternalRelease();
}
template<class TBase>
HRESULT FAR EXPORT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2Server::QueryInterface(REFIID iid, void FAR* FAR* ppvObj)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2Server)
    return static_cast<HRESULT>(
        pThis->ExternalQueryInterface(&iid, ppvObj));
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2Server::SetProxy(IAccessible2Proxy* pProxy)
{
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2Server)
    return pThis->SetProxy(pProxy);
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2Server::GetHWND(HWND *phWnd)
{
    if (phWnd == nullptr)
    {
        return E_POINTER;
    }
    // NOLINTNEXTLINE(clang-diagnostic-cast-align,clang-diagnostic-invalid-offsetof,cppcoreguidelines-pro-type-cstyle-cast)
    METHOD_PROLOGUE(CWndWithCustomizedAccessible2Info<TBase>, Accessible2Server)
    *phWnd = pThis->m_hWnd;
    return S_OK;
}
template<class TBase>
HRESULT CWndWithCustomizedAccessible2Info<TBase>::XAccessible2Server::GetEnumVariant(IEnumVARIANT **ppEnumVariant)
{
    if (ppEnumVariant == nullptr)
    {
        return E_POINTER;
    }
    *ppEnumVariant = nullptr;
    return E_NOTIMPL;
}

// Explicit template instantiation
template class CWndWithCustomizedAccessible2Info<CButton>;
template class CWndWithCustomizedAccessible2Info<CEdit>;
template class CWndWithCustomizedAccessible2Info<CComboBox>;
template class CWndWithCustomizedAccessible2Info<CLinkCtrl>;
template class CWndWithCustomizedAccessible2Info<CRichEditCtrl>;
template class CWndWithCustomizedAccessible2Info<CSliderCtrl>;
template class CWndWithCustomizedAccessible2Info<CWnd>;
// NOLINTEND(clang-analyzer-optin.core.EnumCastOutOfRange)
