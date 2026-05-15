// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_set_pending_write_cb at ares_socket.c:417:6 in ares.h
// ares_process_pending_write at ares_process.c:408:6 in ares.h
// ares_set_socket_configure_callback at ares_socket.c:406:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_set_server_state_callback at ares_update_servers.c:1354:6 in ares.h
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_set_socket_functions at ares_set_socket_functions.c:579:6 in ares.h
// ares_set_socket_functions_ex at ares_set_socket_functions.c:89:3 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <unistd.h>
#include <fcntl.h>

/* Dummy callback functions with correct signatures */
static void dummy_pending_write_cb(void *user_data) {
    /* Do nothing */
}

static int dummy_sock_config_cb(int socket_fd, int type, void *user_data) {
    /* Do nothing */
    return 0;
}

static void dummy_server_state_cb(const char *addr, ares_bool_t is_up, int timeout, void *user_data) {
    /* Do nothing */
}

static int dummy_sock_create_cb(int af, int type, void *user_data) {
    /* Do nothing */
    return ARES_SOCKET_BAD;
}

static ares_socket_t dummy_asocket_cb(int domain, int type, int protocol, void *user_data) {
    /* Do nothing */
    return ARES_SOCKET_BAD;
}

static int dummy_socket_functions_available = 0;
static struct ares_socket_functions_ex dummy_sock_funcs_ex;
static struct ares_socket_functions dummy_sock_funcs;

static void initialize_dummy_socket_functions(void) {
    if (dummy_socket_functions_available) return;
    
    /* Initialize legacy socket functions */
    dummy_sock_funcs.asocket = dummy_asocket_cb;
    dummy_sock_funcs.aclose = NULL;
    dummy_sock_funcs.aconnect = NULL;
    dummy_sock_funcs.arecvfrom = NULL;
    dummy_sock_funcs.asendv = NULL;
    
    /* Initialize extended socket functions */
    dummy_sock_funcs_ex.version = 1;
    dummy_sock_funcs_ex.flags = 0;
    dummy_sock_funcs_ex.asocket = dummy_asocket_cb;
    dummy_sock_funcs_ex.aclose = NULL;
    dummy_sock_funcs_ex.asetsockopt = NULL;
    dummy_sock_funcs_ex.aconnect = NULL;
    dummy_sock_funcs_ex.arecvfrom = NULL;
    dummy_sock_funcs_ex.asendto = NULL;
    dummy_sock_funcs_ex.agetsockname = NULL;
    dummy_sock_funcs_ex.abind = NULL;
    dummy_sock_funcs_ex.aif_nametoindex = NULL;
    dummy_sock_funcs_ex.aif_indextoname = NULL;
    
    dummy_socket_functions_available = 1;
}

static void create_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            size_t write_size = size > 1024 ? 1024 : size;
            write(fd, data, write_size);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    ares_channel_t *channel = NULL;
    ares_status_t status;
    
    /* Create dummy file for potential resolv.conf/hosts file usage */
    create_dummy_file(Data, Size);
    
    /* Initialize channel with default options */
    status = ares_init(&channel);
    if (status != ARES_SUCCESS) {
        unlink("./dummy_file");
        return 0;
    }
    
    if (channel == NULL) {
        unlink("./dummy_file");
        return 0;
    }
    
    /* Initialize dummy socket functions */
    initialize_dummy_socket_functions();
    
    /* Use fuzzer data to determine which functions to call and with what parameters */
    if (Size > 0) {
        uint8_t selector = Data[0];
        size_t data_index = 1;
        
        /* Call functions based on selector bits */
        for (int i = 0; i < 6 && data_index < Size; i++) {
            if (selector & (1 << i)) {
                switch (i) {
                    case 0:
                        /* ares_set_pending_write_cb */
                        ares_set_pending_write_cb(channel, dummy_pending_write_cb, 
                                                  (void *)(uintptr_t)(data_index < Size ? Data[data_index] : 0));
                        data_index++;
                        break;
                        
                    case 1:
                        /* ares_process_pending_write */
                        ares_process_pending_write(channel);
                        break;
                        
                    case 2:
                        /* ares_set_socket_configure_callback */
                        ares_set_socket_configure_callback(channel, dummy_sock_config_cb,
                                                          (void *)(uintptr_t)(data_index < Size ? Data[data_index] : 0));
                        data_index++;
                        break;
                        
                    case 3:
                        /* ares_reinit */
                        ares_reinit(channel);
                        break;
                        
                    case 4:
                        /* ares_set_server_state_callback */
                        ares_set_server_state_callback(channel, dummy_server_state_cb,
                                                      (void *)(uintptr_t)(data_index < Size ? Data[data_index] : 0));
                        data_index++;
                        break;
                        
                    case 5:
                        /* ares_set_socket_callback */
                        ares_set_socket_callback(channel, dummy_sock_create_cb,
                                                (void *)(uintptr_t)(data_index < Size ? Data[data_index] : 0));
                        data_index++;
                        break;
                }
            }
        }
        
        /* Try to set socket functions if we have enough data */
        if (Size >= 2) {
            uint8_t sock_func_selector = Data[1];
            if (sock_func_selector & 0x01) {
                ares_set_socket_functions(channel, &dummy_sock_funcs, NULL);
            }
            if (sock_func_selector & 0x02) {
                ares_set_socket_functions_ex(channel, &dummy_sock_funcs_ex, NULL);
            }
        }
        
        /* Try to set some options based on fuzzer data */
        if (Size >= 3) {
            struct ares_options options;
            int optmask = 0;
            
            if (Data[2] & 0x01) {
                options.timeout = 1000;
                optmask |= ARES_OPT_TIMEOUTMS;
            }
            if (Data[2] & 0x02) {
                options.tries = 2;
                optmask |= ARES_OPT_TRIES;
            }
            if (Data[2] & 0x04) {
                options.ndots = 1;
                optmask |= ARES_OPT_NDOTS;
            }
            
            if (optmask > 0) {
                ares_channel_t *new_channel = NULL;
                ares_init_options(&new_channel, &options, optmask);
                if (new_channel) {
                    ares_destroy(channel);
                    channel = new_channel;
                }
            }
        }
    }
    
    /* Clean up */
    ares_destroy(channel);
    
    /* Remove dummy file */
    unlink("./dummy_file");
    
    return 0;
}