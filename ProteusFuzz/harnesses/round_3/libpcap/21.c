// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_fopen_offline_with_tstamp_precision at savefile.c:467:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline_with_tstamp_precision at savefile.c:335:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline_with_tstamp_precision at savefile.c:335:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_fopen_offline at savefile.c:600:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
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
#include <fcntl.h>
#include <unistd.h>

static void create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        // Write a minimal pcap global header if data is too small or empty
        if (size < 24) {
            uint8_t header[24] = {
                0xa1, 0xb2, 0xc3, 0xd4, // magic number
                0x02, 0x00, 0x04, 0x00, // version major/minor
                0x00, 0x00, 0x00, 0x00, // thiszone
                0x00, 0x00, 0x00, 0x00, // sigfigs
                0x00, 0x00, 0x00, 0x00, // snaplen
                0x01, 0x00, 0x00, 0x00  // network (Ethernet)
            };
            fwrite(header, 1, 24, f);
        } else {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    FILE *fp = NULL;
    int ret = 0;

    // Ensure we have some data to work with for file creation
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for testing
    create_dummy_file("./dummy_file", Data, Size);

    // 1. Test pcap_open_offline_with_tstamp_precision
    handle = pcap_open_offline_with_tstamp_precision("./dummy_file", PCAP_TSTAMP_PRECISION_MICRO, errbuf);
    if (handle) {
        // Test pcap_file
        fp = pcap_file(handle);
        if (fp) {
            // Do a dummy read to exercise the stream
            fseek(fp, 0, SEEK_SET);
            fseek(fp, 0, SEEK_END);
            long fsize = ftell(fp);
            if (fsize > 0) {
                fseek(fp, 0, SEEK_SET);
                uint8_t buf[64];
                fread(buf, 1, 64, fp);
            }
        }
        pcap_close(handle);
        handle = NULL;
    }

    // 2. Test pcap_open_offline
    handle = pcap_open_offline("./dummy_file", errbuf);
    if (handle) {
        pcap_close(handle);
        handle = NULL;
    }

    // 3. Test pcap_open (try to open the file as a source)
    // Note: pcap_open might fail for local files depending on libpcap version/config,
    // but we attempt it to test the parsing logic.
    char source[256];
    snprintf(source, sizeof(source), "file://%s", "./dummy_file");
    handle = pcap_open(source, 65535, 0, 1000, NULL, errbuf);
    if (handle) {
        pcap_close(handle);
        handle = NULL;
    }

    // 4. Test pcap_fopen_offline
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        handle = pcap_fopen_offline(fp, errbuf);
        if (handle) {
            pcap_close(handle);
            handle = NULL;
        }
        fclose(fp);
        fp = NULL;
    }

    // 5. Test pcap_fopen_offline_with_tstamp_precision
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        handle = pcap_fopen_offline_with_tstamp_precision(fp, PCAP_TSTAMP_PRECISION_NANO, errbuf);
        if (handle) {
            pcap_close(handle);
            handle = NULL;
        }
        fclose(fp);
        fp = NULL;
    }

    // 6. Test edge cases with stdin ("-")
    // Redirecting stdin in a fuzzer is tricky, so we just call with "-" to test the path
    // that attempts to read from stdin. It will likely fail or block, but tests the logic.
    // We limit this to a specific condition to avoid hanging the fuzzer if stdin is not a pipe.
    if (Size > 100) {
        handle = pcap_open_offline_with_tstamp_precision("-", PCAP_TSTAMP_PRECISION_MICRO, errbuf);
        if (handle) {
            pcap_close(handle);
            handle = NULL;
        }
    }

    // Cleanup dummy file
    unlink("./dummy_file");

    return 0;
}