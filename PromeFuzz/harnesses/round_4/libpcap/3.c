// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Create dummy file if needed for pcap_open_offline (though not in our call sequence)
    // But we might need it for creating a pcap_t handle
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        // Write minimal pcap file header (global header) and some data
        struct pcap_file_header hdr;
        hdr.magic = 0xa1b2c3d4;
        hdr.version_major = 2;
        hdr.version_minor = 4;
        hdr.thiszone = 0;
        hdr.sigfigs = 0;
        hdr.snaplen = 65535;
        hdr.linktype = 1; // DLT_EN10MB
        
        fwrite(&hdr, sizeof(hdr), 1, fp);
        if (Size > 0) {
            fwrite(Data, 1, Size > 100 ? 100 : Size, fp);
        }
        fclose(fp);
    }
    
    // Create pcap_t handle using pcap_open_offline with dummy file
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_offline(dummy_file, errbuf);
    
    // If that fails, create a dead capture handle
    if (pcap == NULL) {
        pcap = pcap_open_dead(DLT_EN10MB, 65535);
    }
    
    // 1. First pcap_statustostr
    if (Size >= sizeof(int)) {
        int status;
        memcpy(&status, Data, sizeof(int));
        const char *status_str = pcap_statustostr(status);
        (void)status_str; // Use result to avoid unused variable warning
    }
    
    // 2. First pcap_geterr
    if (pcap != NULL) {
        char *err = pcap_geterr(pcap);
        (void)err;
    }
    
    // 3. Second pcap_statustostr
    if (Size >= 2 * sizeof(int)) {
        int status2;
        memcpy(&status2, Data + sizeof(int), sizeof(int));
        const char *status_str2 = pcap_statustostr(status2);
        (void)status_str2;
    }
    
    // 4. Second pcap_geterr
    if (pcap != NULL) {
        char *err2 = pcap_geterr(pcap);
        (void)err2;
    }
    
    // 5. pcap_lookupnet
    bpf_u_int32 netp, maskp;
    char lookupnet_errbuf[PCAP_ERRBUF_SIZE];
    
    // Extract device name from fuzz data if possible
    char device[256];
    if (Size > 0) {
        size_t device_len = Size > 255 ? 255 : Size;
        memcpy(device, Data, device_len);
        device[device_len] = '\0';
    } else {
        device[0] = '\0';
    }
    
    int lookup_result = pcap_lookupnet(device, &netp, &maskp, lookupnet_errbuf);
    (void)lookup_result;
    
    // 6. pcap_compile
    struct bpf_program fp_struct;
    memset(&fp_struct, 0, sizeof(fp_struct));
    
    // Extract filter expression from fuzz data
    char filter[256];
    if (Size > 0) {
        size_t filter_len = Size > 255 ? 255 : Size;
        memcpy(filter, Data, filter_len);
        filter[filter_len] = '\0';
    } else {
        filter[0] = '\0';
    }
    
    if (pcap != NULL) {
        int compile_result = pcap_compile(pcap, &fp_struct, filter, 0, netp);
        (void)compile_result;
        
        // 7. Third pcap_geterr
        char *err3 = pcap_geterr(pcap);
        (void)err3;
        
        // 8. pcap_setfilter
        if (compile_result == 0) {
            int setfilter_result = pcap_setfilter(pcap, &fp_struct);
            (void)setfilter_result;
            
            // 9. Fourth pcap_geterr
            char *err4 = pcap_geterr(pcap);
            (void)err4;
        }
    }
    
    // 10. pcap_breakloop
    if (pcap != NULL) {
        pcap_breakloop(pcap);
    }
    
    // 11. pcap_close
    if (pcap != NULL) {
        pcap_close(pcap);
        pcap = NULL; // Set to NULL to avoid double-free
    }
    
    // 12. pcap_freecode
    pcap_freecode(&fp_struct);
    
    // Clean up dummy file
    remove(dummy_file);
    
    return 0;
}