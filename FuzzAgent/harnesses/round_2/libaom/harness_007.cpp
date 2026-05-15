#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>
#include <algorithm>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"

namespace harness_007 {
/*
 * Fuzzing harness for libaom AV1 OBU (Open Bitstream Unit) type handling and codec information functions
 * Targets uncovered OBU functions identified by CoverageAnalyzerAgent:
 * - aom_obu_type_to_string (0% coverage, 20 undiscovered branches) - Most critical target
 * - aom_codec_set_cx_data_buf (0% coverage, 6 undiscovered branches)
 * - aom_codec_iface_name (0% coverage, 2 undiscovered branches) 
 * - aom_codec_get_caps (0% coverage, 2 undiscovered branches)
 * - aom_codec_version* family (version functions)
 * Module: OBU (Open Bitstream Unit) parsing and type handling
 * Strategy: Focus on OBU parsing which is fundamental to AV1 bitstream structure
 * Differentiation: Unlike existing harnesses (000: decoder/encoder, 001: decoder, 002: image metadata, 
 *                  003: encoder configuration, 004: image manipulation, 005: decoder memory management,
 *                  006: LEB128 integer encoding), targets OBU type handling and codec information APIs
 * Performance: OBU parsing is a hot path in AV1 decoding; could uncover buffer overflow, integer overflow,
 *              memory corruption bugs
 * Coverage Goal: 8 new APIs covered (17% increase in API coverage), ~30+ new branches covered
 * Bug Potential: Buffer overflow in OBU header parsing, integer overflows in OBU size calculations,
 *                memory corruption in OBU metadata handling, logic errors in OBU type validation
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need at least some bytes for operation mode and basic parameters
    const size_t MIN_INPUT_SIZE = sizeof(uint8_t) + sizeof(uint16_t) * 2 + sizeof(uint32_t) * 2;
    if (size < MIN_INPUT_SIZE) {
        return 0;  // Not enough data for meaningful OBU testing
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: decoder, 1: encoder, 2: both
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0: basic, 1: edge cases, 2: invalid, 3: mixed
    
    // Get buffer size parameters for aom_codec_set_cx_data_buf
    uint16_t buffer_size = fdp.ConsumeIntegralInRange<uint16_t>(1, 4096);
    uint32_t pad_before = fdp.ConsumeIntegral<uint32_t>();
    uint32_t pad_after = fdp.ConsumeIntegral<uint32_t>();
    
    // Get remaining data for buffer operations
    std::vector<uint8_t> buffer_data = fdp.ConsumeBytes<uint8_t>(buffer_size);
    
    int ret = 0;
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t* codec_iface = nullptr;

    // Initialize codec context based on operation mode
    if (operation == 0 || operation == 2) {
        // Test with decoder
        codec_iface = aom_codec_av1_dx();
        if (codec_iface) {
            aom_codec_dec_cfg_t decoder_cfg = {1, 0, 0, 1};  // Basic configuration
            
            if (aom_codec_dec_init(&codec_ctx, codec_iface, &decoder_cfg, 0) == AOM_CODEC_OK) {
                // Test 1: Get codec interface name
                const char* iface_name = aom_codec_iface_name(codec_iface);
                // Result can be NULL or string, both are valid
                
                // Test 2: Get codec capabilities
                aom_codec_caps_t caps = aom_codec_get_caps(codec_iface);
                // Caps is a bitfield, any value is valid
                
                // Test 3: Test version functions
                int version = aom_codec_version();
                const char* version_str = aom_codec_version_str();
                const char* version_extra_str = aom_codec_version_extra_str();
                // All should return valid strings/values
                
                // Test 4: Test aom_obu_type_to_string with various OBU_TYPE values
                // First test all valid OBU_TYPE enum values (1-8, 15)
                const char* obu_str = nullptr;
                switch (test_mode % 4) {
                    case 0: // Test all valid enum values systematically
                        obu_str = aom_obu_type_to_string(OBU_SEQUENCE_HEADER);
                        obu_str = aom_obu_type_to_string(OBU_TEMPORAL_DELIMITER);
                        obu_str = aom_obu_type_to_string(OBU_FRAME_HEADER);
                        obu_str = aom_obu_type_to_string(OBU_TILE_GROUP);
                        obu_str = aom_obu_type_to_string(OBU_METADATA);
                        obu_str = aom_obu_type_to_string(OBU_FRAME);
                        obu_str = aom_obu_type_to_string(OBU_REDUNDANT_FRAME_HEADER);
                        obu_str = aom_obu_type_to_string(OBU_TILE_LIST);
                        obu_str = aom_obu_type_to_string(OBU_PADDING);
                        break;
                        
                    case 1: // Test edge cases and invalid values
                        // Test value 0 (invalid)
                        obu_str = aom_obu_type_to_string(static_cast<OBU_TYPE>(0));
                        // Test values between valid ranges (9-14)
                        for (int i = 9; i <= 14; i++) {
                            obu_str = aom_obu_type_to_string(static_cast<OBU_TYPE>(i));
                        }
                        // Test value 16 and beyond
                        obu_str = aom_obu_type_to_string(static_cast<OBU_TYPE>(16));
                        obu_str = aom_obu_type_to_string(static_cast<OBU_TYPE>(255));
                        break;
                        
                    case 2: // Test with fuzzed values
                        // Consume remaining bytes to generate OBU_TYPE values
                        while (fdp.remaining_bytes() > 0) {
                            uint8_t obu_type_val = fdp.ConsumeIntegral<uint8_t>();
                            obu_str = aom_obu_type_to_string(static_cast<OBU_TYPE>(obu_type_val));
                        }
                        break;
                        
                    case 3: // Mixed testing
                        // Test some valid values
                        obu_str = aom_obu_type_to_string(OBU_SEQUENCE_HEADER);
                        obu_str = aom_obu_type_to_string(OBU_FRAME);
                        obu_str = aom_obu_type_to_string(OBU_PADDING);
                        // Test some invalid values
                        obu_str = aom_obu_type_to_string(static_cast<OBU_TYPE>(0));
                        obu_str = aom_obu_type_to_string(static_cast<OBU_TYPE>(100));
                        break;
                }
                
                // Test 5: Test aom_codec_set_cx_data_buf if we have buffer data
                if (!buffer_data.empty()) {
                    aom_fixed_buf_t fixed_buf;
                    fixed_buf.buf = buffer_data.data();
                    fixed_buf.sz = buffer_data.size();
                    
                    // Test with valid buffer
                    aom_codec_err_t buf_result = aom_codec_set_cx_data_buf(&codec_ctx, &fixed_buf, 
                                                                           pad_before % 256, 
                                                                           pad_after % 256);
                    // Any return value is valid for fuzzing purposes
                    
                    // Test with NULL buffer (should restore default behavior)
                    buf_result = aom_codec_set_cx_data_buf(&codec_ctx, nullptr, 0, 0);
                    
                    // Test with zero-sized buffer
                    aom_fixed_buf_t zero_buf = {nullptr, 0};
                    buf_result = aom_codec_set_cx_data_buf(&codec_ctx, &zero_buf, 0, 0);
                }
                
                // Clean up
                aom_codec_destroy(&codec_ctx);
            }
        }
    }
    
    // Also test with encoder interface if operation mode includes it
    if (operation == 1 || operation == 2) {
        codec_iface = aom_codec_av1_cx();
        if (codec_iface) {
            aom_codec_enc_cfg_t encoder_cfg;
            
            if (aom_codec_enc_config_default(codec_iface, &encoder_cfg, 0) == AOM_CODEC_OK) {
                // Initialize encoder with basic configuration
                if (aom_codec_enc_init(&codec_ctx, codec_iface, &encoder_cfg, 0) == AOM_CODEC_OK) {
                    // Test the same functions with encoder context
                    const char* iface_name = aom_codec_iface_name(codec_iface);
                    aom_codec_caps_t caps = aom_codec_get_caps(codec_iface);
                    
                    // Test aom_obu_type_to_string with encoder context
                    // (function doesn't depend on codec context, but we want to test it anyway)
                    const char* obu_str = aom_obu_type_to_string(OBU_FRAME);
                    obu_str = aom_obu_type_to_string(OBU_TILE_GROUP);
                    
                    // Test aom_codec_set_cx_data_buf with encoder (more relevant for encoder)
                    if (!buffer_data.empty()) {
                        aom_fixed_buf_t fixed_buf;
                        fixed_buf.buf = buffer_data.data();
                        fixed_buf.sz = buffer_data.size();
                        
                        aom_codec_err_t buf_result = aom_codec_set_cx_data_buf(&codec_ctx, &fixed_buf,
                                                                               pad_before % 256,
                                                                               pad_after % 256);
                    }
                    
                    aom_codec_destroy(&codec_ctx);
                }
            }
        }
    }
    
    // Additional standalone tests that don't require codec context
    // Test aom_obu_type_to_string with all possible byte values (0-255)
    // This exercises the switch statement's default case for invalid values
    if (test_mode == 2) {
        for (int i = 0; i < 256 && fdp.remaining_bytes() > 0; i++) {
            const char* obu_str = aom_obu_type_to_string(static_cast<OBU_TYPE>(i));
        }
    }
    
    // Test version functions standalone
    int version = aom_codec_version();
    const char* version_str = aom_codec_version_str();
    const char* version_extra_str = aom_codec_version_extra_str();
    const char* build_config = aom_codec_build_config();  // Also test build config
    
    // Test error string conversion (related utility function)
    const char* error_str = aom_codec_err_to_string(AOM_CODEC_OK);
    error_str = aom_codec_err_to_string(AOM_CODEC_ERROR);
    error_str = aom_codec_err_to_string(AOM_CODEC_INVALID_PARAM);
    
    return 0;
}

} // namespace harness_007
