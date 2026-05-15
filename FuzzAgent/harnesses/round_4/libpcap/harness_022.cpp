#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <pcap/pcap.h>
#include <pcap/bpf.h>

namespace harness_022 {


// Forward declaration for internal lexer functions if needed
extern "C" {
    // These are internal functions from scanner/parser that we want to exercise
    // The actual signatures may vary, but we'll call them through pcap_compile APIs
}

// Helper to generate diverse filter expressions targeting pcap_parse/pcap_lex coverage
static std::string generate_filter_expression(FuzzedDataProvider& fdp, uint8_t complexity);

// Helper to generate MAC addresses in various formats from scanner.l patterns
static std::string generate_mac_address(FuzzedDataProvider& fdp);

// Helper to generate IPv6 addresses in various formats from scanner.l patterns
static std::string generate_ipv6_address(FuzzedDataProvider& fdp);

// Helper to generate diverse filter expressions targeting pcap_parse/pcap_lex coverage
static std::string generate_filter_expression(FuzzedDataProvider& fdp, uint8_t complexity) {
    std::string filter;
    
    // Base protocol keywords from scanner.l
    static const std::array<const char*, 30> protocols = {
        "ip", "tcp", "udp", "icmp", "igmp", "arp", "rarp", "sctp", 
        "ip6", "icmp6", "ah", "esp", "ether", "ppp", "slip", "fddi",
        "tr", "wlan", "atalk", "aarp", "decnet", "lat", "sca", "moprc",
        "mopdl", "iso", "esis", "es-is", "isis", "is-is"
    };
    
    // Address format components
    static const std::array<const char*, 4> addr_types = {
        "host", "net", "port", "portrange"
    };
    
    // Direction keywords
    static const std::array<const char*, 2> directions = {
        "src", "dst"
    };
    
    // Logical operators
    static const std::array<const char*, 3> operators = {
        "and", "or", "not"
    };
    
    // BPF primitive keywords
    static const std::array<const char*, 8> bpf_primitives = {
        "greater", "less", "geq", "leq", "=", "!=", ">", "<"
    };
    
    switch (complexity % 6) {
        case 0: {
            // Simple single protocol filter
            filter = std::string(protocols[fdp.ConsumeIntegralInRange<size_t>(0, protocols.size() - 1)]);
            break;
        }
        
        case 1: {
            // Protocol with direction and host
            std::string proto = std::string(protocols[fdp.ConsumeIntegralInRange<size_t>(0, protocols.size() - 1)]);
            std::string dir = std::string(directions[fdp.ConsumeIntegralInRange<size_t>(0, directions.size() - 1)]);
            
            // Generate IP address
            if (fdp.ConsumeBool()) {
                // IPv4 address
                uint32_t a = fdp.ConsumeIntegralInRange<uint8_t>(0, 255);
                uint32_t b = fdp.ConsumeIntegralInRange<uint8_t>(0, 255);
                uint32_t c = fdp.ConsumeIntegralInRange<uint8_t>(0, 255);
                uint32_t d = fdp.ConsumeIntegralInRange<uint8_t>(0, 255);
                filter = proto + " " + dir + " host " + std::to_string(a) + "." + 
                         std::to_string(b) + "." + std::to_string(c) + "." + std::to_string(d);
            } else {
                // Hostname or simple identifier
                filter = proto + " " + dir + " host example" + std::to_string(fdp.ConsumeIntegral<uint8_t>());
            }
            break;
        }
        
        case 2: {
            // Complex expression with AND/OR and parentheses
            std::string expr1, expr2;
            
            // First sub-expression
            if (fdp.ConsumeBool()) {
                expr1 = std::string(protocols[fdp.ConsumeIntegralInRange<size_t>(0, protocols.size() - 1)]) + 
                       " port " + std::to_string(fdp.ConsumeIntegralInRange<uint16_t>(1, 65535));
            } else {
                expr1 = std::string(directions[fdp.ConsumeIntegralInRange<size_t>(0, directions.size() - 1)]) +
                       " net " + std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(0, 255)) + ".0.0.0/8";
            }
            
            // Second sub-expression  
            if (fdp.ConsumeBool()) {
                expr2 = std::string(protocols[fdp.ConsumeIntegralInRange<size_t>(0, protocols.size() - 1)]) + 
                       " " + std::string(bpf_primitives[fdp.ConsumeIntegralInRange<size_t>(0, bpf_primitives.size() - 1)]) +
                       " " + std::to_string(fdp.ConsumeIntegralInRange<uint16_t>(0, 1500));
            } else {
                expr2 = "ether " + std::string(directions[fdp.ConsumeIntegralInRange<size_t>(0, directions.size() - 1)]) +
                       " " + generate_mac_address(fdp);
            }
            
            // Combine with logical operator
            std::string op = std::string(operators[fdp.ConsumeIntegralInRange<size_t>(0, operators.size() - 2)]); // Skip 'not'
            
            if (fdp.ConsumeBool()) {
                filter = "(" + expr1 + " " + op + " " + expr2 + ")";
            } else {
                filter = expr1 + " " + op + " " + expr2;
            }
            break;
        }
        
        case 3: {
            // BPF-style expression with primitives
            filter = "proto " + std::string(protocols[fdp.ConsumeIntegralInRange<size_t>(0, protocols.size() - 1)]) +
                    " and " + std::string(bpf_primitives[fdp.ConsumeIntegralInRange<size_t>(0, bpf_primitives.size() - 1)]) +
                    " " + std::to_string(fdp.ConsumeIntegralInRange<uint16_t>(0, 65535));
            break;
        }
        
        case 4: {
            // MAC address filter (tests MAC lexer patterns)
            filter = "ether " + std::string(directions[fdp.ConsumeIntegralInRange<size_t>(0, directions.size() - 1)]) +
                    " " + generate_mac_address(fdp);
            break;
        }
        
        case 5: {
            // IPv6 filter (tests IPv6 lexer patterns)
            filter = "ip6 " + std::string(directions[fdp.ConsumeIntegralInRange<size_t>(0, directions.size() - 1)]) +
                    " host " + generate_ipv6_address(fdp);
            break;
        }
    }
    
