#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <map>
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

int write_to_csv(std::string root_folder_name, std::string csv_file_name, std::string current_date, std::string message)
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

void sort_date()
{
    std::ifstream input_file;
    input_file.open("bookmark_record.csv");

    std::ofstream output_file;
    output_file.open("temp_record.csv");

    std::string input_file_line;
    int line_counter = 0;
    while (std::getline(input_file, input_file_line))
    {
        line_counter++;
    }

    // https://forums.codeguru.com/showthread.php?320724-how-to-reset-quot-getline(File-string)-quot
    input_file.clear();
    input_file.seekg(0, std::ios::beg);

    std::map<std::string, std::string> my_map =
    {
        { "A8", "A8" },
        { "A8", "A8" },
        { "A8", "A8" }
    };

    std::map<std::string, std::string> date_record_map;
    int pos = 0;
    std::string date_record_map_key = "";
    std::string date_record_map_value = "";

    int last_line = line_counter - 1;
    line_counter = 0;
    while (std::getline(input_file, input_file_line))
    {
        line_counter++;
        date_record_map_key = input_file_line.substr(0, input_file_line.find(","));
        date_record_map_value = input_file_line.substr(input_file_line.find(",") + 1);
        wxLogMessage(date_record_map_key.c_str());
        wxLogMessage(date_record_map_value.c_str());
        date_record_map.insert({ date_record_map_key, date_record_map_value});
        output_file << input_file_line << "\n";
        if (last_line == line_counter)
        {
            wxLogMessage("[!] Skip last line;");
            break;
        }
    }
    input_file.close();
    output_file.close();
    if (remove("bookmark_record.csv") == 0)
    {
        wxLogMessage("[+] Filename deleted successfully");
        wxLogMessage("\"bookmark_record.csv\" deleted");
    }
    else
    {
        wxLogMessage("[-] Error with deletion");
    }
    int value = std::rename("temp_record.csv", "bookmark_record.csv");
    if (!value)
    {
        wxLogMessage("[+] Filename renamed successfully");
        wxLogMessage("temp_record.csv > bookmark_record.csv");
    }
    else
    {
        wxLogMessage("[-] Error with filename change");
        wxLogMessage("temp_record.csv !> bookmark_record.csv");
    }
}

int csv_maintainer_main(std::string csv_file_name, std::string date, std::string message)
{
    std::string root_folder_name("_Tracking_Centraliser_Root_Folder");
    root_folder_creation(root_folder_name, csv_file_name);
    write_to_csv(root_folder_name, csv_file_name, date, message);
    return 0;
}
