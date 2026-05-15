#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>

namespace harness_027 {
/*
 * Harness targeting comprehensive BPF filter expression testing with focus on:
 * - pcap_parse (internal parser with 503 blocked branches - largest blocker)
 * - pcap_compile (BPF filter compilation with pcap context)
 * - pcap_compile_nopcap (BPF filter compilation without pcap context)
 * 
 * Strategy: Implement exhaustive BPF filter expression testing targeting all
 * aspects of filter parsing, compilation, and error handling to reach blocked
 * branches in the parsing subsystem.
 * 
 * Semantic Differentiation from existing harnesses (000, 001, 002, 017):
 * 1. Focus on pcap_parse internal function which handles the actual parsing logic
 * 2. Comprehensive testing of BPF filter expression syntax with all operators
 * 3. Systematic exploration of edge cases in filter grammar and tokenization
 * 4. Deep testing of error recovery and validation logic in the parser
 * 5. Coverage of all BPF filter expression types and protocol qualifiers
 * 
 * Coverage Goal: Target the 503 blocked branches in pcap_parse by generating
 * diverse BPF filter expressions that exercise all parsing code paths.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for comprehensive BPF filter testing
    // Need space for filter strings, parameters, and control flags
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Phase 1: Consume control parameters for test scenario selection
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 7);
    int focus_error_paths = fdp.ConsumeBool() ? 1 : 0;
    int optimize = fdp.ConsumeBool() ? 1 : 0;
    
    // Phase 2: Consume BPF filter expression components
    // Generate complex filter expressions by combining different components
    size_t expr_parts = fdp.ConsumeIntegralInRange<size_t>(1, 8);
    std::vector<std::string> filter_components;
    
    for (size_t i = 0; i < expr_parts && fdp.remaining_bytes() > 0; i++) {
        size_t part_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::string part = fdp.ConsumeBytesAsString(part_len);
        filter_components.push_back(part);
    }
    
    // Phase 3: Consume filter expression construction parameters
    int expression_type = fdp.ConsumeIntegralInRange<int>(0, 10);
    int use_parentheses = fdp.ConsumeBool() ? 1 : 0;
    int use_logical_ops = fdp.ConsumeBool() ? 1 : 0;
    int use_arithmetic_ops = fdp.ConsumeBool() ? 1 : 0;
    
    // Phase 4: Consume pcap context parameters
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535);
    bpf_u_int32 netmask = fdp.ConsumeIntegral<bpf_u_int32>();
    
    // Phase 5: Build comprehensive BPF filter expression
    std::string filter_expr;
    
    if (focus_error_paths) {
        // Test error paths: malformed expressions, edge cases
        switch (expression_type % 5) {
            case 0:
                filter_expr = "";  // Empty filter
                break;
            case 1:
                filter_expr = "(";  // Unmatched parentheses
                break;
            case 2:
                filter_expr = "and";  // Logical operator without operands
                break;
            case 3:
                filter_expr = "ip[1000]";  // Out of bounds offset
                break;
            case 4:
                // Very long expression that might overflow buffers
                filter_expr = std::string(4096, 'a');
                break;
        }
    } else {
        // Build valid (or potentially valid) filter expressions
        if (!filter_components.empty()) {
            if (use_parentheses) {
                filter_expr = "(";
                for (size_t i = 0; i < filter_components.size(); i++) {
                    if (i > 0 && use_logical_ops) {
                        if ((i % 2) == 0) {
                            filter_expr += " and ";
                        } else {
                            filter_expr += " or ";
                        }
                    }
                    filter_expr += filter_components[i];
                }
                filter_expr += ")";
            } else {
                for (size_t i = 0; i < filter_components.size(); i++) {
                    if (i > 0 && use_logical_ops) {
                        filter_expr += " and ";
                    }
                    filter_expr += filter_components[i];
                }
            }
            
            // Add protocol qualifiers based on expression type
            switch (expression_type) {
                case 0:
                    filter_expr = "ip " + filter_expr;
                    break;
                case 1:
                    filter_expr = "tcp " + filter_expr;
                    break;
                case 2:
                    filter_expr = "udp " + filter_expr;
                    break;
                case 3:
                    filter_expr = "icmp " + filter_expr;
                    break;
                case 4:
                    filter_expr = "arp " + filter_expr;
                    break;
                case 5:
                    filter_expr = "vlan " + filter_expr;
                    break;
                case 6:
                    filter_expr = "mpls " + filter_expr;
                    break;
                case 7:
                    filter_expr = "ip6 " + filter_expr;
                    break;
                case 8:
                    filter_expr = "ether " + filter_expr;
                    break;
                case 9:
                    filter_expr = "port " + filter_expr;
                    break;
                case 10:
                    filter_expr = "host " + filter_expr;
                    break;
            }
            
            if (use_arithmetic_ops) {
                filter_expr += " > 0";
            }
        }
    }
    
    // If no expression was built, use a simple default
    if (filter_expr.empty() && !filter_components.empty()) {
        filter_expr = filter_components[0];
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    int result;
    struct bpf_program bpf;
    
    // Phase 6: Execute comprehensive test scenarios
    switch (test_scenario) {
        case 0: {
            // Scenario 1: Standard pcap_compile with pcap_open_dead
            pcap_t* pcap = pcap_open_dead(linktype, snaplen);
            if (pcap == nullptr) {
                return 0;
            }
            
            result = pcap_compile(pcap, &bpf, filter_expr.c_str(), optimize, netmask);
            if (result == 0) {
                // Try to set the filter
                pcap_setfilter(pcap, &bpf);
                
                // Test filter program inspection
                if (bpf.bf_len > 0 && bpf.bf_insns != nullptr) {
                    // Exercise bpf_dump with different formats
                    bpf_dump(&bpf, 0);  // Human readable
                    bpf_dump(&bpf, 1);  // C array format
                    bpf_dump(&bpf, 2);  // Raw instructions
                    
                    // Test bpf_image on some instructions
                    for (int i = 0; i < (bpf.bf_len < 3 ? bpf.bf_len : 3); i++) {
                        const char* instr_str = bpf_image(&bpf.bf_insns[i], i);
                        (void)instr_str;  // Use to avoid unused warning
                    }
                }
                
                pcap_freecode(&bpf);
            } else {
                // Compilation failed - check error buffer
                (void)pcap_geterr(pcap);
            }
            
            pcap_close(pcap);
            break;
        }
        
        case 1: {
            // Scenario 2: pcap_compile_nopcap (no pcap handle)
            result = pcap_compile_nopcap(snaplen, linktype, &bpf, 
                                        filter_expr.c_str(), optimize, netmask);
            if (result == 0) {
                // Successfully compiled
                bpf_dump(&bpf, 0);
                pcap_freecode(&bpf);
            }
            break;
        }
        
        case 2: {
            // Scenario 3: Test with pcap_create and pcap_activate
            pcap_t* pcap = pcap_create("any", errbuf);
            if (pcap != nullptr) {
                pcap_set_snaplen(pcap, snaplen);
                pcap_set_promisc(pcap, 0);
                pcap_set_timeout(pcap, 1000);
                
                if (pcap_activate(pcap) == 0) {
                    result = pcap_compile(pcap, &bpf, filter_expr.c_str(), optimize, netmask);
                    if (result == 0) {
                        pcap_setfilter(pcap, &bpf);
                        pcap_freecode(&bpf);
                    }
                }
                pcap_close(pcap);
            }
            break;
        }
        
        case 3: {
            // Scenario 4: Multiple compilation attempts with variations
            pcap_t* pcap = pcap_open_dead(linktype, snaplen);
            if (pcap != nullptr) {
                // Try different optimization settings
                for (int opt = 0; opt <= 1; opt++) {
                    struct bpf_program bpf2;
                    result = pcap_compile(pcap, &bpf2, filter_expr.c_str(), opt, netmask);
                    if (result == 0) {
                        pcap_freecode(&bpf2);
                    }
                }
                pcap_close(pcap);
            }
            break;
        }
        
        case 4: {
            // Scenario 5: Test with different link types
            int test_linktypes[] = {DLT_EN10MB, DLT_RAW, DLT_NULL, DLT_LOOP, DLT_LINUX_SLL};
            for (int i = 0; i < 5 && fdp.remaining_bytes() > 0; i++) {
                pcap_t* pcap = pcap_open_dead(test_linktypes[i], snaplen);
                if (pcap != nullptr) {
                    result = pcap_compile(pcap, &bpf, filter_expr.c_str(), optimize, netmask);
                    if (result == 0) {
                        pcap_freecode(&bpf);
                    }
                    pcap_close(pcap);
                }
            }
            break;
        }
        
        case 5: {
            // Scenario 6: Test filter expression parsing edge cases
            // Generate multiple filter variations from remaining input
            if (fdp.remaining_bytes() > 16) {
                size_t num_variations = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                pcap_t* pcap = pcap_open_dead(linktype, snaplen);
                if (pcap != nullptr) {
                    for (size_t v = 0; v < num_variations && fdp.remaining_bytes() > 8; v++) {
                        size_t var_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                        std::string var_expr = fdp.ConsumeBytesAsString(var_len);
                        
                        struct bpf_program bpf_var;
                        result = pcap_compile(pcap, &bpf_var, var_expr.c_str(), optimize, netmask);
                        if (result == 0) {
                            pcap_freecode(&bpf_var);
                        }
                    }
                    pcap_close(pcap);
                }
            }
            break;
        }
        
        case 6: {
            // Scenario 7: Test with special netmask values
            bpf_u_int32 test_netmasks[] = {0, 0xFFFFFFFF, 0xFFFFFF00, 0xFFFF0000, 0xFF000000};
            pcap_t* pcap = pcap_open_dead(linktype, snaplen);
            if (pcap != nullptr) {
                for (int i = 0; i < 5; i++) {
                    result = pcap_compile(pcap, &bpf, filter_expr.c_str(), optimize, test_netmasks[i]);
                    if (result == 0) {
                        pcap_freecode(&bpf);
                    }
                }
                pcap_close(pcap);
            }
            break;
        }
        
        case 7: {
            // Scenario 8: Combined testing with remaining input as additional expressions
            pcap_t* pcap = pcap_open_dead(linktype, snaplen);
            if (pcap != nullptr) {
                // Test the main filter expression
                result = pcap_compile(pcap, &bpf, filter_expr.c_str(), optimize, netmask);
                if (result == 0) {
                    pcap_freecode(&bpf);
                }
                
                // Test additional expressions from remaining input
                while (fdp.remaining_bytes() > 8) {
                    size_t extra_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                    std::string extra_expr = fdp.ConsumeBytesAsString(extra_len);
                    
                    struct bpf_program bpf_extra;
                    int extra_result = pcap_compile(pcap, &bpf_extra, extra_expr.c_str(), optimize, netmask);
                    if (extra_result == 0) {
                        pcap_freecode(&bpf_extra);
                    }
                }
                
                pcap_close(pcap);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_027
