// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
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

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Prepare environment: Create a dummy file for pcap_open_offline if needed
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        // Write minimal pcap header to make it a valid file for pcap_open_offline
        // Magic number, version, timezone, sigfigs, snapshot, network
        uint8_t header[24] = {
            0xa1, 0xb2, 0xc3, 0xd4, // Magic number (microseconds)
            0x02, 0x00, 0x04, 0x00, // Version major/minor
            0x00, 0x00, 0x00, 0x00, // Timezone
            0x00, 0x00, 0x00, 0x00, // Sigfigs
            0x00, 0x00, 0x00, 0x00, // Snapshot length
            0x01, 0x00, 0x00, 0x00  // Linktype (Ethernet)
        };
        fwrite(header, 1, 24, f);
        // Write some dummy packet data if size allows
        if (Size > 24) {
            fwrite(Data + 24, 1, Size - 24, f);
        }
        fclose(f);
    }

    pcap_t *handle = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 net, mask;

    // Attempt to open the dummy file to get a handle
    handle = pcap_open_offline(dummy_file, errbuf);
    
    // 1. pcap_close
    // We close the handle immediately to test the cleanup logic.
    if (handle != NULL) {
        pcap_close(handle);
        handle = NULL; // Ensure we don't use it again
    }

    // 2. pcap_lookupnet
    // We need a device name. Let's try "lo" (loopback) or extract from fuzz input.
    const char *device = "lo";
    if (Size > 10) {
        // Use part of the input as device name, ensuring null termination
        char dev_buf[16];
        size_t dev_len = Size > 15 ? 15 : Size;
        memcpy(dev_buf, Data, dev_len);
        dev_buf[dev_len] = '\0';
        device = dev_buf;
    }

    int lookup_res = pcap_lookupnet(device, &net, &mask, errbuf);
    (void)lookup_res; // Suppress unused variable warning

    // 3. pcap_datalink_name_to_val
    // Extract a string from fuzz input to use as datalink name
    const char *dlt_name = "ethernet";
    if (Size > 5) {
        char name_buf[32];
        size_t name_len = Size > 31 ? 31 : Size;
        memcpy(name_buf, Data, name_len);
        name_buf[name_len] = '\0';
        dlt_name = name_buf;
    }
    
    int dlt_val = pcap_datalink_name_to_val(dlt_name);

    // 4. pcap_set_datalink
    // This requires a valid handle. Since we closed it in step 1, we re-open one.
    pcap_t *handle2 = pcap_open_offline(dummy_file, errbuf);
    if (handle2 != NULL) {
        // Try to set datalink to the value we got, or a default if -1
        int target_dlt = (dlt_val != -1) ? dlt_val : 1; // 1 is DLT_EN10MB
        pcap_set_datalink(handle2, target_dlt);
        
        // 5. pcap_geterr
        char *err_str = pcap_geterr(handle2);
        (void)err_str; // Use the result to avoid unused warning
        
        pcap_close(handle2);
    }

    // Cleanup dummy file
    remove(dummy_file);

    return 0;
}