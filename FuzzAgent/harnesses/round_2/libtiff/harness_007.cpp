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

namespace harness_007 {

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

    // Need sufficient data for raw reading operations and field manipulation
    if (Size < 128) {
        return 0;  // Minimum: configuration + buffer sizes + field operations
    }

    FuzzedDataProvider fdp(Data, Size);

    // ============================================
    // 1. CONFIGURATION PHASE: Create test parameters
    // ============================================
    
    // Consume configuration parameters from fuzzer input
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.PickValueInArray({1, 2, 4, 8, 16, 32});
    uint16_t planar_config = fdp.ConsumeBool() ? PLANARCONFIG_CONTIG : PLANARCONFIG_SEPARATE;
    uint16_t photometric = fdp.PickValueInArray({
        PHOTOMETRIC_MINISWHITE, 
        PHOTOMETRIC_MINISBLACK,
        PHOTOMETRIC_RGB,
        PHOTOMETRIC_PALETTE
    });
    
    // Determine if we should create a tiled or stripped image
    bool create_tiled = fdp.ConsumeBool();
    uint32_t tile_width = 0, tile_height = 0;
    uint32_t rows_per_strip = 0;
    
    if (create_tiled) {
        tile_width = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
        tile_height = fdp.ConsumeIntegralInRange<uint32_t>(16, 512);
    } else {
        rows_per_strip = fdp.ConsumeIntegralInRange<uint32_t>(1, 64);
    }
    
    // ============================================
    // 2. CREATE PHASE: Generate TIFF in memory
    // ============================================
    
    // Create a memory stream for writing
    std::ostringstream os;
    TIFF *tif = TIFFStreamOpen("MemTIFF", &os);
    if (!tif) {
        return 0;
    }
    
    // Set basic TIFF tags for writing
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    
    // Set tiling or stripping configuration
    if (create_tiled) {
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
    } else {
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    }
    
    // Optional compression setting
    if (fdp.ConsumeBool()) {
        uint16_t compression = fdp.PickValueInArray({
            COMPRESSION_NONE,
            COMPRESSION_CCITTRLE,
            COMPRESSION_LZW,
            COMPRESSION_PACKBITS,
            COMPRESSION_DEFLATE
        });
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    }
    
    // Write the directory to initialize the TIFF structure
    if (!TIFFWriteDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }
    
    // Now reopen the written data for reading
    std::string tiff_data = os.str();
    std::istringstream is(tiff_data);
    TIFF *read_tif = TIFFStreamOpen("MemTIFFRead", &is);
    if (!read_tif) {
        TIFFClose(tif);
        return 0;
    }
    
    // Close the write handle
    TIFFClose(tif);
    
    // ============================================
    // 3. FIELD MANIPULATION PHASE: Test field APIs
    // ============================================
    
    // Test TIFFGetConfiguredCODECs - enumerate available codecs
    TIFFCodec *codecs = TIFFGetConfiguredCODECs();
    if (codecs) {
        // Iterate through codecs (just to exercise the API)
        TIFFCodec *codec = codecs;
        while (codec->name) {
            // Can't call codec-specific functions without proper setup
            codec++;
        }
        // Note: TIFFGetConfiguredCODECs returns a static array, don't free
    }
    
    // Test field-related APIs
    // Find some fields to manipulate
    uint32_t test_tag = fdp.ConsumeIntegral<uint32_t>() % 65536;
    const TIFFField *field = TIFFFindField(read_tif, test_tag, TIFF_ANY);
    
    if (field) {
        // Test TIFFFieldSetGetCountSize
        int count_size = TIFFFieldSetGetCountSize(field);
        
        // Test TIFFFieldSetGetSize
        int field_size = TIFFFieldSetGetSize(field);
        
        // Test field count methods
        int read_count = TIFFFieldReadCount(field);
        int write_count = TIFFFieldWriteCount(field);
        int pass_count = TIFFFieldPassCount(field);
        
        // Try to unset the field if it exists
        if (fdp.ConsumeBool()) {
            TIFFUnsetField(read_tif, test_tag);
        }
    }
    
    // ============================================
    // 4. RAW READING PHASE: Test raw reading APIs
    // ============================================
    
    // Get image configuration for reading
    uint32_t image_width = 0, image_height = 0;
    TIFFGetField(read_tif, TIFFTAG_IMAGEWIDTH, &image_width);
    TIFFGetField(read_tif, TIFFTAG_IMAGELENGTH, &image_height);
    
    if (image_width == 0 || image_height == 0 || 
        image_width > MAX_SIZE || image_height > MAX_SIZE) {
        TIFFClose(read_tif);
        return 0;
    }
    
    // Check if image is tiled
    int is_tiled = TIFFIsTiled(read_tif);
    
