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
//<ID> 846
//<Prompt> []
/*<Combination>: [
*/
//<score> 57, nr_unique_branch: 86
//<Quality> {"density":6,"unique_branches":{"getRuleCharsText":[[1551,6,1551,51,0,0,4,0]],"compileBrailleIndicator":[[2332,6,2332,36,0,0,4,0],[2333,6,2333,38,0,0,4,0]],"compileSwap":[[1660,6,1660,44,0,0,4,0],[1661,6,1661,50,0,0,4,0],[1662,6,1662,60,0,0,4,0],[1666,7,1666,51,0,0,4,0],[1676,6,1676,51,0,0,4,0]],"compileBeforeAfter":[[2689,6,2689,58,0,0,4,0],[2693,11,2693,60,0,0,4,1]],"lou_dotsToChar":[[4163,7,4163,25,0,0,4,0],[4164,5,4164,39,0,0,4,0],[4164,5,4164,39,0,0,4,1]],"parseCharsInternal":[[1336,5,1336,12,0,0,4,0],[1354,33,1354,45,0,0,4,1],[1358,31,1358,50,0,0,4,0],[1364,8,1364,30,0,0,4,0],[1364,34,1364,61,0,0,4,0]],"_lou_backTranslate":[[222,6,222,23,0,0,4,1]],"compileCharDef":[[2652,6,2652,42,0,0,4,0],[2653,6,2653,27,0,0,4,0]],"parseDots":[[1454,4,1454,12,0,0,4,0],[1478,9,1478,19,0,0,4,0],[1497,3,1497,10,0,0,4,0]],"compileRule":[[3068,3,3068,18,0,0,4,0],[3077,8,3078,56,0,0,4,0],[3206,3,3206,23,0,0,4,0],[3207,3,3207,19,0,0,4,0],[3228,9,3228,50,0,0,4,0],[3236,10,3236,25,0,0,4,0],[3270,9,3275,29,0,0,4,0],[3314,5,3314,12,0,0,4,0],[3324,9,3326,21,0,0,4,0],[3325,8,3325,14,0,0,4,1],[3346,9,3348,21,0,0,4,0],[3368,9,3370,29,0,0,4,0],[3487,8,3487,49,0,0,4,0],[3488,8,3488,45,0,0,4,0],[3493,40,3493,67,0,0,4,1],[3495,33,3495,61,0,0,4,0],[3699,8,3699,43,0,0,4,0],[3731,8,3732,41,0,0,4,0],[3739,8,3739,43,0,0,4,0],[3766,8,3766,43,0,0,4,0],[3782,8,3783,41,0,0,4,0],[3789,8,3789,43,0,0,4,0],[3817,9,3817,11,0,0,4,0],[3837,8,3837,43,0,0,4,0],[3865,8,3866,41,0,0,4,0],[3875,8,3876,41,0,0,4,0],[3912,8,3912,43,0,0,4,0],[3913,8,3913,44,0,0,4,0],[3977,8,3977,43,0,0,4,0],[3978,8,3978,29,0,0,4,0],[3985,8,3985,44,0,0,4,0],[3991,3,3991,21,0,0,4,0],[3992,8,3992,43,0,0,4,1],[3993,8,3993,33,0,0,4,0],[4045,19,4045,25,0,0,4,1],[4055,8,4055,43,0,0,4,0],[4061,11,4061,12,0,0,4,1],[4212,9,4212,54,0,0,4,0],[4223,8,4223,49,0,0,4,0],[4232,8,4232,13,0,0,4,0],[4234,9,4234,14,0,0,4,1],[4236,10,4236,42,0,0,4,1],[4236,46,4236,74,0,0,4,1],[4237,6,4237,34,0,0,4,0],[4237,38,4237,69,0,0,4,1],[4244,8,4244,25,0,0,4,0],[4302,8,4302,43,0,0,4,0]],"compileNumber":[[2342,6,2342,39,0,0,4,0],[2344,6,2344,19,0,0,4,0]],"getRuleDotsPattern":[[1566,6,1566,45,0,0,4,0]],"getMatchPatternCharacters":[[1544,6,1544,43,0,0,4,0]],"getNumber":[[1685,29,1685,45,0,0,4,1]],"addRuleName":[[1603,20,1603,28,0,0,4,1],[1603,34,1603,42,0,0,4,1],[1603,46,1603,54,0,0,4,1]],"addCharacterClass":[[1112,29,1112,43,0,0,4,1],[1113,8,1113,22,0,0,4,1],[1113,26,1113,40,0,0,4,1],[1124,17,1124,27,0,0,4,1],[1126,9,1126,20,0,0,4,0]],"compileSwapDots":[[1640,8,1640,48,0,0,4,0]],"getToken":[[383,7,383,18,0,0,4,0]]},"library_calls":["lou_compileString","lou_getTable","lou_free","lou_dotsToChar","lou_backTranslateString","lou_backTranslate","lou_backTranslateString","lou_free"],"critical_calls":["lou_compileString","lou_getTable","lou_dotsToChar","lou_backTranslateString","lou_backTranslate","lou_backTranslateString","lou_free"],"visited":1}
/**/


