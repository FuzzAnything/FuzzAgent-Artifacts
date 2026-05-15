// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_get_opt at ares_dns_record.c:1016:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_del_opt_byid at ares_dns_record.c:1428:15 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include ares.h first to get CARES_EXTERN definition */
#include "ares.h"
/* Then include the DNS record header */
#include "ares_dns_record.h"

static void dummy_cleanup(void) {
    // Cleanup function if needed
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    // Initialize a DNS record for testing
    ares_dns_record_t *dnsrec = NULL;
    
    // Use input data to set parameters for ares_dns_record_create
    unsigned short id = 0;
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    if (Size >= 2) {
        id = (unsigned short)((Data[0] << 8) | Data[1]);
    }
    if (Size >= 4) {
        flags = (unsigned short)((Data[2] << 8) | Data[3]);
    }
    if (Size >= 5) {
        opcode = (ares_dns_opcode_t)(Data[4] % 16); // Opcode values 0-15
    }
    if (Size >= 6) {
        rcode = (ares_dns_rcode_t)(Data[5] % 16); // Rcode values 0-15
    }
    
    ares_status_t create_status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (create_status != ARES_SUCCESS || !dnsrec) {
        return 0;
    }

    // Test ares_dns_record_rr_get with various sections and indices
    // Use input data to determine section and index
    if (Size >= 8) {
        ares_dns_section_t sect = (ares_dns_section_t)(Data[6] % 4); // 4 sections: QD, AN, NS, AR
        size_t idx = Data[7] % 16; // Try indices up to 16
        
        ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, sect, idx);
        // rr may be NULL if invalid parameters - that's expected
    }

    // Test ares_dns_rr_key_tostr with various keys
    if (Size >= 9) {
        ares_dns_rr_key_t key = (ares_dns_rr_key_t)(Data[8] % 1000); // Try various key values
        const char *str = ares_dns_rr_key_tostr(key);
        // str may be NULL for unknown keys - that's expected
    }

    // Create a resource record to test other functions
    // First, try to get an existing RR or create a dummy one
    ares_dns_rr_t *test_rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, 0);
    if (!test_rr) {
        // If no RR exists, we can't test the RR-specific functions
        ares_dns_record_destroy(dnsrec);
        return 0;
    }

    // Test ares_dns_rr_set_u16
    if (Size >= 12) {
        ares_dns_rr_key_t set_key = (ares_dns_rr_key_t)(Data[9] % 1000);
        unsigned short val = (unsigned short)((Data[10] << 8) | Data[11]);
        ares_status_t status = ares_dns_rr_set_u16(test_rr, set_key, val);
        // status may be error if key is not U16 type - that's expected
    }

    // Test ares_dns_rr_del_opt_byid
    if (Size >= 14) {
        ares_dns_rr_key_t del_key = (ares_dns_rr_key_t)(Data[12] % 1000);
        unsigned short opt_id = (unsigned short)((Data[13] << 8) | Data[13 % Size]);
        ares_status_t del_status = ares_dns_rr_del_opt_byid(test_rr, del_key, opt_id);
        // del_status may be ARES_ENOTFOUND - that's expected
    }

    // Test ares_dns_rr_set_opt
    if (Size >= 18) {
        ares_dns_rr_key_t set_opt_key = (ares_dns_rr_key_t)(Data[14] % 1000);
        unsigned short opt = (unsigned short)((Data[15] << 8) | Data[16]);
        
        // Use remaining data for option value
        size_t val_len = Size - 17;
        if (val_len > 0) {
            const unsigned char *val = Data + 17;
            ares_status_t set_opt_status = ares_dns_rr_set_opt(test_rr, set_opt_key, opt, val, val_len);
            // set_opt_status may be error - that's expected
        }
    }

    // Test ares_dns_rr_get_opt
    if (Size >= 20) {
        ares_dns_rr_key_t get_opt_key = (ares_dns_rr_key_t)(Data[18] % 1000);
        size_t opt_idx = Data[19 % Size] % 8; // Try indices up to 8
        
        const unsigned char *opt_val = NULL;
        size_t opt_val_len = 0;
        unsigned short opt_result = ares_dns_rr_get_opt(test_rr, get_opt_key, opt_idx, &opt_val, &opt_val_len);
        // opt_result may be 65535 on error - that's expected
    }

    // Cleanup
    ares_dns_record_destroy(dnsrec);
    dummy_cleanup();
    
    return 0;
}