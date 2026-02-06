#include <iostream>
#include <unistd.h>
#include <sys/syspage.h>

int main() {
    std::cout << "Hello from QNX!" << std::endl;
    std::cout << "PID: " << getpid() << std::endl;
    std::cout << "CPUs: " << _syspage_ptr->num_cpu << std::endl;
    return 0;
}
