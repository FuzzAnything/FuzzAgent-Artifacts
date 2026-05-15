// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
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
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(struct bpf_insn)) {
        return 0;
    }

    // Create dummy pcap file
    write_dummy_file(Data, Size);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = pcap_open_offline("./dummy_file", errbuf);
    
    if (pcap_handle == NULL) {
        // Create a dead pcap handle if file open fails
        pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
        if (pcap_handle == NULL) {
            cleanup_dummy_file();
            return 0;
        }
    }
    
    // 1. First pcap_geterr call
    char *error_msg = pcap_geterr(pcap_handle);
    (void)error_msg;
    
    // Prepare filter string from fuzz data (null-terminated)
    char *filter_str = (char *)malloc(Size + 1);
    if (!filter_str) {
        pcap_close(pcap_handle);
        cleanup_dummy_file();
        return 0;
    }
    memcpy(filter_str, Data, Size);
    filter_str[Size] = '\0';
    
    // Compile filter using pcap_compile instead of manually constructing BPF program
    struct bpf_program prog;
    memset(&prog, 0, sizeof(prog));
    
    int compile_result = pcap_compile(pcap_handle, &prog, filter_str, 1, PCAP_NETMASK_UNKNOWN);
    
    // 2. pcap_next_ex
    struct pcap_pkthdr *pkthdr = NULL;
    const u_char *pkt_data = NULL;
    int ret = pcap_next_ex(pcap_handle, &pkthdr, &pkt_data);
    (void)ret;
    
    // 3. Second pcap_geterr call
    error_msg = pcap_geterr(pcap_handle);
    (void)error_msg;
    
    // 4. pcap_offline_filter - only if compilation succeeded
    if (compile_result == 0) {
        if (pkthdr != NULL && pkt_data != NULL) {
            // Ensure caplen doesn't exceed available data
            struct pcap_pkthdr safe_hdr = *pkthdr;
            if (safe_hdr.caplen > Size) {
                safe_hdr.caplen = (bpf_u_int32)Size;
            }
            if (safe_hdr.len > Size) {
                safe_hdr.len = (bpf_u_int32)Size;
            }
            pcap_offline_filter(&prog, &safe_hdr, pkt_data);
        } else {
            // Create safe dummy packet
            struct pcap_pkthdr dummy_hdr;
            dummy_hdr.ts.tv_sec = 0;
            dummy_hdr.ts.tv_usec = 0;
            dummy_hdr.caplen = Size > 65535 ? 65535 : (bpf_u_int32)Size;
            dummy_hdr.len = (bpf_u_int32)Size;
            
            const u_char *dummy_data = Data;
            pcap_offline_filter(&prog, &dummy_hdr, dummy_data);
        }
        
        // Free compiled program
        pcap_freecode(&prog);
    }
    
    // Cleanup
    free(filter_str);
    pcap_close(pcap_handle);
    cleanup_dummy_file();
    
    return 0;
}