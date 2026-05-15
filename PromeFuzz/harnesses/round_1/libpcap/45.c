// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_free_tstamp_types at pcap.c:534:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

static pcap_t* create_dummy_pcap(void) {
    return pcap_open_dead(DLT_EN10MB, 65535);
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (selector % 6) {
        case 0: {
            int dlt_val = 0;
            if (fuzz_size >= sizeof(int)) {
                dlt_val = *(const int*)fuzz_data;
            } else if (fuzz_size > 0) {
                dlt_val = (int)fuzz_data[0];
            }
            const char *name = pcap_datalink_val_to_name(dlt_val);
            (void)name;
            break;
        }
        
        case 1: {
            if (fuzz_size > 0) {
                size_t array_size = fuzz_size / sizeof(int) + 1;
                if (array_size > 0 && array_size <= 1024) {
                    int *dlt_list = malloc(sizeof(int) * array_size);
                    if (dlt_list) {
                        size_t copy_len = fuzz_size < array_size ? fuzz_size : array_size;
                        for (size_t i = 0; i < copy_len; i++) {
                            dlt_list[i] = fuzz_data[i];
                        }
                        pcap_free_datalinks(dlt_list);
                    }
                }
            }
            break;
        }
        
        case 2: {
            create_dummy_file(fuzz_data, fuzz_size);
            char errbuf[PCAP_ERRBUF_SIZE];
            pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
            if (pcap) {
                int *dlt_list = NULL;
                int count = pcap_list_datalinks(pcap, &dlt_list);
                if (count > 0 && dlt_list) {
                    pcap_free_datalinks(dlt_list);
                }
                pcap_close(pcap);
            }
            remove_dummy_file();
            break;
        }
        
        case 3: {
            if (fuzz_size > 0) {
                size_t array_size = fuzz_size / sizeof(int) + 1;
                if (array_size > 0 && array_size <= 1024) {
                    int *tstamp_types = malloc(sizeof(int) * array_size);
                    if (tstamp_types) {
                        size_t copy_len = fuzz_size < array_size ? fuzz_size : array_size;
                        for (size_t i = 0; i < copy_len; i++) {
                            tstamp_types[i] = fuzz_data[i];
                        }
                        pcap_free_tstamp_types(tstamp_types);
                    }
                }
            }
            break;
        }
        
        case 4: {
            if (fuzz_size > 0) {
                char *name = malloc(fuzz_size + 1);
                if (name) {
                    memcpy(name, fuzz_data, fuzz_size);
                    name[fuzz_size] = '\0';
                    int val = pcap_datalink_name_to_val(name);
                    (void)val;
                    free(name);
                }
            }
            break;
        }
        
        case 5: {
            pcap_t *pcap = create_dummy_pcap();
            if (pcap) {
                int dlt_val = 0;
                if (fuzz_size >= sizeof(int)) {
                    dlt_val = *(const int*)fuzz_data;
                } else if (fuzz_size > 0) {
                    dlt_val = (int)fuzz_data[0];
                }
                int result = pcap_set_datalink(pcap, dlt_val);
                (void)result;
                pcap_close(pcap);
            }
            break;
        }
    }
    
    return 0;
}