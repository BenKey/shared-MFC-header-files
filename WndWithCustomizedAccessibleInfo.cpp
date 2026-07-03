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

// NOLINTBEGIN(clang-analyzer-optin.core.EnumCastOutOfRange)
#include "stdafx.h"
#include <afxwin.h>

#include "WndWithCustomizedAccessibleInfo.h"
#include "AccessibleProxyWithAccessibleApplicationSupport.h"

namespace {

VARIANT to_VARIANT(bool value)
{
    VARIANT ret{};
    VariantInit(&ret);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    ret.vt = VT_BOOL;
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    ret.boolVal = value ? VARIANT_TRUE : VARIANT_FALSE;
    return ret;
}

} // anonymous namespace

// +++++++++++++++++++++++++++++++++++
// Construction/Destruction
// +++++++++++++++++++++++++++++++++++

template <class TBase>
CWndWithCustomizedAccessibleInfo<TBase>::CWndWithCustomizedAccessibleInfo() noexcept:
    TBase()
{
    EnableActiveAccessibility();
}

// +++++++++++++++++++++++++++++++++++
// Utility.
// +++++++++++++++++++++++++++++++++++
template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::ClearProperty(VARIANT varChild, MSAAPROPID idProp)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{ClearAccessibilityPropertyWorker(varChild.ulVal, idProp)};
    return SUCCEEDED(hr);

}

// +++++++++++++++++++++++++++++++++++
// Customization: Functions that make it possible to override the behavior
// exhibited by the default CWnd implementation of some of the functions in
// the IAccessible interface.
// +++++++++++++++++++++++++++++++++++

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetAccessibleDescription(VARIANT varChild, LPCWSTR description)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    if (description == nullptr || description[0] == 0)
    {
        return false;
    }
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr = SetAccessibilityPropertyWorker(static_cast<DWORD>(varChild.lVal), PROPID_ACC_DESCRIPTION, description);
    if (SUCCEEDED(hr))
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
        NotifyWinEvent(EVENT_OBJECT_DESCRIPTIONCHANGE, OBJID_CLIENT, varChild.lVal);
    }
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetAccessibleDescription(VARIANT varChild, UINT nID)
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
bool CWndWithCustomizedAccessibleInfo<TBase>::SetAccessibleHelp(VARIANT varChild, LPCWSTR help)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    if (help == nullptr || help[0] == 0)
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr = SetAccessibilityPropertyWorker(static_cast<DWORD>(varChild.lVal), PROPID_ACC_HELP, help);
    if (SUCCEEDED(hr))
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
        NotifyWinEvent(EVENT_OBJECT_HELPCHANGE, OBJID_CLIENT, varChild.lVal);
    }
    return SUCCEEDED(hr);
}

