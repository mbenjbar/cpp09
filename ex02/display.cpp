#include "PmergeMe.hpp"

void PmergeMe::display_before() const
{
    std::cout << "Before: ";
    for (size_t i = 0; i < vc.size(); i++)
        std::cout << vc[i].value << " ";
    std::cout << std::endl;
}

void PmergeMe::display_after() const
{
    std::cout << "After: ";
    for (size_t i = 0; i < vc.size(); i++)
        std::cout << vc[i].value << " ";
    std::cout << std::endl;
}

void PmergeMe::display_timing(double vc_time, double dq_time) const
{
    std::cout << "Time to process a range of " << vc.size()
              << " elements with std::vector : "
              << vc_time << " us" << std::endl;

    std::cout << "Time to process a range of " << dq.size()
              << " elements with std::deque : "
              << dq_time << " us" << std::endl;
}