// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_compile_nopcap at gencode.c:1271:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <bpf.h>
#include <time.h>

static void create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    size_t offset = 0;
    int linktype = (Data[offset++] & 0x03) + 1; 
    int snaplen = (Data[offset++] & 0xFF) + 64;
    int timeout = (Data[offset++] & 0xFF) + 100;
    int promisc = Data[offset++] & 1;
    int immediate = Data[offset++] & 1;
    int nonblock = Data[offset++] & 1;
    int optimize = Data[offset++] & 1;
    int dump_opt = Data[offset++] & 3; 
    
    // Ensure we have enough data for netmask (4 bytes) before reading
    if (offset + 4 > Size) {
        return 0;
    }
    
    int netmask = (Data[offset++] << 24) | (Data[offset++] << 16) | (Data[offset++] << 8) | Data[offset++];

    // Create a dummy file for offline testing if we have enough data
    size_t file_data_size = Size - offset;
    
    // Only create file if we have data to write
    if (file_data_size > 0) {
        create_dummy_file("./dummy_file", Data + offset, file_data_size);
    }

    pcap_t *handle = NULL;
    struct bpf_program fp = {0};
    int ret = 0;
    char errbuf[PCAP_ERRBUF_SIZE]; // Allocate buffer for error messages

    // Helper to safely extract a null-terminated string from fuzz data
    static char safe_filter[256];
    size_t str_len = 0;
    if (file_data_size > 0) {
        // Find the first null byte or limit to buffer size
        const uint8_t *ptr = Data + offset;
        while (str_len < file_data_size && str_len < 255 && ptr[str_len] != 0) {
            str_len++;
        }
        memcpy(safe_filter, ptr, str_len);
        safe_filter[str_len] = '\0';
    } else {
        safe_filter[0] = '\0';
    }

    // Strategy 1: Test pcap_compile_nopcap (Deprecated but valid for fuzzing)
    if (str_len > 0) {
        ret = pcap_compile_nopcap(linktype, snaplen, &fp, safe_filter, optimize, netmask);
        if (ret == 0 && fp.bf_insns != NULL) {
            bpf_dump(&fp, dump_opt);
            bpf_validate(fp.bf_insns, fp.bf_len);

            struct pcap_pkthdr hdr;
            memset(&hdr, 0, sizeof(hdr));
            hdr.ts.tv_sec = time(NULL);
            hdr.ts.tv_usec = 0;
            hdr.caplen = file_data_size;
            hdr.len = hdr.caplen;
            
            const u_char *pkt_data = (const u_char *)(Data + offset);
            pcap_offline_filter(&fp, &hdr, pkt_data);

            pcap_freecode(&fp);
        }
    }

    // Strategy 2: Test pcap_compile with pcap_open_dead
    handle = pcap_open_dead(linktype, snaplen);
    if (handle != NULL) {
        if (str_len > 0) {
            ret = pcap_compile(handle, &fp, safe_filter, optimize, netmask);
            if (ret == 0 && fp.bf_insns != NULL) {
                bpf_dump(&fp, dump_opt);
                bpf_validate(fp.bf_insns, fp.bf_len);
                pcap_setfilter(handle, &fp);
                pcap_freecode(&fp);
            }
        }
        pcap_close(handle);
    }

    // Strategy 3: Test with offline file (if file exists and has data)
    // Only attempt to open the file if we actually wrote data to it
    if (file_data_size > 0) {
        handle = pcap_open_offline("./dummy_file", errbuf); // Pass valid errbuf
        if (handle != NULL) {
            if (str_len > 0) {
                ret = pcap_compile(handle, &fp, safe_filter, optimize, netmask);
                if (ret == 0 && fp.bf_insns != NULL) {
                    bpf_dump(&fp, dump_opt);
                    bpf_validate(fp.bf_insns, fp.bf_len);
                    pcap_setfilter(handle, &fp);
                    pcap_freecode(&fp);
                }
            }
            pcap_close(handle);
        }
    }

    // Strategy 4: Direct bpf_validate with crafted instructions
    // Each instruction is 8 bytes (2+1+1+4). We need at least 8 bytes for one instruction.
    if (Size > offset + 8) {
        struct bpf_insn *custom_insns = malloc(sizeof(struct bpf_insn) * 10);
        if (custom_insns) {
            int len = 0;
            // Craft some instructions from data
            // We need 8 bytes per instruction
            size_t remaining = Size - offset;
            int max_insns = (remaining / 8) > 10 ? 10 : (remaining / 8);
            
            for (int i = 0; i < max_insns; i++) {
                size_t idx = offset + (i * 8);
                custom_insns[i].code = Data[idx];
                custom_insns[i].jt = Data[idx + 1];
                custom_insns[i].jf = Data[idx + 2];
                custom_insns[i].k = ((bpf_u_int32)Data[idx + 3] << 24) | 
                                    ((bpf_u_int32)Data[idx + 4] << 16) | 
                                    ((bpf_u_int32)Data[idx + 5] << 8) | 
                                    ((bpf_u_int32)Data[idx + 6]);
                len++;
            }
            bpf_validate(custom_insns, len);
            free(custom_insns);
        }
    }

    return 0;
}