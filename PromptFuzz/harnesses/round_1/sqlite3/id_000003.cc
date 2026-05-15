#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <sqlite3.h>
}
//<ID> 29
//<Prompt> []
/*<Combination>: [
*/
//<score> 308, nr_unique_branch: 158
//<Quality> {"density":14,"unique_branches":{"substExpr":[[145142,7,145142,27,0,0,4,1],[145214,9,145214,34,0,0,4,1],[18935,29,18935,59,20,0,4,1],[18919,34,18919,55,21,0,4,1]],"sqlite3ColumnsFromExprList":[[143522,37,143522,57,0,0,4,0]],"keywordCode":[[14098,27,14098,28,59,0,4,0]],"resolveSelectStep":[[107569,13,107569,21,0,0,4,0]],"generateOutputSubroutine":[[144615,5,144615,17,0,0,4,0],[144653,7,144653,16,0,0,4,0],[14098,27,14098,28,17,0,4,1]],"codeDistinct":[[142230,5,142230,31,0,0,4,0]],"multiSelectOrderBy":[[144906,7,144906,16,0,0,4,0],[144906,20,144906,30,0,0,4,1]],"sqlite3CreateForeignKey":[[123797,16,123797,25,0,0,4,1],[123803,11,123803,21,0,0,4,0]],"sqlite3CreateIndex":[[124110,9,124110,14,0,0,4,0]],"sqlite3_is_interrupted":[[179446,7,179446,32,0,0,4,1]],"fromClauseTermCanBeCoroutine":[[148490,7,148490,22,0,0,4,0],[148492,9,148492,33,0,0,4,1],[148493,9,148493,25,0,0,4,1]],"disableUnusedSubqueryResultColumns":[[146592,33,146592,48,0,0,4,0]],"valueFromExpr":[[83640,12,83640,23,0,0,4,1],[83659,12,83659,28,0,0,4,1]],"searchWith":[[146892,16,146892,17,0,0,4,0],[146892,16,146892,17,0,0,4,1],[146894,14,146894,23,0,0,4,0],[146894,14,146894,23,0,0,4,1],[146895,11,146895,50,0,0,4,0],[146895,11,146895,50,0,0,4,1],[146900,9,146900,17,0,0,4,1]],"wherePathSolver":[[165703,7,165703,49,0,0,4,0],[165704,7,165704,45,0,0,4,1]],"exprCodeBetween":[[14098,27,14098,28,5,0,4,0],[14098,27,14098,28,6,0,4,0],[14098,27,14098,28,7,0,4,0],[14098,27,14098,28,7,0,4,0],[14098,27,14098,28,8,0,4,0],[14098,27,14098,28,8,0,4,0],[14098,27,14098,28,8,0,4,0]],"lookupName":[[106152,17,106152,55,0,0,4,0]],"sqlite3Pragma":[[139205,17,139205,28,0,0,4,0],[139262,13,139262,25,0,0,4,0],[139262,29,139262,65,0,0,4,0],[139264,15,139264,34,0,0,4,0],[139270,36,139270,39,0,0,4,1],[139353,32,139353,56,0,0,4,0]],"parseDateOrTime":[[24406,13,24406,43,0,0,4,0],[24406,47,24406,74,0,0,4,0]],"whereCombineDisjuncts":[[158968,7,158968,71,0,0,4,1],[158969,7,158969,72,0,0,4,0]],"computeLimitRegisters":[[143785,25,143785,60,0,0,4,1]],"sqlite3ExprIfFalse":[[14098,27,14098,28,33,0,4,0]],"sqlite3DeferForeignKey":[[123870,7,123870,37,0,0,4,0]],"findRightmost":[[141487,10,141487,18,0,0,4,0]],"sqlite3Select":[[148737,9,148737,24,0,0,4,0],[148737,28,148737,62,0,0,4,1],[148927,10,148927,28,0,0,4,1],[148928,14,148928,48,0,0,4,0],[148928,52,148928,77,0,0,4,0]],"resolveFromTermToCte":[[146962,7,146962,23,0,0,4,1],[146966,7,146966,19,0,0,4,1],[146971,7,146971,26,0,0,4,1],[146976,7,146976,23,0,0,4,1],[146985,7,146985,11,0,0,4,0],[146985,7,146985,11,0,0,4,1],[147001,9,147001,22,0,0,4,1],[147005,9,147005,40,0,0,4,1],[147009,9,147009,16,0,0,4,1],[147011,9,147011,19,0,0,4,0],[147013,11,147013,21,0,0,4,1],[147014,11,147014,67,0,0,4,1],[147028,9,147028,25,0,0,4,1],[147031,9,147031,30,0,0,4,1],[147041,23,147041,39,0,0,4,1],[147041,43,147041,61,0,0,4,0],[147041,43,147041,61,0,0,4,1],[147042,12,147042,25,0,0,4,0],[147042,12,147042,25,0,0,4,1],[147042,29,147042,51,0,0,4,0],[147046,16,147046,28,0,0,4,1],[147066,11,147066,49,0,0,4,0],[147073,9,147073,38,0,0,4,1],[147088,11,147088,43,0,0,4,0],[147088,11,147088,43,0,0,4,1],[147095,21,147095,34,0,0,4,0],[147095,21,147095,34,0,0,4,1],[147097,9,147097,20,0,0,4,0],[147098,11,147098,17,0,0,4,0],[147098,21,147098,54,0,0,4,1],[147109,9,147109,22,0,0,4,0],[147109,9,147109,22,0,0,4,1],[147110,11,147110,40,0,0,4,1]],"fixDistinctOpenEph":[[142281,8,142281,40,0,0,4,0],[142288,9,142288,42,0,0,4,1]],"sqlite3_str_vappendf":[[31577,15,31577,45,0,0,4,0]],"sqlite3WithAdd":[[125793,16,125793,21,0,0,4,0],[125795,14,125795,27,0,0,4,0],[125795,14,125795,27,0,0,4,1],[125796,11,125796,54,0,0,4,1],[125802,7,125802,12,0,0,4,0]],"sqlite3RowidAlias":[[110748,13,110748,25,0,0,4,0],[110750,17,110750,32,0,0,4,0],[110750,17,110750,32,0,0,4,1],[110751,11,110751,66,0,0,4,1],[110753,9,110753,25,0,0,4,0]],"yy_reduce":[[174658,7,174658,15,0,0,4,0],[175292,7,175292,15,0,0,4,0],[175315,7,175315,15,0,0,4,0]],"sqlite3ColumnDefault":[[151608,7,151608,18,0,0,4,0],[151617,9,151617,15,0,0,4,1]],"sqlite3WhereExplainOneScan":[[155793,16,155793,66,0,0,4,0]],"sqlite3AlterDropColumn":[[117118,7,117118,48,0,0,4,0]],"parserDoubleLinkSelect":[[170508,13,170508,28,0,0,4,0],[170510,14,170510,32,0,0,4,0]],"selectExpander":[[147289,15,147289,69,0,0,4,0],[147290,11,147290,15,0,0,4,0],[147290,11,147290,15,0,0,4,1],[147474,54,147474,78,0,0,4,0],[147479,17,147479,30,0,0,4,0],[147481,19,147481,27,0,0,4,1],[147517,37,147517,66,0,0,4,1],[147555,17,147555,46,0,0,4,0],[147555,50,147555,67,0,0,4,0],[147556,19,147556,30,0,0,4,1],[147564,32,147564,45,0,0,4,0],[147564,32,147564,45,0,0,4,1],[147565,20,147565,37,0,0,4,1],[147567,20,147567,26,0,0,4,1],[147568,20,147568,32,0,0,4,0],[147568,20,147568,32,0,0,4,1],[147568,36,147568,79,0,0,4,1],[14098,27,14098,28,55,0,4,1]],"sqlite3ExprIfTrue":[[113478,5,113478,16,0,0,4,0],[113483,17,113483,27,0,0,4,0],[113527,5,113527,15,0,0,4,0],[113528,5,113528,15,0,0,4,0],[113564,5,113564,20,0,0,4,0],[14098,27,14098,28,6,0,4,1],[14098,27,14098,28,8,0,4,1],[14098,27,14098,28,32,0,4,1],[14098,27,14098,28,35,0,4,0],[14098,27,14098,28,36,0,4,0],[14098,27,14098,28,40,0,4,0],[14098,27,14098,28,41,0,4,1],[14098,27,14098,28,48,0,4,1]],"sqlite3SelectPopWith":[[147136,43,147136,55,0,0,4,1],[147138,9,147138,17,0,0,4,1]],"sqlite3WhereBegin":[[166486,15,166486,26,0,0,4,1],[166714,11,166714,66,0,0,4,1]],"sqlite3AddPrimaryKey":[[121975,7,121975,40,0,0,4,0],[122021,13,122021,20,0,0,4,0]],"flattenSubquery":[[145709,7,145709,14,0,0,4,0],[145709,18,145709,59,0,0,4,1],[145928,9,145928,18,0,0,4,0],[145930,11,145930,18,0,0,4,1]],"sqlite3ExprCompare":[[113880,31,113880,78,0,0,4,0],[113917,9,113917,30,0,0,4,1]],"analyzeFilterKeyword":[[176594,7,176594,23,0,0,4,1]]},"library_calls":["sqlite3_create_filename","sqlite3_open_v2","sqlite3_free_filename","sqlite3_rollback_hook","sqlite3_prepare_v2","sqlite3_step","sqlite3_is_interrupted","sqlite3_total_changes","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_close","sqlite3_free_filename"],"critical_calls":["sqlite3_create_filename","sqlite3_open_v2","sqlite3_rollback_hook","sqlite3_prepare_v2","sqlite3_step","sqlite3_is_interrupted","sqlite3_total_changes","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_close","sqlite3_free_filename"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create temporary files for database operations
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create database filename using sqlite3_create_filename
    const char* db_name = "fuzz.db";
    const char* journal_name = "fuzz.db-journal";
    const char* wal_name = "fuzz.db-wal";
    int nParam = 0;
    const char** azParam = nullptr;
    
    sqlite3_filename filename = sqlite3_create_filename(db_name, journal_name, wal_name, nParam, azParam);
    if (!filename) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Open database using the created filename
    sqlite3* db = nullptr;
    int rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, nullptr);
    if (rc != SQLITE_OK || !db) {
        sqlite3_free_filename(filename);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set rollback hook
    void* user_data = nullptr;
    void (*rollback_callback)(void*) = [](void*) {
        // Empty callback for demonstration
    };
    sqlite3_rollback_hook(db, rollback_callback, user_data);

    // Read SQL commands from input data and execute them
    char sql_buffer[1024];
    size_t total_read = 0;
    
    while (total_read < size) {
        // Read a chunk of data as potential SQL command
        size_t to_read = (size - total_read > sizeof(sql_buffer) - 1) ? 
                         sizeof(sql_buffer) - 1 : size - total_read;
        size_t read_count = fread(sql_buffer, 1, to_read, in_file);
        if (read_count == 0) {
            break;
        }
        
        sql_buffer[read_count] = '\0';
        total_read += read_count;

        // Try to execute as SQL statement
        sqlite3_stmt* stmt = nullptr;
        const char* tail = nullptr;
        
        rc = sqlite3_prepare_v2(db, sql_buffer, -1, &stmt, &tail);
        if (rc == SQLITE_OK && stmt) {
            // Execute the statement
            int step_result = sqlite3_step(stmt);
            
            // Check if database was interrupted
            int is_interrupted = sqlite3_is_interrupted(db);
            (void)is_interrupted; // Suppress unused variable warning
            
            // Check total changes
            int total_changes = sqlite3_total_changes(db);
            (void)total_changes; // Suppress unused variable warning
            
            sqlite3_finalize(stmt);
        }
    }

    // Create a simple statement to ensure sqlite3_step is called
    sqlite3_stmt* simple_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, "SELECT 1;", -1, &simple_stmt, nullptr);
    if (rc == SQLITE_OK && simple_stmt) {
        sqlite3_step(simple_stmt);
        sqlite3_finalize(simple_stmt);
    }

    // Cleanup
    sqlite3_close(db);
    sqlite3_free_filename(filename);
    assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove(db_name);
    remove(journal_name);
    remove(wal_name);

    assert_file_closed(&in_file);
	return 0;
}