// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_on_conflict at sqlite3.c:146910:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global variables for module data */
static sqlite3_module g_module;
static int g_module_initialized = 0;

/* Static helper functions */
static void dummy_destroy(void *p) {
    /* Do nothing */
}

static int dummy_xCreate(sqlite3 *db, void *pAux,
                         int argc, const char *const *argv,
                         sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux,
                          int argc, const char *const *argv,
                          sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    return SQLITE_OK;
}

static int dummy_xUpdate(sqlite3_vtab *pVTab, int nArg,
                         sqlite3_value **argv, sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

/* Initialize module structure once */
static void init_module(void) {
    if (g_module_initialized) return;
    
    memset(&g_module, 0, sizeof(g_module));
    g_module.iVersion = 1;
    g_module.xCreate = dummy_xCreate;
    g_module.xConnect = dummy_xConnect;
    g_module.xBestIndex = dummy_xBestIndex;
    g_module.xUpdate = dummy_xUpdate;
    g_module.xDestroy = NULL;
    g_module.xDisconnect = NULL;
    g_module.xOpen = NULL;
    g_module.xClose = NULL;
    g_module.xFilter = NULL;
    g_module.xNext = NULL;
    g_module.xEof = NULL;
    g_module.xColumn = NULL;
    g_module.xRowid = NULL;
    
    g_module_initialized = 1;
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *zErrMsg = NULL;
    
    /* Initialize database connection */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Initialize module if needed */
    init_module();
    
    /* Use input data to determine which functions to test */
    if (Size == 0) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Extract some parameters from fuzzer input */
    int op = Data[0] % 5;  /* SQLITE_VTAB_CONSTRAINT_SUPPORT, etc. */
    int iCons = Data[1] % 10;
    int bHandle = (Data[2] % 3) - 1;  /* -1, 0, or 1 */
    int nArg = Data[3] % 10;
    int conflict_policy = Data[4] % 5;
    
    /* Test sqlite3_vtab_config - needs to be called within xConnect/xCreate */
    {
        /* Create a virtual table to get into the right context */
        rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE temp.dummy USING dummy", 
                         NULL, NULL, &zErrMsg);
        if (rc != SQLITE_OK && zErrMsg) {
            sqlite3_free(zErrMsg);
            zErrMsg = NULL;
        }
        
        /* Try to call vtab_config - though outside context, undefined behavior */
        sqlite3_vtab_config(db, op, 1);
    }
    
    /* Test sqlite3_create_module_v2 */
    {
        const char *zName = "fuzzmodule";
        void *pClientData = (void*)Data;
        
        rc = sqlite3_create_module_v2(db, zName, &g_module, 
                                      pClientData, dummy_destroy);
        /* Ignore return code - may fail if module already exists */
    }
    
    /* Test sqlite3_vtab_in - needs xBestIndex context */
    {
        sqlite3_index_info idxInfo;
        memset(&idxInfo, 0, sizeof(idxInfo));
        
        /* Set up some dummy constraints */
        struct sqlite3_index_constraint constraints[1];
        struct sqlite3_index_constraint_usage usages[1];
        
        constraints[0].iColumn = -1;
        constraints[0].op = SQLITE_INDEX_CONSTRAINT_EQ;
        constraints[0].usable = 1;
        constraints[0].iTermOffset = 0;
        
        usages[0].argvIndex = 1;
        usages[0].omit = 0;
        
        idxInfo.nConstraint = 1;
        idxInfo.aConstraint = constraints;
        idxInfo.aConstraintUsage = usages;
        
        /* Call vtab_in - though outside xBestIndex, undefined behavior */
        int result = sqlite3_vtab_in(&idxInfo, iCons, bHandle);
        (void)result;  /* Ignore result */
    }
    
    /* Test sqlite3_vtab_on_conflict - needs xUpdate context */
    {
        /* Call vtab_on_conflict - though outside xUpdate, undefined behavior */
        int conflict = sqlite3_vtab_on_conflict(db);
        (void)conflict;  /* Ignore result */
    }
    
    /* Test sqlite3_vtab_rhs_value - needs xBestIndex context */
    {
        sqlite3_index_info idxInfo;
        memset(&idxInfo, 0, sizeof(idxInfo));
        
        struct sqlite3_index_constraint constraints[1];
        struct sqlite3_index_constraint_usage usages[1];
        
        constraints[0].iColumn = -1;
        constraints[0].op = SQLITE_INDEX_CONSTRAINT_EQ;
        constraints[0].usable = 1;
        constraints[0].iTermOffset = 0;
        
        usages[0].argvIndex = 1;
        usages[0].omit = 0;
        
        idxInfo.nConstraint = 1;
        idxInfo.aConstraint = constraints;
        idxInfo.aConstraintUsage = usages;
        
        sqlite3_value *pVal = NULL;
        
        /* Call vtab_rhs_value - though outside xBestIndex, undefined behavior */
        rc = sqlite3_vtab_rhs_value(&idxInfo, 0, &pVal);
        (void)rc;  /* Ignore result */
    }
    
    /* Test sqlite3_overload_function */
    {
        /* Create a dummy function name from input data */
        char funcName[32];
        size_t nameLen = Size > 20 ? 20 : Size;
        if (nameLen > 0) {
            for (size_t i = 0; i < nameLen && i < sizeof(funcName)-1; i++) {
                funcName[i] = 'a' + (Data[i] % 26);
            }
            funcName[nameLen < sizeof(funcName) ? nameLen : sizeof(funcName)-1] = '\0';
            
            rc = sqlite3_overload_function(db, funcName, nArg);
            (void)rc;  /* Ignore result */
        }
    }
    
    /* Create a dummy file if needed */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
        }
        fclose(fp);
    }
    
    /* Cleanup */
    sqlite3_exec(db, "DROP TABLE IF EXISTS temp.dummy", NULL, NULL, NULL);
    sqlite3_close(db);
    
    return 0;
}