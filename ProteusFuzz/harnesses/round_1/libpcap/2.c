// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
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

static void write_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        // Write a minimal pcap file header
        // Magic number (little endian)
        uint32_t magic = 0xa1b2c3d4;
        // Version major
        uint16_t version_major = 2;
        // Version minor
        uint16_t version_minor = 4;
        // Timezone offset
        int32_t tz = 0;
        // Sigfigs
        uint32_t sigfigs = 0;
        // Snaplen
        uint32_t snaplen = 65535;
        // Linktype (DLT_EN10MB)
        uint32_t linktype = 1;

        fwrite(&magic, sizeof(magic), 1, f);
        fwrite(&version_major, sizeof(version_major), 1, f);
        fwrite(&version_minor, sizeof(version_minor), 1, f);
        fwrite(&tz, sizeof(tz), 1, f);
        fwrite(&sigfigs, sizeof(sigfigs), 1, f);
        fwrite(&snaplen, sizeof(snaplen), 1, f);
        fwrite(&linktype, sizeof(linktype), 1, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // 1. pcap_strerror
    // Use the first byte as an error code
    int err_code = Data[0];
    const char *err_str = pcap_strerror(err_code);
    (void)err_str; // Suppress unused warning

    // Prepare dummy file for pcap_open_offline
    write_dummy_file();

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // 2. pcap_open_offline
    pcap_t *handle_offline = pcap_open_offline("./dummy_file", errbuf);
    if (handle_offline == NULL) {
        // If opening fails, we can't proceed with compilation on this handle,
        // but we can still test other functions.
        // We will try pcap_open_dead later.
    }

    // 3. pcap_datalink_name_to_val
    // Use a substring from the input as the datalink name
    // Ensure we have at least 2 bytes to form a string
    char dlt_name[256];
    size_t name_len = (Size < 255) ? Size : 255;
    memcpy(dlt_name, Data, name_len);
    dlt_name[name_len] = '\0';

    int dlt_val = pcap_datalink_name_to_val(dlt_name);
    (void)dlt_val;

    // 4. pcap_open_dead
    // Use the second byte as linktype (modulo to keep it somewhat reasonable, though invalid is fine for fuzzing)
    int linktype = Data[1] % 200; 
    // Use a fixed snaplen or derive from input if larger
    int snaplen = (Size > 2) ? (int)Data[2] : 65535;
    
    pcap_t *handle_dead = pcap_open_dead(linktype, snaplen);
    if (handle_dead == NULL) {
        // If dead handle fails, we can't compile on it.
        // We might try to compile on the offline handle if it exists.
    }

    // 5. pcap_compile
    // We need a filter string. Use a portion of the input.
    // Ensure null termination.
    char filter_str[256];
    size_t filter_len = (Size < 255) ? Size : 255;
    memcpy(filter_str, Data, filter_len);
    filter_str[filter_len] = '\0';

    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));

    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    int optimize = 0;

    int compile_result = -1;

    // Try to compile on the offline handle if available
    if (handle_offline != NULL) {
        compile_result = pcap_compile(handle_offline, &fp, filter_str, optimize, net);
        if (compile_result == 0) {
            pcap_freecode(&fp);
        }
        pcap_close(handle_offline);
    }

    // Try to compile on the dead handle if available
    if (handle_dead != NULL) {
        compile_result = pcap_compile(handle_dead, &fp, filter_str, optimize, net);
        if (compile_result == 0) {
            pcap_freecode(&fp);
        }
        pcap_close(handle_dead);
    }

    return 0;
}