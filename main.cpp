#include <iostream>
#include <filesystem>
#include <string>

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

/*
Project Plan and Design

Brief : Centralise tracking of a number of activities.

[Version 1]

=== Minimum Functions ===
[-] Window that associate entered text/string with a time/date.
[-] Entered text recorded in csv file.
[-] Seperate buttons open seperate windows and accept different types of records.
[-] Optimized to be lightweight and run in the background.

*/
