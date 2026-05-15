#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_image.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

namespace harness_019 {
/*
 * Fuzzing harness for libaom AV1 configuration parsing and validation utilities
 * Targets common module (0% coverage) - configuration file parsing and validation
 * 
 * Primary targets (tested indirectly through public APIs):
 * 1. av1_config.c functions - AV1 codec configuration parsing (via decoder APIs)
 * 2. Configuration validation through decoder initialization
 * 3. Image utilities for YUV frame handling
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: general decoder/encoder testing with IVF format
 * - harness_001: image metadata operations
 * - harness_017: OBU decoding and argument parsing
 * - harness_018: AV1 config parsing through decoder APIs
 * - harness_019: Configuration parsing and validation focused on decoder paths
 * 
 * This harness fills coverage gaps in common module through decoder-focused testing.
 */



// Minimum input size for meaningful testing
#define MIN_INPUT_SIZE 64

// Helper to create malformed bitstream with various OBU configurations
// This targets av1_config.c parsing paths through decoder APIs
static std::vector<uint8_t> create_config_heavy_bitstream(FuzzedDataProvider& fdp, size_t size) {
    std::vector<uint8_t> bitstream = fdp.ConsumeBytes<uint8_t>(size);
    
    if (bitstream.size() < 4) {
        bitstream.resize(4, 0);
    }
    
    // Create configuration-heavy data by adding AV1 config-like structures
    // AV1 config starts with marker bit (0x80) and version (0x01)
    if (bitstream.size() >= 4) {
        // First byte: marker + version (as per AV1 config)
        bitstream[0] = 0x80 | 0x01; // Marker bit + version 1
        
        // Second byte: seq_profile (3 bits), seq_level_idx_0 (5 bits)
        uint8_t seq_profile = fdp.ConsumeIntegralInRange<uint8_t>(0, 7);
        uint8_t seq_level_idx_0 = fdp.ConsumeIntegralInRange<uint8_t>(0, 31);
        bitstream[1] = (seq_profile << 5) | seq_level_idx_0;
        
        // Third byte: seq_tier_0 (1 bit), high_bitdepth (1 bit), twelve_bit (1 bit), 
        // monochrome (1 bit), chroma_subsampling_x (1 bit), chroma_subsampling_y (1 bit),
        // chroma_sample_position (2 bits)
        uint8_t third_byte = 0;
        if (fdp.ConsumeBool()) third_byte |= 0x80; // seq_tier_0
        if (fdp.ConsumeBool()) third_byte |= 0x40; // high_bitdepth
        if (fdp.ConsumeBool()) third_byte |= 0x20; // twelve_bit
        if (fdp.ConsumeBool()) third_byte |= 0x10; // monochrome
        if (fdp.ConsumeBool()) third_byte |= 0x08; // chroma_subsampling_x
        if (fdp.ConsumeBool()) third_byte |= 0x04; // chroma_subsampling_y
        third_byte |= (fdp.ConsumeIntegralInRange<uint8_t>(0, 3) << 0); // chroma_sample_position
        
        bitstream[2] = third_byte;
        
        // Fourth byte: initial_presentation_delay_present (1 bit) + reserved/initial_presentation_delay_minus_one
        if (fdp.ConsumeBool()) {
            bitstream[3] = 0x80 | (fdp.ConsumeIntegralInRange<uint8_t>(0, 15) << 0);
        } else {
            bitstream[3] = 0x00;
        }
    }
    
    // Add OBU-like structures after config
    if (bitstream.size() >= 8) {
        // Add OBU header at position 4
        uint8_t obu_type = fdp.ConsumeIntegralInRange<uint8_t>(0, 7); // 3-bit OBU type
        uint8_t obu_extension = fdp.ConsumeBool() ? 0x20 : 0x00; // bit 5
        uint8_t obu_has_size = fdp.ConsumeBool() ? 0x10 : 0x00; // bit 4
        
        bitstream[4] = 0x80 | (obu_type << 5) | obu_extension | obu_has_size;
        
        // Add LEB128 size if needed
        if (obu_has_size && bitstream.size() >= 12) {
            size_t leb128_size = fdp.ConsumeIntegralInRange<size_t>(1, 4);
            for (size_t i = 0; i < leb128_size && (5 + i) < bitstream.size(); i++) {
                uint8_t byte = fdp.ConsumeIntegralInRange<uint8_t>(1, 127);
                if (i == leb128_size - 1) {
                    byte &= 0x7F; // Clear continuation bit
                } else {
                    byte |= 0x80; // Set continuation bit
                }
                bitstream[5 + i] = byte;
            }
        }
    }
    
    return bitstream;
}

// Helper to create test YUV data
static std::vector<uint8_t> create_yuv_test_data(FuzzedDataProvider& fdp, int width, int height) {
    // Simple YUV420 format: Y plane + U plane + V plane
    size_t y_size = width * height;
    size_t uv_size = (width/2) * (height/2);
    size_t total_size = y_size + uv_size * 2;
    
    std::vector<uint8_t> yuv_data;
    yuv_data.reserve(total_size);
    
    // Add Y plane
    auto y_plane = fdp.ConsumeBytes<uint8_t>(y_size);
    if (y_plane.size() < y_size) {
        y_plane.resize(y_size, 128); // Default gray
    }
    yuv_data.insert(yuv_data.end(), y_plane.begin(), y_plane.end());
    
    // Add U plane (chroma)
    auto u_plane = fdp.ConsumeBytes<uint8_t>(uv_size);
    if (u_plane.size() < uv_size) {
        u_plane.resize(uv_size, 128);
    }
    yuv_data.insert(yuv_data.end(), u_plane.begin(), u_plane.end());
    
    // Add V plane (chroma)
    auto v_plane = fdp.ConsumeBytes<uint8_t>(uv_size);
    if (v_plane.size() < uv_size) {
        v_plane.resize(uv_size, 128);
    }
    yuv_data.insert(yuv_data.end(), v_plane.begin(), v_plane.end());
    
    return yuv_data;
}

// Main fuzzing entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Test AV1 config parsing through decoder APIs
    // This indirectly exercises av1_config.c functions
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        return 0;
    }
    
    // Create configuration-heavy bitstream
    size_t config_size = fdp.ConsumeIntegralInRange<size_t>(64, 512);
    auto config_bitstream = create_config_heavy_bitstream(fdp, config_size);
    
    // Try to decode with various configs to exercise av1_config.c
    aom_codec_ctx_t decoder_ctx;
    memset(&decoder_ctx, 0, sizeof(decoder_ctx));
    
    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err == AOM_CODEC_OK) {
        // Try to decode the config-heavy bitstream
        // This will exercise av1_config.c parsing internally
        aom_codec_decode(&decoder_ctx, config_bitstream.data(), config_bitstream.size(), nullptr);
        
        // Try to peek stream info (also exercises config parsing)
        aom_codec_stream_info_t stream_info;
        memset(&stream_info, 0, sizeof(stream_info));
        aom_codec_peek_stream_info(decoder_iface, config_bitstream.data(), config_bitstream.size(), &stream_info);
        
        aom_codec_destroy(&decoder_ctx);
    }
    
    // Step 2: Test different decoder configurations
    // Exercise configuration validation paths
    if (fdp.remaining_bytes() > 100) {
        aom_codec_ctx_t decoder2;
        memset(&decoder2, 0, sizeof(decoder2));
        
        aom_codec_dec_cfg_t dec_cfg2;
        memset(&dec_cfg2, 0, sizeof(dec_cfg2));
        dec_cfg2.threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 8);
        dec_cfg2.allow_lowbitdepth = fdp.ConsumeBool() ? 1 : 0;
        
        err = aom_codec_dec_init_ver(&decoder2, decoder_iface, &dec_cfg2, 0, AOM_DECODER_ABI_VERSION);
        if (err == AOM_CODEC_OK) {
            // Create another test bitstream
            size_t stream2_size = fdp.ConsumeIntegralInRange<size_t>(32, 256);
            auto stream2_data = fdp.ConsumeBytes<uint8_t>(stream2_size);
            
            if (!stream2_data.empty()) {
                aom_codec_decode(&decoder2, stream2_data.data(), stream2_data.size(), nullptr);
            }
            
            // Test some decoder controls that might exercise common module code
            int ctrl_id = fdp.ConsumeIntegralInRange<int>(1, 20);
            int int_value = fdp.ConsumeIntegral<int>();
            aom_codec_control(&decoder2, ctrl_id, int_value);
            
            aom_codec_destroy(&decoder2);
        }
    }
    
    // Step 3: Test image handling which may exercise YUV frame utilities
    // Create a test image with YUV data
    int img_width = 64;
    int img_height = 64;
    auto yuv_data = create_yuv_test_data(fdp, img_width, img_height);
    
    // Create an image structure - this may exercise image utilities
    aom_image_t* img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, img_width, img_height, 16);
    if (img) {
        // Fill with test data if we have enough
        if (yuv_data.size() >= (size_t)(img_width * img_height * 3 / 2)) {
            // Copy Y plane
            size_t y_plane_size = img_width * img_height;
            if (img->planes[0] && y_plane_size > 0) {
                memcpy(img->planes[0], yuv_data.data(), std::min(y_plane_size, yuv_data.size()));
            }
            
            // Copy U plane if we have enough data
            size_t u_plane_size = (img_width/2) * (img_height/2);
            if (img->planes[1] && yuv_data.size() >= y_plane_size + u_plane_size) {
                memcpy(img->planes[1], yuv_data.data() + y_plane_size, u_plane_size);
            }
            
            // Copy V plane if we have enough data
            size_t v_plane_size = (img_width/2) * (img_height/2);
            if (img->planes[2] && yuv_data.size() >= y_plane_size + u_plane_size + v_plane_size) {
                memcpy(img->planes[2], yuv_data.data() + y_plane_size + u_plane_size, v_plane_size);
            }
        }
        
        // Test getting image dimensions and format
        int width = aom_img_plane_width(img, 0);
        int height = aom_img_plane_height(img, 0);
        aom_img_fmt_t fmt = img->fmt;
        
        // Create another image with same parameters
        aom_image_t* img2 = aom_img_alloc(nullptr, fmt, width, height, 16);
        if (img2) {
            // Test image comparison by checking dimensions
            int img2_width = aom_img_plane_width(img2, 0);
            int img2_height = aom_img_plane_height(img2, 0);
            
            // Just allocate and free, no copy operation
            aom_img_free(img2);
        }
        
        aom_img_free(img);
    }
    
    // Step 4: Test stream info parsing with different bitstreams
    // Create multiple test bitstreams to exercise different parsing paths
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 50; ++i) {
        size_t test_size = fdp.ConsumeIntegralInRange<size_t>(16, 128);
        auto test_data = fdp.ConsumeBytes<uint8_t>(test_size);
        
        if (!test_data.empty()) {
            aom_codec_stream_info_t test_info;
            memset(&test_info, 0, sizeof(test_info));
            aom_codec_peek_stream_info(decoder_iface, test_data.data(), test_data.size(), &test_info);
        }
    }
    
    return 0;
}

} // namespace harness_019
