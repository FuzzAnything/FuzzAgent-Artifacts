#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"

namespace harness_010 {


// Simple virtual table implementation to test vtab APIs
typedef struct simple_vtab {
    sqlite3_vtab base;  // Must be first
    sqlite3* db;        // Database connection for vtab APIs
    // Add any custom fields here
    int dummy;
} simple_vtab;

typedef struct simple_cursor {
    sqlite3_vtab_cursor base;  // Must be first
    sqlite3_int64 rowid;
    int eof;
} simple_cursor;

// xConnect method - called when virtual table is attached
static int simpleConnect(
    sqlite3* db,
    void* pAux,
    int argc, const char* const* argv,
    sqlite3_vtab** ppVtab,
    char** pzErr
) {
    simple_vtab* pNew;
    int rc;
    
    // Call sqlite3_declare_vtab to define table schema
    rc = sqlite3_declare_vtab(db,
        "CREATE TABLE x(id INTEGER PRIMARY KEY, name TEXT, value REAL, flag BOOLEAN)");
    
    if (rc != SQLITE_OK) {
        return rc;
    }
    
    pNew = (simple_vtab*)sqlite3_malloc(sizeof(simple_vtab));
    if (pNew == nullptr) {
        return SQLITE_NOMEM;
    }
    memset(pNew, 0, sizeof(simple_vtab));
    pNew->db = db;
    
    // Call sqlite3_vtab_config to set various options
    // Test SQLITE_VTAB_CONSTRAINT_SUPPORT
    sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    // Test SQLITE_VTAB_INNOCUOUS
    sqlite3_vtab_config(db, SQLITE_VTAB_INNOCUOUS);
    // Test SQLITE_VTAB_DIRECTONLY  
    sqlite3_vtab_config(db, SQLITE_VTAB_DIRECTONLY);
    
    *ppVtab = &pNew->base;
    return SQLITE_OK;
}

// xCreate method - same as xConnect for this simple table
static int simpleCreate(
    sqlite3* db,
    void* pAux,
    int argc, const char* const* argv,
    sqlite3_vtab** ppVtab,
    char** pzErr
) {
    return simpleConnect(db, pAux, argc, argv, ppVtab, pzErr);
}

// xDisconnect method
static int simpleDisconnect(sqlite3_vtab* pVtab) {
    sqlite3_free(pVtab);
    return SQLITE_OK;
}

// xDestroy method
static int simpleDestroy(sqlite3_vtab* pVtab) {
    return simpleDisconnect(pVtab);
}

// xOpen method
static int simpleOpen(sqlite3_vtab* pVtab, sqlite3_vtab_cursor** ppCursor) {
    simple_cursor* pCur = (simple_cursor*)sqlite3_malloc(sizeof(simple_cursor));
    if (pCur == nullptr) {
        return SQLITE_NOMEM;
    }
    memset(pCur, 0, sizeof(simple_cursor));
    pCur->rowid = 0;
    pCur->eof = 0;
    *ppCursor = &pCur->base;
    return SQLITE_OK;
}

// xClose method
static int simpleClose(sqlite3_vtab_cursor* cur) {
    sqlite3_free(cur);
    return SQLITE_OK;
}

// xFilter method
static int simpleFilter(
    sqlite3_vtab_cursor* cur,
    int idxNum, const char* idxStr,
    int argc, sqlite3_value** argv
) {
    simple_cursor* pCur = (simple_cursor*)cur;
    pCur->rowid = 0;
    pCur->eof = 0;
    return SQLITE_OK;
}

// xNext method
static int simpleNext(sqlite3_vtab_cursor* cur) {
    simple_cursor* pCur = (simple_cursor*)cur;
    pCur->rowid++;
    if (pCur->rowid > 10) {  // Return up to 10 rows
        pCur->eof = 1;
    }
    return SQLITE_OK;
}

// xEof method
static int simpleEof(sqlite3_vtab_cursor* cur) {
    simple_cursor* pCur = (simple_cursor*)cur;
    return pCur->eof;
}

// xColumn method
static int simpleColumn(sqlite3_vtab_cursor* cur, sqlite3_context* ctx, int i) {
    simple_cursor* pCur = (simple_cursor*)cur;
    
    // Test sqlite3_vtab_nochange
    if (sqlite3_vtab_nochange(ctx) == 0) {
        // Not a nochange context, set values
        switch (i) {
            case 0:  // id
                sqlite3_result_int64(ctx, pCur->rowid);
                break;
            case 1:  // name
                sqlite3_result_text(ctx, "test", 4, SQLITE_STATIC);
                break;
            case 2:  // value
                sqlite3_result_double(ctx, 3.14159);
                break;
            case 3:  // flag
                sqlite3_result_int(ctx, 1);
                break;
        }
    }
    return SQLITE_OK;
}

// xRowid method
static int simpleRowid(sqlite3_vtab_cursor* cur, sqlite3_int64* pRowid) {
    simple_cursor* pCur = (simple_cursor*)cur;
    *pRowid = pCur->rowid;
    return SQLITE_OK;
}

// xBestIndex method - where we can test many vtab APIs
static int simpleBestIndex(sqlite3_vtab* pVtab, sqlite3_index_info* pIdxInfo) {
    int i;
    int nArg = 0;
    
    // Test sqlite3_vtab_distinct
    int distinct = sqlite3_vtab_distinct(pIdxInfo);
    // distinct can be 0, 1, 2, or 3
    
    // Test sqlite3_vtab_in interface
    for (i = 0; i < pIdxInfo->nConstraint; i++) {
        // Use the constraint directly from the array
        if (pIdxInfo->aConstraint[i].usable) {
            // Test if this constraint uses IN operator
            int is_in = sqlite3_vtab_in(pIdxInfo, i, -1);
            if (is_in) {
                // Mark as used
                pIdxInfo->aConstraintUsage[i].argvIndex = ++nArg;
                pIdxInfo->aConstraintUsage[i].omit = 1;
            } else {
                // For other constraints, try to get rhs value
                sqlite3_value* pVal = nullptr;
                int rc = sqlite3_vtab_rhs_value(pIdxInfo, i, &pVal);
                if (rc == SQLITE_OK && pVal != nullptr) {
                    // Successfully got RHS value
                    pIdxInfo->aConstraintUsage[i].argvIndex = ++nArg;
                    pIdxInfo->aConstraintUsage[i].omit = 1;
                }
            }
        }
    }
    
    // Test sqlite3_vtab_collation
    for (i = 0; i < pIdxInfo->nOrderBy; i++) {
        const char* collation = sqlite3_vtab_collation(pIdxInfo, i);
        // collation could be nullptr or a collation name
    }
    
    // Set estimated rows and cost
    pIdxInfo->estimatedRows = 100;
    pIdxInfo->estimatedCost = 100.0;
    
    return SQLITE_OK;
}

// xUpdate method - to test sqlite3_vtab_on_conflict
static int simpleUpdate(
    sqlite3_vtab* pVtab,
    int argc, sqlite3_value** argv,
    sqlite3_int64* pRowid
) {
    simple_vtab* p = (simple_vtab*)pVtab;
    // Test sqlite3_vtab_on_conflict
    int conflict = sqlite3_vtab_on_conflict(p->db);
    // conflict can be SQLITE_ROLLBACK, SQLITE_ABORT, SQLITE_FAIL, 
    // SQLITE_IGNORE, or SQLITE_REPLACE
    
    // For this simple table, we'll just return SQLITE_OK
    if (argc == 1) {
        // DELETE operation
        return SQLITE_OK;
    } else if (argc > 1 && sqlite3_value_type(argv[0]) == SQLITE_NULL) {
        // INSERT operation
        if (pRowid) {
            *pRowid = 1;
        }
        return SQLITE_OK;
    } else {
        // UPDATE operation
        return SQLITE_OK;
    }
}

// Virtual table module definition
static sqlite3_module simpleModule = {
    0,                          // iVersion
    simpleCreate,               // xCreate
    simpleConnect,              // xConnect
    simpleBestIndex,            // xBestIndex
    simpleDisconnect,           // xDisconnect
    simpleDestroy,              // xDestroy
    simpleOpen,                 // xOpen
    simpleClose,                // xClose
    simpleFilter,               // xFilter
    simpleNext,                 // xNext
    simpleEof,                  // xEof
    simpleColumn,               // xColumn
    simpleRowid,                // xRowid
    simpleUpdate,               // xUpdate
    nullptr,                    // xBegin
    nullptr,                    // xSync
    nullptr,                    // xCommit
    nullptr,                    // xRollback
    nullptr,                    // xFindMethod
    nullptr,                    // xRename
    nullptr,                    // xSavepoint
    nullptr,                    // xRelease
    nullptr,                    // xRollbackTo
    nullptr,                    // xShadowName
    nullptr                     // xIntegrity
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for fuzzing parameters
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fuzzer input for test parameters
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool test_in_clause = fdp.ConsumeBool();
    bool test_multiple_constraints = fdp.ConsumeBool();
    bool test_order_by = fdp.ConsumeBool();
    
    // Consume some values for WHERE clause
    int int_value = fdp.ConsumeIntegral<int>();
    double real_value = fdp.ConsumeFloatingPoint<double>();
    std::string text_value = fdp.ConsumeRandomLengthString(100);
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    int rc;
    
    // 1. Call sqlite3_initialize - PRIMARY TARGET
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // 2. Call sqlite3_open - PRIMARY TARGET
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // 3. Call sqlite3_create_module - PRIMARY TARGET
    rc = sqlite3_create_module(db, "simple_vtab", &simpleModule, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // 4. Execute SQL with CREATE VIRTUAL TABLE
    const char* create_vtab_sql = "CREATE VIRTUAL TABLE vt_test USING simple_vtab";
    rc = sqlite3_exec(db, create_vtab_sql, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        // Clean up and continue - module might already exist
    }
    
    // Build a query based on fuzzer input
    std::string query = "SELECT * FROM vt_test";
    
    // Add WHERE clause based on fuzzer input
    if (test_multiple_constraints) {
        if (test_in_clause) {
            // Use IN clause to trigger sqlite3_vtab_in APIs
            query += " WHERE id IN (1, 2, 3, 4, 5)";
        } else {
            query += " WHERE id = ? AND name = ? AND value > ?";
        }
    } else if (test_scenario == 0) {
        query += " WHERE id = ?";
    } else if (test_scenario == 1) {
        query += " WHERE name LIKE ?";
    } else if (test_scenario == 2) {
        query += " WHERE value BETWEEN ? AND ?";
    }
    
    // Add ORDER BY to test sqlite3_vtab_collation
    if (test_order_by) {
        query += " ORDER BY name COLLATE BINARY, value";
    }
    
    // Prepare the statement - this will trigger xBestIndex
    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt != nullptr) {
        // Bind parameters if needed
        if (test_multiple_constraints && !test_in_clause) {
            sqlite3_bind_int(stmt, 1, int_value);
            sqlite3_bind_text(stmt, 2, text_value.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_double(stmt, 3, real_value);
        } else if (test_scenario == 0) {
            sqlite3_bind_int(stmt, 1, int_value);
        } else if (test_scenario == 1) {
            sqlite3_bind_text(stmt, 1, text_value.c_str(), -1, SQLITE_TRANSIENT);
        } else if (test_scenario == 2) {
            sqlite3_bind_double(stmt, 1, real_value - 10.0);
            sqlite3_bind_double(stmt, 2, real_value + 10.0);
        }
        
        // Execute the query
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process rows if any
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                // Access column data
                int col_type = sqlite3_column_type(stmt, i);
            }
        }
        
        sqlite3_finalize(stmt);
    }
    
    // Test UPDATE to trigger sqlite3_vtab_on_conflict
    const char* update_sql = "UPDATE vt_test SET name = 'updated' WHERE id = 1";
    sqlite3_exec(db, update_sql, nullptr, nullptr, nullptr);
    
    // Test DELETE
    const char* delete_sql = "DELETE FROM vt_test WHERE id = 1";
    sqlite3_exec(db, delete_sql, nullptr, nullptr, nullptr);
    
    // Test INSERT
    const char* insert_sql = "INSERT INTO vt_test (name, value, flag) VALUES ('new', 42.5, 1)";
    sqlite3_exec(db, insert_sql, nullptr, nullptr, nullptr);
    
    // 13. Call sqlite3_close_v2 - PRIMARY TARGET (0 hits)
    sqlite3_close_v2(db);
    
    // Cleanup
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_010
