#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <vector>
#include <algorithm>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_image.h"

namespace harness_001 {
/*
 * Fuzzing harness for libaom AV1 decoder with advanced input handling
 * Based on patterns from existing av1_dec_fuzzer.cc with FuzzedDataProvider
 * Targets decoder API with comprehensive control parameter testing
 */



// IVF format constants from existing fuzzer
#define IVF_FRAME_HDR_SZ (4 + 8) /* 4 byte size + 8 byte timestamp */
#define IVF_FILE_HDR_SZ 32

// Simple implementation of mem_get_le32 to avoid header dependency
static inline uint32_t mem_get_le32(const void* vmem) {
    const uint8_t* mem = static_cast<const uint8_t*>(vmem);
    return static_cast<uint32_t>(mem[0]) |
           (static_cast<uint32_t>(mem[1]) << 8) |
           (static_cast<uint32_t>(mem[2]) << 16) |
           (static_cast<uint32_t>(mem[3]) << 24);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < IVF_FILE_HDR_SZ + 4) {
        return 0;  // Not enough data for IVF header + minimal frame
    }

    FuzzedDataProvider fdp(data, size);

    // Parse IVF header similar to existing fuzzer but using FDP
    std::vector<uint8_t> ivf_header = fdp.ConsumeBytes<uint8_t>(IVF_FILE_HDR_SZ);
    
    // Extract control parameters from header bytes (like existing fuzzer)
    uint8_t header_byte_31 = ivf_header[IVF_FILE_HDR_SZ - 1];
    uint8_t header_byte_30 = ivf_header[IVF_FILE_HDR_SZ - 2];
    uint8_t header_byte_0 = ivf_header[0];
    
    // Decode parameters from header (mimicking existing fuzzer logic)
    unsigned int tile_mode = (header_byte_31 & 2) != 0;
    unsigned int ext_tile_debug = (header_byte_31 & 4) != 0;
    unsigned int is_annexb = (header_byte_31 & 8) != 0;
    int output_all_layers = (header_byte_31 & 0x10) != 0;
    int operating_point = header_byte_30 & 0x1F;
    
    // Get thread count from next byte (like existing fuzzer)
    unsigned int threads = 1;
    if (fdp.remaining_bytes() > 0) {
        uint8_t thread_byte = fdp.ConsumeIntegral<uint8_t>();
        threads = (thread_byte & 0x3f) + 1;  // Range [1, 64]
    }
    
    // Get highbitdepth setting from fuzzed data
    bool allow_lowbitdepth = fdp.ConsumeBool();
    
    // Initialize decoder
    aom_codec_iface_t* codec_interface = aom_codec_av1_dx();
    if (!codec_interface) {
        return 0;
    }
    
    aom_codec_ctx_t codec;
    aom_codec_dec_cfg_t cfg = {threads, 0, 0, static_cast<unsigned int>(!allow_lowbitdepth)};
    
    if (aom_codec_dec_init(&codec, codec_interface, &cfg, 0)) {
        return 0;
    }
    
    // Apply decoder controls based on fuzzed parameters
    // These controls are from the existing fuzzer
    AOM_CODEC_CONTROL_TYPECHECKED(&codec, AV1_SET_TILE_MODE, tile_mode);
    AOM_CODEC_CONTROL_TYPECHECKED(&codec, AV1D_EXT_TILE_DEBUG, ext_tile_debug);
    AOM_CODEC_CONTROL_TYPECHECKED(&codec, AV1D_SET_IS_ANNEXB, is_annexb);
    AOM_CODEC_CONTROL_TYPECHECKED(&codec, AV1D_SET_OUTPUT_ALL_LAYERS, output_all_layers);
    AOM_CODEC_CONTROL_TYPECHECKED(&codec, AV1D_SET_OPERATING_POINT, operating_point);
    
    // Try additional decoder controls based on remaining fuzzed data
    if (fdp.remaining_bytes() > 0) {
        // Try setting reference frame controls
        uint8_t ref_frame_control = fdp.ConsumeIntegral<uint8_t>();
        if (ref_frame_control % 2 == 0) {
            AOM_CODEC_CONTROL_TYPECHECKED(&codec, AV1D_SET_SKIP_FILM_GRAIN, 1);
        }
    }
    
    // Process remaining data as potential frame data
    std::vector<uint8_t> frame_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (frame_data.size() < IVF_FRAME_HDR_SZ) {
        aom_codec_destroy(&codec);
        return 0;
    }
    
    // Simulate IVF frame parsing (simplified version of existing fuzzer)
    size_t remaining_frame_data = frame_data.size();
    const uint8_t* frame_ptr = frame_data.data();
    
    while (remaining_frame_data > IVF_FRAME_HDR_SZ) {
        // Parse frame size from header (little-endian)
        size_t frame_size = mem_get_le32(frame_ptr);
        
        // Skip frame header
        frame_ptr += IVF_FRAME_HDR_SZ;
        remaining_frame_data -= IVF_FRAME_HDR_SZ;
        
        // Ensure we don't read past available data
        frame_size = std::min(remaining_frame_data, frame_size);
        if (frame_size == 0) {
            break;
        }
        
        // Try to parse stream info (like existing fuzzer)
        aom_codec_stream_info_t stream_info;
        stream_info.is_annexb = is_annexb;
        aom_codec_peek_stream_info(codec_interface, frame_ptr, frame_size, &stream_info);
        
        // Attempt to decode the frame
        aom_codec_decode(&codec, frame_ptr, frame_size, nullptr);
        
        // Try to get decoded frames
        aom_codec_iter_t iter = nullptr;
        aom_image_t* img = nullptr;
        while ((img = aom_codec_get_frame(&codec, &iter)) != nullptr) {
            // Frame available - we could extract more info here if needed
            // For fuzzing purposes, just accessing the image is enough
            
            // Try to access image properties (increases coverage)
            if (img) {
                // Access basic image properties
                int width = img->d_w;
                int height = img->d_h;
                int fmt = img->fmt;
                
                // Access plane data pointers (but don't dereference deeply)
                // This helps exercise image access code paths
                for (int plane = 0; plane < 3; ++plane) {
                    uint8_t* plane_ptr = img->planes[plane];
                    int stride = img->stride[plane];
                    // Just having the pointers is enough for coverage
                }
            }
        }
        
        // Move to next frame
        frame_ptr += frame_size;
        remaining_frame_data -= frame_size;
    }
    
    // Additional decoder API testing with remaining fuzzed bytes
    if (frame_data.size() > 0) {
        // Test get_stream_info API
        aom_codec_stream_info_t final_stream_info;
        final_stream_info.is_annexb = is_annexb;
        aom_codec_get_stream_info(&codec, &final_stream_info);
    }
    
    // Clean up
    aom_codec_destroy(&codec);
    
    return 0;
}

} // namespace harness_001
