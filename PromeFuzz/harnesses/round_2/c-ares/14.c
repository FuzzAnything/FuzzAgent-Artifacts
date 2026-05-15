// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_rr_set_u32 at ares_dns_record.c:1176:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static uint8_t read_byte(const uint8_t* data, size_t size, size_t* offset) {
    if (size == 0) {
        return 0;
    }
    uint8_t byte = data[*offset % size];
    (*offset)++;
    return byte;
}

static const char* get_string_from_input(const uint8_t* data, size_t size, size_t* offset, size_t max_len) {
    if (size == 0) {
        return "";
    }
    uint8_t len_byte = read_byte(data, size, offset);
    size_t len = len_byte % (max_len + 1);
    static char str_buf[101];
    for (size_t i = 0; i < len; i++) {
        str_buf[i] = (char)read_byte(data, size, offset);
    }
    str_buf[len] = '\0';
    return str_buf;
}

static const unsigned char* get_bin_from_input(const uint8_t* data, size_t size, size_t* offset, size_t max_len, size_t* bin_len) {
    if (size == 0) {
        *bin_len = 0;
        return NULL;
    }
    uint8_t len_byte = read_byte(data, size, offset);
    *bin_len = len_byte % (max_len + 1);
    static unsigned char bin_buf[100];
    for (size_t i = 0; i < *bin_len; i++) {
        bin_buf[i] = read_byte(data, size, offset);
    }
    return bin_buf;
}

int LLVMFuzzerTestOneInput_14(const uint8_t* Data, size_t Size) {
    size_t offset = 0;
    
    if (Size == 0) {
        return 0;
    }
    
    // Read parameters for ares_dns_record_create
    unsigned short id = (read_byte(Data, Size, &offset) << 8) | read_byte(Data, Size, &offset);
    unsigned short flags = (read_byte(Data, Size, &offset) << 8) | read_byte(Data, Size, &offset);
    ares_dns_opcode_t opcode = (ares_dns_opcode_t)read_byte(Data, Size, &offset);
    ares_dns_rcode_t rcode = (ares_dns_rcode_t)read_byte(Data, Size, &offset);
    
    // Create DNS record
    ares_dns_record_t* dnsrec = NULL;
    ares_status_t create_status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (create_status != ARES_SUCCESS || !dnsrec) {
        return 0;
    }
    
    // Prepare RRs
    ares_dns_rr_t* rr1 = NULL;
    ares_dns_rr_t* rr2 = NULL;
    ares_dns_rr_t* rr3 = NULL;
    
    // Get parameters from fuzzing input
    ares_dns_section_t sect1 = (ares_dns_section_t)(read_byte(Data, Size, &offset) % 4);
    const char* name1 = get_string_from_input(Data, Size, &offset, 100);
    ares_dns_rec_type_t type1 = (ares_dns_rec_type_t)read_byte(Data, Size, &offset);
    ares_dns_class_t class1 = (ares_dns_class_t)read_byte(Data, Size, &offset);
    unsigned int ttl1 = read_byte(Data, Size, &offset);
    
    // Function 2: ares_dns_record_rr_add (create rr1)
    ares_status_t status = ares_dns_record_rr_add(&rr1, dnsrec, sect1, name1, type1, class1, ttl1);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    // Function 1: ares_dns_rr_set_bin (on rr1)
    ares_dns_rr_key_t key1 = (ares_dns_rr_key_t)read_byte(Data, Size, &offset);
    size_t bin_len = 0;
    const unsigned char* bin_data = get_bin_from_input(Data, Size, &offset, 100, &bin_len);
    ares_dns_rr_set_bin(rr1, key1, bin_data, bin_len);
    
    // Function 3: ares_dns_rr_set_str (on rr1)
    ares_dns_rr_key_t key2 = (ares_dns_rr_key_t)read_byte(Data, Size, &offset);
    const char* str1 = get_string_from_input(Data, Size, &offset, 100);
    ares_dns_rr_set_str(rr1, key2, str1);
    
    // Function 4: ares_dns_record_rr_add (create rr2)
    ares_dns_section_t sect2 = (ares_dns_section_t)(read_byte(Data, Size, &offset) % 4);
    const char* name2 = get_string_from_input(Data, Size, &offset, 100);
    ares_dns_rec_type_t type2 = (ares_dns_rec_type_t)read_byte(Data, Size, &offset);
    ares_dns_class_t class2 = (ares_dns_class_t)read_byte(Data, Size, &offset);
    unsigned int ttl2 = read_byte(Data, Size, &offset);
    
    status = ares_dns_record_rr_add(&rr2, dnsrec, sect2, name2, type2, class2, ttl2);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    // Function 5: ares_dns_rr_set_str (on rr2)
    ares_dns_rr_key_t key3 = (ares_dns_rr_key_t)read_byte(Data, Size, &offset);
    const char* str2 = get_string_from_input(Data, Size, &offset, 100);
    ares_dns_rr_set_str(rr2, key3, str2);
    
    // Function 6: ares_dns_record_rr_add (create rr3)
    ares_dns_section_t sect3 = (ares_dns_section_t)(read_byte(Data, Size, &offset) % 4);
    const char* name3 = get_string_from_input(Data, Size, &offset, 100);
    ares_dns_rec_type_t type3 = (ares_dns_rec_type_t)read_byte(Data, Size, &offset);
    ares_dns_class_t class3 = (ares_dns_class_t)read_byte(Data, Size, &offset);
    unsigned int ttl3 = read_byte(Data, Size, &offset);
    
    status = ares_dns_record_rr_add(&rr3, dnsrec, sect3, name3, type3, class3, ttl3);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    // Function 7: ares_dns_rr_set_str (on rr3)
    ares_dns_rr_key_t key4 = (ares_dns_rr_key_t)read_byte(Data, Size, &offset);
    const char* str3 = get_string_from_input(Data, Size, &offset, 100);
    ares_dns_rr_set_str(rr3, key4, str3);
    
    // Function 8: ares_dns_rr_set_str (on rr3 again)
    ares_dns_rr_key_t key5 = (ares_dns_rr_key_t)read_byte(Data, Size, &offset);
    const char* str4 = get_string_from_input(Data, Size, &offset, 100);
    ares_dns_rr_set_str(rr3, key5, str4);
    
    // Function 9: ares_dns_rr_set_u32 (on rr3)
    ares_dns_rr_key_t key6 = (ares_dns_rr_key_t)read_byte(Data, Size, &offset);
    unsigned int val = read_byte(Data, Size, &offset);
    ares_dns_rr_set_u32(rr3, key6, val);
    
    // Cleanup
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}