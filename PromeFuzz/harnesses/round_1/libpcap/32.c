// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_dump_fopen at sf-pcap.c:988:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_dump at sf-pcap.c:819:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_flush at sf-pcap.c:1252:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open_append at sf-pcap.c:1005:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_file at sf-pcap.c:1201:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <sys/time.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

static pcap_t *create_dummy_pcap(void) {
    return pcap_open_dead(DLT_EN10MB, 65535);
}

static void write_packet(pcap_dumper_t *dumper, const uint8_t *data, size_t size) {
    struct pcap_pkthdr hdr;
    gettimeofday(&hdr.ts, NULL);
    hdr.caplen = size > 65535 ? 65535 : size;
    hdr.len = size > 65535 ? 65535 : size;
    pcap_dump((u_char *)dumper, &hdr, data);
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    pcap_t *p = create_dummy_pcap();
    if (!p) return 0;
    
    uint8_t test_case = Data[0] % 6;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (test_case) {
        case 0: {  // pcap_dump_flush
            write_dummy_file(fuzz_data, fuzz_size);
            pcap_dumper_t *dumper = pcap_dump_open(p, "./dummy_file");
            if (dumper) {
                write_packet(dumper, fuzz_data, fuzz_size);
                pcap_dump_flush(dumper);
                pcap_dump_close(dumper);
            }
            cleanup_dummy_file();
            break;
        }
        case 1: {  // pcap_dump_close
            write_dummy_file(fuzz_data, fuzz_size);
            pcap_dumper_t *dumper = pcap_dump_open(p, "./dummy_file");
            if (dumper) {
                write_packet(dumper, fuzz_data, fuzz_size);
                pcap_dump_close(dumper);
            }
            cleanup_dummy_file();
            break;
        }
        case 2: {  // pcap_dump_open_append
            write_dummy_file(fuzz_data, fuzz_size);
            pcap_dumper_t *dumper = pcap_dump_open(p, "./dummy_file");
            if (dumper) {
                pcap_dump_close(dumper);
                dumper = pcap_dump_open_append(p, "./dummy_file");
                if (dumper) {
                    write_packet(dumper, fuzz_data, fuzz_size);
                    pcap_dump_close(dumper);
                }
            }
            cleanup_dummy_file();
            break;
        }
        case 3: {  // pcap_dump_file
            write_dummy_file(fuzz_data, fuzz_size);
            pcap_dumper_t *dumper = pcap_dump_open(p, "./dummy_file");
            if (dumper) {
                FILE *fp = pcap_dump_file(dumper);
                if (fp) {
                    fflush(fp);
                }
                write_packet(dumper, fuzz_data, fuzz_size);
                pcap_dump_close(dumper);
            }
            cleanup_dummy_file();
            break;
        }
        case 4: {  // pcap_dump_open
            write_dummy_file(fuzz_data, fuzz_size);
            pcap_dumper_t *dumper = pcap_dump_open(p, "./dummy_file");
            if (dumper) {
                write_packet(dumper, fuzz_data, fuzz_size);
                pcap_dump_close(dumper);
            }
            cleanup_dummy_file();
            break;
        }
        case 5: {  // pcap_dump_fopen
            write_dummy_file(fuzz_data, fuzz_size);
            FILE *fp = fopen("./dummy_file", "wb");
            if (fp) {
                pcap_dumper_t *dumper = pcap_dump_fopen(p, fp);
                if (dumper) {
                    write_packet(dumper, fuzz_data, fuzz_size);
                    pcap_dump_flush(dumper);
                    pcap_dump_close(dumper);
                } else {
                    fclose(fp);
                }
            }
            cleanup_dummy_file();
            break;
        }
    }
    
    pcap_close(p);
    return 0;
}