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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["lou_compileString","lou_checkTable","lou_charSize","lou_getTableInfo","lou_freeTableInfo","lou_translate","lou_backTranslate","lou_free"],"critical_calls":["lou_compileString","lou_checkTable","lou_charSize","lou_getTableInfo","lou_freeTableInfo","lou_translate","lou_backTranslate","lou_free"],"visited":0}
/**/


extern "C" {
}

// Custom types from liblouis
typedef unsigned short formtype;
typedef unsigned short widechar;

// Event: Process braille translation data by compiling a table from input,
// checking it, getting table info, translating and back-translating text
extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 2) {
	return 0;
	}
	
    
    // 1. Create null-terminated table string from input
    char *tableString = (char *)malloc(size + 1);
    if (!tableString) {
	return 0;
	}
	
    memcpy(tableString, data, size);
    tableString[size] = '\0';
    
    // 2. Use input as table source and compile it
    const char *tableList = "testTable";
    int compileResult = lou_compileString(tableList, tableString);
    free(tableString);
    
    // 3. Check the compiled table
    int checkResult = lou_checkTable(tableList);
    
    // 4. Get character size
    int charSize = lou_charSize();
    
    // 5. Get table information
    char *tableInfo = lou_getTableInfo(tableList, "locale");
    if (tableInfo) {
        // Free the table info
        lou_freeTableInfo(tableInfo);
    }
    
    // 6. Prepare test text for translation
    const char *testText = "Test input for translation";
    int inlen = strlen(testText);
    
    // Convert to widechar array
    widechar *inbuf = (widechar *)malloc((inlen + 1) * sizeof(widechar));
    if (!inbuf) {
	return 0;
	}
	
    
    for (int i = 0; i < inlen; i++) {
        inbuf[i] = (widechar)testText[i];
    }
    inbuf[inlen] = 0;
    
    // Allocate output buffers
    int outlen = 256;  // Reasonable initial size
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    int backOutlen = 256;
    widechar *backOutbuf = (widechar *)malloc(backOutlen * sizeof(widechar));
    
    // Allocate arrays for position tracking
    int *outputPos = (int *)malloc(inlen * sizeof(int));
    int *inputPos = (int *)malloc(outlen * sizeof(int));
    int *cursorPos = (int *)malloc(sizeof(int));
    
    int *backOutputPos = (int *)malloc(outlen * sizeof(int));
    int *backInputPos = (int *)malloc(backOutlen * sizeof(int));
    int *backCursorPos = (int *)malloc(sizeof(int));
    
    // Allocate spacing arrays
    char *spacing = (char *)malloc(outlen * sizeof(char));
    char *backSpacing = (char *)malloc(backOutlen * sizeof(char));
    
    // Allocate typeform
    formtype *typeform = (formtype *)malloc(inlen * sizeof(formtype));
    formtype *backTypeform = (formtype *)malloc(outlen * sizeof(formtype));
    
    // Initialize arrays
    if (outputPos) memset(outputPos, 0, inlen * sizeof(int));
    if (inputPos) memset(inputPos, 0, outlen * sizeof(int));
    if (cursorPos) *cursorPos = 0;
    
    if (backOutputPos) memset(backOutputPos, 0, outlen * sizeof(int));
    if (backInputPos) memset(backInputPos, 0, backOutlen * sizeof(int));
    if (backCursorPos) *backCursorPos = 0;
    
    if (spacing) memset(spacing, 0, outlen * sizeof(char));
    if (backSpacing) memset(backSpacing, 0, backOutlen * sizeof(char));
    if (typeform) memset(typeform, 0, inlen * sizeof(formtype));
    if (backTypeform) memset(backTypeform, 0, outlen * sizeof(formtype));
    
    // 7. Perform forward translation
    int translateResult = 0;
    if (outbuf && spacing && typeform && outputPos && inputPos && cursorPos) {
        translateResult = lou_translate(
            tableList,           // tableList
            inbuf,               // inbuf
            &inlen,              // inlen
            outbuf,              // outbuf
            &outlen,             // outlen
            typeform,            // typeform
            spacing,             // spacing
            outputPos,           // outputPos
            inputPos,            // inputPos
            cursorPos,           // cursorPos
            0                    // mode
        );
    }
    
    // 8. Perform back translation if forward translation succeeded
    int backTranslateResult = 0;
    if (translateResult > 0 && backOutbuf && backSpacing && backTypeform && 
        backOutputPos && backInputPos && backCursorPos) {
        // Note: outlen now contains actual output length from forward translation
        int backInlen = outlen;  // Use output from forward translation as input for back translation
        
        backTranslateResult = lou_backTranslate(
            tableList,           // tableList
            outbuf,              // inbuf (output from forward translation)
            &backInlen,          // inlen
            backOutbuf,          // outbuf
            &backOutlen,         // outlen
            backTypeform,        // typeform
            backSpacing,         // spacing
            backOutputPos,       // outputPos
            backInputPos,        // inputPos
            backCursorPos,       // cursorPos
            0                    // mode
        );
    }
    
    // 9. Clean up all allocated resources
    free(inbuf);
    free(outbuf);
    free(backOutbuf);
    free(outputPos);
    free(inputPos);
    free(cursorPos);
    free(backOutputPos);
    free(backInputPos);
    free(backCursorPos);
    free(spacing);
    free(backSpacing);
    free(typeform);
    free(backTypeform);
    
    // 10. Free liblouis resources
    lou_free();
    
    return 0;
}