#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include "liblouis.h"

namespace harness_003 {
// liblouis table compilation and inspection fuzzing harness
// Target APIs: lou_compileString, lou_readCharFromFile, lou_getTypeformForEmphClass,
//              lou_getEmphClasses, lou_checkTable, lou_registerTableResolver
// Required helpers: lou_setDataPath, lou_getTable, lou_free
// Based on coverage analysis guidance targeting completely uncovered table compilation module
// Invocation sequence: setDataPath -> registerTableResolver -> compileString -> readCharFromFile ->
//                     getTypeformForEmphClass -> getEmphClasses -> checkTable -> getTable -> free


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Simple table resolver callback for lou_registerTableResolver
// Returns a NULL-terminated array of strings (table file paths)
extern "C" char **simple_table_resolver(const char *table, const char *base) {
    (void)base; // Base parameter not used in this simple implementation
    
    // Return a minimal response: single entry with table name, then NULL
    static char *result[2];
    static char table_copy[256];
    
    // Copy table name (limit size to avoid buffer overflow)
    size_t len = strlen(table);
    if (len > 255) len = 255;
    strncpy(table_copy, table, len);
    table_copy[len] = '\0';
    
    result[0] = table_copy;
    result[1] = NULL;
    
    return result;
}

// Simple ASCII to widechar conversion for testing
static size_t ascii_to_widechar(const char* ascii, widechar* wide, size_t max_len) {
    size_t i;
    for (i = 0; i < max_len && ascii[i] != '\0'; i++) {
        wide[i] = ascii[i];
    }
    return i;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for various parameters and table data
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    static bool initialized = false;
    if (!initialized) {
        lou_registerLogCallback(avoid_log_callback);
        initialized = true;
    }
    
    // 1. Call lou_setDataPath to configure table search path
    // Consume a path string from fuzzer input
    size_t path_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string data_path = fdp.ConsumeBytesAsString(path_len);
    char* path_result = lou_setDataPath(data_path.c_str());
    // Return value can be NULL if path is invalid, which is OK for fuzzing
    
    // 2. Call lou_registerTableResolver to register custom table resolver
    lou_registerTableResolver(simple_table_resolver);
    
    // 3. Select a translation table for testing
    // Use various table types to test different compilation scenarios
    const char* tables[] = {
        "en-us-g2.ctb",      // US English Grade 2 braille
        "en-ueb-g2.ctb",     // Unified English Braille Grade 2
        "en-us-comp8.ctb",   // US English computer braille
        "ar-ar-g2.ctb",      // Arabic Grade 2 braille
        "test.ctb",          // Generic test table name
        "example.utb",       // Example uncontracted table
        "invalid-table.ctb"  // Potentially invalid table name
    };
    const size_t num_tables = sizeof(tables) / sizeof(tables[0]);
    size_t table_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_tables - 1);
    const char* table = tables[table_idx];
    
    // 4. Call lou_compileString to compile a translation table from string data
    // Consume table string data from fuzzer input
    size_t table_data_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    std::string table_data = fdp.ConsumeBytesAsString(table_data_len);
    
    // Try to compile the table string
    int compile_result = lou_compileString(table, table_data.c_str());
    // Result can be 0 (failure) or 1 (success) - both are valid for fuzzing
    
    // 5. Call lou_readCharFromFile to test file-based table reading
    // Consume a filename from fuzzer input
    size_t filename_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    std::string filename = fdp.ConsumeBytesAsString(filename_len);
    
    int mode = 0;
    int char_result = lou_readCharFromFile(filename.c_str(), &mode);
    // Result is a character code or -1 on error - both valid for fuzzing
    
    // 6. Call lou_getTypeformForEmphClass to inspect table attributes
    // Consume emphasis class name from fuzzer input
    size_t emph_class_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
    std::string emph_class = fdp.ConsumeBytesAsString(emph_class_len);
    
    formtype typeform_result = lou_getTypeformForEmphClass(table, emph_class.c_str());
    // Returns typeform bit or 0 if class not found
    
    // 7. Call lou_getEmphClasses to get all emphasis classes from table
    char const **emph_classes = lou_getEmphClasses(table);
    if (emph_classes != NULL) {
        // Process the emphasis classes array
        for (int i = 0; emph_classes[i] != NULL; i++) {
            // Access each emphasis class name (read-only)
            const char* class_name = emph_classes[i];
            (void)class_name; // Use to avoid unused variable warning
        }
        
        // Free the emphasis classes array
        lou_freeEmphClasses(emph_classes);
    }
    
    // 8. Call lou_checkTable to verify table correctness
    int check_result = lou_checkTable(table);
    // Returns non-zero if table is valid, 0 if errors found
    
    // 9. Call lou_getTable to retrieve compiled table for validation
    const void* loaded_table = lou_getTable(table);
    // Returns pointer to loaded table or NULL on failure
    
    // 10. Clean up using lou_free
    // This frees all memory allocated by liblouis
    lou_free();
    
    return 0;
}

} // namespace harness_003
