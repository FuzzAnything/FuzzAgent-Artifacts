// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_fopen at sf-pcap.c:988:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>

static void cleanup_dummy_file(const char *filename) {
    if (filename && strcmp(filename, "-") != 0) {
        unlink(filename);
    }
}

static pcap_t *create_dummy_pcap(const uint8_t *data, size_t size) {
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        return NULL;
    }

    // Write a minimal pcap global header (24 bytes)
    // Magic number: 0xa1b2c3d4 (little endian)
    uint32_t magic = 0xa1b2c3d4;
    uint16_t version_major = 2;
    uint16_t version_minor = 4;
    int32_t thiszone = 0;
    uint32_t sigfigs = 0;
    uint32_t snaplen = 65535;
    uint32_t network = 1; // LINKTYPE_ETHERNET

    fwrite(&magic, sizeof(magic), 1, fp);
    fwrite(&version_major, sizeof(version_major), 1, fp);
    fwrite(&version_minor, sizeof(version_minor), 1, fp);
    fwrite(&thiszone, sizeof(thiszone), 1, fp);
    fwrite(&sigfigs, sizeof(sigfigs), 1, fp);
    fwrite(&snaplen, sizeof(snaplen), 1, fp);
    fwrite(&network, sizeof(network), 1, fp);

    // Write one dummy packet if data is available
    if (size > 0) {
        struct pcap_pkthdr hdr;
        memset(&hdr, 0, sizeof(hdr));
        hdr.ts.tv_sec = 1;
        hdr.ts.tv_usec = 0;
        hdr.caplen = size > 65535 ? 65535 : size;
        hdr.len = size;
        
        fwrite(&hdr, sizeof(hdr), 1, fp);
        fwrite(data, 1, hdr.caplen, fp);
    }

    fclose(fp);

    pcap_t *handle = pcap_open_offline(dummy_file, NULL);
    if (handle) {
        // Activate the handle to ensure it's ready for dumping
        // pcap_open_offline usually activates automatically, but we ensure state
        if (pcap_activate(handle) != 0) {
            pcap_close(handle);
            cleanup_dummy_file(dummy_file);
            return NULL;
        }
    }
    return handle;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy pcap handle from the input data
    pcap_t *handle = create_dummy_pcap(Data, Size);
    if (!handle) {
        return 0;
    }

    const char *filename = "./fuzz_output.pcap";
    cleanup_dummy_file(filename);

    // Test 1: pcap_dump_open
    pcap_dumper_t *dumper1 = pcap_dump_open(handle, filename);
    if (dumper1) {
        // Test 5: pcap_dump_file
        FILE *fp = pcap_dump_file(dumper1);
        if (fp) {
            // Try to write something to trigger internal logic
            fwrite("test", 1, 4, fp);
        }
        
        // Test 2: pcap_dump_flush
        pcap_dump_flush(dumper1);

        // Test 4: pcap_dump_close
        pcap_dump_close(dumper1);
    }

    // Test 3: pcap_dump_fopen
    FILE *fp_out = fopen(filename, "wb");
    if (fp_out) {
        pcap_dumper_t *dumper2 = pcap_dump_fopen(handle, fp_out);
        if (dumper2) {
            pcap_dump_flush(dumper2);
            pcap_dump_close(dumper2);
        }
        fclose(fp_out);
    }

    // Test 6 (Append mode): pcap_dump_open_append
    // First create a file with valid header to append to
    if (dumper1) {
        // Re-open for append
        pcap_dumper_t *dumper3 = pcap_dump_open_append(handle, filename);
        if (dumper3) {
            pcap_dump_flush(dumper3);
            pcap_dump_close(dumper3);
        }
    }

    // Cleanup
    pcap_close(handle);
    cleanup_dummy_file(filename);
    cleanup_dummy_file("./dummy_file");

    return 0;
}