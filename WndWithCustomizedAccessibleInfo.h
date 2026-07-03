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

#pragma once

#include <afxpriv.h>
#include <oleacc.h>
#include <UIAutomation.h>

#include <array>
#include <string>
#include <boost/format.hpp>
#include <boost/numeric/conversion/cast.hpp>

template<class TBase>
class CWndWithCustomizedAccessibleInfo:
    public TBase
{
    inline std::wstring loadString(UINT uID)
    {
        using namespace std::string_literals;
        constexpr size_t maxStringTableResourceLen{4096};
        std::array<wchar_t, maxStringTableResourceLen> buffer{};
        try
        {
            int result{AfxLoadString(uID, buffer.data(), boost::numeric_cast<UINT>(buffer.size()))};
            if (result == 0 || buffer.at(0) == 0)
            {
                return {};
            }
        }
        catch (boost::bad_numeric_cast&)
        {
            return {};
        }
        return buffer.data();
    }

    template<typename... Arguments>
    inline std::wstring FormatArgs(const std::wstring& fmt, const Arguments&... args)
    {
        // NOLINTNEXTLINE(clang-diagnostic-sign-conversion)
        return boost::str((boost::wformat(fmt) % ... % args));
    }

    template<typename... Arguments>
    inline std::wstring FormatArgs(UINT fmt, const Arguments&... args)
    {
        using namespace std::string_literals;
        const auto formatStr{loadString(fmt)};
        if (formatStr.empty())
        {
            return {};
        }
        // NOLINTNEXTLINE(clang-diagnostic-sign-conversion)
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
    CWndWithCustomizedAccessibleInfo() noexcept;
    // NOLINTNEXTLINE(hicpp-use-override,modernize-use-override)
    ~CWndWithCustomizedAccessibleInfo() override = default;

    // +++++++++++++++++++++++++++++++++++
    // Disallow copy and assignment.
    // +++++++++++++++++++++++++++++++++++

    CWndWithCustomizedAccessibleInfo(const CWndWithCustomizedAccessibleInfo&) = delete;
    CWndWithCustomizedAccessibleInfo(CWndWithCustomizedAccessibleInfo&&) = delete;
    CWndWithCustomizedAccessibleInfo& operator=(const CWndWithCustomizedAccessibleInfo&) = delete;
    CWndWithCustomizedAccessibleInfo& operator=(CWndWithCustomizedAccessibleInfo&&) = delete;

    // +++++++++++++++++++++++++++++++++++
    // Utility.
    // +++++++++++++++++++++++++++++++++++
    bool ClearProperty(VARIANT varChild, MSAAPROPID idProp);

    // +++++++++++++++++++++++++++++++++++
    // Customization: Functions that make it possible to override the behavior
    // exhibited by the default CWnd implementation of some of the functions in
    // the IAccessible interface.
    // +++++++++++++++++++++++++++++++++++

    bool SetAccessibleDescription(VARIANT varChild, LPCWSTR description);
    bool SetAccessibleDescription(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetAccessibleDescriptionF(
        VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto description{FormatArgs(format, args...)};
        if (description.empty())
        {
            return false;
        }
        return SetAccessibleDescription(varChild, description.c_str());
    }
    template<typename... Arguments>
    bool SetAccessibleDescriptionF(
        VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto description{FormatArgs(format, args...)};
        if (description.empty())
        {
            return false;
        }
        return SetAccessibleDescription(varChild, description.c_str());
    }
    bool SetAccessibleHelp(VARIANT varChild, LPCWSTR help);
    bool SetAccessibleHelp(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetAccessibleHelpF(
        VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto help{FormatArgs(format, args...)};
        if (help.empty())
        {
            return false;
        }
        return SetAccessibleHelp(varChild, help.c_str());
    }
    template<typename... Arguments>
    bool SetAccessibleHelpF(
        VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto help{FormatArgs(format, args...)};
        if (help.empty())
        {
            return false;
        }
        return SetAccessibleHelp(varChild, help.c_str());
    }
    bool SetAccessibleName(VARIANT varChild, LPCWSTR name);
    bool SetAccessibleName(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetAccessibleNameF(
        VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto name{FormatArgs(format, args...)};
        if (name.IsEmpty())
        {
            return false;
        }
        return SetAccessibleName(varChild, name.c_str());
    }
    template<typename... Arguments>
    bool SetAccessibleNameF(
        VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto name{FormatArgs(format, args...)};
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
    bool SetAccessibleValueF(
        VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto value{FormatArgs(format, args...)};
        if (value.empty())
        {
            return false;
        }
        return SetAccessibleValue(varChild, value.c_str());
    }
    template<typename... Arguments>
    bool SetAccessibleValueF(
        VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto value{FormatArgs(format, args...)};
        if (value.empty())
        {
            return false;
        }
        return SetAccessibleValue(varChild, value);
    }


    // +++++++++++++++++++++++++++++++++++
    // UI Automation.
    // +++++++++++++++++++++++++++++++++++

    bool SetControlTypeProperty(VARIANT varChild, LONG controlType);

    bool SetFrameworkIdProperty(VARIANT varChild, LPCWSTR value);
    bool SetFrameworkIdProperty(VARIANT varChild, UINT nID);

    bool SetFullDescriptionProperty(VARIANT varChild, LPCWSTR description);
    bool SetFullDescriptionProperty(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetFullDescriptionPropertyF(VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto description{FormatArgs(format, args...)};
        if (description.empty())
        {
            return false;
        }
        return SetFullDescriptionProperty(varChild, description.c_str());
    }
    template<typename... Arguments>
    bool SetFullDescriptionPropertyF(VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto description{FormatArgs(format, args...)};
        if (description.empty())
        {
            return false;
        }
        return SetFullDescriptionProperty(varChild, description.c_str());
    }

    bool SetHeadingLevelProperty(VARIANT varChild, long headingLevel);

    bool SetHelpTextProperty(VARIANT varChild, LPCWSTR helpText);
    bool SetHelpTextProperty(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetHelpTextPropertyF(VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto helpText{FormatArgs(format, args...)};
        if (helpText.empty())
        {
            return false;
        }
        return SetHelpTextProperty(varChild, helpText.c_str());
    }
    template<typename... Arguments>
    bool SetHelpTextPropertyF(VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto helpText{FormatArgs(format, args...)};
        if (helpText.empty())
        {
            return false;
        }
        return SetHelpTextProperty(varChild, helpText.c_str());
    }

    bool SetIsContentElementProperty(VARIANT varChild, bool value);
    bool SetIsControlElementProperty(VARIANT varChild, bool value);
    bool SetIsDataValidForFormProperty(VARIANT varChild, bool value);
    bool SetIsDialogProperty(VARIANT varChild, bool value);
    bool SetIsPasswordProperty(VARIANT varChild, bool value);
    bool SetIsRequiredForFormProperty(VARIANT varChild, bool value);

    bool SetItemStatusProperty(VARIANT varChild, LPCWSTR itemStatus);
    bool SetItemStatusProperty(VARIANT varChild, UINT nID);

    bool SetItemTypeProperty(VARIANT varChild, LPCWSTR itemType);
    bool SetItemTypeProperty(VARIANT varChild, UINT nID);

    bool SetLevelProperty(VARIANT varChild, long level);

    bool SetLocalizedControlTypeProperty(VARIANT varChild, LPCWSTR name);
    bool SetLocalizedControlTypeProperty(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetLocalizedControlTypePropertyF(VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto name{FormatArgs(format, args...)};
        if (name.empty())
        {
            return false;
        }
        return SetLocalizedControlTypeProperty(varChild, name.c_str());
    }
    template<typename... Arguments>
    bool SetLocalizedControlTypePropertyF(VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto name{FormatArgs(format, args...)};
        if (name.empty())
        {
            return false;
        }
        return SetLocalizedControlTypeProperty(varChild, name.c_str());
    }

    bool SetNameProperty(VARIANT varChild, LPCWSTR name);
    bool SetNameProperty(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetNamePropertyF(VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto name{FormatArgs(format, args...)};
        if (name.empty())
        {
            return false;
        }
        return SetNameProperty(varChild, name.c_str());
    }
    template<typename... Arguments>
    bool SetNamePropertyF(VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto name{FormatArgs(format, args...)};
        if (name.empty())
        {
            return false;
        }
        return SetNameProperty(varChild, name.c_str());
    }

    bool SetPositionInSetProperty(VARIANT varChild, long positionInSet);

    bool SetRangeValueProviderIsReadOnlyProperty(VARIANT varChild, bool value);
    bool SetRangeValueProviderLargeChangeProperty(VARIANT varChild, double value);
    bool SetRangeValueProviderMaximumProperty(VARIANT varChild, double value);
    bool SetRangeValueProviderMinimumProperty(VARIANT varChild, double value);
    bool SetRangeValueProviderSmallChangeProperty(VARIANT varChild, double value);
    bool SetRangeValueProviderValueProperty(VARIANT varChild, double value);

    bool SetSizeOfSetProperty(VARIANT varChild, long sizeOfSet);

    bool SetValueProviderIsReadOnlyProperty(VARIANT varChild, bool value);

    bool SetValueProviderValueProperty(VARIANT varChild, LPCWSTR value);
    bool SetValueProviderValueProperty(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetValueProviderValuePropertyF(
        VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto value{FormatArgs(format, args...)};
        if (value.empty())
        {
            return false;
        }
        return SetValueProviderValueProperty(varChild, value.c_str());
    }
    template<typename... Arguments>
    bool SetValueProviderValuePropertyF(
        VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto value{FormatArgs(format, args...)};
        if (value.empty())
        {
            return false;
        }
        return SetValueProviderValueProperty(varChild, value);
    }

    // +++++++++++++++++++++++++++++++++++
    // CWnd method overrides.
    // +++++++++++++++++++++++++++++++++++

    HRESULT CreateAccessibleProxy(WPARAM wParam, LPARAM lParam, LRESULT* pResult) override;

    BOOL SafeRedrawWindow(
        LPCRECT lpRectUpdate = nullptr, CRgn* prgnUpdate = nullptr,
        UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

protected:
    using CWnd::m_xAccessible;
    using CWnd::m_xAccessibleServer;
    using CWnd::m_pProxy;
    bool CommonPrologue(VARIANT varChild);
    bool InitAccPropServices();
    HRESULT ClearAccessibilityPropertyWorker(DWORD idChild, MSAAPROPID idProp);
    HRESULT SetAccessibilityPropertyWorker(DWORD idChild, MSAAPROPID idProp, LPCWSTR value);
    HRESULT SetAccessibilityPropertyWorker(DWORD idChild, MSAAPROPID idProp, VARIANT value);
    HRESULT SetAccessibilityPropertyWorker(DWORD idChild, MSAAPROPID idProp, bool value);

private:
    CComPtr<IAccPropServices> m_pAccPropServices{nullptr};
    bool m_initAccPropServicesFailed{false};
};
