#include <iostream>
#include <kanet.h>
int main(int argc, char const *argv[])
{
    try{
        Kanet kanet;
        kanet.start(13003);
    }catch(SocketException & e){
        std::cout << e.description() << std::endl;
    }
    return 0;
}
