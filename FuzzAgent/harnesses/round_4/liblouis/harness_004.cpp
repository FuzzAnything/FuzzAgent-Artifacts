#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include "liblouis.h"

namespace harness_004 {
// liblouis logging functionality fuzzing harness
// Target APIs: lou_logFile (completely uncovered, 16 undiscovered branches), 
//              lou_logPrint (completely uncovered, 6 undiscovered branches)
// Required helpers: lou_setLogLevel, lou_registerLogCallback, lou_logEnd
// Based on coverage analysis guidance targeting completely uncovered logging module
// Invocation sequence: setLogLevel -> optionally registerLogCallback -> logFile -> 
//                      logPrint multiple times -> logEnd -> verify log file


// Include liblouis public header
extern "C" {
}

// Custom log callback to capture log messages for verification
static std::string captured_log_message;
static logLevels captured_log_level;

static void custom_log_callback(logLevels level, const char *message) {
    captured_log_level = level;
    if (message) {
        captured_log_message = message;
    } else {
        captured_log_message = "";
    }
    // Also forward to default logging (lou_logPrint)
    lou_logPrint("%s", message);
}

// Generate a unique temporary filename
static std::string generate_temp_filename() {
    char template_path[] = "/tmp/liblouis_log_XXXXXX";
    int fd = mkstemp(template_path);
    if (fd != -1) {
        close(fd);
        return std::string(template_path);
    }
    return "/tmp/liblouis_log_fallback.log";
}

// Check if file exists and has content
static bool file_exists_and_nonempty(const std::string& filename) {
    struct stat st;
    if (stat(filename.c_str(), &st) != 0) {
        return false;
    }
    return st.st_size > 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for various parameters
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    static bool initialized = false;
    if (!initialized) {
        // Register a custom callback to capture logs for verification
        lou_registerLogCallback(custom_log_callback);
        initialized = true;
    }
    
    // 1. Call lou_setLogLevel to set an appropriate log level
    // Consume log level from fuzzer input
    uint8_t log_level_choice = fdp.ConsumeIntegral<uint8_t>() % 6;
    logLevels level;
    switch (log_level_choice) {
        case 0: level = LOU_LOG_ALL; break;
        case 1: level = LOU_LOG_DEBUG; break;
        case 2: level = LOU_LOG_INFO; break;
        case 3: level = LOU_LOG_WARN; break;
        case 4: level = LOU_LOG_ERROR; break;
        default: level = LOU_LOG_ALL; break;
    }
    lou_setLogLevel(level);
    
    // 2. Optionally call lou_registerLogCallback to register a custom callback
    // Use fuzzer input to decide whether to register custom callback or use default
    bool use_custom_callback = fdp.ConsumeBool();
    if (use_custom_callback) {
        lou_registerLogCallback(custom_log_callback);
    } else {
        lou_registerLogCallback(NULL); // Reset to default callback
    }
    
    // 3. Call lou_logFile with a temporary filename to open a log file
    std::string temp_filename = generate_temp_filename();
    lou_logFile(temp_filename.c_str());
    
    // 4. Call lou_logPrint multiple times with various format strings and arguments
    // Test different format specifiers and log levels
    
    // First, test simple string format
    size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string simple_msg = fdp.ConsumeBytesAsString(str_len);
    if (!simple_msg.empty()) {
        lou_logPrint("Simple message: %s", simple_msg.c_str());
    }
    
    // Test integer format with various integers
    int int_val = fdp.ConsumeIntegral<int>();
    lou_logPrint("Integer value: %d", int_val);
    
    // Test floating point if enough data
    if (fdp.remaining_bytes() >= sizeof(float)) {
        float float_val = fdp.ConsumeFloatingPoint<float>();
        lou_logPrint("Float value: %f", float_val);
    }
    
    // Test hexadecimal format
    unsigned int hex_val = fdp.ConsumeIntegral<unsigned int>();
    lou_logPrint("Hex value: 0x%08X", hex_val);
    
    // Test combined format specifiers
    size_t combined_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    std::string combined_str = fdp.ConsumeBytesAsString(combined_len);
    int combined_int = fdp.ConsumeIntegral<int>();
    lou_logPrint("Combined: str='%s', int=%d", combined_str.c_str(), combined_int);
    
    // Test with different log levels via _lou_logMessage (internal function)
    // We can't directly call _lou_logMessage as it's internal, but lou_logPrint
    // will write to the log file regardless of log level
    
    // Test edge cases: NULL format string (should be handled gracefully)
    // lou_logPrint(NULL); // This would crash, so we don't call it
    
    // Test empty string
    lou_logPrint("");
    
    // Test format string with only format specifiers
    int spec_int = fdp.ConsumeIntegral<int>();
    lou_logPrint("%d", spec_int);
    
    // Test multiple format specifiers
    int val1 = fdp.ConsumeIntegral<int>();
    int val2 = fdp.ConsumeIntegral<int>();
    lou_logPrint("Values: %d, %d", val1, val2);
    
    // 5. Call lou_logEnd to clean up and close the log file
    lou_logEnd();
    
    // 6. Verify the log file was created and contains the expected output
    // Note: Since we're fuzzing, we don't want to fail if file doesn't exist
    // due to permissions or other issues. Just check if we can read it.
    if (file_exists_and_nonempty(temp_filename)) {
        // Try to read the file to ensure it's accessible
        FILE* f = fopen(temp_filename.c_str(), "r");
        if (f) {
            // Read first few bytes to verify content
            char buf[128];
            size_t bytes_read = fread(buf, 1, sizeof(buf) - 1, f);
            buf[bytes_read] = '\0';
            fclose(f);
            
            // Clean up the temporary file
            unlink(temp_filename.c_str());
        } else {
            // File exists but can't be opened - might be locked, but that's OK for fuzzing
            unlink(temp_filename.c_str());
        }
    } else {
        // File doesn't exist or is empty - clean up if it exists
        unlink(temp_filename.c_str());
    }
    
    // Test reopening log file with different filename
    std::string second_temp_filename = generate_temp_filename();
    lou_logFile(second_temp_filename.c_str());
    
    // Log a final message to the new file
    size_t final_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
    std::string final_msg = fdp.ConsumeBytesAsString(final_len);
    if (!final_msg.empty()) {
        lou_logPrint("Final: %s", final_msg.c_str());
    }
    
    // Close and clean up
    lou_logEnd();
    unlink(second_temp_filename.c_str());
    
    // Test setting different log levels
    logLevels levels_to_test[] = {LOU_LOG_ALL, LOU_LOG_DEBUG, LOU_LOG_INFO, 
                                   LOU_LOG_WARN, LOU_LOG_ERROR};
    for (size_t i = 0; i < sizeof(levels_to_test)/sizeof(levels_to_test[0]) && 
         fdp.remaining_bytes() > 0; i++) {
        lou_setLogLevel(levels_to_test[i]);
        // Consume a small string for logging
        if (fdp.remaining_bytes() > 16) {
            size_t test_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
            std::string test_msg = fdp.ConsumeBytesAsString(test_len);
            lou_logPrint("Level test %zu: %s", i, test_msg.c_str());
        }
    }
    
    return 0;
}

} // namespace harness_004
