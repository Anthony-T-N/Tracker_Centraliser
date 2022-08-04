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

void sort_record_dates(std::string csv_file_name)
{
    std::string root_folder_name("_Tracking_Centraliser_Root_Folder");
    root_folder_creation(root_folder_name, csv_file_name);

    std::ifstream input_file;
    input_file.open(root_folder_name + "/" + csv_file_name);

    std::ofstream output_file;
    output_file.open(root_folder_name + "/" + "temp_record.csv");

    std::ofstream sorted_file;
    sorted_file.open(root_folder_name + "/" + "temp_sorted_file_record.csv");

    std::string input_file_line;
    int line_counter = 0;
    while (std::getline(input_file, input_file_line))
    {
        line_counter++;
    }

    // https://forums.codeguru.com/showthread.php?320724-how-to-reset-quot-getline(File-string)-quot
    input_file.clear();
    input_file.seekg(0, std::ios::beg);

    std::vector<std::string> input_file_line_vec;

    int last_line = line_counter - 1;
    line_counter = 0;
    while (std::getline(input_file, input_file_line))
    {
        line_counter++;
        input_file_line_vec.push_back(input_file_line);
        output_file << input_file_line << "\n";
        /*
        if (last_line == line_counter)
        {
            wxLogMessage("[!] Skip last line;");
            break;
        }
        */
    }

    for (int i = 0; i <= input_file_line_vec.size() - 1; i++)
    {
        for (int j = i; j < (input_file_line_vec.size()); j++)
        {
            std::string primary_line = input_file_line_vec[i].substr(0, input_file_line_vec[i].find(","));
            primary_line.erase(std::remove(primary_line.begin(), primary_line.end(), '-'), primary_line.end());
            std::string secondary_line = input_file_line_vec[j].substr(0, input_file_line_vec[j].find(","));
            secondary_line.erase(std::remove(secondary_line.begin(), secondary_line.end(), '-'), secondary_line.end());
            std::string full_test = primary_line + " > " + secondary_line;
            wxLogMessage(full_test.c_str());
            if (primary_line > secondary_line)
            {
                std::swap(input_file_line_vec[i], input_file_line_vec[j]);
            }
        }
    }
    wxLogMessage("BREAK");
    for (int i = 0; i < (input_file_line_vec.size()); i++)
    {
        wxLogMessage(input_file_line_vec[i].c_str());
        sorted_file << input_file_line_vec[i] << "\n";
    }
    input_file.close();
    output_file.close();
    std::string file_to_remove = root_folder_name + "/" + csv_file_name;
    // https://stackoverflow.com/questions/6674611/deleting-a-file-with-string-in-the-arguments
    if (remove(file_to_remove.c_str()) == 0)
    {
        wxLogMessage("[+] Filename deleted successfully");
        wxLogMessage("\"bookmark_record.csv\" deleted");
    }
    else
    {
        wxLogMessage("[-] Error with deletion");
    }
    int value = std::rename((root_folder_name + "/" + "temp_record.csv").c_str(), (root_folder_name + "/" + csv_file_name).c_str());
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
