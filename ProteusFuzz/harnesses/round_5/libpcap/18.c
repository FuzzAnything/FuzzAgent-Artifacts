// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_datalink_val_to_description_or_dlt at pcap.c:3388:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_val_to_description at pcap.c:3376:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
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
#include <errno.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Fuzz pcap_datalink_val_to_description_or_dlt
    // This function takes an int, so we extract one from the input if possible
    int dlt_val;
    if (Size >= sizeof(int)) {
        memcpy(&dlt_val, Data, sizeof(int));
    } else {
        dlt_val = (int)Data[0];
    }
    const char *desc_or_dlt = pcap_datalink_val_to_description_or_dlt(dlt_val);
    if (desc_or_dlt != NULL) {
        // Just consume the pointer to avoid unused variable warnings
        (void)desc_or_dlt;
    }

    // Fuzz pcap_datalink_val_to_name
    const char *name = pcap_datalink_val_to_name(dlt_val);
    if (name != NULL) {
        (void)name;
    }

    // Fuzz pcap_datalink_val_to_description
    const char *desc = pcap_datalink_val_to_description(dlt_val);
    if (desc != NULL) {
        (void)desc;
    }

    // Fuzz pcap_datalink_name_to_val
    // We need a string. Let's use the input data as a potential name string.
    // Ensure it's null-terminated for safety, though the function might handle it.
    char *name_buf = NULL;
    if (Size > 0) {
        name_buf = (char *)malloc(Size + 1);
        if (name_buf != NULL) {
            memcpy(name_buf, Data, Size);
            name_buf[Size] = '\0';
            int val = pcap_datalink_name_to_val(name_buf);
            if (val != -1) {
                (void)val;
            }
            free(name_buf);
        }
    }

    // Fuzz pcap_set_datalink and pcap_datalink
    // These require a valid, activated pcap_t handle.
    // We try to create a handle using a dummy file or loopback.
    // Since we cannot guarantee a live interface, we try opening a savefile or loopback.
    // Creating a dummy file for savefile mode.
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f != NULL) {
        // Write a minimal pcap header to make it a valid savefile
        // Magic number (little endian), version major, version minor
        uint32_t magic = 0xa1b2c3d4;
        uint16_t ver_major = 2;
        uint16_t ver_minor = 4;
        fwrite(&magic, sizeof(magic), 1, f);
        fwrite(&ver_major, sizeof(ver_major), 1, f);
        fwrite(&ver_minor, sizeof(ver_minor), 1, f);
        // Timezone, sigfigs, snaplen, network (linktype)
        int32_t tz = 0;
        uint32_t sigfigs = 0;
        uint32_t snaplen = 65535;
        uint32_t network = 1; // DLT_EN10MB
        fwrite(&tz, sizeof(tz), 1, f);
        fwrite(&sigfigs, sizeof(sigfigs), 1, f);
        fwrite(&snaplen, sizeof(snaplen), 1, f);
        fwrite(&network, sizeof(network), 1, f);
        fclose(f);
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;

    // Try opening the dummy savefile
    handle = pcap_open_offline(dummy_file, errbuf);
    
    if (handle == NULL) {
        // If offline failed, try loopback (might fail on some systems, but worth a try)
        handle = pcap_create("lo", errbuf);
        if (handle != NULL) {
            if (pcap_activate(handle) == 0) {
                // Handle is active
            } else {
                pcap_close(handle);
                handle = NULL;
            }
        }
    }

    if (handle != NULL) {
        // Fuzz pcap_datalink
        int current_dlt = pcap_datalink(handle);
        (void)current_dlt;

        // Fuzz pcap_set_datalink with various values
        // Use the extracted dlt_val and some variations
        int test_dlt_vals[] = {dlt_val, dlt_val + 1, dlt_val - 1, 0, 1, 100, -1, 65535};
        for (int i = 0; i < 8; i++) {
            int ret = pcap_set_datalink(handle, test_dlt_vals[i]);
            if (ret == 0) {
                // Success, maybe check the new value
                int new_dlt = pcap_datalink(handle);
                (void)new_dlt;
            }
            // ret == -1 is expected for unsupported DLTs, no error handling needed for fuzzing
        }

        cleanup_pcap(handle);
    }

    // Clean up dummy file
    remove(dummy_file);

    return 0;
}