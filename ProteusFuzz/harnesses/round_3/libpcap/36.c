// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_tstamp_type_val_to_description at pcap.c:3443:1 in pcap.h
// pcap_datalink_val_to_description at pcap.c:3376:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* 
 * Fuzz driver for libpcap API functions:
 * - pcap_tstamp_type_val_to_description
 * - pcap_datalink
 * - pcap_datalink_val_to_description
 * - pcap_set_datalink
 * - pcap_datalink_val_to_name
 * - pcap_datalink_name_to_val
 */

/* 
 * Helper to create a dummy pcap handle for testing pcap_datalink and pcap_set_datalink.
 * We use pcap_open_dead which creates a handle without activating it on a real interface.
 * This allows us to test pcap_datalink (which requires activation) and pcap_set_datalink.
 */
static pcap_t *create_dummy_handle(int dlt) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_dead(dlt, 65535);
    if (handle == NULL) {
        return NULL;
    }
    
    /* Activate the handle so pcap_datalink works */
    if (pcap_activate(handle) != 0) {
        pcap_close(handle);
        return NULL;
    }
    
    return handle;
}

/* 
 * Helper to extract an integer from the fuzz input data.
 * Returns a value in the range of typical pcap constants or -1 if out of bounds.
 */
static int extract_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) {
        return -1;
    }
    int val;
    memcpy(&val, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return val;
}

/* 
 * Helper to extract a null-terminated string from fuzz input.
 * Returns a newly allocated string or NULL if invalid.
 */
static char *extract_string(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) {
        return NULL;
    }
    
    size_t start = *offset;
    size_t len = 0;
    
    /* Find null terminator or end of buffer */
    while (*offset < size && data[*offset] != '\0') {
        (*offset)++;
        len++;
    }
    
    if (*offset >= size) {
        return NULL;
    }
    
    /* Skip the null terminator */
    (*offset)++;
    
    if (len == 0) {
        return NULL;
    }
    
    char *str = malloc(len + 1);
    if (str == NULL) {
        return NULL;
    }
    
    memcpy(str, data + start, len);
    str[len] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    size_t offset = 0;
    
    /* Test 1: pcap_tstamp_type_val_to_description */
    int tstamp_type = extract_int(Data, Size, &offset);
    if (tstamp_type != -1) {
        const char *desc = pcap_tstamp_type_val_to_description(tstamp_type);
        (void)desc; /* Use the result to prevent unused variable warnings */
    }
    
    /* Test 2: pcap_datalink_val_to_description */
    int dlt_val = extract_int(Data, Size, &offset);
    if (dlt_val != -1) {
        const char *dlt_desc = pcap_datalink_val_to_description(dlt_val);
        (void)dlt_desc;
    }
    
    /* Test 3: pcap_datalink_val_to_name */
    int dlt_val2 = extract_int(Data, Size, &offset);
    if (dlt_val2 != -1) {
        const char *dlt_name = pcap_datalink_val_to_name(dlt_val2);
        (void)dlt_name;
    }
    
    /* Test 4: pcap_datalink_name_to_val */
    char *name_str = extract_string(Data, Size, &offset);
    if (name_str != NULL) {
        int dlt_from_name = pcap_datalink_name_to_val(name_str);
        (void)dlt_from_name;
        free(name_str);
    }
    
    /* Test 5 & 6: pcap_datalink and pcap_set_datalink with a dummy handle */
    int dlt_for_handle = extract_int(Data, Size, &offset);
    if (dlt_for_handle == -1) {
        dlt_for_handle = 1; /* Default to DLT_EN10MB if extraction failed */
    }
    
    pcap_t *handle = create_dummy_handle(dlt_for_handle);
    if (handle != NULL) {
        /* Test pcap_datalink */
        int current_dlt = pcap_datalink(handle);
        (void)current_dlt;
        
        /* Test pcap_set_datalink with various values */
        int new_dlt = extract_int(Data, Size, &offset);
        if (new_dlt == -1) {
            new_dlt = 1; /* Default fallback */
        }
        
        int set_result = pcap_set_datalink(handle, new_dlt);
        (void)set_result;
        
        /* Try setting to another value */
        int another_dlt = extract_int(Data, Size, &offset);
        if (another_dlt != -1) {
            int set_result2 = pcap_set_datalink(handle, another_dlt);
            (void)set_result2;
        }
        
        /* Cleanup */
        pcap_close(handle);
    }
    
    return 0;
}