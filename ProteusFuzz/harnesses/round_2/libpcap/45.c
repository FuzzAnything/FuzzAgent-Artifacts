// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile_nopcap at gencode.c:1271:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_filter at bpf_filter.c:534:1 in bpf.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <bpf.h>

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Determine operation type based on first byte
    int op = Data[0] % 6;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    // Ensure payload is not empty for string operations
    if (payload_size == 0) return 0;

    // Create a null-terminated filter string from payload
    // We limit the size to prevent excessive memory usage
    size_t max_str_len = 1024;
    if (payload_size > max_str_len) payload_size = max_str_len;
    
    char *filter_str = (char *)malloc(payload_size + 1);
    if (!filter_str) return 0;
    memcpy(filter_str, payload, payload_size);
    filter_str[payload_size] = '\0';

    pcap_t *handle = NULL;
    struct bpf_program fp;
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    int linktype = DLT_EN10MB;
    int snaplen = 65535;
    int optimize = (payload_size % 2) ? 1 : 0;
    int dump_opt = (payload_size % 3); // 0, 1, or 2

    // Prepare packet data for filtering functions
    u_char *packet_data = (u_char *)malloc(1500);
    if (!packet_data) {
        free(filter_str);
        return 0;
    }
    memset(packet_data, 0, 1500);
    if (payload_size > 0 && payload_size < 1500) {
        memcpy(packet_data, payload, payload_size);
    }
    
    struct pcap_pkthdr hdr;
    memset(&hdr, 0, sizeof(hdr));
    hdr.caplen = payload_size > 1500 ? 1500 : payload_size;
    hdr.len = hdr.caplen;

    int ret = 0;

    switch (op) {
        case 0: // pcap_compile
            // pcap_compile requires an activated handle
            handle = pcap_open_dead(linktype, snaplen);
            if (handle) {
                ret = pcap_compile(handle, &fp, filter_str, optimize, net);
                if (ret == 0) {
                    pcap_freecode(&fp);
                }
                pcap_close(handle);
            }
            break;

        case 1: // pcap_compile_nopcap
            // This function internally opens and closes a dead handle
            ret = pcap_compile_nopcap(linktype, snaplen, &fp, filter_str, optimize, net);
            if (ret == 0) {
                pcap_freecode(&fp);
            }
            break;

        case 2: // bpf_dump
            // Need a valid program to dump
            handle = pcap_open_dead(linktype, snaplen);
            if (handle) {
                ret = pcap_compile(handle, &fp, filter_str, optimize, net);
                if (ret == 0) {
                    bpf_dump(&fp, dump_opt);
                    pcap_freecode(&fp);
                }
                pcap_close(handle);
            }
            break;

        case 3: // bpf_filter
            // Need a valid program to filter
            handle = pcap_open_dead(linktype, snaplen);
            if (handle) {
                ret = pcap_compile(handle, &fp, filter_str, optimize, net);
                if (ret == 0) {
                    u_int len = bpf_filter(fp.bf_insns, packet_data, hdr.caplen, hdr.len);
                    (void)len; // suppress unused warning
                    pcap_freecode(&fp);
                }
                pcap_close(handle);
            }
            break;

        case 4: // pcap_offline_filter
            // Need a valid program to filter
            handle = pcap_open_dead(linktype, snaplen);
            if (handle) {
                ret = pcap_compile(handle, &fp, filter_str, optimize, net);
                if (ret == 0) {
                    int match = pcap_offline_filter(&fp, &hdr, packet_data);
                    (void)match;
                    pcap_freecode(&fp);
                }
                pcap_close(handle);
            }
            break;

        case 5: // bpf_validate
            // Need a valid program to validate
            handle = pcap_open_dead(linktype, snaplen);
            if (handle) {
                ret = pcap_compile(handle, &fp, filter_str, optimize, net);
                if (ret == 0) {
                    int valid = bpf_validate(fp.bf_insns, fp.bf_len);
                    (void)valid;
                    pcap_freecode(&fp);
                }
                pcap_close(handle);
            }
            break;
    }

    free(filter_str);
    free(packet_data);
    
    return 0;
}