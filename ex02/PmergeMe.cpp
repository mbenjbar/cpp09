#include "PmergeMe.hpp"


bool is_valid(std::string arg)
{
    double nbr;
    if (arg.empty() || (arg.find_first_not_of("0123456789") != std::string::npos))
        return (false);
    nbr = std::strtod(arg.c_str(), 0);
    if (nbr <= 0 || nbr > std::numeric_limits<int>::max())
        return (false);
    return (true);
}

PmergeMe::PmergeMe(){}

PmergeMe::PmergeMe(int ac, char **av)
{
    g_comparaisons = 0;
    std::set<int> seen;

    for (int i = 1; i < ac; i++)
    {
        if (is_valid(av[i]) == false)
            throw std::invalid_argument("Error");

        int value = std::atoi(av[i]);

        if (seen.find(value) != seen.end())
            throw std::invalid_argument("Error");

        seen.insert(value);

        Node tmp;
        tmp.value = value;
        tmp.id = static_cast<size_t>(i - 1);

        vc.push_back(tmp);
        dq.push_back(tmp);
    }
}

PmergeMe::PmergeMe(const PmergeMe &other) {*this = other;}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    if (this != &other)
    {
        vc = other.vc;
        dq = other.dq;
    }
    return (*this);
}

PmergeMe::~PmergeMe() {}

template <typename Iterator>
Iterator countedLowerBound(Iterator first, Iterator last, int value)
{
    while (first < last)
    {
        Iterator middle = first + (last - first) / 2;

        ++g_comparisons;
        if (middle->value < value)
            first = middle + 1;
        else
            last = middle;
    }
    return first;
}

std::vector<size_t> PmergeMe::build_jacob_order(size_t n) const
{
    std::vector<size_t> order;

    if (n == 0)
        return order;

    order.push_back(0);

    size_t prev = 1;
    size_t curr = 1;
    size_t last = 1;

    while (last < n)
    {
        size_t limit = curr;

        if (limit > n)
            limit = n;

        for (size_t i = limit; i > last; i--)
            order.push_back(i - 1);

        last = limit;

        size_t next = curr + 2 * prev;
        prev = curr;
        curr = next;
    }

    return order;
}


double PmergeMe::get_time_us() const
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return static_cast<double>(time.tv_sec) * 1000000.0
         + static_cast<double>(time.tv_usec);
}

template <typename Container>
void PmergeMe::check_sorted(const Container &container) const
{
    for (size_t i = 1; i < container.size(); i++)
    {
        if (container[i].value < container[i - 1].value)
            throw std::runtime_error("Error");
    }
}

void PmergeMe::run()
{
    display_before();

    g_comparaisons = 0;
    double vc_time = sort(vc);
    // std::cout << "Number of comparisons with std::vector : "
    //           << g_comparaisons << std::endl;

    g_comparaisons = 0;
    double dq_time = sort(dq);
    // std::cout << "Number of comparisons with std::deque : "
    //           << g_comparaisons << std::endl;

    check_sorted(vc);
    check_sorted(dq);

    display_after();
    display_timing(vc_time, dq_time);
}