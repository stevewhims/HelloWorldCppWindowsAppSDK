# A C++/WinRT Windows App SDK 0.8.1 Hello, World! app ported from a UWP app

This repo contains the result of taking a simple C++/WinRT UWP Hello, World! app ([HelloWorldCppUWP](https://github.com/stevewhims/HelloWorldCppUWP)), and porting it to the Windows App SDK.

## Install the Windows App SDK 0.8.1 VSIX

In [Visual Studio](https://visualstudio.microsoft.com/downloads/), click **Extensions** > **Manage Extensions** > **Installed**, search for *Project Reunion* (which is the code name of the Windows App SDK), and download the Project Reunion extension. Close and reopen Visual Studio, and follow the prompts to install the extension. For more info, see [0.8.1 is now available!](https://github.com/microsoft/WindowsAppSDK/releases/tag/v0.8.1).

## Create a new project

In Visual Studio, create a new project from the **Blank App, Packaged (WinUI 3 in Desktop)** project template. You can find that project template in the **Create a new project** dialog by choosing language: *C++*; platform: *Project Reunion*; project type: *Desktop* (which is referring to Win32 in this context) or *WinUI*. Name the project *HellowWorldCppWindowsAppSDK*, uncheck **Place solution and project in the same directory**, and target the most recent release (not preview) of the client operating system (19041 at time of writing).

You'll now see two projects in Solution Explorer&mdash;one is qualified as **(Desktop)**, and the other as **(Package)**. I'll refer to these as the *Desktop* and *Package* projects, respectively.

## Add asset files

In Solution Explorer, in the Package project under **Images**, add a new folder (not a filter) named *SampleMedia*. Open the Package project folder in File Explorer and place [the four images](https://github.com/stevewhims/HellowWorldCppWindowsAppSDK/tree/master/HellowWorldCppWindowsAppSDK/HellowWorldCppWindowsAppSDK%20(Package)/Images/SampleMedia) in that folder.

Back in Solution Explorer, in the Package project, add the existing images to the **SampleMedia** folder.

## UI

When porting the **VisualStateManager** (VSM) functionality from your UWP app, it's not possible to copy the VSM markup and imperative code out of **MainPage** and into **MainWindow**. That's because VSM needs to be in a **Control** in order to work. And **Window** isn't a **Control**. But **Page** and **UserControl** are **Control**s, so you need to factor the VSM functionality out into either a new **Page** or a new **UserControl** (and then embed that new **Page** or **UserControl** inside **MainWindow**).

To the Desktop project, add a new item of type **User Control (WinUI 3)**. Name it *VSMUC.xaml*.

From [HelloWorldCppUWP](https://github.com/stevewhims/HelloWorldCppUWP) in `MainPage.xaml`, select and copy the contents of the **Page** element (not the **Page** element itself; just its inner XML). In `VSMUC.xaml`, replace the contents of the **UserControl** element (not the **UserControl** element itself; just its inner XML) with the content you copied.

* In the markup you pasted, change the name of the **Button** click event handler from **ClickHandler** to **myButton_Click**.
* In the **Image** **Source** urls, change `Assets` to `Images`.
* In the **TextBlock**, change `(UWP)` to `(WAS)` (just so we know this is the Windows App SDK version of the app).

In `VSMUC.xaml.cpp`, delete the line of code inside **VSMUC::myButton_Click**.

In `MainWindow.xaml`, replace the contents of the **Window** element (not the **Window** element itself; just its inner XML) with the following markup.

```xaml
<!-- MainWindow.xaml -->
<local:VSMUC/>
```

In `MainWindow.xaml.cpp`, delete the line of code inside **MainWindow::myButton_Click**.

## Test

Before working on file activation, confirm that at this point the project builds and runs. Just make sure you can use the flip view to navigate to each of the four images.

## Add a file type association

From the Package project, open `Package.appxmanifest`. In **Declarations**, choose **File Type Associations**, and click **Add**. Set the following properties.

**Display name**: Hello File 2
**Name**: hellofile2
**File type**: .hi2

To register the file type association, build the app, launch it, and close it.

## Imperative code

### File type association feature

There's no need to add code to `App.h` nor `App.cpp` (we don't need to add an **App::OnFileActivated** handler like we did for **HelloWorldCppUWP**).

### Visual states

From [HelloWorldCppUWP](https://github.com/stevewhims/HelloWorldCppUWP) in `MainPage.h`, copy the declaration of **SizeChangedHandler**. Paste that into `VSMUC.xaml.h`.

From **HelloWorldCppUWP** in `MainPage.cpp`, copy the code you added to the constructor, and the definition of **SizeChangedHandler**. Paste that into `VSMUC.xaml.cpp`.

In the code you pasted, change the `Windows::UI::` namespace to `Microsoft::UI::`; and change `MainPage::` to `VSMUC::`.

### Button click handler

In `VSMUC.xaml.h` and `VSMUC.xaml.cpp`, change the return type of **myButton_Click** to **winrt::fire_and_forget**.

From [HelloWorldCppUWP](https://github.com/stevewhims/HelloWorldCppUWP) in `MainPage.cpp`, copy the definition of **ClickHandler** (not the function signature, just the code inside the curly brackets). Paste that into `VSMUC.xaml.cpp` as the definition of **myButton_Click**.

To port that code, make the following changes.

* In `pch.h`, add the following at the end of the file.
  
  ```cppwinrt
  // pch.h
  ...
  #include <winrt/Microsoft.UI.Dispatching.h>
  #include <microsoft.ui.dispatching.co_await.h>
  #include <microsoft.ui.xaml.window.h>
  #include <Shobjidl.h>
  #include <winrt/Windows.Storage.h>
  #include <winrt/Windows.Storage.AccessCache.h>
  #include <winrt/Windows.Storage.Pickers.h>
  #include <winrt/Windows.Storage.Search.h>
  #include <winrt/Windows.UI.Core.h>
  #include <winrt/Windows.UI.Popups.h>
  ```

* In `App.xaml.h` and `App.xaml.cpp`, add the following.
  
  ```cppwinrt
  // App.xaml.h
  ...
  struct App : AppT<App>
  {
    ...
    static HWND WindowHandle() { return m_hWnd; }

  private:
    ...
    static HWND m_hWnd;
  };
  ...

  // App.xaml.cpp
  ...
  HWND App::m_hWnd = 0;
  ...
  void App::OnLaunched(LaunchActivatedEventArgs const&)
  {
    ...
    auto windowNative { window.as<::IWindowNative>() };
    HWND hWnd { 0 };
    windowNative->get_WindowHandle(&hWnd);
    App::m_hWnd = hWnd;
  }
  ...
  ```

* In `VSMUC.xaml.h` and `VSMUC.xaml.cpp`, add the following.

  ```cppwinrt
  // VSMUC.xaml.h
  ...
  #include <App.xaml.h>
  ...
  struct VSMUC : VSMUCT<VSMUC>
  {
    ...
    private:
      static void ParentDialogToWindow(Windows::Foundation::IInspectable const&);
  };
  ...

  // VSMUC.xaml.cpp
  ...
  void VSMUC::ParentDialogToWindow(IInspectable const& dialog)
  {
    // Helper function for non-CoreWindow.
    auto initializeWithWindow = dialog.as<::IInitializeWithWindow>();
    initializeWithWindow->Initialize(App::WindowHandle());
  }
  ...
  ```

* In `VSMUC.xaml.cpp`, in **myButton_Click**, immediately after declaring *showDialog*, add the line of code `VSMUC::ParentDialogToWindow(showDialog);`
* In `VSMUC.xaml.cpp`, in **myButton_Click**, immediately after declaring *folderPicker*, add the line of code `VSMUC::ParentDialogToWindow(folderPicker);`
* In `VSMUC.xaml.cpp`, in **myButton_Click**, change `co_await winrt::resume_foreground(this->Dispatcher());` to `co_await winrt::resume_foreground(this->DispatcherQueue());`.

## Test again

In your local **Documents** folder (or anywhere else, if you like), create a new **Text Document** file, and name it *MyHelloFile2.hi2*.

Double-click on the file, and (after) confirm that the app launches and displays the UI. With the launched app still running, double-click on the file a second time, and confirm that a second instance is launched (multi-instancing).

Resize the window, and confirm that the correct visual states are applied.

Make sure you can use the flip view to navigate to each of the four images.

Click the button, dismiss the message dialog, pick a folder containing files, and confirm that the first three filenames are displayed in the list box.
