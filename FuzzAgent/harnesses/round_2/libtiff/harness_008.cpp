#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>
#include <tiffio.hxx>

namespace harness_008 {

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

// Custom I/O handlers for TIFFClientOpen
extern "C" tmsize_t client_read(thandle_t handle, void *buf, tmsize_t size) {
    // Simple read implementation - read from memory buffer
    std::vector<uint8_t>* data = reinterpret_cast<std::vector<uint8_t>*>(handle);
    tmsize_t bytes_to_read = size;
    if (bytes_to_read > (tmsize_t)data->size()) {
        bytes_to_read = (tmsize_t)data->size();
    }
    if (bytes_to_read > 0) {
        memcpy(buf, data->data(), bytes_to_read);
    }
    return bytes_to_read;
}

extern "C" tmsize_t client_write(thandle_t handle, void *buf, tmsize_t size) {
    // Simple write implementation - ignore for fuzzing
    (void)handle;
    (void)buf;
    return size;
}

extern "C" toff_t client_seek(thandle_t handle, toff_t offset, int whence) {
    std::vector<uint8_t>* data = reinterpret_cast<std::vector<uint8_t>*>(handle);
    toff_t new_offset = 0;
    
    switch (whence) {
        case SEEK_SET:
            new_offset = offset;
            break;
        case SEEK_CUR:
            new_offset = offset; // Simplified
            break;
        case SEEK_END:
            new_offset = data->size() + offset;
            break;
        default:
            return (toff_t)-1;
    }
    
    if (new_offset < 0 || new_offset > (toff_t)data->size()) {
        return (toff_t)-1;
    }
    
    return new_offset;
}

extern "C" int client_close(thandle_t handle) {
    // Close handler - just return success
    (void)handle;
    return 0;
}

extern "C" toff_t client_size(thandle_t handle) {
    std::vector<uint8_t>* data = reinterpret_cast<std::vector<uint8_t>*>(handle);
    return (toff_t)data->size();
}

extern "C" int client_map(thandle_t handle, void **base, toff_t *size) {
    // Map handler - return failure (unused in fuzzing)
    (void)handle;
    (void)base;
    (void)size;
    return 0;
}

extern "C" void client_unmap(thandle_t handle, void *base, toff_t size) {
    // Unmap handler
    (void)handle;
    (void)base;
    (void)size;
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

    if (Size < 64) {
        return 0;  // Need minimum data for comprehensive opening operations
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

    // ============================================
    // 1. ERROR HANDLER SETUP
    // ============================================
    // Already done at the beginning of the function
    
    // ============================================
    // 2. OPEN OPTIONS CREATION
    // ============================================
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        unlink(temp_filename);
        return 0;
    }
    
    // Configure options based on fuzzer input
    if (fdp.ConsumeBool()) {
        uint64_t max_alloc = fdp.ConsumeIntegral<uint64_t>();
        TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_alloc);
    }
    
    if (fdp.ConsumeBool()) {
        TIFFOpenOptionsSetWarnAboutUnknownTags(opts, fdp.ConsumeBool());
    }
    
    // ============================================
    // 3. COMPREHENSIVE FILE OPENING TESTS
    // ============================================
    // Test different opening methods based on fuzzer input
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    TIFF* tif = nullptr;
    
    switch (test_scenario) {
        case 0: {
            // TIFFOpen - standard file opening
            std::string mode = fdp.ConsumeBool() ? "r" : "w";
            tif = TIFFOpen(temp_filename, mode.c_str());
            break;
        }
        case 1: {
            // TIFFOpenExt - extended file opening with options
            std::string mode = fdp.ConsumeBool() ? "r" : "w";
            tif = TIFFOpenExt(temp_filename, mode.c_str(), opts);
            break;
        }
        case 2: {
            // TIFFFdOpen - file descriptor based
            int tiff_fd = open(temp_filename, O_RDWR);
            if (tiff_fd >= 0) {
                std::string mode = fdp.ConsumeBool() ? "r" : "w";
                tif = TIFFFdOpen(tiff_fd, temp_filename, mode.c_str());
                // Note: TIFFFdOpen takes ownership of the fd
            }
            break;
        }
        case 3: {
            // TIFFFdOpenExt - extended file descriptor based
            int tiff_fd = open(temp_filename, O_RDWR);
            if (tiff_fd >= 0) {
                std::string mode = fdp.ConsumeBool() ? "r" : "w";
                tif = TIFFFdOpenExt(tiff_fd, temp_filename, mode.c_str(), opts);
            }
            break;
        }
        case 4: {
            // TIFFClientOpen - custom I/O handlers (without options)
            std::vector<uint8_t> client_data = temp_data;
            std::string mode = fdp.ConsumeBool() ? "r" : "w";
            tif = TIFFClientOpen("client_tiff", mode.c_str(), 
                                reinterpret_cast<thandle_t>(&client_data),
                                client_read, client_write,
                                client_seek, client_close,
                                client_size, client_map, client_unmap);
            break;
        }
        case 5: {
            // TIFFClientOpenExt - custom I/O handlers with options
            std::vector<uint8_t> client_data = temp_data;
            std::string mode = fdp.ConsumeBool() ? "r" : "w";
            tif = TIFFClientOpenExt("client_tiff_ext", mode.c_str(),
                                   reinterpret_cast<thandle_t>(&client_data),
                                   client_read, client_write,
                                   client_seek, client_close,
                                   client_size, client_map, client_unmap,
                                   opts);
            break;
        }
    }

    if (!tif) {
        TIFFOpenOptionsFree(opts);
        unlink(temp_filename);
        return 0;
    }

    // ============================================
    // 4. BASIC OPERATIONS ON OPENED FILE
    // ============================================
    // Try to read basic information from the opened file
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    // Check directory information
    tdir_t dir_count = TIFFNumberOfDirectories(tif);
    tdir_t current_dir = TIFFCurrentDirectory(tif);
    
    // Try to set directory if multiple directories exist
    if (dir_count > 1 && fdp.ConsumeBool()) {
        uint32_t dir_index = fdp.ConsumeIntegralInRange<uint32_t>(0, dir_count - 1);
        TIFFSetDirectory(tif, dir_index);
    }
    
    // Test basic properties
    int is_tiled = TIFFIsTiled(tif);
    int is_big_tiff = TIFFIsBigTIFF(tif);
    int is_msb2lsb = TIFFIsMSB2LSB(tif);
    
    // ============================================
    // 5. CLOSE AND CLEANUP
    // ============================================
    TIFFClose(tif);
    TIFFOpenOptionsFree(opts);
    
    // ============================================
    // 6. CLEANUP TEMPORARY FILE
    // ============================================
    // Clean up temporary file
    unlink(temp_filename);

    return 0;
}

} // namespace harness_008
