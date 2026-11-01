// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_dump_fopen at sf-pcap.c:988:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <errno.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle) {
        pcap_close(handle);
    }
}

static void cleanup_dumper(pcap_dumper_t *dumper) {
    if (dumper) {
        pcap_dump_close(dumper);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a dummy file for offline capture to ensure we have a valid pcap_t
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        return 0;
    }

    // Write a minimal pcap header to make it a valid savefile
    // Magic number, version, timezone, sigfigs, snaplen, network
    uint32_t header[6];
    header[0] = 0xa1b2c3d4; // Magic number (little endian)
    header[1] = 0x02000002; // Version major.minor
    header[2] = 0;          // Timezone
    header[3] = 0;          // Sigfigs
    header[4] = 65535;      // Snaplen
    header[5] = 1;          // Linktype (Ethernet)
    fwrite(header, sizeof(header), 1, fp);
    fclose(fp);

    // Initialize error buffer to prevent NULL pointer dereference in libpcap internals
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // Open the dummy file for reading
    pcap_t *handle = pcap_open_offline(dummy_file, errbuf);
    if (!handle) {
        remove(dummy_file);
        return 0;
    }

    pcap_dumper_t *dumper = NULL;
    pcap_dumper_t *dumper_append = NULL;
    FILE *fp_out = NULL;
    int ret = 0;

    // Use input data to generate a filename (ensure it's safe and not too long)
    char filename[256];
    size_t len = (Size < 255) ? Size : 255;
    memcpy(filename, Data, len);
    filename[len] = '\0';
    
    // Test pcap_dump_open
    dumper = pcap_dump_open(handle, filename);
    if (dumper) {
        // Test pcap_dump_flush
        ret = pcap_dump_flush(dumper);
        (void)ret; 
        
        // Test pcap_dump_file
        FILE *stream = pcap_dump_file(dumper);
        if (stream) {
            fflush(stream);
        }
        
        cleanup_dumper(dumper);
        dumper = NULL;
    }

    // Test pcap_dump_open_append
    dumper_append = pcap_dump_open_append(handle, filename);
    if (dumper_append) {
        ret = pcap_dump_flush(dumper_append);
        (void)ret;
        
        FILE *stream = pcap_dump_file(dumper_append);
        if (stream) {
            fflush(stream);
        }
        
        cleanup_dumper(dumper_append);
        dumper_append = NULL;
    }

    // Test pcap_dump_fopen
    fp_out = fopen("./fuzz_dump_output", "wb");
    if (fp_out) {
        dumper = pcap_dump_fopen(handle, fp_out);
        if (dumper) {
            ret = pcap_dump_flush(dumper);
            (void)ret;
            
            FILE *stream = pcap_dump_file(dumper);
            if (stream) {
                fflush(stream);
            }
            
            cleanup_dumper(dumper);
            dumper = NULL;
        }
        fclose(fp_out);
    }

    // Test with stdout (special case mentioned in docs)
    dumper = pcap_dump_open(handle, "-");
    if (dumper) {
        ret = pcap_dump_flush(dumper);
        (void)ret;
        FILE *stream = pcap_dump_file(dumper);
        if (stream) {
            fflush(stream);
        }
        cleanup_dumper(dumper);
    }

    cleanup_pcap(handle);
    
    // Clean up dummy files created during fuzzing
    remove(dummy_file);
    remove("./fuzz_dump_output");

    return 0;
}