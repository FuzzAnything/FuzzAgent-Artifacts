// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_dump_fopen at sf-pcap.c:988:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

static void cleanup_dumper(pcap_dumper_t *dumper) {
    if (dumper != NULL) {
        pcap_dump_close(dumper);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = NULL;
    pcap_dumper_t *dumper = NULL;
    FILE *fp = NULL;
    
    if (Size < 2) {
        return 0;
    }

    // Use first byte to decide which API to test
    int mode = Data[0] % 6;
    
    // Create a dummy savefile for pcap_open_offline
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        // Write minimal pcap header (magic + version)
        uint32_t magic = 0xa1b2c3d4;
        uint16_t version_major = 2;
        uint16_t version_minor = 4;
        uint32_t thiszone = 0;
        uint32_t sigfigs = 0;
        uint32_t snaplen = 65535;
        uint32_t network = 1; // LINKTYPE_ETHERNET
        
        fwrite(&magic, sizeof(magic), 1, f);
        fwrite(&version_major, sizeof(version_major), 1, f);
        fwrite(&version_minor, sizeof(version_minor), 1, f);
        fwrite(&thiszone, sizeof(thiszone), 1, f);
        fwrite(&sigfigs, sizeof(sigfigs), 1, f);
        fwrite(&snaplen, sizeof(snaplen), 1, f);
        fwrite(&network, sizeof(network), 1, f);
        // Write a dummy packet header
        struct pcap_pkthdr hdr;
        memset(&hdr, 0, sizeof(hdr));
        hdr.caplen = 0;
        hdr.len = 0;
        fwrite(&hdr, sizeof(hdr), 1, f);
        fclose(f);
    }

    // Try to open a pcap handle
    pcap_handle = pcap_open_offline(dummy_file, errbuf);
    if (pcap_handle == NULL) {
        // If we can't open the file, try to create a live handle on a dummy device (might fail, but safe)
        pcap_handle = pcap_open_live("lo", 65535, 1, 1000, errbuf);
        if (pcap_handle == NULL) {
            return 0;
        }
    }

    switch (mode) {
        case 0: // pcap_dump_open
        {
            const char *filename = (Size > 10) ? (const char *)(Data + 1) : "./test_dump.pcap";
            dumper = pcap_dump_open(pcap_handle, filename);
            if (dumper != NULL) {
                long pos = pcap_dump_ftell(dumper);
                (void)pos; // Use return value
                FILE *fp_dump = pcap_dump_file(dumper);
                if (fp_dump != NULL) {
                    // Just ensure file handle is valid
                }
                cleanup_dumper(dumper);
            }
            break;
        }
        case 1: // pcap_dump_open_append
        {
            const char *filename = (Size > 10) ? (const char *)(Data + 1) : "./test_append.pcap";
            dumper = pcap_dump_open_append(pcap_handle, filename);
            if (dumper != NULL) {
                long pos = pcap_dump_ftell(dumper);
                (void)pos;
                cleanup_dumper(dumper);
            }
            break;
        }
        case 2: // pcap_dump_fopen
        {
            fp = fopen("./test_fopen.pcap", "w");
            if (fp != NULL) {
                dumper = pcap_dump_fopen(pcap_handle, fp);
                if (dumper != NULL) {
                    long pos = pcap_dump_ftell(dumper);
                    (void)pos;
                    cleanup_dumper(dumper);
                    // pcap_dump_close closes the underlying FILE* if it was opened by libpcap,
                    // but for pcap_dump_fopen, the ownership of FILE* usually remains with the caller
                    // or is explicitly handled. However, to be safe and avoid double close if libpcap
                    // took ownership, we check if the dumper was valid. 
                    // In standard libpcap, pcap_dump_fopen does NOT close the FILE* on dump_close 
                    // unless it was opened internally, but behavior can vary. 
                    // The safest approach for fuzzing when using pcap_dump_fopen is to NOT fclose 
                    // if the dumper was successfully created, as the dumper might have taken ownership 
                    // or we might be in a state where closing is handled by the dumper.
                    // Actually, standard libpcap documentation says: "If the file was opened by 
                    // pcap_dump_fopen(), the caller is responsible for closing it." 
                    // BUT, if the dumper failed, we must close. If it succeeded, we close it ourselves.
                    // The error log suggests double free, meaning pcap_dump_close called fclose on fp.
                    // This happens if the dumper took ownership. Let's assume if dumper != NULL, 
                    // we let pcap_dump_close handle it, and we do NOT fclose here.
                    // If dumper == NULL, we MUST fclose.
                } else {
                    // dumper creation failed, we own the file
                    fclose(fp);
                    fp = NULL;
                }
            }
            break;
        }
        case 3: // pcap_dump_ftell on NULL or invalid
        {
            // Test with NULL
            long pos = pcap_dump_ftell(NULL);
            (void)pos;
            
            // Test with a valid dumper created via pcap_dump_open
            dumper = pcap_dump_open(pcap_handle, "./test_ftell.pcap");
            if (dumper != NULL) {
                pos = pcap_dump_ftell(dumper);
                (void)pos;
                cleanup_dumper(dumper);
            }
            break;
        }
        case 4: // pcap_dump_file
        {
            dumper = pcap_dump_open(pcap_handle, "./test_file.pcap");
            if (dumper != NULL) {
                FILE *fp_dump = pcap_dump_file(dumper);
                if (fp_dump != NULL) {
                    // Do nothing, just retrieve
                }
                cleanup_dumper(dumper);
            }
            break;
        }
        case 5: // pcap_dump_close
        {
            dumper = pcap_dump_open(pcap_handle, "./test_close.pcap");
            if (dumper != NULL) {
                pcap_dump_close(dumper);
                dumper = NULL; // Ensure we don't double close
            }
            // Do NOT call pcap_dump_close(NULL) as it may cause internal issues in some libpcap versions
            break;
        }
        default:
            break;
    }

    cleanup_pcap(pcap_handle);
    
    // Cleanup dummy files
    remove(dummy_file);
    remove("./test_dump.pcap");
    remove("./test_append.pcap");
    remove("./test_fopen.pcap");
    remove("./test_ftell.pcap");
    remove("./test_file.pcap");
    remove("./test_close.pcap");

    return 0;
}