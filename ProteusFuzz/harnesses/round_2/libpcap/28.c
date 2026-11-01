// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_free_tstamp_types at pcap.c:534:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_fopen_offline at savefile.c:600:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static int create_dummy_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;

    // Write a minimal valid pcap global header
    uint32_t magic = 0xa1b2c3d4;
    uint16_t version_major = 2;
    uint16_t version_minor = 4;
    int32_t thiszone = 0;
    uint32_t sigfigs = 0;
    uint32_t snaplen = 65535;
    uint32_t network = 1; // LINKTYPE_ETHERNET

    fwrite(&magic, sizeof(magic), 1, f);
    fwrite(&version_major, sizeof(version_major), 1, f);
    fwrite(&version_minor, sizeof(version_minor), 1, f);
    fwrite(&thiszone, sizeof(thiszone), 1, f);
    fwrite(&sigfigs, sizeof(sigfigs), 1, f);
    fwrite(&snaplen, sizeof(snaplen), 1, f);
    fwrite(&network, sizeof(network), 1, f);

    fclose(f);
    return 0;
}

static pcap_t *open_pcap_for_fuzzing(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    FILE *fp;

    // Ensure the dummy file exists and is valid before trying to open it
    if (create_dummy_file("./dummy_file") != 0) {
        return NULL;
    }

    // Open the file to get a FILE* pointer, then pass that to pcap_fopen_offline
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        return NULL;
    }

    // pcap_fopen_offline expects a FILE*, not a filename string
    handle = pcap_fopen_offline(fp, errbuf);

    if (handle) {
        if (pcap_activate(handle) != 0) {
            pcap_close(handle);
            return NULL;
        }
    }
    
    return handle;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pcap_t *handle = open_pcap_for_fuzzing(Data, Size);
    int *datalinks = NULL;
    int ret;

    // 1. Test pcap_datalink_name_to_val
    // Construct a null-terminated string from fuzz data
    if (Size > 0) {
        char *name_str = malloc(Size + 1);
        if (name_str) {
            memcpy(name_str, Data, Size);
            name_str[Size] = '\0';
            
            // Test with the full string
            pcap_datalink_name_to_val(name_str);
            
            // Test with a truncated version if size allows
            if (Size > 1) {
                name_str[Size / 2] = '\0';
                pcap_datalink_name_to_val(name_str);
            }
            free(name_str);
        }
    }

    // 2. Test pcap_list_datalinks and pcap_free_datalinks
    if (handle) {
        ret = pcap_list_datalinks(handle, &datalinks);
        if (ret > 0 && datalinks) {
            // Try to set each datalink in the list
            for (int i = 0; i < ret; i++) {
                pcap_set_datalink(handle, datalinks[i]);
            }
            pcap_free_datalinks(datalinks);
            datalinks = NULL;
        }
    }

    // 3. Test pcap_datalink
    if (handle) {
        int dlt = pcap_datalink(handle);
        (void)dlt; // Suppress unused warning
    }

    // 4. Test pcap_set_datalink with fuzzed values
    if (handle) {
        if (Size >= 4) {
            int fuzzed_dlt = *(int *)Data;
            pcap_set_datalink(handle, fuzzed_dlt);
        } else if (Size >= 2) {
            int fuzzed_dlt = *(int16_t *)Data;
            pcap_set_datalink(handle, fuzzed_dlt);
        } else if (Size >= 1) {
            int fuzzed_dlt = Data[0];
            pcap_set_datalink(handle, fuzzed_dlt);
        }
        // Test with known constants
        pcap_set_datalink(handle, 1); // ETHERNET
        pcap_set_datalink(handle, -1);
        pcap_set_datalink(handle, 0);
    }

    // 5. Test pcap_free_tstamp_types
    // The API documentation warns that passing NULL causes undefined behavior.
    // We allocate a dummy buffer to safely test the free function.
    int *dummy_tstamp = malloc(sizeof(int));
    if (dummy_tstamp) {
        pcap_free_tstamp_types(dummy_tstamp);
    }

    // Cleanup
    if (handle) {
        pcap_close(handle);
    }

    return 0;
}