template<class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetAccessibleHelp(VARIANT varChild, UINT nID)
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

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetAccessibleName(VARIANT varChild, LPCWSTR name)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    if (name == nullptr || name[0] == 0)
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr = SetAccessibilityPropertyWorker(static_cast<DWORD>(varChild.lVal), PROPID_ACC_NAME, name);
    if (SUCCEEDED(hr))
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
        NotifyWinEvent(EVENT_OBJECT_NAMECHANGE, OBJID_CLIENT, varChild.lVal);
    }
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetAccessibleName(VARIANT varChild, UINT nID)
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

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetAccessibleRole(VARIANT varChild, LONG role)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    if (role == 0)
    {
        return false;
    }
    CComVariant var(role);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr = SetAccessibilityPropertyWorker(static_cast<DWORD>(varChild.lVal), PROPID_ACC_ROLE, var);
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetAccessibleState(VARIANT varChild, LONG state, bool overrideMSAA)
{
    if (!CommonPrologue(varChild))
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
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr = SetAccessibilityPropertyWorker(static_cast<DWORD>(varChild.lVal), PROPID_ACC_STATE, var);
    if (SUCCEEDED(hr))
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
        NotifyWinEvent(EVENT_OBJECT_STATECHANGE, OBJID_CLIENT, varChild.lVal);
    }
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetAccessibleValue(VARIANT varChild, LPCWSTR value)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    if (value == nullptr || value[0] == 0)
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr = SetAccessibilityPropertyWorker(static_cast<DWORD>(varChild.lVal), PROPID_ACC_VALUE, value);
    if (SUCCEEDED(hr))
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
        NotifyWinEvent(EVENT_OBJECT_VALUECHANGE, OBJID_CLIENT, varChild.lVal);
    }
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetAccessibleValue(VARIANT varChild, UINT nID)
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
// UI Automation.
// +++++++++++++++++++++++++++++++++++

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetControlTypeProperty(VARIANT varChild, LONG controlType)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    CComVariant var{controlType};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, ControlType_Property_GUID, var)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetFrameworkIdProperty(VARIANT varChild, LPCWSTR value)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    if (value == nullptr || value[0] == 0)
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, FrameworkId_Property_GUID, value)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetFrameworkIdProperty(VARIANT varChild, UINT nID)
{
    if (nID == 0)
    {
        return false;
    }
    const auto value{loadString(nID)};
    if (value.empty())
    {
        return false;
    }
    return SetFrameworkIdProperty(varChild, value.c_str());
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetFullDescriptionProperty(VARIANT varChild, LPCWSTR description)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    if (description == nullptr || description[0] == 0)
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, FullDescription_Property_GUID, description)};
    if (SUCCEEDED(hr))
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
        NotifyWinEvent(EVENT_OBJECT_DESCRIPTIONCHANGE, OBJID_CLIENT, varChild.lVal);
    }
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetFullDescriptionProperty(VARIANT varChild, UINT nID)
{
    if (nID == 0)
    {
        return false;
    }
    const auto description{loadString(nID)};
    if (description.empty())
    {
        return false;
    }
    return SetFullDescriptionProperty(varChild, description.c_str());
}

template <class TBase>
// NOLINTNEXTLINE(google-runtime-int)
bool CWndWithCustomizedAccessibleInfo<TBase>::SetHeadingLevelProperty(VARIANT varChild, long headingLevel)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    if (headingLevel < HeadingLevel_None || headingLevel > HeadingLevel9)
    {
        return false;
    }
    CComVariant var(headingLevel);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, HeadingLevel_Property_GUID, var)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetHelpTextProperty(VARIANT varChild, LPCWSTR helpText)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    if (helpText == nullptr || helpText[0] == 0)
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, HelpText_Property_GUID, helpText)};
    if (SUCCEEDED(hr))
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
        NotifyWinEvent(EVENT_OBJECT_HELPCHANGE, OBJID_CLIENT, varChild.lVal);
    }
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetHelpTextProperty(VARIANT varChild, UINT nID)
{
    if (nID == 0)
    {
        return false;
    }
    const auto helpText{loadString(nID)};
    if (helpText.empty())
    {
        return false;
    }
    return SetHelpTextProperty(varChild, helpText.c_str());
}


template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetIsContentElementProperty(VARIANT varChild, bool value)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, IsContentElement_Property_GUID, value)};
    return SUCCEEDED(hr);
}


template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetIsControlElementProperty(VARIANT varChild, bool value)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, IsControlElement_Property_GUID, value)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetIsDataValidForFormProperty(VARIANT varChild, bool value)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, IsDataValidForForm_Property_GUID, value)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetIsDialogProperty(VARIANT varChild, bool value)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, IsDialog_Property_GUID, value)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetIsPasswordProperty(VARIANT varChild, bool value)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, IsPassword_Property_GUID, value)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetIsRequiredForFormProperty(VARIANT varChild, bool value)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, IsRequiredForForm_Property_GUID, value)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetItemStatusProperty(VARIANT varChild, LPCWSTR itemStatus)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    if (itemStatus == nullptr || itemStatus[0] == 0)
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, ItemStatus_Property_GUID, itemStatus)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetItemStatusProperty(VARIANT varChild, UINT nID)
{
    if (nID == 0)
    {
        return false;
    }
    const auto itemStatus{loadString(nID)};
    if (itemStatus.empty())
    {
        return false;
    }
    return SetItemStatusProperty(varChild, itemStatus.c_str());
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetItemTypeProperty(VARIANT varChild, LPCWSTR itemType)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    if (itemType == nullptr || itemType[0] == 0)
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, ItemType_Property_GUID, itemType)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetItemTypeProperty(VARIANT varChild, UINT nID)
{
    if (nID == 0)
    {
        return false;
    }
    const auto itemType{loadString(nID)};
    if (itemType.empty())
    {
        return false;
    }
    return SetItemTypeProperty(varChild, itemType.c_str());
}

