#pragma once

#include "winrt/Microsoft.UI.Xaml.h"
#include "winrt/Microsoft.UI.Xaml.Markup.h"
#include "winrt/Microsoft.UI.Xaml.Controls.Primitives.h"
#include "VSMUC.g.h"
#include <App.xaml.h>

namespace winrt::HelloWorldCppWindowsAppSDK::implementation
{
    struct VSMUC : VSMUCT<VSMUC>
    {
        VSMUC();

        winrt::fire_and_forget myButton_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void SizeChangedHandler(Windows::Foundation::IInspectable const&, Microsoft::UI::Xaml::SizeChangedEventArgs const&);

    private:
        static void ParentDialogToWindow(Windows::Foundation::IInspectable const&);
    };
}

namespace winrt::HelloWorldCppWindowsAppSDK::factory_implementation
{
    struct VSMUC : VSMUCT<VSMUC, implementation::VSMUC>
    {
    };
}
