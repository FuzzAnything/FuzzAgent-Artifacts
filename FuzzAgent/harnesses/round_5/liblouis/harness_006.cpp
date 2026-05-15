#include <fuzzer/FuzzedDataProvider.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <memory>
#include <vector>
#include <string>
#include <liblouis.h>

namespace harness_006 {
/*
 * Fuzzing harness for liblouis table resolver and management functions
 * Targets: lou_registerTableResolver (primary), lou_getDataPath, lou_version, lou_charSize (all 0% coverage)
 * Required helpers: lou_setDataPath, lou_findTable, lou_freeTableFiles
 * 
 * This harness focuses on table resolution system workflow:
 * 1. Configure data path for table lookup
 * 2. Define custom table resolver function that returns table file paths based on fuzzer input
 * 3. Register custom resolver with lou_registerTableResolver
 * 4. Trigger table lookup using lou_findTable to exercise the registered resolver
 * 5. Test getter functions: lou_getDataPath, lou_version, lou_charSize
 * 6. Clean up resources with lou_freeTableFiles
 * 
 * Semantic differentiation from previous harnesses:
 * - harness_000: Basic translation APIs (lou_translateString, lou_backTranslateString)
 * - harness_001: Metadata discovery APIs (lou_findTables, lou_listTables, lou_getTableInfo)
 * - harness_002: Hyphenation generation (lou_hyphenate)
 * - harness_003: Table compilation and validation (lou_compileString, lou_checkTable)
 * - harness_004: Logging functionality (lou_logFile, lou_logPrint)
 * - harness_005: Pre-hyphenated translation with emphasis classes
 * - harness_006: Table resolver system (current - focuses on table resolution and management)
 */



// Log callback to avoid excessive logging during fuzzing
void avoid_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Global initialization flag
static int initialized = 0;

// Custom table resolver implementation
// Returns table file paths based on fuzzer input
static char **EXPORT_CALL custom_table_resolver(const char *tableList, const char *base) {
    (void)base; // base parameter not used in this simple resolver
    
    // Handle NULL input
    if (!tableList) {
        return NULL;
    }
    
    // For safety, limit the number of tables we can return
    const int MAX_TABLES = 5;
    
    // Count commas to determine number of subtables
    int table_count = 1;
    const char *p = tableList;
    while (*p) {
        if (*p == ',') table_count++;
        p++;
    }
    
    if (table_count > MAX_TABLES) {
        table_count = MAX_TABLES;
    }
    
    // Allocate array for table paths
    char **table_files = (char **)calloc(table_count + 1, sizeof(char *));
    if (!table_files) {
        return NULL;
    }
    
    // Create simple table paths based on table names
    // In a real resolver, we would search the filesystem
    for (int i = 0; i < table_count; i++) {
        // Create a simple path: "tables/<tablename>.ctb"
        // Use a safe buffer and truncate if necessary
        char path[256];
        if (i == 0) {
            // Extract first table name (before first comma)
            const char *comma = strchr(tableList, ',');
            size_t table_len = comma ? (size_t)(comma - tableList) : strlen(tableList);
            if (table_len > 200) table_len = 200; // Leave room for prefix/suffix
            char safe_table[201];
            strncpy(safe_table, tableList, table_len);
            safe_table[table_len] = '\0';
            // Ensure path fits in buffer
            int written = snprintf(path, sizeof(path), "tables/%s.ctb", safe_table);
            if (written < 0 || (size_t)written >= sizeof(path)) {
                path[sizeof(path) - 1] = '\0'; // Ensure null termination
            }
        } else {
            snprintf(path, sizeof(path), "tables/table%d.ctb", i);
        }
        table_files[i] = strdup(path);
        if (!table_files[i]) {
            // Cleanup on allocation failure
            for (int j = 0; j < i; j++) {
                free(table_files[j]);
            }
            free(table_files);
            return NULL;
        }
    }
    
    table_files[table_count] = NULL;
    return table_files;
}

// Alternative custom resolver that returns NULL for certain inputs
// to test error handling in table resolution
static char **EXPORT_CALL null_table_resolver(const char *tableList, const char *base) {
    (void)base;
    
    // Return NULL for empty table list
    if (!tableList || strlen(tableList) == 0) {
        return NULL;
    }
    
    // Return a single dummy table path
    char **table_files = (char **)calloc(2, sizeof(char *));
    if (!table_files) {
        return NULL;
    }
    
    table_files[0] = strdup("tables/dummy.ctb");
    table_files[1] = NULL;
    
    return table_files;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(avoid_log);
        initialized = 1;
    }
    
    // Consume operation type to decide which resolver to test
    uint8_t resolver_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume data path string
    size_t max_path_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    std::string data_path = fdp.ConsumeRandomLengthString(max_path_len);
    
    // Consume table query string
    size_t max_query_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    std::string table_query = fdp.ConsumeRandomLengthString(max_query_len);
    
    if (data_path.empty() || table_query.empty()) {
        return 0;
    }
    
    // Step 1: Set data path
    char *old_data_path = lou_setDataPath(data_path.c_str());
    (void)old_data_path; // Old path may be NULL, ignore
    
    // Step 2: Register custom table resolver based on resolver_type
    switch (resolver_type) {
        case 0:
            // Register custom resolver
            lou_registerTableResolver(custom_table_resolver);
            break;
        case 1:
            // Register null resolver
            lou_registerTableResolver(null_table_resolver);
            break;
        case 2:
            // Register NULL resolver (should trigger error but test error handling)
            lou_registerTableResolver(NULL);
            break;
    }
    
    // Step 3: Trigger table lookup with lou_findTable
    // Note: lou_findTable will use the registered resolver
    char *found_table = NULL;
    if (resolver_type != 2) { // Skip if NULL resolver
        found_table = lou_findTable(table_query.c_str());
    }
    
    // Step 4: Test getter functions
    
    // Get data path (should return what we set)
    char *current_data_path = lou_getDataPath();
    if (current_data_path) {
        // Verify it matches our set path (or at least check it's not NULL)
        (void)current_data_path;
    }
    
    // Get library version
    const char *version = lou_version();
    if (version) {
        // Version string should be non-NULL
        (void)version;
    }
    
    // Get character size
    int char_size = lou_charSize();
    (void)char_size; // char_size should be positive
    
    // Step 5: Test lou_findTables if we have enough input
    if (fdp.remaining_bytes() > 16 && resolver_type != 2) {
        std::string tables_query = fdp.ConsumeRandomLengthString(
            fdp.ConsumeIntegralInRange<size_t>(1, 64));
        
        char **found_tables = lou_findTables(tables_query.c_str());
        if (found_tables) {
            // Free the returned table files
            lou_freeTableFiles(found_tables);
        }
    }
    
    // Step 6: Clean up
    
    // Free found table if we got one
    if (found_table) {
        // Use lou_freeTableFile for single string result
        // According to the header, lou_findTable returns a string that should be freed with lou_freeTableFile
        lou_freeTableFile(found_table);
    }
    // Reset to default resolver for clean state
    // The default resolver is _lou_defaultTableResolver
    // We can get it by calling lou_registerTableResolver with default function
    // but we don't have access to it. Instead, we can call lou_free() to reset
    // some state, but that might be too aggressive.
    // For fuzzing, it's okay to leave custom resolver registered.
    
    return 0;
}

} // namespace harness_006
