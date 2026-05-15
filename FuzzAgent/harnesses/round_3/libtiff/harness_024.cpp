#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "tiffio.h"

namespace harness_024 {
/*
 * Fuzzing harness for libtiff error handling and open options configuration.
 * Targets: TIFFWarning, TIFFError, TIFFWarningExt, TIFFSetWarningHandlerExt,
 * TIFFSetErrorHandlerExt, TIFFOpenOptionsSetErrorHandlerExtR,
 * TIFFOpenOptionsSetWarningHandlerExtR, TIFFOpenOptionsSetMaxCumulatedMemAlloc,
 * TIFFOpenOptionsSetMaxSingleMemAlloc, TIFFOpenOptionsSetWarnAboutUnknownTags.
 * Required helpers: TIFFOpenOptionsAlloc, TIFFOpenOptionsFree,
 * TIFFOpenExt or TIFFFdOpenExt, TIFFClose.
 */



// Custom error/warning handler data structure
struct HandlerUserData {
    char error_buffer[1024];
    char warning_buffer[1024];
    char error_module[64];
    char warning_module[64];
    int error_count;
    int warning_count;
    TIFF* tif_from_callback;
};

// Custom error handler function for use with TIFFOpenOptionsSetErrorHandlerExtR
static int TIFF_ATTRIBUTE((__format__(__printf__, 4, 0)))
custom_error_handler(TIFF* tiff, void* user_data, const char* module,
                     const char* fmt, va_list ap) {
    HandlerUserData* data = static_cast<HandlerUserData*>(user_data);
    vsnprintf(data->error_buffer, sizeof(data->error_buffer), fmt, ap);
    snprintf(data->error_module, sizeof(data->error_module), "%s", module);
    data->error_count++;
    data->tif_from_callback = tiff;
    return 1; // Return 1 to indicate we handled it
}

// Custom warning handler function for use with TIFFOpenOptionsSetWarningHandlerExtR
static int TIFF_ATTRIBUTE((__format__(__printf__, 4, 0)))
custom_warning_handler(TIFF* tiff, void* user_data, const char* module,
                       const char* fmt, va_list ap) {
    HandlerUserData* data = static_cast<HandlerUserData*>(user_data);
    vsnprintf(data->warning_buffer, sizeof(data->warning_buffer), fmt, ap);
    snprintf(data->warning_module, sizeof(data->warning_module), "%s", module);
    data->warning_count++;
    data->tif_from_callback = tiff;
    return 1; // Return 1 to indicate we handled it
}

// Global error/warning handlers for TIFFSetErrorHandlerExt/TIFFSetWarningHandlerExt
static int global_error_count = 0;
static int global_warning_count = 0;

static void TIFF_ATTRIBUTE((__format__(__printf__, 3, 0)))
global_error_handler(thandle_t client_data, const char* module,
                     const char* fmt, va_list ap) {
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
    global_error_count++;
}

static void TIFF_ATTRIBUTE((__format__(__printf__, 3, 0)))
global_warning_handler(thandle_t client_data, const char* module,
                       const char* fmt, va_list ap) {
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
    global_warning_count++;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 20) {
        return 0; // Need minimum input for configuration
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // First test: Direct error/warning handler setting without open options
    {
        // Set global handlers
        TIFFErrorHandlerExt prev_error_handler = TIFFSetErrorHandlerExt(global_error_handler);
        TIFFErrorHandlerExt prev_warning_handler = TIFFSetWarningHandlerExt(global_warning_handler);
        
        // Reset counters
        global_error_count = 0;
        global_warning_count = 0;
        
        // Trigger some warnings/errors directly
        TIFFWarning("test_module", "Test warning %d", fdp.ConsumeIntegral<int>());
        TIFFError("test_module", "Test error %d", fdp.ConsumeIntegral<int>());
        
        // Also test Ext versions
        TIFFWarningExt(nullptr, "test_ext_module", "Test warning ext %d", 
                      fdp.ConsumeIntegral<int>());
        TIFFErrorExt(nullptr, "test_ext_module", "Test error ext %d",
                    fdp.ConsumeIntegral<int>());
        
        // Restore previous handlers
        TIFFSetErrorHandlerExt(prev_error_handler);
        TIFFSetWarningHandlerExt(prev_warning_handler);
    }
    
    // Second test: Open options with custom handlers and memory limits
    {
        // Allocate options structure
        TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
        if (!opts) {
            return 0; // Failed to allocate options
        }
        
        // Prepare user data for handlers
        HandlerUserData handler_data = {};
        handler_data.error_count = 0;
        handler_data.warning_count = 0;
        handler_data.tif_from_callback = nullptr;
        
        // Set custom error and warning handlers
        TIFFOpenOptionsSetErrorHandlerExtR(opts, custom_error_handler, &handler_data);
        TIFFOpenOptionsSetWarningHandlerExtR(opts, custom_warning_handler, &handler_data);
        
        // Set memory allocation limits based on fuzzed input
        tmsize_t max_single_mem = fdp.ConsumeIntegralInRange<tmsize_t>(0, 1024 * 1024); // 0-1MB
        tmsize_t max_cumulated_mem = fdp.ConsumeIntegralInRange<tmsize_t>(0, 10 * 1024 * 1024); // 0-10MB
        
        TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_single_mem);
        TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, max_cumulated_mem);
        
