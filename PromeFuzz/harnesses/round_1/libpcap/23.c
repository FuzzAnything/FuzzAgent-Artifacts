// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_dump_fopen at sf-pcap.c:988:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_fopen at sf-pcap.c:988:1 in pcap.h
// pcap_file at pcap.c:3495:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static pcap_t* create_test_pcap(int is_savefile) {
    pcap_t *pcap;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    if (is_savefile) {
        pcap = pcap_open_offline("./dummy_file", errbuf);
    } else {
        pcap = pcap_open_dead(DLT_EN10MB, 65535);
    }
    
    return pcap;
}

static pcap_dumper_t* create_test_dumper(pcap_t *pcap) {
    if (!pcap) return NULL;
    
    pcap_dumper_t *dumper = pcap_dump_open(pcap, "./dummy_dump");
    if (!dumper) {
        dumper = pcap_dump_fopen(pcap, stdout);
    }
    
    return dumper;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    int test_case = Data[0] % 6;
    
    switch (test_case) {
        case 0: {
            pcap_t *pcap = create_test_pcap(1);
            if (pcap) {
                FILE *fp = pcap_file(pcap);
                if (fp) {
                    fflush(fp);
                }
                pcap_close(pcap);
            }
            break;
        }
        
        case 1: {
            pcap_t *pcap = create_test_pcap(0);
            pcap_dumper_t *dumper = create_test_dumper(pcap);
            if (dumper) {
                pcap_dump_close(dumper);
            }
            if (pcap) pcap_close(pcap);
            break;
        }
        
        case 2: {
            pcap_t *pcap = create_test_pcap(0);
            if (pcap) {
                pcap_dumper_t *dumper = pcap_dump_open_append(pcap, "./dummy_file");
                if (dumper) {
                    pcap_dump_close(dumper);
                }
                pcap_close(pcap);
            }
            break;
        }
        
        case 3: {
            pcap_t *pcap = create_test_pcap(0);
            pcap_dumper_t *dumper = create_test_dumper(pcap);
            if (dumper) {
                FILE *fp = pcap_dump_file(dumper);
                if (fp) {
                    fflush(fp);
                }
                pcap_dump_close(dumper);
            }
            if (pcap) pcap_close(pcap);
            break;
        }
        
        case 4: {
            pcap_t *pcap = create_test_pcap(0);
            if (pcap) {
                pcap_dumper_t *dumper = pcap_dump_open(pcap, "./dummy_output");
                if (dumper) {
                    pcap_dump_close(dumper);
                }
                pcap_close(pcap);
            }
            break;
        }
        
        case 5: {
            pcap_t *pcap = create_test_pcap(0);
            if (pcap) {
                FILE *fp = fopen("./dummy_file", "rb");
                if (fp) {
                    pcap_dumper_t *dumper = pcap_dump_fopen(pcap, fp);
                    if (dumper) {
                        pcap_dump_close(dumper);
                    }
                    /* Do NOT close fp here - pcap_dump_fopen or pcap_dump_close handles it */
                }
                pcap_close(pcap);
            }
            break;
        }
    }
    
    unlink("./dummy_file");
    unlink("./dummy_dump");
    unlink("./dummy_output");
    
    return 0;
}