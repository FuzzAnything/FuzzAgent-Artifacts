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
//<ID> 231
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 83
//<Quality> {"density":4,"unique_branches":{"findEmphasisClass":[[525,14,525,34,0,0,4,0],[526,7,526,39,0,0,4,1],[529,7,529,54,0,0,4,0],[529,7,529,54,0,0,4,1],[533,7,533,55,0,0,4,1],[537,7,537,55,0,0,4,0],[537,7,537,55,0,0,4,1],[541,7,541,57,0,0,4,0],[541,7,541,57,0,0,4,1],[545,7,545,63,0,0,4,0],[545,7,545,63,0,0,4,1],[549,7,549,62,0,0,4,0],[549,7,549,62,0,0,4,1],[553,7,553,51,0,0,4,1],[557,7,557,51,0,0,4,1]],"back_passDoTest":[[1599,6,1599,34,0,0,4,1],[1603,9,1603,39,0,0,4,0],[1605,7,1605,26,0,0,4,1],[1607,3,1607,18,0,0,4,1],[1611,3,1611,17,0,0,4,1],[1615,3,1615,21,0,0,4,1],[1623,3,1623,16,0,0,4,1],[1627,3,1627,19,0,0,4,1],[1628,3,1628,17,0,0,4,1],[1633,3,1633,25,0,0,4,1],[1637,3,1637,23,0,0,4,1],[1641,3,1641,23,0,0,4,1],[1677,3,1677,17,0,0,4,1],[1681,3,1681,20,0,0,4,1],[1691,3,1691,10,0,0,4,0],[1692,8,1692,72,0,0,4,0],[1695,8,1695,20,0,0,4,0],[1695,24,1695,32,0,0,4,0]],"back_selectRule":[[729,8,729,37,0,0,4,0],[741,10,741,50,0,0,4,1],[742,8,742,50,0,0,4,0],[742,8,742,50,0,0,4,1],[746,6,746,22,0,0,4,0],[747,11,748,63,0,0,4,1],[753,6,753,21,0,0,4,0],[754,6,754,24,0,0,4,0],[777,6,777,25,0,0,4,0],[779,6,779,26,0,0,4,0],[782,11,782,21,0,0,4,0]],"_lou_handlePassVariableTest":[[42,2,42,14,0,0,4,0],[43,7,43,68,0,0,4,0],[47,2,47,14,0,0,4,1],[52,2,52,14,0,0,4,1],[57,2,57,16,0,0,4,1],[62,2,62,16,0,0,4,1],[67,2,67,9,0,0,4,1]],"insertSpace":[[1069,6,1070,39,0,0,4,0],[1072,6,1072,14,0,0,4,0]],"_lou_backTranslate":[[257,6,257,21,0,0,4,0],[258,6,258,22,0,0,4,0],[261,30,261,48,0,0,4,0],[307,8,307,32,0,0,4,0]],"backTranslateString":[[1304,3,1304,22,0,0,4,0],[1306,3,1306,23,0,0,4,0],[1309,3,1309,25,0,0,4,0],[1310,3,1310,25,0,0,4,0],[1313,8,1313,22,0,0,4,0],[1316,5,1316,24,0,0,4,0],[1316,5,1316,24,0,0,4,1],[1320,5,1320,25,0,0,4,1],[1324,5,1324,25,0,0,4,0],[1324,5,1324,25,0,0,4,1],[1330,10,1330,42,0,0,4,1],[1332,17,1332,51,0,0,4,0],[1336,5,1336,27,0,0,4,0],[1336,5,1336,27,0,0,4,1],[1340,5,1340,27,0,0,4,0],[1340,5,1340,27,0,0,4,1],[1344,5,1344,21,0,0,4,1],[1348,5,1348,21,0,0,4,1],[1358,5,1358,12,0,0,4,1],[1362,11,1362,31,0,0,4,0],[1362,11,1362,31,0,0,4,1],[1429,8,1430,42,0,0,4,0],[1447,7,1447,22,0,0,4,0],[1447,26,1447,59,0,0,4,0],[1452,31,1452,49,0,0,4,0],[1452,31,1452,49,0,0,4,1]]},"library_calls":["lou_getDataPath","lou_readCharFromFile","lou_findTable","lou_getTable","lou_freeTableFile","lou_charToDots","lou_dotsToChar","lou_backTranslateString","lou_freeTableFile"],"critical_calls":["lou_getDataPath","lou_readCharFromFile","lou_findTable","lou_getTable","lou_charToDots","lou_dotsToChar","lou_backTranslateString","lou_freeTableFile"],"visited":1}
/**/


