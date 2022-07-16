#ifndef Bookmark_Counter_h_H
#define Bookmark_Counter_h_H

#include <string>

int calculate_difference(int current_bookmark_total_input);
void remove_csv_last_line();
int write_to_csv(std::string current_date, int current_bookmark_total_input);
std::string user_input_validation();
int bookmark_counter_main();
std::string get_current_date();

#endif
