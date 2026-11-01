// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_get_tstamp_precision at pcap.c:2689:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_tstamp_type_val_to_description at pcap.c:3443:1 in pcap.h
// pcap_tstamp_type_val_to_name at pcap.c:3431:1 in pcap.h
// pcap_tstamp_type_name_to_val at pcap.c:3419:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_list_tstamp_types at pcap.c:492:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_get_tstamp_precision at pcap.c:2689:1 in pcap.h
// pcap_free_tstamp_types at pcap.c:534:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_get_tstamp_precision at pcap.c:2689:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int *tstamp_types = NULL;
    int ret;
    int tstamp_type;
    int precision;
    const char *desc;
    const char *name;
    int val;

    // 1. Fuzz pcap_tstamp_type_val_to_description
    if (Size >= sizeof(int)) {
        memcpy(&tstamp_type, Data, sizeof(int));
        desc = pcap_tstamp_type_val_to_description(tstamp_type);
        (void)desc; 
    }

    // 2. Fuzz pcap_tstamp_type_val_to_name
    if (Size >= sizeof(int)) {
        memcpy(&tstamp_type, Data, sizeof(int));
        name = pcap_tstamp_type_val_to_name(tstamp_type);
        (void)name;
    }

    // 3. Fuzz pcap_tstamp_type_name_to_val
    if (Size > 0) {
        char *str_buf = (char *)malloc(Size + 1);
        if (str_buf) {
            memcpy(str_buf, Data, Size);
            str_buf[Size] = '\0';
            for (size_t i = 0; i < Size; i++) {
                if (str_buf[i] == '\0') {
                    str_buf[i] = ' ';
                }
            }
            val = pcap_tstamp_type_name_to_val(str_buf);
            (void)val;
            free(str_buf);
        }
    }

    // 4. Fuzz pcap_list_tstamp_types and pcap_free_tstamp_types
    write_dummy_file(Data, Size);
    handle = pcap_open_offline("./dummy_file", errbuf);
    
    if (handle) {
        // pcap_list_tstamp_types expects (pcap_t *, int **)
        ret = pcap_list_tstamp_types(handle, &tstamp_types);
        
        if (tstamp_types) {
            // Fuzz pcap_set_tstamp_type with various values from input
            if (Size >= sizeof(int)) {
                memcpy(&tstamp_type, Data, sizeof(int));
                ret = pcap_set_tstamp_type(handle, tstamp_type);
                (void)ret;
            }
            
            // Fuzz pcap_get_tstamp_precision
            precision = pcap_get_tstamp_precision(handle);
            (void)precision;

            // Cleanup timestamp types
            pcap_free_tstamp_types(tstamp_types);
            tstamp_types = NULL;
        } else {
            // If list is NULL, we cannot free it, so skip free
            // But we can still try to set/get on the handle if it's valid
            if (Size >= sizeof(int)) {
                memcpy(&tstamp_type, Data, sizeof(int));
                ret = pcap_set_tstamp_type(handle, tstamp_type);
                (void)ret;
            }
            precision = pcap_get_tstamp_precision(handle);
            (void)precision;
        }
        
        pcap_close(handle);
        handle = NULL;
    }

    // 5. Additional state exploration: Try to open a non-existent file
    handle = pcap_open_offline("./non_existent_file", errbuf);
    if (handle) {
        if (Size >= sizeof(int)) {
            memcpy(&tstamp_type, Data, sizeof(int));
            ret = pcap_set_tstamp_type(handle, tstamp_type);
            (void)ret;
        }
        precision = pcap_get_tstamp_precision(handle);
        (void)precision;
        pcap_close(handle);
    }

    // 6. Cleanup dummy file
    remove("./dummy_file");

    return 0;
}