    // Sometimes add negation
    if (fdp.ConsumeBool() && complexity > 1) {
        filter = "not " + filter;
    }
    
    return filter;
}

// Helper to generate MAC addresses in various formats from scanner.l patterns
static std::string generate_mac_address(FuzzedDataProvider& fdp) {
    uint8_t format = fdp.ConsumeIntegral<uint8_t>() % 3;
    std::string mac;
    
    // Generate 6 random bytes
    uint8_t bytes[6];
    for (int i = 0; i < 6; i++) {
        bytes[i] = fdp.ConsumeIntegral<uint8_t>();
    }
    
    char buf[32];
    switch (format) {
        case 0:
            // Colon-separated: xx:xx:xx:xx:xx:xx
            snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
                    bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5]);
            break;
        case 1:
            // Dash-separated: xx-xx-xx-xx-xx-xx
            snprintf(buf, sizeof(buf), "%02x-%02x-%02x-%02x-%02x-%02x",
                    bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5]);
            break;
        case 2:
            // Dot-separated: xxxx.xxxx.xxxx (last 3 bytes as 2-byte groups)
            snprintf(buf, sizeof(buf), "%02x%02x.%02x%02x.%02x%02x",
                    bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5]);
            break;
    }
    
    return std::string(buf);
}

// Helper to generate IPv6 addresses in various formats from scanner.l patterns
static std::string generate_ipv6_address(FuzzedDataProvider& fdp) {
    uint8_t format = fdp.ConsumeIntegral<uint8_t>() % 4;
    std::string ipv6;
    
    // Generate 8 random 16-bit segments
    uint16_t segments[8];
    for (int i = 0; i < 8; i++) {
        segments[i] = fdp.ConsumeIntegral<uint16_t>();
    }
    
    char buf[64];
    switch (format) {
        case 0:
            // Full IPv6: xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx
            snprintf(buf, sizeof(buf), "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
                    segments[0], segments[1], segments[2], segments[3],
                    segments[4], segments[5], segments[6], segments[7]);
            break;
        case 1:
            // IPv6 with double colon compression (varying position)
            {
                int compress_pos = fdp.ConsumeIntegralInRange<int>(1, 6);
                if (compress_pos == 1) {
                    snprintf(buf, sizeof(buf), "::%04x:%04x:%04x:%04x:%04x:%04x:%04x",
                            segments[1], segments[2], segments[3], segments[4],
                            segments[5], segments[6], segments[7]);
                } else if (compress_pos == 7) {
                    snprintf(buf, sizeof(buf), "%04x:%04x:%04x:%04x:%04x:%04x:%04x::",
                            segments[0], segments[1], segments[2], segments[3],
                            segments[4], segments[5], segments[6]);
                } else {
                    snprintf(buf, sizeof(buf), "%04x:%04x:%04x::%04x:%04x:%04x:%04x",
                            segments[0], segments[1], segments[2],
                            segments[5], segments[6], segments[7], fdp.ConsumeIntegral<uint16_t>());
                }
            }
            break;
        case 2:
            // IPv4-mapped IPv6: ::ffff:192.168.1.1
            {
                uint8_t a = fdp.ConsumeIntegral<uint8_t>();
                uint8_t b = fdp.ConsumeIntegral<uint8_t>();
                uint8_t c = fdp.ConsumeIntegral<uint8_t>();
                uint8_t d = fdp.ConsumeIntegral<uint8_t>();
                snprintf(buf, sizeof(buf), "::ffff:%u.%u.%u.%u", a, b, c, d);
            }
            break;
        case 3:
            // Mixed with IPv4: xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:192.168.1.1
            {
                uint8_t a = fdp.ConsumeIntegral<uint8_t>();
                uint8_t b = fdp.ConsumeIntegral<uint8_t>();
                uint8_t c = fdp.ConsumeIntegral<uint8_t>();
                uint8_t d = fdp.ConsumeIntegral<uint8_t>();
                snprintf(buf, sizeof(buf), "%04x:%04x:%04x:%04x:%04x:%04x:%u.%u.%u.%u",
                        segments[0], segments[1], segments[2], segments[3],
                        segments[4], segments[5], a, b, c, d);
            }
            break;
    }
    
    return std::string(buf);
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Need sufficient input for comprehensive lexer/parser testing
    // Minimum: scenario + complexity + linktype + snaplen + optimize + netmask
    if (Size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for different test scenarios targeting lexer/parser coverage
    // First byte: test scenario selector (0-7) - different API combinations
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Second byte: filter complexity level (0-7) - affects generated filter structure
    uint8_t complexity = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Third byte: link type
    int linktype = fdp.ConsumeIntegral<uint8_t>();
    if (linktype < 0) linktype = DLT_EN10MB; // Default to Ethernet
    
    // Fourth and fifth bytes: snapshot length
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535); // Minimum Ethernet frame to max
    
    // Sixth byte: optimize flag
    int optimize = fdp.ConsumeBool() ? 1 : 0;
    
    // Seventh to tenth bytes: netmask
    bpf_u_int32 netmask = fdp.ConsumeIntegral<bpf_u_int32>();
    
    // Eleventh byte: number of filter attempts (1-10)
    uint8_t num_filters = fdp.ConsumeIntegralInRange<uint8_t>(1, 10);
    
    // Remaining bytes: additional parameters and filter variations
    std::vector<std::string> filter_expressions;
    for (uint8_t i = 0; i < num_filters && fdp.remaining_bytes() > 1; i++) {
        filter_expressions.push_back(generate_filter_expression(fdp, complexity + i));
    }
    
    // If no filters generated, use a minimal default
    if (filter_expressions.empty()) {
        filter_expressions.push_back("ip");
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    struct bpf_program bpf;
    memset(&bpf, 0, sizeof(bpf));
    
    // Results tracking
    int total_compilations = 0;
    int successful_compilations = 0;
    
    switch (scenario) {
        case 0:
        case 1:
        case 2:
        case 3: {
            // Scenarios 0-3: Test pcap_compile_nopcap with various filters
            // This directly exercises the parser/lexer without pcap handle overhead
            
            for (const auto& filter_str : filter_expressions) {
                total_compilations++;
                
                // Try compilation with current parameters
                int result = pcap_compile_nopcap(snaplen, linktype, &bpf, 
                                                filter_str.c_str(), optimize, netmask);
                
                if (result == 0) {
                    successful_compilations++;
                    
                    // Clean up for next iteration
                    pcap_freecode(&bpf);
                    memset(&bpf, 0, sizeof(bpf));
                }
                
                // Also try with opposite optimization setting
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
                    total_compilations++;
                    int result2 = pcap_compile_nopcap(snaplen, linktype, &bpf,
                                                     filter_str.c_str(), 
                                                     optimize ? 0 : 1, netmask);
                    if (result2 == 0) {
                        successful_compilations++;
                        pcap_freecode(&bpf);
                        memset(&bpf, 0, sizeof(bpf));
                    }
                }
            }
            break;
        }
        
        case 4:
        case 5:
        case 6:
        case 7: {
            // Scenarios 4-7: Test pcap_compile with pcap_open_dead context
            // Tests the same lexer/parser but with full pcap context
            
            pcap_t *pcap_handle = pcap_open_dead(linktype, snaplen);
            if (pcap_handle == nullptr) {
                // Can't proceed without pcap handle
                return 0;
            }
            
            for (const auto& filter_str : filter_expressions) {
                total_compilations++;
                
                // Try compilation with pcap_compile
                int result = pcap_compile(pcap_handle, &bpf, 
                                         filter_str.c_str(), optimize, netmask);
                
                if (result == 0) {
                    successful_compilations++;
                    
                    // Additional validation if compilation succeeded
                    if (bpf.bf_len > 0 && bpf.bf_insns != nullptr) {
                        // Could call bpf_validate or other BPF functions here
                        // but just having the compiled program exercises the lexer/parser
                    }
                    
                    pcap_freecode(&bpf);
                    memset(&bpf, 0, sizeof(bpf));
                }
                
                // Try different netmask if available
                if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 4) {
                    total_compilations++;
                    bpf_u_int32 alt_netmask = fdp.ConsumeIntegral<bpf_u_int32>();
                    int result2 = pcap_compile(pcap_handle, &bpf,
                                              filter_str.c_str(), optimize, alt_netmask);
                    if (result2 == 0) {
                        successful_compilations++;
                        pcap_freecode(&bpf);
                        memset(&bpf, 0, sizeof(bpf));
                    }
                }
            }
            
            pcap_close(pcap_handle);
            break;
        }
    }
    
    // Final cleanup if any program still allocated
    if (bpf.bf_insns != nullptr) {
        pcap_freecode(&bpf);
    }
    
    // Return value doesn't matter for coverage, but we've exercised the lexer/parser
    return 0;
}

} // namespace harness_022
