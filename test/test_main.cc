
#include "my_socket.h"
#include <unistd.h>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <thread>
#include <vector>

void testFunc1();
void testFunc2();
void testFunc3();


int main(int argc, char const *argv[])
{
    // testFunc3();
    // testFunc1();
    std::vector<std::thread> ts;
    for(int i = 0; i < 2; i++)
    {
        ts.emplace_back([](){
            testFunc1();
        });
    }
    for(int i = 0; i < ts.size(); i++)
    {
        ts[i].join();
    }

    return 0;
}

void testFunc1()
{
    MySocket socket = MySocket::CreateSocket();
    socket.connect("0.0.0.0", 13003);
    std::cout << "connect success fd:" << socket.sock_fd()<<std::endl;
    usleep(1000000);
    // std::cout << "send size:" << socket.send("hello world!!!!", 16, 0) << std::endl;

    std::string replay("A good morning text doesn't only mean 'Good morning'. It has a silent, loving message that says...'I think of you when I wake up.' 一条早安短信不只是道句'早安'。而是以一种静默而充满爱意的方式告诉你：'我一醒来就想你了。'");
    socket.send(replay.c_str(), replay.size() + 1, 0);
    // usleep(1000000);
    // std::cout << "send size:" << socket.send("hello world!!!!", 16, 0) << std::endl;
    // usleep(1000000);
    // std::cout << "send size:" << socket.send("hello world!!!!", 16, 0) << std::endl;
    // usleep(1000000);
    // std::cout << "send size:" << socket.send("hello world!!!!", 16, 0) << std::endl;
    // usleep(1000000);
    // std::cout << "send size:" << socket.send("hello world!!!!", 16, 0) << std::endl;
    // usleep(1000000);
    // std::cout << "send size:" << socket.send("hello world!!!!", 16, 0) << std::endl;
    // usleep(1000000);
    char buf[1024];
    std:: cout << "recv size:" << socket.recv(buf, 1024, 0) << std::endl;
    std::cout<< "recv data:" << buf << std::endl;
    socket.close();
}

class Parent;
class Child;
typedef std::shared_ptr<Parent> parent_ptr;
typedef std::shared_ptr<Child> child_ptr; 
typedef std::weak_ptr<Parent> parent_wptr;
class Parent
{
public:
    ~Parent()
    {
        std::cout << "Parent 对象析构" << std::endl;
    }

    child_ptr children;
};

class Child
{
public:
    ~Child()
    {
        std::cout << "Child 对象析构" << std::endl;
    }
    parent_wptr parent;
};

void testFunc2()
{
    {
        parent_ptr father(new Parent);
        child_ptr son(new Child);
        std::cout << "father ref count:" << father.use_count() << std::endl;
        std::cout << "son ref count:" << son.use_count() << std::endl;

        father->children = son;
        son->parent = father;
        std::cout << "father ref count:" << father.use_count() << std::endl;
        std::cout << "son ref count:" << son.use_count() << std::endl;
    }
    while(true);
}

void testFunc3()
{
    std::unordered_map<std::string, parent_ptr> map;
    {
        parent_ptr father(new Parent);
        map["father"] = father;
        std::cout << "father ref count:" << father.use_count() << std::endl;
    }
    std::cout << "father ref count:" << map["father"].use_count() << std::endl;
    parent_ptr father = map["father"];
    std::cout << "father ref count:" << father.use_count() << std::endl;
    map.erase("father");
    std::cout << "father ref count:" << father.use_count() << std::endl;    
}
