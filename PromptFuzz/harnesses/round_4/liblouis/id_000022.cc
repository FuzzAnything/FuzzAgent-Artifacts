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
//<ID> 490
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 1
//<Quality> {"density":4,"unique_branches":{"_lou_defaultTableResolver":[[4831,7,4831,13,0,0,4,1]]},"library_calls":["lou_checkTable","lou_getTable","lou_getDataPath","lou_translateString","lou_translatePrehyphenated"],"critical_calls":["lou_checkTable","lou_getTable","lou_getDataPath","lou_translateString","lou_translatePrehyphenated"],"visited":0}
/**/


// Include the necessary headers for liblouis
extern "C" {
    // The following are liblouis API declarations
    const void *lou_getTable(const char *tableList);
    int lou_translatePrehyphenated(const char *tableList, const widechar *inbuf, int *inlen, 
                                   widechar *outbuf, int *outlen, formtype *typeform, 
                                   char *spacing, int *outputPos, int *inputPos, 
                                   int *cursorPos, char *inputHyphens, char *outputHyphens, 
                                   int mode);
    char *lou_getDataPath();
    int lou_checkTable(const char *tableList);
    int lou_translateString(const char *tableList, const widechar *inbuf, int *inlen, 
                            widechar *outbuf, int *outlen, formtype *typeform, 
                            char *spacing, int mode);
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size < 2) {
	return 0;
	}
	  // Need at least some data for table name and input

    // Step 1: Parse the input data
    // First part: table list (null-terminated string)
    size_t table_name_len = 0;
    while (table_name_len < size && data[table_name_len] != 0) {
        table_name_len++;
    }
    
    if (table_name_len >= size) {
	return 0;
	}
	  // No null terminator found
    
    const char *table_list = reinterpret_cast<const char *>(data);
    
    // Step 2: Get the remaining data for translation input
    size_t input_offset = table_name_len + 1;
    if (input_offset >= size) {
	return 0;
	}
	
    
    size_t input_data_size = size - input_offset;
    const widechar *input_buf = reinterpret_cast<const widechar *>(data + input_offset);
    int inlen = static_cast<int>(input_data_size / sizeof(widechar));
    
    if (inlen <= 0) {
	return 0;
	}
	

    // Step 3: Check the table
    int table_check = lou_checkTable(table_list);
    if (table_check == 0) {
        // Table is not valid, but we'll still continue to test other APIs
        // This helps fuzzer find invalid table scenarios
    }

    // Step 4: Get the table handle
    const void *table_handle = lou_getTable(table_list);
    // Note: table_handle might be null for invalid tables, but we continue

    // Step 5: Get data path
    char *data_path = lou_getDataPath();
    if (data_path) {
        // We could use the data path for something, but for fuzzing we just get it
        // No need to free - lou_getDataPath returns a pointer to internal data
    }

    // Step 6: Prepare buffers for translation
    int outlen = inlen * 4;  // Conservative estimate for output buffer size
    std::vector<widechar> outbuf(outlen);
    std::vector<widechar> outbuf2(outlen);
    
    // Additional buffers for translatePrehyphenated
    std::vector<formtype> typeform(inlen);
    std::vector<char> spacing(inlen);
    std::vector<int> outputPos(inlen);
    std::vector<int> inputPos(inlen);
    std::vector<int> cursorPos(inlen);
    std::vector<char> inputHyphens(inlen);
    std::vector<char> outputHyphens(outlen);
    
    // Initialize some values
    for (int i = 0; i < inlen; i++) {
        typeform[i] = 0;
        spacing[i] = 0;
        inputHyphens[i] = (i % 2 == 0) ? 1 : 0;  // Simple pattern for hyphens
    }

    // Step 7: Call translateString
    int inlen_copy1 = inlen;
    int outlen_copy1 = outlen;
    lou_translateString(table_list, input_buf, &inlen_copy1, 
                        outbuf.data(), &outlen_copy1, 
                        typeform.data(), spacing.data(), fuzz_int32_t_1);

    // Step 8: Call translatePrehyphenated
    int inlen_copy2 = inlen;
    int outlen_copy2 = outlen;
    lou_translatePrehyphenated(table_list, input_buf, &inlen_copy2,
                               outbuf2.data(), &outlen_copy2,
                               typeform.data(), spacing.data(),
                               outputPos.data(), inputPos.data(),
                               cursorPos.data(), inputHyphens.data(),
                               outputHyphens.data(), fuzz_int32_t_2);

    // Step 9: Clean up - liblouis doesn't require explicit cleanup for most resources
    // The table handle from lou_getTable doesn't need to be freed
    // lou_getDataPath returns a pointer to internal data that shouldn't be freed
    
    return 0;
}