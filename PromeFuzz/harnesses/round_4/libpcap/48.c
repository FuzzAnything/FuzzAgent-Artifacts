// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_filter at bpf_filter.c:534:1 in bpf.h
// pcap_compile_nopcap at gencode.c:1270:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void test_pcap_compile_nopcap(const uint8_t *data, size_t size) {
    struct bpf_program prog;
    memset(&prog, 0, sizeof(prog));
    
    int snaplen = 65535;
    int linktype = DLT_EN10MB;
    int optimize = 1;
    bpf_u_int32 netmask = 0xffffff00;
    
    char *filter_str = NULL;
    if (size > 0) {
        filter_str = (char *)malloc(size + 1);
        if (filter_str) {
            memcpy(filter_str, data, size);
            filter_str[size] = '\0';
            
            pcap_compile_nopcap(snaplen, linktype, &prog, filter_str, optimize, netmask);
            pcap_freecode(&prog);
            
            free(filter_str);
        }
    }
}

static void test_pcap_open_dead_with_tstamp_precision(const uint8_t *data, size_t size) {
    int linktype = DLT_EN10MB;
    int snaplen = 65535;
    u_int precision = PCAP_TSTAMP_PRECISION_MICRO;
    
    if (size > 0) {
        linktype = data[0] % 256;
        if (size > 1) {
            snaplen = (data[1] << 8) | data[0];
            if (size > 2) {
                precision = data[2] % 3;
            }
        }
    }
    
    pcap_t *pcap = pcap_open_dead_with_tstamp_precision(linktype, snaplen, precision);
    if (pcap) {
        pcap_close(pcap);
    }
}

static void test_bpf_validate(const uint8_t *data, size_t size) {
    if (size < sizeof(struct bpf_insn)) {
        return;
    }
    
    int len = size / sizeof(struct bpf_insn);
    if (len > 0) {
        const struct bpf_insn *insns = (const struct bpf_insn *)data;
        bpf_validate(insns, len);
    }
}

static void test_pcap_compile(const uint8_t *data, size_t size) {
    pcap_t *pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (!pcap) {
        return;
    }
    
    struct bpf_program prog;
    memset(&prog, 0, sizeof(prog));
    
    int optimize = 1;
    bpf_u_int32 netmask = 0xffffff00;
    
    char *filter_str = NULL;
    if (size > 0) {
        filter_str = (char *)malloc(size + 1);
        if (filter_str) {
            memcpy(filter_str, data, size);
            filter_str[size] = '\0';
            
            pcap_compile(pcap, &prog, filter_str, optimize, netmask);
            pcap_freecode(&prog);
            
            free(filter_str);
        }
    }
    
    pcap_close(pcap);
}

static void test_bpf_dump(const uint8_t *data, size_t size) {
    struct bpf_program prog;
    memset(&prog, 0, sizeof(prog));
    
    if (size >= sizeof(struct bpf_insn)) {
        int len = size / sizeof(struct bpf_insn);
        if (len > 0) {
            prog.bf_len = len;
            prog.bf_insns = (struct bpf_insn *)malloc(size);
            if (prog.bf_insns) {
                memcpy(prog.bf_insns, data, size);
                
                int option = 1;
                if (size > 0) {
                    option = data[0] % 3;
                }
                bpf_dump(&prog, option);
                
                free(prog.bf_insns);
            }
        }
    }
}

static void test_bpf_filter(const uint8_t *data, size_t size) {
    if (size < sizeof(struct bpf_insn)) {
        return;
    }
    
    int insn_len = size / sizeof(struct bpf_insn);
    if (insn_len > 0) {
        const struct bpf_insn *insns = (const struct bpf_insn *)data;
        
        if (bpf_validate(insns, insn_len) != 0) {
            size_t packet_offset = insn_len * sizeof(struct bpf_insn);
            if (size > packet_offset) {
                const u_char *packet_data = data + packet_offset;
                u_int packet_size = size - packet_offset;
                
                if (packet_size > 0) {
                    u_int wirelen = packet_size;
                    u_int caplen = packet_size;
                    
                    bpf_filter(insns, packet_data, wirelen, caplen);
                }
            }
        }
    }
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    write_dummy_file(Data, Size);
    
    uint8_t test_selector = Data[0] % 6;
    const uint8_t *test_data = Data + 1;
    size_t test_size = Size > 1 ? Size - 1 : 0;
    
    switch (test_selector) {
        case 0:
            test_pcap_compile_nopcap(test_data, test_size);
            break;
        case 1:
            test_pcap_open_dead_with_tstamp_precision(test_data, test_size);
            break;
        case 2:
            test_bpf_validate(test_data, test_size);
            break;
        case 3:
            test_pcap_compile(test_data, test_size);
            break;
        case 4:
            test_bpf_dump(test_data, test_size);
            break;
        case 5:
            test_bpf_filter(test_data, test_size);
            break;
    }
    
    unlink("./dummy_file");
    
    return 0;
}