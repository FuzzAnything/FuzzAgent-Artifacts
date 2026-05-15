#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tiffio.hxx>
//<ID> 1645
//<Prompt> []
/*<Combination>: [
*/
//<score> 216, nr_unique_branch: 67
//<Quality> {"density":12,"unique_branches":{"TIFFWriteDirectoryTagSampleformatArray":[[1589,22,1589,57,0,0,4,0],[1591,29,1591,38,0,0,4,0],[1591,29,1591,38,0,0,4,1]],"TIFFClampDoubleToUInt32":[[1519,9,1519,16,0,0,4,0]],"LogL10fromY":[[898,14,898,28,0,0,4,0]],"TIFFClampDoubleToUInt16":[[1510,9,1510,16,0,0,4,1],[1512,9,1512,20,0,0,4,1],[1512,24,1512,34,0,0,4,1]],"LogLuv24fromXYZ":[[1071,9,1071,12,0,0,4,0],[1071,9,1071,12,0,0,4,1],[1071,16,1071,23,0,0,4,1],[1082,9,1082,15,0,0,4,1]],"TIFFWriteDirectorySec":[[716,21,716,61,0,0,4,0]],"_TIFFStrileSize64":[[131,14,131,38,0,0,4,0]],"_TIFFSetGetType":[[1083,9,1083,27,0,0,4,1],[1086,14,1086,24,0,0,4,0],[1086,28,1086,42,0,0,4,0],[1090,13,1090,27,0,0,4,1],[1091,13,1091,32,0,0,4,1],[1093,13,1093,28,0,0,4,1],[1095,13,1095,28,0,0,4,1],[1097,13,1097,27,0,0,4,0],[1099,13,1099,31,0,0,4,1],[1100,13,1100,32,0,0,4,1],[1101,13,1101,28,0,0,4,1],[1103,13,1103,28,0,0,4,1],[1105,13,1105,29,0,0,4,1],[1107,13,1107,28,0,0,4,1],[1109,13,1109,29,0,0,4,1],[1111,13,1111,26,0,0,4,1],[1112,13,1112,27,0,0,4,1],[1114,13,1114,28,0,0,4,1],[1116,13,1116,29,0,0,4,1],[1118,13,1118,29,0,0,4,1],[1119,13,1119,20,0,0,4,1]],"TIFFReadRawStrip":[[744,9,744,32,0,0,4,0]],"ToRationalEuclideanGCD":[[2802,39,2802,53,0,0,4,1],[2853,12,2853,35,0,0,4,0]],"CalcFinalIFDdatasizeReading":[[4298,13,4298,52,0,0,4,0]],"TIFFUnsetField":[[1153,9,1153,13,0,0,4,1],[1156,9,1156,39,0,0,4,0]],"TIFFSetDirectory":[[2066,9,2066,60,0,0,4,1],[2083,30,2083,55,0,0,4,1],[2087,13,2087,21,0,0,4,1],[2093,18,2093,50,0,0,4,0],[2101,24,2101,29,0,0,4,1],[2106,13,2106,28,0,0,4,0]],"TIFFCleanup":[[100,9,100,35,0,0,4,0],[104,21,104,47,0,0,4,0],[104,21,104,47,0,0,4,1],[106,17,106,56,0,0,4,0]],"_TIFFGetOffsetFromDirNumber":[[5971,9,5971,50,0,0,4,0]],"TIFFDefaultDirectory":[[1794,9,1794,35,0,0,4,0],[1798,21,1798,47,0,0,4,0],[1798,21,1798,47,0,0,4,1],[1800,17,1800,56,0,0,4,0]],"TIFFMergeFieldInfo":[[1249,9,1249,35,0,0,4,1],[1260,9,1260,31,0,0,4,1],[1272,9,1272,47,0,0,4,1],[1279,17,1279,22,0,0,4,0],[1279,17,1279,22,0,0,4,1],[1282,13,1282,53,0,0,4,1],[1283,13,1283,54,0,0,4,1],[1294,14,1294,42,0,0,4,1],[1294,46,1294,75,0,0,4,1],[1319,13,1319,39,0,0,4,1],[1332,9,1332,73,0,0,4,1]]},"library_calls":["TIFFOpen","TIFFMergeFieldInfo","TIFFCurrentDirOffset","TIFFSetDirectory","LogLuv24fromXYZ","TIFFGetField","TIFFUnsetField","TIFFNumberOfStrips","TIFFStripSize","_TIFFmalloc","TIFFReadRawStrip","_TIFFfree","TIFFClose"],"critical_calls":["TIFFOpen","TIFFMergeFieldInfo","TIFFCurrentDirOffset","TIFFSetDirectory","LogLuv24fromXYZ","TIFFGetField","TIFFUnsetField","TIFFNumberOfStrips","TIFFStripSize","_TIFFmalloc","TIFFReadRawStrip","_TIFFfree","TIFFClose"],"visited":0}
/**/


