#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <liblouis/liblouis.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <vector>

namespace harness_005 {
// liblouis fuzzing harness - Logging subsystem focus
// Targets: lou_logFile, lou_readCharFromFile, lou_logPrint, lou_setLogLevel, lou_registerLogCallback, lou_logEnd
// Focus on logging configuration, file I/O during table compilation, and log message generation
// Based on coverage analysis identifying 3 completely uncovered APIs with 22+ undiscovered branches


// Global initialization flag
static int initialized = 0;
static int file_counter = 0;

// Custom log callback for fuzzing - can be used to test logging system
void fuzzer_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
    // Log callback can be tested with various inputs
}

// Another log callback for testing callback switching
void alternate_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Cleanup function
static void free_resources(void) {
    lou_free();
}

// Create a temporary file with fuzzer data for logging tests
static char* create_temp_file(const uint8_t* data, size_t size, size_t* consumed_size, const char* suffix) {
    char filename[256];
    snprintf(filename, sizeof(filename), "/tmp/liblouis-log-fuzz-%d-%d%s", getpid(), file_counter++, suffix);
    
    // Determine how much data to write to the file (min 1 byte, max 1024 bytes)
    size_t file_size = (size > 0) ? (size % 1024) + 1 : 1;
    if (file_size > size) file_size = size;
    
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return NULL;
    }
    
    fwrite(data, 1, file_size, fp);
    fclose(fp);
    
    *consumed_size = file_size;
    
    // Duplicate filename for caller to free
    return strdup(filename);
}

// Create a temporary table file for compilation logging tests
static char* create_temp_table_file(const uint8_t* data, size_t size, size_t* consumed_size) {
    return create_temp_file(data, size, consumed_size, ".ctb");
}

