#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_013 {
/*
 * Fuzzing harness for SQLite library - targeting SQLite extension loading APIs
 * as identified by coverage analysis.
 * 
 * This harness targets SQLite extension loading APIs with 0% coverage despite high complexity:
 * - sqlite3_auto_extension (549 undiscovered branches) - register statically linked extensions
 * - sqlite3_load_extension (542 undiscovered branches) - load dynamic extension from shared library
 * - sqlite3_cancel_auto_extension (8 undiscovered branches) - remove auto-extension registration
 * - sqlite3_enable_load_extension (6 undiscovered branches) - enable/disable extension loading
 * - sqlite3_create_filename (545 undiscovered branches) - create database filename with options
 * - sqlite3_free_filename (cleanup) - free filename created by sqlite3_create_filename
 * 
 * Plus required helper APIs: sqlite3_initialize, sqlite3_open_v2, sqlite3_close, sqlite3_free.
 * 
 * Following the exact invocation sequence:
 * 1. sqlite3_initialize
 * 2. sqlite3_enable_load_extension
 * 3. sqlite3_create_filename
 * 4. sqlite3_open_v2
 * 5. sqlite3_auto_extension
 * 6. sqlite3_load_extension
 * 7. sqlite3_cancel_auto_extension
 * 8. sqlite3_close
 * 9. sqlite3_free_filename
 * 10. sqlite3_free
 * 
 * These 6 target APIs form a logical lifecycle for extension management and are currently
 * 0% covered despite high complexity. Focus on extension loading which is a critical,
 * security-sensitive area of SQLite.
 * 
 * Semantic differentiation from existing harnesses (000-012):
 * - First harness specifically targeting extension loading and management APIs
 * - Tests both static (auto) and dynamic (load) extension mechanisms
 * - Exercises filename creation with various options for extension loading
 * - Tests extension enable/disable configuration
 * - Covers security-sensitive extension loading paths
 */


// Forward declarations for test extension functions
// These simulate extension entry points for auto-extension testing

