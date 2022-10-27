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
        void insert_to_csv(std::string category_label, std::string text_date_field, std::string text_record_field);
        void update_category_item_arr();
};

enum
{
    ID_Hello = 1,
    TEXT_Main = wxID_HIGHEST + 2,
    BUTTON_Insert = wxID_HIGHEST + 3,
    BUTTON_Undo = wxID_HIGHEST + 4
};

const int ID_Media = 2;
const int ID_Bookmark = 3;
const int ID_COMBOBOX1 = 4;

wxStaticText* category_label;
wxComboBox* category_combo_box;
wxTextCtrl* text_date_field;
wxTextCtrl* text_record_field;

wxArrayString category_item_arr;

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
    wxFlexGridSizer* fgs = new wxFlexGridSizer(5, 2, 9, 25);

    //SetMinSize(GetBestSize());

    update_category_item_arr();

    wxStaticText* date_label = new wxStaticText(panel, -1, wxT("Current_Date:"));
    date_label->SetForegroundColour(wxColour(255,255,255));
    category_label = new wxStaticText(panel, -1, wxT("Category:"));
    category_label->SetForegroundColour(wxColour(255, 255, 255));
    wxStaticText* record_label = new wxStaticText(panel, -1, wxT("Record:"));
    record_label->SetForegroundColour(wxColour(255, 255, 255));
    wxStaticText* review_label = new wxStaticText(panel, -1, wxT("Review:"));
    review_label->SetForegroundColour(wxColour(255, 255, 255));

    text_date_field = new wxTextCtrl(panel, -1, get_current_date());
    category_combo_box = new wxComboBox(panel, ID_COMBOBOX1, wxEmptyString, wxDefaultPosition, wxDefaultSize, category_item_arr, wxCB_DROPDOWN, wxDefaultValidator, _T("ID_COMBOBOX1"));
    text_record_field = new wxTextCtrl(panel, -1, wxT(""),
        wxPoint(-1, -1), wxSize(-1, -1), wxTE_MULTILINE);

    wxButton* insert_button = new wxButton(panel, BUTTON_Insert, _T("INSERT"),
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_VERTICAL);

    fgs->Add(date_label);
    fgs->Add(text_date_field, 1, wxEXPAND);
    fgs->Add(category_label);
    fgs->Add(category_combo_box, 1, wxEXPAND);
    fgs->Add(record_label);
    fgs->Add(text_record_field, 1, wxEXPAND);
    fgs->Add(review_label, 1, wxEXPAND);
    fgs->Add(insert_button);

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

