// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_tstamp_type_name_to_val at pcap.c:3419:1 in pcap.h
// pcap_tstamp_type_val_to_name at pcap.c:3431:1 in pcap.h
// pcap_list_tstamp_types at pcap.c:492:1 in pcap.h
// pcap_tstamp_type_val_to_name at pcap.c:3431:1 in pcap.h
// pcap_tstamp_type_val_to_description at pcap.c:3443:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_free_tstamp_types at pcap.c:534:1 in pcap.h
// pcap_tstamp_type_val_to_description at pcap.c:3443:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap) {
        pcap_close(pcap);
    }
}

static pcap_t *create_dummy_pcap(void) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (!pcap) {
        pcap = pcap_open_dead(DLT_RAW, 65535);
    }
    return pcap;
}

static int parse_int_from_data(const uint8_t *data, size_t size, size_t *offset) {
    int value = 0;
    if (*offset < size) {
        value = data[*offset];
        (*offset)++;
    }
    return value;
}

static void extract_string(const uint8_t *data, size_t size, char *buffer, size_t buffer_size) {
    if (buffer_size == 0) return;
    
    size_t copy_len = size;
    if (copy_len > buffer_size - 1) {
        copy_len = buffer_size - 1;
    }
    
    if (copy_len > 0) {
        memcpy(buffer, data, copy_len);
    }
    buffer[copy_len] = '\0';
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    size_t offset = 0;
    
    char tstamp_name[256];
    extract_string(Data, Size, tstamp_name, sizeof(tstamp_name));
    
    int tstamp_val = parse_int_from_data(Data, Size, &offset);
    
    pcap_t *pcap = create_dummy_pcap();
    if (!pcap) {
        return 0;
    }

    int result;
    const char *str_result;
    int *tstamp_types = NULL;
    int tstamp_count;

    result = pcap_tstamp_type_name_to_val(tstamp_name);

    str_result = pcap_tstamp_type_val_to_name(tstamp_val);

    tstamp_count = pcap_list_tstamp_types(pcap, &tstamp_types);
    if (tstamp_count > 0 && tstamp_types) {
        for (int i = 0; i < tstamp_count && i < 5; i++) {
            int current_val = tstamp_types[i];
            
            str_result = pcap_tstamp_type_val_to_name(current_val);
            
            str_result = pcap_tstamp_type_val_to_description(current_val);
            
            if (Size > 1) {
                int test_val = Data[i % Size];
                result = pcap_set_tstamp_type(pcap, test_val);
            }
        }
        pcap_free_tstamp_types(tstamp_types);
    }

    str_result = pcap_tstamp_type_val_to_description(tstamp_val);

    if (Size > 2) {
        int set_val = Data[1];
        result = pcap_set_tstamp_type(pcap, set_val);
    }

    cleanup_pcap(pcap);
    return 0;
}