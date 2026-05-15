#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <memory>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_008 {
/*
 * Fuzzing harness for libvpx vpx_scale module image scaling operations
 * Target APIs: vpx_scale_frame and related image scaling operations
 * Focus: Image scaling operations, resolution transformation, format conversion
 *        Specifically targets vpx_scale module which has only 37.30% line coverage
 *        and completely uncovered scaling functions like vpx_scale_frame
 * Semantic differentiation: This harness uniquely focuses on image scaling operations
 * rather than encoder/decoder APIs, image manipulation, or error handling.
 * Previous harnesses covered: decoder APIs (000), encoder APIs (001), multi-encoder (002),
 * decoder advanced features (003), image manipulation (004), error handling (005),
 * advanced encoder control (006), encoder output control (007).
 * 
 * Note: Using forward declarations to avoid header inclusion issues with vpx_scale.h
 */



// Forward declarations for vpx_scale functions to avoid header issues
// These match the declarations in vpx_scale/vpx_scale.h and vpx_scale/yv12config.h

// Minimal YV12_BUFFER_CONFIG structure based on yv12config.h
typedef struct yv12_buffer_config {
  int y_width;
  int y_height;
  int y_crop_width;
  int y_crop_height;
  int y_stride;
  int uv_width;
  int uv_height;
  int uv_crop_width;
  int uv_crop_height;
  int uv_stride;
  int alpha_width;
  int alpha_height;
  int alpha_stride;
  uint8_t *y_buffer;
  uint8_t *u_buffer;
  uint8_t *v_buffer;
  uint8_t *alpha_buffer;
  uint8_t *buffer_alloc;
  size_t buffer_alloc_sz;
  int border;
  size_t frame_size;
  int subsampling_x;
  int subsampling_y;
  unsigned int bit_depth;
  // Using int for color space/range to avoid including vpx_codec.h
  int color_space;
  int color_range;
  int render_width;
  int render_height;
  int corrupted;
  int flags;
} YV12_BUFFER_CONFIG;

// Declare vpx_scale_frame function
extern "C" void vpx_scale_frame(YV12_BUFFER_CONFIG *src, YV12_BUFFER_CONFIG *dst,
                               unsigned char *temp_area, unsigned char temp_height,
                               unsigned int hscale, unsigned int hratio,
                               unsigned int vscale, unsigned int vratio,
                               unsigned int interlaced);

// Declare frame buffer management functions
extern "C" int vpx_alloc_frame_buffer(YV12_BUFFER_CONFIG *ybf, int width, int height,
                                     int ss_x, int ss_y, int border, int byte_alignment);

