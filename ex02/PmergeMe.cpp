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

template <typename Container>
void PmergeMe::sort_pending(Container &main_chain, std::vector<PendingNode> &pending, std::vector<PendingNode> &pending_sorted)
{
    for (size_t i = 0; i < main_chain.size(); i++)
    {
        for (size_t j = 0; j < pending.size(); j++)
        {
            if (main_chain[i].id == pending[j].big_pair_id)
            {
                pending_sorted.push_back(pending[j]);
                break;
            }
        }
    }
}

template <typename Iterator>
Iterator find_pos(Iterator first, Iterator last, int value, size_t &g_comparaisons)
{
    while (first < last)
    {
        Iterator middle = first + (last - first) / 2;

        ++g_comparaisons;
        if (middle->value < value)
            first = middle + 1;
        else
            last = middle;
    }
    return (first);
}

template <typename Container>
void PmergeMe::merge_insert(Container &container)
{
    if (container.size() < 2) return;

    Container main_chain;
    std::vector<PendingNode> pending;

    bool has_odd = false;
    Node odd_value;

    size_t i = 0;
    while (i + 1 < container.size())
    {
        Node first = container[i];
        Node second = container[i + 1];
        PendingNode p;

        ++g_comparaisons;
        if (second.value < first.value)
        {
            main_chain.push_back(first);
            p.small = second;
            p.big_pair_id = first.id;
            pending.push_back(p);
        }
        else
        {
            main_chain.push_back(second);
            p.small = first;
            p.big_pair_id = second.id;
            pending.push_back(p);
        }
        i += 2;
    }
    if (i < container.size())
    {
        has_odd = true;
        odd_value = container[i];
    }
    merge_insert(main_chain);

    std::vector<PendingNode> pending_sorted;
    sort_pending(main_chain, pending, pending_sorted);

    std::vector<size_t> order = build_jacob_order(pending_sorted.size());

    for (size_t j = 0; j < order.size(); j++)
    {
        size_t idx = order[j];

        Node value = pending_sorted[idx].small;
        size_t bigId = pending_sorted[idx].big_pair_id;

        typename Container::iterator bound = main_chain.begin();

        while (bound != main_chain.end() && bound->id != bigId)
            ++bound;

        typename Container::iterator pos = find_pos(main_chain.begin(), bound, value.value, g_comparaisons);

        main_chain.insert(pos, value);
    }

    if (has_odd == true)
    {
        typename Container::iterator pos = find_pos(main_chain.begin(), main_chain.end(), odd_value.value, g_comparaisons);
        main_chain.insert(pos, odd_value);
    }

    container.clear();
    container.insert(container.end(), main_chain.begin(), main_chain.end());
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

double PmergeMe::get_time_us() const
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return static_cast<double>(time.tv_sec) * 1000000.0
         + static_cast<double>(time.tv_usec);
}

template <typename Container>
double PmergeMe::sort(Container &container)
{
    double start = get_time_us();

    merge_insert(container);

    double end = get_time_us();

    return (end - start);
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