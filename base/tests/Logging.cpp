#include "../Logging.h"
#include "../Thread.h"
#include <string>
#include <unistd.h>
#include <iostream>
#include <vector>
using namespace std;

void type_test() {
    cout << "this is type test" << endl;
    LOG << 0;
    LOG << 1234567890123;
    LOG << 1.0f;
    LOG << 3.1415926;
    LOG << 'c';
    LOG << "abcdefg";
    LOG << string("string test");
}

void thread_test() {

    for(int i = 0; i < 1000; i++) {
        LOG << i;
    
    }
}

int main() {

    type_test();
    sleep(3);

    thread_test();
    sleep(3);

    return 0;
}