extern "C" int vpx_free_frame_buffer(YV12_BUFFER_CONFIG *ybf);

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check: need enough for buffer configuration and basic image data
    const size_t MIN_SIZE = 256;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    
    // Source image dimensions (limited to reasonable sizes for fuzzing)
    unsigned int src_width = fdp.ConsumeIntegralInRange<unsigned int>(16, 512);
    unsigned int src_height = fdp.ConsumeIntegralInRange<unsigned int>(16, 512);
    
    // Destination image dimensions (scaled versions of source)
    // Scaling factors: 0.5x to 2.0x range
    unsigned int dst_width = fdp.ConsumeIntegralInRange<unsigned int>(
        src_width / 2, src_width * 2);
    unsigned int dst_height = fdp.ConsumeIntegralInRange<unsigned int>(
        src_height / 2, src_height * 2);
    
    // Ensure minimum dimensions
    if (dst_width < 8) dst_width = 8;
    if (dst_height < 8) dst_height = 8;
    
    // Choose scaling algorithm parameters
    unsigned int hscale = fdp.ConsumeIntegralInRange<unsigned int>(1, 16);
    unsigned int hratio = fdp.ConsumeIntegralInRange<unsigned int>(1, 16);
    unsigned int vscale = fdp.ConsumeIntegralInRange<unsigned int>(1, 16);
    unsigned int vratio = fdp.ConsumeIntegralInRange<unsigned int>(1, 16);
    
    // Interlaced mode (0=progressive, 1=interlaced)
    unsigned int interlaced = fdp.ConsumeBool() ? 1 : 0;
    
    // Border size for frame buffers
    int border = fdp.ConsumeIntegralInRange<int>(0, 32);
    
    // Byte alignment for buffer allocation (power of 2, 0-256)
    unsigned int byte_alignment = 1;
    uint8_t align_power = fdp.ConsumeIntegralInRange<uint8_t>(0, 8);
    byte_alignment = 1 << align_power;
    if (byte_alignment == 0) byte_alignment = 1;
    if (byte_alignment > 256) byte_alignment = 256;
    
    // Subsample factors (chroma subsampling)
    int ss_x = fdp.ConsumeIntegralInRange<int>(0, 1);
    int ss_y = fdp.ConsumeIntegralInRange<int>(0, 1);
    
    // Test Case: Allocate source and destination frame buffers
    YV12_BUFFER_CONFIG src_buffer, dst_buffer;
    
    // Initialize buffers
    memset(&src_buffer, 0, sizeof(src_buffer));
    memset(&dst_buffer, 0, sizeof(dst_buffer));
    
    // Allocate source frame buffer (simplified call without highbitdepth parameter)
    int alloc_src_result = vpx_alloc_frame_buffer(&src_buffer, 
                                                  src_width, src_height,
                                                  ss_x, ss_y, border, byte_alignment);
    
    if (alloc_src_result != 0) {
        // Allocation failed, exit early
        return 0;
    }
    
    // Fill source buffer with fuzzed image data
    size_t y_plane_size = src_buffer.y_stride * src_height;
    size_t uv_plane_size = src_buffer.uv_stride * (src_height >> ss_y);
    
    if (fdp.remaining_bytes() < y_plane_size + 2 * uv_plane_size) {
        vpx_free_frame_buffer(&src_buffer);
        return 0;
    }
    
    // Fill Y plane
    std::vector<uint8_t> y_data = fdp.ConsumeBytes<uint8_t>(y_plane_size);
    for (size_t i = 0; i < y_plane_size && i < y_data.size(); ++i) {
        src_buffer.y_buffer[i] = y_data[i];
    }
    
    // Fill U plane
    std::vector<uint8_t> u_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
    for (size_t i = 0; i < uv_plane_size && i < u_data.size(); ++i) {
        src_buffer.u_buffer[i] = u_data[i];
    }
    
    // Fill V plane
    std::vector<uint8_t> v_data = fdp.ConsumeBytes<uint8_t>(uv_plane_size);
    for (size_t i = 0; i < uv_plane_size && i < v_data.size(); ++i) {
        src_buffer.v_buffer[i] = v_data[i];
    }
    
    // Allocate destination frame buffer
    int alloc_dst_result = vpx_alloc_frame_buffer(&dst_buffer,
                                                  dst_width, dst_height,
                                                  ss_x, ss_y, border, byte_alignment);
    
    if (alloc_dst_result != 0) {
        // Destination allocation failed, cleanup and exit
        vpx_free_frame_buffer(&src_buffer);
        return 0;
    }
    
    // Prepare temporary scaling buffer
    // The temp_area needs to be at least dst_width * temp_height bytes
    unsigned int temp_height = 16; // Common temp height for scaling
    size_t temp_buffer_size = dst_width * temp_height;
    
    if (fdp.remaining_bytes() < temp_buffer_size) {
        vpx_free_frame_buffer(&src_buffer);
        vpx_free_frame_buffer(&dst_buffer);
        return 0;
    }
    
    std::vector<uint8_t> temp_buffer_data = fdp.ConsumeBytes<uint8_t>(temp_buffer_size);
    uint8_t* temp_area = temp_buffer_data.data();
    
    // Primary target: vpx_scale_frame
    // Perform image scaling operation
    vpx_scale_frame(&src_buffer, &dst_buffer, temp_area, temp_height,
                    hscale, hratio, vscale, vratio, interlaced);
    
    // Test scaling with different parameter combinations if enough data remains
    bool test_variations = fdp.ConsumeBool();
    if (test_variations && fdp.remaining_bytes() > 50) {
        // Test with different scaling parameters
        unsigned int alt_hscale = fdp.ConsumeIntegralInRange<unsigned int>(1, 8);
        unsigned int alt_hratio = fdp.ConsumeIntegralInRange<unsigned int>(1, 8);
        unsigned int alt_vscale = fdp.ConsumeIntegralInRange<unsigned int>(1, 8);
        unsigned int alt_vratio = fdp.ConsumeIntegralInRange<unsigned int>(1, 8);
        unsigned int alt_interlaced = fdp.ConsumeBool() ? 1 : 0;
        
        // Use remaining temp buffer or allocate new if needed
        if (fdp.remaining_bytes() >= temp_buffer_size) {
            std::vector<uint8_t> alt_temp_data = fdp.ConsumeBytes<uint8_t>(temp_buffer_size);
            uint8_t* alt_temp_area = alt_temp_data.data();
            
            vpx_scale_frame(&src_buffer, &dst_buffer, alt_temp_area, temp_height,
                            alt_hscale, alt_hratio, alt_vscale, alt_vratio, alt_interlaced);
        }
    }
    
    // Cleanup
    vpx_free_frame_buffer(&src_buffer);
    vpx_free_frame_buffer(&dst_buffer);
    
    return 0;
}

} // namespace harness_008
