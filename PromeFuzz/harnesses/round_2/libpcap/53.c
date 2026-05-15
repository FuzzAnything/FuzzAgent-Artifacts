// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_tstamp_type_val_to_description at pcap.c:3443:1 in pcap.h
// pcap_datalink_val_to_description at pcap.c:3376:1 in pcap.h
// pcap_datalink_val_to_description_or_dlt at pcap.c:3388:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_timestamp_type(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int tstamp_type;
    memcpy(&tstamp_type, data, sizeof(int));
    const char *desc = pcap_tstamp_type_val_to_description(tstamp_type);
    (void)desc;
}

static void fuzz_datalink_val_to_description(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int dlt_val;
    memcpy(&dlt_val, data, sizeof(int));
    const char *desc = pcap_datalink_val_to_description(dlt_val);
    (void)desc;
}

static void fuzz_datalink_val_to_description_or_dlt(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int dlt_val;
    memcpy(&dlt_val, data, sizeof(int));
    const char *desc = pcap_datalink_val_to_description_or_dlt(dlt_val);
    (void)desc;
}

static void fuzz_free_datalinks(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int count;
    memcpy(&count, data, sizeof(int));
    if (count <= 0 || count > 100) return;
    
    size_t array_size = count * sizeof(int);
    if (size < sizeof(int) + array_size) return;
    
    int *dlt_array = (int *)malloc(array_size);
    if (!dlt_array) return;
    
    memcpy(dlt_array, data + sizeof(int), array_size);
    pcap_free_datalinks(dlt_array);
}

static void fuzz_datalink_val_to_name(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int dlt_val;
    memcpy(&dlt_val, data, sizeof(int));
    const char *name = pcap_datalink_val_to_name(dlt_val);
    (void)name;
}

static void fuzz_datalink_name_to_val(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *name = (char *)malloc(size + 1);
    if (!name) return;
    
    memcpy(name, data, size);
    name[size] = '\0';
    
    int dlt_val = pcap_datalink_name_to_val(name);
    (void)dlt_val;
    
    free(name);
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_timestamp_type(Data, Size);
    fuzz_datalink_val_to_description(Data, Size);
    fuzz_datalink_val_to_description_or_dlt(Data, Size);
    fuzz_free_datalinks(Data, Size);
    fuzz_datalink_val_to_name(Data, Size);
    fuzz_datalink_name_to_val(Data, Size);
    
    return 0;
}