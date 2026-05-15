// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_ftell64 at sf-pcap.c:1220:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_fopen at sf-pcap.c:988:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_ftell64 at sf-pcap.c:1220:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_ftell64 at sf-pcap.c:1220:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <time.h>

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);
    return written == size;
}

static void setup_pcap_header(FILE *fp, int linktype) {
    struct pcap_file_header hdr;
    hdr.magic = 0xa1b2c3d4;
    hdr.version_major = 2;
    hdr.version_minor = 4;
    hdr.thiszone = 0;
    hdr.sigfigs = 0;
    hdr.snaplen = 65535;
    hdr.linktype = linktype;
    
    fwrite(&hdr, sizeof(hdr), 1, fp);
}

static pcap_t *create_pcap_dead(int linktype, int snaplen) {
    return pcap_open_dead(linktype, snaplen);
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }
    
    cleanup_dummy_file();
    
    int linktype = Data[0] % 256;
    int snaplen = (Data[1] % 1024) + 1;
    size_t data_offset = 2;
    
    pcap_t *pcap = create_pcap_dead(linktype, snaplen);
    if (!pcap) {
        return 0;
    }
    
    pcap_dumper_t *dumper = NULL;
    
    if (Size >= data_offset + 16) {
        write_dummy_file(Data + data_offset, Size - data_offset);
        
        dumper = pcap_dump_open(pcap, "./dummy_file");
        if (dumper) {
            pcap_dump_ftell(dumper);
            pcap_dump_ftell64(dumper);
            pcap_dump_close(dumper);
            dumper = NULL;
        }
        
        FILE *fp = fopen("./dummy_file", "wb+");
        if (fp) {
            setup_pcap_header(fp, linktype);
            if (Size - data_offset > 0) {
                fwrite(Data + data_offset, 1, Size - data_offset, fp);
            }
            fflush(fp);
            rewind(fp);
            
            dumper = pcap_dump_fopen(pcap, fp);
            if (dumper) {
                pcap_dump_ftell(dumper);
                pcap_dump_ftell64(dumper);
                pcap_dump_close(dumper);
                dumper = NULL;
            } else {
                fclose(fp);
            }
        }
        
        FILE *append_fp = fopen("./dummy_file", "wb");
        if (append_fp) {
            setup_pcap_header(append_fp, linktype);
            fclose(append_fp);
            
            dumper = pcap_dump_open_append(pcap, "./dummy_file");
            if (dumper) {
                pcap_dump_ftell(dumper);
                pcap_dump_ftell64(dumper);
                pcap_dump_close(dumper);
            }
            dumper = NULL;
        }
    }
    
    pcap_close(pcap);
    cleanup_dummy_file();
    
    return 0;
}