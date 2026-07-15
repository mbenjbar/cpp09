#include "BitcoinExchange.hpp"


bool process_line(std::string &line, double &btc_val)
{
    size_t pos;
    std::string date;
    std::string val;
    char *fin = NULL;

    pos = line.find(" | ");
    if (pos == std::string::npos)
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        return (false);
    }
    date = line.substr(0, pos);
    val = line.substr(pos + 3);
    btc_val = std::strtod(val.c_str(), &fin);
    if (btc_val > 1000)
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        return (false);
    }
    if (btc_val < 0)
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        return (false);
    }
    if (btc_val != btc_val) // NAN case
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        return (false);
    }
    if ((fin && *fin != '\0') || isspace(val[0]))
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        return (false);
    }

    std::string::size_type n = date.find_first_not_of("0123456789");
    if (date.size() != 10)
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        return (false);
    }
    if (date[4] != '-' || date[7] != '-')
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        return (false);
    }

    for (size_t i = 0; i < date.size(); i++)
    {
        if (i == 4 || i == 7)
            continue;
        if (!std::isdigit(date[i]))
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            return (false);
        }
    }
}





bool handle_date(std::string &date, std::string &line);
void output_print(std::map<std::string, float> &data ,std::string &date, double &btc_val);