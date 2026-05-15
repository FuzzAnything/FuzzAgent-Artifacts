// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static pcap_t *create_dummy_handle(int dlt) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_dead(dlt, 65535);
    if (!pcap) {
        return NULL;
    }
    return pcap;
}

static pcap_t *create_live_handle(void) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_create("any", errbuf);
    if (!pcap) {
        return NULL;
    }
    return pcap;
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Use first byte to control behavior
    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;

    // Test pcap_datalink_val_to_name with fuzzed DLT value
    int dlt_val = 0;
    if (fuzz_size >= sizeof(int)) {
        memcpy(&dlt_val, fuzz_data, sizeof(int));
    } else if (fuzz_size > 0) {
        dlt_val = fuzz_data[0];
    }
    const char *name = pcap_datalink_val_to_name(dlt_val);
    (void)name; // Use result to avoid unused variable warning

    // Test pcap_datalink_name_to_val with fuzzed string
    if (fuzz_size > 0) {
        // Create null-terminated string from fuzz data
        char *dlt_name = malloc(fuzz_size + 1);
        if (dlt_name) {
            memcpy(dlt_name, fuzz_data, fuzz_size);
            dlt_name[fuzz_size] = '\0';
            int val = pcap_datalink_name_to_val(dlt_name);
            (void)val; // Use result
            free(dlt_name);
        }
    }

    // Test handle-based functions with dummy handle
    pcap_t *dummy_handle = create_dummy_handle(DLT_EN10MB);
    if (dummy_handle) {
        // Test pcap_datalink
        int current_dlt = pcap_datalink(dummy_handle);
        (void)current_dlt;

        // Test pcap_datalink_ext
        int ext_dlt = pcap_datalink_ext(dummy_handle);
        (void)ext_dlt;

        // Test pcap_list_datalinks
        int *dlt_list = NULL;
        int count = pcap_list_datalinks(dummy_handle, &dlt_list);
        if (count > 0 && dlt_list) {
            free(dlt_list);
        }

        pcap_close(dummy_handle);
    }

    // Test pcap_set_datalink with live handle (must be called before activation)
    pcap_t *live_handle = create_live_handle();
    if (live_handle) {
        // Try to set datalink with fuzzed value
        int set_dlt = dlt_val;
        if (selector & 0x80) {
            set_dlt = DLT_EN10MB; // Use valid DLT for some cases
        }
        int result = pcap_set_datalink(live_handle, set_dlt);
        (void)result;

        // Try to activate after setting datalink
        if (pcap_activate(live_handle) == 0) {
            // After activation, test pcap_datalink again
            int activated_dlt = pcap_datalink(live_handle);
            (void)activated_dlt;

            // Test pcap_list_datalinks on activated handle
            int *activated_dlt_list = NULL;
            int activated_count = pcap_list_datalinks(live_handle, &activated_dlt_list);
            if (activated_count > 0 && activated_dlt_list) {
                free(activated_dlt_list);
            }
        }

        pcap_close(live_handle);
    }

    return 0;
}