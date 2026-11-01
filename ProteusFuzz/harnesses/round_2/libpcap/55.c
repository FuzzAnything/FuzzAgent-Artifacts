// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_minor_version at pcap.c:3479:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
// pcap_major_version at pcap.c:3471:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
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
#include <unistd.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int ret;

    if (Size == 0) {
        return 0;
    }

    // Use the first byte to decide which API to test
    int api_choice = Data[0] % 6;

    // Prepare a dummy file if needed (for savefile scenarios, though these APIs require active handles)
    // Since pcap_snapshot, pcap_minor_version, etc. require an ACTIVE handle,
    // we must open a live capture or create and activate a handle.
    // We will try to open "any" or a loopback interface. If that fails, we try pcap_create.
    
    // Strategy:
    // 1. Try pcap_create with "any" or "lo"
    // 2. Activate it
    // 3. Call the target function
    // 4. Cleanup

    const char *device = "any";
    
    // Create a handle
    handle = pcap_create(device, errbuf);
    if (handle == NULL) {
        // If "any" fails, try "lo" (loopback)
        handle = pcap_create("lo", errbuf);
        if (handle == NULL) {
            // If both fail, we cannot test the active APIs.
            // We might try to open a dummy file, but pcap_activate on a file handle 
            // works differently. However, pcap_snapshot etc work on active handles.
            // Let's try to open a live capture directly if create fails.
            handle = pcap_open_live("any", 65535, 0, 1000, errbuf);
            if (handle == NULL) {
                handle = pcap_open_live("lo", 65535, 0, 1000, errbuf);
            }
        }
    }

    if (handle == NULL) {
        // If we cannot get a handle, we cannot test these specific APIs effectively.
        // Return 0 to indicate no crash, but no coverage on the target logic.
        return 0;
    }

    // Ensure the handle is activated if it wasn't opened live directly
    // pcap_create returns an inactive handle. pcap_open_live returns an active one.
    // We check the 'activated' flag or just try to activate.
    // Note: pcap_activate might fail on some interfaces if permissions are missing.
    // We assume the fuzzer runs in an environment where at least one interface is accessible.
    
    // If it was created via pcap_create, it needs activation.
    // If it was opened via pcap_open_live, it is already active.
    // We can check the 'activated' field in the struct, but that's internal.
    // Instead, we rely on the fact that pcap_activate on an already active handle 
    // might fail or be a no-op depending on implementation, but usually pcap_open_live
    // sets it up.
    
    // Let's try to activate if we used pcap_create. 
    // Since we don't track which path we took easily without flags, 
    // and pcap_activate on an already active handle is generally safe or returns error,
    // we will attempt activation. If it fails, we proceed to test the APIs which will 
    // likely return PCAP_ERROR_NOT_ACTIVATED, which is a valid state to test.
    
    // However, to get meaningful coverage (success path), we need an active handle.
    // If pcap_open_live succeeded, we are good.
    // If pcap_create succeeded, we must activate.
    
    // Heuristic: If we are here, handle is valid. 
    // We will try to activate. If it fails, the subsequent API calls will return error codes,
    // which is also a valid test case for error handling.
    
    // We use a small buffer size for activation to reduce resource usage.
    // We pass a dummy buffer from the fuzzer input if available, or allocate one.
    // pcap_activate signature: int pcap_activate(pcap_t *);
    // It uses internal options.
    
    // Note: pcap_activate might require the handle to be configured.
    // pcap_create sets defaults.
    
    // Attempt activation
    int activate_ret = pcap_activate(handle);
    
    // Now test the APIs based on api_choice
    switch (api_choice) {
        case 0: // pcap_snapshot
            ret = pcap_snapshot(handle);
            break;
        case 1: // pcap_minor_version
            ret = pcap_minor_version(handle);
            break;
        case 2: // pcap_inject
            // pcap_inject requires data and size.
            // Size must be > 0 and <= INT_MAX.
            // We use the fuzzer input as the packet data.
            // If Size is 0, we skip or use a dummy byte.
            size_t inject_size = Size;
            const void *inject_data = Data;
            
            if (inject_size == 0) {
                // Create a dummy 1-byte packet to satisfy >0 constraint
                static uint8_t dummy_byte = 0xFF;
                inject_data = &dummy_byte;
                inject_size = 1;
            }
            
            // Ensure size doesn't exceed INT_MAX (unlikely for fuzzing but safe)
            if (inject_size > 2147483647) {
                inject_size = 2147483647;
            }
            
            ret = pcap_inject(handle, inject_data, inject_size);
            break;
        case 3: // pcap_is_swapped
            ret = pcap_is_swapped(handle);
            break;
        case 4: // pcap_major_version
            ret = pcap_major_version(handle);
            break;
        case 5: // pcap_datalink_ext
            ret = pcap_datalink_ext(handle);
            break;
        default:
            ret = 0;
            break;
    }

    // Cleanup
    cleanup_pcap(handle);

    return 0;
}