void debugging_function(std::string category_label, std::string delta, std::string text_record_field)
{
    wxLogDebug("<===================LABELS===================>");
    wxLogDebug(category_label.c_str());
    wxLogDebug(delta.c_str());
    wxLogDebug(text_record_field.c_str());
    wxLogDebug("<===================LABELS===================>\n");

    for (auto i : delta)
        wxLogDebug(i + "");


    std::string test1 = delta.c_str();
        //text_date_field->GetValue().ToStdString();
    //test1 += '\0';
    //test1 = test1.c_str();

    char src[40];
    char dest[100];

    memset(dest, '\0', sizeof(dest));
    strcpy(src, test1.c_str());
    strcpy(dest, src);

    wxLogDebug("Final copied string : %s\n", dest);
    wxLogDebug("Display below: " + '\n');
    wxLogDebug("CHAR PRINT " + dest[0] + '\n');

    OutputDebugStringA("\n============================================================END\n");
    OutputDebugStringA(test1.c_str());
    size_t sizer = 0;
    OutputDebugStringA("" + test1[sizer] + '\n');
    OutputDebugStringA(" ");
    OutputDebugStringA(" " + test1[1] + '\n');
    OutputDebugStringA(" " + test1[2] + '\n');
    OutputDebugStringA(" " + test1[3] + '\n');
    OutputDebugStringA("\n============================================================END\n");

    std::vector<char> tmp_string(test1.begin(), test1.end());
    tmp_string.push_back('\0');
    OutputDebugStringA("DEBUG: ");
    for (char i : tmp_string)
        OutputDebugStringA(i + "");
    wxLogDebug(std::to_string(tmp_string.size()).c_str());
    wxLogDebug(std::to_string(tmp_string[2]).c_str());
    for (int i = 0; i <= tmp_string.size() - 1; i++)
    {
        //wxLogWarning(std::to_string(tmp_string[i]).c_str());
        wxLogDebug("" + tmp_string[i]);
    }
    wxLogDebug(std::to_string(tmp_string[4]).c_str());
    OutputDebugStringA("\n============================================================END\n");
    std::string frienda(category_label);
    //std::string frienda = category_label;
    frienda = frienda + '\0';
    category_label = category_label + '\0';
    OutputDebugStringA("\n============================================================END\n");
    OutputDebugStringA("" + frienda[0]);
    OutputDebugStringA("" + frienda[1]);
    OutputDebugStringA("" + frienda[2]);
    OutputDebugStringA("" + frienda[3]);
    /*
    std::vector<char> illegal = { '<', '>', ':', '"', '/', '\\', '|', '?', '*' };
    for (int i = 0; i <= frienda.size(); i++)
    {
        wxLogError("" + frienda[i]);
    }
    */
    /*
    if (text_record_field.find('<') != std::string::npos || text_record_field.find('>') != std::string::npos || text_record_field.find('>') != std::string::npos)
    {

    }
    */
    // Issue: Unable to convert to print chars/strings without strange characters.
    /*
    std::string test(text_date_field);
    //sprintf(msgbuf, "My variable is %d\n", test);
    std::wstring stemp = std::wstring(test.begin(), test.end());
    LPCWSTR sw = stemp.c_str();
    OutputDebugStringA("DEBUG: ");
    OutputDebugString(sw);
    //OutputDebugStringA(sw[4]);

    char* fchar = new char[test.size() + 1];
    std::strncpy(fchar, test.c_str(), test.size() + 1);
    fchar[10] = '\0';
    OutputDebugStringA("DEBUG: ");
    //OutputDebugString(fchar + "");

    test = test.c_str();
    std::vector<char> tmp_string(test.begin(), test.end());
    tmp_string.push_back('\0');
    OutputDebugStringA("DEBUG: ");
    for (char i : tmp_string)
        OutputDebugStringA(i + "");
    wxLogWarning(std::to_string(tmp_string.size()).c_str());
    wxLogWarning(std::to_string(tmp_string[2]).c_str());
    for (int i = 0; i <= tmp_string.size() - 1; i++)
    {
        wxLogWarning(std::to_string(tmp_string[i]).c_str());
        //wxLogError("" + tmp_string[i]);
    }
    wxLogError(std::to_string(tmp_string[4]).c_str());
    wxLogError(std::to_string(tmp_string[7]).c_str());
    */
}

void Main_Frame::OnInsert(wxCommandEvent& event)
{
    //debugging_function(category_combo_box->GetValue().ToStdString(), text_date_field->GetValue().ToStdString(), text_record_field->GetValue().ToStdString());
    //category_label->SetLabel("new value"); Example to change label.
    insert_to_csv(category_combo_box->GetValue().ToStdString(), text_date_field->GetValue().ToStdString(), text_record_field->GetValue().ToStdString());
}

