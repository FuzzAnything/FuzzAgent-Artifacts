// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump at sf-pcap.c:819:1 in pcap.h
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_ftell64 at sf-pcap.c:1220:1 in pcap.h
// pcap_dump_fopen at sf-pcap.c:988:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_ftell64 at sf-pcap.c:1220:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_ftell64 at sf-pcap.c:1220:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <unistd.h>
#include <sys/stat.h>

static void cleanup_dumper(pcap_dumper_t *dumper) {
    if (dumper != NULL) {
        pcap_dump_close(dumper);
    }
}

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

static void cleanup_file(const char *filename) {
    if (filename != NULL) {
        unlink(filename);
    }
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *dummy_file = "./dummy_file";
    const char *dummy_append_file = "./dummy_append_file";
    const char *dummy_stdin_file = "./dummy_stdin_file";

    // Clean up any existing dummy files
    cleanup_file(dummy_file);
    cleanup_file(dummy_append_file);
    cleanup_file(dummy_stdin_file);

    // Create a dummy file for pcap_open_offline with a valid pcap header
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        return 0;
    }

    // Write a minimal valid pcap header (24 bytes)
    // Magic number: 0xa1b2c3d4 (microseconds, not nanoseconds)
    // Version: 2.4
    // Timezone: 0
    // Sigfigs: 0
    // Snaplen: 65535
    // Linktype: 1 (Ethernet)
    unsigned char header[24] = {
        0xa1, 0xb2, 0xc3, 0xd4, // magic number
        0x02, 0x00,             // major version
        0x04, 0x00,             // minor version
        0x00, 0x00, 0x00, 0x00, // timezone
        0x00, 0x00, 0x00, 0x00, // sigfigs
        0xff, 0xff, 0x00, 0x00, // snaplen
        0x01, 0x00, 0x00, 0x00  // linktype (Ethernet)
    };
    if (fwrite(header, 1, 24, fp) != 24) {
        fclose(fp);
        cleanup_file(dummy_file);
        return 0;
    }
    fclose(fp);

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    pcap_dumper_t *dumper = NULL;
    pcap_dumper_t *dumper_append = NULL;
    pcap_dumper_t *dumper_fopen = NULL;
    FILE *fp_stream = NULL;

    // Open pcap handle from file
    handle = pcap_open_offline(dummy_file, errbuf);
    if (handle == NULL) {
        // If we can't open the file, return early
        cleanup_file(dummy_file);
        return 0;
    }

    // Test pcap_dump_open
    dumper = pcap_dump_open(handle, dummy_file);
    if (dumper != NULL) {
        long pos = pcap_dump_ftell(dumper);
        (void)pos; // Use the value to avoid unused variable warning

        int64_t pos64 = pcap_dump_ftell64(dumper);
        (void)pos64; // Use the value to avoid unused variable warning

        cleanup_dumper(dumper);
    }

    // Test pcap_dump_open_append
    // First, create a valid pcap file to append to
    dumper = pcap_dump_open(handle, dummy_append_file);
    if (dumper != NULL) {
        // Write a dummy packet header to make it a valid pcap file
        struct pcap_pkthdr hdr;
        memset(&hdr, 0, sizeof(hdr));
        hdr.caplen = 0;
        hdr.len = 0;
        pcap_dump(dumper, &hdr, NULL);
        cleanup_dumper(dumper);
    }

    // Now try to open in append mode
    dumper_append = pcap_dump_open_append(handle, dummy_append_file);
    if (dumper_append != NULL) {
        long pos = pcap_dump_ftell(dumper_append);
        (void)pos;

        int64_t pos64 = pcap_dump_ftell64(dumper_append);
        (void)pos64;

        cleanup_dumper(dumper_append);
    }

    // Test pcap_dump_fopen
    fp_stream = fopen(dummy_stdin_file, "wb");
    if (fp_stream != NULL) {
        dumper_fopen = pcap_dump_fopen(handle, fp_stream);
        if (dumper_fopen != NULL) {
            long pos = pcap_dump_ftell(dumper_fopen);
            (void)pos;

            int64_t pos64 = pcap_dump_ftell64(dumper_fopen);
            (void)pos64;

            // Close the dumper, which will close the file
            cleanup_dumper(dumper_fopen);
        } else {
            // If pcap_dump_fopen failed, we need to close the file ourselves
            fclose(fp_stream);
            fp_stream = NULL;
        }
    }

    // Cleanup
    cleanup_pcap(handle);
    cleanup_file(dummy_file);
    cleanup_file(dummy_append_file);
    cleanup_file(dummy_stdin_file);

    return 0;
}