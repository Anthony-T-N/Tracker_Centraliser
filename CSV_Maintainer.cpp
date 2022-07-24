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
void root_folder_creation(std::string csv_file_name)
{
    std::string root_folder_name("_Tracking_Centraliser_Root_Folder");
    if (std::filesystem::exists(root_folder_name) == false)
    {
        wxLogMessage("[-] _Tracking_Centraliser_Root_Folder does not exist;");
        std::filesystem::create_directories(root_folder_name);
    }
    std::ofstream output_file;
    if (std::filesystem::exists(root_folder_name + "/" + csv_file_name) == false)
    {
        const wxString& temp_log_message = "[-]" + root_folder_name + "/" + csv_file_name;
        wxLogMessage(temp_log_message);
        output_file.open(root_folder_name + "/" + csv_file_name);
        wxLogMessage("[+] Created " + temp_log_message + "successfully;");
    }
}
