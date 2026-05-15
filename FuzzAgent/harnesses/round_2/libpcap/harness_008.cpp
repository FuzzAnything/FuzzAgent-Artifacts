#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <pcap/socket.h>  // For PCAP_SOCKET type
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

namespace harness_008 {

// Weak symbol declarations in case RPCAP functions are not available
#ifdef __cplusplus
extern "C" {
#endif

PCAP_SOCKET __attribute__((weak)) pcap_remoteact_accept(const char *address,
    const char *port, const char *hostlist, char *connectinghost,
    struct pcap_rmtauth *auth, char *errbuf);

PCAP_SOCKET __attribute__((weak)) pcap_remoteact_accept_ex(const char *address,
    const char *port, const char *hostlist, char *connectinghost,
    struct pcap_rmtauth *auth, int uses_ssl, char *errbuf);

int __attribute__((weak)) pcap_remoteact_list(char *hostlist, char sep, int size,
    char *errbuf);

int __attribute__((weak)) pcap_remoteact_close(const char *host, char *errbuf);

void __attribute__((weak)) pcap_remoteact_cleanup(void);

#ifdef __cplusplus
}
#endif

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need at least operation mode (1 byte), address length (1 byte),
    // port length (1 byte), hostlist length (1 byte), auth type (1 byte),
    // SSL flag (1 byte) = 6 bytes minimum
    if (size < 6) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation mode: 0 = pcap_remoteact_accept, 1 = pcap_remoteact_accept_ex
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Consume address string (listen address, can be NULL or empty for any address)
    size_t address_len = fdp.ConsumeIntegralInRange<size_t>(0, 128);
    std::string address = fdp.ConsumeBytesAsString(address_len);
    
    // Consume port string (can be NULL or empty for default port)
    size_t port_len = fdp.ConsumeIntegralInRange<size_t>(0, 16);
    std::string port = fdp.ConsumeBytesAsString(port_len);
    
    // Consume hostlist string (comma-separated list of allowed hosts, can be NULL)
    size_t hostlist_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string hostlist = fdp.ConsumeBytesAsString(hostlist_len);
    
    // Consume authentication type: 0 = NULL auth, 1 = password auth
    uint8_t auth_type = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Consume SSL flag (only for pcap_remoteact_accept_ex)
    uint8_t use_ssl = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Remaining bytes are for additional test strings (could be used for error messages or other parameters)
    std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_rmtauth auth;
    char connectinghost[RPCAP_HOSTLIST_SIZE] = {0};  // Buffer for connecting host info
    
    // Initialize authentication structure if needed
    if (auth_type == 1) {
        auth.type = RPCAP_RMTAUTH_PWD;
        // Use simple test credentials
        auth.username = (char*)"fuzzer_user";
        auth.password = (char*)"fuzzer_pass";
    } else {
        // NULL authentication
        auth.type = RPCAP_RMTAUTH_NULL;
        auth.username = NULL;
        auth.password = NULL;
    }
    
    // Prepare parameters for the API calls
    const char* address_ptr = address.empty() ? NULL : address.c_str();
    const char* port_ptr = port.empty() ? NULL : port.c_str();
    const char* hostlist_ptr = hostlist.empty() ? NULL : hostlist.c_str();
    
    PCAP_SOCKET sock = (PCAP_SOCKET)-1;
    
    // Check if RPCAP functions are available before calling
    if (pcap_remoteact_accept != NULL) {
        if (operation_mode == 0) {
            // Test pcap_remoteact_accept (non-SSL version)
            sock = pcap_remoteact_accept(address_ptr, port_ptr, hostlist_ptr, 
                                        connectinghost, &auth, errbuf);
        } else if (pcap_remoteact_accept_ex != NULL) {
            // Test pcap_remoteact_accept_ex (with SSL option)
            sock = pcap_remoteact_accept_ex(address_ptr, port_ptr, hostlist_ptr,
                                           connectinghost, &auth, use_ssl, errbuf);
        }
    }
    
    // The function may return different error values:
    // -1: sock_init() failed (Winsock initialization error)
    // -2: Other errors (address resolution, socket creation, accept failure, etc.)
    // Positive value: valid socket handle (connection accepted)
    
    if (sock != (PCAP_SOCKET)-1 && sock != (PCAP_SOCKET)-2 && sock != (PCAP_SOCKET)-1) {
        // If we got a valid socket (unlikely in fuzzing environment but possible),
        // we should close it and potentially test other remoteact functions
        
        // Test pcap_remoteact_list to see current active connections
        if (pcap_remoteact_list != NULL) {
            char hostlist_buf[RPCAP_HOSTLIST_SIZE];
            char list_errbuf[PCAP_ERRBUF_SIZE];
            int list_result = pcap_remoteact_list(hostlist_buf, ',', RPCAP_HOSTLIST_SIZE, list_errbuf);
        }
        
        // Try to close the connection using pcap_remoteact_close
        if (pcap_remoteact_close != NULL) {
            // We need to provide a host identifier - use the connectinghost if available
            if (connectinghost[0] != '\0') {
                char close_errbuf[PCAP_ERRBUF_SIZE];
                int close_result = pcap_remoteact_close(connectinghost, close_errbuf);
                // Result may be -1 on error, 0 on success
            } else {
                // If we don't have a connecting host, try to close with address if available
                if (address_ptr != NULL) {
                    char close_errbuf[PCAP_ERRBUF_SIZE];
                    int close_result = pcap_remoteact_close(address_ptr, close_errbuf);
                }
            }
        }
    } else {
        // Function failed - check if we have error message in errbuf
        // In fuzzing context, failures are expected and help test error paths
    }
    
