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
//<ID> 781
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 26
//<Quality> {"density":5,"unique_branches":{"translateString":[[3736,8,3736,20,0,0,4,0],[4017,7,4017,19,0,0,4,0]],"_lou_allocMem":[[5312,7,5312,32,0,0,4,1]],"insertEmphasesAt":[[3483,13,3483,17,0,0,4,1],[3494,13,3494,18,0,0,4,0],[3498,19,3498,39,0,0,4,0],[3498,19,3498,39,0,0,4,1],[3500,8,3500,25,0,0,4,0],[3508,20,3508,40,0,0,4,0],[3508,20,3508,40,0,0,4,1],[3509,9,3509,27,0,0,4,1],[3511,8,3511,15,0,0,4,0],[3518,15,3518,20,0,0,4,0],[3522,19,3522,39,0,0,4,0],[3522,19,3522,39,0,0,4,1],[3524,8,3524,25,0,0,4,0],[3532,39,3532,45,0,0,4,0],[3532,39,3532,45,0,0,4,1],[3533,9,3533,42,0,0,4,1],[3534,8,3534,25,0,0,4,0],[3541,38,3541,44,0,0,4,0],[3541,38,3541,44,0,0,4,1],[3542,8,3544,33,0,0,4,1],[3549,15,3549,19,0,0,4,1]],"getCharForDots":[[640,6,640,19,0,0,4,0]],"_lou_translate":[[1207,8,1207,29,0,0,4,0]]},"library_calls":["lou_indexTables","lou_getEmphClasses","lou_freeEmphClasses","lou_registerTableResolver","lou_checkTable","lou_translateString","lou_translate","lou_free"],"critical_calls":["lou_indexTables","lou_getEmphClasses","lou_freeEmphClasses","lou_registerTableResolver","lou_checkTable","lou_translateString","lou_translate","lou_free"],"visited":0}
/**/


extern "C" {
}

// Custom table resolver function
static char**  id_000040_custom_table_resolver(const char* tableList, const char* base) {
    // Simple resolver that returns NULL - this is just to demonstrate the API usage
    (void)tableList;
    (void)base;
    return NULL;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if data is too small for basic operations
    if (size < 4) {
        return 0;
    }

    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Extract table list from input data (first part until null byte)
    std::vector<char> tableListBuffer;
    int c;
    while ((c = fgetc(in_file)) != EOF && c != 0) {
        tableListBuffer.push_back(static_cast<char>(c));
    }
    tableListBuffer.push_back('\0');
    
    const char* tableList = tableListBuffer.data();
    
    // Prepare remaining data for translation
    // Read the rest as widechar input
    std::vector<widechar> inbuf;
    while ((c = fgetc(in_file)) != EOF) {
        inbuf.push_back(static_cast<widechar>(c));
    }
    assert_file_closed(&in_file);;
    
    if (inbuf.empty()) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Index tables
    const char* tables[] = {tableList, NULL};
    lou_indexTables(tables);

    // 2. Get emphasis classes
    const char** emphClasses = lou_getEmphClasses(tableList);
    // Free emphasis classes immediately after getting them
    if (emphClasses) {
        lou_freeEmphClasses(emphClasses);
    }

    // 3. Register table resolver
    lou_registerTableResolver( id_000040_custom_table_resolver);

    // 4. Check table
    lou_checkTable(tableList);

    // 5. Prepare buffers for translation
    int inlen = static_cast<int>(inbuf.size());
    int outlen = inlen * 2; // Reasonable estimate
    std::vector<widechar> outbuf(outlen);
    std::vector<formtype> typeform(inlen, 0);
    std::vector<char> spacing(inlen, 0);
    int outputPos, inputPos, cursorPos;
    
    // 6. First translation with lou_translateString
    int result1 = lou_translateString(
        tableList, 
        inbuf.data(), 
        &inlen,
        outbuf.data(), 
        &outlen,
        typeform.data(),
        spacing.data(),
        0
    );
    
    // Reset output buffer for second translation
    std::fill(outbuf.begin(), outbuf.end(), 0);
    outlen = static_cast<int>(inbuf.size()) * 2;
    
    // 7. Second translation with lou_translate (more comprehensive)
    int result2 = lou_translate(
        tableList,
        inbuf.data(),
        &inlen,
        outbuf.data(),
        &outlen,
        typeform.data(),
        spacing.data(),
        &outputPos,
        &inputPos,
        &cursorPos,
        0
    );

    // 8. Clean up
    lou_free();

    // Consume the results to avoid optimization
    (void)result1;
    (void)result2;
    (void)outputPos;
    (void)inputPos;
    (void)cursorPos;

    assert_file_closed(&in_file);
	return 0;
}