// Include necessary headers for liblouis
extern "C" {
    // These are the liblouis APIs we'll use
    char *lou_getDataPath();
    int lou_dotsToChar(const char *tableList, widechar *inbuf, widechar *outbuf, int length, int mode);
    const void *lou_getTable(const char *tableList);
    int lou_readCharFromFile(const char *fileName, int *mode);
    char *lou_findTable(const char *query);
    int lou_charToDots(const char *tableList, const widechar *inbuf, widechar *outbuf, int length, int mode);
    int lou_backTranslateString(const char *tableList, const widechar *inbuf, int *inlen, widechar *outbuf, int *outlen, formtype *typeform, char *spacing, int mode);
    
    // We'll also need lou_freeTableFile to free resources
    void lou_freeTableFile(char *);
}

// Custom types from liblouis
typedef unsigned short widechar;
typedef unsigned short formtype;

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 2) {
        return 0;  // Need at least some data
    }

    // 1. Get data path
    char *dataPath = lou_getDataPath();
    if (dataPath) {
        // Could use dataPath for something, but we'll just note it's available
        // In a real fuzzer, we might use it to set up test data
    }

    // 2. Create a FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Create an output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 4. Use lou_readCharFromFile with our input file
    // First, write the data to a file that lou_readCharFromFile can read
    FILE *temp_file = fopen("input_file", "wb");
    if (temp_file) {
        fwrite(data, 1, size, temp_file);
        assert_file_closed(&temp_file);;
        
        int mode = 0;
        int readChar = lou_readCharFromFile("input_file", &mode);
        // We don't use the result, but we called the API as requested
    }

    // 5. Find a table using part of the input data as query
    // Use first min(100, size) bytes as query string
    size_t query_len = size > 100 ? 100 : size;
    char *query = (char *)malloc(query_len + 1);
    if (!query) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("input_file");
        remove("output_file");
        assert_file_closed(&temp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    memcpy(query, data, query_len);
    query[query_len] = '\0';
    
    char *tableName = lou_findTable(query);
    free(query);
    
    // Use a default table if query didn't find one
    const char *tableList = tableName ? tableName : "en-us-g2.ctb";

    // 6. Get the table (just to call the API, we don't use the result)
    const void *table = lou_getTable(tableList);

    // 7. Prepare input buffer for braille operations
    // Convert input data to widechar array for braille operations
    // We'll use up to size/2 widechars (since widechar is 2 bytes)
    int max_widechars = size / sizeof(widechar);
    if (max_widechars < 1) {
        max_widechars = 1;
    }
    
    widechar *inbuf = (widechar *)malloc(max_widechars * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(max_widechars * sizeof(widechar));
    if (!inbuf || !outbuf) {
        free(inbuf);
        free(outbuf);
        if (tableName) lou_freeTableFile(tableName);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("input_file");
        remove("output_file");
        assert_file_closed(&temp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Fill input buffer with data (convert bytes to widechars)
    int inlen = 0;
    for (size_t i = 0; i < size && inlen < max_widechars; i += sizeof(widechar)) {
        if (i + 1 < size) {
            inbuf[inlen] = (data[i] << 8) | data[i + 1];
        } else {
            inbuf[inlen] = data[i];
        }
        inlen++;
    }

    // 8. Call lou_charToDots (convert characters to braille dots)
    int dotsResult = lou_charToDots(tableList, inbuf, outbuf, inlen, fuzz_int32_t_1);
    
    // 9. Call lou_dotsToChar (convert braille dots back to characters)
    widechar *reconverted = (widechar *)malloc(max_widechars * sizeof(widechar));
    if (reconverted) {
        int dotsToCharResult = lou_dotsToChar(tableList, outbuf, reconverted, inlen, fuzz_int32_t_2);
        free(reconverted);
    }

    // 10. Call lou_backTranslateString
    // Prepare buffers and parameters for back translation
    int back_inlen = inlen;
    int back_outlen = max_widechars;
    widechar *back_outbuf = (widechar *)malloc(back_outlen * sizeof(widechar));
    formtype *typeform = (formtype *)calloc(inlen, sizeof(formtype));
    char *spacing = (char *)calloc(inlen, sizeof(char));
    
    if (back_outbuf && typeform && spacing) {
        int backTranslateResult = lou_backTranslateString(
            tableList, inbuf, &back_inlen, back_outbuf, 
            &back_outlen, typeform, spacing, fuzz_int32_t_3
        );
    }
    
    // Clean up
    free(back_outbuf);
    free(typeform);
    free(spacing);
    free(inbuf);
    free(outbuf);
    
    if (tableName) {
        lou_freeTableFile(tableName);
    }
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&temp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}