// libtiff headers

// Custom TIFFFieldInfo structure for TIFFMergeFieldInfo
struct TIFFFieldInfoEx : TIFFFieldInfo {
    TIFFFieldInfoEx(uint32_t tag, short readcount, short writecount,
                   TIFFDataType type, unsigned short bit,
                   unsigned char oktochange, unsigned char passcount,
                   const char* name) {
        field_tag = tag;
        field_readcount = readcount;
        field_writecount = writecount;
        field_type = type;
        field_bit = bit;
        field_oktochange = oktochange;
        field_passcount = passcount;
        field_name = name;
    }
};

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to temporary file for libtiff to read
    FILE* in_file = fopen("input_file", "wb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Open the TIFF file from the input data
    TIFF* tif = TIFFOpen("input_file", fuzz_str_1);
    if (!tif) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 1. TIFFMergeFieldInfo - Add custom field info
    // Create a custom field info structure for a private tag
    TIFFFieldInfoEx customFieldInfo(
        65000,  // Custom tag number (private tag space)
        1,      // Read count
        1,      // Write count
        TIFFDataType::TIFF_LONG,  // Data type
        0,      // Bit offset
        1,      // OK to change
        0,      // Pass count
        "CustomField"
    );
    
    int merge_result = TIFFMergeFieldInfo(tif, &customFieldInfo, 1);
    (void)merge_result; // Result intentionally unused for fuzzing
    
    // 2. TIFFSetSubDirectory - Set to a subdirectory if exists
    // First get current directory offset
    uint64_t current_offset = TIFFCurrentDirOffset(tif);
    
    // Try to set to the same directory (simulating subdirectory navigation)
    // In real usage, we would navigate to different IFDs
    int setdir_result = TIFFSetDirectory(tif, fuzz_uint32_t_2);
    (void)setdir_result; // Result intentionally unused for fuzzing
    
    // 3. LogLuv24fromXYZ - Convert XYZ color to LogLuv24
    // Create XYZ color values (normalized 0-1 range)
    float xyz[3];
    if (size >= 12) {
        // Use input data to initialize XYZ values
        memcpy(xyz, data, sizeof(float) * 3);
    } else {
        // Default values if input is too small
        xyz[0] = 0.412453f;  // X
        xyz[1] = 0.212671f;  // Y
        xyz[2] = 0.019334f;  // Z
    }
    
    // Convert to LogLuv24 format
    uint32_t logluv_value = LogLuv24fromXYZ(xyz, 0);
    (void)logluv_value; // Result intentionally unused for fuzzing
    
    // 4. TIFFUnsetField - Remove a field value
    // Try to unset a common field (ImageWidth)
    uint32_t image_width;
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &image_width)) {
        // Field exists, try to unset it
        int unset_result = TIFFUnsetField(tif, fuzz_uint32_t_3);
        (void)unset_result; // Result intentionally unused for fuzzing
    }
    
    // 5. TIFFSetMode is not in the provided API list, but we can use
    // TIFFGetMode to get current mode and simulate mode operations
    
    // Create output file to write modified TIFF
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // We would normally write TIFF data here
        // For fuzzing, we just create the file handle
        assert_file_closed(&out_file);;
        remove("output_file");
    }
    
    // Additional operations to ensure proper coverage
    // Read and process some TIFF data
    
    // Try to read a strip if the image has strips
    uint32_t num_strips = TIFFNumberOfStrips(tif);
    if (num_strips > 0) {
        tmsize_t strip_size = TIFFStripSize(tif);
        if (strip_size > 0 && strip_size < 1024*1024) { // Limit size for fuzzing
            void* strip_buffer = _TIFFmalloc(strip_size);
            if (strip_buffer) {
                // Read first strip
                tmsize_t bytes_read = TIFFReadRawStrip(tif, fuzz_uint32_t_4, strip_buffer, strip_size);
                (void)bytes_read; // Result intentionally unused for fuzzing
                _TIFFfree(strip_buffer);
            }
        }
    }
    
    // Clean up resources
    TIFFClose(tif);
    remove("input_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}