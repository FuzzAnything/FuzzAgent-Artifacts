// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_vtab_distinct at sqlite3.c:157041:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:156966:24 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:156966:24 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Global variables to simulate virtual table context
static sqlite3 *g_db = NULL;
static sqlite3_index_info g_index_info;
static int g_initialized = 0;

// Helper function to initialize test structures
static void initialize_test_structures(void) {
    if (g_initialized) return;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &g_db) != SQLITE_OK) {
        return;
    }
    
    // Initialize a dummy sqlite3_index_info structure
    memset(&g_index_info, 0, sizeof(g_index_info));
    g_index_info.nConstraint = 1;
    g_index_info.nOrderBy = 1;
    
    // Allocate constraint array
    g_index_info.aConstraint = (struct sqlite3_index_constraint*)malloc(sizeof(struct sqlite3_index_constraint));
    if (g_index_info.aConstraint) {
        g_index_info.aConstraint[0].iColumn = 0;
        g_index_info.aConstraint[0].op = SQLITE_INDEX_CONSTRAINT_EQ;
        g_index_info.aConstraint[0].usable = 1;
    }
    
    // Allocate order by array
    g_index_info.aOrderBy = (struct sqlite3_index_orderby*)malloc(sizeof(struct sqlite3_index_orderby));
    if (g_index_info.aOrderBy) {
        g_index_info.aOrderBy[0].iColumn = 0;
        g_index_info.aOrderBy[0].desc = 0;
    }
    
    // Allocate constraint usage array
    g_index_info.aConstraintUsage = (struct sqlite3_index_constraint_usage*)malloc(sizeof(struct sqlite3_index_constraint_usage));
    if (g_index_info.aConstraintUsage) {
        g_index_info.aConstraintUsage[0].argvIndex = 1;
        g_index_info.aConstraintUsage[0].omit = 0;
    }
    
    g_initialized = 1;
}

// Helper function to cleanup test structures
static void cleanup_test_structures(void) {
    if (!g_initialized) return;
    
    if (g_index_info.aConstraint) free(g_index_info.aConstraint);
    if (g_index_info.aOrderBy) free(g_index_info.aOrderBy);
    if (g_index_info.aConstraintUsage) free(g_index_info.aConstraintUsage);
    
    if (g_db) sqlite3_close(g_db);
    
    g_initialized = 0;
}

// Helper to write data to dummy file
static void write_to_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

// Fuzzer entry point
int LLVMFuzzerTestOneInput_45(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    // Initialize test structures
    initialize_test_structures();
    if (!g_initialized) return 0;
    
    // Write data to dummy file for any file operations
    write_to_dummy_file(data, size);
    
    // Use first byte to determine which APIs to test
    uint8_t selector = data[0];
    size_t data_offset = 1;
    
    // Test sqlite3_vtab_config with various operations
    if (selector & 0x01) {
        // Try different virtual table configuration options
        int ops[] = {
            SQLITE_VTAB_CONSTRAINT_SUPPORT,
            SQLITE_VTAB_INNOCUOUS,
            SQLITE_VTAB_DIRECTONLY
        };
        
        for (int i = 0; i < 3 && data_offset < size; i++) {
            // Use data bytes for additional arguments if needed
            int arg = data_offset < size ? data[data_offset] : 0;
            sqlite3_vtab_config(g_db, ops[i % 3], arg);
            data_offset++;
        }
    }
    
    // Test sqlite3_vtab_rhs_value
    if (selector & 0x02) {
        sqlite3_value *out_val = NULL;
        int constraint_idx = 0;  // Always test with first constraint
        
        if (data_offset < size) {
            constraint_idx = data[data_offset] % (g_index_info.nConstraint + 1);
            data_offset++;
        }
        
        sqlite3_vtab_rhs_value(&g_index_info, constraint_idx, &out_val);
    }
    
    // Test sqlite3_vtab_in
    if (selector & 0x04) {
        int constraint_idx = 0;
        int bHandle = -1;  // Start with feasibility check
        
        if (data_offset + 1 < size) {
            constraint_idx = data[data_offset] % (g_index_info.nConstraint + 1);
            bHandle = (int8_t)data[data_offset + 1] % 3 - 1;  // Range -1 to 1
            data_offset += 2;
        }
        
        sqlite3_vtab_in(&g_index_info, constraint_idx, bHandle);
    }
    
    // Test sqlite3_overload_function
    if (selector & 0x08) {
        // Create function name from data
        char func_name[32] = "fuzz_func";
        int nArg = 1;
        
        if (data_offset < size) {
            nArg = data[data_offset] % 10;  // 0-9 arguments
            data_offset++;
        }
        
        // Try to create a few different function names
        for (int i = 0; i < 3 && data_offset + i < size; i++) {
            snprintf(func_name, sizeof(func_name), "func_%c", 'A' + (data[data_offset + i] % 26));
            sqlite3_overload_function(g_db, func_name, nArg);
        }
        data_offset += 3;
    }
    
    // Test sqlite3_vtab_distinct
    if (selector & 0x10) {
        sqlite3_vtab_distinct(&g_index_info);
    }
    
    // Test sqlite3_vtab_collation
    if (selector & 0x20) {
        int constraint_idx = 0;
        
        if (data_offset < size) {
            constraint_idx = data[data_offset] % (g_index_info.nConstraint + 1);
            data_offset++;
        }
        
        const char *collation = sqlite3_vtab_collation(&g_index_info, constraint_idx);
        (void)collation;  // Use result to avoid unused variable warning
    }
    
    // Test combinations and edge cases with remaining data
    while (data_offset < size) {
        // Use data to create more diverse test cases
        int test_case = data[data_offset] % 8;
        
        switch (test_case) {
            case 0:
                // Test vtab_config with invalid op
                sqlite3_vtab_config(g_db, 999, 0);
                break;
            case 1:
                // Test vtab_rhs_value with out-of-bounds index
                {
                    sqlite3_value *out_val = NULL;
                    sqlite3_vtab_rhs_value(&g_index_info, -1, &out_val);
                    sqlite3_vtab_rhs_value(&g_index_info, 999, &out_val);
                }
                break;
            case 2:
                // Test vtab_in with extreme bHandle values
                sqlite3_vtab_in(&g_index_info, 0, -2);
                sqlite3_vtab_in(&g_index_info, 0, 2);
                break;
            case 3:
                // Test overload_function with empty name
                sqlite3_overload_function(g_db, "", 0);
                break;
            case 4:
                // Test vtab_collation with negative index
                sqlite3_vtab_collation(&g_index_info, -1);
                break;
            case 5:
                // Multiple vtab_config calls
                sqlite3_vtab_config(g_db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
                sqlite3_vtab_config(g_db, SQLITE_VTAB_INNOCUOUS, 0);
                break;
            case 6:
                // Chain vtab_in calls
                sqlite3_vtab_in(&g_index_info, 0, -1);
                sqlite3_vtab_in(&g_index_info, 0, 1);
                break;
            case 7:
                // Test with NULL database (should crash but we try to avoid)
                if (data_offset + 1 < size && (data[data_offset + 1] & 0x80)) {
                    // Only test with NULL occasionally to avoid too many crashes
                    sqlite3_vtab_config(NULL, SQLITE_VTAB_CONSTRAINT_SUPPORT, 0);
                }
                break;
        }
        
        data_offset++;
    }
    
    return 0;
}