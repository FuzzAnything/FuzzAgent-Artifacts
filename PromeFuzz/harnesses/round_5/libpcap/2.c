// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file for pcap_open_offline
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }

    // 1. pcap_strerror
    const char *err_str = pcap_strerror(0);
    if (Size > 0) {
        // Use first byte as error number
        err_str = pcap_strerror((int)Data[0]);
    }

    // 2. pcap_open_offline
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_offline = pcap_open_offline("./dummy_file", errbuf);
    
    // 3. pcap_datalink_name_to_val
    int dlt_val = -1;
    if (Size > 0) {
        // Create null-terminated string from fuzzer data for link type name
        size_t name_len = Size > 100 ? 100 : Size;
        char *dlt_name = (char *)malloc(name_len + 1);
        if (dlt_name) {
            memcpy(dlt_name, Data, name_len);
            dlt_name[name_len] = '\0';
            dlt_val = pcap_datalink_name_to_val(dlt_name);
            free(dlt_name);
        }
    }

    // 4. pcap_open_dead
    int linktype = DLT_EN10MB; // Default Ethernet
    int snaplen = 65535; // Default maximum
    if (Size > 1) {
        linktype = (int)Data[0] % 256;
        snaplen = (int)Data[1] % 2048;
    }
    pcap_t *pcap_dead = pcap_open_dead(linktype, snaplen);

    // 5. pcap_compile
    if (pcap_dead) {
        struct bpf_program fp;
        memset(&fp, 0, sizeof(fp));
        
        // Create filter expression from fuzzer data
        const char *filter_str = "ip";
        if (Size > 0) {
            size_t filter_len = Size > 200 ? 200 : Size;
            char *filter = (char *)malloc(filter_len + 1);
            if (filter) {
                memcpy(filter, Data, filter_len);
                filter[filter_len] = '\0';
                filter_str = filter;
                
                int optimize = 1;
                bpf_u_int32 netmask = 0xffffff00;
                
                if (Size > 2) {
                    optimize = (int)Data[2] & 1;
                    netmask = (bpf_u_int32)Data[0] << 24 | 
                             (bpf_u_int32)Data[1] << 16 | 
                             (bpf_u_int32)Data[2] << 8;
                }
                
                int compile_result = pcap_compile(pcap_dead, &fp, filter_str, optimize, netmask);
                (void)compile_result; // Result intentionally ignored for fuzzing
                
                pcap_freecode(&fp);
                free(filter);
            }
        }
    }

    // Cleanup
    if (pcap_offline) {
        pcap_close(pcap_offline);
    }
    if (pcap_dead) {
        pcap_close(pcap_dead);
    }
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}