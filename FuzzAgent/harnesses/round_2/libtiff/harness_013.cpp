#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>
#include <tiffio.hxx>

namespace harness_013 {

/* stolen from tiffiop.h, which is a private header so we can't just include it */
/* safe multiply returns either the multiplied value or 0 if it overflowed */
#define __TIFFSafeMultiply(t, v, m)                                            \
    ((((t)(m) != (t)0) && (((t)(((v) * (m)) / (m))) == (t)(v)))                \
         ? (t)((v) * (m))                                                      \
         : (t)0)

const uint64_t MAX_SIZE = 500000000;

extern "C" void handle_error(const char *unused, const char *unused2,
                             va_list unused3)
{
    return;
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
#ifndef STANDALONE
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);
#endif

#if defined(__has_feature)
#if __has_feature(memory_sanitizer)
    // libjpeg-turbo has issues with MSAN and SIMD code
    // See https://bugs.chromium.org/p/oss-fuzz/issues/detail?id=7547
    // and https://github.com/libjpeg-turbo/libjpeg-turbo/pull/365
    setenv("JSIMD_FORCENONE", "1", 1);
#endif
#endif

    // Need sufficient data for directory manipulation operations
    if (Size < 64) {
        return 0;  // Minimum data for meaningful testing
    }

    FuzzedDataProvider fdp(Data, Size);

    // Create a memory stream for operations (avoid file I/O dependencies)
    std::ostringstream os;
    TIFF *tif = TIFFStreamOpen("MemTIFF", &os);
    if (!tif) {
        return 0;
    }

    // Track directory offsets for TIFFSetSubDirectory testing
    std::vector<uint64_t> directory_offsets;
    std::vector<tdir_t> directory_indices;
    
    // Create multiple directories with varying configurations
    const int NUM_DIRECTORIES = 3;
    for (int i = 0; i < NUM_DIRECTORIES; i++) {
        // Set basic TIFF tags for each directory
        uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 256);
        uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 256);
        uint16_t samples = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
        uint16_t bits = fdp.PickValueInArray({1, 2, 4, 8, 16});
        uint16_t photometric = fdp.PickValueInArray({
            PHOTOMETRIC_MINISWHITE, PHOTOMETRIC_MINISBLACK,
            PHOTOMETRIC_RGB
        });
        
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
        
        // Write the directory
        if (!TIFFWriteDirectory(tif)) {
            TIFFClose(tif);
            return 0;
        }
        
        // Get current directory offset for TIFFSetSubDirectory testing
        uint64_t offset = TIFFCurrentDirOffset(tif);
        directory_offsets.push_back(offset);
        directory_indices.push_back(TIFFCurrentDirectory(tif));
        
        // Create a new directory for next iteration
        TIFFCreateDirectory(tif);
    }
    
    // Now test the target directory manipulation APIs
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (test_scenario) {
        case 0: {
            // Test TIFFSetSubDirectory with actual directory offsets
            if (!directory_offsets.empty()) {
                size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, directory_offsets.size() - 1);
                TIFFSetSubDirectory(tif, directory_offsets[idx]);
                
                // Also test with invalid offset
                if (fdp.ConsumeBool()) {
                    TIFFSetSubDirectory(tif, fdp.ConsumeIntegral<uint64_t>());
                }
                
                // Test with offset 0 to invalidate directory
                if (fdp.ConsumeBool()) {
                    TIFFSetSubDirectory(tif, 0);
                }
            }
            break;
        }
        
        case 1: {
            // Test TIFFCheckpointDirectory
            if (!directory_indices.empty()) {
                // Navigate to a random directory
                size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, directory_indices.size() - 1);
                TIFFSetDirectory(tif, directory_indices[idx]);
                
                // Checkpoint the directory
                TIFFCheckpointDirectory(tif);
                
                // Test multiple checkpoints
                if (fdp.ConsumeBool()) {
                    TIFFCheckpointDirectory(tif);
                }
            }
            break;
        }
        
        case 2: {
            // Test TIFFUnlinkDirectory
            if (directory_indices.size() > 1) {
                // Unlink a random directory (not the first)
                size_t idx = fdp.ConsumeIntegralInRange<size_t>(1, directory_indices.size() - 1);
                TIFFUnlinkDirectory(tif, directory_indices[idx]);
                
                // Try to navigate to unlinked directory
                if (fdp.ConsumeBool()) {
                    TIFFSetDirectory(tif, directory_indices[idx]);
                }
            }
            break;
        }
        
        case 3: {
            // Test TIFFDeferStrileArrayWriting
            // Navigate to a directory first
            if (!directory_indices.empty()) {
                size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, directory_indices.size() - 1);
                TIFFSetDirectory(tif, directory_indices[idx]);
                
                // Defer strile array writing
                TIFFDeferStrileArrayWriting(tif);
                
                // Try to write some data (simulated)
                uint32_t data_size = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
                std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(data_size);
                // Note: Actual strile writing would require more TIFF setup
            }
            break;
        }
        
        case 4: {
            // Test directory lifecycle: Create -> Checkpoint -> Navigate -> Unlink
            // Create a new directory
            TIFFCreateDirectory(tif);
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32_t)10);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, (uint32_t)10);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, (uint16_t)8);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, (uint16_t)3);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
            
            if (TIFFWriteDirectory(tif)) {
                tdir_t new_dir = TIFFCurrentDirectory(tif);
                uint64_t new_offset = TIFFCurrentDirOffset(tif);
                
                // Checkpoint
                TIFFCheckpointDirectory(tif);
                
                // Navigate using SetSubDirectory
                TIFFSetSubDirectory(tif, new_offset);
                
                // Unlink
                TIFFUnlinkDirectory(tif, new_dir);
            }
            break;
        }
        
        case 5: {
            // Test error/recovery scenarios
            // Invalidate with offset 0
            TIFFSetSubDirectory(tif, 0);
            
            // Try operations on invalid directory
            TIFFCheckpointDirectory(tif);
            TIFFDeferStrileArrayWriting(tif);
            
            // Create new directory to recover
            TIFFCreateDirectory(tif);
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32_t)5);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, (uint32_t)5);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, (uint16_t)8);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, (uint16_t)1);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
            TIFFWriteDirectory(tif);
            break;
        }
    }
    
    // Test directory navigation functions
    if (fdp.ConsumeBool() && !directory_indices.empty()) {
        // Test TIFFSetDirectory and TIFFReadDirectory
        for (size_t i = 0; i < directory_indices.size() && i < 3; i++) {
            TIFFSetDirectory(tif, directory_indices[i]);
            TIFFReadDirectory(tif);
        }
        
        // Test TIFFNumberOfDirectories
        tdir_t num_dirs = TIFFNumberOfDirectories(tif);
        (void)num_dirs; // Use to avoid unused variable warning
    }
    
    // Clean up
    TIFFClose(tif);
    
    return 0;
}

} // namespace harness_013
