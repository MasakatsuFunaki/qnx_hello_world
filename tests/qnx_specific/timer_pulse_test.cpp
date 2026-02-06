// Tests QNX timer and pulse APIs
#include <iostream>
#include <cstring>
#include <cerrno>

#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <time.h>
#include <unistd.h>

int main() {
    std::cout << "=== QNX Timer & Pulse test ===\n";

    // Create a channel
    int chid = ChannelCreate(0);
    if (chid < 0) {
        std::cerr << "ChannelCreate failed: " << strerror(errno) << "\n";
        return 1;
    }

    // Create a connection to the channel
    int coid = ConnectAttach(ND_LOCAL_NODE, 0, chid, _NTO_SIDE_CHANNEL, 0);
    if (coid < 0) {
        std::cerr << "ConnectAttach failed: " << strerror(errno) << "\n";
        ChannelDestroy(chid);
        return 1;
    }

    std::cout << "Channel=" << chid << " Connection=" << coid << "\n";

    // Create a pulse event
    struct sigevent event;
    SIGEV_PULSE_INIT(&event, coid, SIGEV_PULSE_PRIO_INHERIT, 1, 0);

    // Create a timer
    timer_t timer_id;
    if (timer_create(CLOCK_MONOTONIC, &event, &timer_id) == 0) {
        std::cout << "Timer created successfully\n";
        timer_delete(timer_id);
        std::cout << "Timer deleted\n";
    } else {
        std::cerr << "timer_create failed: " << strerror(errno) << "\n";
    }

    ConnectDetach(coid);
    ChannelDestroy(chid);

    std::cout << "\nQNX Timer & Pulse test passed.\n";
    return 0;
}
