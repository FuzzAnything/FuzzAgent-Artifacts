#include <fuzzer/FuzzedDataProvider.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <memory>
#include <vector>
#include <string>
#include <liblouis.h>

namespace harness_004 {
/*
 * Fuzzing harness for liblouis logging functionality
 * Targets: lou_logFile, lou_logPrint, lou_logEnd
 * Required helpers: lou_setLogLevel, lou_registerLogCallback, lou_translate/lou_backTranslate
 * 
 * This harness focuses on logging lifecycle:
 * 1. Configure log level and callback
 * 2. Open log file with various parameters
 * 3. Generate log messages through translation operations
 * 4. Direct print operations with format strings
 * 5. Proper cleanup of logging resources
 * 
 * Differentiated from previous harnesses:
 * - harness_000: Translation APIs (lou_translateString, lou_backTranslateString)
 * - harness_001: Table metadata discovery (lou_findTables, lou_getTableInfo)
 * - harness_002: Hyphenation functionality (lou_hyphenate, lou_translatePrehyphenated)
 * - harness_003: Table compilation and validation (lou_compileString, lou_checkTable)
 * - harness_004: Logging functionality (current)
 */


void custom_log_callback(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
    // Just consume the message - we could optionally write to stderr
    // but avoid excessive output during fuzzing
}
class TempLogFile {
public:
    TempLogFile() {
        char template_path[] = "/tmp/liblouis_log_XXXXXX";
        fd = mkstemp(template_path);
        if (fd != -1) {
            path = template_path;
        }
    }
    
    ~TempLogFile() {
        if (fd != -1) {
            close(fd);
            unlink(path.c_str());
        }
    }
    
    const char* get_path() const {
        return path.c_str();
    }
    
