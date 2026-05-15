#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "ares.h"
#include "ares_dns_record.h"

namespace harness_009 {
/* Comprehensive fuzzing harness for c-ares library targeting legacy socket configuration
 * and error handling APIs identified as critical coverage gaps:
 * - ares_set_socket_functions (legacy API, different from harness_008's ares_set_socket_functions_ex)
 * - ares_strerror (error message retrieval)
 * - Related socket configuration and error handling callback APIs
 * 
 * This harness implements the legacy socket callback functions (simpler API than _ex version)
 * to test socket function override mechanism and comprehensive error handling scenarios.
 * 
 * Semantic diversity from existing harnesses (000-008):
 * - harness_000: Modern APIs (getaddrinfo, DNS record)
 * - harness_001: Legacy DNS processing pipeline  
 * - harness_002: Event processing and channel management
 * - harness_003: Server configuration and option setting
 * - harness_004: Legacy DNS parsing and query creation
 * - harness_005: Legacy DNS mapping and Android/system config
 * - harness_006: Metrics and search operations
 * - harness_007: DNS record binary data manipulation
 * - harness_008: Socket callback mechanisms (ares_set_socket_functions_ex) and socket management
 * - harness_009: Legacy socket configuration (ares_set_socket_functions) and error handling (ares_strerror) (NEW)
 */



/* Custom legacy socket callback functions for ares_set_socket_functions */
/* Simpler API than the _ex version used in harness_008 */

/* Custom socket creation function (legacy API) */
static ares_socket_t legacy_asocket(int domain, int type, int protocol, void *user_data) {
    (void)user_data;
    
    /* Try to create socket based on fuzzed parameters */
    ares_socket_t sock = socket(domain, type, protocol);
    if (sock == ARES_SOCKET_BAD) {
        /* Set errno appropriately */
        errno = EAFNOSUPPORT;
        return ARES_SOCKET_BAD;
    }
    
    /* Try to set non-blocking mode */
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags != -1) {
        fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    }
    
    return sock;
}

/* Custom socket close function (legacy API) */
static int legacy_aclose(ares_socket_t sock, void *user_data) {
    (void)user_data;
    
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    /* Sometimes simulate close failure based on fuzzed input */
    return close(sock);
}

/* Custom socket connect function (legacy API) */
static int legacy_aconnect(ares_socket_t sock, const struct sockaddr *address, 
                           ares_socklen_t address_len, void *user_data) {
    (void)user_data;
    
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    /* Try to connect */
    int result = connect(sock, address, address_len);
    if (result == -1 && errno == EINPROGRESS) {
        /* Expected for non-blocking sockets */
        return -1;
    }
    
    return result;
}

/* Custom socket receive function (legacy API) */
static ares_ssize_t legacy_arecvfrom(ares_socket_t sock, void *buffer, size_t length,
                                     int flags, struct sockaddr *address,
                                     ares_socklen_t *address_len, void *user_data) {
    (void)flags;
    (void)user_data;
    
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    /* Simulate different receive scenarios */
    if (address != NULL && address_len != NULL && *address_len > 0) {
        /* Fill with dummy address data */
        memset(address, 0, (size_t)*address_len);
        struct sockaddr_in *addr_in = (struct sockaddr_in *)address;
        addr_in->sin_family = AF_INET;
        addr_in->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        addr_in->sin_port = htons(53);
    }
    
    /* Simulate receiving some data */
    if (length > 0 && buffer != NULL) {
        memset(buffer, 0, length);
        return (ares_ssize_t)(length > 512 ? 512 : length);
    }
    
    return -1;
}

/* Custom socket send function (legacy API) */
static ares_ssize_t legacy_asendv(ares_socket_t sock, const struct iovec *vector,
                                  int count, void *user_data) {
    (void)user_data;
    
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    if (vector == NULL || count <= 0) {
        errno = EINVAL;
        return -1;
    }
    
    /* Calculate total size */
    size_t total = 0;
    for (int i = 0; i < count; i++) {
        if (vector[i].iov_base != NULL) {
            total += vector[i].iov_len;
        }
    }
    
    /* Simulate sending data */
    return (ares_ssize_t)(total > 1024 ? 1024 : total);
}

