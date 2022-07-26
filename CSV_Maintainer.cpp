#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <wx/log.h>
#include "Bookmark_Counter_h.h"

// Create file -> Get date -> Add event/item -> Close

/*
   Date | Series Name
   Date | Events
   Date | URLS

   Directory Tree:
   .exe
   -> Root_folder
			-> file1.csv
			-> file2.csv
			-> file3.csv
*/

void root_folder_creation(std::string root_folder_name, std::string csv_file_name)
{
    if (std::filesystem::exists(root_folder_name) == false)
    {
        wxLogMessage("[-] _Tracking_Centraliser_Root_Folder does not exist;");
        std::filesystem::create_directories(root_folder_name);
    }
    std::ofstream output_file;
    if (std::filesystem::exists(root_folder_name + "/" + csv_file_name) == false)
    {
        wxString temp_log_message = "[-] " + root_folder_name + "/" + csv_file_name + " does not exist";
        wxLogMessage(temp_log_message);
        output_file.open(root_folder_name + "/" + csv_file_name);
        temp_log_message = root_folder_name + "/" + csv_file_name;
        wxLogMessage("[+] Created " + temp_log_message + " successfully;");
        output_file.close();
    }
}

int write_to_csv(std::string current_date, std::string root_folder_name, std::string csv_file_name, std::string message)
{
    std::vector<std::string> temp_report_c = {};
    // output file stream allows you to write contents to a file.
    std::ofstream output_file;
    if (std::filesystem::exists(root_folder_name + "/" + csv_file_name) == false)
    {
        wxLogMessage("[!] Creating new bookmark_record.csv;");
        output_file.open(root_folder_name + "/" + csv_file_name, std::ios::app);
        wxLogMessage("[+] Opened bookmark_record.csv successfully;");
        output_file << "Date" << "," << "Current Total" << "\n";
        wxLogMessage("[+] Adding new entry: ");
        const wxString& msg = current_date + " | " + message;
        wxLogMessage(msg);
        output_file << current_date << "," << message << "\n";
        temp_report_c.push_back(current_date + "," + message);
        output_file.close();
        return 0;
    }
    else
    {
        // std::ios::app informs program to append and not to overwrite.
        output_file.open(root_folder_name + "/" + csv_file_name, std::ios::app);
        wxLogMessage("[+] Opened bookmark_record.csv successfully;");
        const wxString& msg = "[+] Adding new entry: \n" + current_date + " | " + message;
        wxLogMessage(msg);
        output_file << current_date << "," << message << "\n";
        temp_report_c.push_back(current_date + "," + message);
        output_file.close();
        return 1;
    }
}

int csv_maintainer_main(std::string csv_file_name, std::string message)
{
    std::string root_folder_name("_Tracking_Centraliser_Root_Folder");
    root_folder_creation(root_folder_name, csv_file_name);
    write_to_csv(get_current_date(), root_folder_name, csv_file_name, message);
    return 0;
}
