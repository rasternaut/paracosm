#include <print>

class autorun
{
private:
    /* data */
public:
    autorun(/* args */);
    ~autorun();
};

autorun::autorun(/* args */)
{
    std::print("I live!");
}

autorun::~autorun()
{
}
