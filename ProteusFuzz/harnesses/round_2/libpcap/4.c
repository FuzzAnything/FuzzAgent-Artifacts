// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
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
    if (f) {
        // Write a minimal pcap header (24 bytes)
        uint8_t header[24] = {
            0xa1, 0xb2, 0xc3, 0xd4, // magic number
            0x02, 0x00,             // version major
            0x04, 0x00,             // version minor
            0x00, 0x00,             // timezone
            0x00, 0x00,             // sigfigs
            0xff, 0xff, 0x00, 0x00, // snaplen
            0x01, 0x00, 0x00, 0x00  // network (Ethernet)
        };
        fwrite(header, 1, sizeof(header), f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    struct bpf_program fp;
    bpf_u_int32 net, mask;
    const char *dummy_file = "./dummy_file";

    // Ensure dummy file exists for potential savefile operations
    write_dummy_file(dummy_file);

    // 1. pcap_statustostr
    int status_code = Data[0];
    const char *status_str = pcap_statustostr(status_code);
    (void)status_str; // Use to avoid unused warning

    // 2. pcap_geterr (handle is NULL, but function might be safe or we test behavior)
    // Note: pcap_geterr typically expects a valid handle. Passing NULL might crash or return generic.
    // To be safe and follow the sequence, we might need a handle. However, the prompt asks for this order.
    // If pcap_geterr(NULL) crashes, the fuzzer will catch it.
    char *err = pcap_geterr(NULL);
    (void)err;

    // 3. pcap_statustostr again
    status_str = pcap_statustostr(Data[0] ^ 0xFF);
    (void)status_str;

    // 4. pcap_geterr again
    err = pcap_geterr(NULL);
    (void)err;

    // 5. pcap_lookupnet
    // We need a device name. Use "any" or a name derived from input if possible, but "any" is safest for 0.0.0.0
    const char *device = "any";
    if (Size > 10 && Data[1] == 'a' && Data[2] == 'n' && Data[3] == 'y') {
        device = (const char *)Data + 1;
    }
    // Ensure null termination if using input data
    if (Size > 10 && device != "any") {
        char dev_buf[256];
        size_t copy_len = Size - 1;
        if (copy_len > 255) copy_len = 255;
        memcpy(dev_buf, Data + 1, copy_len);
        dev_buf[copy_len] = '\0';
        device = dev_buf;
    }

    int lookup_res = pcap_lookupnet(device, &net, &mask, errbuf);
    (void)lookup_res;

    // 6. pcap_compile
    // We need a valid handle to compile. Let's try to open the dummy file.
    handle = pcap_open_offline(dummy_file, errbuf);
    if (handle) {
        // Prepare filter string from input
        char filter[256];
        size_t filter_len = Size > 255 ? 255 : Size;
        memcpy(filter, Data, filter_len);
        filter[filter_len] = '\0';

        // Compile
        int compile_res = pcap_compile(handle, &fp, filter, 1, net);
        (void)compile_res;

        // 7. pcap_geterr
        err = pcap_geterr(handle);
        (void)err;

        // 8. pcap_setfilter
        // Only set filter if compilation succeeded (compile_res == 0)
        if (compile_res == 0) {
            int set_res = pcap_setfilter(handle, &fp);
            (void)set_res;
        }

        // 9. pcap_geterr
        err = pcap_geterr(handle);
        (void)err;

        // 10. pcap_breakloop
        // Safe to call even if no loop is running
        pcap_breakloop(handle);

        // 11. pcap_close
        pcap_close(handle);
        handle = NULL;

        // 12. pcap_freecode
        pcap_freecode(&fp);
    } else {
        // If handle is NULL, we can't compile or setfilter.
        // We still need to call pcap_freecode if we had allocated something, but we didn't.
        // Just ensure we don't use handle.
        // The sequence requires pcap_freecode at the end.
        // If we didn't compile successfully, fp might be uninitialized or zeroed.
        // pcap_freecode handles NULL bf_insns safely.
        memset(&fp, 0, sizeof(fp));
        pcap_freecode(&fp);
    }

    return 0;
}