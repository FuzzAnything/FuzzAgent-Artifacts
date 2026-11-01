// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(void) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        /* Write minimal pcap global header (24 bytes) */
        uint32_t magic = 0xa1b2c3d4;
        uint16_t version_major = 2;
        uint16_t version_minor = 4;
        int32_t thiszone = 0;
        uint32_t sigfigs = 0;
        uint32_t snaplen = 65535;
        uint32_t network = 1; /* DLT_EN10MB */
        
        fwrite(&magic, sizeof(magic), 1, fp);
        fwrite(&version_major, sizeof(version_major), 1, fp);
        fwrite(&version_minor, sizeof(version_minor), 1, fp);
        fwrite(&thiszone, sizeof(thiszone), 1, fp);
        fwrite(&sigfigs, sizeof(sigfigs), 1, fp);
        fwrite(&snaplen, sizeof(snaplen), 1, fp);
        fwrite(&network, sizeof(network), 1, fp);
        
        /* Write one minimal packet header (16 bytes) + empty data */
        struct pcap_pkthdr pkt_header;
        pkt_header.ts.tv_sec = 0;
        pkt_header.ts.tv_usec = 0;
        pkt_header.caplen = 0;
        pkt_header.len = 0;
        
        fwrite(&pkt_header, sizeof(pkt_header), 1, fp);
        
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    struct bpf_program fp;
    int ret;

    /* 1. pcap_strerror: Test with various error codes derived from input */
    if (Size > 0) {
        int err_code = Data[0];
        const char *str = pcap_strerror(err_code);
        (void)str; /* Suppress unused warning */
    }

    /* Prepare dummy file for pcap_open_offline */
    write_dummy_file();

    /* 2. pcap_open_offline: Try to open the dummy file */
    handle = pcap_open_offline("./dummy_file", errbuf);

    /* 3. pcap_datalink_name_to_val: Test with input string as datalink name */
    if (Size > 0) {
        /* Ensure null termination */
        char name_buf[256];
        size_t len = (Size < 255) ? Size : 255;
        memcpy(name_buf, Data, len);
        name_buf[len] = '\0';
        
        int dlt_val = pcap_datalink_name_to_val(name_buf);
        (void)dlt_val; /* Suppress unused warning */
    }

    /* 4. pcap_open_dead: Create a dead handle for compilation tests */
    /* Use input data to determine linktype and snaplen if available */
    int linktype = DLT_EN10MB;
    int snaplen = 65535;
    
    if (Size >= 2) {
        linktype = (int)Data[0];
        snaplen = (int)Data[1] * 100;
    }
    
    pcap_t *dead_handle = pcap_open_dead(linktype, snaplen);
    if (dead_handle == NULL) {
        /* If dead handle fails, try with default EN10MB */
        dead_handle = pcap_open_dead(DLT_EN10MB, 65535);
    }

    /* 5. pcap_compile: Compile a filter string from input */
    if (dead_handle != NULL) {
        memset(&fp, 0, sizeof(fp));
        
        /* Create a null-terminated filter string from input */
        char filter_buf[256];
        size_t len = (Size < 255) ? Size : 255;
        memcpy(filter_buf, Data, len);
        filter_buf[len] = '\0';
        
        /* Compile with optimize flag based on input */
        int optimize = (Size > 1 && Data[1] % 2 == 0) ? 1 : 0;
        bpf_u_int32 net = 0;
        bpf_u_int32 mask = 0;
        
        if (Size >= 4) {
            net = Data[2];
            mask = Data[3];
        }
        
        ret = pcap_compile(dead_handle, &fp, filter_buf, optimize, net);
        
        /* Cleanup bpf_program if compiled successfully */
        if (ret == 0) {
            pcap_freecode(&fp);
        }
        
        pcap_close(dead_handle);
    }

    /* Cleanup offline handle */
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}