// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_vtab_in_first at sqlite3.c:79335:16 in sqlite3.h
// sqlite3_vtab_in_next at sqlite3.c:79343:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned int u32;

typedef struct Mem {
    union {
        double r;
        sqlite3_int64 i;
        int nZero;
        const char *zPType;
        void *pDef;
    } u;
    char *z;
    int n;
    u16 flags;
    u8 enc;
    u8 eSubtype;
    sqlite3 *db;
    int szMalloc;
    u32 uTemp;
    char *zMalloc;
    void (*xDel)(void *);
#ifdef SQLITE_DEBUG
    struct Mem *pScopyFrom;
    u16 mScopyFlags;
    u8 bScopy;
#endif
} Mem;

static int dummy_xDestroy(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_xDisconnect(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    return SQLITE_OK;
}

static int dummy_xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    return SQLITE_OK;
}

static int dummy_xClose(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) {
    return SQLITE_OK;
}

static int dummy_xNext(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_xEof(sqlite3_vtab_cursor *pCursor) {
    return 1;
}

static int dummy_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) {
    return SQLITE_OK;
}

static int dummy_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static sqlite3_module dummy_module = {
    0,
    NULL,
    NULL,
    dummy_xBestIndex,
    dummy_xDestroy,
    dummy_xDisconnect,
    dummy_xOpen,
    dummy_xClose,
    dummy_xFilter,
    dummy_xNext,
    dummy_xEof,
    dummy_xColumn,
    dummy_xRowid,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

static sqlite3_value* create_dummy_value(const uint8_t *data, size_t size) {
    Mem *val = (Mem*)malloc(sizeof(Mem));
    if (!val) return (sqlite3_value*)NULL;
    
    memset(val, 0, sizeof(Mem));
    val->enc = SQLITE_UTF8;
    
    if (size > 0) {
        val->z = (char*)malloc(size + 1);
        if (val->z) {
            memcpy(val->z, data, size);
            val->z[size] = '\0';
            val->n = (int)size;
            val->flags = 0x02 | 0x08;
            val->szMalloc = (int)size + 1;
            val->zMalloc = val->z;
        }
    } else {
        val->flags = 0x01;
    }
    
    return (sqlite3_value*)val;
}

static void free_dummy_value(sqlite3_value *val) {
    if (val) {
        Mem *m = (Mem*)val;
        if (m->zMalloc) {
            free(m->zMalloc);
        }
        free(m);
    }
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    sqlite3 *db = NULL;
    int rc;
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        return 0;
    }
    
    rc = sqlite3_create_module(db, "dummy_vtab", &dummy_module, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    if (Size > 10) {
        char func_name[64];
        size_t name_len = Size > 74 ? 64 : Size - 10;
        memcpy(func_name, Data + 1, name_len);
        func_name[name_len < 64 ? name_len : 63] = '\0';
        
        int nArg = Data[1] % 10;
        sqlite3_overload_function(db, func_name, nArg);
    }
    
    sqlite3_index_info idxInfo;
    memset(&idxInfo, 0, sizeof(idxInfo));
    
    static struct sqlite3_index_constraint constraints[2];
    static struct sqlite3_index_constraint_usage constraintUsage[2];
    
    constraints[0].iColumn = -1;
    constraints[0].op = 2;
    constraints[0].usable = 1;
    constraints[0].iTermOffset = 0;
    
    constraints[1].iColumn = 0;
    constraints[1].op = 27;
    constraints[1].usable = 1;
    constraints[1].iTermOffset = 0;
    
    idxInfo.nConstraint = 2;
    idxInfo.aConstraint = constraints;
    idxInfo.aConstraintUsage = constraintUsage;
    
    constraintUsage[0].argvIndex = 0;
    constraintUsage[0].omit = 0;
    constraintUsage[1].argvIndex = 1;
    constraintUsage[1].omit = 0;
    
    int bHandle = (int)Data[2] % 3 - 1;
    int result = sqlite3_vtab_in(&idxInfo, 1, bHandle);
    
    sqlite3_value *rhs_val = NULL;
    rc = sqlite3_vtab_rhs_value(&idxInfo, 0, &rhs_val);
    
    sqlite3_value *in_val = create_dummy_value(Data, Size > 100 ? 100 : Size);
    if (in_val) {
        sqlite3_value *first_out = NULL;
        rc = sqlite3_vtab_in_first(in_val, &first_out);
        
        if (rc == SQLITE_OK && first_out) {
            sqlite3_value *next_out = NULL;
            int count = 0;
            while (count < 10) {
                rc = sqlite3_vtab_in_next(first_out, &next_out);
                if (rc != SQLITE_ROW) break;
                count++;
                first_out = next_out;
                next_out = NULL;
            }
        }
        
        free_dummy_value(in_val);
    }
    
    sqlite3_close(db);
    
    return 0;
}