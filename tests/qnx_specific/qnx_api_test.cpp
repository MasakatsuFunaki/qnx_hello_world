// Tests QNX-specific POSIX and system APIs
// These headers are provided by QNX SDP and verify the toolchain finds them
#include <iostream>
#include <cstring>
#include <cerrno>

// POSIX headers
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <dirent.h>
#include <sys/mman.h>
#include <sys/wait.h>

// QNX-specific headers
#include <sys/neutrino.h>
#include <sys/syspage.h>
#include <sys/netmgr.h>

// Networking
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// ── POSIX thread function ───────────────────────────────────────────────────
void* thread_func(void* arg) {
    int* val = static_cast<int*>(arg);
    *val = *val * 2;
    return nullptr;
}

int main() {
    std::cout << "=== QNX-specific API test ===\n";

    // ── Process info ────────────────────────────────────────────────────────
    std::cout << "PID: " << getpid() << "\n";
    std::cout << "PPID: " << getppid() << "\n";
    std::cout << "UID: " << getuid() << "\n";
    std::cout << "GID: " << getgid() << "\n";

    // ── QNX syspage ─────────────────────────────────────────────────────────
    std::cout << "CPUs: " << _syspage_ptr->num_cpu << "\n";

    // ── Clock / time ────────────────────────────────────────────────────────
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    std::cout << "CLOCK_MONOTONIC: " << ts.tv_sec << "s " << ts.tv_nsec << "ns\n";

    clock_gettime(CLOCK_REALTIME, &ts);
    std::cout << "CLOCK_REALTIME: " << ts.tv_sec << "s\n";

    // ── POSIX pthread ───────────────────────────────────────────────────────
    std::cout << "\n--- POSIX pthread ---\n";
    int thread_val = 21;
    pthread_t tid;
    int rc = pthread_create(&tid, nullptr, thread_func, &thread_val);
    if (rc == 0) {
        pthread_join(tid, nullptr);
        std::cout << "pthread result: " << thread_val << " (expected 42)\n";
    } else {
        std::cout << "pthread_create failed: " << strerror(rc) << "\n";
    }

    // ── POSIX mutex ─────────────────────────────────────────────────────────
    std::cout << "\n--- POSIX mutex ---\n";
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mtx);
    std::cout << "mutex locked and unlocked successfully\n";
    pthread_mutex_unlock(&mtx);
    pthread_mutex_destroy(&mtx);

    // ── POSIX semaphore ─────────────────────────────────────────────────────
    std::cout << "\n--- POSIX semaphore ---\n";
    sem_t sem;
    sem_init(&sem, 0, 1);
    sem_wait(&sem);
    std::cout << "semaphore acquired\n";
    sem_post(&sem);
    sem_destroy(&sem);

    // ── File operations ─────────────────────────────────────────────────────
    std::cout << "\n--- File operations ---\n";
    int fd = open("/dev/null", O_WRONLY);
    if (fd >= 0) {
        const char* msg = "test";
        ssize_t written = write(fd, msg, strlen(msg));
        std::cout << "wrote " << written << " bytes to /dev/null\n";
        close(fd);
    }

    // ── stat ────────────────────────────────────────────────────────────────
    struct stat st;
    if (stat("/proc", &st) == 0) {
        std::cout << "/proc is " << (S_ISDIR(st.st_mode) ? "a directory" : "not a directory") << "\n";
    }

    // ── Socket (just create and close, don't connect) ───────────────────────
    std::cout << "\n--- Socket API ---\n";
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock >= 0) {
        std::cout << "TCP socket created (fd=" << sock << ")\n";

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(0);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        std::cout << "inet_ntoa: " << inet_ntoa(addr.sin_addr) << "\n";

        close(sock);
    }

    // ── Signal handling ─────────────────────────────────────────────────────
    std::cout << "\n--- Signals ---\n";
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sigaction(SIGUSR1, &sa, nullptr);
    std::cout << "SIGUSR1 handler set to SIG_IGN\n";

    // ── QNX Neutrino: ChannelCreate/Destroy ─────────────────────────────────
    std::cout << "\n--- QNX Neutrino ---\n";
    int chid = ChannelCreate(0);
    if (chid >= 0) {
        std::cout << "Channel created (chid=" << chid << ")\n";
        ChannelDestroy(chid);
    } else {
        std::cout << "ChannelCreate: " << strerror(errno) << "\n";
    }

    // ── QNX Network Manager ────────────────────────────────────────────────
    std::cout << "ND_LOCAL_NODE = " << ND_LOCAL_NODE << "\n";

    std::cout << "\nQNX-specific API test passed.\n";
    return 0;
}
