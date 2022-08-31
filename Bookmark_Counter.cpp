#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <wx/log.h>
#include "Bookmark_Counter_h.h"
#include <wx/msgdlg.h>

#pragma warning(disable:4996);

std::vector<std::string> temp_report = {};

int calculate_difference(std::string full_bookmark_record_path, int current_bookmark_total_input)
{
    // Function uses: <iostream>, <fstream>, <string>, <filesystem>

    std::ifstream input_file;
    input_file.open(full_bookmark_record_path);
    std::string input_file_line;
    std::string last_input_line;
    while (std::getline(input_file, input_file_line))
    {
        last_input_line = input_file_line;
        temp_report.push_back(input_file_line);
    }
    input_file.close();

    for (double i = 0; i <= temp_report.size() - 1; i++)
    {
        if (temp_report[i] == "" || temp_report[i] == " " || temp_report[i] == ",,")
        {
            wxLogMessage("[-] ATTENTION: Empty row located;");
            wxLogMessage("[!] Recommendation: Delete existing csv file or fix empty row;");
            return 0;
        }
    }
    last_input_line.erase(0, last_input_line.find_first_of(",") + 1);
    last_input_line.erase(last_input_line.find_last_of(","), last_input_line.length());
    return current_bookmark_total_input - stoi(last_input_line);
}

void remove_csv_last_line(std::string full_bookmark_record_path, std::string full_temp_record_path)
{
    // Function uses: <iostream>, <fstream>

    wxLogMessage("[!] Removing last line;");

    std::ifstream input_file;
    input_file.open(full_bookmark_record_path);

    std::ofstream output_file;
    output_file.open(full_temp_record_path);

    std::string input_file_line;
    int line_counter = 0;
    while (std::getline(input_file, input_file_line))
    {
        line_counter++;
    }

    // https://forums.codeguru.com/showthread.php?320724-how-to-reset-quot-getline(File-string)-quot
    input_file.clear();
    input_file.seekg(0, std::ios::beg);

    int last_line = line_counter - 1;
    line_counter = 0;
    while (std::getline(input_file, input_file_line))
    {
        line_counter++;
        output_file << input_file_line << "\n";
        if (last_line == line_counter)
        {
            wxLogMessage("[!] Skipping last line");
            break;
        }
    }
    input_file.close();
    output_file.close();
    if (remove(full_bookmark_record_path.c_str()) == 0)
    {
        wxLogMessage("[+] Filename deleted successfully");
        wxLogMessage("\"_Bookmark_record.csv\" deleted");
    }
    else
    {
        wxLogMessage("[-] Error with deletion");
    }
    int value = std::rename(full_temp_record_path.c_str(), full_bookmark_record_path.c_str());
    if (!value)
    {
        wxLogMessage("[+] Filename renamed successfully");
        wxLogMessage("temp_record.csv > _Bookmark_record.csv");
    }
    else
    {
        wxLogMessage("[-] Error with filename change");
        wxLogMessage("temp_record.csv !> _Bookmark_record.csv");
    }
}

int write_to_csv(std::string full_bookmark_record_path, std::string current_date, int current_bookmark_total_input)
{
    // Function uses: <iostream>, <fstream>, <filesystem>

    OutputDebugStringA("====================================================================================================================================");
    OutputDebugStringA(full_bookmark_record_path.c_str());

    int difference = 0;
    // output file stream allows you to write contents to a file.
    std::ofstream output_file;
    if (std::filesystem::exists(full_bookmark_record_path) == false)
    {
        wxLogMessage("[!] Creating new _Bookmark_record.csv;");
        output_file.open(full_bookmark_record_path, std::ios::app);
        wxLogMessage("[+] Opened _Bookmark_record.csv successfully;");
        // Adding in column headings.
        output_file << "Date" << "," << "Current Total" << "," << "Difference" << "\n";
        wxLogMessage("[+] Adding new entry: ");
        const wxString& msg = current_date + " | " + std::to_string(current_bookmark_total_input) + " | " + std::to_string(difference);
        wxLogMessage(msg);
        output_file << current_date << "," << current_bookmark_total_input << "," << difference << "\n";
        temp_report.push_back(current_date + "," + std::to_string(current_bookmark_total_input) + "," + std::to_string(difference));
        output_file.close();
        return 0;
    }
    else
    {
        OutputDebugStringA("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        OutputDebugStringA(full_bookmark_record_path.c_str());

        // std::ios::app informs program to append and not to overwrite.
        output_file.open(full_bookmark_record_path, std::ios::app);
        //output_file.open("_Bookmark_record.csv", std::ios::app);
        wxLogMessage("[+] Opened _Bookmark_record.csv successfully;");
        difference = calculate_difference(full_bookmark_record_path, current_bookmark_total_input);
        const wxString& msg = "[+] Adding new entry: \n" + current_date + " | " + std::to_string(current_bookmark_total_input) + " | " + std::to_string(difference);
        wxLogMessage(msg);
        output_file << current_date << "," << current_bookmark_total_input << "," << difference << "\n";
        temp_report.push_back(current_date + "," + std::to_string(current_bookmark_total_input) + "," + std::to_string(difference));
        output_file.close();
        return 1;
    }
}

int bookmark_counter_main(int current_bookmark_total_input)
{
    std::string temp_report_log;

    std::string root_folder_name("_Tracking_Centraliser_Root_Folder");
    if (std::filesystem::exists(root_folder_name) == false)
    {
        wxLogMessage("[-] _Tracking_Centraliser_Root_Folder does not exist;");
        std::filesystem::create_directories(root_folder_name);
    }
    if (std::filesystem::exists(root_folder_name + "/" + "_Backup_Record_Folder") == false)
    {
        wxLogMessage("[-] _Tracking_Centraliser_Root_Folder Backup Folder does not exist;");
        std::filesystem::create_directories(root_folder_name + "/" + "_Backup_Record_Folder");
    }
    std::string full_bookmark_record_path = root_folder_name + "/_Bookmark_record.csv";
    std::string full_temp_record_path = root_folder_name + "/temp_record.csv";

    // TO-DO: Fix temp_record path.

    while (true)
    {
        int first_csv = write_to_csv(full_bookmark_record_path, get_current_date(), current_bookmark_total_input);
        wxLogMessage("[========== Report ==========]");
        if (temp_report.size() > 20)
        {
            for (int i = 20; i > 0; i--)
            {
                temp_report_log += temp_report[temp_report.size() - i] + "\n";
            }
            wxLogMessage(temp_report_log.c_str());
        }
        else if (temp_report.size() < 20)
        {
            for (int i = temp_report.size(); i > 0; i--)
            {
                temp_report_log += temp_report[temp_report.size() - i] + "\n";
            }
            wxLogMessage(temp_report_log.c_str());
        }
        if (first_csv == 1)
        {            
            int answer = wxMessageBox("> Undo ? (y):", "Confirm", wxYES_NO | wxICON_INFORMATION);
            if (answer == wxYES)
            {
                remove_csv_last_line(full_bookmark_record_path, full_temp_record_path);
                break;
            }
            else if (answer == wxNO)
            {
                break;
            }
        }
        else
        {
            break;
        }
        temp_report.clear();
        temp_report_log.clear();
    }
    return 0;
}
