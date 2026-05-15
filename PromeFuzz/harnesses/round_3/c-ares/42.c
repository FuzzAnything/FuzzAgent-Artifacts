// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_query at ares_query.c:133:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_process_fd at ares_process.c:267:6 in ares.h
// ares_process_fd at ares_process.c:267:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_fds at ares_fds.c:30:5 in ares.h
// ares_getsock at ares_getsock.c:29:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <sys/select.h>

static void dummy_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    // Empty callback for fuzzing
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    ares_channel_t *channel = NULL;
    int result;
    
    // Initialize channel
    result = ares_init(&channel);
    if (result != ARES_SUCCESS) {
        // If initialization fails, we can't proceed with other operations
        return 0;
    }
    
    // Use fuzzer data to decide which operations to perform
    // We'll use the first byte to select an operation
    if (Size > 0) {
        uint8_t operation = Data[0] % 6;  // 6 different operations
        
        switch (operation) {
            case 0:
                // ares_cancel
                ares_cancel(channel);
                break;
                
            case 1:
                // ares_process_fd - use fuzzer data for file descriptors
                if (Size >= 3) {
                    ares_socket_t read_fd = (ares_socket_t)Data[1];
                    ares_socket_t write_fd = (ares_socket_t)Data[2];
                    ares_process_fd(channel, read_fd, write_fd);
                } else {
                    // Default to invalid file descriptors
                    ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
                }
                break;
                
            case 2:
                // ares_reinit
                ares_reinit(channel);
                break;
                
            case 3:
                // ares_fds
                if (Size >= 2) {
                    fd_set read_fds, write_fds;
                    FD_ZERO(&read_fds);
                    FD_ZERO(&write_fds);
                    
                    // Use fuzzer data to set some file descriptors
                    uint8_t fd_count = Data[1] % 32;  // Limit to reasonable number
                    for (int i = 0; i < fd_count && i < 32; i++) {
                        if (Size > 2 + i) {
                            int fd = Data[2 + i] % 1024;  // Limit fd numbers
                            if (Data[1] % 2 == 0) {
                                FD_SET(fd, &read_fds);
                            } else {
                                FD_SET(fd, &write_fds);
                            }
                        }
                    }
                    
                    ares_fds(channel, &read_fds, &write_fds);
                }
                break;
                
            case 4:
                // ares_getsock
                if (Size >= 2) {
                    int numsocks = Data[1] % 64;  // Reasonable array size
                    if (numsocks > 0) {
                        ares_socket_t *socks = (ares_socket_t*)malloc(numsocks * sizeof(ares_socket_t));
                        if (socks) {
                            // Initialize with fuzzer data
                            for (int i = 0; i < numsocks && Size > 2 + i; i++) {
                                socks[i] = (ares_socket_t)Data[2 + i];
                            }
                            ares_getsock(channel, socks, numsocks);
                            free(socks);
                        }
                    }
                }
                break;
                
            case 5:
                // Try to create a dummy query to have something to cancel
                if (Size >= 2) {
                    char dummy_name[256];
                    size_t name_len = (Size - 1) < 255 ? (Size - 1) : 255;
                    memcpy(dummy_name, Data + 1, name_len);
                    dummy_name[name_len] = '\0';
                    
                    // Use integer values for DNS record types (1 = A record, 1 = IN class)
                    ares_query(channel, dummy_name, 1, 1, dummy_callback, NULL);
                }
                break;
        }
    }
    
    // Clean up
    if (channel) {
        ares_destroy(channel);
    }
    
    return 0;
}