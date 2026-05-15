#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <pcap/socket.h>  // For PCAP_SOCKET type
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

namespace harness_010 {

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
    // Minimum size check: need at least operation mode (1 byte), 
    // address length (1 byte), port length (1 byte), hostlist length (1 byte),
    // auth type (1 byte), SSL flag (1 byte), pcap_init flags (4 bytes) = 10 bytes
    if (size < 10) {
        return 0;
    }
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation mode: 
    // 0 = full lifecycle test (accept -> list -> close)
    // 1 = accept only (various parameter combinations)
    // 2 = list and close operations only
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume pcap_init flags
    unsigned int pcap_init_flags = fdp.ConsumeIntegral<unsigned int>();
    
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
    
    // Consume username and password for auth (if needed)
    size_t username_len = fdp.ConsumeIntegralInRange<size_t>(0, 64);
    std::string username = fdp.ConsumeBytesAsString(username_len);
    
    size_t password_len = fdp.ConsumeIntegralInRange<size_t>(0, 64);
    std::string password = fdp.ConsumeBytesAsString(password_len);
    
    // Consume SSL flag (only for pcap_remoteact_accept_ex)
    uint8_t use_ssl = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Consume host for close operation
    size_t close_host_len = fdp.ConsumeIntegralInRange<size_t>(0, 128);
    std::string close_host = fdp.ConsumeBytesAsString(close_host_len);
    
    // Consume separator for list operation
    char list_separator = fdp.ConsumeIntegral<char>();
    
    // Remaining bytes are for additional test data
    std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_rmtauth auth;
    char connectinghost[RPCAP_HOSTLIST_SIZE] = {0};  // Buffer for connecting host info
    
    // Step 1: Initialize pcap library
    int init_result = pcap_init(pcap_init_flags, errbuf);
    // Note: pcap_init may fail if already initialized or invalid flags, continue anyway
    
