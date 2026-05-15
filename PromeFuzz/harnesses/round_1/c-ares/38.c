// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_threadsafety at ares_threads.c:632:13 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_set_socket_functions_ex at ares_set_socket_functions.c:89:3 in ares.h
// ares_queue_wait_empty at ares_threads.c:768:15 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "ares.h"

static int dummy_socket_callback(int sock, int type, void *user_data) {
    return ARES_SOCKET_BAD;
}

static ares_socket_t dummy_asocket(int domain, int type, int protocol, void *user_data) {
    return ARES_SOCKET_BAD;
}

static int dummy_aclose(ares_socket_t sock, void *user_data) {
    return 0;
}

static int dummy_aconnect(ares_socket_t sock, const struct sockaddr *address,
                          ares_socklen_t address_len, unsigned int flags,
                          void *user_data) {
    return -1;
}

static ares_ssize_t dummy_arecvfrom(ares_socket_t sock, void *buffer, size_t length,
                                    int flags, struct sockaddr *address,
                                    ares_socklen_t *address_len, void *user_data) {
    return -1;
}

static ares_ssize_t dummy_asendto(ares_socket_t sock, const void *buffer, size_t length,
                                  int flags, const struct sockaddr *address,
                                  ares_socklen_t address_len, void *user_data) {
    return -1;
}

static int dummy_asetsockopt(ares_socket_t sock, ares_socket_opt_t opt, const void *val,
                             ares_socklen_t val_size, void *user_data) {
    return 0;
}

static int dummy_agetsockname(ares_socket_t sock, struct sockaddr *address,
                              ares_socklen_t *address_len, void *user_data) {
    return -1;
}

static int dummy_abind(ares_socket_t sock, unsigned int flags,
                       const struct sockaddr *address, socklen_t address_len,
                       void *user_data) {
    return -1;
}

static unsigned int dummy_aif_nametoindex(const char *ifname, void *user_data) {
    return 0;
}

static const char *dummy_aif_indextoname(unsigned int ifindex, char *ifname_buf,
                                         size_t ifname_buf_len, void *user_data) {
    return NULL;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    ares_channel_t *channel = NULL;
    ares_channel_t *dup_channel = NULL;
    
    int options_mask = 0;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    
    if (Size > 0) {
        options.flags = Data[0];
        options_mask |= ARES_OPT_FLAGS;
    }
    
    if (Size > 1) {
        options.timeout = Data[1] * 10;
        options_mask |= ARES_OPT_TIMEOUTMS;
    }
    
    if (Size > 2) {
        options.tries = Data[2] % 5;
        if (options.tries == 0) options.tries = 1;
        options_mask |= ARES_OPT_TRIES;
    }
    
    const char *resolvconf_path = "./dummy_file";
    if (Size > 3) {
        int fd = open(resolvconf_path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd >= 0) {
            write(fd, Data + 3, Size > 100 ? 100 : Size - 3);
            close(fd);
        }
        options.resolvconf_path = (char *)resolvconf_path;
        options_mask |= ARES_OPT_RESOLVCONF;
    }
    
    int status = ares_init_options(&channel, &options, options_mask);
    if (status != ARES_SUCCESS) {
        return 0;
    }
    
    if (Size > 0) {
        uint8_t action = Data[0] % 6;
        
        switch (action) {
            case 0: {
                ares_dup(&dup_channel, channel);
                if (dup_channel) {
                    ares_destroy(dup_channel);
                }
                break;
            }
            case 1: {
                ares_cancel(channel);
                break;
            }
            case 2: {
                ares_bool_t ts = ares_threadsafety();
                (void)ts;
                break;
            }
            case 3: {
                ares_reinit(channel);
                break;
            }
            case 4: {
                ares_set_socket_callback(channel, dummy_socket_callback, NULL);
                
                struct ares_socket_functions_ex funcs;
                memset(&funcs, 0, sizeof(funcs));
                funcs.version = 1;
                funcs.asocket = dummy_asocket;
                funcs.aclose = dummy_aclose;
                funcs.aconnect = dummy_aconnect;
                funcs.arecvfrom = dummy_arecvfrom;
                funcs.asendto = dummy_asendto;
                funcs.asetsockopt = dummy_asetsockopt;
                funcs.agetsockname = dummy_agetsockname;
                funcs.abind = dummy_abind;
                funcs.aif_nametoindex = dummy_aif_nametoindex;
                funcs.aif_indextoname = dummy_aif_indextoname;
                
                ares_set_socket_functions_ex(channel, &funcs, NULL);
                break;
            }
            case 5: {
                int timeout = -1;
                if (Size > 1) {
                    timeout = (int)Data[1];
                }
                ares_queue_wait_empty(channel, timeout);
                break;
            }
        }
    }
    
    ares_destroy(channel);
    
    if (Size > 3) {
        unlink(resolvconf_path);
    }
    
    return 0;
}