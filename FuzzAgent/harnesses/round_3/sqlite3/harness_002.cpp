#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"

namespace harness_002 {

// Simple virtual table module implementation for fuzzing
typedef struct fuzz_vtab {
  sqlite3_vtab base;           /* Base class - must be first */
  int nRows;                   /* Number of rows in this virtual table */
  int currentRow;              /* Current row for iteration */
} fuzz_vtab;

typedef struct fuzz_cursor {
  sqlite3_vtab_cursor base;    /* Base class - must be first */
  int currentRow;              /* Current row */
  int maxRow;                  /* Maximum row to iterate to */
} fuzz_cursor;

// xConnect method - called when virtual table is referenced
static int fuzzConnect(
  sqlite3* db,
  void* pAux,
  int argc, const char* const* argv,
  sqlite3_vtab** ppVtab,
  char** pzErr
) {
  (void)argc;
  (void)argv;
  (void)pzErr;
  
  fuzz_vtab* pNew;
  int rc;
  
  // Declare the schema of the virtual table
  rc = sqlite3_declare_vtab(db,
    "CREATE TABLE fuzz_table(id INTEGER PRIMARY KEY, data TEXT, value INTEGER)");
  
  if (rc != SQLITE_OK) {
    return rc;
  }
  
  // Allocate the virtual table structure
  pNew = (fuzz_vtab*)sqlite3_malloc(sizeof(fuzz_vtab));
  if (pNew == nullptr) {
    return SQLITE_NOMEM;
  }
  
  memset(pNew, 0, sizeof(fuzz_vtab));
  // Use the client data (max_rows) passed from sqlite3_create_module_v2
  pNew->nRows = pAux ? (int)(intptr_t)pAux : 100;  // Cast back from void* to int
  *ppVtab = (sqlite3_vtab*)pNew;
  
  return SQLITE_OK;
}

// xDisconnect method - called when virtual table is no longer referenced
static int fuzzDisconnect(sqlite3_vtab* pVtab) {
  sqlite3_free(pVtab);
  return SQLITE_OK;
}

// xOpen method - creates a new cursor
static int fuzzOpen(sqlite3_vtab* pVtab, sqlite3_vtab_cursor** ppCursor) {
  fuzz_cursor* pCur;
  fuzz_vtab* pVtabImpl = (fuzz_vtab*)pVtab;
  
  pCur = (fuzz_cursor*)sqlite3_malloc(sizeof(fuzz_cursor));
  if (pCur == nullptr) {
    return SQLITE_NOMEM;
  }
  
  memset(pCur, 0, sizeof(fuzz_cursor));
  pCur->currentRow = 0;
  pCur->maxRow = pVtabImpl->nRows;
  *ppCursor = (sqlite3_vtab_cursor*)pCur;
  
  return SQLITE_OK;
}

// xClose method - closes a cursor
static int fuzzClose(sqlite3_vtab_cursor* cur) {
  sqlite3_free(cur);
  return SQLITE_OK;
}

// xFilter method - initializes a cursor for a query
static int fuzzFilter(
  sqlite3_vtab_cursor* pVtabCursor,
  int idxNum, const char* idxStr,
  int argc, sqlite3_value** argv
) {
  fuzz_cursor* pCur = (fuzz_cursor*)pVtabCursor;
  (void)idxNum;
  (void)idxStr;
  (void)argc;
  (void)argv;
  
  pCur->currentRow = 0;
  return SQLITE_OK;
}

// xNext method - advances to the next row
static int fuzzNext(sqlite3_vtab_cursor* pVtabCursor) {
  fuzz_cursor* pCur = (fuzz_cursor*)pVtabCursor;
  pCur->currentRow++;
  return SQLITE_OK;
}

// xEof method - checks if cursor is at end
static int fuzzEof(sqlite3_vtab_cursor* pVtabCursor) {
  fuzz_cursor* pCur = (fuzz_cursor*)pVtabCursor;
  return pCur->currentRow >= pCur->maxRow;
}

// xColumn method - returns column values
static int fuzzColumn(sqlite3_vtab_cursor* pVtabCursor, sqlite3_context* ctx, int col) {
  fuzz_cursor* pCur = (fuzz_cursor*)pVtabCursor;
  
  switch (col) {
    case 0:  // id column
      sqlite3_result_int(ctx, pCur->currentRow);
      break;
    case 1:  // data column - use dynamic data based on row number
      {
        char data_buf[32];
        snprintf(data_buf, sizeof(data_buf), "row_%d", pCur->currentRow);
        sqlite3_result_text(ctx, data_buf, -1, SQLITE_TRANSIENT);
      }
      break;
    case 2:  // value column
      sqlite3_result_int(ctx, pCur->currentRow * 10);
      break;
    default:
      sqlite3_result_null(ctx);
      break;
  }
  
  return SQLITE_OK;
}

// xRowid method - returns the rowid
static int fuzzRowid(sqlite3_vtab_cursor* pVtabCursor, sqlite3_int64* pRowid) {
  fuzz_cursor* pCur = (fuzz_cursor*)pVtabCursor;
  *pRowid = pCur->currentRow;
  return SQLITE_OK;
}

// xBestIndex method - helps query optimizer
static int fuzzBestIndex(sqlite3_vtab* pVtab, sqlite3_index_info* pIdxInfo) {
  (void)pVtab;
  (void)pIdxInfo;
  return SQLITE_OK;
}

// Virtual table module definition
static sqlite3_module fuzzModule = {
  1,                          /* iVersion */
  nullptr,                    /* xCreate - not implemented */
  fuzzConnect,                /* xConnect */
  fuzzBestIndex,              /* xBestIndex */
  fuzzDisconnect,             /* xDisconnect */
  nullptr,                    /* xDestroy - same as xDisconnect */
  fuzzOpen,                   /* xOpen */
  fuzzClose,                  /* xClose */
  fuzzFilter,                 /* xFilter */
  fuzzNext,                   /* xNext */
  fuzzEof,                    /* xEof */
  fuzzColumn,                 /* xColumn */
  fuzzRowid,                  /* xRowid */
  nullptr,                    /* xUpdate */
  nullptr,                    /* xBegin */
  nullptr,                    /* xSync */
  nullptr,                    /* xCommit */
  nullptr,                    /* xRollback */
  nullptr,                    /* xFindFunction */
  nullptr,                    /* xRename */
  nullptr,                    /* xSavepoint */
  nullptr,                    /* xRelease */
  nullptr,                    /* xRollbackTo */
  nullptr,                    /* xShadowName */
  nullptr,                    /* xIntegrity */
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for virtual table operations
  if (size < 16) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Consume fixed-size configuration parameters
  uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
  uint8_t module_flags = fdp.ConsumeIntegral<uint8_t>();
  int max_rows = fdp.ConsumeIntegralInRange<int>(1, 1000);
  bool use_transaction = fdp.ConsumeBool();
  bool drop_module = fdp.ConsumeBool();
  
  // Consume variable-size data
  std::string table_name = fdp.ConsumeRandomLengthString(64);
  std::string sql_query = fdp.ConsumeRandomLengthString(1024);
  std::string remaining_input = fdp.ConsumeRemainingBytesAsString();

  sqlite3* db = nullptr;
  char* err_msg = nullptr;
  int rc;

  // Initialize SQLite
  if (sqlite3_initialize() != SQLITE_OK) {
    return 0;
  }

  // Open in-memory database
  rc = sqlite3_open_v2(":memory:", &db, 
                      SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY, 
                      nullptr);
  if (rc != SQLITE_OK || db == nullptr) {
    sqlite3_shutdown();
    return 0;
  }

  // Register the virtual table module
  rc = sqlite3_create_module_v2(db, "fuzz_module", &fuzzModule, 
                                (void*)(intptr_t)max_rows, nullptr);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    sqlite3_shutdown();
    return 0;
  }

