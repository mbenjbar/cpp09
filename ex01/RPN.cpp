#include "RPN.hpp"

bool is_operator(char c) {return (c == '+' || c == '-' || c == '*' || c == '/');}

bool check_input(char *input)
{
    for (int i = 0; input[i] != '\0'; i++)
    {
        if (input[i] <= 32)
            continue;
        if ((input[i] < '0' || input[i] > '9') && !is_operator(input[i]))
            return (std::cerr << "Error" << std::endl, false);
        if (input[i + 1] != '\0' && input[i + 1] != ' ')
            return (std::cerr << "Error" << std::endl, false);
    }
    return (true);
}

bool run_op(char *input)
{
    std::stack<int> op;
    int a, b;

    for (int i = 0; input[i]; i++)
    {
        if (input[i] <= 32)
            continue;

        if (input[i] >= '0' && input[i] <= '9')
            op.push(input[i] - '0');
        else if (is_operator(input[i]))
        {
            if (op.size() < 2)
                return (std::cerr << "Error" << std::endl, false);

            a = op.top();
            op.pop();

            b = op.top();
            op.pop();

            if (input[i] == '*')
                op.push(b * a);
            else if (input[i] == '+')
                op.push(b + a);
            else if (input[i] == '-')
                op.push(b - a);
            else if (input[i] == '/')
            {
                if (a == 0)
                    return (std::cerr << "Error" << std::endl, false);
                op.push(b / a);
            }
        }
    }
    if (op.size() != 1)
        return (std::cerr << "Error" << std::endl, false);
    std::cout << op.top() << std::endl;
    return (true);
}