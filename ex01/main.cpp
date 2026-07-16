#include "RPN.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
        return (std::cerr << "Error" << std::endl, 1);
    if (check_input(av[1]) == false)
        return (1);
    if (!run_op(av[1]))
        return (1);
    return (0);
}