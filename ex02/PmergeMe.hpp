#pragma once
#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <limits>
#include <sys/time.h>
#include <set>

struct Node
{
    int value;
    size_t id;
};

struct PendingNode
{
    Node small;
    size_t big_pair_id;
};

class PmergeMe
{
    private:
        size_t g_comparaisons;
        std::vector<Node> vc;
        std::deque<Node> dq;

    public:
        PmergeMe();
        PmergeMe(int ac, char **av);
        PmergeMe(const PmergeMe &other);
        PmergeMe &operator=(const PmergeMe &other);
        ~PmergeMe();

        void display_before() const;
        void display_after() const;
        void display_timing(double vectorTime, double dequeTime) const;
        void run();

        double get_time_us() const;
        std::vector<size_t> build_jacob_order(size_t n) const;

        template <typename Container>
        void merge_insert(Container &container);

        template <typename Container>
        double sort(Container &container);

        template <typename Container>
        void check_sorted(const Container &container) const;
};

bool is_valid(std::string arg);