        // Set warn about unknown tags flag
        int warn_unknown_tags = fdp.ConsumeBool();
        TIFFOpenOptionsSetWarnAboutUnknownTags(opts, warn_unknown_tags);
        
        // Create a test TIFF file in memory or use fd approach
        // We'll create a temporary file for testing
        const char* filename = "/tmp/test_fuzz_tiff.tif";
        const char* mode = fdp.ConsumeBool() ? "w" : "w8"; // Regular or BigTIFF
        
        // Open TIFF with configured options
        TIFF* tif = TIFFOpenExt(filename, mode, opts);
        
        // Free options - they are copied internally during TIFFOpenExt
        TIFFOpenOptionsFree(opts);
        
        if (tif) {
            // Now trigger some warnings/errors to test the handlers
            
            // Test 1: Trigger a warning by setting an unusual field value
            TIFFSetField(tif, TIFFTAG_COMPRESSION, 
                        fdp.ConsumeIntegralInRange<uint16_t>(0, 65535));
            
            // Test 2: Try to trigger error by attempting invalid operations
            // Try to read from write-only file
            uint32_t width, height;
            if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) == 0) {
                // This might trigger error/warning
            }
            if (TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height) == 0) {
                // This might trigger error/warning
            }
            
            // Test 3: Direct error/warning emission using ExtR functions
            std::string error_msg = fdp.ConsumeRandomLengthString(256);
            std::string warning_msg = fdp.ConsumeRandomLengthString(256);
            
            TIFFErrorExtR(tif, "fuzzer_error_module", "%s", error_msg.c_str());
            TIFFWarningExtR(tif, "fuzzer_warning_module", "%s", warning_msg.c_str());
            
            // Test 4: Write some minimal directory to exercise more code paths
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, fdp.ConsumeIntegralInRange<uint32_t>(1, 1000));
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, fdp.ConsumeIntegralInRange<uint32_t>(1, 1000));
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
            TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
            TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 
                        fdp.ConsumeIntegralInRange<uint32_t>(1, 100));
            
            // This might trigger memory limit errors if limits are too low
            TIFFWriteDirectory(tif);
            
            // Clean up
            TIFFClose(tif);
            
            // Remove temporary file
            remove(filename);
        } else {
            // TIFFOpenExt failed, which is OK - might be due to memory limits
            // or other constraints
        }
        
        // Note: handler_data should have been updated by callbacks if any
        // warnings/errors were triggered
    }
    
    // Third test: Test with TIFFFdOpenExt using file descriptor
    {
        // Create a simple in-memory TIFF file first
        std::vector<uint8_t> tiff_data;
        
        // Minimal TIFF header (Little-endian, IFD offset = 8)
        uint8_t header[] = {
            0x49, 0x49, 0x2A, 0x00, // Little-endian, magic number
            0x08, 0x00, 0x00, 0x00, // First IFD offset
            0x00, 0x00,             // Number of directory entries
            0x00, 0x00, 0x00, 0x00  // Next IFD offset (0 = end)
        };
        
        tiff_data.insert(tiff_data.end(), header, header + sizeof(header));
        
        // Write to a temporary file
        const char* fd_filename = "/tmp/test_fuzz_fd.tif";
        FILE* tmp_file = fopen(fd_filename, "wb");
        if (tmp_file) {
            fwrite(tiff_data.data(), 1, tiff_data.size(), tmp_file);
            fclose(tmp_file);
            
            // Reopen for reading with custom options
            tmp_file = fopen(fd_filename, "rb");
            if (tmp_file) {
                int fd = fileno(tmp_file);
                
                // Allocate new options
                TIFFOpenOptions* fd_opts = TIFFOpenOptionsAlloc();
                if (fd_opts) {
                    // Set some random configuration
                    if (fdp.ConsumeBool()) {
                        tmsize_t limit = fdp.ConsumeIntegralInRange<tmsize_t>(1, 10000);
                        TIFFOpenOptionsSetMaxSingleMemAlloc(fd_opts, limit);
                    }
                    
                    if (fdp.ConsumeBool()) {
                        int warn = fdp.ConsumeBool();
                        TIFFOpenOptionsSetWarnAboutUnknownTags(fd_opts, warn);
                    }
                    
                    // Try to open with file descriptor
                    TIFF* fd_tif = TIFFFdOpenExt(fd, fd_filename, "r", fd_opts);
                    TIFFOpenOptionsFree(fd_opts);
                    
                    if (fd_tif) {
                        // Try some operations that might trigger warnings
                        uint32_t dir_count = 0;
                        do {
                            // Try to read fields - might trigger warnings for missing fields
                            uint32_t width, height;
                            TIFFGetField(fd_tif, TIFFTAG_IMAGEWIDTH, &width);
                            TIFFGetField(fd_tif, TIFFTAG_IMAGELENGTH, &height);
                            
                            dir_count++;
                        } while (TIFFReadDirectory(fd_tif));
                        
                        TIFFClose(fd_tif);
                    }
                    
                    fclose(tmp_file);
                } else {
                    fclose(tmp_file);
                }
                
                remove(fd_filename);
            }
        }
    }
    
    return 0;
}

} // namespace harness_024
