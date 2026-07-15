#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <string>
#include <limits>

bool process_line(std::string &line, double &btc_val);
bool handle_date(std::string &date, std::string &line);
void output_print(std::map<std::string, float> &data ,std::string &date, double &btc_val);