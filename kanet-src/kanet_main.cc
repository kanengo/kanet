#include <iostream>
#include <kanet.h>
int main(int argc, char const *argv[])
{
    Server::GetInstance()->run();
    return 0;
}
