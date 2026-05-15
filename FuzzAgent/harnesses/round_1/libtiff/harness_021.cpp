#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdarg>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "tiffio.h"

namespace harness_021 {
/*
 * Fuzzing harness for libtiff library - OpenOptions Error/Warning Handler Configuration
 * Targets completely uncovered OpenOptions configuration APIs:
 * - TIFFOpenOptionsSetErrorHandlerExtR: Set extended error handler with file descriptor
 * - TIFFOpenOptionsSetWarningHandlerExtR: Set extended warning handler with file descriptor  
 * - TIFFOpenOptionsSetWarnAboutUnknownTags: Configure warning for unknown tags
 * - TIFFSetTagExtender: Register tag extender callback for custom tags
 * 
 * Coverage analysis indicates these are completely uncovered configuration APIs that form
 * a logical configuration workflow for OpenOptions-based TIFF file handling.
 * 
 * Semantic differentiation from existing harnesses (harness_000 through harness_020):
 * - harness_017 mentions TIFFOpenOptionsSetErrorHandlerExtR/TIFFOpenOptionsSetWarningHandlerExtR 
 *   but doesn't implement them due to type complexity
 * - This harness fully implements the OpenOptions configuration workflow with proper handlers
 * - Specifically targets TIFFOpenOptions-based configuration rather than global handlers
 * - Tests tag extender functionality for custom tag registration
 * - Tests warning configuration for unknown tags
 * - Exercises configuration-before-opening pattern used in production code
 * 
 * Testing strategy:
 * 1. Implement custom error/warning handlers compatible with TIFFErrorHandlerExtR
 * 2. Create TIFFOpenOptions object and configure all four target APIs
 * 3. Open TIFF file with configured options using TIFFOpenExt
 * 4. Trigger errors/warnings to test configured handlers
 * 5. Test tag extender functionality with custom tags
 * 6. Clean up resources properly
 */



// User data structure for error/warning handlers
typedef struct {
    char* buffer;
    size_t buffer_size;
    TIFF* tif_got_from_callback;
    char module[64];
    int call_count;
} HandlerUserData;

// Custom error handler for TIFFErrorHandlerExtR signature
static int TIFF_ATTRIBUTE((__format__(__printf__, 4, 0)))
custom_error_handler_ext_r(TIFF* tiff, void* user_data, const char* module,
                          const char* fmt, va_list ap) {
    HandlerUserData* data = (HandlerUserData*)user_data;
    data->call_count++;
    data->tif_got_from_callback = tiff;
    
    if (module) {
        strncpy(data->module, module, sizeof(data->module) - 1);
        data->module[sizeof(data->module) - 1] = '\0';
    } else {
        data->module[0] = '\0';
    }
    
    if (data->buffer && data->buffer_size > 0) {
        vsnprintf(data->buffer, data->buffer_size, fmt, ap);
    }
    return 1;
}

// Custom warning handler for TIFFErrorHandlerExtR signature (same signature)
static int TIFF_ATTRIBUTE((__format__(__printf__, 4, 0)))
custom_warning_handler_ext_r(TIFF* tiff, void* user_data, const char* module,
                            const char* fmt, va_list ap) {
    HandlerUserData* data = (HandlerUserData*)user_data;
    data->call_count++;
    data->tif_got_from_callback = tiff;
    
    if (module) {
        strncpy(data->module, module, sizeof(data->module) - 1);
        data->module[sizeof(data->module) - 1] = '\0';
    } else {
        data->module[0] = '\0';
    }
    
    if (data->buffer && data->buffer_size > 0) {
        vsnprintf(data->buffer, data->buffer_size, fmt, ap);
    }
    return 1;
}

// Tag extender callback for custom tag registration
static TIFFExtendProc g_parent_extender = NULL;
static int g_extender_called = 0;

static void custom_tag_extender(TIFF* tif) {
    g_extender_called++;
    
    // Define some custom tags for testing
    static const TIFFFieldInfo custom_fields[] = {
        {65000, 1, 1, TIFF_LONG, FIELD_CUSTOM, 0, 0, "CUSTOM_TAG_1"},
        {65001, 1, 1, TIFF_ASCII, FIELD_CUSTOM, 0, 0, "CUSTOM_TAG_2"},
        {65002, TIFF_VARIABLE, TIFF_VARIABLE, TIFF_BYTE, FIELD_CUSTOM, 0, 1, "CUSTOM_TAG_3"},
    };
    
    // Merge custom field info
    TIFFMergeFieldInfo(tif, custom_fields, 
                      sizeof(custom_fields) / sizeof(custom_fields[0]));
    
    // Call parent extender if set
    if (g_parent_extender) {
        (*g_parent_extender)(tif);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for configuration parameters
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Set global error handlers to NULL to avoid interference
    TIFFSetErrorHandler(NULL);
    TIFFSetWarningHandler(NULL);
    
    // Create temporary file for TIFF operations
    char temp_file_template[] = "/tmp/fuzz_tiff_openoptions_XXXXXX";
    int fd = mkstemp(temp_file_template);
    if (fd < 0) {
        return 0;
    }
    std::string temp_filename = temp_file_template;
    
    // Write minimal valid TIFF header
    uint8_t tiff_header[] = {
        0x49, 0x49, 0x2A, 0x00,  // Little-endian, classic TIFF
        0x08, 0x00, 0x00, 0x00,  // First IFD offset
        0x00, 0x00,              // Number of directory entries (0 = end)
        0x00, 0x00, 0x00, 0x00   // Next IFD offset (0 = no more)
    };
    
    if (write(fd, tiff_header, sizeof(tiff_header)) != (ssize_t)sizeof(tiff_header)) {
        close(fd);
        unlink(temp_filename.c_str());
        return 0;
    }
    close(fd);
    
    // Allocate and initialize handler user data
    HandlerUserData error_data = {0};
    HandlerUserData warning_data = {0};
    
    char error_buffer[1024] = {0};
    char warning_buffer[1024] = {0};
    
    error_data.buffer = error_buffer;
    error_data.buffer_size = sizeof(error_buffer);
    warning_data.buffer = warning_buffer;
    warning_data.buffer_size = sizeof(warning_buffer);
    
    // Test 1: OpenOptions with error/warning handlers
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        unlink(temp_filename.c_str());
        return 0;
    }
    
    // Configure OpenOptions with all target APIs
    // 1. Set error handler
    TIFFOpenOptionsSetErrorHandlerExtR(opts, custom_error_handler_ext_r, &error_data);
    
    // 2. Set warning handler  
    TIFFOpenOptionsSetWarningHandlerExtR(opts, custom_warning_handler_ext_r, &warning_data);
    
    // 3. Configure warning about unknown tags (consume boolean from fuzzer input)
    int warn_about_unknown = fdp.ConsumeBool();
    TIFFOpenOptionsSetWarnAboutUnknownTags(opts, warn_about_unknown);
    
    // 4. Set tag extender globally (not part of OpenOptions, but related configuration)
    g_parent_extender = TIFFSetTagExtender(custom_tag_extender);
    
    // Open file with configured options
    TIFF* tif = TIFFOpenExt(temp_filename.c_str(), "r", opts);
    if (!tif) {
        TIFFOpenOptionsFree(opts);
        TIFFSetTagExtender(g_parent_extender); // Restore previous extender
        unlink(temp_filename.c_str());
        return 0;
    }
    
    // Test configured error handlers by triggering an error
    // Try to read directory (should work since it's a minimal valid TIFF)
    int read_result = TIFFReadDirectory(tif);
    
    // Try to trigger an error with invalid operation
    // Attempt to get field that doesn't exist
    uint32_t nonexistent_value;
    int getfield_result = TIFFGetField(tif, 99999, &nonexistent_value);
    
    // Try to trigger a warning
    // Write a warning message
    std::string warning_module = fdp.ConsumeRandomLengthString(32);
    std::string warning_msg = fdp.ConsumeRandomLengthString(256);
    TIFFWarningExtR(tif, warning_module.c_str(), "Test warning: %s", warning_msg.c_str());
    
    // Try to trigger an error
    std::string error_module = fdp.ConsumeRandomLengthString(32);
    std::string error_msg = fdp.ConsumeRandomLengthString(256);
    TIFFErrorExtR(tif, error_module.c_str(), "Test error: %s", error_msg.c_str());
    
    // Test tag extender functionality if it was called
    if (g_extender_called > 0) {
        // Try to set a custom tag value
        uint32_t custom_value = fdp.ConsumeIntegral<uint32_t>();
        TIFFSetField(tif, 65000, custom_value);
        
        // Try to read it back
        uint32_t readback_value;
        if (TIFFGetField(tif, 65000, &readback_value)) {
            // Custom tag was successfully registered and accessible
        }
    }
    
    // Test memory allocation limits (additional OpenOptions APIs for completeness)
    tmsize_t max_single_alloc = fdp.ConsumeIntegralInRange<tmsize_t>(0, 1024 * 1024);
    tmsize_t max_cumulative_alloc = fdp.ConsumeIntegralInRange<tmsize_t>(0, 1024 * 1024 * 10);
    
    // Note: These would require re-opening with new options, so we just demonstrate usage
    // TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_single_alloc);
    // TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, max_cumulative_alloc);
    
    // Clean up
    TIFFClose(tif);
    TIFFOpenOptionsFree(opts);
    
    // Restore previous tag extender
    TIFFSetTagExtender(g_parent_extender);
    
    // Clean up temporary file
    unlink(temp_filename.c_str());
    
    return 0;
}

} // namespace harness_021
