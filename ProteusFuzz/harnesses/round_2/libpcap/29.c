// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_fopen at sf-pcap.c:988:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    pcap_dumper_t *dumper = NULL;
    FILE *fp = NULL;
    int ret;

    // Ensure errbuf is initialized
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);

    // Create a dummy file to use as a savefile source
    write_dummy_file("./dummy_file", Data, Size);

    // Attempt to open the dummy file as a pcap handle
    handle = pcap_open_offline("./dummy_file", errbuf);
    if (handle == NULL) {
        // If we cannot open the file, try to create a live handle on a loopback or similar
        // This might fail on some systems, but we try to get a valid handle for testing
        handle = pcap_create("lo", errbuf);
        if (handle != NULL) {
            if (pcap_activate(handle) == 0) {
                // Successfully activated
            } else {
                pcap_close(handle);
                handle = NULL;
            }
        }
    }

    if (handle != NULL) {
        // Test pcap_dump_open
        dumper = pcap_dump_open(handle, "./fuzz_output.pcap");
        if (dumper != NULL) {
            // Test pcap_dump_flush
            ret = pcap_dump_flush(dumper);
            (void)ret; // Ignore return value for fuzzing

            // Test pcap_dump_file
            FILE *stream = pcap_dump_file(dumper);
            if (stream != NULL) {
                // Just verify it's not NULL
            }

            // Test pcap_dump_close
            pcap_dump_close(dumper);
            dumper = NULL;
        }

        // Test pcap_dump_open_append
        dumper = pcap_dump_open_append(handle, "./fuzz_output.pcap");
        if (dumper != NULL) {
            ret = pcap_dump_flush(dumper);
            (void)ret;
            pcap_dump_close(dumper);
            dumper = NULL;
        }

        // Test pcap_dump_fopen
        fp = fopen("./fuzz_output_fopen.pcap", "wb");
        if (fp != NULL) {
            dumper = pcap_dump_fopen(handle, fp);
            if (dumper != NULL) {
                ret = pcap_dump_flush(dumper);
                (void)ret;
                pcap_dump_close(dumper);
                dumper = NULL;
            }
            fclose(fp);
            fp = NULL;
        }
    }

    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}