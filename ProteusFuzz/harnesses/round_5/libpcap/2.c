// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void write_dummy_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // Write a minimal pcap header (24 bytes)
    uint32_t magic = 0xa1b2c3d4;
    fwrite(&magic, sizeof(uint32_t), 1, f);
    uint16_t ver_major = 2;
    uint16_t ver_minor = 4;
    fwrite(&ver_major, sizeof(uint16_t), 1, f);
    fwrite(&ver_minor, sizeof(uint16_t), 1, f);
    int32_t tz = 0;
    uint32_t sigfigs = 0;
    uint32_t snaplen = 65535;
    uint32_t net = 1; // DLT_EN10MB
    fwrite(&tz, sizeof(int32_t), 1, f);
    fwrite(&sigfigs, sizeof(uint32_t), 1, f);
    fwrite(&snaplen, sizeof(uint32_t), 1, f);
    fwrite(&net, sizeof(uint32_t), 1, f);
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // 1. pcap_strerror
    int err_code = (int)Data[0];
    const char *err_str = pcap_strerror(err_code);
    (void)err_str;

    // Prepare dummy file for pcap_open_offline
    const char *dummy_file = "./dummy_file";
    write_dummy_file(dummy_file);

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);

    // 2. pcap_open_offline
    pcap_t *handle_offline = pcap_open_offline(dummy_file, errbuf);
    if (handle_offline) {
        pcap_close(handle_offline);
    }

    // 3. pcap_datalink_name_to_val
    char dlt_name[256];
    size_t name_len = Size < 255 ? Size : 255;
    memcpy(dlt_name, Data, name_len);
    dlt_name[name_len] = '\0';
    
    if (name_len == 0 || (Data[0] < 32 && Data[0] != 10 && Data[0] != 13)) {
        int val1 = pcap_datalink_name_to_val("ETHERNET");
        int val2 = pcap_datalink_name_to_val("EN10MB");
        int val3 = pcap_datalink_name_to_val("invalid_name_xyz");
        (void)val1; (void)val2; (void)val3;
    } else {
        int val = pcap_datalink_name_to_val(dlt_name);
        (void)val;
    }

    // 4. pcap_open_dead
    int linktype = (int)Data[0] % 100;
    int snaplen = 65535; // Default safe value
    
    // Only access Data[1] if Size is sufficient
    if (Size > 1) {
        snaplen = (int)Data[1] % 10000;
        if (snaplen == 0) snaplen = 65535; // Avoid 0 snaplen if possible
    }
    
    pcap_t *handle_dead = pcap_open_dead(linktype, snaplen);
    if (handle_dead) {
        // 5. pcap_compile
        struct bpf_program fp;
        memset(&fp, 0, sizeof(fp));
        
        char filter[128];
        size_t filter_len = Size < 127 ? Size : 127;
        memcpy(filter, Data, filter_len);
        filter[filter_len] = '\0';
        
        if (filter_len == 0 || (filter[0] < 32 && filter[0] != 10 && filter[0] != 13)) {
            strcpy(filter, "host 127.0.0.1");
        }

        bpf_u_int32 net = 0;
        bpf_u_int32 mask = 0;
        int optimize = 0;
        
        // Only access Data[2] if Size is sufficient
        if (Size > 2) {
            optimize = (Data[2] % 2);
        }
        
        int compile_result = pcap_compile(handle_dead, &fp, filter, optimize, net);
        
        if (compile_result == 0) {
            pcap_freecode(&fp);
        }
        
        pcap_close(handle_dead);
    }

    return 0;
}