    // Test pcap_remoteact_cleanup to clean up any global state
    // This is important because pcap_remoteact_accept may initialize global socket state
    if (pcap_remoteact_cleanup != NULL) {
        pcap_remoteact_cleanup();
    }
    
    // Additional test: Try calling pcap_remoteact_list after cleanup
    // This should work (returning empty list) or fail gracefully
    if (pcap_remoteact_list != NULL) {
        char post_cleanup_list[RPCAP_HOSTLIST_SIZE];
        char post_cleanup_errbuf[PCAP_ERRBUF_SIZE];
        int post_cleanup_result = pcap_remoteact_list(post_cleanup_list, ',', 
                                                      RPCAP_HOSTLIST_SIZE, post_cleanup_errbuf);
    }
    
    // Test edge cases with NULL pointers
    if (remaining_data.size() > 0 && pcap_remoteact_accept != NULL) {
        // Test with NULL address (listen on all interfaces)
        PCAP_SOCKET sock_null_addr = pcap_remoteact_accept(NULL, port_ptr, hostlist_ptr,
                                                          connectinghost, &auth, errbuf);
        if (sock_null_addr != (PCAP_SOCKET)-1 && sock_null_addr != (PCAP_SOCKET)-2 && sock_null_addr != (PCAP_SOCKET)-1) {
            // Close if somehow succeeded
            if (connectinghost[0] != '\0' && pcap_remoteact_close != NULL) {
                char close_err[PCAP_ERRBUF_SIZE];
                pcap_remoteact_close(connectinghost, close_err);
            }
        }
        if (pcap_remoteact_cleanup != NULL) {
            pcap_remoteact_cleanup();
        }
        
        // Test with NULL port (use default port)
        PCAP_SOCKET sock_null_port = pcap_remoteact_accept(address_ptr, NULL, hostlist_ptr,
                                                          connectinghost, &auth, errbuf);
        if (sock_null_port != (PCAP_SOCKET)-1 && sock_null_port != (PCAP_SOCKET)-2 && sock_null_port != (PCAP_SOCKET)-1) {
            if (connectinghost[0] != '\0' && pcap_remoteact_close != NULL) {
                char close_err[PCAP_ERRBUF_SIZE];
                pcap_remoteact_close(connectinghost, close_err);
            }
        }
        if (pcap_remoteact_cleanup != NULL) {
            pcap_remoteact_cleanup();
        }
        
        // Test with NULL hostlist (allow connections from any host)
        PCAP_SOCKET sock_null_hostlist = pcap_remoteact_accept(address_ptr, port_ptr, NULL,
                                                              connectinghost, &auth, errbuf);
        if (sock_null_hostlist != (PCAP_SOCKET)-1 && sock_null_hostlist != (PCAP_SOCKET)-2 && sock_null_hostlist != (PCAP_SOCKET)-1) {
            if (connectinghost[0] != '\0' && pcap_remoteact_close != NULL) {
                char close_err[PCAP_ERRBUF_SIZE];
                pcap_remoteact_close(connectinghost, close_err);
            }
        }
        if (pcap_remoteact_cleanup != NULL) {
            pcap_remoteact_cleanup();
        }
        
        // Test with NULL auth (implies RPCAP_RMTAUTH_NULL)
        PCAP_SOCKET sock_null_auth = pcap_remoteact_accept(address_ptr, port_ptr, hostlist_ptr,
                                                          connectinghost, NULL, errbuf);
        if (sock_null_auth != (PCAP_SOCKET)-1 && sock_null_auth != (PCAP_SOCKET)-2 && sock_null_auth != (PCAP_SOCKET)-1) {
            if (connectinghost[0] != '\0' && pcap_remoteact_close != NULL) {
                char close_err[PCAP_ERRBUF_SIZE];
                pcap_remoteact_close(connectinghost, close_err);
            }
        }
        if (pcap_remoteact_cleanup != NULL) {
            pcap_remoteact_cleanup();
        }
    }
    
    // Test invalid address formats
    if (!address.empty() && remaining_data.size() > 10 && pcap_remoteact_accept != NULL) {
        // Create intentionally malformed address strings
        std::string malformed_addr = "invalid!@#$%^&*()_address";
        PCAP_SOCKET sock_bad_addr = pcap_remoteact_accept(malformed_addr.c_str(), port_ptr,
                                                         hostlist_ptr, connectinghost, 
                                                         &auth, errbuf);
        if (pcap_remoteact_cleanup != NULL) {
            pcap_remoteact_cleanup();
        }
        
        // Test with extremely long address
        std::string long_addr(1024, 'x');  // 1KB address string
        PCAP_SOCKET sock_long_addr = pcap_remoteact_accept(long_addr.c_str(), port_ptr,
                                                          hostlist_ptr, connectinghost,
                                                          &auth, errbuf);
        if (pcap_remoteact_cleanup != NULL) {
            pcap_remoteact_cleanup();
        }
    }
    
    // Test invalid port formats
    if (!port.empty() && remaining_data.size() > 5 && pcap_remoteact_accept != NULL) {
        std::string malformed_port = "99999";  // Port out of valid range
        PCAP_SOCKET sock_bad_port = pcap_remoteact_accept(address_ptr, malformed_port.c_str(),
                                                         hostlist_ptr, connectinghost,
                                                         &auth, errbuf);
        if (pcap_remoteact_cleanup != NULL) {
            pcap_remoteact_cleanup();
        }
        
        std::string non_numeric_port = "abcde";
        PCAP_SOCKET sock_nonnumeric = pcap_remoteact_accept(address_ptr, non_numeric_port.c_str(),
                                                           hostlist_ptr, connectinghost,
                                                           &auth, errbuf);
        if (pcap_remoteact_cleanup != NULL) {
            pcap_remoteact_cleanup();
        }
    }
    
    return 0;
}

} // namespace harness_008
