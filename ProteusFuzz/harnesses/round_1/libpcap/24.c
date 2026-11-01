// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lib_version at pcap-linux.c:6223:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
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

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    const char *version_str;
    int major_ver;
    int ret;

    // 1. pcap_lib_version: Always succeeds, no arguments needed.
    version_str = pcap_lib_version();
    (void)version_str; // Use the return value to suppress warnings if unused

    // Prepare source string from fuzz data
    // Ensure null termination and reasonable length for source strings
    char source[256];
    size_t src_len = (Size < sizeof(source) - 1) ? Size : sizeof(source) - 1;
    memcpy(source, Data, src_len);
    source[src_len] = '\0';

    // Create a dummy file if we want to test file-based opening
    setup_dummy_file(Data, Size);

    // 2. pcap_open: Try to open with fuzzed source
    // We use "any" or the fuzzed string. If the string is invalid, it should return NULL.
    // We also try to open the dummy file.
    if (src_len > 0 && source[0] != '\0') {
        handle = pcap_open(source, 65535, 0, 1000, NULL, errbuf);
    } else {
        // Fallback to "any" if source is empty or invalid
        handle = pcap_open("any", 65535, 0, 1000, NULL, errbuf);
    }

    if (handle != NULL) {
        // 3. pcap_set_rfmon: Try to enable/disable monitor mode
        // This must be called before activation.
        // We fuzz the mode (0 or 1) based on data
        int rfmon_mode = (Size > 0) ? (Data[0] % 2) : 0;
        ret = pcap_set_rfmon(handle, rfmon_mode);
        (void)ret;

        // 6. pcap_set_promisc: Try to set promiscuous mode
        int promisc_mode = (Size > 1) ? (Data[1] % 2) : 0;
        ret = pcap_set_promisc(handle, promisc_mode);
        (void)ret;

        // Attempt to activate the handle to test post-activation behaviors
        // pcap_activate is not in the target list but is needed to test
        // pcap_major_version and ensure set_rfmon/set_promisc fail if called after.
        // However, since we can't call pcap_activate directly if it's not exposed or
        // if we want to stick strictly to the provided API list for "fuzzing",
        // we rely on the fact that pcap_open might activate or not depending on flags.
        // pcap_open with default flags usually activates.
        
        // If the handle was activated by pcap_open, set_rfmon/set_promisc should have failed.
        // If not, we can try to activate manually if we had the function, but we don't.
        // Instead, we test pcap_major_version which requires activation.
        
        // 5. pcap_major_version: Requires activated handle.
        // If handle is active, this returns version. If not, PCAP_ERROR_NOT_ACTIVATED.
        major_ver = pcap_major_version(handle);
        (void)major_ver;

        // Cleanup
        pcap_close(handle);
        handle = NULL;
    }

    // 4. pcap_open_live: Try to open live capture
    // We reuse the source string or use "any"
    if (src_len > 0 && source[0] != '\0') {
        handle = pcap_open_live(source, 65535, 0, 1000, errbuf);
    } else {
        handle = pcap_open_live("any", 65535, 0, 1000, errbuf);
    }

    if (handle != NULL) {
        // Try to set rfmon and promisc on live handle (should fail if activated)
        int rfmon_mode_live = (Size > 2) ? (Data[2] % 2) : 0;
        ret = pcap_set_rfmon(handle, rfmon_mode_live);
        (void)ret;

        int promisc_mode_live = (Size > 3) ? (Data[3] % 2) : 0;
        ret = pcap_set_promisc(handle, promisc_mode_live);
        (void)ret;

        // Check major version
        major_ver = pcap_major_version(handle);
        (void)major_ver;

        // Cleanup
        pcap_close(handle);
        handle = NULL;
    }

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}