// Include liblouis headers (simplified for the fuzz driver)
extern "C" {
    typedef unsigned short widechar;
    typedef unsigned short formtype;
    
    // Function prototypes
    int lou_dotsToChar(const char *tableList, widechar *inbuf, widechar *outbuf, int length, int mode);
    int lou_backTranslate(const char *tableList, const widechar *inbuf, int *inlen, widechar *outbuf, int *outlen, formtype *typeform, char *spacing, int *outputPos, int *inputPos, int *cursorPos, int mode);
    int lou_backTranslateString(const char *tableList, const widechar *inbuf, int *inlen, widechar *outbuf, int *outlen, formtype *typeform, char *spacing, int mode);
    const void *lou_getTable(const char *tableList);
    int lou_compileString(const char *tableList, const char *inString);
    
    // Additional needed function
    void lou_free();
}

// Fuzzer entry point
extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<91) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    // Early exit if insufficient data
    if (size < 2) {
	return 0;
	}
	
    
    // Use a fixed braille table for consistency
    const char *tableList = "en-us-g2.ctb";
    
    // 1. First, try to compile a table string from input data
    // We'll use a portion of the input as a table string
    size_t tableStringSize = size > 100 ? 100 : size;
    std::vector<char> tableString(tableStringSize + 1);
    memcpy(tableString.data(), data, tableStringSize);
    tableString[tableStringSize] = '\0';
    
    // Call lou_compileString (may fail, but that's OK for fuzzing)
    int compileResult = lou_compileString(tableList, tableString.data());
    // Result ignored - compilation may fail with arbitrary input
    
    // 2. Get the table handle
    const void *tableHandle = lou_getTable(fuzz_str_1);
    // We don't need to use the handle directly, but we've called the API
    
    // 3. Prepare input buffer for braille dots (widechar array)
    // We'll interpret the remaining data as widechars
    size_t widecharCount = (size - tableStringSize) / sizeof(widechar);
    if (widecharCount < 1) {
        lou_free();
        return 0;
    }
    
    std::vector<widechar> inbuf(widecharCount);
    memcpy(inbuf.data(), data + tableStringSize, widecharCount * sizeof(widechar));
    
    // 4. Call lou_dotsToChar to convert dots to characters
    std::vector<widechar> outbuf_dtc(widecharCount);
    int dtcResult = lou_dotsToChar(fuzz_str_2, inbuf.data(), outbuf_dtc.data(), 
                                   static_cast<int>(widecharCount), fuzz_int32_t_3);
    
    // 5. Prepare for back translation
    int inlen = static_cast<int>(widecharCount);
    int outlen = inlen * 2; // Conservative estimate for output buffer
    std::vector<widechar> outbuf_bt(outlen);
    std::vector<widechar> outbuf_bts(outlen);
    
    // Additional parameters
    formtype *typeform = nullptr;
    char *spacing = nullptr;
    int *outputPos = nullptr;
    int *inputPos = nullptr;
    int *cursorPos = nullptr;
    
    // 6. Call lou_backTranslateString
    int btsResult = lou_backTranslateString(fuzz_str_4, inbuf.data(), &inlen, 
                                            outbuf_bts.data(), &outlen, 
                                            typeform, spacing, fuzz_int32_t_5);
    
    // Reset inlen for next call
    inlen = static_cast<int>(widecharCount);
    outlen = inlen * 2;
    
    // 7. Call lou_backTranslate with more parameters
    int btResult = lou_backTranslate(fuzz_str_6, inbuf.data(), &inlen, 
                                     outbuf_bt.data(), &outlen, 
                                     typeform, spacing, 
                                     outputPos, inputPos, cursorPos, fuzz_int32_t_7);
    
    // 8. Also try using the output from dotsToChar as input to backTranslate
    if (dtcResult > 0) {
        int dtc_inlen = dtcResult; // lou_dotsToChar returns number of characters
        int dtc_outlen = dtc_inlen * 2;
        std::vector<widechar> dtc_outbuf(dtc_outlen);
        
        lou_backTranslateString(fuzz_str_8, outbuf_dtc.data(), &dtc_inlen,
                                dtc_outbuf.data(), &dtc_outlen,
                                typeform, spacing, fuzz_int32_t_9);
    }
    
    // 9. Clean up
    lou_free();
    
    return 0;
}