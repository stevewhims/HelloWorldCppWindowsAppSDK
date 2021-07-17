#pragma once

#pragma push_macro("GetCurrentTime")
#undef GetCurrentTime

#include "App.xaml.g.h"

#pragma pop_macro("GetCurrentTime")

namespace winrt::HelloWorldCppWindowsAppSDK::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);
        void OnSuspending(IInspectable const&, Windows::ApplicationModel::SuspendingEventArgs const&);

        static HWND WindowHandle() { return m_hWnd; }

    private:
        winrt::Microsoft::UI::Xaml::Window window{ nullptr };
        static HWND m_hWnd;
    };
}
