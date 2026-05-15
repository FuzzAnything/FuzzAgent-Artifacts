// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_close at pcap.c:4177:1 in pcap.h
// bpf_filter at bpf_filter.c:534:1 in bpf.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile_nopcap at gencode.c:1270:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/bpf.h"
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int parse_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) return 0;
    int value = *(int*)(data + *offset);
    *offset += sizeof(int);
    return value;
}

static bpf_u_int32 parse_bpf_u_int32(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(bpf_u_int32) > size) return 0;
    bpf_u_int32 value = *(bpf_u_int32*)(data + *offset);
    *offset += sizeof(bpf_u_int32);
    return value;
}

static u_int parse_u_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(u_int) > size) return 0;
    u_int value = *(u_int*)(data + *offset);
    *offset += sizeof(u_int);
    return value;
}

static char* parse_string(const uint8_t *data, size_t size, size_t *offset, size_t max_len) {
    if (*offset >= size) return NULL;
    
    size_t remaining = size - *offset;
    size_t str_len = remaining < max_len ? remaining : max_len;
    
    char* str = malloc(str_len + 1);
    if (!str) return NULL;
    
    memcpy(str, data + *offset, str_len);
    str[str_len] = '\0';
    *offset += str_len;
    
    return str;
}

static struct bpf_insn* parse_bpf_insns(const uint8_t *data, size_t size, size_t *offset, int *len) {
    if (*offset + sizeof(int) > size) return NULL;
    
    *len = parse_int(data, size, offset);
    if (*len <= 0 || *len > 100) return NULL;
    
    size_t insns_size = *len * sizeof(struct bpf_insn);
    if (*offset + insns_size > size) return NULL;
    
    struct bpf_insn* insns = malloc(insns_size);
    if (!insns) return NULL;
    
    memcpy(insns, data + *offset, insns_size);
    *offset += insns_size;
    
    return insns;
}

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    size_t offset = 0;
    int selector = Data[0] % 6;
    offset++;
    
    switch (selector) {
        case 0: { // pcap_compile_nopcap
            int snaplen = parse_int(Data, Size, &offset);
            int linktype = parse_int(Data, Size, &offset);
            int optimize = parse_int(Data, Size, &offset);
            bpf_u_int32 netmask = parse_bpf_u_int32(Data, Size, &offset);
            
            char *filter_str = parse_string(Data, Size, &offset, 256);
            if (!filter_str) break;
            
            struct bpf_program program;
            program.bf_len = 0;
            program.bf_insns = NULL;
            
            pcap_compile_nopcap(snaplen, linktype, &program, filter_str, optimize, netmask);
            
            if (program.bf_insns) {
                free(program.bf_insns);
            }
            free(filter_str);
            break;
        }
        
        case 1: { // bpf_validate
            int len;
            struct bpf_insn *insns = parse_bpf_insns(Data, Size, &offset, &len);
            if (insns) {
                bpf_validate(insns, len);
                free(insns);
            }
            break;
        }
        
        case 2: { // pcap_compile
            int linktype = parse_int(Data, Size, &offset);
            int snaplen = parse_int(Data, Size, &offset);
            u_int precision = parse_u_int(Data, Size, &offset);
            
            pcap_t *pcap = pcap_open_dead_with_tstamp_precision(linktype, snaplen, precision);
            if (!pcap) break;
            
            int optimize = parse_int(Data, Size, &offset);
            bpf_u_int32 netmask = parse_bpf_u_int32(Data, Size, &offset);
            
            char *filter_str = parse_string(Data, Size, &offset, 256);
            if (!filter_str) {
                pcap_close(pcap);
                break;
            }
            
            struct bpf_program program;
            program.bf_len = 0;
            program.bf_insns = NULL;
            
            pcap_compile(pcap, &program, filter_str, optimize, netmask);
            
            if (program.bf_insns) {
                free(program.bf_insns);
            }
            free(filter_str);
            pcap_close(pcap);
            break;
        }
        
        case 3: { // bpf_dump
            int linktype = parse_int(Data, Size, &offset);
            int snaplen = parse_int(Data, Size, &offset);
            u_int precision = parse_u_int(Data, Size, &offset);
            
            pcap_t *pcap = pcap_open_dead_with_tstamp_precision(linktype, snaplen, precision);
            if (!pcap) break;
            
            int optimize = parse_int(Data, Size, &offset);
            bpf_u_int32 netmask = parse_bpf_u_int32(Data, Size, &offset);
            
            char *filter_str = parse_string(Data, Size, &offset, 256);
            if (!filter_str) {
                pcap_close(pcap);
                break;
            }
            
            struct bpf_program program;
            program.bf_len = 0;
            program.bf_insns = NULL;
            
            if (pcap_compile(pcap, &program, filter_str, optimize, netmask) == 0) {
                bpf_dump(&program, 1);
                bpf_dump(&program, 0);
                
                if (program.bf_insns) {
                    free(program.bf_insns);
                }
            }
            
            free(filter_str);
            pcap_close(pcap);
            break;
        }
        
        case 4: { // bpf_filter
            int len;
            struct bpf_insn *insns = parse_bpf_insns(Data, Size, &offset, &len);
            if (!insns) break;
            
            u_int packet_len = parse_u_int(Data, Size, &offset);
            if (packet_len > 65535) packet_len = 65535;
            
            u_int wire_len = parse_u_int(Data, Size, &offset);
            if (wire_len > 65535) wire_len = 65535;
            
            size_t remaining = Size - offset;
            size_t data_size = remaining < packet_len ? remaining : packet_len;
            
            u_char *packet_data = malloc(data_size);
            if (packet_data) {
                if (data_size > 0) {
                    memcpy(packet_data, Data + offset, data_size);
                }
                bpf_filter(insns, packet_data, packet_len, wire_len);
                free(packet_data);
            }
            
            free(insns);
            break;
        }
        
        case 5: { // pcap_open_dead_with_tstamp_precision
            int linktype = parse_int(Data, Size, &offset);
            int snaplen = parse_int(Data, Size, &offset);
            u_int precision = parse_u_int(Data, Size, &offset);
            
            pcap_t *pcap = pcap_open_dead_with_tstamp_precision(linktype, snaplen, precision);
            if (pcap) {
                pcap_close(pcap);
            }
            break;
        }
    }
    
    // Write dummy file for any file-based operations
    write_dummy_file(Data, Size);
    
    return 0;
}