// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_fopen at sf-pcap.c:988:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <unistd.h>
#include <fcntl.h>

/*
 * Fuzz driver for libpcap dump and file handling functions.
 * Target APIs:
 * - pcap_dump_open
 * - pcap_dump_open_append
 * - pcap_dump_fopen
 * - pcap_dump_ftell
 * - pcap_file
 * - pcap_dump_file
 */

static void cleanup_resources(pcap_t *handle, pcap_dumper_t *dumper) {
    if (dumper != NULL) {
        pcap_dump_close(dumper);
    }
    if (handle != NULL) {
        pcap_close(handle);
    }
}

static pcap_t *create_test_pcap(const uint8_t *data, size_t size) {
    pcap_t *handle = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    const char *device = "lo";
    
    /* Try live capture on loopback */
    handle = pcap_create(device, errbuf);
    if (handle != NULL) {
        if (pcap_set_snaplen(handle, 65535) == -1) {
            pcap_close(handle);
            handle = NULL;
        }
        if (handle != NULL && pcap_set_promisc(handle, 0) == -1) {
            pcap_close(handle);
            handle = NULL;
        }
        if (handle != NULL && pcap_set_timeout(handle, 1000) == -1) {
            pcap_close(handle);
            handle = NULL;
        }
        if (handle != NULL && pcap_activate(handle) == -1) {
            pcap_close(handle);
            handle = NULL;
        }
    }

    /* If live capture failed, try to create a dummy file and open offline */
    if (handle == NULL) {
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp != NULL) {
            uint32_t magic = 0xa1b2c3d4;
            uint16_t version_major = 2;
            uint16_t version_minor = 4;
            int32_t timezone = 0;
            int32_t sigfigs = 0;
            uint32_t snaplen = 65535;
            uint32_t network = 1; /* Ethernet */
            
            fwrite(&magic, sizeof(magic), 1, fp);
            fwrite(&version_major, sizeof(version_major), 1, fp);
            fwrite(&version_minor, sizeof(version_minor), 1, fp);
            fwrite(&timezone, sizeof(timezone), 1, fp);
            fwrite(&sigfigs, sizeof(sigfigs), 1, fp);
            fwrite(&snaplen, sizeof(snaplen), 1, fp);
            fwrite(&network, sizeof(network), 1, fp);
            
            if (size > 0) {
                fwrite(data, 1, size, fp);
            }
            fclose(fp);
            
            handle = pcap_open_offline("./dummy_file", errbuf);
        }
    }
    
    return handle;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pcap_t *handle = NULL;
    pcap_dumper_t *dumper = NULL;
    FILE *fp = NULL;
    long pos = 0;
    FILE *file_stream = NULL;
    FILE *dump_file_stream = NULL;

    /* Step 1: Create a pcap handle */
    handle = create_test_pcap(Data, Size);
    if (handle == NULL) {
        return 0;
    }

    /* Step 2: Test pcap_dump_open */
    dumper = pcap_dump_open(handle, "./dummy_file");
    if (dumper != NULL) {
        pos = pcap_dump_ftell(dumper);
        (void)pos;
        
        dump_file_stream = pcap_dump_file(dumper);
        if (dump_file_stream != NULL) {
            fflush(dump_file_stream);
        }
        
        pcap_dump_close(dumper);
        dumper = NULL;
    }

    /* Step 3: Test pcap_dump_open_append */
    dumper = pcap_dump_open_append(handle, "./dummy_file");
    if (dumper != NULL) {
        pos = pcap_dump_ftell(dumper);
        (void)pos;
        pcap_dump_close(dumper);
        dumper = NULL;
    }

    /* Step 4: Test pcap_dump_fopen */
    /* Open a file manually and pass it to pcap_dump_fopen */
    fp = fopen("./dummy_file_fopen", "wb");
    if (fp != NULL) {
        dumper = pcap_dump_fopen(handle, fp);
        if (dumper != NULL) {
            pos = pcap_dump_ftell(dumper);
            (void)pos;
            dump_file_stream = pcap_dump_file(dumper);
            if (dump_file_stream != NULL) {
                fflush(dump_file_stream);
            }
            /* pcap_dump_close will close the underlying FILE* passed to pcap_dump_fopen */
            pcap_dump_close(dumper);
            dumper = NULL;
        } else {
            /* If pcap_dump_fopen fails, we must close the file ourselves */
            fclose(fp);
            fp = NULL;
        }
    }

    /* Step 5: Test pcap_file */
    file_stream = pcap_file(handle);
    if (file_stream != NULL) {
        fflush(file_stream);
        /* Do not fclose here, pcap_close will handle it */
    }

    /* Cleanup */
    cleanup_resources(handle, dumper);

    /* Remove dummy files */
    unlink("./dummy_file");
    unlink("./dummy_file_fopen");

    return 0;
}