#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ares.h"
#include "ares_dns_record.h"

namespace harness_012 {
/* Comprehensive fuzzing harness for c-ares library targeting DNS mapping string
 * conversion functions identified as highest priority coverage gap:
 * - ares_dns_rr_key_tostr: String representation of DNS Resource Record parameters
 * - ares_dns_opt_get_datatype: Get datatype for DNS option/parameter
 * - ares_dns_opt_get_name: Get name for DNS option/parameter
 * 
 * Target DNS option handling and mapping functions with 134+ undiscovered branches.
 * Ensure semantic diversity from existing 12 harnesses which cover various areas
 * including DNS record binary data manipulation, socket callbacks, file-based
 * host resolution, etc.
 * 
 * This harness specifically focuses on DNS mapping functions by testing
 * various enum values and option parameters.
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    /* Minimum size check */
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    /* Initialize c-ares library */
    ares_library_init(ARES_LIB_INIT_ALL);

    /* Consume operation type */
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;

    switch (operation) {
        case 0: {
            /* Test ares_dns_rr_key_tostr with various RR keys */
            for (int i = 0; i < 100; i++) {
                ares_dns_rr_key_t key = static_cast<ares_dns_rr_key_t>(i);
                const char *str = ares_dns_rr_key_tostr(key);
                (void)str; /* Use result to prevent optimization */
            }
            break;
        }

        case 1: {
            /* Test ares_dns_opt_get_datatype with various keys and options */
            for (unsigned short opt = 0; opt < 256; opt++) {
                /* Test with OPT record key */
                ares_dns_opt_datatype_t dt1 = ares_dns_opt_get_datatype(ARES_RR_OPT_OPTIONS, opt);
                (void)dt1;
                
                /* Test with SVCB params key */
                ares_dns_opt_datatype_t dt2 = ares_dns_opt_get_datatype(ARES_RR_SVCB_PARAMS, opt);
                (void)dt2;
                
                /* Test with HTTPS params key */
                ares_dns_opt_datatype_t dt3 = ares_dns_opt_get_datatype(ARES_RR_HTTPS_PARAMS, opt);
                (void)dt3;
            }
            break;
        }

        case 2: {
            /* Test ares_dns_opt_get_name with various keys and options */
            for (unsigned short opt = 0; opt < 256; opt++) {
                /* Test with OPT record key */
                const char *name1 = ares_dns_opt_get_name(ARES_RR_OPT_OPTIONS, opt);
                (void)name1;
                
                /* Test with SVCB params key */
                const char *name2 = ares_dns_opt_get_name(ARES_RR_SVCB_PARAMS, opt);
                (void)name2;
                
                /* Test with HTTPS params key */
                const char *name3 = ares_dns_opt_get_name(ARES_RR_HTTPS_PARAMS, opt);
                (void)name3;
            }
            break;
        }

        case 3: {
            /* Test all three functions together with random inputs */
            for (int i = 0; i < 50; i++) {
                /* Test various RR keys */
                ares_dns_rr_key_t key = static_cast<ares_dns_rr_key_t>(fdp.ConsumeIntegral<uint8_t>());
                const char *key_str = ares_dns_rr_key_tostr(key);
                (void)key_str;
                
                /* Test option functions for specific keys */
                if (key == ARES_RR_OPT_OPTIONS || key == ARES_RR_SVCB_PARAMS || 
                    key == ARES_RR_HTTPS_PARAMS) {
                    unsigned short opt = fdp.ConsumeIntegral<uint16_t>();
                    ares_dns_opt_datatype_t dt = ares_dns_opt_get_datatype(key, opt);
                    (void)dt;
                    
                    const char *opt_name = ares_dns_opt_get_name(key, opt);
                    (void)opt_name;
                }
                
                /* Test other mapping functions */
                ares_dns_rec_type_t type = static_cast<ares_dns_rec_type_t>(fdp.ConsumeIntegral<uint8_t>() % 20);
                const char *type_str = ares_dns_rec_type_tostr(type);
                (void)type_str;
                
                ares_dns_class_t qclass = static_cast<ares_dns_class_t>(fdp.ConsumeIntegral<uint8_t>() % 6);
                const char *class_str = ares_dns_class_tostr(qclass);
                (void)class_str;
                
                ares_dns_opcode_t opcode = static_cast<ares_dns_opcode_t>(fdp.ConsumeIntegral<uint8_t>() % 5);
                const char *opcode_str = ares_dns_opcode_tostr(opcode);
                (void)opcode_str;
                
                ares_dns_section_t section = static_cast<ares_dns_section_t>((fdp.ConsumeIntegral<uint8_t>() % 3) + 1);
                const char *section_str = ares_dns_section_tostr(section);
                (void)section_str;
                
                ares_dns_rcode_t rcode = static_cast<ares_dns_rcode_t>(fdp.ConsumeIntegral<uint8_t>() % 20);
                const char *rcode_str = ares_dns_rcode_tostr(rcode);
                (void)rcode_str;
            }
            break;
        }
    }

    /* Clean up */
    ares_library_cleanup();

    return 0;
}

} // namespace harness_012
