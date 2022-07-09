#include <iostream>
#include <filesystem>
#include <string>
#include "Tracker_Centralisation.h"
#include "flexgridsizer.h"

/* Troubleshoot

Severity	Code	Description	Project	File	Line	Suppression State
Error	LNK2019	unresolved external symbol main referenced in function "int __cdecl invoke_main(void)" (?invoke_main@@YAHXZ)	Tracker_Centralisation	C:\Users\Anthony\source\repos\Tracker_Centralisation\MSVCRTD.lib(exe_main.obj)	1

Configuration->Linker->System->Subsystem property to Windows (/SUBSYSTEM:WINDOWS).

*/

// $(wxwin)\include\msvc;$(wxwin)\include
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#pragma warning(disable:4996)

std::string get_current_date()
{
    // https://stackoverflow.com/questions/16357999/current-date-and-time-as-string/16358264
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];
    time(&rawtime);
    // Error C4996 'localtime': This function or variable may be unsafe.Consider using localtime_s instead.To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
    return buffer;
}
/*
int main()
{
    std::cout << "=======================================" << "\n";
    std::cout << "- Tracker_Centralisation GUI application" << "\n";
    std::cout << "- GUI Application Version: 1.0" << "\n";
    std::cout << "- Created By: Anthony-T-N." << "\n";
    std::cout << "- Current location of executable: " << std::filesystem::current_path() << "\n";
    std::cout << "=======================================" << "\n\n";

    std::string user_input;

    std::cout << get_current_date() << "\n";
    std::cout << "Input: " << "\n";
    std::cout << "> ";
    std::getline(std::cin, user_input);

    std::cout << "[!] END" << "\n";
    std::cout << "[!] Exiting..." << "\n\n";
    return 0;

}
*/

/*
Project Plan and Design

Brief : Centralise tracking of a number of activities.

[Version 1]

=== Minimum Functions ===
[-] Window that associate entered text/string with a time/date.
[-] Entered text recorded in csv file.
[-] Seperate buttons open seperate windows and accept different types of records.
[-] Optimized to be lightweight and run in the background.
[-] Fixed minimum window size.

*/

class Main_Application : public wxApp
{
public:
    virtual bool OnInit();
};

class Main_Frame : public wxFrame
{
    public:
        Main_Frame();
        wxTextCtrl* MainEditBox;
        wxButton* HelloWorld;

    private:
        void OnHello(wxCommandEvent& event);
        void OnMedia(wxCommandEvent& event);
        void OnExit(wxCommandEvent& event);
        void OnExercise(wxCommandEvent& event);
        void OnBookmark(wxCommandEvent& event);
};

enum
{
    ID_Hello = 1,
    TEXT_Main = wxID_HIGHEST + 1,
    BUTTON_Hello = wxID_HIGHEST + 1
};

const int ID_Media = 2;
const int ID_Bookmark = 3;

wxIMPLEMENT_APP(Main_Application);

bool Main_Application::OnInit()
{
    FlexGridSizer* fgs = new FlexGridSizer(wxT("FlexGridSizer"));
    // Create an instance of frame or window
    //Main_Frame* frame = new Main_Frame();
    // Show the window
    fgs->Show(true);
    
    return true;
}

Main_Frame::Main_Frame()
    : wxFrame(NULL, wxID_ANY, "Window_Title_Bar")
{
    HelloWorld = new wxButton(this, BUTTON_Hello, _T("Hello World"),
        // shows a button on this window
        wxDefaultPosition, wxDefaultSize, wxBU_LEFT); // with the text "hello World"

    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Main_Item...\tCtrl-H",
        "String shown in bottom status bar");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuExercise = new wxMenu;
    menuExercise->Append(wxID_ABOUT);

    wxMenu* menuFile_T = new wxMenu;
    menuFile_T->Append(ID_Media, "&Time_Record\tCtrl-H",
        "Bar: Record time");

    wxMenu* menu_bookmark = new wxMenu;
    menu_bookmark->Append(ID_Bookmark, "&Bookmark\tCtrl-H",
        "Bar: Bookmark");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&Tab_1");
    menuBar->Append(menuExercise, "&Exercise");
    menuBar->Append(menuFile_T, "&Media");
    menuBar->Append(menu_bookmark, "&Bookmark");

    SetMenuBar(menuBar);

    CreateStatusBar();
    // "Bottom_Status_Bar"
    SetStatusText(get_current_date());

    MainEditBox = new wxTextCtrl(this, TEXT_Main,
        wxT("Enter text here..."), wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxTE_RICH, wxDefaultValidator, wxTextCtrlNameStr);

    // Maximises window.
    //Maximize();

    Bind(wxEVT_MENU, &Main_Frame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &Main_Frame::OnMedia, this, ID_Media);
    Bind(wxEVT_MENU, &Main_Frame::OnExercise, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &Main_Frame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &Main_Frame::OnBookmark, this, ID_Bookmark);
}

void Main_Frame::OnExercise(wxCommandEvent& event)
{
    wxMessageBox("This is the Exercise tab",
        "Exercise Tab", wxOK | wxICON_INFORMATION);
}

void Main_Frame::OnMedia(wxCommandEvent& event)
{
    //wxMessageBox(get_current_date());
    wxMessageDialog* dial = new wxMessageDialog(NULL,
        wxT("Message here"), wxT("Info"), wxOK);
    dial->ShowModal();
}

void Main_Frame::OnBookmark(wxCommandEvent& event)
{
    wxMessageBox("This is the Bookmark tab",
        "Bookmark Tab", wxOK | wxICON_INFORMATION);
}

void Main_Frame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Tab_1 Window Message");
}

void Main_Frame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
