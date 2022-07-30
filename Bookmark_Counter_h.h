#ifndef Bookmark_Counter_h_H
#define Bookmark_Counter_h_H

#include <string>

//Bookmark_Counter.cpp
int bookmark_counter_main(int test);
std::string get_current_date();

//CSV_Maintainer.cpp
int csv_maintainer_main(std::string csv_file_name, std::string date, std::string message);
void root_folder_creation(std::string root_folder_name, std::string csv_file_name);
void sort_record_dates(std::string csv_file_name);

#endif
