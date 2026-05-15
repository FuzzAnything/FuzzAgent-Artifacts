// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_set_socket_configure_callback at ares_socket.c:406:6 in ares.h
// ares_set_server_state_callback at ares_update_servers.c:1354:6 in ares.h
// ares_set_query_enqueue_cb at ares_init.c:607:6 in ares.h
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_set_pending_write_cb at ares_socket.c:417:6 in ares.h
// ares_process_fds at ares_process.c:251:15 in ares.h
// ares_process_fds at ares_process.c:251:15 in ares.h
// ares_process_fds at ares_process.c:251:15 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "ares.h"

/* Dummy callback functions with correct signatures */
static int dummy_sock_config_callback(ares_socket_t socket_fd, int type, void *user_data) {
    return 0;
}

static void dummy_server_state_callback(const char *host, ares_bool_t is_up, int time_ms, void *user_data) {
    /* Do nothing */
}

static void dummy_query_enqueue_cb(void *user_data) {
    /* Do nothing */
}

static int dummy_sock_create_callback(int domain, int type, void *user_data) {
    return ARES_SOCKET_BAD;
}

static void dummy_pending_write_cb(void *user_data) {
    /* Do nothing */
}

/* Helper to create dummy events array from fuzzer data */
static void create_dummy_events(const uint8_t *data, size_t size,
                                ares_fd_events_t **events_out, size_t *nevents_out) {
    if (size < sizeof(ares_fd_events_t)) {
        *events_out = NULL;
        *nevents_out = 0;
        return;
    }
    
    /* Use first byte to determine number of events (0-4) */
    size_t max_events = (data[0] % 5);
    if (max_events == 0) {
        *events_out = NULL;
        *nevents_out = 0;
        return;
    }
    
    /* Allocate events array */
    ares_fd_events_t *events = malloc(max_events * sizeof(ares_fd_events_t));
    if (!events) {
        *events_out = NULL;
        *nevents_out = 0;
        return;
    }
    
    /* Fill events with fuzzer data */
    size_t data_offset = 1;
    for (size_t i = 0; i < max_events; i++) {
        if (data_offset + sizeof(ares_fd_events_t) > size) {
            break;
        }
        
        /* Copy raw data for the event structure */
        memcpy(&events[i], data + data_offset, sizeof(ares_fd_events_t));
        data_offset += sizeof(ares_fd_events_t);
        (*nevents_out)++;
    }
    
    *events_out = events;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    ares_channel_t *channel = NULL;
    ares_status_t status;
    int options_mask = 0;
    
    /* Initialize channel with minimal options */
    struct ares_options options;
    options.timeout = 1000;
    options.tries = 1;
    options.ndots = 1;
    options.udp_port = 0;
    options.tcp_port = 0;
    options.flags = ARES_FLAG_NOALIASES;
    options.sock_state_cb = NULL;
    options.sock_state_cb_data = NULL;
    options.sortlist = NULL;
    options.nsort = 0;
    options.ednspsz = 0;
    options.qcache_max_ttl = 0;
    options.maxtimeout = 0;
    
    options_mask = ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES | ARES_OPT_NDOTS |
                   ARES_OPT_FLAGS | ARES_OPT_UDP_PORT | ARES_OPT_TCP_PORT;
    
    status = ares_init_options(&channel, &options, options_mask);
    if (status != ARES_SUCCESS || channel == NULL) {
        return 0;
    }
    
    /* Use fuzzer data to determine which callbacks to set */
    if (Size > 0) {
        uint8_t callback_selector = Data[0];
        size_t data_offset = 1;
        
        /* Set socket configure callback if bit 0 is set */
        if ((callback_selector & 0x01) && data_offset < Size) {
            ares_set_socket_configure_callback(channel, dummy_sock_config_callback,
                                              (void *)(uintptr_t)Data[data_offset]);
            data_offset++;
        }
        
        /* Set server state callback if bit 1 is set */
        if ((callback_selector & 0x02) && data_offset < Size) {
            ares_set_server_state_callback(channel, dummy_server_state_callback,
                                          (void *)(uintptr_t)Data[data_offset]);
            data_offset++;
        }
        
        /* Set query enqueue callback if bit 2 is set */
        if ((callback_selector & 0x04) && data_offset < Size) {
            ares_set_query_enqueue_cb(channel, dummy_query_enqueue_cb,
                                     (void *)(uintptr_t)Data[data_offset]);
            data_offset++;
        }
        
        /* Set socket callback if bit 3 is set */
        if ((callback_selector & 0x08) && data_offset < Size) {
            ares_set_socket_callback(channel, dummy_sock_create_callback,
                                    (void *)(uintptr_t)Data[data_offset]);
            data_offset++;
        }
        
        /* Set pending write callback if bit 4 is set */
        if ((callback_selector & 0x10) && data_offset < Size) {
            ares_set_pending_write_cb(channel, dummy_pending_write_cb,
                                     (void *)(uintptr_t)Data[data_offset]);
            data_offset++;
        }
        
        /* Process fds with remaining data */
        if (data_offset < Size) {
            ares_fd_events_t *events = NULL;
            size_t nevents = 0;
            unsigned int flags = 0;
            
            /* Create dummy events from remaining data */
            create_dummy_events(Data + data_offset, Size - data_offset,
                               &events, &nevents);
            
            /* Use last byte (or 0 if no data left) for flags */
            if (Size > 0) {
                flags = Data[Size - 1];
            }
            
            /* Process the events */
            ares_process_fds(channel, events, nevents, flags);
            
            /* Free events array if allocated */
            if (events) {
                free(events);
            }
        } else {
            /* No data left for events, process with NULL events */
            ares_process_fds(channel, NULL, 0, 0);
        }
    } else {
        /* Empty input, still test basic functionality */
        ares_process_fds(channel, NULL, 0, 0);
    }
    
    /* Clean up */
    ares_destroy(channel);
    
    return 0;
}