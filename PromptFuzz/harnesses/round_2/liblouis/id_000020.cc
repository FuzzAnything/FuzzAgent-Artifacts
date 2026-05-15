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
//<ID> 295
//<Prompt> []
/*<Combination>: [
*/
//<score> 37.2, nr_unique_branch: 228
//<Quality> {"density":6,"unique_branches":{"markSyllables":[[2509,10,2509,37,0,0,4,0],[2519,3,2519,18,0,0,4,1],[2523,3,2523,20,0,0,4,0],[2529,8,2529,45,0,0,4,1],[2530,16,2530,33,0,0,4,0],[2530,16,2530,33,0,0,4,1]],"checkEmphasisChange":[[1847,34,1847,55,0,0,4,0]],"_lou_allocMem":[[5338,8,5338,27,0,0,4,0],[5352,8,5352,27,0,0,4,0],[5366,8,5366,27,0,0,4,0]],"undefinedCharacter":[[2320,10,2320,16,0,0,4,1],[2329,7,2329,15,0,0,4,0]],"_lou_translate":[[1250,6,1250,26,0,0,4,1],[1250,30,1250,48,0,0,4,0]],"doCompbrl":[[2384,6,2384,57,0,0,4,1],[2385,16,2385,32,0,0,4,0],[2387,9,2387,34,0,0,4,0],[2387,9,2387,34,0,0,4,1],[2388,4,2388,61,0,0,4,0],[2388,4,2388,61,0,0,4,1]],"doCompTrans":[[2405,6,2405,24,0,0,4,0],[2405,28,2405,86,0,0,4,0],[2406,7,2407,62,0,0,4,1],[2409,18,2409,25,0,0,4,0],[2409,18,2409,25,0,0,4,1],[2415,7,2415,40,0,0,4,1],[2421,7,2421,20,0,0,4,0],[2421,7,2421,20,0,0,4,1],[2423,8,2425,56,0,0,4,1],[2427,14,2428,45,0,0,4,1],[2431,6,2431,24,0,0,4,0],[2431,28,2431,86,0,0,4,0],[2432,7,2433,62,0,0,4,1],[2436,6,2436,20,0,0,4,1]],"_lou_charToFallbackDots":[[434,6,434,15,0,0,4,1],[439,10,439,16,0,0,4,0],[439,10,439,16,0,0,4,1]],"lou_getEmphClasses":[[5090,6,5090,12,0,0,4,1],[5092,9,5092,33,0,0,4,0],[5094,7,5094,12,0,0,4,0],[5094,7,5094,12,0,0,4,1],[5102,7,5102,14,0,0,4,1]],"checkDotsAttr":[[197,10,197,47,0,0,4,0],[197,10,197,47,0,0,4,1]],"matchFeatureLists":[[468,9,468,36,0,0,4,0],[476,11,476,16,0,0,4,1],[478,16,478,18,0,0,4,0],[481,11,481,13,0,0,4,1],[481,17,481,48,0,0,4,0],[481,17,481,48,0,0,4,1],[483,10,483,18,0,0,4,1]],"for_selectRule":[[2030,10,2030,48,0,0,4,1],[2031,8,2031,48,0,0,4,0],[2031,8,2031,48,0,0,4,1],[2039,7,2039,22,0,0,4,0],[2049,7,2049,23,0,0,4,0],[2075,7,2075,24,0,0,4,0],[2078,12,2078,68,0,0,4,0],[2099,12,2099,44,0,0,4,0],[2100,10,2100,40,0,0,4,0],[2101,10,2102,43,0,0,4,0],[2118,7,2118,24,0,0,4,0],[2119,12,2119,24,0,0,4,1],[2119,28,2119,51,0,0,4,1],[2120,12,2120,43,0,0,4,0],[2120,12,2120,43,0,0,4,1],[2121,10,2121,40,0,0,4,0],[2143,7,2143,23,0,0,4,0],[2144,12,2144,24,0,0,4,1],[2144,28,2144,51,0,0,4,1],[2145,12,2145,42,0,0,4,0],[2145,12,2145,42,0,0,4,1],[2146,10,2146,39,0,0,4,0],[2146,10,2146,39,0,0,4,1],[2147,10,2147,47,0,0,4,0],[2147,10,2147,47,0,0,4,1],[2150,7,2150,28,0,0,4,0],[2151,12,2151,24,0,0,4,1],[2151,28,2151,51,0,0,4,1],[2152,12,2152,60,0,0,4,0],[2152,12,2152,60,0,0,4,1],[2153,10,2154,28,0,0,4,0],[2153,10,2154,28,0,0,4,1],[2155,10,2156,43,0,0,4,0],[2159,7,2159,30,0,0,4,0],[2160,12,2160,24,0,0,4,1],[2160,28,2160,51,0,0,4,1],[2161,12,2163,31,0,0,4,0],[2164,10,2166,32,0,0,4,0],[2179,7,2179,23,0,0,4,0],[2180,12,2180,24,0,0,4,1],[2180,28,2180,51,0,0,4,1],[2181,12,2181,62,0,0,4,0],[2181,12,2181,62,0,0,4,1],[2182,10,2182,40,0,0,4,0],[2182,10,2182,40,0,0,4,1],[2192,7,2192,23,0,0,4,0],[2193,12,2193,24,0,0,4,1],[2193,28,2193,51,0,0,4,1],[2194,12,2194,41,0,0,4,0],[2194,12,2194,41,0,0,4,1],[2195,10,2195,38,0,0,4,0],[2195,10,2195,38,0,0,4,1],[2198,7,2198,26,0,0,4,0],[2199,12,2199,24,0,0,4,1],[2199,28,2199,51,0,0,4,1],[2200,12,2200,41,0,0,4,0],[2200,12,2200,41,0,0,4,1],[2201,10,2202,54,0,0,4,0],[2201,10,2202,54,0,0,4,1],[2205,7,2205,23,0,0,4,0],[2206,12,2206,24,0,0,4,1],[2206,28,2206,51,0,0,4,1],[2207,12,2207,41,0,0,4,0],[2207,12,2207,41,0,0,4,1],[2208,10,2208,57,0,0,4,1],[2219,10,2219,37,0,0,4,0],[2222,7,2222,22,0,0,4,0],[2223,12,2223,40,0,0,4,1],[2232,12,2233,53,0,0,4,0],[2247,12,2248,53,0,0,4,0]],"validMatch":[[1672,7,1672,42,0,0,4,1],[1674,6,1674,42,0,0,4,1],[1675,6,1677,66,0,0,4,0]],"insertLetterSign":[[1727,6,1727,68,0,0,4,1],[1735,7,1735,37,0,0,4,0]],"insertEmphasisEnd":[[3410,6,3410,35,0,0,4,0],[3412,7,3412,37,0,0,4,0],[3413,8,3413,75,0,0,4,0]],"onlyLettersAhead":[[1771,6,1771,36,0,0,4,0],[1771,6,1771,36,0,0,4,1],[1772,36,1772,53,0,0,4,0],[1772,36,1772,53,0,0,4,1],[1775,7,1775,25,0,0,4,0],[1775,7,1775,25,0,0,4,1],[1776,7,1776,43,0,0,4,0],[1776,7,1776,43,0,0,4,1]],"resetsEmphMode":[[2553,11,2553,21,0,0,4,0],[2555,9,2555,52,0,0,4,0],[2556,7,2556,39,0,0,4,0]],"matchLanguageTags":[[379,6,379,35,0,0,4,0],[379,6,379,35,0,0,4,1],[381,11,381,50,0,0,4,0],[385,9,385,14,0,0,4,0],[386,7,386,11,0,0,4,0]],"translateString":[[3657,17,3657,41,0,0,4,1],[3668,5,3668,38,0,0,4,1],[3718,3,3718,19,0,0,4,0],[3719,8,3721,33,0,0,4,1],[3724,3,3724,10,0,0,4,1],[3825,8,3825,40,0,0,4,0],[3827,12,3827,30,0,0,4,0],[3828,7,3829,61,0,0,4,0],[3828,7,3829,61,0,0,4,1],[3830,10,3830,61,0,0,4,0],[3830,10,3830,61,0,0,4,1],[3835,9,3835,27,0,0,4,0],[3835,9,3835,27,0,0,4,1],[3905,17,3905,34,0,0,4,1],[3996,3,3996,24,0,0,4,0],[3997,11,3997,30,0,0,4,0],[3998,6,3998,56,0,0,4,0],[3998,6,3998,56,0,0,4,1],[3999,6,3999,41,0,0,4,0],[3999,6,3999,41,0,0,4,1]],"insertNumberSign":[[1690,6,1690,68,0,0,4,1]],"backTranslateString":[[1300,3,1300,19,0,0,4,0],[1301,11,1301,31,0,0,4,0],[1301,11,1301,31,0,0,4,1],[1387,3,1387,18,0,0,4,0]],"insertEmphasisBegin":[[3398,7,3398,39,0,0,4,1]],"passDoTest":[[800,3,800,25,0,0,4,0],[804,3,804,23,0,0,4,0],[821,9,821,44,0,0,4,0],[895,8,895,26,0,0,4,1]],"toUppercase":[[960,6,960,17,0,0,4,0]],"back_selectRule":[[741,10,741,50,0,0,4,1],[742,8,742,50,0,0,4,0],[742,8,742,50,0,0,4,1],[753,6,753,21,0,0,4,0],[775,6,775,22,0,0,4,0],[821,9,821,38,0,0,4,0],[822,9,822,45,0,0,4,0],[822,9,822,45,0,0,4,1],[828,10,828,40,0,0,4,1],[828,44,828,63,0,0,4,1],[833,11,833,59,0,0,4,0],[843,11,843,51,0,0,4,0],[854,6,854,23,0,0,4,0],[855,11,855,45,0,0,4,0],[856,10,856,39,0,0,4,0],[856,10,856,39,0,0,4,1],[857,11,858,29,0,0,4,0],[857,11,858,29,0,0,4,1],[867,11,867,42,0,0,4,0],[885,11,885,25,0,0,4,0],[885,29,885,62,0,0,4,0]],"noCompbrlAhead":[[1794,5,1794,36,0,0,4,0],[1795,7,1795,73,0,0,4,1],[1796,8,1796,65,0,0,4,1],[1827,43,1827,53,0,0,4,1],[1833,25,1833,48,0,0,4,0],[1834,10,1834,41,0,0,4,0]],"lou_translatePrehyphenated":[[1418,7,1418,23,0,0,4,0],[1419,8,1419,64,0,0,4,1],[1424,6,1425,42,0,0,4,0],[1426,7,1426,27,0,0,4,0],[1429,21,1429,37,0,0,4,0],[1429,21,1429,37,0,0,4,1],[1431,9,1431,26,0,0,4,1],[1435,9,1435,26,0,0,4,0],[1435,9,1435,26,0,0,4,1],[1443,6,1443,28,0,0,4,0]],"resolveEmphasisResets":[[3189,10,3189,38,0,0,4,0],[3203,11,3203,21,0,0,4,0],[3204,9,3204,54,0,0,4,1],[3221,30,3221,45,0,0,4,1]],"_lou_backTranslate":[[261,30,261,48,0,0,4,0],[307,8,307,32,0,0,4,0]],"resolveEmphasisWords":[[2773,11,2773,36,0,0,4,0],[2792,11,2792,50,0,0,4,0],[2793,9,2793,48,0,0,4,1],[2858,7,2858,46,0,0,4,0],[2859,5,2859,44,0,0,4,1],[2877,8,2877,36,0,0,4,0],[2878,9,2878,24,0,0,4,0],[2878,28,2878,53,0,0,4,0],[2882,19,2882,51,0,0,4,1]]},"library_calls":["lou_setDataPath","lou_getDataPath","lou_setDataPath","lou_setDataPath","lou_logFile","lou_findTable","lou_getTable","lou_getEmphClasses","lou_freeEmphClasses","lou_freeTableFile","lou_freeTableFile","lou_charToDots","lou_backTranslateString","lou_translatePrehyphenated","lou_freeTableFile","lou_logEnd"],"critical_calls":["lou_setDataPath","lou_getDataPath","lou_setDataPath","lou_setDataPath","lou_logFile","lou_findTable","lou_getTable","lou_getEmphClasses","lou_freeEmphClasses","lou_charToDots","lou_backTranslateString","lou_translatePrehyphenated","lou_freeTableFile","lou_logEnd"],"visited":4}
/**/


