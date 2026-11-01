// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_ftell64 at sf-pcap.c:1220:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
// pcap_dump_ftell64 at sf-pcap.c:1220:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_fopen at sf-pcap.c:988:1 in pcap.h
// pcap_dump_ftell64 at sf-pcap.c:1220:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void cleanup_resources(pcap_t *handle, pcap_dumper_t *dumper) {
    if (dumper != NULL) {
        pcap_dump_close(dumper);
    }
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    pcap_dumper_t *dumper = NULL;
    FILE *fp = NULL;

    // Ensure dummy file exists with valid pcap header for offline mode
    FILE *tmp = fopen("./dummy_file", "wb");
    if (tmp) {
        uint32_t magic = 0xa1b2c3d4;
        uint16_t ver_major = 2;
        uint16_t ver_minor = 4;
        uint32_t thiszone = 0;
        uint32_t sigfigs = 0;
        uint32_t snaplen = 65535;
        uint32_t network = 1; // Ethernet
        fwrite(&magic, sizeof(magic), 1, tmp);
        fwrite(&ver_major, sizeof(ver_major), 1, tmp);
        fwrite(&ver_minor, sizeof(ver_minor), 1, tmp);
        fwrite(&thiszone, sizeof(thiszone), 1, tmp);
        fwrite(&sigfigs, sizeof(sigfigs), 1, tmp);
        fwrite(&snaplen, sizeof(snaplen), 1, tmp);
        fwrite(&network, sizeof(network), 1, tmp);
        fclose(tmp);
    }

    // Try to open offline file
    handle = pcap_open_offline("./dummy_file", errbuf);
    if (handle == NULL) {
        // Fallback to live capture if offline fails
        handle = pcap_open_live("lo", 65535, 1, 1000, errbuf);
    }

    if (handle == NULL) {
        return 0;
    }

    // Activate the handle if necessary
    if (pcap_activate(handle) != 0) {
        // Activation failed, but handle might still be usable for some operations
        // We proceed cautiously
    }

    // Test pcap_dump_open
    dumper = pcap_dump_open(handle, "./dummy_file");
    if (dumper != NULL) {
        (void)pcap_dump_ftell64(dumper);
        (void)pcap_dump_ftell(dumper);
        pcap_dump_close(dumper);
        dumper = NULL;
    }

    // Test pcap_dump_open_append
    dumper = pcap_dump_open_append(handle, "./dummy_file");
    if (dumper != NULL) {
        (void)pcap_dump_ftell64(dumper);
        (void)pcap_dump_ftell(dumper);
        pcap_dump_close(dumper);
        dumper = NULL;
    }

    // Test pcap_dump_fopen
    // We must ensure we don't close the FILE* if pcap_dump_close does it.
    // According to docs, pcap_dump_close calls fclose on the underlying stream unless it's stdout.
    // So we should NOT fclose(fp) manually if dumper was successfully created.
    fp = fopen("./dummy_file", "wb");
    if (fp != NULL) {
        dumper = pcap_dump_fopen(handle, fp);
        if (dumper != NULL) {
            (void)pcap_dump_ftell64(dumper);
            (void)pcap_dump_ftell(dumper);
            pcap_dump_close(dumper);
            dumper = NULL;
            // Do NOT fclose(fp) here because pcap_dump_close already closed it
        } else {
            // If pcap_dump_fopen failed, we own the FILE* and must close it
            fclose(fp);
            fp = NULL;
        }
    }

    cleanup_resources(handle, dumper);
    
    return 0;
}