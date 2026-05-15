// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:156966:24 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_vtab_distinct at sqlite3.c:157041:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
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

static const uint8_t *g_fuzz_data = NULL;
static size_t g_fuzz_size = 0;
static size_t g_fuzz_offset = 0;

static uint8_t fuzz_read_byte(void) {
    if (g_fuzz_offset >= g_fuzz_size) {
        return 0;
    }
    return g_fuzz_data[g_fuzz_offset++];
}

static int fuzz_read_int(void) {
    int value = 0;
    for (int i = 0; i < 4; i++) {
        value = (value << 8) | fuzz_read_byte();
    }
    return value;
}

static void fuzz_reset(void) {
    g_fuzz_offset = 0;
}

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

static int mock_xBestIndex(sqlite3_index_info *pIdxInfo) {
    if (!pIdxInfo) return 0;
    
    int nCons = pIdxInfo->nConstraint;
    if (nCons <= 0) return 0;
    
    for (int i = 0; i < nCons; i++) {
        if (i >= 10) break;
        
        int bHandle = fuzz_read_byte() % 3 - 1;
        int result = sqlite3_vtab_in(pIdxInfo, i, bHandle);
        
        const char *coll = sqlite3_vtab_collation(pIdxInfo, i);
        
        sqlite3_value *pVal = NULL;
        int rhs_result = sqlite3_vtab_rhs_value(pIdxInfo, i, &pVal);
    }
    
    int distinct_result = sqlite3_vtab_distinct(pIdxInfo);
    
    return 0;
}

int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    g_fuzz_data = Data;
    g_fuzz_size = Size;
    fuzz_reset();
    
    create_dummy_file(Data, Size);
    
    sqlite3 *db = NULL;
    int rc = sqlite3_open_v2("./dummy_file", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    
    if (rc == SQLITE_OK && db) {
        for (int i = 0; i < 5; i++) {
            int nArg = fuzz_read_byte() % 10;
            sqlite3_overload_function(db, "fuzz_func", nArg);
        }
        
        for (int i = 0; i < 5; i++) {
            int op = fuzz_read_byte() % 10;
            sqlite3_vtab_config(db, op, fuzz_read_int());
        }
        
        sqlite3_index_info idxInfo;
        memset(&idxInfo, 0, sizeof(idxInfo));
        
        idxInfo.nConstraint = fuzz_read_byte() % 10;
        if (idxInfo.nConstraint > 0) {
            struct sqlite3_index_constraint constraints[10];
            struct sqlite3_index_constraint_usage constraintUsage[10];
            
            for (int i = 0; i < idxInfo.nConstraint && i < 10; i++) {
                constraints[i].iColumn = fuzz_read_byte() % 10 - 1;
                constraints[i].op = fuzz_read_byte() % 50;
                constraints[i].usable = fuzz_read_byte() & 1;
                constraints[i].iTermOffset = 0;
                
                constraintUsage[i].argvIndex = fuzz_read_byte() % 10;
                constraintUsage[i].omit = fuzz_read_byte() & 1;
            }
            
            idxInfo.aConstraint = constraints;
            idxInfo.aConstraintUsage = constraintUsage;
            
            idxInfo.nOrderBy = fuzz_read_byte() % 5;
            if (idxInfo.nOrderBy > 0) {
                struct sqlite3_index_orderby orderBy[5];
                for (int i = 0; i < idxInfo.nOrderBy && i < 5; i++) {
                    orderBy[i].iColumn = fuzz_read_byte() % 10;
                    orderBy[i].desc = fuzz_read_byte() & 1;
                }
                idxInfo.aOrderBy = orderBy;
            }
            
            mock_xBestIndex(&idxInfo);
        }
        
        sqlite3_close(db);
    }
    
    cleanup_dummy_file();
    return 0;
}