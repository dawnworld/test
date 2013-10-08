#include <stdio.h>
class TestClass
{
public:
    int i;
};

int main()
{
    TestClass *c = new TestClass;
    c->i = 10;
    delete c;
    delete c;
    return 0;
}
