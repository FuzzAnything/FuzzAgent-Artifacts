#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>
#include <tiffio.hxx>

namespace harness_002 {

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

// Helper function to create a temporary TIFF file with basic content
bool create_temp_tiff_file(const std::string& filename, const std::vector<uint8_t>& data) {
    FILE* fp = fopen(filename.c_str(), "wb");
    if (!fp) return false;
    
    // Write minimal TIFF header
    uint8_t header[] = {
        0x49, 0x49, 0x2A, 0x00,  // Little-endian, TIFF magic
        0x08, 0x00, 0x00, 0x00,  // IFD offset
        0x00, 0x00,              // Number of directory entries
        0x00, 0x00, 0x00, 0x00   // Next IFD offset
    };
    fwrite(header, 1, sizeof(header), fp);
    
    // Write some fuzzer data if available
    if (!data.empty()) {
        fwrite(data.data(), 1, std::min(data.size(), (size_t)1024), fp);
    }
    
    fclose(fp);
    return true;
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

    if (Size < 32) {
        return 0;  // Need minimum data for meaningful testing
    }

    FuzzedDataProvider fdp(Data, Size);

    // Create a temporary file for file-based operations
    char temp_filename[] = "/tmp/temp_tiff_XXXXXX.tiff";
    int fd = mkstemps(temp_filename, 6);
    if (fd < 0) {
        return 0;
    }
    close(fd);  // Close it so TIFFOpen can open it

    // Create some data for the temporary file
    std::vector<uint8_t> temp_data;
    if (fdp.remaining_bytes() > 1024) {
        temp_data = fdp.ConsumeBytes<uint8_t>(1024);
    } else {
        temp_data = fdp.ConsumeRemainingBytes<uint8_t>();
    }
    
    if (!create_temp_tiff_file(temp_filename, temp_data)) {
        unlink(temp_filename);
        return 0;
    }

    // 1. FILE OPENING APIS - TIFFOpen family
    TIFF* tif = nullptr;
    uint8_t open_method = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (open_method) {
        case 0:
            // TIFFOpen - standard file opening
            tif = TIFFOpen(temp_filename, fdp.ConsumeBool() ? "r" : "w");
            break;
        case 1:
            // TIFFOpenExt - extended file opening with options
            {
                TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
                if (opts) {
                    // Configure some options
                    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, fdp.ConsumeIntegral<uint64_t>());
                    TIFFOpenOptionsSetWarnAboutUnknownTags(opts, fdp.ConsumeBool());
                    tif = TIFFOpenExt(temp_filename, fdp.ConsumeBool() ? "r" : "w", opts);
                    TIFFOpenOptionsFree(opts);
                } else {
                    tif = TIFFOpen(temp_filename, fdp.ConsumeBool() ? "r" : "w");
                }
            }
            break;
        case 2:
            // TIFFFdOpen - file descriptor based
            {
                int tiff_fd = open(temp_filename, O_RDWR);
                if (tiff_fd >= 0) {
                    tif = TIFFFdOpen(tiff_fd, temp_filename, fdp.ConsumeBool() ? "r" : "w");
                    // Note: TIFFFdOpen takes ownership of the fd
                }
            }
            break;
        case 3:
            // TIFFFdOpenExt - extended file descriptor based
            {
                int tiff_fd = open(temp_filename, O_RDWR);
                if (tiff_fd >= 0) {
                    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
                    if (opts) {
                        TIFFOpenOptionsSetMaxSingleMemAlloc(opts, fdp.ConsumeIntegral<uint64_t>());
                        TIFFOpenOptionsSetWarnAboutUnknownTags(opts, fdp.ConsumeBool());
                        tif = TIFFFdOpenExt(tiff_fd, temp_filename, fdp.ConsumeBool() ? "r" : "w", opts);
                        TIFFOpenOptionsFree(opts);
                    } else {
                        tif = TIFFFdOpen(tiff_fd, temp_filename, fdp.ConsumeBool() ? "r" : "w");
                    }
                }
            }
            break;
    }

    if (!tif) {
        unlink(temp_filename);
        return 0;
    }

