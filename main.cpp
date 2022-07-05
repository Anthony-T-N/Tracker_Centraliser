#include <iostream>
#include <filesystem>
#include <string>

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

private:
    void OnHello(wxCommandEvent& event);
    void OnTest(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};

enum
{
    ID_Hello = 1,
    TEXT_Main = wxID_HIGHEST + 1
};

const int ID_Test = 2;
const int ID_Dynamic = 3;

wxIMPLEMENT_APP(Main_Application);

bool Main_Application::OnInit()
{
    Main_Frame* frame = new Main_Frame();
    frame->Show(true);
    return true;
}

Main_Frame::Main_Frame()
    : wxFrame(NULL, wxID_ANY, "Window_Title_Bar")
{
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Main_Item...\tCtrl-H",
        "String shown in bottom status bar");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenu* menuFile_T = new wxMenu;
    menuFile_T->Append(ID_Test, "&Test_Item...\tCtrl-H",
        "Test Status Bar Item");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&Tab_1");
    menuBar->Append(menuHelp, "&Tab_2");
    menuBar->Append(menuFile_T, "&Test_Tab");

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Bottom_Status_Bar");

    MainEditBox = new wxTextCtrl(this, TEXT_Main,
        wxT("Enter text here..."), wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxTE_RICH, wxDefaultValidator, wxTextCtrlNameStr);
    Maximize();

    Bind(wxEVT_MENU, &Main_Frame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &Main_Frame::OnTest, this, ID_Test);
    Bind(wxEVT_MENU, &Main_Frame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &Main_Frame::OnExit, this, wxID_EXIT);
}

void Main_Frame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("About Message Here",
        "About Window Title Bar", wxOK | wxICON_INFORMATION);
}

void Main_Frame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Tab_1 Window Message");
}

void Main_Frame::OnTest(wxCommandEvent& event)
{
    // get_current_date()
    std::string test = "12345";
    wxLogMessage(test);
}

void Main_Frame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