/* Test ares_strerror with various error codes */
static void test_ares_strerror(FuzzedDataProvider &fdp) {
    /* Test with valid error codes */
    const int valid_error_codes[] = {
        ARES_SUCCESS,
        ARES_ENODATA,
        ARES_EFORMERR,
        ARES_ESERVFAIL,
        ARES_ENOTFOUND,
        ARES_ENOTIMP,
        ARES_EREFUSED,
        ARES_EBADQUERY,
        ARES_EBADNAME,
        ARES_EBADFAMILY,
        ARES_EBADRESP,
        ARES_ECONNREFUSED,
        ARES_ETIMEOUT,
        ARES_EOF,
        ARES_EFILE,
        ARES_ENOMEM,
        ARES_EDESTRUCTION,
        ARES_EBADSTR,
        ARES_EBADFLAGS,
        ARES_ENONAME,
        ARES_EBADHINTS,
        ARES_ENOTINITIALIZED,
        ARES_ELOADIPHLPAPI,
        ARES_EADDRGETNETWORKPARAMS,
        ARES_ECANCELLED,
        ARES_ESERVICE,
        ARES_ENOSERVER
    };
    /* Test some valid codes */
    for (int i = 0; i < 5 && fdp.remaining_bytes() >= sizeof(int); i++) {
        int error_idx = fdp.ConsumeIntegralInRange<int>(0, sizeof(valid_error_codes)/sizeof(valid_error_codes[0]) - 1);
        const char *error_str = ares_strerror(valid_error_codes[error_idx]);
        (void)error_str; /* Use result to avoid unused variable warning */
    }
    
    /* Test with random/invalid error codes */
    for (int i = 0; i < 3 && fdp.remaining_bytes() >= sizeof(int); i++) {
        int random_error = fdp.ConsumeIntegral<int>();
        const char *error_str = ares_strerror(random_error);
        (void)error_str; /* Use result to avoid unused variable warning */
    }
    ares_strerror(-1);
    ares_strerror(0);
    ares_strerror(99999);
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    /* Minimum size check */
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    /* Initialize c-ares library */
    ares_library_init(ARES_LIB_INIT_ALL);

    /* Create channel */
    ares_channel_t *channel = NULL;
    int init_result = ares_init(&channel);
    if (init_result != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    /* Consume operation selection */
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 3;

    /* Test different socket configuration and error handling scenarios */
    switch (operation) {
        case 0: {
            /* Test ares_set_socket_functions with legacy callbacks */
            struct ares_socket_functions legacy_funcs;
            legacy_funcs.asocket = legacy_asocket;
            legacy_funcs.aclose = legacy_aclose;
            legacy_funcs.aconnect = legacy_aconnect;
            legacy_funcs.arecvfrom = legacy_arecvfrom;
            legacy_funcs.asendv = legacy_asendv;
            
            /* Create user data for callbacks */
            void *user_data = (void *)(uintptr_t)fdp.ConsumeIntegral<uint32_t>();
            
            /* Set the legacy socket functions */
            ares_set_socket_functions(channel, &legacy_funcs, user_data);
            
            /* Test that socket functions are set by doing some operations */
            
            /* Clean up socket functions by setting them to NULL */
            ares_set_socket_functions(channel, NULL, NULL);
            break;
        }
        
        case 1: {
            /* Test comprehensive error handling with ares_strerror */
            test_ares_strerror(fdp);
            
            /* Also test error handling with channel operations */
            struct ares_socket_functions legacy_funcs;
            memset(&legacy_funcs, 0, sizeof(legacy_funcs));
            
            /* Test with partially NULL function table */
            ares_set_socket_functions(channel, &legacy_funcs, NULL);
            
            /* Test with completely NULL function table */
            ares_set_socket_functions(channel, NULL, NULL);
            break;
        }
        
        case 2: {
            /* Test socket configuration variations */
            
            /* Test with different function combinations */
            struct ares_socket_functions partial_funcs;
            memset(&partial_funcs, 0, sizeof(partial_funcs));
            
            /* Randomly set some functions */
            if (fdp.ConsumeBool()) {
                partial_funcs.asocket = legacy_asocket;
            }
            if (fdp.ConsumeBool()) {
                partial_funcs.aclose = legacy_aclose;
            }
            if (fdp.ConsumeBool()) {
                partial_funcs.aconnect = legacy_aconnect;
            }
            if (fdp.ConsumeBool()) {
                partial_funcs.arecvfrom = legacy_arecvfrom;
            }
            if (fdp.ConsumeBool()) {
                partial_funcs.asendv = legacy_asendv;
            }
            
            void *user_data = (void *)(uintptr_t)fdp.ConsumeIntegral<uint32_t>();
            ares_set_socket_functions(channel, &partial_funcs, user_data);
            
            /* Test error message retrieval for operation status */
            /* Note: ares_init would fail here since channel already exists, so test with dummy operation */
            const char *success_msg = ares_strerror(ARES_SUCCESS);
            const char *error_msg = ares_strerror(ARES_ENOMEM);
            (void)success_msg;
            (void)error_msg;
            
            /* Clean up */
            ares_set_socket_functions(channel, NULL, NULL);
            break;
        }
    }

    /* Additional error handling tests with various error conditions */
    
    /* Test ares_strerror with more edge cases */
    if (fdp.remaining_bytes() > sizeof(int)) {
        int edge_error = fdp.ConsumeIntegral<int>();
        const char *edge_error_str = ares_strerror(edge_error);
        (void)edge_error_str;
    }
    
    /* Test with NULL channel (should be handled gracefully by ares_set_socket_functions) */
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        struct ares_socket_functions dummy_funcs;
        memset(&dummy_funcs, 0, sizeof(dummy_funcs));
        ares_set_socket_functions(NULL, &dummy_funcs, NULL);
    }
    
    /* Test channel destruction with active socket functions */
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        struct ares_socket_functions temp_funcs;
        temp_funcs.asocket = legacy_asocket;
        temp_funcs.aclose = legacy_aclose;
        temp_funcs.aconnect = legacy_aconnect;
        temp_funcs.arecvfrom = legacy_arecvfrom;
        temp_funcs.asendv = legacy_asendv;
        
        ares_set_socket_functions(channel, &temp_funcs, NULL);
    }

    /* Clean up channel */
    ares_destroy(channel);
    
    /* Clean up library */
    ares_library_cleanup();

    return 0;
}

} // namespace harness_009