  // Test different operations based on the operation selector
  switch (operation) {
    case 0: {
      // Create and query virtual table
      std::string create_sql = "CREATE VIRTUAL TABLE " + table_name + 
                              " USING fuzz_module;";
      rc = sqlite3_exec(db, create_sql.c_str(), nullptr, nullptr, &err_msg);
      if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
      }
      
      if (rc == SQLITE_OK) {
        // Query the virtual table
        std::string select_sql = "SELECT * FROM " + table_name + 
                                " LIMIT " + std::to_string(max_rows / 10 + 1) + ";";
        rc = sqlite3_exec(db, select_sql.c_str(), nullptr, nullptr, &err_msg);
        if (err_msg != nullptr) {
          sqlite3_free(err_msg);
          err_msg = nullptr;
        }
        
        // Test more complex queries
        std::string where_sql = "SELECT id, value FROM " + table_name + 
                               " WHERE id % 2 = 0 ORDER BY value DESC;";
        rc = sqlite3_exec(db, where_sql.c_str(), nullptr, nullptr, &err_msg);
        if (err_msg != nullptr) {
          sqlite3_free(err_msg);
          err_msg = nullptr;
        }
      }
      break;
    }
    
    case 1: {
      // Test prepared statements with virtual table
      std::string create_sql = "CREATE VIRTUAL TABLE vt_" + table_name + 
                              " USING fuzz_module;";
      rc = sqlite3_exec(db, create_sql.c_str(), nullptr, nullptr, &err_msg);
      if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
      }
      
