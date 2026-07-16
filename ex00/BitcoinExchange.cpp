#include "BitcoinExchange.hpp"

bool check_btc(std::string val, double &btc_val)
{
    char *fin = NULL;
    btc_val = std::strtod(val.c_str(), &fin);
    if (btc_val > 1000)
        return (false);
    if (btc_val < 0)
        return (false);
    if (btc_val != btc_val)
        return (false);
    if ((fin && *fin != '\0') || isspace(val[0]))
        return (false);
}

bool check_date_format(std::string date)
{
    if (date.size() != 10)
        return (false);
    if (date[4] != '-' || date[7] != '-')
        return (false);
    for (size_t i = 0; i < date.size(); i++)
    {
        if (i == 4 || i == 7)
            continue;
        return (false);
    }
}

bool check_date(std::string &date, std::string &line)
{
    size_t len = date.size();
    int year = atoi(date.substr(0, len - 6).c_str());
    int day = atoi(date.substr(len - 2, 2).c_str());
    int month = atoi(date.substr(len - 5, 2).c_str());

    if (date < "2009-01-03")
	    return (std::cerr << "Error: btc did not exist in this date => " << line << std::endl, false);
    bool leap_year = false;
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        leap_year = true;

    if (month > 12 || month < 1 || day < 1)
        return (std::cerr << "Error: bad input => " << line << std::endl, false);
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
    {
        if (day > 31)
            return (std::cerr << "Error: bad input => " << line << std::endl, false);
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        if (day > 30)
            return (std::cerr << "Error: bad input => " << line << std::endl, false);
    }
    else if (month == 2)
    {
        if ((leap_year && day > 29) || (!leap_year && day > 28))
            return (std::cerr << "Error: bad input => " << line << std::endl, false);
    }
    line = date;
    return (true);
}

bool process_line(std::string &line, double &btc_val)
{
    size_t pos;
    std::string date;
    std::string val;
    

    pos = line.find(" | ");
    if (pos == std::string::npos)
        return (std::cerr << "Error: bad input => " << line << std::endl, false);
    date = line.substr(0, pos);
    val = line.substr(pos + 3);
    if (check_btc(val, btc_val) == false)
        return (std::cerr << "Error: bad input => " << line << std::endl, false);
    
    if (check_date_format(date) == false)
        return (std::cerr << "Error: bad input => " << line << std::endl, false);

    if (check_date(date, line) == false)
        return (false);
}

void output_print(std::map<std::string, float> &data ,std::string &date, double &btc_val)
{
    if (data.size() == 0)
    {
        std::cerr << "Error: the Data is empty." << std::endl;
        return ;
    }
    std::map<std::string, float>::iterator it = data.upper_bound(date);
    it = data.upper_bound(date);
    if (it != data.begin())
        --it;
    std::cout << date << " => " << btc_val << " = " << btc_val * it->second << std::endl;
}