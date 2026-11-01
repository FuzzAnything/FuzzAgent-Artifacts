// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_image at bpf_image.c:130:1 in bpf.h
// pcap_compile_nopcap at gencode.c:1271:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// bpf_filter at bpf_filter.c:534:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <bpf.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

static void cleanup_bpf_program(struct bpf_program *prog) {
    if (prog && prog->bf_insns) {
        free(prog->bf_insns);
        prog->bf_insns = NULL;
        prog->bf_len = 0;
    }
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    struct bpf_program prog = {0};
    struct bpf_insn *insns = NULL;
    int num_insns;
    int ret;
    char *img;
    int dump_opt;
    struct pcap_pkthdr hdr;
    u_char *pkt_data = NULL;
    u_int pkt_len;
    u_int cap_len;

    // Parse input to determine number of instructions
    // First 4 bytes: number of instructions (little endian)
    if (Size >= 4) {
        num_insns = *(const uint32_t *)(Data);
        // Limit to prevent excessive allocation
        if (num_insns > 1000) {
            num_insns = 1000;
        }
    } else {
        num_insns = 0;
    }

    // Allocate instruction array
    if (num_insns > 0) {
        insns = (struct bpf_insn *)malloc(num_insns * sizeof(struct bpf_insn));
        if (!insns) {
            return 0;
        }

        // Fill instructions from input data
        size_t offset = 4;
        size_t remaining = Size - offset;
        size_t needed = num_insns * sizeof(struct bpf_insn);
        
        if (remaining < needed) {
            needed = remaining;
        }

        for (int i = 0; i < num_insns && offset + sizeof(struct bpf_insn) <= Size; i++) {
            // Use input data to populate instruction fields
            // Ensure we don't read out of bounds
            if (offset + sizeof(u_short) <= Size) {
                insns[i].code = *(const uint16_t *)(Data + offset);
                offset += sizeof(u_short);
            } else {
                insns[i].code = 0;
            }
            
            if (offset < Size) {
                insns[i].jt = Data[offset];
                offset++;
            } else {
                insns[i].jt = 0;
            }

            if (offset < Size) {
                insns[i].jf = Data[offset];
                offset++;
            } else {
                insns[i].jf = 0;
            }

            if (offset + sizeof(bpf_u_int32) <= Size) {
                insns[i].k = *(const bpf_u_int32 *)(Data + offset);
                offset += sizeof(bpf_u_int32);
            } else {
                insns[i].k = 0;
            }
        }
        
        prog.bf_insns = insns;
        prog.bf_len = num_insns;
    }

    // 1. Test bpf_validate
    if (insns && num_insns > 0) {
        ret = bpf_validate(insns, num_insns);
        (void)ret; // Suppress unused variable warning
    }

    // 2. Test bpf_image
    if (insns && num_insns > 0) {
        img = bpf_image(insns, num_insns);
        if (img) {
            // Just use the result to ensure it's valid
            (void)strlen(img);
        }
    }

    // 3. Test pcap_compile_nopcap (deprecated but still functional)
    // We need to create a filter string from input data
    if (Size > 10) {
        size_t filter_len = Size - 10;
        if (filter_len > 1000) filter_len = 1000;
        
        char *filter_str = (char *)malloc(filter_len + 1);
        if (filter_str) {
            memcpy(filter_str, Data + 10, filter_len);
            filter_str[filter_len] = '\0';
            
            // Try to compile the filter
            ret = pcap_compile_nopcap(0, 0, &prog, filter_str, 1, 0);
            if (ret == 0 && prog.bf_insns) {
                // Test with the compiled program
                // Create dummy packet data for testing
                if (Size > 20) {
                    pkt_len = *(const uint32_t *)(Data + 10 + filter_len);
                    if (pkt_len > 65535) pkt_len = 65535;
                    if (pkt_len > Size - 10 - filter_len - 4) {
                        pkt_len = Size - 10 - filter_len - 4;
                    }
                    
                    if (pkt_len > 0) {
                        pkt_data = (u_char *)malloc(pkt_len);
                        if (pkt_data) {
                            memcpy(pkt_data, Data + 10 + filter_len + 4, pkt_len);
                            
                            // Setup packet header
                            hdr.caplen = pkt_len;
                            hdr.len = pkt_len;
                            hdr.ts.tv_sec = 0;
                            hdr.ts.tv_usec = 0;
                            
                            // 4. Test pcap_offline_filter
                            ret = pcap_offline_filter(&prog, &hdr, pkt_data);
                            (void)ret;
                            
                            // 5. Test bpf_filter (deprecated)
                            u_int filtered_len = bpf_filter(prog.bf_insns, pkt_data, pkt_len, pkt_len);
                            (void)filtered_len;
                        }
                    }
                }
            }
            free(filter_str);
        }
    }

    // 6. Test bpf_dump with different options
    if (prog.bf_insns && prog.bf_len > 0) {
        dump_opt = (Size > 2) ? (int)Data[0] : 0;
        // Limit dump_opt to reasonable range
        if (dump_opt > 5) dump_opt = 5;
        if (dump_opt < 0) dump_opt = 0;
        
        bpf_dump(&prog, dump_opt);
    }

    // Cleanup
    cleanup_bpf_program(&prog);
    if (pkt_data) {
        free(pkt_data);
    }

    return 0;
}