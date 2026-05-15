// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_filter at bpf_filter.c:534:1 in bpf.h
// pcap_compile_nopcap at gencode.c:1270:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include "pcap/bpf.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void init_bpf_program(struct bpf_program *program) {
    program->bf_len = 0;
    program->bf_insns = NULL;
}

static int extract_int(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) return 0;
    int value = *(const int *)*data;
    *data += sizeof(int);
    *size -= sizeof(int);
    return value;
}

static bpf_u_int32 extract_u32(const uint8_t **data, size_t *size) {
    if (*size < sizeof(bpf_u_int32)) return 0;
    bpf_u_int32 value = *(const bpf_u_int32 *)*data;
    *data += sizeof(bpf_u_int32);
    *size -= sizeof(bpf_u_int32);
    return value;
}

static const char *extract_string(const uint8_t **data, size_t *size, size_t max_len) {
    if (*size == 0) return "";
    
    size_t len = *size;
    if (len > max_len) len = max_len;
    
    const char *str = (const char *)*data;
    *data += len;
    *size -= len;
    
    static char buffer[4096];
    size_t copy_len = len < sizeof(buffer) - 1 ? len : sizeof(buffer) - 1;
    memcpy(buffer, str, copy_len);
    buffer[copy_len] = '\0';
    
    return buffer;
}

static struct bpf_insn *extract_bpf_insns(const uint8_t **data, size_t *size, int *len) {
    if (*size < sizeof(int)) {
        *len = 0;
        return NULL;
    }
    
    *len = extract_int(data, size);
    if (*len <= 0 || *len > 1024) {
        *len = 0;
        return NULL;
    }
    
    size_t insns_size = *len * sizeof(struct bpf_insn);
    if (*size < insns_size) {
        *len = 0;
        return NULL;
    }
    
    struct bpf_insn *insns = (struct bpf_insn *)*data;
    *data += insns_size;
    *size -= insns_size;
    
    return insns;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    const uint8_t *cursor = Data;
    size_t remaining = Size;
    
    int function_selector = extract_int(&cursor, &remaining) % 6;
    
    switch (function_selector) {
        case 0: {  // pcap_compile_nopcap
            int snaplen = extract_int(&cursor, &remaining);
            int linktype = extract_int(&cursor, &remaining);
            struct bpf_program program;
            init_bpf_program(&program);
            
            const char *filter_str = extract_string(&cursor, &remaining, 256);
            int optimize = extract_int(&cursor, &remaining);
            bpf_u_int32 netmask = extract_u32(&cursor, &remaining);
            
            pcap_compile_nopcap(snaplen, linktype, &program, filter_str, optimize, netmask);
            pcap_freecode(&program);
            break;
        }
        
        case 1: {  // pcap_open_dead_with_tstamp_precision
            int linktype = extract_int(&cursor, &remaining);
            int snaplen = extract_int(&cursor, &remaining);
            u_int precision = extract_int(&cursor, &remaining);
            
            pcap_t *pcap = pcap_open_dead_with_tstamp_precision(linktype, snaplen, precision);
            if (pcap) {
                pcap_close(pcap);
            }
            break;
        }
        
        case 2: {  // bpf_validate
            int len;
            const struct bpf_insn *insns = extract_bpf_insns(&cursor, &remaining, &len);
            if (insns && len > 0) {
                bpf_validate(insns, len);
            }
            break;
        }
        
        case 3: {  // pcap_compile
            int linktype = extract_int(&cursor, &remaining);
            int snaplen = extract_int(&cursor, &remaining);
            u_int precision = extract_int(&cursor, &remaining);
            
            pcap_t *pcap = pcap_open_dead_with_tstamp_precision(linktype, snaplen, precision);
            if (!pcap) {
                pcap = pcap_open_dead(linktype, snaplen);
            }
            
            if (pcap) {
                struct bpf_program program;
                init_bpf_program(&program);
                
                const char *filter_str = extract_string(&cursor, &remaining, 256);
                int optimize = extract_int(&cursor, &remaining);
                bpf_u_int32 netmask = extract_u32(&cursor, &remaining);
                
                pcap_compile(pcap, &program, filter_str, optimize, netmask);
                pcap_freecode(&program);
                pcap_close(pcap);
            }
            break;
        }
        
        case 4: {  // bpf_dump
            int linktype = extract_int(&cursor, &remaining);
            int snaplen = extract_int(&cursor, &remaining);
            
            pcap_t *pcap = pcap_open_dead(linktype, snaplen);
            if (pcap) {
                struct bpf_program program;
                init_bpf_program(&program);
                
                const char *filter_str = extract_string(&cursor, &remaining, 256);
                int optimize = extract_int(&cursor, &remaining);
                bpf_u_int32 netmask = extract_u32(&cursor, &remaining);
                
                if (pcap_compile(pcap, &program, filter_str, optimize, netmask) == 0) {
                    int option = extract_int(&cursor, &remaining);
                    bpf_dump(&program, option);
                }
                
                pcap_freecode(&program);
                pcap_close(pcap);
            }
            break;
        }
        
        case 5: {  // bpf_filter
            int len;
            const struct bpf_insn *insns = extract_bpf_insns(&cursor, &remaining, &len);
            if (insns && len > 0 && bpf_validate(insns, len) != 0) {
                size_t packet_size = remaining > 65536 ? 65536 : remaining;
                if (packet_size > 0) {
                    const u_char *packet = cursor;
                    u_int buflen = extract_u32(&cursor, &remaining);
                    u_int caplen = extract_u32(&cursor, &remaining);
                    
                    if (buflen > packet_size) buflen = packet_size;
                    if (caplen > packet_size) caplen = packet_size;
                    if (caplen > buflen) caplen = buflen;
                    
                    bpf_filter(insns, packet, buflen, caplen);
                }
            }
            break;
        }
    }
    
    return 0;
}