#ifdef __cplusplus
extern "C" {
#endif

/* Simple dummy extension init function for testing auto-extension */
static void test_extension_init_sqr(void) {
    // Dummy function for auto-extension testing
    // In real usage, this would call SQLITE_EXTENSION_INIT2 and register functions
    // For fuzzing, we just need a function pointer to test the APIs
}

/* Another dummy extension init function */
static void test_extension_init_cube(void) {
    // Dummy function for auto-extension testing
}

/* Broken extension init function that does nothing */
static void test_extension_init_broken(void) {
    // Dummy broken extension
}

#ifdef __cplusplus
} /* extern "C" */
#endif

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing of extension APIs
    // Minimum: operation type + filename + extension parameters
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Step 2: Determine test scenario from fuzzer input
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Step 3: Consume database filename and path options
    std::string db_name = fdp.ConsumeRandomLengthString(128);
    if (db_name.empty()) {
        db_name = ":memory:";  // Use in-memory database as fallback
    }
    
    std::string zj_path = fdp.ConsumeRandomLengthString(128);
    std::string zw_path = fdp.ConsumeRandomLengthString(128); // WAL file path
    
    // Step 4: Create database filename with options using sqlite3_create_filename
    // This API is used internally but can be tested
    const char* filename = nullptr;
    
    // Determine which parameters to pass based on fuzzer input
    uint8_t filename_flags = fdp.ConsumeIntegral<uint8_t>();
    
    // Use NULL for parameters we don't have or want to test specific combinations
    const char* zDatabase = db_name.c_str();
    const char* zJournal = zj_path.c_str();
    const char* zWal = zw_path.c_str();
    
    // Create parameter array for additional options
    // For simplicity, we'll use NULL parameters array
    int nParam = 0;
    const char** azParam = nullptr;
    
    // If fuzzer input allows, create some parameters
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        nParam = fdp.ConsumeIntegralInRange<int>(0, 4);
        if (nParam > 0) {
            // Create array of parameter strings
            // Note: This is simplified for fuzzing - real usage would have specific param meanings
            const char** params = new const char*[nParam];
            for (int i = 0; i < nParam; i++) {
                std::string param = fdp.ConsumeRandomLengthString(32);
                // Need to allocate memory that will persist until sqlite3_free_filename
                // For fuzzing simplicity, we'll use nullptr or static strings
                params[i] = nullptr; // Could use param.c_str() but need persistent storage
            }
            azParam = params;
        }
    }
    
    // Create filename with various options
    filename = sqlite3_create_filename(zDatabase, zJournal, zWal, nParam, azParam);
    
    // Clean up temporary parameter array if we created one
    if (azParam != nullptr) {
        delete[] azParam;
    }
    
    sqlite3* db = nullptr;
    int rc = SQLITE_OK;
    
    // Step 5: Open database with created filename or original name
    const char* open_filename = (filename != nullptr) ? filename : db_name.c_str();
    
    int open_flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (fdp.ConsumeBool()) {
        open_flags |= SQLITE_OPEN_MEMORY;
    }
    if (fdp.ConsumeBool()) {
        open_flags |= SQLITE_OPEN_URI;
    }
    if (fdp.ConsumeBool()) {
        open_flags |= SQLITE_OPEN_NOMUTEX;
    }
    
    rc = sqlite3_open_v2(open_filename, &db, open_flags, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        // Cleanup and return early if database open fails
        if (filename) {
            sqlite3_free_filename(filename);
        }
        sqlite3_shutdown();
        return 0;
    }

    
    // Set reasonable limits to prevent timeouts and excessive memory usage
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
    sqlite3_limit(db, SQLITE_LIMIT_LIKE_PATTERN_LENGTH, 250);
    
    // Step 6: Enable or disable extension loading based on fuzzer input
    int enable_ext = fdp.ConsumeBool() ? 1 : 0;
    rc = sqlite3_enable_load_extension(db, enable_ext);
    
    // Step 7: Test auto-extension registration based on scenario
    switch (scenario) {
        case 0: {
            // Register auto-extensions and then cancel some
            rc = sqlite3_auto_extension((void(*)(void))test_extension_init_sqr);
            rc = sqlite3_auto_extension((void(*)(void))test_extension_init_cube);
            
            // Try to cancel one of them
            int canceled = sqlite3_cancel_auto_extension((void(*)(void))test_extension_init_sqr);
            
            // Try to load extension (will likely fail for in-memory or missing files)
            char* errmsg = nullptr;
            std::string ext_file = fdp.ConsumeRandomLengthString(64);
            std::string ext_proc = fdp.ConsumeRandomLengthString(32);
            
            if (!ext_file.empty() && !ext_proc.empty()) {
                rc = sqlite3_load_extension(db, ext_file.c_str(), ext_proc.c_str(), &errmsg);
                if (errmsg) {
                    sqlite3_free(errmsg);
                    errmsg = nullptr;
                }
            }
            
            // Try broken extension
            rc = sqlite3_load_extension(db, "/nonexistent/library.so", "sqlite3_extension_init", &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = nullptr;
            }
            break;
        }
        
        case 1: {
            // Test with broken auto-extension
            rc = sqlite3_auto_extension((void(*)(void))test_extension_init_broken);
            
            // Cancel the broken extension
            int canceled = sqlite3_cancel_auto_extension((void(*)(void))test_extension_init_broken);
            
            // Try loading with empty/null parameters
            char* errmsg = nullptr;
            rc = sqlite3_load_extension(db, "", "", &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = nullptr;
            }
            
            rc = sqlite3_load_extension(db, "", "", &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = nullptr;
            }
            break;
        }
        
        case 2: {
            // Multiple auto-extensions and load attempts
            for (int i = 0; i < 3; i++) {
                if (fdp.ConsumeBool()) {
                    rc = sqlite3_auto_extension((void(*)(void))test_extension_init_sqr);
                } else {
                    rc = sqlite3_auto_extension((void(*)(void))test_extension_init_cube);
                }
            }
            
            // Try to load various extension names
            char* errmsg = nullptr;
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
                std::string lib_name = fdp.ConsumeRandomLengthString(32);
                std::string proc_name = fdp.ConsumeRandomLengthString(16);
                
                if (!lib_name.empty()) {
                    rc = sqlite3_load_extension(db, lib_name.c_str(), 
                                                proc_name.c_str(), 
                                                &errmsg);
                    if (errmsg) {
                        sqlite3_free(errmsg);
                        errmsg = nullptr;
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Test extension enable/disable toggle and auto-extensions
            for (int i = 0; i < 2; i++) {
                int toggle = fdp.ConsumeBool() ? 1 : 0;
                rc = sqlite3_enable_load_extension(db, toggle);
                
                if (toggle) {
                    // Try to register auto-extension when enabled
                    rc = sqlite3_auto_extension((void(*)(void))test_extension_init_sqr);
                }
            }
            
            // Cancel all auto-extensions
            int canceled_sqr = sqlite3_cancel_auto_extension((void(*)(void))test_extension_init_sqr);
            int canceled_cube = sqlite3_cancel_auto_extension((void(*)(void))test_extension_init_cube);
            int canceled_broken = sqlite3_cancel_auto_extension((void(*)(void))test_extension_init_broken);
            
            // Final load attempt
            char* errmsg = nullptr;
            rc = sqlite3_load_extension(db, "test_lib.so", "sqlite3_extension_init", &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = nullptr;
            }
            break;
        }
    }
    
    // Step 8: Try to use any registered extension functions if database is open
    if (db) {
        // Create a simple test table
        char* errmsg = nullptr;
        rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS ext_test (id INTEGER, value REAL);",
                         nullptr, nullptr, &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = nullptr;
        }
        
        // Try to call extension functions if they were registered
        // Note: For in-memory databases with auto-extensions, functions may be available
        rc = sqlite3_exec(db, "SELECT test_sqr(5), test_cube(3);", nullptr, nullptr, &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = nullptr;
        }
    }
    
    // Step 9: Cleanup - close database
    if (db) {
        sqlite3_close(db);
    }
    
    // Step 10: Free created filename if any
    if (filename) {
        sqlite3_free_filename(filename);
    }
    
    // Step 11: Shutdown SQLite
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_013
