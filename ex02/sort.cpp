#include "PmergeMe.hpp"

template <typename Container>
double PmergeMe::sort(Container &container)
{
    double start = get_time_us();

    merge_insert(container);

    double end = get_time_us();

    return (end - start);
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

        ++g_comparaisons;
        if (second.value < first.value)
        {
            main_chain.push_back(first);

            PendingNode p;
            p.small = second;
            p.bigId = first.id;
            pending.push_back(p);
        }
        else
        {
            main_chain.push_back(second);

            PendingNode p;
            p.small = first;
            p.bigId = second.id;
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

    std::vector<PendingNode> sortedPending;

    for (size_t i = 0; i < main_chain.size(); i++)
    {
        for (size_t j = 0; j < pending.size(); j++)
        {
            if (main_chain[i].id == pending[j].bigId)
            {
                sortedPending.push_back(pending[j]);
                break;
            }
        }
    }

    std::vector<size_t> order = buildJacobsthalOrder(sortedPending.size());

    for (size_t j = 0; j < order.size(); j++)
    {
        size_t idx = order[j];

        Node value = sortedPending[idx].small;
        size_t bigId = sortedPending[idx].bigId;

        typename Container::iterator bound = main_chain.begin();

        while (bound != main_chain.end() && bound->id != bigId)
            ++bound;

        typename Container::iterator pos = countedLowerBound(main_chain.begin(), bound, value.value);

        main_chain.insert(pos, value);
    }

    if (has_odd)
    {
        typename Container::iterator pos =
            countedLowerBound(main_chain.begin(), main_chain.end(), odd_value.value);
        main_chain.insert(pos, odd_value);
    }

    container.clear();
    container.insert(container.end(), main_chain.begin(), main_chain.end());
}