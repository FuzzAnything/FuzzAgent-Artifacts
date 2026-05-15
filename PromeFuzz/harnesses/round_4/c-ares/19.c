// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_process at ares_process.c:334:6 in ares.h
// ares_query at ares_query.c:133:6 in ares.h
// ares_fds at ares_fds.c:30:5 in ares.h
// ares_process at ares_process.c:334:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_library_init at ares_library_init.c:108:5 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_fds at ares_fds.c:30:5 in ares.h
// ares_timeout at ares_timeout.c:135:17 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_timeout at ares_timeout.c:135:17 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

static void dummy_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    // Empty callback for queries
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    // Initialize library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Initialize channel
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    // Set some options from fuzzer data if available
    if (Size >= sizeof(struct ares_options)) {
        memcpy(&options, Data, sizeof(struct ares_options));
        optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    }
    
    int status = ares_init_options(&channel, &options, optmask);
    if (status != ARES_SUCCESS) {
        // Try with NULL options
        status = ares_init(&channel);
        if (status != ARES_SUCCESS) {
            ares_library_cleanup();
            return 0;
        }
    }
    
    // Create dummy file if needed (though not directly used by these functions)
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size > 1024 ? 1024 : Size);
        }
        close(fd);
    }
    
    // Prepare file descriptor sets
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    // Call functions in specified order
    // 1. ares_fds
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    
    // 2. ares_timeout (first call)
    struct timeval maxtv, tv;
    struct timeval *timeout_result;
    
    // Initialize maxtv with some values from fuzzer data
    if (Size >= 2 * sizeof(struct timeval)) {
        memcpy(&maxtv, Data, sizeof(struct timeval));
        // Use different part of data for tv
        memcpy(&tv, Data + sizeof(struct timeval), sizeof(struct timeval));
    } else {
        maxtv.tv_sec = 5;
        maxtv.tv_usec = 0;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
    }
    
    timeout_result = ares_timeout(channel, &maxtv, &tv);
    
    // 3. ares_cancel
    ares_cancel(channel);
    
    // 4. ares_timeout (second call) - after cancel
    // Use different maxtv values
    if (Size >= 3 * sizeof(struct timeval)) {
        memcpy(&maxtv, Data + 2 * sizeof(struct timeval), sizeof(struct timeval));
    } else {
        maxtv.tv_sec = 1;
        maxtv.tv_usec = 500000;
    }
    timeout_result = ares_timeout(channel, &maxtv, &tv);
    
    // 5. ares_process
    // Reset fd sets and set some bits based on fuzzer data
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    // Set some file descriptors based on fuzzer data
    if (Size > 0) {
        for (size_t i = 0; i < Size && i < FD_SETSIZE; i++) {
            if (Data[i] % 2 == 0) {
                FD_SET(i % FD_SETSIZE, &read_fds);
            }
            if (Data[i] % 3 == 0) {
                FD_SET(i % FD_SETSIZE, &write_fds);
            }
        }
    }
    
    ares_process(channel, &read_fds, &write_fds);
    
    // Try to trigger more states by initiating a query and processing again
    if (Size > 10) {
        // Use part of fuzzer data as domain name
        char domain[256];
        size_t domain_len = Size > 255 ? 255 : Size;
        memcpy(domain, Data, domain_len);
        domain[domain_len] = '\0';
        
        // Use standard DNS class and type values
        ares_query(channel, domain, 1, 1, dummy_callback, NULL);
        
        // Get fds again
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        ares_fds(channel, &read_fds, &write_fds);
        
        // Process with new fds
        ares_process(channel, &read_fds, &write_fds);
    }
    
    // Cleanup
    ares_destroy(channel);
    ares_library_cleanup();
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}