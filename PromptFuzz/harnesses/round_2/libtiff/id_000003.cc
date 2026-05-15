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
//<ID> 214
//<Prompt> []
/*<Combination>: [
*/
//<score> 456, nr_unique_branch: 320
//<Quality> {"density":19,"unique_branches":{"TIFFReadDirEntryDoubleArray":[[3214,21,3214,29,0,0,4,0]],"TIFFFieldSetGetSize":[[779,9,779,36,0,0,4,0]],"TIFFListDestroy":[[264,12,264,21,0,0,4,0]],"TIFFReadDirEntryShortArray":[[1777,9,1777,24,0,0,4,0],[1873,9,1873,24,0,0,4,0],[1880,25,1880,34,0,0,4,0],[1882,21,1882,47,0,0,4,1],[1885,21,1885,49,0,0,4,0],[1885,21,1885,49,0,0,4,1]],"OJPEGReadBufferFill":[[2232,17,2232,54,0,0,4,1]],"TIFFHashSetFindPtr":[[414,12,414,15,0,0,4,0],[416,13,416,46,0,0,4,0],[416,13,416,46,0,0,4,1]],"_tiffReadProc":[[74,9,74,38,0,0,4,1],[80,26,80,50,0,0,4,0],[80,26,80,50,0,0,4,1],[84,13,84,34,0,0,4,1],[89,13,89,23,0,0,4,0],[89,13,89,23,0,0,4,1],[92,9,92,18,0,0,4,1]],"TIFFWriteDirectorySec":[[497,9,497,34,0,0,4,1],[507,9,507,18,0,0,4,0],[509,13,509,45,0,0,4,1],[527,13,527,31,0,0,4,1],[536,13,536,45,0,0,4,1],[550,9,550,61,0,0,4,1],[565,13,565,24,0,0,4,0],[565,13,565,24,0,0,4,1],[567,13,567,20,0,0,4,0],[573,21,575,80,0,0,4,1],[577,21,579,53,0,0,4,1],[631,21,633,77,0,0,4,1],[666,21,668,57,0,0,4,1],[673,21,675,54,0,0,4,1],[694,21,696,78,0,0,4,1],[716,21,716,61,0,0,4,0],[892,29,892,49,0,0,4,0],[892,29,892,49,0,0,4,1],[896,25,896,54,0,0,4,0],[896,25,896,54,0,0,4,1],[897,25,897,58,0,0,4,1],[1018,21,1018,69,0,0,4,1],[1188,13,1188,24,0,0,4,0],[1188,13,1188,24,0,0,4,1],[1193,13,1193,45,0,0,4,0],[1200,13,1200,24,0,0,4,1],[1205,13,1205,20,0,0,4,0],[1215,17,1215,37,0,0,4,0],[1217,21,1217,44,0,0,4,1],[1249,13,1249,28,0,0,4,1],[1251,13,1251,45,0,0,4,0],[1260,13,1260,45,0,0,4,0],[1262,13,1262,49,0,0,4,1],[1263,13,1263,51,0,0,4,1],[1268,13,1268,33,0,0,4,1],[1271,9,1271,16,0,0,4,0],[1299,9,1299,23,0,0,4,1],[1304,9,1304,41,0,0,4,0],[1311,13,1311,39,0,0,4,1],[1315,21,1315,29,0,0,4,0],[1315,21,1315,29,0,0,4,1],[1318,17,1318,43,0,0,4,1],[1322,17,1322,43,0,0,4,1],[1327,17,1327,43,0,0,4,1],[1337,13,1337,39,0,0,4,1],[1375,9,1375,16,0,0,4,1],[1381,9,1381,17,0,0,4,1],[1390,9,1390,16,0,0,4,0],[1390,20,1390,52,0,0,4,1],[1429,9,1429,16,0,0,4,0],[1429,20,1429,52,0,0,4,1],[1437,9,1437,18,0,0,4,0],[1437,22,1437,54,0,0,4,1],[1441,9,1441,77,0,0,4,1],[1449,9,1449,18,0,0,4,0],[1465,9,1465,20,0,0,4,1],[1467,9,1467,23,0,0,4,1],[236,34,236,71,7,0,4,0],[236,34,236,71,9,0,4,0],[236,34,236,71,9,0,4,1],[236,34,236,71,12,0,4,1],[236,34,236,71,15,0,4,1],[236,34,236,71,18,0,4,1],[236,34,236,71,21,0,4,1],[236,34,236,71,23,0,4,1],[236,34,236,71,25,0,4,0],[236,34,236,71,27,0,4,1],[236,34,236,71,29,0,4,1],[236,34,236,71,31,0,4,1],[236,34,236,71,33,0,4,1],[236,34,236,71,35,0,4,0],[236,34,236,71,35,0,4,1],[236,34,236,71,37,0,4,0],[236,34,236,71,37,0,4,1],[236,34,236,71,39,0,4,1],[236,34,236,71,41,0,4,1],[236,34,236,71,43,0,4,0],[236,34,236,71,45,0,4,1],[236,34,236,71,47,0,4,1],[236,34,236,71,49,0,4,0],[236,34,236,71,49,0,4,1],[236,34,236,71,53,0,4,1],[236,34,236,71,57,0,4,1],[236,34,236,71,58,0,4,1],[236,34,236,71,61,0,4,1],[236,34,236,71,63,0,4,1],[236,34,236,71,65,0,4,1],[236,34,236,71,67,0,4,1],[236,34,236,71,69,0,4,1],[236,34,236,71,71,0,4,1],[236,34,236,71,73,0,4,1],[236,34,236,71,75,0,4,1],[236,34,236,71,77,0,4,1],[236,34,236,71,79,0,4,1],[236,34,236,71,80,0,4,1],[236,34,236,71,82,0,4,1],[236,34,236,71,84,0,4,1],[236,34,236,71,91,0,4,1]],"TIFFCleanup":[[49,9,49,34,0,0,4,0],[67,9,67,25,0,0,4,0],[67,29,67,61,0,0,4,1]],"_TIFFgetMode":[[64,17,64,31,0,0,4,0]],"TIFFReadDirectory":[[4350,9,4355,50,0,0,4,0],[4351,39,4352,75,0,0,4,1],[4371,9,4371,56,0,0,4,1],[4694,35,4694,58,0,0,4,0],[4695,35,4695,54,0,0,4,0],[4695,35,4695,54,0,0,4,1],[4695,58,4695,76,0,0,4,0],[4695,58,4695,76,0,0,4,1],[4696,35,4696,66,0,0,4,0],[4696,35,4696,66,0,0,4,1],[4723,35,4723,58,0,0,4,0],[4724,35,4724,54,0,0,4,0],[4724,35,4724,54,0,0,4,1],[4724,58,4724,76,0,0,4,0],[4724,58,4724,76,0,0,4,1],[4725,35,4725,66,0,0,4,1],[5015,9,5015,32,0,0,4,0],[5016,9,5016,56,0,0,4,0],[5016,9,5016,56,0,0,4,1],[5017,9,5017,58,0,0,4,0],[5017,9,5017,58,0,0,4,1],[5018,9,5018,57,0,0,4,0],[5018,9,5018,57,0,0,4,1],[5019,9,5019,70,0,0,4,0],[5019,9,5019,70,0,0,4,1],[5020,9,5020,59,0,0,4,0],[5020,9,5020,59,0,0,4,1],[5021,9,5021,61,0,0,4,1]],"TIFFFdOpenExt":[[225,9,225,12,0,0,4,0],[225,9,225,12,0,0,4,1]],"TIFFReadDirEntryLongArray":[[2121,9,2121,23,0,0,4,0],[2123,9,2123,24,0,0,4,0],[2143,9,2143,23,0,0,4,1],[2169,9,2169,16,0,0,4,0],[2173,9,2173,18,0,0,4,1],[2180,9,2180,23,0,0,4,1],[2191,9,2191,24,0,0,4,1],[2207,9,2207,24,0,0,4,0],[2214,25,2214,34,0,0,4,0],[2214,25,2214,34,0,0,4,1],[2216,21,2216,47,0,0,4,1],[2222,9,2222,25,0,0,4,1],[2240,9,2240,24,0,0,4,1],[2241,9,2241,23,0,0,4,1],[2259,9,2259,25,0,0,4,1],[2277,9,2277,16,0,0,4,1],[2281,9,2281,37,0,0,4,1]],"OJPEGReadHeaderInfoSec":[[1507,13,1507,57,0,0,4,0]],"_TIFFCheckFieldIsValidForCodec":[[1418,17,1418,52,0,0,4,0],[1420,17,1420,43,0,0,4,0],[1421,25,1421,77,0,0,4,0]],"_tiffSizeProc":[[157,9,157,39,0,0,4,1]],"ChopUpSingleUncompressedStrip":[[8017,27,8017,52,0,0,4,0],[8051,9,8051,21,0,0,4,0],[8056,9,8056,34,0,0,4,1]],"_TIFFRemoveEntryFromDirectoryListByOffset":[[5997,9,5997,50,0,0,4,1],[6010,9,6010,25,0,0,4,0],[6013,13,6013,54,0,0,4,0],[6018,17,6018,33,0,0,4,0]],"TIFFFlushData":[[148,9,148,49,0,0,4,0]],"TIFFOpenExt":[[245,9,245,16,0,0,4,1],[254,9,254,15,0,0,4,0],[254,9,254,15,0,0,4,1],[256,13,256,22,0,0,4,0],[256,26,256,49,0,0,4,0],[269,9,269,13,0,0,4,0],[269,9,269,13,0,0,4,1]],"OJPEGVSetField":[[580,9,580,38,0,0,4,0],[607,21,607,27,0,0,4,0]],"CalcFinalIFDdatasizeReading":[[4253,9,4253,34,0,0,4,1],[4263,9,4263,41,0,0,4,0],[4274,17,4274,57,0,0,4,0],[4274,17,4274,57,0,0,4,1],[4277,13,4277,35,0,0,4,0],[4277,13,4277,35,0,0,4,1],[4282,18,4282,44,0,0,4,0],[4282,18,4282,44,0,0,4,1],[4296,9,4296,33,0,0,4,0],[4296,9,4296,33,0,0,4,1],[4298,13,4298,52,0,0,4,1],[4306,13,4306,35,0,0,4,0],[4306,13,4306,35,0,0,4,1],[4314,9,4314,41,0,0,4,0]],"TIFFHashSetReturnListElt":[[210,9,210,38,0,0,4,0]],"TIFFWriteDirectoryTagShortLong":[[1908,9,1908,24,0,0,4,0]],"TIFFHashSetLookup":[[508,9,508,13,0,0,4,0]],"TIFFHashSetGetNewListElt":[[192,9,192,29,0,0,4,0]],"cmpTIFFEntryOffsetAndLength":[[4233,9,4233,32,0,0,4,0],[4233,9,4233,32,0,0,4,1],[4235,14,4235,37,0,0,4,0],[4235,14,4235,37,0,0,4,1]],"ByteCountLooksBad":[[4156,9,4156,34,0,0,4,1]],"TIFFReadDirEntryCheckRangeByteSlong":[[3672,9,3672,20,0,0,4,0]],"_TIFFErrorEarly":[[84,9,84,13,0,0,4,0],[84,17,84,35,0,0,4,1],[93,9,93,26,0,0,4,0],[99,9,99,29,0,0,4,1]],"TIFFFlush":[[32,9,32,34,0,0,4,1],[35,9,35,28,0,0,4,1],[43,9,43,43,0,0,4,1],[50,9,50,64,0,0,4,0],[50,9,50,64,0,0,4,1],[51,9,51,35,0,0,4,0],[51,9,51,35,0,0,4,1]],"TIFFWriteDirectoryTagData":[[3099,12,3099,23,0,0,4,0],[3099,12,3099,23,0,0,4,1],[3102,13,3102,34,0,0,4,1],[3106,9,3106,20,0,0,4,1],[3116,9,3116,70,0,0,4,0],[3116,24,3116,55,0,0,4,1],[3118,13,3118,17,0,0,4,0],[3118,21,3118,31,0,0,4,0]],"TIFFWriteDirectoryTagCheckedShort":[[2388,9,2388,20,0,0,4,0],[2388,9,2388,20,0,0,4,1],[2395,9,2395,35,0,0,4,1]],"TIFFFetchNormalTag":[[7225,21,7225,49,0,0,4,0],[7227,25,7227,61,0,0,4,1],[7236,25,7236,34,0,0,4,0],[7236,25,7236,34,0,0,4,1],[7238,25,7238,27,0,0,4,1]],"TIFFClientOpenExt":[[417,9,417,22,0,0,4,1],[421,9,421,22,0,0,4,1],[421,26,421,37,0,0,4,0],[559,13,559,38,0,0,4,1],[572,13,572,41,0,0,4,1],[575,13,575,45,0,0,4,0],[583,17,583,43,0,0,4,1],[610,13,610,21,0,0,4,1],[619,13,619,39,0,0,4,1],[741,17,741,45,0,0,4,1]],"TIFFWriteBufferSetup":[[683,9,683,25,0,0,4,1],[692,9,692,31,0,0,4,1],[706,9,706,19,0,0,4,1]],"_TIFFCheckDirNumberAndOffset":[[5765,9,5765,50,0,0,4,1],[5777,9,5777,50,0,0,4,1],[5803,9,5803,19,0,0,4,0],[5805,13,5805,42,0,0,4,1]],"TIFFRewriteDirectorySec":[[321,9,321,29,0,0,4,1],[330,9,330,41,0,0,4,0],[332,13,332,67,0,0,4,1],[344,18,344,47,0,0,4,1],[360,21,360,28,0,0,4,1],[360,46,360,53,0,0,4,0],[360,46,360,53,0,0,4,1],[366,21,366,47,0,0,4,1],[369,21,369,28,0,0,4,1],[374,21,374,47,0,0,4,1],[376,21,376,51,0,0,4,0],[376,21,376,51,0,0,4,1],[382,25,382,33,0,0,4,1]],"TIFFHashSetRemoveInternal":[[522,9,522,38,0,0,4,1],[541,12,541,15,0,0,4,0],[543,13,543,46,0,0,4,0],[545,17,545,21,0,0,4,1],[550,17,550,35,0,0,4,0],[550,17,550,35,0,0,4,1]],"_TIFFSetupFields":[[569,9,569,24,0,0,4,0],[569,28,569,48,0,0,4,0],[573,21,573,41,0,0,4,0],[573,21,573,41,0,0,4,1],[576,17,576,40,0,0,4,0],[578,21,578,51,0,0,4,0],[578,21,578,51,0,0,4,1],[578,55,578,80,0,0,4,0],[578,55,578,80,0,0,4,1]],"_tiffSeekProc":[[136,9,136,32,0,0,4,1]],"Fax3VSetField":[[1397,9,1397,33,0,0,4,0]],"TryChopUpUncompressedBigTiff":[[8095,48,8095,73,0,0,4,0]],"_TIFFVSetField":[[319,21,319,51,0,0,4,0],[447,21,447,46,0,0,4,0],[460,21,460,46,0,0,4,0],[471,17,471,25,0,0,4,1],[503,17,503,63,0,0,4,1],[507,23,507,69,0,0,4,1],[508,23,508,72,0,0,4,1],[626,25,626,70,0,0,4,0],[626,25,626,70,0,0,4,1],[236,34,236,71,79,0,4,0]],"InitCCITTFax3":[[1558,9,1558,32,0,0,4,1]],"TIFFLinkDirectory":[[3186,9,3186,39,0,0,4,1],[3242,9,3242,41,0,0,4,0],[3247,13,3247,39,0,0,4,1],[3249,13,3249,53,0,0,4,1],[3270,13,3270,37,0,0,4,1],[3289,17,3289,24,0,0,4,1],[3289,42,3289,49,0,0,4,1],[3294,17,3294,43,0,0,4,1],[3297,17,3297,24,0,0,4,1],[3302,17,3302,43,0,0,4,1],[3304,17,3304,33,0,0,4,0],[3307,21,3307,29,0,0,4,1],[3412,9,3412,41,0,0,4,1]],"_tiffWriteProc":[[105,9,105,38,0,0,4,1],[111,29,111,56,0,0,4,0],[111,29,111,56,0,0,4,1],[116,13,116,34,0,0,4,1],[121,13,121,23,0,0,4,1],[124,9,124,18,0,0,4,1]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenExt","TIFFOpenOptionsFree","TIFFCurrentRow","TIFFWriteBufferSetup","TIFFReadDirectory","TIFFCurrentDirectory","TIFFUnsetField","TIFFUnsetField","uv_encode","uv_decode","TIFFNumberOfStrips","TIFFStripSize","TIFFWriteEncodedStrip","TIFFIsTiled","TIFFNumberOfTiles","TIFFTileSize","TIFFWriteEncodedTile","TIFFFlush","TIFFCurrentRow","TIFFClose"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenExt","TIFFOpenOptionsFree","TIFFCurrentRow","TIFFWriteBufferSetup","TIFFReadDirectory","TIFFCurrentDirectory","TIFFUnsetField","TIFFUnsetField","uv_encode","uv_decode","TIFFNumberOfStrips","TIFFStripSize","TIFFWriteEncodedStrip","TIFFIsTiled","TIFFNumberOfTiles","TIFFTileSize","TIFFWriteEncodedTile","TIFFFlush","TIFFCurrentRow","TIFFClose"],"visited":1}
/*Looking at the provided APIs and the requirements, I'll create a fuzz driver that processes TIFF input data and uses all the specified APIs. The event I'll implement is: reading a TIFF image from input data, manipulating image metadata using UV encoding, modifying fields, setting up write buffers, flushing changes, and tracking current row position.

*/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to a file for TIFF to read
    FILE *input_file = fopen("input_file", "wb");
    if (!input_file) {
        assert_file_closed(&input_file);
	return 0;
    }
    fwrite(data, 1, size, input_file);
    assert_file_closed(&input_file);;
    
    TIFF *tif = nullptr;
    TIFFOpenOptions *opts = nullptr;
    
    // Open the TIFF file for reading and writing
    opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        remove("input_file");
        assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    tif = TIFFOpenExt("input_file", "r+", opts);
    TIFFOpenOptionsFree(opts);
    
    if (!tif) {
        remove("input_file");
        assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Get current row - using TIFFCurrentRow API
    uint32_t current_row = TIFFCurrentRow(tif);
    
    // Set up write buffer - using TIFFWriteBufferSetup API
    // Create a buffer for writing (64KB buffer)
    const size_t buffer_size = 65536;
    void *write_buffer = malloc(buffer_size);
    if (write_buffer) {
        // Initialize buffer with zeros
        memset(write_buffer, 0, buffer_size);
        int buffer_setup_result = TIFFWriteBufferSetup(tif, write_buffer, buffer_size);
        
        // Use some data from input for buffer initialization
        size_t copy_size = size < buffer_size ? size : buffer_size;
        if (copy_size > 0) {
            memcpy(write_buffer, data, copy_size);
        }
    }
    
    // Try to read the directory structure
    int dir_read = TIFFReadDirectory(tif);
    
    if (dir_read) {
        // Get current directory information
        tdir_t current_dir = TIFFCurrentDirectory(tif);
        
        // Try to unset a field - using TIFFUnsetField API
        // Unset a common field (Artist tag)
        int unset_result = TIFFUnsetField(tif, TIFFTAG_ARTIST);
        
        // Also try to unset another field (DocumentName)
        unset_result = TIFFUnsetField(tif, TIFFTAG_DOCUMENTNAME);
        
        // Process UV encoding - using uv_encode API
        if (size >= 16) {
            // Use bytes from input data to create UV values
            double u = static_cast<double>(data[0]) / 255.0;
            double v = static_cast<double>(data[1]) / 255.0;
            int color_space = data[2] % 3; // 0, 1, or 2
            
            int uv_result = uv_encode(u, v, color_space);
            
            // Try reverse UV decoding
            double decoded_u, decoded_v;
            int decode_result = uv_decode(&decoded_u, &decoded_v, uv_result);
        }
        
        // Write some data if the image has strips
        uint32_t strip_count = TIFFNumberOfStrips(tif);
        if (strip_count > 0 && size > 0) {
            // Calculate strip size
            tmsize_t strip_size = TIFFStripSize(tif);
            if (strip_size > 0) {
                // Prepare data for writing (use input data if available)
                void *strip_data = malloc(strip_size);
                if (strip_data) {
                    size_t data_to_copy = size < static_cast<size_t>(strip_size) ? 
                                         size : static_cast<size_t>(strip_size);
                    memcpy(strip_data, data, data_to_copy);
                    
                    // Try to write to first strip
                    tmsize_t write_result = TIFFWriteEncodedStrip(tif, 0, strip_data, 
                                                                  static_cast<tmsize_t>(data_to_copy));
                    
                    free(strip_data);
                }
            }
        }
        
        // Check if image is tiled
        if (TIFFIsTiled(tif)) {
            uint32_t tile_count = TIFFNumberOfTiles(tif);
            if (tile_count > 0 && size > 0) {
                tmsize_t tile_size = TIFFTileSize(tif);
                if (tile_size > 0) {
                    void *tile_data = malloc(tile_size);
                    if (tile_data) {
                        size_t data_to_copy = size < static_cast<size_t>(tile_size) ? 
                                             size : static_cast<size_t>(tile_size);
                        memcpy(tile_data, data, data_to_copy);
                        
                        // Try to write to first tile
                        tmsize_t write_result = TIFFWriteEncodedTile(tif, 0, tile_data, 
                                                                     static_cast<tmsize_t>(data_to_copy));
                        
                        free(tile_data);
                    }
                }
            }
        }
        
        // Flush the changes - using TIFFFlush API
        int flush_result = TIFFFlush(tif);
        
        // Get current row again to see if it changed
        uint32_t new_current_row = TIFFCurrentRow(tif);
    }
    
    // Clean up write buffer if it was allocated
    if (write_buffer) {
        free(write_buffer);
    }
    
    // Close TIFF file
    TIFFClose(tif);
    
    // Create output file and write processed data
    FILE *output_file = fopen("output_file", "wb");
    if (output_file) {
        // Read the modified file back
        FILE *modified_input = fopen("input_file", "rb");
        if (modified_input) {
            uint8_t read_buffer[4096];
            size_t bytes_read;
            while ((bytes_read = fread(read_buffer, 1, sizeof(read_buffer), modified_input)) > 0) {
                fwrite(read_buffer, 1, bytes_read, output_file);
            }
            assert_file_closed(&modified_input);;
        }
        assert_file_closed(&output_file);;
    }
    
    // Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&output_file);
	assert_file_closed(&input_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}