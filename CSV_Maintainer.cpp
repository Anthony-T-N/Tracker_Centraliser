#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <map>
#include <wx/log.h>
#include "Bookmark_Counter_h.h"

void root_folder_creation(std::string root_folder_name, std::string csv_file_name)
{
    // Detect duplicate root folders ?
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
    std::ofstream output_file;
    if (std::filesystem::exists(root_folder_name + "/" + csv_file_name) == false)
    {
        wxLogMessage(("[-] " + root_folder_name + "/" + csv_file_name + " does not exist").c_str());
        output_file.open(root_folder_name + "/" + csv_file_name);
        wxLogMessage(("[+] Created " + root_folder_name + "/" + csv_file_name + " successfully;").c_str());
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
        wxLogMessage((current_date + " | " + message).c_str());
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
        wxLogMessage(("[+] Adding new entry: \n" + current_date + " | " + message).c_str());
        output_file << current_date << "," << message << "\n";
        temp_report_c.push_back(current_date + "," + message);
        output_file.close();
        return 1;
    }
}

void sort_record_dates(std::string csv_file_name)
{
    std::string root_folder_name("_Tracking_Centraliser_Root_Folder");

    std::ifstream input_file;
    input_file.open(root_folder_name + "/" + csv_file_name);

    std::ofstream output_temp_file;
    if (std::filesystem::exists(root_folder_name + "/" + "temp_record.csv") != false)
    {
        wxLogError("[!] Error: Pre-existing temp_record.csv file. Unable to sort csv file;");
        input_file.close();
        output_temp_file.close();
        return;
    }
    else
    {
        output_temp_file.open(root_folder_name + "/" + "temp_record.csv");
    }

    std::ofstream output_backup_file;
    output_backup_file.open(root_folder_name + "/" + "_Backup_Record_Folder" + "/" + csv_file_name + "_BAK");

    std::string input_file_line;
    int line_count = 0;
    while (std::getline(input_file, input_file_line))
    {
        line_count++;
    }

    // https://forums.codeguru.com/showthread.php?320724-how-to-reset-quot-getline(File-string)-quot
    input_file.clear();
    input_file.seekg(0, std::ios::beg);

    std::vector<std::string> input_file_line_vec;

    int last_line = line_count - 1;
    line_count = 0;
    while (std::getline(input_file, input_file_line))
    {
        line_count++;
        input_file_line_vec.push_back(input_file_line);
        output_backup_file << input_file_line << "\n";
    }

    for (int i = 0; i <= input_file_line_vec.size() - 1; i++)
    {
        for (int j = i; j < (input_file_line_vec.size()); j++)
        {
            std::string primary_line = input_file_line_vec[i].substr(0, input_file_line_vec[i].find(","));
            primary_line.erase(std::remove(primary_line.begin(), primary_line.end(), '-'), primary_line.end());
            std::string secondary_line = input_file_line_vec[j].substr(0, input_file_line_vec[j].find(","));
            secondary_line.erase(std::remove(secondary_line.begin(), secondary_line.end(), '-'), secondary_line.end());
            wxLogMessage((primary_line + " > " + secondary_line).c_str());
            if (primary_line > secondary_line)
            {
                std::swap(input_file_line_vec[i], input_file_line_vec[j]);
            }
        }
    }
    int sorted_line_count = 0;
    for (int i = 0; i < (input_file_line_vec.size()); i++)
    {
        wxLogMessage(input_file_line_vec[i].c_str());
        output_temp_file << input_file_line_vec[i] << "\n";
        sorted_line_count++;
    }
    if (line_count != sorted_line_count)
    {
        wxLogError("[-] Line count mismatch between original file and sorted file");
    }
    else
    {
        wxLogMessage(("[!] Line count " + std::to_string(sorted_line_count)).c_str());
    }
    input_file.close();
    output_temp_file.close();
    output_backup_file.close();
    std::string file_to_remove = root_folder_name + "/" + csv_file_name;
    // https://stackoverflow.com/questions/6674611/deleting-a-file-with-string-in-the-arguments
    if (remove(file_to_remove.c_str()) == 0)
    {
        wxLogMessage("[+] Filename deleted successfully");
        wxLogMessage((csv_file_name + " deleted").c_str());
    }
    else
    {
        wxLogError("[-] Error with deletion");
    }
    int value = std::rename((root_folder_name + "/" + "temp_record.csv").c_str(), (root_folder_name + "/" + csv_file_name).c_str());
    if (!value)
    {
        wxLogMessage("[+] Filename renamed successfully");
        wxLogMessage(("temp_record.csv > " + csv_file_name).c_str());
    }
    else
    {
        wxLogError("[-] Error with filename change");
        wxLogError(("temp_record.csv > " + csv_file_name).c_str());
    }
}

int csv_maintainer_main(std::string csv_file_name, std::string date, std::string message)
{
    std::string root_folder_name("_Tracking_Centraliser_Root_Folder");
    root_folder_creation(root_folder_name, csv_file_name);
    write_to_csv(root_folder_name, csv_file_name, date, message);
    sort_record_dates(csv_file_name);

    std::ifstream input_file;
    input_file.open("_Tracking_Centraliser_Category_List.txt");
    wxLogMessage(csv_file_name.substr(0, csv_file_name.find(".csv")).c_str());
    bool csv_found = false;
    std::string input_file_line;
    while (std::getline(input_file, input_file_line))
    {
        if (csv_file_name.substr(0, csv_file_name.find(".csv")) == input_file_line)
        {
            csv_found = true;
            break;
        }
    }
    input_file.close();
    if (csv_found == false)
    {
        std::ofstream output_file;
        output_file.open("_Tracking_Centraliser_Category_List.txt", std::ios::app);
        output_file << csv_file_name.substr(0, csv_file_name.find(".csv")) << "\n";
        output_file.close();
    }
    return 0;
}

// Create file -> Get date -> Add event/item -> Close

/*
   Date | Series Name
   Date | Events
   Date | URLS

   Directory Tree:
   .exe
   -> _Tracking_Centraliser_Root_Folder
            -> file1.csv
            -> file2.csv
            -> file3.csv
                    -> _Backup_Record_Folder
*/