    bool is_valid() const {
        return fd != -1;
    }
    
private:
    int fd = -1;
    std::string path;
};

// Helper function to convert string to widechar array
static widechar* string_to_widechar(const std::string& str, size_t* out_len) {
    if (str.empty()) {
        *out_len = 0;
        return nullptr;
    }
    
    size_t len = str.length();
    widechar* wstr = (widechar*)malloc((len + 1) * sizeof(widechar));
    if (!wstr) {
        *out_len = 0;
        return nullptr;
    }
    
    // Simple conversion: assuming ASCII/UTF-8 single-byte chars
    for (size_t i = 0; i < len; i++) {
        wstr[i] = (widechar)(unsigned char)str[i];
    }
    wstr[len] = 0;
    *out_len = len;
    return wstr;
}

// Helper to check if table name looks valid
static bool is_likely_valid_table(const std::string& table_name) {
    if (table_name.empty() || table_name.length() > 256) {
        return false;
    }
    
    // Check for common table extensions
    return table_name.find(".ctb") != std::string::npos || 
           table_name.find(".utb") != std::string::npos;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - we need enough for basic logging operations
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Register log callback for this iteration
    lou_registerLogCallback(custom_log_callback);

    // Consume operation type to decide which logging flow to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume log level with proper validation
    uint8_t log_level_raw = fdp.ConsumeIntegral<uint8_t>() % 7;  // 7 log levels
    logLevels log_level = LOU_LOG_INFO;  // Default
    switch (log_level_raw) {
        case 0: log_level = LOU_LOG_ALL; break;
        case 1: log_level = LOU_LOG_DEBUG; break;
        case 2: log_level = LOU_LOG_INFO; break;
        case 3: log_level = LOU_LOG_WARN; break;
        case 4: log_level = LOU_LOG_ERROR; break;
        case 5: log_level = LOU_LOG_FATAL; break;
        case 6: log_level = LOU_LOG_OFF; break;
        default: log_level = LOU_LOG_INFO; break;
    }
    
    // Set log level
    lou_setLogLevel(log_level);
    
    // Consume filename for logging - validate length and clean null bytes
    std::string filename = fdp.ConsumeRandomLengthString(128);
    size_t null_pos = filename.find('\0');
    if (null_pos != std::string::npos) {
        filename.resize(null_pos);
    }
    
    // Consume table name for translation operations
    std::string table_name = fdp.ConsumeRandomLengthString(128);
    null_pos = table_name.find('\0');
    if (null_pos != std::string::npos) {
        table_name.resize(null_pos);
    }
    
    // If table doesn't look valid, use a known valid one
    if (!is_likely_valid_table(table_name)) {
        const char* default_tables[] = {
            "en-us-g2.ctb",
            "en-us-g1.ctb",
            "de-de-g2.ctb",
            "fr-bfu-g2.ctb"
        };
        if (fdp.remaining_bytes() >= 1) {
            uint8_t table_idx = fdp.ConsumeIntegral<uint8_t>() % 4;
            table_name = default_tables[table_idx];
        } else {
            table_name = "en-us-g2.ctb";  // Default fallback
        }
    }
    
    // Consume input text for translation (to generate log messages)
    size_t max_input_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
    std::string input_text = fdp.ConsumeRandomLengthString(max_input_len);
    null_pos = input_text.find('\0');
    if (null_pos != std::string::npos) {
        input_text.resize(null_pos);
    }
    
    // Consume format string and arguments for lou_logPrint
    std::string format_str = fdp.ConsumeRandomLengthString(64);
    null_pos = format_str.find('\0');
    if (null_pos != std::string::npos) {
        format_str.resize(null_pos);
    }
    
    std::string arg1 = fdp.ConsumeRandomLengthString(32);
    null_pos = arg1.find('\0');
    if (null_pos != std::string::npos) {
        arg1.resize(null_pos);
    }
    
    int arg2 = fdp.ConsumeIntegral<int>();
    double arg3 = fdp.ConsumeFloatingPoint<double>();
    
    // Execute the selected logging operation with defensive programming
    switch (operation) {
        case 0: {
            // Test 1: Basic logging lifecycle with file
            // Test with various filename scenarios
            if (filename.empty()) {
                // Test with NULL filename (should use default)
                lou_logFile(NULL);
            } else if (filename.length() < 5) {
                // Test with very short filename (edge case)
                lou_logFile(filename.c_str());
            } else {
                // Create temporary file for logging
                TempLogFile temp_file;
                if (temp_file.is_valid()) {
                    lou_logFile(temp_file.get_path());
                } else {
                    // Fall back to provided filename
                    lou_logFile(filename.c_str());
                }
            }
            
            // Generate some log messages through translation
            if (!input_text.empty() && input_text.length() <= 100) {
                size_t widechar_len = 0;
                widechar* inbuf = string_to_widechar(input_text, &widechar_len);
                if (inbuf && widechar_len > 0 && widechar_len <= 100) {
                    // Use unique_ptr with custom deleter for automatic cleanup
                    std::unique_ptr<widechar, decltype(&free)> inbuf_guard(inbuf, &free);
                    
                    int inlen = (int)widechar_len;
                    int outlen = inlen * 2; // Conservative estimate
                    widechar* outbuf = (widechar*)malloc(outlen * sizeof(widechar));
                    if (outbuf) {
                        std::unique_ptr<widechar, decltype(&free)> outbuf_guard(outbuf, &free);
                        
                        formtype* typeform = (formtype*)malloc(inlen * sizeof(formtype));
                        char* spacing = (char*)malloc(inlen * sizeof(char));
                        
                        if (typeform && spacing) {
                            std::unique_ptr<formtype, decltype(&free)> typeform_guard(typeform, &free);
                            std::unique_ptr<char, decltype(&free)> spacing_guard(spacing, &free);
                            
                            // This may generate internal log messages via _lou_logMessage
                            lou_translateString(table_name.c_str(), inbuf, &inlen, 
                                              outbuf, &outlen, typeform, spacing, 0);
                        } else {
                            if (typeform) free(typeform);
                            if (spacing) free(spacing);
                        }
                    }
                }
            }
            
            // Direct logging with lou_logPrint - use safe format strings
            if (!format_str.empty() && format_str.length() <= 64) {
                // Use simple format strings to avoid format string vulnerabilities
                lou_logPrint("Fuzzer message: %s", arg1.c_str());
                lou_logPrint("Values: %d, %.2f", arg2, arg3);
                // Only use format_str with %s to avoid injection
                lou_logPrint("%s", format_str.c_str());
            }
            
            lou_logEnd();
            break;
        }
        
        case 1: {
            // Test 2: Log level variations without file
            // Don't call lou_logFile - should default to stderr
            lou_logPrint("Testing without explicit log file, level: %d", (int)log_level);
            
            // Generate log messages through back translation
            if (!input_text.empty() && input_text.length() <= 100) {
                size_t widechar_len = 0;
                widechar* inbuf = string_to_widechar(input_text, &widechar_len);
                if (inbuf && widechar_len > 0 && widechar_len <= 100) {
                    std::unique_ptr<widechar, decltype(&free)> inbuf_guard(inbuf, &free);
                    
                    int inlen = (int)widechar_len;
                    int outlen = inlen * 2;
                    widechar* outbuf = (widechar*)malloc(outlen * sizeof(widechar));
                    if (outbuf) {
                        std::unique_ptr<widechar, decltype(&free)> outbuf_guard(outbuf, &free);
                        
                        formtype* typeform = (formtype*)malloc(inlen * sizeof(formtype));
                        char* spacing = (char*)malloc(inlen * sizeof(char));
                        
                        if (typeform && spacing) {
                            std::unique_ptr<formtype, decltype(&free)> typeform_guard(typeform, &free);
                            std::unique_ptr<char, decltype(&free)> spacing_guard(spacing, &free);
                            
                            // May generate log messages
                            lou_backTranslateString(table_name.c_str(), inbuf, &inlen,
                                                  outbuf, &outlen, typeform, spacing, 0);
                        } else {
                            if (typeform) free(typeform);
                            if (spacing) free(spacing);
                        }
                    }
                }
            }
            
            lou_logEnd(); // Still call to ensure cleanup
            break;
        }
        
        case 2: {
            // Test 3: Multiple file changes
            lou_setLogLevel(LOU_LOG_ALL); // Log everything for this test
            
            // Open first file
            TempLogFile temp_file1;
            if (temp_file1.is_valid()) {
                lou_logFile(temp_file1.get_path());
                lou_logPrint("Logging to first temp file");
            }
            
            // Switch to second file
            TempLogFile temp_file2;
            if (temp_file2.is_valid()) {
                lou_logFile(temp_file2.get_path());
                lou_logPrint("Switched to second temp file");
            }
            
            // Switch back to stderr
            lou_logFile(NULL);
            lou_logPrint("Back to stderr");
            
            // Generate translation log messages
            if (!input_text.empty() && input_text.length() > 10 && input_text.length() <= 100) {
                // Take first 10 chars for translation
                std::string short_text = input_text.substr(0, 10);
                size_t widechar_len = 0;
                widechar* inbuf = string_to_widechar(short_text, &widechar_len);
                if (inbuf && widechar_len > 0 && widechar_len <= 10) {
                    std::unique_ptr<widechar, decltype(&free)> inbuf_guard(inbuf, &free);
                    
                    int inlen = (int)widechar_len;
                    int outlen = inlen * 2;
                    widechar* outbuf = (widechar*)malloc(outlen * sizeof(widechar));
                    if (outbuf) {
                        std::unique_ptr<widechar, decltype(&free)> outbuf_guard(outbuf, &free);
                        
                        // Simple translation to trigger logging
                        lou_translateString(table_name.c_str(), inbuf, &inlen,
                                          outbuf, &outlen, NULL, NULL, 0);
                    }
                }
            }
            
            lou_logEnd();
            break;
        }
        
        case 3: {
            // Test 4: Edge cases and error conditions
            lou_setLogLevel(LOU_LOG_ERROR); // Only log errors
            
            // Test with empty filename string - should be safe
            lou_logFile("");
            
            // Test with very long filename (should be rejected)
            // FILENAMESIZE is 256 in logging.c
            std::string long_filename(300, 'x');
            lou_logFile(long_filename.c_str());
            
            // Test with invalid path
            lou_logFile("/nonexistent/path/to/file.log");
            
            // Still try to log - should go to stderr
            lou_logPrint("Testing error conditions");
            
            // Try translation with potentially problematic input
            if (!input_text.empty() && input_text.length() <= 100) {
                size_t widechar_len = 0;
                widechar* inbuf = string_to_widechar(input_text, &widechar_len);
                if (inbuf && widechar_len > 0 && widechar_len <= 100) {
                    std::unique_ptr<widechar, decltype(&free)> inbuf_guard(inbuf, &free);
                    
                    int inlen = (int)widechar_len;
                    // Use NULL for output buffers to test error paths
                    lou_translateString("invalid_table_name.ctb", inbuf, &inlen,
                                      NULL, NULL, NULL, NULL, 0);
                }
            }
            
            lou_logEnd();
            break;
        }
    }
    
    // Don't call lou_free() here - it's not needed for logging functions
    // and might interfere with other fuzzing threads or cause crashes
    
    return 0;
}

} // namespace harness_004
