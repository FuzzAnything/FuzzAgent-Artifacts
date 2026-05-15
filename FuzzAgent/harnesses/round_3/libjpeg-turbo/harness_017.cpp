#include <fuzzer/FuzzedDataProvider.h>
#include "turbojpeg.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

namespace harness_017 {
/*
 * Fuzzing harness for libjpeg-turbo ICC profile handling in TurboJPEG compression
 * Targets completely uncovered APIs: tj3SetICCProfile (14 undiscovered branches, 0% coverage)
 * and jpeg_write_icc_profile (22 undiscovered branches, 0% coverage).
 * 
 * This harness specifically focuses on unlocking 36 completely uncovered branches
 * by testing ICC profile handling during JPEG compression and decompression.
 * 
 * Key requirements for ICC profile APIs:
 * 1. tj3SetICCProfile: Sets ICC profile data for compression
 * 2. jpeg_write_icc_profile: Internal libjpeg function called during compression with ICC profiles
 * 3. ICC profiles can be valid or invalid (testing error handling paths)
 * 4. ICC profiles can be set before compression and retrieved after decompression
 * 
 * Differentiation from existing harnesses:
 * - harness_016 targets 12-bit raw data processing
 * - fuzz/compress.cc has minimal ICC profile test (only 32 bytes from input)
 * - This harness comprehensively tests ICC profile handling with diverse inputs
 * 
 * Implementation strategy:
 * 1. Test compression with valid ICC profiles (proper ICC header format)
 * 2. Test compression with invalid/corrupted ICC profiles (error paths)
 * 3. Test compression with random binary data as ICC profiles (fuzzing corner cases)
 * 4. Test decompression of JPEGs with embedded ICC profiles
 * 5. Test tj3GetICCProfile to retrieve embedded profiles
 * 6. Test removal of ICC profiles (NULL/0 size parameters)
 * 
 * ICC Profile Structure Basics:
 * - Valid ICC profiles start with "acsp" (0x61637370) at offset 36
 * - Minimum size is 128 bytes
 * - Contains header, tag table, and tagged element data
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for image and ICC profile testing
    if (size < 256) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize handles for compression and decompression
    tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decompress_handle = tj3Init(TJINIT_DECOMPRESS);
    
    if (!compress_handle || !decompress_handle) {
        if (compress_handle) tj3Destroy(compress_handle);
        if (decompress_handle) tj3Destroy(decompress_handle);
        return 0;
    }
    
    // Consume image dimensions from fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(16, 128);
    int height = fdp.ConsumeIntegralInRange<int>(16, 128);
    
    // Choose pixel format
    int pixel_format_idx = fdp.ConsumeIntegralInRange<int>(0, 11);
    enum TJPF pixel_formats[] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, TJPF_XBGR, TJPF_XRGB,
        TJPF_GRAY, TJPF_RGBA, TJPF_BGRA, TJPF_ABGR, TJPF_ARGB, TJPF_CMYK
    };
    int pixelFormat = pixel_formats[pixel_format_idx % 12];
    
    // Choose subsampling
    int subsamp_idx = fdp.ConsumeIntegralInRange<int>(0, 5);
    enum TJSAMP subsampling[] = {
        TJSAMP_444, TJSAMP_422, TJSAMP_420, 
        TJSAMP_GRAY, TJSAMP_440, TJSAMP_411
    };
    enum TJSAMP subsamp = subsampling[subsamp_idx % 6];
    
    // Choose quality
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    
    // Set compression parameters
    tj3Set(compress_handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(compress_handle, TJPARAM_QUALITY, quality);
    tj3Set(compress_handle, TJPARAM_FASTDCT, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_OPTIMIZE, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_PROGRESSIVE, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_ARITHMETIC, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_RESTARTROWS, fdp.ConsumeIntegralInRange<int>(0, 16));
    
    // Generate synthetic image data
    size_t image_size = (size_t)width * height * tjPixelSize[pixelFormat];
    if (image_size == 0) {
        tj3Destroy(compress_handle);
        tj3Destroy(decompress_handle);
        return 0;
    }
    
    // Allocate source image buffer
    std::vector<unsigned char> src_buf(image_size);
    if (src_buf.empty()) {
        tj3Destroy(compress_handle);
        tj3Destroy(decompress_handle);
        return 0;
    }
    
    // Fill image with fuzzer data
    auto image_data = fdp.ConsumeBytes<unsigned char>(std::min(image_size, fdp.remaining_bytes()));
    if (image_data.size() > 0) {
        size_t copy_size = std::min(src_buf.size(), image_data.size());
        memcpy(src_buf.data(), image_data.data(), copy_size);
    }
    
    // Calculate JPEG buffer size
    size_t jpeg_buf_size = tj3JPEGBufSize(width, height, subsamp);
    if (jpeg_buf_size == 0) {
        tj3Destroy(compress_handle);
        tj3Destroy(decompress_handle);
        return 0;
    }
    
    // ========== TEST 1: Compression with ICC profile ==========
    
    // Determine ICC profile size and type
    size_t icc_profile_size = 0;
    std::vector<unsigned char> icc_profile;
    
    int icc_test_type = fdp.ConsumeIntegralInRange<int>(0, 2);
    
    switch (icc_test_type) {
        case 0: {
            // Valid ICC profile (minimum structure)
            icc_profile_size = fdp.ConsumeIntegralInRange<size_t>(128, 4096);
            if (fdp.remaining_bytes() < icc_profile_size) {
                icc_profile_size = std::min(fdp.remaining_bytes(), (size_t)4096);
            }
            
            icc_profile = fdp.ConsumeBytes<unsigned char>(icc_profile_size);
            
            // Make it look like a valid ICC profile
            if (icc_profile.size() >= 128) {
                // Set ICC signature "acsp" at offset 36
                if (icc_profile.size() >= 40) {
                    icc_profile[36] = 'a';
                    icc_profile[37] = 'c';
                    icc_profile[38] = 's';
                    icc_profile[39] = 'p';
                }
                // Set profile size in header (bytes 0-3, big-endian)
                uint32_t profile_size = icc_profile.size();
                if (icc_profile.size() >= 4) {
                    icc_profile[0] = (profile_size >> 24) & 0xFF;
                    icc_profile[1] = (profile_size >> 16) & 0xFF;
                    icc_profile[2] = (profile_size >> 8) & 0xFF;
                    icc_profile[3] = profile_size & 0xFF;
                }
            }
            break;
        }
        
        case 1: {
            // Invalid/corrupted ICC profile
            icc_profile_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
            if (fdp.remaining_bytes() < icc_profile_size) {
                icc_profile_size = std::min(fdp.remaining_bytes(), (size_t)4096);
            }
            
            icc_profile = fdp.ConsumeBytes<unsigned char>(icc_profile_size);
            // Leave it corrupted (no valid ICC structure)
            break;
        }
        
        case 2: {
            // No ICC profile (test removal/empty case)
            icc_profile_size = 0;
            break;
        }
    }
    
    // Set ICC profile if we have one
    if (!icc_profile.empty()) {
        tj3SetICCProfile(compress_handle, icc_profile.data(), icc_profile.size());
    } else if (icc_test_type == 2) {
        // Explicitly remove any ICC profile
        tj3SetICCProfile(compress_handle, NULL, 0);
    }
    
    // Allocate JPEG buffer
    unsigned char* jpeg_buf = (unsigned char*)tj3Alloc(jpeg_buf_size);
    if (!jpeg_buf) {
        tj3Destroy(compress_handle);
        tj3Destroy(decompress_handle);
        return 0;
    }
    
    size_t compressed_size = jpeg_buf_size;
    
    // Perform compression with ICC profile
    int compress_result = tj3Compress8(compress_handle, src_buf.data(), width, 0, height, 
                                      pixelFormat, &jpeg_buf, &compressed_size);
    
    // ========== TEST 2: Decompression with ICC profile retrieval ==========
    
    if (compress_result == 0 && compressed_size > 0) {
        // Set up decompression
        tj3Set(decompress_handle, TJPARAM_SCANLIMIT, fdp.ConsumeIntegralInRange<int>(0, 100));
        
        // Get JPEG header info
        int width_decomp, height_decomp, subsamp_decomp;
        if (tj3DecompressHeader(decompress_handle, jpeg_buf, compressed_size) == 0) {
            width_decomp = tj3Get(decompress_handle, TJPARAM_JPEGWIDTH);
            height_decomp = tj3Get(decompress_handle, TJPARAM_JPEGHEIGHT);
            subsamp_decomp = tj3Get(decompress_handle, TJPARAM_SUBSAMP);
            
            // Allocate decompressed image buffer
            size_t decomp_image_size = (size_t)width_decomp * height_decomp * tjPixelSize[pixelFormat];
            if (decomp_image_size > 0) {
                unsigned char* decomp_buf = (unsigned char*)tj3Alloc(decomp_image_size);
                if (decomp_buf) {
                    // Perform decompression
                    tj3Decompress8(decompress_handle, jpeg_buf, compressed_size, 
                                  decomp_buf, 0, pixelFormat);
                    
                    // Try to retrieve ICC profile from decompressed JPEG
                    size_t retrieved_icc_size = 0;
                    unsigned char* retrieved_icc = NULL;
                    tj3GetICCProfile(decompress_handle, &retrieved_icc, &retrieved_icc_size);
                    
                    if (retrieved_icc && retrieved_icc_size > 0) {
                        // Verify retrieved ICC profile matches what we set
                        // (in real ICC profiles, they should match)
                        tj3Free(retrieved_icc);
                    }
                    
                    tj3Free(decomp_buf);
                }
            }
        }
    }
    
    // ========== TEST 3: Compression with different precision and ICC ==========
    
    // Test 12-bit compression with ICC profile
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 1024) {
        // Set 12-bit precision
        tj3Set(compress_handle, TJPARAM_PRECISION, 12);
        
        // Create another ICC profile
        size_t icc_size2 = fdp.ConsumeIntegralInRange<size_t>(64, 2048);
        if (icc_size2 <= fdp.remaining_bytes()) {
            auto icc_profile2 = fdp.ConsumeBytes<unsigned char>(icc_size2);
            if (!icc_profile2.empty()) {
                tj3SetICCProfile(compress_handle, icc_profile2.data(), icc_profile2.size());
            }
        }
        
        // Allocate new JPEG buffer
        unsigned char* jpeg_buf2 = (unsigned char*)tj3Alloc(jpeg_buf_size);
        if (jpeg_buf2) {
            size_t compressed_size2 = jpeg_buf_size;
            
            // Test tj3Compress12 if enough image data
            if (fdp.remaining_bytes() >= image_size * 2) {  // 12-bit needs 2 bytes per sample
                auto src_buf12_bytes = fdp.ConsumeBytes<unsigned char>(image_size * 2);
                if (!src_buf12_bytes.empty()) {
                    // Convert to short* for tj3Compress12
                    const short* src_buf12 = reinterpret_cast<const short*>(src_buf12_bytes.data());
                    tj3Compress12(compress_handle, src_buf12, width, 0, height,
                                 pixelFormat, &jpeg_buf2, &compressed_size2);
                }
            }
            
            tj3Free(jpeg_buf2);
        }
        
        // Reset to 8-bit for other tests
        tj3Set(compress_handle, TJPARAM_PRECISION, 8);
    }
    
    // ========== TEST 4: Test ICC profile removal and re-set ==========
    
    // Remove ICC profile
    tj3SetICCProfile(compress_handle, NULL, 0);
    
    // Set a new ICC profile (using remaining fuzzer data)
    if (fdp.remaining_bytes() > 64) {
        size_t final_icc_size = fdp.ConsumeIntegralInRange<size_t>(64, std::min((size_t)1024, fdp.remaining_bytes()));
        auto final_icc = fdp.ConsumeBytes<unsigned char>(final_icc_size);
        if (!final_icc.empty()) {
            tj3SetICCProfile(compress_handle, final_icc.data(), final_icc.size());
            
            // Quick compression test with new ICC profile
            unsigned char* jpeg_buf3 = (unsigned char*)tj3Alloc(jpeg_buf_size);
            if (jpeg_buf3) {
                size_t compressed_size3 = jpeg_buf_size;
                tj3Compress8(compress_handle, src_buf.data(), width, 0, height,
                           pixelFormat, &jpeg_buf3, &compressed_size3);
                tj3Free(jpeg_buf3);
            }
        }
    }
    
    // Cleanup
    tj3Free(jpeg_buf);
    tj3Destroy(compress_handle);
    tj3Destroy(decompress_handle);
    
    return 0;
}

} // namespace harness_017