// Include liblouis headers (assuming they're available)
extern "C" {
    #include <liblouis/liblouis.h>
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if data is too small for basic operations
    if (size < 2) {
	return 0;
	}
	
    
    // Initialize liblouis
    lou_setDataPath(nullptr);  // Use default data path
    
    // 1. Use lou_setDataPath and lou_getDataPath
    char *data_path = lou_getDataPath();
    if (data_path) {
        // Set a new data path (use current directory)
        char *old_path = lou_setDataPath(".");
        // Restore original path
        lou_setDataPath(data_path);
    }
    
    // 2. Use lou_logFile to set up logging
    lou_logFile("liblouis_fuzz_log.txt");
    
    // 3. Extract a table name from the input data
    // Find null terminator for table name or use first part of data
    size_t table_name_len = 0;
    while (table_name_len < size && data[table_name_len] != 0) {
        table_name_len++;
    }
    
    // If no null terminator found, use a default table
    std::string table_name;
    if (table_name_len < size && table_name_len > 0) {
        table_name.assign(reinterpret_cast<const char*>(data), table_name_len);
    } else {
        table_name = "en-us-g2.ctb";  // Default braille table
    }
    
    // 4. Use lou_findTable to locate the table
    char *found_table = lou_findTable(table_name.c_str());
    const char *table_to_use = found_table ? found_table : table_name.c_str();
    
    // 5. Use lou_getTable to load the table
    const void *table_ptr = lou_getTable(table_to_use);
    
    // 6. Use lou_getEmphClasses
    const char **emph_classes = lou_getEmphClasses(table_to_use);
    if (emph_classes) {
        lou_freeEmphClasses(emph_classes);
    }
    
    // Calculate remaining data after table name
    size_t data_offset = table_name_len;
    if (data_offset < size && data[data_offset] == 0) {
        data_offset++;  // Skip the null terminator
    }
    
    // Ensure we have enough data for widechar operations
    size_t remaining_size = size - data_offset;
    if (remaining_size < 2) {
        if (found_table) lou_freeTableFile(found_table);
        return 0;
    }
    
    // Convert remaining data to widechar array
    // Each widechar is 2 bytes, but we'll use the data as-is
    int widechar_count = static_cast<int>(remaining_size / sizeof(widechar));
    if (widechar_count == 0) {
        if (found_table) lou_freeTableFile(found_table);
        return 0;
    }
    
    // Create input buffer (copy the data)
    std::vector<widechar> inbuf(widechar_count);
    memcpy(inbuf.data(), data + data_offset, std::min(remaining_size, inbuf.size() * sizeof(widechar)));
    
    // 7. Use lou_charToDots - convert characters to braille dots
    std::vector<widechar> dots_buf(inbuf.size());
    int dots_result = lou_charToDots(table_to_use, inbuf.data(), dots_buf.data(), 
                                     static_cast<int>(inbuf.size()), 0);
    
    // 8. Use lou_backTranslateString - translate braille back to text
    int inlen = static_cast<int>(inbuf.size());
    std::vector<widechar> back_outbuf(inbuf.size() * 2);  // Allocate extra space
    int outlen = static_cast<int>(back_outbuf.size());
    
    int back_translate_result = lou_backTranslateString(table_to_use, inbuf.data(), 
                                                        &inlen, back_outbuf.data(), 
                                                        &outlen, nullptr, nullptr, 0);
    
    // 9. Use lou_translatePrehyphenated - forward translation with hyphenation
    // Prepare buffers for translation with hyphenation
    if (back_translate_result > 0 && outlen > 0) {
        int prehyphen_inlen = outlen;
        std::vector<widechar> prehyphen_outbuf(outlen * 2);
        int prehyphen_outlen = static_cast<int>(prehyphen_outbuf.size());
        
        // Create hyphenation arrays
        std::vector<char> input_hyphens(prehyphen_inlen, 0);
        std::vector<char> output_hyphens(prehyphen_outbuf.size(), 0);
        
        // Add some hyphenation markers (every 3 characters for testing)
        for (size_t i = 3; i < input_hyphens.size(); i += 3) {
            if (i < input_hyphens.size()) {
                input_hyphens[i] = 1;
            }
        }
        
        lou_translatePrehyphenated(table_to_use, back_outbuf.data(), &prehyphen_inlen,
                                   prehyphen_outbuf.data(), &prehyphen_outlen,
                                   nullptr, nullptr, nullptr, nullptr, nullptr,
                                   input_hyphens.data(), output_hyphens.data(), 0);
    }
    
    // Clean up
    if (found_table) {
        lou_freeTableFile(found_table);
    }
    
    // End logging
    lou_logEnd();
    
    return 0;
}