// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
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

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    const char *str;
    int dlt_val;
    pcap_t *handle_dead = NULL;
    pcap_t *handle_offline = NULL;
    struct bpf_program fp;
    int ret;
    int linktype;
    int snaplen;

    // 1. pcap_strerror
    // Test with a standard error code (PCAP_ERROR) and potentially fuzzed values
    str = pcap_strerror(-1);
    (void)str; // Use the return value to avoid unused warnings

    // 2. pcap_open_offline
    // Create a dummy file with fuzzed data to open
    if (Size > 0) {
        write_dummy_file(Data, Size);
        handle_offline = pcap_open_offline("./dummy_file", errbuf);
    }

    // 3. pcap_datalink_name_to_val
    // Extract a potential string from the fuzzed input or use a default
    const char *dlt_name = "ethernet";
    if (Size > 10) {
        // Try to find a null-terminated string in the input or use a slice
        // For simplicity, we just use the first 10 bytes as a potential name if it looks safe
        // or fall back to default. Since we can't easily validate C strings from raw fuzz input,
        // we'll just use the default or a simple slice if it contains no nulls.
        // A safer approach for fuzzing: just pass a fixed string or a slice that we ensure is null-terminated.
        // Here we just use the default "ethernet" to ensure stability, or try to parse if Size allows.
        // Let's try to construct a name from the input if possible.
        char name_buf[16] = {0};
        size_t copy_len = Size < 15 ? Size : 15;
        memcpy(name_buf, Data, copy_len);
        dlt_name = name_buf;
    }
    dlt_val = pcap_datalink_name_to_val(dlt_name);
    (void)dlt_val;

    // 4. pcap_open_dead
    // Extract linktype and snaplen from input if available, otherwise use defaults
    linktype = DLT_EN10MB;
    snaplen = 65535;

    if (Size >= 4) {
        linktype = (int)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
        // Ensure linktype is within a reasonable range for pcap_open_dead to handle gracefully
        // pcap_open_dead might fail on invalid linktypes, which is good for fuzzing.
    }
    if (Size >= 8) {
        snaplen = (int)(Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24));
    }

    handle_dead = pcap_open_dead(linktype, snaplen);

    // 5. pcap_compile
    // We need a valid handle. Try handle_dead first, then handle_offline if dead failed.
    pcap_t *target_handle = handle_dead;
    if (!target_handle && handle_offline) {
        target_handle = handle_offline;
    }

    if (target_handle) {
        // Prepare filter string from input or use default
        const char *filter_str = "host 127.0.0.1";
        char filter_buf[256] = {0};
        size_t filter_len = Size < 255 ? Size : 255;
        memcpy(filter_buf, Data, filter_len);
        filter_str = filter_buf;

        // Compile the filter
        ret = pcap_compile(target_handle, &fp, filter_str, 0, 0);
        if (ret == 0) {
            // Cleanup the program if compilation succeeded
            pcap_freecode(&fp);
        }
    }

    // Cleanup
    if (handle_dead) {
        pcap_close(handle_dead);
    }
    if (handle_offline) {
        pcap_close(handle_offline);
    }

    return 0;
}