    // Step 2: Setup authentication structure
    if (auth_type == 1 && !username.empty() && !password.empty()) {
        auth.type = RPCAP_RMTAUTH_PWD;
        // Use dynamically allocated strings that match consumed data
        auth.username = const_cast<char*>(username.c_str());
        auth.password = const_cast<char*>(password.c_str());
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
    const char* close_host_ptr = close_host.empty() ? NULL : close_host.c_str();
    
    PCAP_SOCKET sock = (PCAP_SOCKET)-1;
    
    // Test pcap_createsrcstr and pcap_parsesrcstr to understand source string format
    char source_str[PCAP_BUF_SIZE] = {0};
    char parsed_host[PCAP_BUF_SIZE] = {0};
    char parsed_port[PCAP_BUF_SIZE] = {0};
    char parsed_name[PCAP_BUF_SIZE] = {0};
    int src_type = 0;
    
    if (!address.empty() && !port.empty()) {
        // Test creating a source string
        int create_result = pcap_createsrcstr(source_str, PCAP_SRC_IFREMOTE,
                                             address_ptr, port_ptr, "eth0", errbuf);
        
        if (create_result == 0) {
            // Test parsing the source string back
            int parse_result = pcap_parsesrcstr(source_str, &src_type, 
                                               parsed_host, parsed_port, 
                                               parsed_name, errbuf);
        }
    }
    
    if (operation_mode == 0 || operation_mode == 1) {
        // Test pcap_remoteact_accept_ex (with SSL option)
        sock = pcap_remoteact_accept_ex(address_ptr, port_ptr, hostlist_ptr,
                                       connectinghost, &auth, use_ssl, errbuf);
        
        // The function returns different error values:
        // -1: sock_init() failed (Winsock initialization error)
        // -2: Other errors (address resolution, socket creation, accept failure, etc.)
        // Positive value: valid socket handle (connection accepted)
        
        if (sock != (PCAP_SOCKET)-1 && sock != (PCAP_SOCKET)-2) {
            // If we got a valid socket (unlikely in fuzzing environment but possible),
            // we should test other remoteact functions
            
            if (operation_mode == 0) {
                // Test pcap_remoteact_list to see current active connections
                char hostlist_buf[RPCAP_HOSTLIST_SIZE];
                char list_errbuf[PCAP_ERRBUF_SIZE];
                int list_result = pcap_remoteact_list(hostlist_buf, list_separator, 
                                                     RPCAP_HOSTLIST_SIZE, list_errbuf);
                
                // Test with different separator
                if (remaining_data.size() > 0) {
                    char alt_separator = remaining_data[0] % 128;
                    if (alt_separator == 0) alt_separator = ',';
                    int list_result2 = pcap_remoteact_list(hostlist_buf, alt_separator,
                                                          RPCAP_HOSTLIST_SIZE, list_errbuf);
                }
                
                // Try to close the connection using pcap_remoteact_close
                if (connectinghost[0] != '\0') {
                    char close_errbuf[PCAP_ERRBUF_SIZE];
                    int close_result = pcap_remoteact_close(connectinghost, close_errbuf);
                } else if (close_host_ptr != NULL) {
                    char close_errbuf[PCAP_ERRBUF_SIZE];
                    int close_result = pcap_remoteact_close(close_host_ptr, close_errbuf);
                }
            }
        } else {
            // Function failed - check if we have error message in errbuf
            // In fuzzing context, failures are expected and help test error paths
        }
    }
    
    if (operation_mode == 0 || operation_mode == 2) {
        // Test pcap_remoteact_list independently
        char hostlist_buf[RPCAP_HOSTLIST_SIZE];
        char list_errbuf[PCAP_ERRBUF_SIZE];
        
        // Test with various buffer sizes
        int buffer_size = fdp.ConsumeIntegralInRange<int>(1, RPCAP_HOSTLIST_SIZE);
        if (buffer_size <= 0) buffer_size = RPCAP_HOSTLIST_SIZE;
        
        int list_result = pcap_remoteact_list(hostlist_buf, list_separator, 
                                             buffer_size, list_errbuf);
        
        // Test pcap_remoteact_close with various host strings
        if (close_host_ptr != NULL) {
            char close_errbuf[PCAP_ERRBUF_SIZE];
            int close_result = pcap_remoteact_close(close_host_ptr, close_errbuf);
        }
        
        // Test with NULL host (should fail)
        if (remaining_data.size() > 0) {
            char close_errbuf[PCAP_ERRBUF_SIZE];
            int close_result = pcap_remoteact_close(NULL, close_errbuf);
        }
    }
    
    // Test pcap_remoteact_cleanup to clean up any global state
    // This is important because pcap_remoteact_accept may initialize global socket state
    pcap_remoteact_cleanup();
    
    // Additional parameter variation tests
    if (remaining_data.size() > 0) {
        FuzzedDataProvider var_fdp(remaining_data.data(), remaining_data.size());
        
        // Test with empty strings (different from NULL)
        if (var_fdp.ConsumeBool()) {
            std::string empty_addr = "";
            std::string empty_port = "";
            std::string empty_hostlist = "";
            
            PCAP_SOCKET sock_empty = pcap_remoteact_accept_ex(
                empty_addr.c_str(), empty_port.c_str(), empty_hostlist.c_str(),
                connectinghost, &auth, use_ssl, errbuf);
            
            if (sock_empty != (PCAP_SOCKET)-1 && sock_empty != (PCAP_SOCKET)-2) {
                pcap_remoteact_cleanup();
            }
        }
        
        // Test with IPv6 address format if available
        if (var_fdp.ConsumeBool() && var_fdp.remaining_bytes() > 10) {
            std::string ipv6_addr = "::1";
            std::string ipv6_port = var_fdp.ConsumeBytesAsString(5);
            
            PCAP_SOCKET sock_ipv6 = pcap_remoteact_accept_ex(
                ipv6_addr.c_str(), ipv6_port.c_str(), hostlist_ptr,
                connectinghost, &auth, use_ssl, errbuf);
            
            if (sock_ipv6 != (PCAP_SOCKET)-1 && sock_ipv6 != (PCAP_SOCKET)-2) {
                pcap_remoteact_cleanup();
            }
        }
        
        // Test with wildcard hostlist
        if (var_fdp.ConsumeBool()) {
            std::string wildcard_hostlist = "*";
            
            PCAP_SOCKET sock_wildcard = pcap_remoteact_accept_ex(
                address_ptr, port_ptr, wildcard_hostlist.c_str(),
                connectinghost, &auth, use_ssl, errbuf);
            
            if (sock_wildcard != (PCAP_SOCKET)-1 && sock_wildcard != (PCAP_SOCKET)-2) {
                pcap_remoteact_cleanup();
            }
        }
        
        // Test with multiple hosts in hostlist
        if (var_fdp.ConsumeBool() && var_fdp.remaining_bytes() > 20) {
            std::string multi_hostlist = "192.168.1.1,10.0.0.1,localhost";
            
            PCAP_SOCKET sock_multi = pcap_remoteact_accept_ex(
                address_ptr, port_ptr, multi_hostlist.c_str(),
                connectinghost, &auth, use_ssl, errbuf);
            
            if (sock_multi != (PCAP_SOCKET)-1 && sock_multi != (PCAP_SOCKET)-2) {
                pcap_remoteact_cleanup();
            }
        }
    }
    
    // Final cleanup
    pcap_remoteact_cleanup();
    
    return 0;
}

} // namespace harness_010
