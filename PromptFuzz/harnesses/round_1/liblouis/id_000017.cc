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
//<ID> 142
//<Prompt> []
/*<Combination>: [
*/
//<score> 2, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["lou_getDataPath","lou_getTable","lou_dotsToChar","lou_backTranslateString","lou_backTranslate","lou_free"],"critical_calls":["lou_getDataPath","lou_getTable","lou_dotsToChar","lou_backTranslateString","lou_backTranslate","lou_free"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}




    // Validate minimal input size
    if (size < sizeof(widechar)) {
        return 0;
    }

    // Setup output files
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&out_file);
	return 0;
    }

    // 1. Get data path using lou_getDataPath
    char *data_path = lou_getDataPath();
    
    // 2. Load a braille table using lou_getTable
    const char *table_list = "en-us-g2.ctb";  // Common English braille table
    const void *table = lou_getTable(fuzz_str_1);
    if (!table) {
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }

    // Convert input to widechar array for liblouis processing
    // Ensure we have an even number of bytes for widechar conversion
    size_t widechar_count = size / sizeof(widechar);
    if (widechar_count == 0) {
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	return 0;
    }

    std::vector<widechar> inbuf(widechar_count);
    memcpy(inbuf.data(), data, widechar_count * sizeof(widechar));

    // Prepare output buffers
    std::vector<widechar> outbuf1(widechar_count * 2);  // For dotsToChar
    std::vector<widechar> outbuf2(widechar_count * 2);  // For backTranslateString
    std::vector<widechar> outbuf3(widechar_count * 2);  // For backTranslate
    
    // Prepare auxiliary arrays
    int inlen1 = static_cast<int>(widechar_count);
    int outlen1 = static_cast<int>(outbuf1.size());
    
    int inlen2 = static_cast<int>(widechar_count);
    int outlen2 = static_cast<int>(outbuf2.size());
    
    int inlen3 = static_cast<int>(widechar_count);
    int outlen3 = static_cast<int>(outbuf3.size());

    // 3. First conversion: dotsToChar
    int result1 = lou_dotsToChar(
        fuzz_str_2,
        inbuf.data(),
        outbuf1.data(),
        inlen1,
        fuzz_int32_t_3  // mode
    );

    // If dotsToChar succeeded, use its output as input for back-translation
    if (result1 > 0) {
        // Update lengths for back-translation
        int converted_len = result1;
        
        // 4. backTranslateString
        int result2 = lou_backTranslateString(
            fuzz_str_4,
            outbuf1.data(),  // Use dotsToChar output as input
            &converted_len,
            outbuf2.data(),
            &outlen2,
            nullptr,  // typeform
            nullptr,  // spacing
            0         // mode
        );

        // 5. backTranslate (with position mappings)
        if (result2 > 0) {
            std::vector<int> outputPos(outbuf3.size());
            std::vector<int> inputPos(outbuf3.size());
            std::vector<int> cursorPos(outbuf3.size());
            
            int result3 = lou_backTranslate(
                fuzz_str_5,
                outbuf1.data(),  // Use same input as backTranslateString
                &converted_len,
                outbuf3.data(),
                &outlen3,
                nullptr,        // typeform
                nullptr,        // spacing
                outputPos.data(),
                inputPos.data(),
                cursorPos.data(),
                0              // mode
            );

            // Write some results to output file
            if (result3 > 0) {
                fwrite(outbuf3.data(), sizeof(widechar), 
                       std::min<size_t>(result3, outbuf3.size()), out_file);
            }
        }
    }

    // Cleanup
    assert_file_closed(&out_file);;
    
    // Free data path string if allocated
    if (data_path) {
        free(data_path);
    }
    
    // Final cleanup of liblouis resources
    lou_free();

    assert_file_closed(&out_file);
	return 0;
}