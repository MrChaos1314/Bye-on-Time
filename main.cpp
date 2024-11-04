#include <iostream>
#include <stdlib.h>
#include <ctime>

#define CLEAR_SCREEN std::cout << "\x1B[2J\x1B[H"

struct tm start_time_prompt(void);
bool is_format_correct(std::string);
bool correct_start_time(struct tm &, std::string);

int working_hours_prompt(void);

int break_minutes_prompt(void);

void calc_and_print_time(struct tm, int, int);

int main()
{
    struct tm start_time = start_time_prompt();

    int working_hours = working_hours_prompt();

    //Error of start_prompt
    if(working_hours == -1)
    {
        std::cout << "something went wrong!\n" << std::endl;
    }

    int break_minutes = break_minutes_prompt();


    int formatted_working_hours = working_hours * 60;

    if(formatted_working_hours <= break_minutes)
    {
        std::cout << "go home... " << std::endl;
        return 0;
    }

    calc_and_print_time(start_time, working_hours, break_minutes);

    return 0;
}


struct tm start_time_prompt()
{
    std::string input = "";
    //get cur_time and formate to date struct
    time_t cur_timestamp = time(NULL);
    struct tm result_time = *localtime(&cur_timestamp);

    //change time to start_time of user_input
    while(true)
    {
        CLEAR_SCREEN;
        std::cout << "When do u want to start (Format: HH:MM:SS): ";
        std::cin >> input;
        if(is_format_correct(input))
        {
            std::cout << "Test" << std::endl;
            //corrects the start time and checks for wrong time like 65 seconds
            if(correct_start_time(result_time, input))
            {
                std::cout << "format" << std::endl;
                return result_time;
            }else{
                std::cout << "Not a correct start time" << std::endl;
            }
        }else{
            std::cout << "Format error" << std::endl;
        }
    }

    return result_time;
}

// it just checks the format HH:MM:SS but not if numbers are wrong like 65 seconds
bool is_format_correct(std::string input)
{
    //counter for HH MM SS - for a pair of two
    //semicolon sets it back to 0
    unsigned int counter = 0;

    //checking for time pair (HH - MM - SS) 
    for(int index = 0; index < 8; index++) //8 for 6 pairs and 2 semicolons
    {
        //checks if the pair is a digit
        if(isdigit(input[index]))
        {
            counter++;

        }else if(counter == 2 && input[index] == ':'){
            //if it was a pair of two digits and now a semicolon
            //start check again for pair, semicolon
            counter = 0;

        }else{
            //not correct format
            return false;
        }
    }
    //correct format
    return true;
}

bool correct_start_time(struct tm &start_time, std::string input)
{
    unsigned int hours, minutes, seconds = 0;

    //get the HH:MM:SS as individuel numbers
    //
    std::sscanf(input.substr(0,2).c_str(), "%d", &hours);
    input.erase(0,3);

    //wrong format 
    if(hours > 24)
    {
        return false;
    }

    std::sscanf(input.substr(0,2).c_str(), "%d", &minutes);
    input.erase(0,3);
    
    std::sscanf(input.substr(0,2).c_str(), "%d", &seconds);
    input.erase(0,3);

    //wrong format 
    if(minutes > 60 || seconds > 60)
    {
        return false;
    }

    start_time.tm_hour = hours;
    start_time.tm_min = minutes;
    start_time.tm_sec = seconds;

    return true;
}



int working_hours_prompt()
{
    //Time of Working hours in hours
    int working_hours = 0;

    //Get Input
    while(true)
    {

        std::string input = "";

        //TUI
        CLEAR_SCREEN;
        std::cout << "How many hours do u want to work in hours: ";
        std::cin >> input;

        //Error handleing of cin
        if(std::cin.fail())
        {
            continue;
        }

        //get succesfull user input
        if(std::sscanf(input.c_str(), "%d", &working_hours) > 0)
        {
            //finish if working hours is a positive number
            if(working_hours >= 0)
            {
                return working_hours;
            }
            continue;
        }

        //error - everything failed
        return -1;
    }
}


int break_minutes_prompt()
{
    //Time of Working hours in hours
    int break_minutes = 0;

    //Get Input
    while(true)
    {

        std::string input = "";

        //TUI
        CLEAR_SCREEN;
        std::cout << "How many minutes do u have as break in minutes: ";
        std::cin >> input;

        //Error handleing of cin
        if(std::cin.fail())
        {
            continue;
        }

        //get succesfull user input
        if(std::sscanf(input.c_str(), "%d", &break_minutes) > 0)
        {
            //finish if working hours is a positive number
            if(break_minutes >= 0)
            {
                return break_minutes;
            }
            continue;
        }

        //error - everything failed
        return -1;
    }
}

//calculates the time it will be after break and working
//prints the result to cout
void calc_and_print_time(struct tm start_time, int working_hours, int break_minutes)
{
    //string for formatted cout "HH:MM:SS"
    char time_output[50];

    struct std::tm result_time = start_time;

    //add working_hours and break_minutes
    result_time.tm_hour = result_time.tm_hour + working_hours;
    result_time.tm_min = result_time.tm_min + break_minutes;

    //Correction of time - rebuild struct 
    //e.g. 25:12:34 -> 01:12:34
    mktime(&result_time);

    //char[] as "HH:MM:SS"
    strftime(time_output, 50, "%H:%M:%S", &result_time);

    std::cout << "Your time to go home: " << time_output << std::endl;
}

