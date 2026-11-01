// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_fopen at sf-pcap.c:988:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // Write a minimal valid pcap header
    uint32_t magic = 0xa1b2c3d4;
    uint16_t version_major = 2;
    uint16_t version_minor = 4;
    int32_t thiszone = 0;
    uint32_t sigfigs = 0;
    uint32_t snaplen = 65535;
    uint32_t network = 1; // Ethernet
    
    fwrite(&magic, sizeof(magic), 1, f);
    fwrite(&version_major, sizeof(version_major), 1, f);
    fwrite(&version_minor, sizeof(version_minor), 1, f);
    fwrite(&thiszone, sizeof(thiszone), 1, f);
    fwrite(&sigfigs, sizeof(sigfigs), 1, f);
    fwrite(&snaplen, sizeof(snaplen), 1, f);
    fwrite(&network, sizeof(network), 1, f);
    
    // Write the fuzz data as packet payload
    if (size > 0) {
        struct pcap_pkthdr hdr;
        hdr.ts.tv_sec = 0;
        hdr.ts.tv_usec = 0;
        hdr.caplen = size;
        hdr.len = size;
        
        fwrite(&hdr, sizeof(hdr), 1, f);
        fwrite(data, 1, size, f);
    }
    
    fclose(f);
}

static void cleanup_dummy_file(const char *filename) {
    unlink(filename);
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    const char *dummy_file = "./dummy_file";
    pcap_t *handle = NULL;
    pcap_dumper_t *dumper = NULL;
    FILE *fp = NULL;
    int test_fopen_success = 0;
    
    if (Size == 0) {
        return 0;
    }
    
    // Create a dummy pcap file
    write_dummy_file(dummy_file, Data, Size);
    
    // Open the dummy file for reading
    handle = pcap_open_offline(dummy_file, NULL);
    if (!handle) {
        cleanup_dummy_file(dummy_file);
        return 0;
    }
    
    // Test 1: pcap_dump_open
    dumper = pcap_dump_open(handle, dummy_file);
    if (dumper) {
        // Test 2: pcap_dump_flush
        pcap_dump_flush(dumper);
        
        // Test 3: pcap_dump_file
        fp = pcap_dump_file(dumper);
        if (fp) {
            fflush(fp);
        }
        
        // Test 4: pcap_dump_close
        pcap_dump_close(dumper);
        dumper = NULL;
    }
    
    // Test 5: pcap_dump_fopen
    // Open a NEW file for writing. 
    // pcap_dump_fopen takes ownership of the FILE* stream.
    // We must NOT call fclose on fp after pcap_dump_fopen succeeds,
    // because pcap_dump_close will close it.
    fp = fopen(dummy_file, "wb");
    if (fp) {
        dumper = pcap_dump_fopen(handle, fp);
        if (dumper) {
            test_fopen_success = 1;
            pcap_dump_flush(dumper);
            pcap_dump_close(dumper); // This closes the underlying fp
            dumper = NULL;
        } else {
            // If pcap_dump_fopen failed, we own fp and must close it
            fclose(fp);
            fp = NULL;
        }
    }
    
    // Test 6: pcap_dump_open_append
    // Recreate the file to ensure it exists and is valid for append
    write_dummy_file(dummy_file, Data, Size);
    dumper = pcap_dump_open_append(handle, dummy_file);
    if (dumper) {
        pcap_dump_flush(dumper);
        pcap_dump_close(dumper);
        dumper = NULL;
    }
    
    // Cleanup
    if (handle) {
        pcap_close(handle);
    }
    
    cleanup_dummy_file(dummy_file);
    
    return 0;
}