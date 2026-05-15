// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_ftell64 at sf-pcap.c:1220:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_ftell64 at sf-pcap.c:1220:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
// pcap_dump_ftell64 at sf-pcap.c:1220:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_fopen at sf-pcap.c:988:1 in pcap.h
// pcap_dump_ftell at sf-pcap.c:1207:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_fopen at sf-pcap.c:988:1 in pcap.h
// pcap_dump_ftell64 at sf-pcap.c:1220:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (size > 0) {
            fwrite(data, 1, size, fp);
        }
        fclose(fp);
    }
}

static pcap_t* create_dummy_pcap(void) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *p = pcap_open_dead(DLT_EN10MB, 65535);
    if (!p) {
        p = pcap_open_dead(DLT_RAW, 65535);
    }
    return p;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    pcap_t *pcap = create_dummy_pcap();
    if (!pcap) return 0;
    
    uint8_t mode = Data[0] % 8;
    const char *filename = "./dummy_file";
    
    switch (mode) {
        case 0: {
            pcap_dumper_t *dumper = pcap_dump_open(pcap, filename);
            if (dumper) {
                long pos = pcap_dump_ftell(dumper);
                (void)pos;
                pcap_dump_close(dumper);
            }
            break;
        }
        case 1: {
            pcap_dumper_t *dumper = pcap_dump_open(pcap, filename);
            if (dumper) {
                int64_t pos64 = pcap_dump_ftell64(dumper);
                (void)pos64;
                pcap_dump_close(dumper);
            }
            break;
        }
        case 2: {
            pcap_dumper_t *dumper = pcap_dump_open_append(pcap, filename);
            if (dumper) {
                long pos = pcap_dump_ftell(dumper);
                (void)pos;
                pcap_dump_close(dumper);
            }
            break;
        }
        case 3: {
            pcap_dumper_t *dumper = pcap_dump_open_append(pcap, filename);
            if (dumper) {
                int64_t pos64 = pcap_dump_ftell64(dumper);
                (void)pos64;
                pcap_dump_close(dumper);
            }
            break;
        }
        case 4: {
            FILE *fp = fopen(filename, "wb");
            if (fp) {
                pcap_dumper_t *dumper = pcap_dump_fopen(pcap, fp);
                if (dumper) {
                    long pos = pcap_dump_ftell(dumper);
                    (void)pos;
                    pcap_dump_close(dumper);
                } else {
                    fclose(fp);
                }
            }
            break;
        }
        case 5: {
            FILE *fp = fopen(filename, "wb");
            if (fp) {
                pcap_dumper_t *dumper = pcap_dump_fopen(pcap, fp);
                if (dumper) {
                    int64_t pos64 = pcap_dump_ftell64(dumper);
                    (void)pos64;
                    pcap_dump_close(dumper);
                } else {
                    fclose(fp);
                }
            }
            break;
        }
        case 6: {
            pcap_dumper_t *dumper1 = pcap_dump_open(pcap, filename);
            if (dumper1) {
                pcap_dump_close(dumper1);
                
                pcap_dumper_t *dumper2 = pcap_dump_open_append(pcap, filename);
                if (dumper2) {
                    pcap_dump_close(dumper2);
                }
            }
            break;
        }
        case 7: {
            pcap_dumper_t *dumper = pcap_dump_open(pcap, filename);
            if (dumper) {
                long pos1 = pcap_dump_ftell(dumper);
                int64_t pos2 = pcap_dump_ftell64(dumper);
                (void)pos1;
                (void)pos2;
                pcap_dump_close(dumper);
            }
            break;
        }
    }
    
    pcap_close(pcap);
    return 0;
}