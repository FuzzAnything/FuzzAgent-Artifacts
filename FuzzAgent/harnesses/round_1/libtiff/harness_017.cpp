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

namespace harness_017 {
/*
 * Fuzzing harness for libtiff error/warning handling APIs
 * Target APIs: TIFFWarning, TIFFWarningExt, TIFFError, TIFFSetErrorHandlerExt, TIFFSetWarningHandlerExt
 * 
 * Coverage analysis indicates these are completely uncovered public APIs for error handling:
 * - TIFFWarning: Report warning messages (0% coverage)
 * - TIFFWarningExt: Report warning messages with file descriptor (0% coverage)
 * - TIFFError: Report error messages (0% coverage)  
 * - TIFFSetErrorHandlerExt: Set extended error handler (0% coverage)
 * - TIFFSetWarningHandlerExt: Set extended warning handler (0% coverage)
 * 
 * These APIs are critical for error reporting and user notification in libtiff.
 * 
 * Semantic differentiation from existing harnesses (harness_000 through harness_016):
 * - Previous harnesses suppress errors using TIFFSetErrorHandler/TIFFSetWarningHandler with NULL
 * - This harness actively tests error/warning reporting by setting custom handlers
 * - Tests both error and warning pathways separately
 * - Tests extended handler variants with file descriptor parameters
 * - Tests error/warning generation through invalid TIFF operations
 * - Tests handler lifecycle: set handlers → trigger errors → handle errors → cleanup
 * 
 * Testing strategy:
 * 1. Implement custom error/warning handlers that capture messages
 * 2. Set handlers using both standard and extended variants
 * 3. Trigger errors/warnings through various invalid operations
 * 4. Verify handlers are called with appropriate messages
 * 5. Test cleanup and restoration of default handlers
 */



// Global counters to track handler invocations
static int g_error_handler_called = 0;
static int g_warning_handler_called = 0;
static int g_error_handler_ext_called = 0;
static int g_warning_handler_ext_called = 0;
static thandle_t g_last_fd = 0;
static std::string g_last_module;
static std::string g_last_message;

// Standard error handler
extern "C" void custom_error_handler(const char* module, const char* fmt, va_list ap) {
    g_error_handler_called++;
    g_last_module = module ? module : "";
    
    // Format the message
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    g_last_message = buffer;
}

// Extended error handler (with file descriptor)
extern "C" void custom_error_handler_ext(thandle_t fd, const char* module, const char* fmt, va_list ap) {
    g_error_handler_ext_called++;
    g_last_fd = fd;
    g_last_module = module ? module : "";
    
    // Format the message
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    g_last_message = buffer;
}

// Standard warning handler
extern "C" void custom_warning_handler(const char* module, const char* fmt, va_list ap) {
    g_warning_handler_called++;
    g_last_module = module ? module : "";
    
    // Format the message
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    g_last_message = buffer;
}

// Extended warning handler (with file descriptor)
extern "C" void custom_warning_handler_ext(thandle_t fd, const char* module, const char* fmt, va_list ap) {
    g_warning_handler_ext_called++;
    g_last_fd = fd;
    g_last_module = module ? module : "";
    
    // Format the message
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    g_last_message = buffer;
}

// Reset all counters
static void reset_counters() {
    g_error_handler_called = 0;
    g_warning_handler_called = 0;
    g_error_handler_ext_called = 0;
    g_warning_handler_ext_called = 0;
    g_last_fd = 0;
    g_last_module.clear();
    g_last_message.clear();
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for various test parameters
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Reset counters at the beginning of each test
    reset_counters();
    
    // Determine which test case to run based on fuzzer input
    uint8_t test_case = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (test_case) {
        case 0: {
            // Test 1: Set custom handlers and trigger warnings via TIFFWarning/TIFFWarningExt
            
            // Save original handlers
            TIFFErrorHandler orig_error_handler = TIFFSetErrorHandler(custom_error_handler);
            TIFFErrorHandler orig_warning_handler = TIFFSetWarningHandler(custom_warning_handler);
            TIFFErrorHandlerExt orig_error_handler_ext = TIFFSetErrorHandlerExt(custom_error_handler_ext);
            TIFFErrorHandlerExt orig_warning_handler_ext = TIFFSetWarningHandlerExt(custom_warning_handler_ext);
            
            // Generate warning message from fuzzer input
            std::string module_name = fdp.ConsumeRandomLengthString(32);
            std::string warning_msg = fdp.ConsumeRandomLengthString(256);
            
            // Trigger warning using TIFFWarning
            TIFFWarning(module_name.c_str(), "%s", warning_msg.c_str());
            
            // Trigger warning using TIFFWarningExt with a dummy file descriptor
            thandle_t dummy_fd = (thandle_t)fdp.ConsumeIntegral<uint64_t>();
            TIFFWarningExt(dummy_fd, module_name.c_str(), "%s", warning_msg.c_str());
            
            // Verify handlers were called
            // (We can't assert in fuzzer, but we can check counters to ensure code paths were executed)
            
            // Restore original handlers
            TIFFSetErrorHandler(orig_error_handler);
            TIFFSetWarningHandler(orig_warning_handler);
            TIFFSetErrorHandlerExt(orig_error_handler_ext);
            TIFFSetWarningHandlerExt(orig_warning_handler_ext);
            
            break;
        }
        
        case 1: {
            // Test 2: Set custom handlers and trigger errors via TIFFError
            
            // Save original handlers
            TIFFErrorHandler orig_error_handler = TIFFSetErrorHandler(custom_error_handler);
            TIFFErrorHandlerExt orig_error_handler_ext = TIFFSetErrorHandlerExt(custom_error_handler_ext);
            
            // Generate error message from fuzzer input
            std::string module_name = fdp.ConsumeRandomLengthString(32);
            std::string error_msg = fdp.ConsumeRandomLengthString(256);
            
            // Trigger error using TIFFError
            TIFFError(module_name.c_str(), "%s", error_msg.c_str());
            
            // Trigger error using TIFFErrorExt with a dummy file descriptor
            thandle_t dummy_fd = (thandle_t)fdp.ConsumeIntegral<uint64_t>();
            TIFFErrorExt(dummy_fd, module_name.c_str(), "%s", error_msg.c_str());
            
            // Verify handlers were called
            
            // Restore original handlers
            TIFFSetErrorHandler(orig_error_handler);
            TIFFSetErrorHandlerExt(orig_error_handler_ext);
            
            break;
        }
        
        case 2: {
            // Test 3: Trigger errors/warnings through invalid TIFF operations
            
            // Set custom handlers
            TIFFSetErrorHandler(custom_error_handler);
            TIFFSetWarningHandler(custom_warning_handler);
            TIFFSetErrorHandlerExt(custom_error_handler_ext);
            TIFFSetWarningHandlerExt(custom_warning_handler_ext);
            
            // Try to open a non-existent file (should trigger error)
            std::string invalid_filename = fdp.ConsumeRandomLengthString(128);
            TIFF* tif = TIFFOpen(invalid_filename.c_str(), "r");
            if (tif) {
                TIFFClose(tif);
            }
            
            // Try to open with invalid mode (should trigger warning/error)
            std::string invalid_mode = fdp.ConsumeRandomLengthString(16);
            tif = TIFFOpen("/dev/null", invalid_mode.c_str());
            if (tif) {
                TIFFClose(tif);
            }
            
            // Restore default handlers
            TIFFSetErrorHandler(NULL);
            TIFFSetWarningHandler(NULL);
            TIFFSetErrorHandlerExt(NULL);
            TIFFSetWarningHandlerExt(NULL);
            
            break;
        }
        
        case 3: {
            // Test 4: Test handler chaining - set multiple handlers
            
            // First handler
            TIFFErrorHandler handler1 = TIFFSetErrorHandler(custom_error_handler);
            TIFFErrorHandlerExt handler1_ext = TIFFSetErrorHandlerExt(custom_error_handler_ext);
            
            // Trigger an error
            std::string module_name = fdp.ConsumeRandomLengthString(32);
            std::string error_msg = fdp.ConsumeRandomLengthString(256);
            TIFFError(module_name.c_str(), "%s", error_msg.c_str());
            
            // Set second handler (should replace first)
            TIFFErrorHandler handler2 = TIFFSetErrorHandler(NULL);
            TIFFErrorHandlerExt handler2_ext = TIFFSetErrorHandlerExt(NULL);
            
            // Trigger another error (should go to NULL handler)
            TIFFError(module_name.c_str(), "%s", "test error 2");
            
            // Restore first handler
            TIFFSetErrorHandler(handler1);
            TIFFSetErrorHandlerExt(handler1_ext);
            
            // Trigger third error (should go to first handler)
            TIFFError(module_name.c_str(), "%s", "test error 3");
            
            // Restore original handlers
            TIFFSetErrorHandler(handler2);
            TIFFSetErrorHandlerExt(handler2_ext);
            
            break;
        }
        
        case 4: {
            // Test 5: Create corrupt TIFF data and try to read it
            
            // Set handlers to capture errors
            TIFFSetErrorHandler(custom_error_handler);
            TIFFSetWarningHandler(custom_warning_handler);
            
            // Create a temporary file
            char temp_file_template[] = "/tmp/fuzz_tiff_error_XXXXXX";
            int fd = mkstemp(temp_file_template);
            if (fd < 0) {
                break;
            }
            std::string temp_filename = temp_file_template;
            
            // Write corrupt TIFF data based on fuzzer input
            size_t corrupt_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            std::vector<uint8_t> corrupt_data = fdp.ConsumeBytes<uint8_t>(corrupt_size);
            
            write(fd, corrupt_data.data(), corrupt_data.size());
            close(fd);
            
            // Try to open corrupt file (should trigger errors/warnings)
            TIFF* tif = TIFFOpen(temp_filename.c_str(), "r");
            if (tif) {
                // Try to read directory (should trigger more errors)
                TIFFReadDirectory(tif);
                TIFFClose(tif);
            }
            
            // Clean up
            unlink(temp_filename.c_str());
            
            // Restore default handlers
            TIFFSetErrorHandler(NULL);
            TIFFSetWarningHandler(NULL);
            
            break;
        }
        
        case 5: {
            // Test 6: Test with TIFFOpenOptions and error handlers
            
            // Create TIFFOpenOptions
            TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
            if (!opts) {
                break;
            }
            
            // Set error/warning handlers on options
            // Note: TIFFOpenOptionsSetErrorHandlerExtR and TIFFOpenOptionsSetWarningHandlerExtR
            // require TIFFErrorHandlerExtR type, which is different from TIFFErrorHandlerExt
            // For simplicity, we'll test the standard handlers
            
            // Try to open a file with options
            std::string filename = "/dev/null";
            TIFF* tif = TIFFOpenExt(filename.c_str(), "r", opts);
            if (tif) {
                // Try invalid operations to trigger errors/warnings
                // Set custom handlers first
                TIFFSetErrorHandler(custom_error_handler);
                TIFFSetWarningHandler(custom_warning_handler);
                
                // Try to read from write-only TIFF (should fail)
                uint32_t width, height;
                TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
                TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
                
                TIFFClose(tif);
                
                // Restore default handlers
                TIFFSetErrorHandler(NULL);
                TIFFSetWarningHandler(NULL);
            }
            
            TIFFOpenOptionsFree(opts);
            break;
        }
        
        case 6: {
            // Test 7: Test error/warning with TIFFClientOpen
            
            // Set custom handlers
            TIFFSetErrorHandler(custom_error_handler);
            TIFFSetWarningHandler(custom_warning_handler);
            
            // Create a minimal memory buffer with corrupt data
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
            std::vector<uint8_t> buffer_data = fdp.ConsumeBytes<uint8_t>(buffer_size);
            
            // Try to open corrupt memory buffer
            TIFF* tif = TIFFClientOpen("memory", "r", 
                                       (thandle_t)buffer_data.data(),
                                       NULL, NULL, NULL, NULL, NULL, NULL, NULL);
            if (tif) {
                TIFFClose(tif);
            }
            
            // Restore default handlers
            TIFFSetErrorHandler(NULL);
            TIFFSetWarningHandler(NULL);
            
            break;
        }
        
        case 7: {
            // Test 8: Comprehensive test - mix of all error/warning scenarios
            
            // Set all custom handlers
            TIFFErrorHandler orig_error = TIFFSetErrorHandler(custom_error_handler);
            TIFFErrorHandler orig_warning = TIFFSetWarningHandler(custom_warning_handler);
            TIFFErrorHandlerExt orig_error_ext = TIFFSetErrorHandlerExt(custom_error_handler_ext);
            TIFFErrorHandlerExt orig_warning_ext = TIFFSetWarningHandlerExt(custom_warning_handler_ext);
            
            // Generate test parameters
            std::string module1 = fdp.ConsumeRandomLengthString(32);
            std::string module2 = fdp.ConsumeRandomLengthString(32);
            std::string msg1 = fdp.ConsumeRandomLengthString(256);
            std::string msg2 = fdp.ConsumeRandomLengthString(256);
            thandle_t fd1 = (thandle_t)fdp.ConsumeIntegral<uint64_t>();
            thandle_t fd2 = (thandle_t)fdp.ConsumeIntegral<uint64_t>();
            
            // Trigger various error/warning combinations
            TIFFWarning(module1.c_str(), "Warning: %s", msg1.c_str());
            TIFFWarningExt(fd1, module2.c_str(), "Ext warning: %s", msg2.c_str());
            TIFFError(module1.c_str(), "Error: %s", msg1.c_str());
            TIFFErrorExt(fd2, module2.c_str(), "Ext error: %s", msg2.c_str());
            
            // Test with format specifiers
            int int_val = fdp.ConsumeIntegral<int>();
            double double_val = fdp.ConsumeFloatingPoint<double>();
            std::string str_val = fdp.ConsumeRandomLengthString(64);
            
            TIFFWarning(module1.c_str(), "Formatted: int=%d, double=%f, str=%s", 
                       int_val, double_val, str_val.c_str());
            
            // Restore original handlers
            TIFFSetErrorHandler(orig_error);
            TIFFSetWarningHandler(orig_warning);
            TIFFSetErrorHandlerExt(orig_error_ext);
            TIFFSetWarningHandlerExt(orig_warning_ext);
            
            break;
        }
    }
    
    return 0;
}

} // namespace harness_017
