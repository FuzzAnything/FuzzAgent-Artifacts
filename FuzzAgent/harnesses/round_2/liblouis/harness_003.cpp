#include <config.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "liblouis.h"

namespace harness_003 {
// Targeted liblouis fuzzing harness for compilation and logging subsystem operations
// Harness_003: Specifically targets compilation and logging APIs with 40+ undiscovered branches
// Targets: lou_compileString (4 branches), lou_readCharFromFile (12), lou_getEmphClasses (8), 
//          lou_getTypeformForEmphClass (8), lou_logFile (16), lou_logPrint (6), lou_registerTableResolver (2)
// Invocation sequence: logging setup → table compilation → emphasis class query → logging output → cleanup
// Differentiated from previous harnesses by focusing on compilation/logging lifecycle




// Log callback to capture or suppress logging during fuzzing
static void fuzzer_log_callback(logLevels level, const char* message) {
    // For fuzzing, we can either ignore logs or do minimal processing
    // This callback is registered to avoid default console output
    (void)level;
    (void)message;
    // Optionally, we could track log messages for error detection
}

// Simple table resolver for testing lou_registerTableResolver
static char** test_table_resolver(const char* table, const char* base) {
    // Minimal resolver implementation for fuzzing
    // Return NULL to indicate resolver couldn't find table
    (void)table;
    (void)base;
    return NULL;
}

// Destructor to free liblouis resources
static void __attribute__((destructor)) free_resources(void) {
    lou_free();
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful testing
    // We need: log level (enum) + operation type + string data for various functions
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Operation type from fuzzer input
    // 0: Full compilation/logging lifecycle
    // 1: Compilation-specific operations  
    // 2: Logging-specific operations
    // 3: Emphasis class operations
    // 4: File reading operations
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Initialize liblouis logging
    static int logging_initialized = 0;
    if (!logging_initialized) {
        // Setup logging system
        logLevels log_level = static_cast<logLevels>(fdp.ConsumeIntegralInRange<int>(0, 60000));
        lou_setLogLevel(log_level);
        lou_registerLogCallback(fuzzer_log_callback);
        logging_initialized = 1;
    }
    
    // Consume log file path for lou_logFile
    bool use_log_file = fdp.ConsumeBool();
    std::string log_file_path;
    if (use_log_file && fdp.remaining_bytes() > 10) {
        log_file_path = fdp.ConsumeRandomLengthString(128);
        lou_logFile(log_file_path.c_str());
    }
    
    // Common braille tables for testing
    const char* test_tables[] = {
        "en-us-g2.ctb",      // US English Grade 2
        "en-ueb-g2.ctb",     // Unified English Braille Grade 2
        "en-us-comp6.ctb",   // US computer braille 6-dot
        "en-us-comp8.ctb",   // US computer braille 8-dot
        "de-g0.utb",         // German Grade 0
        "sv-g1.ctb",         // Swedish Grade 1
        NULL
    };
    
    // Select a table for operations
    int table_index = fdp.ConsumeIntegralInRange<int>(0, 5);
    const char* table_name = test_tables[table_index];
    
    // Variable to track if we loaded a valid table
    const void* loaded_table = NULL;
    int table_valid = 0;
    
    // Step 4: lou_getTable/lou_checkTable - load or check translation table
    if (operation != 2) { // Skip for logging-only operations
        // Try lou_checkTable first
        table_valid = lou_checkTable(table_name);
        
        if (!table_valid) {
            // If check fails, try lou_getTable
            loaded_table = lou_getTable(table_name);
            if (loaded_table != NULL) {
                table_valid = 1;
            }
        }
    }
    
    // Step 5: lou_compileString - compile table entry on the fly
    std::string compile_string;
    int compile_success = 0;
    
    if ((operation == 0 || operation == 1) && table_valid && fdp.remaining_bytes() > 20) {
        // Consume string to compile
        compile_string = fdp.ConsumeRandomLengthString(512);
        
        // lou_compileString returns int (1 on success, 0 on failure)
        compile_success = lou_compileString(table_name, compile_string.c_str());
    }
    
    // Step 6: lou_readCharFromFile - read character from file
    int file_read_result = -1;
    int read_mode = 1; // First call mode
    
    if ((operation == 0 || operation == 4) && fdp.remaining_bytes() > 10) {
        // Create a temporary test file
        std::string temp_file_content = fdp.ConsumeRandomLengthString(256);
        std::string temp_file_path = "/tmp/liblouis_fuzz_test_XXXXXX";
        
        // Create temporary file
        int fd = mkstemp(&temp_file_path[0]);
        if (fd >= 0) {
            write(fd, temp_file_content.c_str(), temp_file_content.size());
            close(fd);
            
            // Call lou_readCharFromFile
            file_read_result = lou_readCharFromFile(temp_file_path.c_str(), &read_mode);
            
            // Clean up temporary file
            unlink(temp_file_path.c_str());
        }
    }
    
    // Step 7: lou_getEmphClasses - retrieve emphasis classes
    char const** emph_classes = NULL;
    
    if ((operation == 0 || operation == 3) && table_valid) {
        emph_classes = lou_getEmphClasses(table_name);
    }
    
    // Step 8: lou_getTypeformForEmphClass - get typeform for emphasis class
    formtype typeform_result = plain_text;
    
    if ((operation == 0 || operation == 3) && emph_classes != NULL) {
        // Get typeform for first emphasis class if available
        if (emph_classes[0] != NULL) {
            typeform_result = lou_getTypeformForEmphClass(table_name, emph_classes[0]);
        }
    }
    // Step 9: lou_logPrint - test logging output
    if (operation == 0 || operation == 2) {
        // Consume format string and arguments
        std::string log_format = fdp.ConsumeRandomLengthString(128);
        std::string log_arg1 = fdp.ConsumeRandomLengthString(64);
        
        // Simple logging - avoid complex format strings that might crash
        if (!log_format.empty() && log_format.find('%') == std::string::npos) {
            lou_logPrint("%s: %s", log_format.c_str(), log_arg1.c_str());
        }
    }
    
    // Step 10: lou_registerTableResolver - register table resolver
    if (operation == 0 || operation == 1) {
        lou_registerTableResolver(test_table_resolver);
    }
    
    // Step 11: Cleanup
    // Free emphasis classes if allocated
    if (emph_classes != NULL) {
        // According to documentation in liblouis.h.in, lou_getEmphClasses returns
        // memory that needs to be freed with lou_free()
        // lou_free() will handle all allocated resources
    }
    
    // Log end to ensure clean state
    if (use_log_file) {
        lou_logEnd();
    }
    
    // For operations that loaded a table, ensure cleanup
    if (loaded_table != NULL || table_valid) {
        // lou_free() is called by destructor, but we can also call it here
        // to ensure clean state between fuzzing iterations
        // Note: lou_free() frees all tables, which might affect other harnesses
        // running in same process. We rely on the destructor for final cleanup.
    }
    
    return 0;
}

} // namespace harness_003
