#include "BitcoinExchange.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "Error: try ./btc input_file" << std::endl;
        return (1);
    }
    std::ifstream data_file("data.csv");
	if (!data_file.is_open())
		return (std::cerr << "Error can not open the file data.csv" << std::endl, 1);
    std::map<std::string, float> data;
    std::string line;
    size_t pos;
    std::getline(data_file, line);
    while (std::getline(data_file, line))
    {
        pos = line.find(",");
        std::string key = line.substr(0, pos);
        float value = std::strtof(line.substr(pos + 1).c_str(), NULL);
        data[key] = value;
    }
    data_file.close();

    std::ifstream input_file(av[1]);
    if (!input_file.is_open())
		return (std::cerr << "Error can not open the input file" << std::endl, 1);
    std::getline(input_file, line);
	if (line.compare("date | value"))
		return (std::cerr << "Synatx error in the input file" << std::endl, 1);

    double btc_val;
    while (std::getline(input_file, line))
    {
        if (process_line(line, btc_val) == true)
			output_print(data, line, btc_val);
    }
    return 0;
}