// Create a temporary log file for logging output tests
static char* create_temp_log_file(const uint8_t* data, size_t size, size_t* consumed_size) {
    return create_temp_file(data, size, consumed_size, ".log");
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for meaningful testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    if (!initialized) {
        // Start with default logging
        initialized = 1;
        // Register cleanup
        atexit(free_resources);
    }
    
    // Consume operation type to decide which logging API to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume log level from input
    uint8_t log_level_val = fdp.ConsumeIntegral<uint8_t>() % 7;
    logLevels log_level;
    switch (log_level_val) {
        case 0: log_level = LOU_LOG_ALL; break;
        case 1: log_level = LOU_LOG_DEBUG; break;
        case 2: log_level = LOU_LOG_INFO; break;
        case 3: log_level = LOU_LOG_WARN; break;
        case 4: log_level = LOU_LOG_ERROR; break;
        case 5: log_level = LOU_LOG_FATAL; break;
        case 6: log_level = LOU_LOG_OFF; break;
        default: log_level = LOU_LOG_INFO; break;
    }
    
    // Consume filename for logging operations
    std::string filename = fdp.ConsumeRandomLengthString(128);
    
    // Consume log message content
    std::string log_message = fdp.ConsumeRandomLengthString(512);
    
    // For file-based operations, create temporary files
    char* temp_table_file = NULL;
    char* temp_log_file = NULL;
    size_t table_data_consumed = 0;
    size_t log_data_consumed = 0;
    
    // Create temporary table file for compilation logging tests
    if (operation == 0 || operation == 1 || operation == 4) {
        std::vector<uint8_t> table_data = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(1, 1024));
        if (!table_data.empty()) {
            temp_table_file = create_temp_table_file(table_data.data(), table_data.size(), &table_data_consumed);
        }
    }
    
    // Create temporary log file for log output tests
    if (operation == 2 || operation == 3 || operation == 5) {
        std::vector<uint8_t> log_file_data = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(1, 1024));
        if (!log_file_data.empty()) {
            temp_log_file = create_temp_log_file(log_file_data.data(), log_file_data.size(), &log_data_consumed);
        }
    }
    
    // Test different logging operations
    switch (operation) {
        case 0: {
            // Test lou_logFile with fuzzer-generated filename
            if (!filename.empty()) {
                lou_logFile(filename.c_str());
            }
            // Also test with temporary log file
            if (temp_log_file) {
                lou_logFile(temp_log_file);
            }
            break;
        }
        
        case 1: {
            // Test lou_readCharFromFile - requires mode variable
            int mode = 1; // Start with mode = 1 for first call
            if (temp_table_file) {
                // Read characters from the temporary table file
                while (mode != 1) {
                    int ch = lou_readCharFromFile(temp_table_file, &mode);
                    if (ch == EOF) break;
                    // Process character if needed
                }
            }
            // Also test with provided filename
            if (!filename.empty()) {
                int mode2 = 1;
                int ch = lou_readCharFromFile(filename.c_str(), &mode2);
                (void)ch;
            }
            break;
        }
        
        case 2: {
            // Test lou_logPrint with various log messages
            if (!log_message.empty()) {
                lou_logPrint("%s", log_message.c_str());
            }
            // Test with different format specifiers
            int int_val = fdp.ConsumeIntegral<int>();
            double double_val = fdp.ConsumeFloatingPoint<double>();
            lou_logPrint("Int: %d, Double: %f, String: %s", int_val, double_val, log_message.c_str());
            break;
        }
        
        case 3: {
            // Test lou_setLogLevel with fuzzer-provided level
            lou_setLogLevel(log_level);
            
            // Test logging with different levels
            lou_registerLogCallback(fuzzer_log_callback);
            
            // Generate some log messages through table compilation
            if (temp_table_file) {
                // Check table to trigger logging
                lou_checkTable(temp_table_file);
            }
            break;
        }
        
        case 4: {
            // Test lou_registerLogCallback with different callbacks
            uint8_t callback_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (callback_choice) {
                case 0:
                    lou_registerLogCallback(fuzzer_log_callback);
                    break;
                case 1:
                    lou_registerLogCallback(alternate_log_callback);
                    break;
                case 2:
                    lou_registerLogCallback(NULL); // Reset to default
                    break;
            }
            
            // Set log level and trigger some logging
            lou_setLogLevel(log_level);
            
            // Try to compile a string to trigger logging
            std::string compile_string = fdp.ConsumeRandomLengthString(256);
            if (!compile_string.empty() && temp_table_file) {
                lou_compileString(temp_table_file, compile_string.c_str());
            }
            break;
        }
        
        case 5: {
            // Test comprehensive logging configuration
            // Set log file
            if (temp_log_file) {
                lou_logFile(temp_log_file);
            }
            
            // Set log level
            lou_setLogLevel(log_level);
            
            // Register callback
            lou_registerLogCallback(fuzzer_log_callback);
            
            // Generate log messages through various operations
            if (!log_message.empty()) {
                lou_logPrint("Test message: %s", log_message.c_str());
            }
            
            // Test table compilation with logging
            if (temp_table_file) {
                lou_checkTable(temp_table_file);
                const void* table = lou_getTable(temp_table_file);
                (void)table;
            }
            
            // Test lou_logEnd
            lou_logEnd();
            break;
        }
        
        case 6: {
            // Test error path logging
            // Set log level to capture errors
            lou_setLogLevel(LOU_LOG_ERROR);
            
            // Try to trigger logging with invalid inputs
            lou_logFile(NULL); // NULL filename
            lou_logFile("");   // Empty filename
            lou_logFile("/nonexistent/path/to/file.log"); // Invalid path
            
            // Test lou_readCharFromFile with invalid inputs
            int mode = 1;
            lou_readCharFromFile(NULL, &mode);
            lou_readCharFromFile("/nonexistent/file", &mode);
            
            // Test lou_logPrint with NULL format
            lou_logPrint(NULL);
            
            break;
        }
        
        case 7: {
            // Test logging during table compilation workflow
            // Configure logging first
            if (temp_log_file) {
                lou_logFile(temp_log_file);
            }
            lou_setLogLevel(log_level);
            lou_registerLogCallback(fuzzer_log_callback);
            
            // Perform table operations that generate logs
            if (temp_table_file) {
                // Check table (generates warnings/errors)
                int check_result = lou_checkTable(temp_table_file);
                (void)check_result;
                
                // Get table (may generate logs)
                const void* table = lou_getTable(temp_table_file);
                (void)table;
                
                // Try to compile a string entry
                std::string table_entry = fdp.ConsumeRemainingBytesAsString();
                if (!table_entry.empty()) {
                    lou_compileString(temp_table_file, table_entry.c_str());
                }
            }
            
            // Generate direct log messages
            std::string final_message = fdp.ConsumeRemainingBytesAsString();
            if (!final_message.empty()) {
                lou_logPrint("Final: %s", final_message.c_str());
            }
            
            // Clean up logging
            lou_logEnd();
            break;
        }
    }
    
    // Clean up temporary files
    if (temp_table_file) {
        unlink(temp_table_file);
        free(temp_table_file);
    }
    
    if (temp_log_file) {
        unlink(temp_log_file);
        free(temp_log_file);
    }
    
    return 0;
}

} // namespace harness_005
