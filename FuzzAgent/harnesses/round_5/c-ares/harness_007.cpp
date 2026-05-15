#include <fuzzer/FuzzedDataProvider.h>
#include "ares.h"
#include "ares_dns_record.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_007 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Create a DNS record
    ares_dns_record_t* dnsrec = nullptr;
    unsigned short id = fdp.ConsumeIntegral<unsigned short>();
    unsigned short flags = fdp.ConsumeIntegral<unsigned short>();
    ares_dns_opcode_t opcode = static_cast<ares_dns_opcode_t>(fdp.ConsumeIntegralInRange<uint8_t>(0, 5));
    ares_dns_rcode_t rcode = static_cast<ares_dns_rcode_t>(fdp.ConsumeIntegralInRange<uint8_t>(0, 15));

    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || dnsrec == nullptr) {
        return 0;
    }

    // Test different DNS record types that support binary data
    ares_dns_rec_type_t record_types[] = {
        ARES_REC_TYPE_TXT,
        ARES_REC_TYPE_OPT,
        ARES_REC_TYPE_SIG,
        ARES_REC_TYPE_TLSA,
        ARES_REC_TYPE_RAW_RR
    };

    // Create resource records for each section
    for (int section_idx = 0; section_idx < 3 && fdp.remaining_bytes() > 16; section_idx++) {
        ares_dns_section_t section = static_cast<ares_dns_section_t>(section_idx + 1);
        ares_dns_rec_type_t rec_type = record_types[fdp.ConsumeIntegralInRange<size_t>(0, sizeof(record_types)/sizeof(record_types[0]) - 1)];
        ares_dns_class_t rr_class = static_cast<ares_dns_class_t>(fdp.ConsumeIntegralInRange<uint16_t>(1, 255));

        // Get keys for this record type
        size_t key_count = 0;
        const ares_dns_rr_key_t* keys = ares_dns_rr_get_keys(rec_type, &key_count);
        if (keys == nullptr || key_count == 0) {
            continue;
        }

        // Create RR using ares_dns_record_rr_add
        std::string name = fdp.ConsumeRandomLengthString(64);
        ares_dns_rr_t* rr = nullptr;
        status = ares_dns_record_rr_add(&rr, dnsrec, section, name.c_str(), rec_type, rr_class, 0);
        if (status != ARES_SUCCESS || rr == nullptr) {
            continue;
        }

        // Test ares_dns_rr_set_bin for BIN/BINP datatypes
        for (size_t i = 0; i < key_count && fdp.remaining_bytes() > 8; i++) {
            ares_dns_rr_key_t key = keys[i];
            ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
            
            if (datatype == ARES_DATATYPE_BIN || datatype == ARES_DATATYPE_BINP) {
                // Consume binary data
                size_t bin_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                if (fdp.remaining_bytes() < bin_len) {
                    break;
                }
                std::vector<uint8_t> bin_data = fdp.ConsumeBytes<uint8_t>(bin_len);
                
                status = ares_dns_rr_set_bin(rr, key, bin_data.data(), bin_len);
                // Status may fail for various reasons, that's okay for fuzzing
            }
        }

        // Test ares_dns_rr_add_abin for ABINP datatypes
        for (size_t i = 0; i < key_count && fdp.remaining_bytes() > 16; i++) {
            ares_dns_rr_key_t key = keys[i];
            ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
            
            if (datatype == ARES_DATATYPE_ABINP) {
                // Add multiple binary entries to the array
                size_t num_entries = fdp.ConsumeIntegralInRange<size_t>(1, 4);
                for (size_t entry_idx = 0; entry_idx < num_entries && fdp.remaining_bytes() > 8; entry_idx++) {
                    size_t bin_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                    if (fdp.remaining_bytes() < bin_len) {
                        break;
                    }
                    std::vector<uint8_t> bin_data = fdp.ConsumeBytes<uint8_t>(bin_len);
                    
                    status = ares_dns_rr_add_abin(rr, key, bin_data.data(), bin_len);
                    // Status may fail for various reasons, that's okay for fuzzing
                    
                    // Test ares_dns_rr_del_abin occasionally
                    if (entry_idx > 0 && fdp.ConsumeBool()) {
                        size_t del_idx = fdp.ConsumeIntegralInRange<size_t>(0, entry_idx - 1);
                        ares_dns_rr_del_abin(rr, key, del_idx);
                    }
                }
            }
        }

        // Test ares_dns_rr_set_str for STR/NAME datatypes
        for (size_t i = 0; i < key_count && fdp.remaining_bytes() > 4; i++) {
            ares_dns_rr_key_t key = keys[i];
            ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
            
            if (datatype == ARES_DATATYPE_STR || datatype == ARES_DATATYPE_NAME) {
                std::string str_val = fdp.ConsumeRandomLengthString(32);
                status = ares_dns_rr_set_str(rr, key, str_val.c_str());
                // Status may fail for various reasons, that's okay for fuzzing
            }
        }

        // Set other data types based on fuzzer input
        for (size_t i = 0; i < key_count && fdp.remaining_bytes() > 4; i++) {
            ares_dns_rr_key_t key = keys[i];
            ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
            
            switch (datatype) {
                case ARES_DATATYPE_U8:
                    ares_dns_rr_set_u8(rr, key, fdp.ConsumeIntegral<uint8_t>());
                    break;
                case ARES_DATATYPE_U16:
                    ares_dns_rr_set_u16(rr, key, fdp.ConsumeIntegral<uint16_t>());
                    break;
                case ARES_DATATYPE_U32:
                    ares_dns_rr_set_u32(rr, key, fdp.ConsumeIntegral<uint32_t>());
                    break;
                default:
                    break;
            }
        }
    }

    // Additional test: Create a TXT record specifically for binary array testing
    if (fdp.remaining_bytes() > 32) {
        std::string txt_name = fdp.ConsumeRandomLengthString(32);
        ares_dns_rr_t* txt_rr = nullptr;
        status = ares_dns_record_rr_add(&txt_rr, dnsrec, ARES_SECTION_ANSWER, 
                                        txt_name.c_str(), ARES_REC_TYPE_TXT, 
                                        ARES_CLASS_IN, 0);
        if (status == ARES_SUCCESS && txt_rr != nullptr) {
            // TXT records have TXT_DATA key with ABINP datatype
            size_t key_count = 0;
            const ares_dns_rr_key_t* keys = ares_dns_rr_get_keys(ARES_REC_TYPE_TXT, &key_count);
            if (keys != nullptr && key_count > 0) {
                // Find TXT_DATA key
                for (size_t i = 0; i < key_count; i++) {
                    ares_dns_rr_key_t key = keys[i];
                    ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
                    if (datatype == ARES_DATATYPE_ABINP) {
                        // Add multiple binary entries
                        size_t num_txt_entries = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                        for (size_t txt_idx = 0; txt_idx < num_txt_entries && fdp.remaining_bytes() > 4; txt_idx++) {
                            size_t txt_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                            if (fdp.remaining_bytes() < txt_len) {
                                break;
                            }
                            std::vector<uint8_t> txt_data = fdp.ConsumeBytes<uint8_t>(txt_len);
                            status = ares_dns_rr_add_abin(txt_rr, key, txt_data.data(), txt_len);
                            
                            // Test deletion of some entries
                            if (txt_idx > 0 && fdp.ConsumeBool()) {
                                size_t del_idx = fdp.ConsumeIntegralInRange<size_t>(0, txt_idx - 1);
                                ares_dns_rr_del_abin(txt_rr, key, del_idx);
                            }
                        }
                        break;
                    }
                }
            }
        }
    }

    // Clean up
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}

} // namespace harness_007