template <class TBase>
// NOLINTNEXTLINE(google-runtime-int)
bool CWndWithCustomizedAccessibleInfo<TBase>::SetLevelProperty(VARIANT varChild, long level)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    CComVariant var{level};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, Level_Property_GUID, var)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetLocalizedControlTypeProperty(VARIANT varChild, LPCWSTR name)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    if (name == nullptr || name[0] == 0)
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, LocalizedControlType_Property_GUID, name)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetLocalizedControlTypeProperty(VARIANT varChild, UINT nID)
{
    if (nID == 0)
    {
        return false;
    }
    const auto type{loadString(nID)};
    if (type.empty())
    {
        return false;
    }
    return SetLocalizedControlTypeProperty(varChild, type.c_str());
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetNameProperty(VARIANT varChild, LPCWSTR name)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    if (name == nullptr || name[0] == 0)
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, Name_Property_GUID, name)};
    if (SUCCEEDED(hr))
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
        NotifyWinEvent(EVENT_OBJECT_NAMECHANGE, OBJID_CLIENT, varChild.lVal);
    }
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetNameProperty(VARIANT varChild, UINT nID)
{
    if (nID == 0)
    {
        return false;
    }
    const auto name{loadString(nID)};
    if (name.empty())
    {
        return false;
    }
    return SetNameProperty(varChild, name.c_str());
}

template <class TBase>
// NOLINTNEXTLINE(google-runtime-int)
bool CWndWithCustomizedAccessibleInfo<TBase>::SetPositionInSetProperty(VARIANT varChild, long positionInSet)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    CComVariant var{positionInSet};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, PositionInSet_Property_GUID, var)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetRangeValueProviderIsReadOnlyProperty(VARIANT varChild, bool value)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, RangeValue_IsReadOnly_Property_GUID, value)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetRangeValueProviderLargeChangeProperty(VARIANT varChild, double value)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    CComVariant var{value};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, RangeValue_LargeChange_Property_GUID, var)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetRangeValueProviderMaximumProperty(VARIANT varChild, double value)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    CComVariant var{value};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, RangeValue_Maximum_Property_GUID, var)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetRangeValueProviderMinimumProperty(VARIANT varChild, double value)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    CComVariant var{value};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, RangeValue_Minimum_Property_GUID, var)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetRangeValueProviderSmallChangeProperty(VARIANT varChild, double value)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    CComVariant var{value};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, RangeValue_SmallChange_Property_GUID, var)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetRangeValueProviderValueProperty(VARIANT varChild, double value)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    CComVariant var{value};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, RangeValue_Value_Property_GUID, var)};
    return SUCCEEDED(hr);
}

