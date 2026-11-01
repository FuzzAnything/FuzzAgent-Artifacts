// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
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

static void write_dummy_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // Write a minimal pcap header (24 bytes)
    // Magic number (little endian)
    uint32_t magic = 0xa1b2c3d4;
    fwrite(&magic, sizeof(uint32_t), 1, f);
    
    // Version major (2)
    uint16_t ver_major = 2;
    fwrite(&ver_major, sizeof(uint16_t), 1, f);
    
    // Version minor (4)
    uint16_t ver_minor = 4;
    fwrite(&ver_minor, sizeof(uint16_t), 1, f);
    
    // Timezone (0)
    int32_t tz = 0;
    fwrite(&tz, sizeof(int32_t), 1, f);
    
    // Sigfigs (0)
    int32_t sigfigs = 0;
    fwrite(&sigfigs, sizeof(int32_t), 1, f);
    
    // Snaplen (65535)
    uint32_t snaplen = 65535;
    fwrite(&snaplen, sizeof(uint32_t), 1, f);
    
    // Linktype (1 = Ethernet)
    uint32_t linktype = 1;
    fwrite(&linktype, sizeof(uint32_t), 1, f);
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a dummy file for pcap_open_offline
    write_dummy_file("./dummy_file");

    // Try to open the file
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline("./dummy_file", errbuf);
    
    if (!handle) {
        // If we can't open the file, try to test functions that don't require a handle
        // pcap_datalink_name_to_val can be tested with string data
        if (Size > 0) {
            // Null-terminate the input if it's not already
            char *str = (char *)malloc(Size + 1);
            if (str) {
                memcpy(str, Data, Size);
                str[Size] = '\0';
                pcap_datalink_name_to_val(str);
                free(str);
            }
        }
        return 0;
    }

    // Activate the handle if not already activated
    int activation_result = pcap_activate(handle);
    if (activation_result != 0) {
        pcap_close(handle);
        // Still test pcap_datalink_name_to_val
        if (Size > 0) {
            char *str = (char *)malloc(Size + 1);
            if (str) {
                memcpy(str, Data, Size);
                str[Size] = '\0';
                pcap_datalink_name_to_val(str);
                free(str);
            }
        }
        return 0;
    }

    // Test pcap_datalink
    pcap_datalink(handle);

    // Test pcap_datalink_ext
    pcap_datalink_ext(handle);

    // Test pcap_list_datalinks
    int *datalinks = NULL;
    int count = pcap_list_datalinks(handle, &datalinks);
    
    if (count > 0 && datalinks != NULL) {
        // Try to set datalink using values from the list
        for (int i = 0; i < count; i++) {
            pcap_set_datalink(handle, datalinks[i]);
        }
        
        // Also try setting with fuzzed data if size permits
        if (Size >= 4) {
            int fuzzed_dlt = *(const int *)Data;
            pcap_set_datalink(handle, fuzzed_dlt);
        }
        
        // Free the datalinks list
        pcap_free_datalinks(datalinks);
    }

    // Test pcap_datalink_name_to_val with fuzzed string
    if (Size > 0) {
        char *str = (char *)malloc(Size + 1);
        if (str) {
            memcpy(str, Data, Size);
            str[Size] = '\0';
            pcap_datalink_name_to_val(str);
            free(str);
        }
    }

    // Cleanup
    pcap_close(handle);
    
    return 0;
}