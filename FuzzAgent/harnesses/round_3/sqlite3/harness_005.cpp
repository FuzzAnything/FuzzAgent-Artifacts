#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"

namespace harness_005 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for test control operations
  if (size < 16) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Consume fixed-size configuration parameters
  uint8_t test_opcode = fdp.ConsumeIntegral<uint8_t>();
  uint8_t flags = fdp.ConsumeIntegral<uint8_t>();
  int int_param = fdp.ConsumeIntegral<int>();
  int int_param2 = fdp.ConsumeIntegral<int>();
  bool use_db = fdp.ConsumeBool();
  bool test_multiple = fdp.ConsumeBool();
  
  // Consume variable-size data
  std::string string_data = fdp.ConsumeRandomLengthString(256);
  std::string remaining_input = fdp.ConsumeRemainingBytesAsString();

  sqlite3* db = nullptr;
  int rc = SQLITE_OK;
  
  // Initialize SQLite
  if (sqlite3_initialize() != SQLITE_OK) {
    return 0;
  }

  // Open an in-memory database if needed
  if (use_db) {
    rc = sqlite3_open_v2(":memory:", &db, 
                         SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY, 
                         nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
      sqlite3_shutdown();
      return 0;
    }
    
    // Set memory limits
    sqlite3_hard_heap_limit64(50000000); // 50MB limit
  }

  // Map test_opcode to actual TESTCTRL opcodes (range 5-34)
  int actual_opcode = SQLITE_TESTCTRL_FIRST + (test_opcode % (SQLITE_TESTCTRL_LAST - SQLITE_TESTCTRL_FIRST + 1));
  
  // Test different sqlite3_test_control operations based on the opcode
  // We'll test a subset of safe operations that don't cause side effects
  
  switch (actual_opcode) {
    case SQLITE_TESTCTRL_PRNG_SAVE: {
      // Save PRNG state
      rc = sqlite3_test_control(SQLITE_TESTCTRL_PRNG_SAVE);
      break;
    }
    
    case SQLITE_TESTCTRL_PRNG_RESTORE: {
      // Restore PRNG state (only works if PRNG_SAVE was called)
      rc = sqlite3_test_control(SQLITE_TESTCTRL_PRNG_RESTORE);
      break;
    }
    
    case SQLITE_TESTCTRL_BYTEORDER: {
      // Get byte order
      rc = sqlite3_test_control(SQLITE_TESTCTRL_BYTEORDER);
      break;
    }
    
    case SQLITE_TESTCTRL_ISINIT: {
      // Check if SQLite is initialized
      rc = sqlite3_test_control(SQLITE_TESTCTRL_ISINIT);
      break;
    }
    
    case SQLITE_TESTCTRL_ALWAYS: {
      // Test ALWAYS macro behavior
      rc = sqlite3_test_control(SQLITE_TESTCTRL_ALWAYS, int_param);
      break;
    }
    
    case SQLITE_TESTCTRL_ASSERT: {
      // Test ASSERT macro behavior - SKIPPED for fuzzing safety
      // This operation intentionally aborts when parameter is 0 and assert() is enabled
      // Skipping to avoid process termination during fuzzing
      break;
    }
    
    case SQLITE_TESTCTRL_NEVER_CORRUPT: {
      // Test NEVER_CORRUPT flag
      rc = sqlite3_test_control(SQLITE_TESTCTRL_NEVER_CORRUPT, int_param);
      break;
    }
    
    case SQLITE_TESTCTRL_EXTRA_SCHEMA_CHECKS: {
      // Test extra schema checks
      rc = sqlite3_test_control(SQLITE_TESTCTRL_EXTRA_SCHEMA_CHECKS, int_param);
      break;
    }
    
    case SQLITE_TESTCTRL_JSON_SELFCHECK: {
      // Test JSON self-check
      int onoff = int_param & 1;
      rc = sqlite3_test_control(SQLITE_TESTCTRL_JSON_SELFCHECK, &onoff);
      break;
    }
    
    case SQLITE_TESTCTRL_ONCE_RESET_THRESHOLD: {
      // Test ONCE reset threshold
      rc = sqlite3_test_control(SQLITE_TESTCTRL_ONCE_RESET_THRESHOLD, int_param);
      break;
    }
    
    case SQLITE_TESTCTRL_PRNG_SEED: {
      // Seed PRNG (requires db parameter)
      if (db != nullptr) {
        rc = sqlite3_test_control(SQLITE_TESTCTRL_PRNG_SEED, int_param, db);
      } else {
        // Without db, just seed with value
        rc = sqlite3_test_control(SQLITE_TESTCTRL_PRNG_SEED, int_param, nullptr);
      }
      break;
    }
    
    case SQLITE_TESTCTRL_OPTIMIZATIONS: {
      // Test optimizations control (requires db)
      if (db != nullptr) {
        rc = sqlite3_test_control(SQLITE_TESTCTRL_OPTIMIZATIONS, db, int_param);
      }
      break;
    }
    
    case SQLITE_TESTCTRL_GETOPT: {
      // Test getting compile-time options (requires db)
      if (db != nullptr) {
        int opt_value = 0;
        rc = sqlite3_test_control(SQLITE_TESTCTRL_GETOPT, db, &opt_value);
      }
      break;
    }
    
    case SQLITE_TESTCTRL_INTERNAL_FUNCTIONS: {
      // Test internal functions (requires db)
      if (db != nullptr) {
        rc = sqlite3_test_control(SQLITE_TESTCTRL_INTERNAL_FUNCTIONS, db);
      }
      break;
    }
    case SQLITE_TESTCTRL_BITVEC_TEST: {
      // BITVEC_TEST requires size and program array arguments
      // Skip this opcode to avoid null pointer dereference
      // According to crash report, it needs: int sz, int *aProg
      break;
    }
    
    case SQLITE_TESTCTRL_LOGEST: {
      // LOGEST requires: double fIn, int *pLogEst, u64 *pInt, int *pLogEst2
      // Provide valid local variables for pointers
      double fIn = static_cast<double>(int_param);
      int logEst = 0;
      sqlite3_uint64 intVal = 0;
      int logEst2 = 0;
      rc = sqlite3_test_control(SQLITE_TESTCTRL_LOGEST, fIn, &logEst, &intVal, &logEst2);
      break;
    }
    
    case SQLITE_TESTCTRL_ATOF: {
      // ATOF requires: const char *z, double *pR
      double result = 0.0;
      rc = sqlite3_test_control(SQLITE_TESTCTRL_ATOF, string_data.c_str(), &result);
      break;
    }
    
    case SQLITE_TESTCTRL_TRACEFLAGS: {
      // TRACEFLAGS requires: int opTrace, u32 *ptr
      uint32_t traceValue = 0;
      rc = sqlite3_test_control(SQLITE_TESTCTRL_TRACEFLAGS, int_param & 3, &traceValue);
      break;
    }
    
    case SQLITE_TESTCTRL_TUNE: {
      // TUNE requires: int id, int *piValue (debug only)
      #if defined(SQLITE_DEBUG) && !defined(SQLITE_OMIT_WSD)
      int tuneValue = int_param2;
      rc = sqlite3_test_control(SQLITE_TESTCTRL_TUNE, int_param, &tuneValue);
      #else
      // Skip in non-debug builds
      break;
      #endif
      break;
    }
    
    default: {
      // For other opcodes that don't require arguments, try a simple call
      // But first check if it's one of the safe no-argument opcodes
      if (actual_opcode == SQLITE_TESTCTRL_PRNG_SAVE ||
          actual_opcode == SQLITE_TESTCTRL_PRNG_RESTORE ||
          actual_opcode == SQLITE_TESTCTRL_BYTEORDER ||
          actual_opcode == SQLITE_TESTCTRL_ISINIT) {
        rc = sqlite3_test_control(actual_opcode);
      }
      // Otherwise skip potentially dangerous opcodes
      break;
    }
  }

  // If test_multiple is true, test additional diagnostic operations
  if (test_multiple && db != nullptr) {
    // Test some diagnostic functions that are related to testing
    int value = 0;
    
    // Test sqlite3_db_status
    sqlite3_db_status(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &value, &rc, 0);
    sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_USED, &value, &rc, 0);
    
    // Test sqlite3_status
    sqlite3_status(SQLITE_STATUS_MEMORY_USED, &value, &rc, 0);
    sqlite3_status(SQLITE_STATUS_MALLOC_COUNT, &value, &rc, 0);
    
    // Test sqlite3_stmt_status on a dummy statement
    sqlite3_stmt* stmt = nullptr;
    const char* dummy_sql = "SELECT 1";
    if (sqlite3_prepare_v2(db, dummy_sql, -1, &stmt, nullptr) == SQLITE_OK && stmt != nullptr) {
      sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_FULLSCAN_STEP, 1);
      sqlite3_finalize(stmt);
    }
    
    // Test compile-time option checking
    const char* option_name = "SQLITE_THREADSAFE";
    int option_used = sqlite3_compileoption_used(option_name);
    const char* option_get = sqlite3_compileoption_get(0);
    (void)option_used;
    (void)option_get;
    
    // Test memory debugging if enabled
    #ifdef SQLITE_DEBUG
    sqlite3_memdebug_backtrace();
    #endif
  }

  // Clean up
  if (db != nullptr) {
    sqlite3_close(db);
  }
  
  sqlite3_shutdown();
  
  return 0;
}

} // namespace harness_005
