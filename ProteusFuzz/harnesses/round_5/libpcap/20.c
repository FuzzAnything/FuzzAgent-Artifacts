// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_tstamp_type_val_to_description at pcap.c:3443:1 in pcap.h
// pcap_tstamp_type_val_to_name at pcap.c:3431:1 in pcap.h
// pcap_tstamp_type_name_to_val at pcap.c:3419:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_get_tstamp_precision at pcap.c:2689:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <errno.h>
#include <unistd.h>

static void create_dummy_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return;
    }
    // Write a minimal pcap global header (24 bytes)
    unsigned char header[] = {
        0xa1, 0xb2, 0xc3, 0xd4, // Magic number (microseconds)
        0x02, 0x00,             // Version major
        0x04, 0x00,             // Version minor
        0x00, 0x00, 0x00, 0x00, // thiszone
        0x00, 0x00, 0x00, 0x00, // sigfigs
        0x00, 0x00, 0x00, 0x00, // snaplen
        0x01, 0x00, 0x00, 0x00  // network (Ethernet)
    };
    fwrite(header, 1, sizeof(header), f);
    fclose(f);
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Extract parameters from fuzz input
    int tstamp_type_val = (int)Data[0];
    int tstamp_precision_val = (int)Data[1];
    
    // Create a dummy file for savefile operations
    const char *dummy_file = "./dummy_file";
    create_dummy_file(dummy_file);

    // 1. Test pcap_tstamp_type_val_to_description
    const char *desc = pcap_tstamp_type_val_to_description(tstamp_type_val);
    (void)desc; // Suppress unused warning

    // 2. Test pcap_tstamp_type_val_to_name
    const char *name = pcap_tstamp_type_val_to_name(tstamp_type_val);
    (void)name; // Suppress unused warning

    // 3. Test pcap_tstamp_type_name_to_val
    // Use a portion of the input as a string (ensure null termination)
    char str_buf[256];
    // Ensure we don't read past the input buffer
    size_t str_len = (Size > 2) ? (Size - 2) : 0;
    // Limit to buffer size minus 1 for null terminator
    if (str_len >= sizeof(str_buf)) {
        str_len = sizeof(str_buf) - 1;
    }
    
    if (str_len > 0) {
        memcpy(str_buf, Data + 2, str_len);
        str_buf[str_len] = '\0';
        int val = pcap_tstamp_type_name_to_val(str_buf);
        (void)val;
    }

    // 4. Test pcap_get_tstamp_precision and pcap_set_tstamp_type
    pcap_t *handle = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];

    // Try to open the dummy file
    handle = pcap_open_offline(dummy_file, errbuf);
    if (handle != NULL) {
        // Test pcap_get_tstamp_precision
        int precision = pcap_get_tstamp_precision(handle);
        (void)precision;

        // Test pcap_set_tstamp_type with various values
        pcap_set_tstamp_type(handle, tstamp_type_val);
        
        // Try setting with the value from input
        pcap_set_tstamp_type(handle, tstamp_precision_val);

        // Cleanup
        pcap_close(handle);
        handle = NULL;
    }

    // Cleanup dummy file
    unlink(dummy_file);

    return 0;
}