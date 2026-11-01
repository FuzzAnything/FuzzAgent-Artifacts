// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 net, mask;
    pcap_t *handle = NULL;
    int dlt_val;
    const char *device = "any";

    // Step 1: Prepare environment
    // Initialize error buffer
    memset(errbuf, 0, sizeof(errbuf));

    // Attempt to open a handle to test pcap_close and pcap_geterr
    // We try to open the dummy file as a savefile
    if (Size > 0) {
        write_dummy_file(Data, Size);
        handle = pcap_open_offline("./dummy_file", errbuf);
    }

    // Step 2: Invoke pcap_close (Target 1)
    // Even if handle is NULL, pcap_close is safe in newer versions, 
    // but per instructions we must be careful. If handle is valid, close it.
    if (handle != NULL) {
        pcap_close(handle);
        handle = NULL;
    }

    // Re-open handle for subsequent tests if we want to test pcap_set_datalink
    if (Size > 0) {
        handle = pcap_open_offline("./dummy_file", errbuf);
    }

    // Step 3: Invoke pcap_lookupnet (Target 2)
    // Use a device name derived from input or default to "any"
    const char *lookup_device = (Size > 0 && Data[0] != 0) ? (const char *)Data : "any";
    // Truncate device name if it's not null-terminated or too long
    char dev_buf[256];
    size_t dev_len = (Size < 255) ? Size : 255;
    memcpy(dev_buf, Data, dev_len);
    dev_buf[dev_len] = '\0';
    
    pcap_lookupnet(dev_buf, &net, &mask, errbuf);

    // Step 4: Invoke pcap_datalink_name_to_val (Target 3)
    // Use input data as a potential datalink name
    if (Size > 0) {
        dlt_val = pcap_datalink_name_to_val(dev_buf);
        (void)dlt_val; // Suppress unused warning
    }

    // Step 5: Invoke pcap_set_datalink (Target 4)
    // Only if handle is valid
    if (handle != NULL) {
        // Try setting datalink to a value derived from input or a known constant
        int dlt_to_set = (Size > 0) ? (int)Data[0] : DLT_EN10MB;
        pcap_set_datalink(handle, dlt_to_set);
    }

    // Step 6: Invoke pcap_geterr (Target 5)
    // Check for errors on the handle
    if (handle != NULL) {
        const char *err = pcap_geterr(handle);
        (void)err; // Suppress unused warning
    } else {
        // If handle is NULL, we can't call pcap_geterr on it safely as per libpcap docs
        // But we can check the last error from pcap_open_offline if we stored it
        // For this fuzz driver, we just ensure we don't crash.
    }

    // Cleanup
    if (handle != NULL) {
        pcap_close(handle);
    }

    // Clean up dummy file
    remove("./dummy_file");

    return 0;
}