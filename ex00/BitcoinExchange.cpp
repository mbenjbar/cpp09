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
    std::string y_str = date.substr(0, n);
    double year = std::strtod(y_str.c_str(), NULL);
    if(year > std::numeric_limits<double>::max() || year <= 0)
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        return (false);
    }
}





bool handle_date(std::string &date, std::string &line);
void output_print(std::map<std::string, float> &data ,std::string &date, double &btc_val);