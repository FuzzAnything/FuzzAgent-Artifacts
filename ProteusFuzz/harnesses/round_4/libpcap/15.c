// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 net, mask;
    pcap_t *handle1 = NULL;
    pcap_t *handle2 = NULL;
    
    // Prepare a safe device name string from fuzz input
    char safe_device[256];
    size_t len = (Size < 255) ? Size : 255;
    memcpy(safe_device, Data, len);
    safe_device[len] = '\0';

    // Step 1: Prepare for pcap_close. We need a handle to close.
    // Try to open "any" first.
    handle1 = pcap_open_live("any", 65535, 1, 1000, errbuf);
    
    // If "any" fails, try to create and open a dummy pcap file
    if (handle1 == NULL) {
        FILE *f = fopen("./dummy_file", "wb");
        if (f) {
            // Write minimal pcap header
            uint32_t magic = 0xa1b2c3d4;
            uint16_t version_major = 2;
            uint16_t version_minor = 4;
            int32_t thiszone = 0;
            uint32_t sigfigs = 0;
            uint32_t snaplen = 65535;
            uint32_t network = 1; // Ethernet
            
            fwrite(&magic, sizeof(magic), 1, f);
            fwrite(&version_major, sizeof(version_major), 1, f);
            fwrite(&version_minor, sizeof(version_minor), 1, f);
            fwrite(&thiszone, sizeof(thiszone), 1, f);
            fwrite(&sigfigs, sizeof(sigfigs), 1, f);
            fwrite(&snaplen, sizeof(snaplen), 1, f);
            fwrite(&network, sizeof(network), 1, f);
            fclose(f);
            
            handle1 = pcap_open_offline("./dummy_file", errbuf);
        }
    }

    // Invoke pcap_close (Target 1)
    if (handle1 != NULL) {
        pcap_close(handle1);
        handle1 = NULL;
    }

    // Invoke pcap_lookupnet (Target 2)
    int lookup_res = pcap_lookupnet(safe_device, &net, &mask, errbuf);
    (void)lookup_res;

    // Invoke pcap_datalink_name_to_val (Target 3)
    int dlt_val = pcap_datalink_name_to_val(safe_device);
    (void)dlt_val;

    // Invoke pcap_set_datalink (Target 4)
    // Requires a valid handle. Open a new one.
    handle2 = pcap_open_live("any", 65535, 1, 1000, errbuf);
    if (handle2 == NULL) {
        handle2 = pcap_open_offline("./dummy_file", errbuf);
    }
    
    if (handle2 != NULL) {
        int dlt = DLT_EN10MB;
        if (Size >= 4) {
            dlt = *(int*)Data;
        }
        int set_res = pcap_set_datalink(handle2, dlt);
        (void)set_res;
    }

    // Invoke pcap_geterr (Target 5)
    if (handle2 != NULL) {
        char *err = pcap_geterr(handle2);
        (void)err;
    }

    // Cleanup
    if (handle2 != NULL) {
        pcap_close(handle2);
    }

    return 0;
}