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
#include <liblouis/liblouis.h>
}
//<ID> 398
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["lou_logPrint","lou_getDataPath","lou_logPrint","lou_listTables","lou_logPrint","lou_logPrint","lou_getTable","lou_logPrint","lou_freeTableFiles","lou_freeTableFiles","lou_freeTableFiles","lou_freeTableFiles","lou_translateString","lou_logPrint","lou_charToDots","lou_logPrint","lou_dotsToChar","lou_logPrint","lou_translatePrehyphenated","lou_logPrint","lou_freeTableFiles","lou_free"],"critical_calls":["lou_logPrint","lou_getDataPath","lou_logPrint","lou_listTables","lou_logPrint","lou_logPrint","lou_getTable","lou_translateString","lou_logPrint","lou_charToDots","lou_logPrint","lou_dotsToChar","lou_logPrint","lou_translatePrehyphenated","lou_logPrint","lou_freeTableFiles","lou_free"],"visited":1}
/**/


// Include liblouis headers

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small for meaningful processing
    if (size < 2) {
	return 0;
	}
	
    
    // 1. Use lou_logPrint to log version info
    lou_logPrint("Starting liblouis fuzzing test\n");
    
    // 2. Get and log data path
    char *dataPath = lou_getDataPath();
    if (dataPath) {
        lou_logPrint("Data path: %s\n", dataPath);
    }
    
    // 3. List available tables
    char **tables = lou_listTables();
    if (tables) {
        lou_logPrint("Available tables:\n");
        for (int i = 0; tables[i] != nullptr; i++) {
            lou_logPrint("  %s\n", tables[i]);
        }
    }
    
    // 4. Try to use a table (use first available if exists, otherwise default)
    const char *tableList = nullptr;
    if (tables && tables[0]) {
        tableList = tables[0];
    } else {
        tableList = "en-us-g2.ctb";  // Default table
    }
    
    // 5. Get table handle
    const void *tableHandle = lou_getTable(tableList);
    if (!tableHandle) {
        lou_logPrint("Failed to get table handle for %s\n", tableList);
        if (tables) lou_freeTableFiles(tables);
        if (dataPath) free(dataPath);
        return 0;
    }
    
    // 6. Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        if (tables) lou_freeTableFiles(tables);
        if (dataPath) free(dataPath);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Prepare input buffer (treat data as widechar array)
    // Each widechar is 2 bytes, so divide by 2
    int inlen = size / 2;
    if (inlen <= 0) {
        assert_file_closed(&in_file);;
        if (tables) lou_freeTableFiles(tables);
        if (dataPath) free(dataPath);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Allocate and copy input data as widechar
    widechar *inbuf = (widechar *)malloc(size);
    if (!inbuf) {
        assert_file_closed(&in_file);;
        if (tables) lou_freeTableFiles(tables);
        if (dataPath) free(dataPath);
        assert_file_closed(&in_file);
	return 0;
    }
    memcpy(inbuf, data, size);
    
    // 8. Test lou_translateString
    int outlen1 = inlen * 2;  // Conservative estimate
    widechar *outbuf1 = (widechar *)malloc(outlen1 * sizeof(widechar));
    if (outbuf1) {
        int actual_inlen1 = inlen;
        int actual_outlen1 = outlen1;
        formtype *typeform1 = nullptr;
        char *spacing1 = nullptr;
        
        int result1 = lou_translateString(
            tableList, 
            inbuf, 
            &actual_inlen1, 
            outbuf1, 
            &actual_outlen1, 
            typeform1, 
            spacing1, 
            0
        );
        lou_logPrint("translateString result: %d\n", result1);
        free(outbuf1);
    }
    
    // 9. Test lou_charToDots
    widechar *dotsbuf = (widechar *)malloc(inlen * sizeof(widechar));
    if (dotsbuf) {
        int result2 = lou_charToDots(
            tableList,
            inbuf,
            dotsbuf,
            inlen,
            0
        );
        lou_logPrint("charToDots result: %d\n", result2);
        
        // 10. Test lou_dotsToChar (using the dots we just created)
        widechar *backbuf = (widechar *)malloc(inlen * sizeof(widechar));
        if (backbuf) {
            int result3 = lou_dotsToChar(
                tableList,
                dotsbuf,
                backbuf,
                inlen,
                0
            );
            lou_logPrint("dotsToChar result: %d\n", result3);
            free(backbuf);
        }
        free(dotsbuf);
    }
    
    // 11. Test lou_translatePrehyphenated (if we have enough data)
    if (inlen > 0) {
        int outlen4 = inlen * 2;
        widechar *outbuf4 = (widechar *)malloc(outlen4 * sizeof(widechar));
        if (outbuf4) {
            int actual_inlen4 = inlen;
            int actual_outlen4 = outlen4;
            formtype *typeform4 = nullptr;
            char *spacing4 = nullptr;
            int *outputPos4 = nullptr;
            int *inputPos4 = nullptr;
            int *cursorPos4 = nullptr;
            
            // Allocate hyphen arrays (1 byte per input character)
            char *inputHyphens = (char *)malloc(inlen);
            char *outputHyphens = (char *)malloc(outlen4);
            
            if (inputHyphens && outputHyphens) {
                // Initialize hyphens (simple pattern)
                for (int i = 0; i < inlen; i++) {
                    inputHyphens[i] = (i % 3 == 0) ? '1' : '0';
                }
                
                int result4 = lou_translatePrehyphenated(
                    tableList,
                    inbuf,
                    &actual_inlen4,
                    outbuf4,
                    &actual_outlen4,
                    typeform4,
                    spacing4,
                    outputPos4,
                    inputPos4,
                    cursorPos4,
                    inputHyphens,
                    outputHyphens,
                    0
                );
                lou_logPrint("translatePrehyphenated result: %d\n", result4);
            }
            
            if (inputHyphens) free(inputHyphens);
            if (outputHyphens) free(outputHyphens);
            free(outbuf4);
        }
    }
    
    // 12. Cleanup
    assert_file_closed(&in_file);;
    free(inbuf);
    if (tables) lou_freeTableFiles(tables);
    if (dataPath) free(dataPath);
    
    // 13. Call lou_free to cleanup library resources
    lou_free();
    
    assert_file_closed(&in_file);
	return 0;
}