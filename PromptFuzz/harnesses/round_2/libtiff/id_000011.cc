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
//<ID> 556
//<Prompt> []
/*<Combination>: [
*/
//<score> 481, nr_unique_branch: 135
//<Quality> {"density":13,"unique_branches":{"TIFFClampDoubleToUInt8":[[1501,9,1501,16,0,0,4,0],[1503,9,1503,18,0,0,4,0]],"TIFFWriteDirectoryTagLong8Array":[[1737,9,1737,38,0,0,4,1],[1746,9,1746,18,0,0,4,0],[1746,9,1746,18,0,0,4,1],[1752,37,1752,47,0,0,4,0],[1752,37,1752,47,0,0,4,1],[1754,13,1754,29,0,0,4,1]],"TIFFFetchNormalTag":[[7475,17,7475,55,0,0,4,0],[7476,17,7476,43,0,0,4,1]],"TIFFReadDirEntryArrayWithLimit":[[1342,17,1342,43,0,0,4,0]],"TIFFClampDoubleToUInt32":[[1521,9,1521,26,0,0,4,0]],"_TIFFmalloc":[[336,9,336,15,0,0,4,0]],"_WriteAsType":[[1923,14,1923,45,0,0,4,0]],"TIFFWriteDirectoryTagCheckedSshortArray":[[2426,9,2426,20,0,0,4,0],[2426,9,2426,20,0,0,4,1],[2431,9,2431,35,0,0,4,1]],"TIFFWriteDirectoryTagCheckedSlongArray":[[2481,9,2481,20,0,0,4,0],[2481,9,2481,20,0,0,4,1],[2486,9,2486,35,0,0,4,1]],"_TIFFRemoveEntryFromDirectoryListByOffset":[[6010,9,6010,25,0,0,4,1]],"TIFFCheckpointDirectory":[[296,9,296,46,0,0,4,0],[296,9,296,46,0,0,4,1]],"Fax3VGetField":[[1433,9,1433,35,0,0,4,0],[1437,9,1437,33,0,0,4,0]],"TIFFRasterScanlineSize64":[[388,9,388,51,0,0,4,0],[388,9,388,51,0,0,4,1],[328,6,328,16,2,0,4,0],[328,6,328,16,2,0,4,1],[328,6,328,16,3,0,4,0],[328,6,328,16,3,0,4,1]],"DoubleToSrational":[[2969,9,2969,34,0,0,4,1],[2976,9,2976,42,0,0,4,1],[2991,9,2991,29,0,0,4,1],[2991,33,2991,55,0,0,4,1],[2992,9,2992,30,0,0,4,1],[2992,34,2992,57,0,0,4,1],[3005,9,3005,27,0,0,4,0],[3005,9,3005,27,0,0,4,1]],"OJPEGVGetField":[[551,9,551,30,0,0,4,0],[554,9,554,41,0,0,4,0]],"TIFFWriteDirectorySec":[[507,9,507,18,0,0,4,1],[550,9,550,61,0,0,4,0],[733,25,736,62,0,0,4,0],[798,21,800,54,0,0,4,1],[828,21,830,74,0,0,4,1],[835,21,837,59,0,0,4,1],[849,21,851,58,0,0,4,1],[929,29,929,52,0,0,4,0],[936,37,938,43,0,0,4,1],[1031,17,1031,36,0,0,4,0],[1032,25,1034,78,0,0,4,1],[1043,17,1043,32,0,0,4,0],[1044,25,1046,77,0,0,4,1],[1055,17,1055,33,0,0,4,0],[1056,25,1058,78,0,0,4,1],[1067,17,1067,32,0,0,4,0],[1068,25,1070,78,0,0,4,1],[1073,17,1073,32,0,0,4,0],[1074,25,1076,79,0,0,4,0],[1074,25,1076,79,0,0,4,1],[1079,17,1079,33,0,0,4,0],[1080,25,1082,78,0,0,4,1],[1104,29,1106,80,0,0,4,0],[1157,17,1157,32,0,0,4,0],[1158,25,1160,76,0,0,4,1],[1175,17,1175,31,0,0,4,0],[1176,25,1178,79,0,0,4,0],[1228,21,1228,79,0,0,4,0],[1437,9,1437,18,0,0,4,1],[1449,9,1449,18,0,0,4,1],[236,34,236,71,61,0,4,0],[236,34,236,71,69,0,4,0],[236,34,236,71,71,0,4,0],[236,34,236,71,75,0,4,0]],"TIFFWriteDirectoryTagCheckedRationalArray":[[2596,9,2596,18,0,0,4,0]],"_TIFFGetStrileOffsetOrByteCountValue":[[8489,13,8489,18,0,0,4,1]],"TIFFReadDirEntryFloatArray":[[2815,9,2815,25,0,0,4,0],[3006,9,3006,25,0,0,4,0],[3011,17,3011,43,0,0,4,1],[3016,25,3016,34,0,0,4,0],[3016,25,3016,34,0,0,4,1],[3019,21,3019,34,0,0,4,1],[3021,26,3021,40,0,0,4,1]],"TIFFRawStripSize64":[[181,9,181,23,0,0,4,0],[181,9,181,23,0,0,4,1]],"TIFFWriteDirectoryTagSampleformatArray":[[1589,22,1589,57,0,0,4,0],[1591,29,1591,38,0,0,4,0],[1591,29,1591,38,0,0,4,1]],"TIFFReadDirEntryDoubleArray":[[3049,9,3049,24,0,0,4,0],[3152,9,3152,24,0,0,4,0],[3159,25,3159,34,0,0,4,0],[3159,25,3159,34,0,0,4,1],[3161,21,3161,47,0,0,4,1]],"TIFFReadDirEntryLong8":[[887,17,887,45,0,0,4,0],[890,17,890,45,0,0,4,1]],"TIFFFetchDirectory":[[6109,17,6109,43,0,0,4,0],[6164,21,6164,47,0,0,4,0],[6331,13,6331,39,0,0,4,0],[6335,13,6335,39,0,0,4,0],[6341,17,6341,43,0,0,4,0]],"TIFFWriteDirectoryTagCheckedFloatArray":[[3024,9,3024,20,0,0,4,0],[3024,9,3024,20,0,0,4,1],[3030,9,3030,35,0,0,4,1]],"TIFFWriteDirectoryTagCheckedSbyteArray":[[2373,9,2373,20,0,0,4,0],[2373,9,2373,20,0,0,4,1]],"TIFFWriteDirectoryTagLongLong8Array":[[2006,27,2006,56,0,0,4,0]],"TIFFWriteDirectoryTagSlong8Array":[[1790,9,1790,38,0,0,4,1],[1799,9,1799,18,0,0,4,1],[1805,37,1805,47,0,0,4,0],[1805,37,1805,47,0,0,4,1],[1807,13,1807,31,0,0,4,1],[1817,18,1817,41,0,0,4,1]],"_TIFFVSetField":[[531,9,531,38,0,0,4,0]],"TIFFGetTagListEntry":[[47,9,47,22,0,0,4,1],[47,26,47,62,0,0,4,0],[47,26,47,62,0,0,4,1]],"TIFFReadDirEntryByte":[[331,9,331,25,0,0,4,0],[336,17,336,45,0,0,4,0]],"TIFFWriteDirectoryTagIfdIfd8Array":[[2098,9,2098,38,0,0,4,1],[2108,9,2108,18,0,0,4,0]],"ToRationalEuclideanGCD":[[2780,9,2780,26,0,0,4,0]],"TIFFClampDoubleToUInt16":[[1510,9,1510,16,0,0,4,1],[1512,9,1512,20,0,0,4,1],[1512,24,1512,34,0,0,4,1]],"_TIFFSetDefaultPostDecode":[[152,9,152,35,0,0,4,0],[155,13,155,39,0,0,4,1],[157,18,157,44,0,0,4,1],[159,18,159,44,0,0,4,1],[161,18,161,44,0,0,4,1],[163,18,163,45,0,0,4,1]],"TIFFSetupStrips":[[576,9,576,53,0,0,4,0],[583,9,583,40,0,0,4,0],[553,30,553,64,3,0,4,1],[236,34,236,71,10,0,4,0]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarnAboutUnknownTags","TIFFOpenExt","_TIFFfree","TIFFCheckpointDirectory","TIFFGetTagListEntry","TIFFRasterScanlineSize64","TIFFRawStripSize64","TIFFSetWriteOffset","TIFFSetWriteOffset","TIFFSetSubDirectory","TIFFWriteDirectory","TIFFClose","_TIFFfree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarnAboutUnknownTags","TIFFOpenExt","TIFFCheckpointDirectory","TIFFGetTagListEntry","TIFFRasterScanlineSize64","TIFFRawStripSize64","TIFFSetWriteOffset","TIFFSetSubDirectory","TIFFWriteDirectory","TIFFClose","_TIFFfree"],"visited":0}
/*Here's a fuzz driver that uses all the specified libtiff APIs together in a realistic scenario:

*/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to a temporary file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Create TIFFOpenOptions
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Enable warning about unknown tags
    TIFFOpenOptionsSetWarnAboutUnknownTags(opts, 1);
    
    // Open the TIFF file for reading and writing
    TIFF *tif = TIFFOpenExt("input_file", "r+", opts);
    if (!tif) {
        _TIFFfree(opts);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Get current directory information
    uint64_t dir_offset = 0;
    int checkpoint_result = TIFFCheckpointDirectory(tif);
    
    // Try to get tag information
    uint32_t tag_value = TIFFGetTagListEntry(tif, 0);
    
    // Calculate raster scanline size
    uint64_t scanline_size = TIFFRasterScanlineSize64(tif);
    
    // Try to get raw strip size for first strip
    uint64_t raw_strip_size = TIFFRawStripSize64(tif, 0);
    
    // Set write offset based on scanline size or raw strip size
    if (raw_strip_size > 0 && raw_strip_size < 1000000) {
        TIFFSetWriteOffset(tif, raw_strip_size);
    } else if (scanline_size > 0 && scanline_size < 1000000) {
        TIFFSetWriteOffset(tif, scanline_size);
    }
    
    // Try to set subdirectory
    if (size > sizeof(uint64_t)) {
        uint64_t subdir_offset = 0;
        memcpy(&subdir_offset, data, sizeof(uint64_t));
        if (subdir_offset < 1000000) { // Reasonable limit
            TIFFSetSubDirectory(tif, subdir_offset);
        }
    }
    
    // Write a new directory
    TIFFWriteDirectory(tif);
    
    // Clean up
    TIFFClose(tif);
    _TIFFfree(opts);
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}