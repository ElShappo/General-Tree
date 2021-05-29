#ifndef NODE_EXCEPTION_HPP
#define NODE_EXCEPTION_HPP

class NodeException : public std::invalid_argument
{
    public:
        NodeException(const char* ex)
        : invalid_argument(ex)
        {

        }

    private:
};

#endif // NODE_EXCEPTION_HPP
