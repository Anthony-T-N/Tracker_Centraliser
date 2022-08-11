#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include "Bookmark_Counter_h.h"

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

class Main_Application : public wxApp
{
public:
    virtual bool OnInit();
};

class Main_Frame : public wxFrame
{
    public:
        Main_Frame();
        void UpdateStatusBar(wxString message);
        wxTextCtrl* MainEditBox;

    private:
        void OnInsert(wxCommandEvent& event);
        void OnMedia(wxCommandEvent& event);
        void OnExit(wxCommandEvent& event);
        void OnExercise(wxCommandEvent& event);
        void OnBookmark(wxCommandEvent& event);
        void insert_to_csv(std::string category_label, std::string text_field_0, std::string text_field_1);
        
};

enum
{
    ID_Hello = 1,
    TEXT_Main = wxID_HIGHEST + 1,
    BUTTON_Insert = wxID_HIGHEST + 1
};

const int ID_Media = 2;
const int ID_Bookmark = 3;
const int ID_COMBOBOX1 = 4;

wxStaticText* category_label;
wxComboBox* category_combo_box;
wxTextCtrl* text_field_0;
wxTextCtrl* text_field_1;

bool Main_Application::OnInit()
{
    Main_Frame* fgs = new Main_Frame();
    // Create an instance of frame or window
    //Main_Frame* frame = new Main_Frame();
    // Show the window
    //fgs->HasFlag(wxSTAY_ON_TOP);
    //fgs->SetWindowStyleFlag(fgs->GetWindowStyleFlag() & ~wxSTAY_ON_TOP);
    fgs->SetWindowStyleFlag(fgs->GetWindowStyleFlag() | wxSTAY_ON_TOP);
    fgs->Show(true);

    return true;
}
wxIMPLEMENT_APP(Main_Application);

Main_Frame::Main_Frame()
    : wxFrame(NULL, wxID_ANY, "Tracker_Centraliser", wxPoint(50, 50), wxSize(250, 250))
{
    wxPanel* panel = new wxPanel(this, -1);

    panel->SetBackgroundColour(wxColour(34, 48, 60));

    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

    // wxFlexGridSizer(int rows, int cols, int vgap, int hgap)
    wxFlexGridSizer* fgs = new wxFlexGridSizer(4, 2, 9, 25);

    //SetMinSize(GetBestSize());

    wxArrayString category_item_arr;

    // Create common wxArrayString array
    category_item_arr.Add(wxT("_Critical_URLs"));
    category_item_arr.Add(wxT("_Sleep_Time"));
    category_item_arr.Add(wxT("_Bookmark"));
    category_item_arr.Add(wxT("_Exercise"));
    category_item_arr.Add(wxT("_Events"));
    category_item_arr.Add(wxT("_Series"));
    category_item_arr.Add(wxT("_Films"));
    category_item_arr.Add(wxT("_Books"));
    category_item_arr.Add(wxT("_Art"));
    category_item_arr.Add(wxT("_A"));
    category_item_arr.Add(wxT("_Record_Sort_Debug"));

    wxCheckBox* m_checkbox;
    m_checkbox = new wxCheckBox();
    m_checkbox->SetValue(false);

    wxStaticText* date_label = new wxStaticText(panel, -1, wxT("Current_Date:"));
    date_label->SetForegroundColour(wxColour(255,255,255));
    category_label = new wxStaticText(panel, -1, wxT("Category:"));
    category_label->SetForegroundColour(wxColour(255, 255, 255));
    wxStaticText* record_label = new wxStaticText(panel, -1, wxT("Record:"));
    record_label->SetForegroundColour(wxColour(255, 255, 255));
    wxStaticText* review_label = new wxStaticText(panel, -1, wxT("Review:"));
    review_label->SetForegroundColour(wxColour(255, 255, 255));

    text_field_0 = new wxTextCtrl(panel, -1, get_current_date());
    category_combo_box = new wxComboBox(panel, ID_COMBOBOX1, wxEmptyString, wxDefaultPosition, wxDefaultSize, category_item_arr, wxCB_READONLY, wxDefaultValidator, _T("ID_COMBOBOX1"));
    text_field_1 = new wxTextCtrl(panel, -1, wxT(""),
        wxPoint(-1, -1), wxSize(-1, -1), wxTE_MULTILINE);

    wxButton* insert_button = new wxButton(panel, BUTTON_Insert, _T("INSERT"),
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_VERTICAL);

    fgs->Add(date_label);
    fgs->Add(text_field_0, 1, wxEXPAND);
    fgs->Add(category_label);
    //fgs->Add(m_checkbox);
    fgs->Add(category_combo_box, 1, wxEXPAND);
    fgs->Add(record_label);
    fgs->Add(text_field_1, 1, wxEXPAND);
    fgs->Add(review_label, 1, wxEXPAND);
    fgs->Add(insert_button);
    //fgs->Add(m_checkbox);

    Bind(wxEVT_BUTTON, &Main_Frame::OnInsert, this, BUTTON_Insert);

    fgs->AddGrowableRow(2, 1);
    fgs->AddGrowableCol(1, 1);

    hbox->Add(fgs, 1, wxALL | wxEXPAND, 15);
    panel->SetSizer(hbox);
    Centre();

    CreateStatusBar();
    Main_Frame::UpdateStatusBar(": " + std::filesystem::current_path().generic_string());
}