      if (rc == SQLITE_OK) {
        sqlite3_stmt* stmt = nullptr;
        std::string prep_sql = "SELECT * FROM vt_" + table_name + 
                              " WHERE id BETWEEN ? AND ?;";
        rc = sqlite3_prepare_v2(db, prep_sql.c_str(), -1, &stmt, nullptr);
        
        if (rc == SQLITE_OK && stmt != nullptr) {
          // Bind parameters from fuzzed data
          int start_id = fdp.ConsumeIntegralInRange<int>(0, max_rows / 2);
          int end_id = fdp.ConsumeIntegralInRange<int>(start_id, max_rows);
          
          sqlite3_bind_int(stmt, 1, start_id);
          sqlite3_bind_int(stmt, 2, end_id);
          
          // Execute the statement
          while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            // Access columns to ensure they're read
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
              int col_type = sqlite3_column_type(stmt, i);
              (void)col_type; // Prevent unused warning
            }
          }
          
          sqlite3_finalize(stmt);
        }
      }
      break;
    }
    
    case 2: {
      // Test multiple virtual tables and joins
      rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE vt1 USING fuzz_module;", 
                       nullptr, nullptr, &err_msg);
      if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
      }
      
      if (rc == SQLITE_OK) {
        rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE vt2 USING fuzz_module;", 
                         nullptr, nullptr, &err_msg);
        if (err_msg != nullptr) {
          sqlite3_free(err_msg);
          err_msg = nullptr;
        }
        
        if (rc == SQLITE_OK) {
          // Test join between virtual tables
          std::string join_sql = "SELECT vt1.id, vt1.value, vt2.value "
                                "FROM vt1 JOIN vt2 ON vt1.id = vt2.id "
                                "LIMIT 10;";
          rc = sqlite3_exec(db, join_sql.c_str(), nullptr, nullptr, &err_msg);
          if (err_msg != nullptr) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
          }
        }
      }
      break;
    }
    
    case 3: {
      // Test virtual table with transactions
      if (use_transaction) {
        rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &err_msg);
        if (err_msg != nullptr) {
          sqlite3_free(err_msg);
          err_msg = nullptr;
        }
      }
      
      rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE trans_vt USING fuzz_module;", 
                       nullptr, nullptr, &err_msg);
      if (err_msg != nullptr) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
      }
      
      if (rc == SQLITE_OK) {
        // Insert some data (though virtual tables don't normally support INSERT)
        // Just query it instead
        rc = sqlite3_exec(db, "SELECT COUNT(*) FROM trans_vt;", 
                         nullptr, nullptr, &err_msg);
        if (err_msg != nullptr) {
          sqlite3_free(err_msg);
          err_msg = nullptr;
        }
        
        // Test DROP virtual table
        rc = sqlite3_exec(db, "DROP TABLE trans_vt;", nullptr, nullptr, &err_msg);
        if (err_msg != nullptr) {
          sqlite3_free(err_msg);
          err_msg = nullptr;
        }
      }
      
      if (use_transaction) {
        rc = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &err_msg);
        if (err_msg != nullptr) {
          sqlite3_free(err_msg);
          err_msg = nullptr;
        }
      }
      break;
    }
  }

  // Test dropping the module if requested
  if (drop_module) {
    rc = sqlite3_drop_modules(db, nullptr);  // Remove all modules
    if (rc != SQLITE_OK) {
      // Try to drop just our module
      const char* keep[] = { nullptr };
      rc = sqlite3_drop_modules(db, keep);
    }
  }

  // Clean up
  if (db != nullptr) {
    sqlite3_close(db);
  }
  
  sqlite3_shutdown();
  return 0;
}

} // namespace harness_002
