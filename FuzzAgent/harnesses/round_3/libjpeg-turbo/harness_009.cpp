#include <fuzzer/FuzzedDataProvider.h>
#include "turbojpeg.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

namespace harness_009 {
/*
 * Fuzzing harness for libjpeg-turbo old turbojpeg YUV decompression API
 * This harness targets completely uncovered old turbojpeg YUV decompression operations:
 * - tjDecompressToYUV: Old-style YUV decompression (143 undiscovered branches)
 * - tjDecompressToYUV2: Extended old-style YUV decompression (143 undiscovered branches) 
 * - tjDecompressHeader: JPEG header reading (108 undiscovered branches)
 * 
 * Required Helper APIs:
 * - tjInitDecompress: Decompressor initialization (11 undiscovered branches)
 * - tjDestroy: Cleanup (5 undiscovered branches)
 * - tjBufSizeYUV: Buffer sizing (17 undiscovered branches)
 * - tjAlloc/tjFree: Buffer allocation/deallocation
 * 
 * Differentiates from existing harnesses (000-008):
 * - harness_000.cpp: Modern tj3* TurboJPEG APIs
 * - harness_001.cpp: Traditional tj* compression/decompression (not YUV-specific)
 * - harness_002.cpp: Modern tj3* YUV APIs (tj3DecodeYUV, not old tj*)
 * - harness_003.cpp: TurboJPEG 12/16-bit precision APIs
 * - harness_004.cpp: YUV plane operations (tjDecompressToYUVPlanes, not old tjDecompressToYUV)
 * - harness_005.cpp: High-bit-depth image I/O
 * - harness_006.cpp: Traditional YUV encoding
 * - harness_007.cpp: Traditional libjpeg I/O APIs (8-bit)
 * - harness_008.cpp: Traditional libjpeg 12-bit APIs
 * 
 * Strategy: Implement full old turbojpeg YUV decompression lifecycle
 * tjInitDecompress() -> tjDecompressHeader() -> tjBufSizeYUV() -> 
 * allocation -> tjDecompressToYUV/tjDecompressToYUV2() -> cleanup
 * 
 * Expected Coverage Gain: 394+ undiscovered branches in critical YUV decompression path
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for JPEG header and basic operations
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize decompressor handle using old turbojpeg API
    tjhandle decompress_handle = tjInitDecompress();
    if (!decompress_handle) {
        return 0;
    }
    
    // Consume JPEG buffer size from fuzzer input (first part of input is JPEG data)
    size_t jpeg_data_size = fdp.ConsumeIntegralInRange<size_t>(2, std::min(size / 2, (size_t)1024));
    std::vector<unsigned char> jpeg_buf = fdp.ConsumeBytes<unsigned char>(jpeg_data_size);
    
    if (jpeg_buf.size() < 2) {
        tjDestroy(decompress_handle);
        return 0;
    }
    
    // Consume operation type: 0 = tjDecompressToYUV, 1 = tjDecompressToYUV2
    int operation = fdp.ConsumeIntegralInRange<int>(0, 1);
    
    // Consume width and height for tjDecompressToYUV2 (ignored for tjDecompressToYUV)
    int width = 0, height = 0;
    int subsamp = 0;
    
    if (operation == 1) {
        // For tjDecompressToYUV2, need explicit dimensions
        width = fdp.ConsumeIntegralInRange<int>(8, 256);
        height = fdp.ConsumeIntegralInRange<int>(8, 256);
        
        // Consume subsampling type
        int subsamp_idx = fdp.ConsumeIntegralInRange<int>(0, 5);
        enum TJSAMP subsampling[] = {
            TJSAMP_444, TJSAMP_422, TJSAMP_420, 
            TJSAMP_GRAY, TJSAMP_440, TJSAMP_411
        };
        subsamp = subsampling[subsamp_idx % 6];
    }
    
    // Consume alignment for tjDecompressToYUV2 (must be power of 2)
    int align_options[] = {1, 2, 4, 8, 16};
    int align = align_options[fdp.ConsumeIntegralInRange<int>(0, 4) % 5];
    
    // Consume flags
    int flags = 0;
    if (fdp.ConsumeBool()) flags |= TJFLAG_BOTTOMUP;
    if (fdp.ConsumeBool()) flags |= TJFLAG_FASTUPSAMPLE;
    if (fdp.ConsumeBool()) flags |= TJFLAG_NOREALLOC;
    if (fdp.ConsumeBool()) flags |= TJFLAG_FASTDCT;
    if (fdp.ConsumeBool()) flags |= TJFLAG_ACCURATEDCT;
    if (fdp.ConsumeBool()) flags |= TJFLAG_STOPONWARNING;
    if (fdp.ConsumeBool()) flags |= TJFLAG_PROGRESSIVE;
    
    // ========== STEP 1: Read JPEG header with tjDecompressHeader ==========
    int header_width = 0, header_height = 0;
    int ret = tjDecompressHeader(decompress_handle, jpeg_buf.data(), 
                                 (unsigned long)jpeg_buf.size(), 
                                 &header_width, &header_height);
    
    // If header reading fails, try with remaining input as alternative JPEG data
    if (ret != 0) {
        // Try alternative: use remaining bytes as JPEG data
        if (fdp.remaining_bytes() > 2) {
            std::vector<unsigned char> alt_jpeg_buf = fdp.ConsumeRemainingBytes<unsigned char>();
            ret = tjDecompressHeader(decompress_handle, alt_jpeg_buf.data(),
                                     (unsigned long)alt_jpeg_buf.size(),
                                     &header_width, &header_height);
            if (ret != 0) {
                tjDestroy(decompress_handle);
                return 0;
            }
            jpeg_buf = std::move(alt_jpeg_buf);
        } else {
            tjDestroy(decompress_handle);
            return 0;
        }
    }
    
    // Use header dimensions if operation is 0 (tjDecompressToYUV)
    if (operation == 0) {
        width = header_width;
        height = header_height;
        
        // Try to determine subsampling from header using tjDecompressHeader2
        int jpegSubsamp = 0;
        if (tjDecompressHeader2(decompress_handle, jpeg_buf.data(),
                               (unsigned long)jpeg_buf.size(),
                               &width, &height, &jpegSubsamp) == 0) {
            subsamp = jpegSubsamp;
        }
    }
    
    // Validate dimensions
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        tjDestroy(decompress_handle);
        return 0;
    }
    
    // ========== STEP 2: Calculate YUV buffer size with tjBufSizeYUV ==========
    unsigned long yuv_buf_size = 0;
    
    if (operation == 0) {
        // For tjDecompressToYUV, use standard buffer size calculation
        yuv_buf_size = tjBufSizeYUV(width, height, subsamp);
    } else {
        // For tjDecompressToYUV2, use extended buffer size calculation
        yuv_buf_size = tjBufSizeYUV2(width, align, height, subsamp);
    }
    
    if (yuv_buf_size == 0 || yuv_buf_size > 100 * 1024 * 1024) { // Limit to 100MB
        tjDestroy(decompress_handle);
        return 0;
    }
    
    // ========== STEP 3: Allocate YUV buffer with tjAlloc ==========
    unsigned char* yuv_buf = tjAlloc((int)yuv_buf_size);
    if (!yuv_buf) {
        tjDestroy(decompress_handle);
        return 0;
    }
    
    // ========== STEP 4: Perform YUV decompression ==========
    int decompress_result = -1;
    
    if (operation == 0) {
        // Test tjDecompressToYUV (old API)
        decompress_result = tjDecompressToYUV(decompress_handle, 
                                             jpeg_buf.data(),
                                             (unsigned long)jpeg_buf.size(),
                                             yuv_buf,
                                             flags);
    } else {
        // Test tjDecompressToYUV2 (extended API)
        decompress_result = tjDecompressToYUV2(decompress_handle,
                                              jpeg_buf.data(),
                                              (unsigned long)jpeg_buf.size(),
                                              yuv_buf,
                                              width, align, height,
                                              flags);
    }
    
    // ========== STEP 5: Error handling (optional) ==========
    if (decompress_result != 0) {
        // Check error codes if available
        int error_code = tjGetErrorCode(decompress_handle);
        const char* error_str = tjGetErrorStr2 ? tjGetErrorStr2(decompress_handle) : tjGetErrorStr();
        // Error information can be used for debugging but not for coverage
    }
    
    // ========== STEP 6: Cleanup ==========
    tjFree(yuv_buf);
    tjDestroy(decompress_handle);
    
    return 0;
}

} // namespace harness_009