void Main_Frame::UpdateStatusBar(wxString message)
{
    wxStatusBar* status_text_bar = Main_Frame::GetStatusBar();
    //bar->SetForegroundColour(wxColour(color));
    status_text_bar->SetStatusText(message, 0); //text in field 0
}

/*
void Main_Frame::new_checkbox(wxCommandEvent& event)
{
    status_text_bar->SetStatusText(message, 0); //text in field 0
}
*/

void Main_Frame::OnInsert(wxCommandEvent& event)
{
    if (category_combo_box->GetValue().ToStdString() == "")
    {
        wxLogWarning("[-] Missing Category");
    }
    else
    {
        //category_label->SetLabel("new value"); Example to change label.
        insert_to_csv(category_combo_box->GetValue().ToStdString(), text_field_0->GetValue().ToStdString(), text_field_1->GetValue().ToStdString());
        //wxLogMessage("Inserted: " + text_field_1->GetValue() + " into: " + category_combo_box->GetValue());
    }
}

void Main_Frame::insert_to_csv(std::string category_label, std::string text_field_0, std::string text_field_1)
{
    //TO-DO Date validator

    if (text_field_0.empty() || text_field_1.empty())
    {
        wxLogError("[-] Invalid input - Empty Field");
        return;
    }
    if (text_field_0.find('-') == std::string::npos)
    {
        "YYYY-mm-dd";
        wxLogError("[-] Invalid input - Invalid Date");
        return;
    }

    // Removing new lines from inputted text.
    text_field_1.erase(std::remove(text_field_1.begin(), text_field_1.end(), '\n'), text_field_1.end());
    if (category_label == "_Critical_URLs")
    {
        csv_maintainer_main("_Critical_URLs.csv", text_field_0, text_field_1);
    }
    else if (category_label == "_Sleep_Time")
    {
        csv_maintainer_main(category_label + ".csv", text_field_0, text_field_1);
    }
    else if (category_label == "_Bookmark")
    {
        // Note: Fails to validate very large numbers (Above 2147483647 to be exact).
        if (text_field_1.find_first_not_of("0123456789") != std::string::npos || text_field_1.empty())
        {
            wxLogError("[-] Invalid input - Please try again:");
            return;
        }
        bookmark_counter_main(std::stoi(text_field_1));
        Main_Frame::UpdateStatusBar(": " + std::filesystem::current_path().generic_string() + "/bookmark_record.csv");
    }
    else if (category_label == "_Exercise")
    {
        if (text_field_1.find_first_not_of("0123456789") != std::string::npos || text_field_1.empty())
        {
            wxLogError("[-] Invalid input - Please try again:");
            return;
        }
        csv_maintainer_main("_Exercise.csv", text_field_0, text_field_1);
    }
    else if (category_label == "_Record_Sort_Debug")
    {
        csv_maintainer_main(category_label + ".csv", text_field_0, text_field_1);
    }
    else
    {
        csv_maintainer_main(category_label + ".csv", text_field_0, text_field_1);
    }
    Main_Frame::UpdateStatusBar(": " + std::filesystem::current_path().generic_string() + "/" + category_label + ".csv");
    wxLogMessage("Inserted: " + text_field_1 + " into: " + category_combo_box->GetValue());
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
*/

/*
Project Plan and Design

Brief : Centralise tracking of a number of activities.

[Version 1]

=== Minimum Functions ===
[+] Window that associate entered text/string with a time/date.
[+] Entered text recorded in csv file.
[-] Optimized to be lightweight and run in the background.
[-] Fixed minimum window size ^ Maximum size
[+] Dark background + White Text
[-] Change colour when Unfocused
[+] Organise entries by date.
[+] Record Intergity Check 

*/