    // 2. CONFIGURATION APIS - TIFFSetField/TIFFVSetField
    // Set basic image parameters
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.PickValueInArray({1, 2, 4, 8, 16, 32});
    uint16_t planar_config = fdp.ConsumeBool() ? PLANARCONFIG_CONTIG : PLANARCONFIG_SEPARATE;
    uint16_t photometric = fdp.PickValueInArray({PHOTOMETRIC_MINISWHITE, PHOTOMETRIC_MINISBLACK,
                                                 PHOTOMETRIC_RGB});
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    // Note: TIFFVSetField is intentionally omitted as it requires va_list and is harder to use
    TIFFSetDirectory(tif, 0);
    // First write a directory to have something to navigate to
    TIFFWriteDirectory(tif);
    // Try to set subdirectory if we have one
    if (TIFFNumberOfDirectories(tif) > 1) {
        TIFFSetSubDirectory(tif, 0);
    }

    // 4. RAW WRITING OPERATIONS - TIFFWriteRawStrip/TIFFWriteRawTile
    // Calculate buffer sizes
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    tmsize_t strip_size = TIFFStripSize(tif);
    tmsize_t tile_size = TIFFTileSize(tif);
    
    // Don't continue if sizes are too large
    if (scanline_size > MAX_SIZE || strip_size > MAX_SIZE || tile_size > MAX_SIZE ||
        scanline_size == 0 || strip_size == 0 || tile_size == 0) {
        TIFFClose(tif);
        unlink(temp_filename);
        return 0;
    }

    // Allocate buffers for raw writing
    std::vector<uint8_t> raw_buffer;
    tmsize_t raw_data_size = 0;
    
    // Choose between strip and tile operations
    bool use_tiles = fdp.ConsumeBool();
    
    if (use_tiles && TIFFIsTiled(tif)) {
        // Configure tiles
        uint32_t tile_width = 256;
        uint32_t tile_height = 256;
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_height);
        
        raw_data_size = tile_size;
        if (raw_data_size > 0 && raw_data_size <= MAX_SIZE) {
            raw_buffer.resize(raw_data_size);
            // Fill with fuzzer data if available
            if (fdp.remaining_bytes() >= (size_t)raw_data_size) {
                auto tile_data = fdp.ConsumeBytes<uint8_t>(raw_data_size);
                if (tile_data.size() == (size_t)raw_data_size) {
                    memcpy(raw_buffer.data(), tile_data.data(), raw_data_size);
                }
            }
            
            // Try TIFFWriteRawTile
            uint32_t tile_num = fdp.ConsumeIntegralInRange<uint32_t>(0, TIFFNumberOfTiles(tif) - 1);
            if (TIFFNumberOfTiles(tif) > 0) {
                TIFFWriteRawTile(tif, tile_num, raw_buffer.data(), raw_data_size);
            }
        }
    } else {
        // Use strips
        raw_data_size = strip_size;
        if (raw_data_size > 0 && raw_data_size <= MAX_SIZE) {
            raw_buffer.resize(raw_data_size);
            // Fill with fuzzer data if available
            if (fdp.remaining_bytes() >= (size_t)raw_data_size) {
                auto strip_data = fdp.ConsumeBytes<uint8_t>(raw_data_size);
                if (strip_data.size() == (size_t)raw_data_size) {
                    memcpy(raw_buffer.data(), strip_data.data(), raw_data_size);
                }
            }
            
            // Try TIFFWriteRawStrip
            uint32_t strip_num = fdp.ConsumeIntegralInRange<uint32_t>(0, TIFFNumberOfStrips(tif) - 1);
            if (TIFFNumberOfStrips(tif) > 0) {
                TIFFWriteRawStrip(tif, strip_num, raw_buffer.data(), raw_data_size);
            }
        }
    }

    // 5. CUSTOM DIRECTORY OPERATIONS - TIFFWriteCustomDirectory
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 128) {
        // Create a simple custom directory
        std::vector<uint8_t> custom_dir_data = fdp.ConsumeBytes<uint8_t>(128);
        if (custom_dir_data.size() == 128) {
            // TIFFWriteCustomDirectory needs a pointer to store the offset
            uint64_t dir_offset = 0;
            TIFFWriteCustomDirectory(tif, &dir_offset);
        }
    }

    // 6. CHECKPOINT OPERATION - TIFFCheckpointDirectory
    if (fdp.ConsumeBool()) {
        TIFFCheckpointDirectory(tif);
    }

    // 7. CLEANUP - TIFFClose
    TIFFClose(tif);
    
    // Clean up temporary file
    unlink(temp_filename);

    return 0;
}

} // namespace harness_002