    if (is_tiled) {
        // ============================================
        // 4A. TILE-BASED RAW READING
        // ============================================
        
        uint32_t tile_width_read = 0, tile_height_read = 0;
        TIFFGetFieldDefaulted(read_tif, TIFFTAG_TILEWIDTH, &tile_width_read);
        TIFFGetFieldDefaulted(read_tif, TIFFTAG_TILELENGTH, &tile_height_read);
        
        if (tile_width_read == 0 || tile_height_read == 0) {
            TIFFClose(read_tif);
            return 0;
        }
        
        uint32_t tiles_across = TIFFNumberOfTiles(read_tif);
        if (tiles_across == 0) {
            TIFFClose(read_tif);
            return 0;
        }
        
        // Calculate tile size for buffer allocation
        uint64_t tile_size64 = TIFFTileSize64(read_tif);
        tmsize_t tile_size = TIFFTileSize(read_tif);
        
        if (tile_size64 > MAX_SIZE || tile_size > MAX_SIZE || 
            tile_size64 == 0 || tile_size == 0) {
            TIFFClose(read_tif);
            return 0;
        }
        
        // Allocate buffer for raw tile reading
        std::vector<uint8_t> tile_buffer(tile_size);
        
        // Test TIFFReadRawTile for some tiles
        uint32_t max_tiles_to_test = fdp.ConsumeIntegralInRange<uint32_t>(0, tiles_across);
        for (uint32_t tile_idx = 0; tile_idx < max_tiles_to_test && tile_idx < tiles_across; tile_idx++) {
            tmsize_t bytes_read = TIFFReadRawTile(read_tif, tile_idx, tile_buffer.data(), tile_size);
            // bytes_read may be -1 on error, which is fine for fuzzing
        }
        
        // Test TIFFReadFromUserBuffer with tile data
        if (fdp.ConsumeBool() && tile_size > 0) {
            std::vector<uint8_t> in_buffer(tile_size);
            std::vector<uint8_t> out_buffer(tile_size);
            
            // Fill input buffer with fuzzer data
            auto fuzzer_data = fdp.ConsumeBytes<uint8_t>(tile_size);
            if (fuzzer_data.size() == (size_t)tile_size) {
                memcpy(in_buffer.data(), fuzzer_data.data(), tile_size);
                
                // Test user buffer reading
                uint32_t test_tile = fdp.ConsumeIntegralInRange<uint32_t>(0, tiles_across - 1);
                int result = TIFFReadFromUserBuffer(read_tif, test_tile, 
                                                    in_buffer.data(), tile_size,
                                                    out_buffer.data(), tile_size);
                // Result may be -1 on error, which is fine for fuzzing
            }
        }
        
    } else {
        // ============================================
        // 4B. STRIP-BASED RAW READING
        // ============================================
        
        uint32_t strips_per_image = TIFFNumberOfStrips(read_tif);
        if (strips_per_image == 0) {
            TIFFClose(read_tif);
            return 0;
        }
        
        // Calculate strip size for buffer allocation
        uint64_t strip_size64 = TIFFStripSize64(read_tif);
        tmsize_t strip_size = TIFFStripSize(read_tif);
        
        if (strip_size64 > MAX_SIZE || strip_size > MAX_SIZE || 
            strip_size64 == 0 || strip_size == 0) {
            TIFFClose(read_tif);
            return 0;
        }
        
        // Allocate buffer for raw strip reading
        std::vector<uint8_t> strip_buffer(strip_size);
        
        // Test TIFFReadRawStrip for some strips
        uint32_t max_strips_to_test = fdp.ConsumeIntegralInRange<uint32_t>(0, strips_per_image);
        for (uint32_t strip_idx = 0; strip_idx < max_strips_to_test && strip_idx < strips_per_image; strip_idx++) {
            tmsize_t bytes_read = TIFFReadRawStrip(read_tif, strip_idx, strip_buffer.data(), strip_size);
            // bytes_read may be -1 on error, which is fine for fuzzing
        }
        
        // Test TIFFReadFromUserBuffer with strip data
        if (fdp.ConsumeBool() && strip_size > 0) {
            std::vector<uint8_t> in_buffer(strip_size);
            std::vector<uint8_t> out_buffer(strip_size);
            
            // Fill input buffer with fuzzer data
            auto fuzzer_data = fdp.ConsumeBytes<uint8_t>(strip_size);
            if (fuzzer_data.size() == (size_t)strip_size) {
                memcpy(in_buffer.data(), fuzzer_data.data(), strip_size);
                
                // Test user buffer reading
                uint32_t test_strip = fdp.ConsumeIntegralInRange<uint32_t>(0, strips_per_image - 1);
                int result = TIFFReadFromUserBuffer(read_tif, test_strip, 
                                                    in_buffer.data(), strip_size,
                                                    out_buffer.data(), strip_size);
                // Result may be -1 on error, which is fine for fuzzing
            }
        }
    }
    
    // ============================================
    // 5. FIELD INFO MERGING PHASE: Test TIFFMergeFieldInfo
    // ============================================
    
    // Create custom field info for merging
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
        // Create a simple custom field info structure
        TIFFFieldInfo custom_fields[2];
        
        // Field 1: Custom tag
        custom_fields[0].field_tag = fdp.ConsumeIntegral<uint32_t>() % 65536 + 65000; // Use private tag space
        custom_fields[0].field_readcount = TIFF_VARIABLE;
        custom_fields[0].field_writecount = TIFF_VARIABLE;
        custom_fields[0].field_type = TIFF_ASCII;
        custom_fields[0].field_bit = 0;
        custom_fields[0].field_oktochange = 1;
        custom_fields[0].field_passcount = 0;
        custom_fields[0].field_name = "CustomField1";
        
        // Field 2: Another custom tag
        custom_fields[1].field_tag = fdp.ConsumeIntegral<uint32_t>() % 65536 + 65000; // Use private tag space
        custom_fields[1].field_readcount = 1;
        custom_fields[1].field_writecount = 1;
        custom_fields[1].field_type = TIFF_LONG;
        custom_fields[1].field_bit = 0;
        custom_fields[1].field_oktochange = 1;
        custom_fields[1].field_passcount = 0;
        custom_fields[1].field_name = "CustomField2";
        
        // Try to merge the field info
        int merge_result = TIFFMergeFieldInfo(read_tif, custom_fields, 2);
        // merge_result may be -1 on error, which is fine for fuzzing
    }
    
    // ============================================
    // 6. CLEANUP PHASE
    // ============================================
    
    TIFFClose(read_tif);
    
    return 0;
}

} // namespace harness_007