void Main_Frame::insert_to_csv(std::string category_label, std::string text_date_field, std::string text_record_field)
{
    /* TO_DO:
    - Date validation - User can insert any number (And characters) into "Current_Date" field.
    - Text file creation name validation.
    */

    if (category_label.empty() || text_date_field.empty() || text_record_field.empty())
    {
        wxLogError("[-] Invalid input - Empty Field");
        return;
    }

    if (text_date_field.find_first_of("0123456789") == std::string::npos || text_date_field.find_first_of("-") == std::string::npos)
    {
        wxLogError("[-] Invalid date format");
        return;
    }
    std::string target_char = "-";
    int count = 0;
    std::string::size_type i = text_date_field.find(target_char);
    while (i != std::string::npos)
    {
        count++;
        i = text_date_field.find(target_char, i + target_char.length());
    }
    if (count != 2)
    {
        wxLogError((std::to_string(count)).c_str());
        wxLogError(("[-] Invalid date format - Hyphen Mismatch: " + std::to_string(count)).c_str());
        return;
    }
    count = 0;
    for (auto i : text_date_field)
    {
        if (isdigit(i))
        {
            count++;
        }
    }
    if (count != 8)
    {
        wxLogError((std::to_string(count)).c_str());
        wxLogError(("[-] Invalid date format - Date Mismatch: " + std::to_string(count)).c_str());
        return;
    }

    std::string special_characters = "\\/:*?\"<>|";
    for (int i = 0; i <= special_characters.length(); i++)
    {
        if (category_label.find(special_characters[i]) != std::string::npos)
        {
            wxLogError("[-] A special character detected");
            return;
        }
    }

    /*
    //Date validator
    if (text_date_field[4] == '-' && text_date_field[7] == '-')
    {
        for (int i = 0; i <= text_date_field.size(); i++)
        {
            if (i != 4 || i != 7)
            {
                if (!isdigit(text_date_field[i]))
                {
                    wxLogError("[-] Invalid input - Non-digital detected");
                    wxLogError("" + text_date_field[i]);
                    break;
                }
            }
        }
        "YYYY-mm-dd";
        wxLogError("[-] Invalid input - Invalid Date");
        return;
    }
    
    else
    {
        wxLogError("[-] Invalid input - Invalid Date");
        return;
    }
    */

    // Removing new lines from inputted text.
    text_record_field.erase(std::remove(text_record_field.begin(), text_record_field.end(), '\n'), text_record_field.end());
    if (category_label == "_Critical_URLs")
    {
        csv_maintainer_main("_Critical_URLs.csv", text_date_field, text_record_field);
    }
    // Inserting records into _Bookmark_record does not pass user_defined date.
    else if (category_label == "_Bookmark_record")
    {
        // Note: Fails to validate very large numbers (Above 2147483647 to be exact).
        if (text_record_field.find_first_not_of("0123456789") != std::string::npos || text_record_field.empty())
        {
            wxLogError("[-] Invalid input - Please try again");
            return;
        }
        if (text_record_field.length() >= 10)
        {
            wxLogError("[-] Large number");
            return;
        }
        bookmark_counter_main(std::stoi(text_record_field));
    }
    else if (category_label == "_Exercise")
    {
        if (text_record_field.find_first_not_of("0123456789") != std::string::npos || text_record_field.empty())
        {
            wxLogError("[-] Invalid input - Please try again");
            return;
        }
        csv_maintainer_main("_Exercise.csv", text_date_field, text_record_field);
    }
    else if (category_label == "_Record_Sort_Debug")
    {
        csv_maintainer_main(category_label + ".csv", text_date_field, text_record_field);
    }
    else
    {
        csv_maintainer_main(category_label + ".csv", text_date_field, text_record_field);
    }
    //Main_Frame::UpdateStatusBar(std::filesystem::current_path().generic_string() + "/" + "_Tracking_Centraliser_Root_Folder" + "/" + category_label + ".csv"); #Latest update to file.
    Main_Frame::UpdateStatusBar("[+] " + category_label + ".csv" + " | " + text_date_field  + " | " + text_record_field);
    wxLogMessage("[+] Inserted: [" + text_date_field + "] " + text_record_field + " to: " + category_combo_box->GetValue() + ".csv");
    
    /*
    // "Dynamically" add items to dropdown list.
    // // OLD Method of adding filenames to application from text file.
    category_item_arr.Clear();
    std::ifstream input_file;
    std::string input_file_line;
    input_file.open("_Tracking_Centraliser_Category_List.txt");
    while (std::getline(input_file, input_file_line))
    {
        category_item_arr.Add(input_file_line);
    }
    input_file.close();
    category_combo_box->Set(category_item_arr);
    category_combo_box->SetValue(category_label);
    */
    
    category_item_arr.Clear();
    update_category_item_arr();
    category_combo_box->Set(category_item_arr);
    category_combo_box->SetValue(category_label);
}

void Main_Frame::update_category_item_arr()
{
    /*
    // OLD Method of adding filenames to application from text file.
    std::ifstream input_file;
    std::string input_file_line;
    input_file.open("_Tracking_Centraliser_Category_List.txt");
    while (std::getline(input_file, input_file_line))
    {
        category_item_arr.Add(input_file_line);
    }
    input_file.close();
    */

    std::string path = std::filesystem::current_path().generic_string() + "/_Tracking_Centraliser_Root_Folder/";
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.path().generic_string().find(".csv") != std::string::npos)
        {
            std::string sub_filename = (entry.path().generic_string().substr(entry.path().generic_string().find_last_of("//") + 1)).c_str();
            sub_filename = sub_filename.substr(0, sub_filename.find_last_of(".csv") - 3);
            OutputDebugStringA(sub_filename.c_str());
            category_item_arr.Add(sub_filename);
        }
    }
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

Brief : Centralise the tracking of a number of activities and insert into various text files.

[Version 1]

=== Minimum Functions ===
[+] Window that associate entered text/string with a time/date.
[+] Entered text recorded in csv file.
[+] Dark background + White Text
[+] Organise entries by date.
[+] Record Intergity Check 

[Version 1]
[-] Change colour when Unfocused
[-] Optimized to be lightweight and run in the background.
[-] Fixed minimum window size ^ Maximum size

*/
