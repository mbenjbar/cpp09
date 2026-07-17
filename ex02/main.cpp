#include "PmergeMe.hpp"

int main(int ac, char **av)
{
    try
    {
        if (ac <= 1)
            throw std::invalid_argument("Error");
        PmergeMe app(ac, av);
        app.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return (1);
    }
    return (0);
}