template <class TBase>
// NOLINTNEXTLINE(google-runtime-int)
bool CWndWithCustomizedAccessibleInfo<TBase>::SetSizeOfSetProperty(VARIANT varChild, long sizeOfSet)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    CComVariant var{sizeOfSet};
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, SizeOfSet_Property_GUID, var)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetValueProviderIsReadOnlyProperty(VARIANT varChild, bool value)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, Value_IsReadOnly_Property_GUID, value)};
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetValueProviderValueProperty(VARIANT varChild, LPCWSTR value)
{
    if (!CommonPrologue(varChild))
    {
        return false;
    }
    if (value == nullptr || value[0] == 0)
    {
        return false;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    auto hr{SetAccessibilityPropertyWorker(varChild.ulVal, Value_Value_Property_GUID, value)};
    if (SUCCEEDED(hr))
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
        NotifyWinEvent(EVENT_OBJECT_VALUECHANGE, OBJID_CLIENT, varChild.lVal);
    }
    return SUCCEEDED(hr);
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::SetValueProviderValueProperty(VARIANT varChild, UINT nID)
{
    if (nID == 0)
    {
        return false;
    }
    const auto itemStatus{loadString(nID)};
    if (itemStatus.empty())
    {
        return false;
    }
    return SetValueProviderValueProperty(varChild, itemStatus.c_str());
}

// +++++++++++++++++++++++++++++++++++
// CWnd method overrides.
// +++++++++++++++++++++++++++++++++++

template <class TBase>
HRESULT CWndWithCustomizedAccessibleInfo<TBase>::CreateAccessibleProxy(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    ASSERT(pResult != nullptr);
    if (pResult == nullptr)
    {
        return E_POINTER;
    }
    auto ret = CreateAccessibleProxyWithAccessibleApplicationSupportHelper(
        wParam, lParam, this, reinterpret_cast<IAccessible*>(&m_xAccessible),
        reinterpret_cast<IAccessibleServer*>(&m_xAccessibleServer), pResult, &m_pProxy);
    return ret;
}

template <class TBase>
BOOL CWndWithCustomizedAccessibleInfo<TBase>::SafeRedrawWindow(LPCRECT lpRectUpdate /* = nullptr */,
    CRgn* prgnUpdate /* = nullptr */, UINT flags /* = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE */)
{
    ASSERT(::IsWindow(m_hWnd));
    HRGN hrgnUpdate = nullptr;
    if (prgnUpdate != nullptr)
    {
        hrgnUpdate = static_cast<HRGN>(prgnUpdate->GetSafeHandle());
    }
    return ::RedrawWindow(m_hWnd, lpRectUpdate, hrgnUpdate, flags);
}

// +++++++++++++++++++++++++++++++++++
// Protected methods.
// +++++++++++++++++++++++++++++++++++

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::CommonPrologue(VARIANT varChild)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
    if (varChild.vt != VT_I4)
    {
        return false;
    }
    return InitAccPropServices();
}

template <class TBase>
bool CWndWithCustomizedAccessibleInfo<TBase>::InitAccPropServices()
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

template <class TBase>
HRESULT CWndWithCustomizedAccessibleInfo<TBase>::ClearAccessibilityPropertyWorker(DWORD idChild, MSAAPROPID idProp)
{
    if (!m_pAccPropServices)
    {
        return E_POINTER;
    }
    std::array<MSAAPROPID, 1> props{idProp};
    // NOLINTNEXTLINE(portability-template-virtual-member-function)
    auto hr{m_pAccPropServices->ClearHwndProps(GetSafeHwnd(), static_cast<DWORD>(OBJID_CLIENT), idChild, props.data(),
        static_cast<int>(props.size()))};
    return hr;
}

template <class TBase>
HRESULT CWndWithCustomizedAccessibleInfo<TBase>::SetAccessibilityPropertyWorker(DWORD idChild, MSAAPROPID idProp, LPCWSTR value)
{
    if (!m_pAccPropServices)
    {
        return E_POINTER;
    }
    // NOLINTNEXTLINE(portability-template-virtual-member-function)
    auto hr{m_pAccPropServices->SetHwndPropStr(GetSafeHwnd(), static_cast<DWORD>(OBJID_CLIENT), idChild, idProp, value)};
    return hr;
}

template <class TBase>
HRESULT CWndWithCustomizedAccessibleInfo<TBase>::SetAccessibilityPropertyWorker(DWORD idChild, MSAAPROPID idProp, VARIANT value)
{
    if (!m_pAccPropServices)
    {
        return E_POINTER;
    }
    // NOLINTNEXTLINE(portability-template-virtual-member-function)
    auto hr{m_pAccPropServices->SetHwndProp(GetSafeHwnd(), static_cast<DWORD>(OBJID_CLIENT), idChild, idProp, value)};
    return hr;
}

template <class TBase>
HRESULT CWndWithCustomizedAccessibleInfo<TBase>::SetAccessibilityPropertyWorker(DWORD idChild, MSAAPROPID idProp, bool value)
{
    return SetAccessibilityPropertyWorker(idChild, idProp, to_VARIANT(value));
}

// Explicit template instantiation
template class CWndWithCustomizedAccessibleInfo<CButton>;
template class CWndWithCustomizedAccessibleInfo<CComboBox>;
template class CWndWithCustomizedAccessibleInfo<CLinkCtrl>;
template class CWndWithCustomizedAccessibleInfo<CRichEditCtrl>;
template class CWndWithCustomizedAccessibleInfo<CSliderCtrl>;
template class CWndWithCustomizedAccessibleInfo<CWnd>;
// NOLINTEND(clang-analyzer-optin.core.EnumCastOutOfRange)
