#include "pch.h"
#include "VSMUC.xaml.h"
#if __has_include("VSMUC.g.cpp")
#include "VSMUC.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::HelloWorldCppWindowsAppSDK::implementation
{
    VSMUC::VSMUC()
    {
        InitializeComponent();

        VisualStateManager::GoToState(*this, L"DefaultOrientation", false);
        VisualStateManager::GoToState(*this, L"DefaultSize", false);

        SizeChanged({ this, &VSMUC::SizeChangedHandler });
    }

    void VSMUC::SizeChangedHandler(IInspectable const&, SizeChangedEventArgs const& e)
    {
        if (e.NewSize().Width < e.NewSize().Height)
        {
            VisualStateManager::GoToState(*this, L"PortraitOrientation", false);
        }
        else
        {
            VisualStateManager::GoToState(*this, L"DefaultOrientation", false);
        }

        if (e.NewSize().Width < 950 || e.NewSize().Height < 500)
        {
            VisualStateManager::GoToState(*this, L"SmallSize", false);
        }
        else
        {
            VisualStateManager::GoToState(*this, L"DefaultSize", false);
        }
    }

    winrt::fire_and_forget VSMUC::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        auto showDialog{ Windows::UI::Popups::MessageDialog(L"Please be sure to pick a folder that contains files") };
        VSMUC::ParentDialogToWindow(showDialog);
        co_await showDialog.ShowAsync();

        auto folderPicker{ Windows::Storage::Pickers::FolderPicker() };
        VSMUC::ParentDialogToWindow(folderPicker);
        folderPicker.SuggestedStartLocation(Windows::Storage::Pickers::PickerLocationId::ComputerFolder);
        folderPicker.FileTypeFilter().Append(L"*");

        Windows::Storage::StorageFolder folder{ co_await folderPicker.PickSingleFolderAsync() };
        if (folder)
        {
            // Application now has read/write access to all contents in the picked folder
            // (including other sub-folder contents)
            Windows::Storage::AccessCache::StorageApplicationPermissions::FutureAccessList().AddOrReplace(L"PickedFolderToken", folder);

            // Get the first 3 files in the folder.
            Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFile> filesInFolder{
                co_await folder.GetFilesAsync(Windows::Storage::Search::CommonFileQuery::DefaultQuery, 0, 3) };

            co_await winrt::resume_foreground(this->DispatcherQueue());
            fileListLB().Items().Clear();

            for (Windows::Storage::StorageFile const& fileInFolder : filesInFolder)
            {
                fileListLB().Items().Append(winrt::box_value(fileInFolder.Name().c_str()));
            }
        }
    }

    void VSMUC::ParentDialogToWindow(IInspectable const& dialog)
    {
        // Helper function for non-CoreWindow.
        auto initializeWithWindow = dialog.as<::IInitializeWithWindow>();
        initializeWithWindow->Initialize(App::WindowHandle());
    }
}
