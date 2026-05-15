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
//<ID> 1444
//<Prompt> []
/*<Combination>: [
*/
//<score> 130, nr_unique_branch: 30
//<Quality> {"density":10,"unique_branches":{"_TIFFRemoveEntryFromDirectoryListByOffset":[[6010,9,6010,25,0,0,4,1]],"TIFFWriteDirectorySec":[[849,21,851,58,0,0,4,1],[1026,25,1028,75,0,0,4,0],[1262,13,1262,49,0,0,4,0],[236,34,236,71,75,0,4,0]],"TIFFTileRowSize64":[[147,9,147,31,0,0,4,0]],"TIFFWriteRawStrip":[[346,9,346,32,0,0,4,1],[367,9,367,35,0,0,4,0],[375,9,375,35,0,0,4,1],[381,13,381,63,0,0,4,0],[381,13,381,63,0,0,4,1],[38,6,38,43,1,0,4,1],[38,47,38,79,1,0,4,0]],"OJPEGVGetField":[[554,9,554,41,0,0,4,0]],"TIFFAppendToStrip":[[809,17,809,24,0,0,4,0],[841,9,841,30,0,0,4,0]],"_TIFFRewriteField":[[3487,13,3487,20,0,0,4,0],[3517,12,3517,24,0,0,4,1],[3536,9,3536,25,0,0,4,0]],"TIFFWriteDirectoryTagData":[[3130,13,3130,22,0,0,4,0]],"TIFFSwabArrayOfDouble":[[228,12,228,19,0,0,4,0],[228,12,228,19,0,0,4,1]],"TIFFRegisterCODEC":[[244,9,244,19,0,0,4,0]],"TIFFUnRegisterCODEC":[[270,35,270,54,0,0,4,0],[271,13,271,26,0,0,4,0]],"OJPEGVSetField":[[587,9,587,33,0,0,4,0],[589,17,589,24,0,0,4,0],[591,21,591,27,0,0,4,1],[599,29,599,35,0,0,4,0],[599,29,599,35,0,0,4,1]]},"library_calls":["TIFFOpen","TIFFGetField","TIFFGetField","TIFFTileRowSize64","TIFFSwabArrayOfDouble","TIFFIsTiled","TIFFNumberOfStrips","TIFFStripSize","TIFFWriteRawStrip","TIFFRewriteDirectory","TIFFRegisterCODEC","TIFFUnRegisterCODEC","TIFFClose"],"critical_calls":["TIFFOpen","TIFFGetField","TIFFGetField","TIFFTileRowSize64","TIFFSwabArrayOfDouble","TIFFIsTiled","TIFFNumberOfStrips","TIFFStripSize","TIFFWriteRawStrip","TIFFRewriteDirectory","TIFFRegisterCODEC","TIFFUnRegisterCODEC","TIFFClose"],"visited":0}
/*We are going to write a fuzz driver that processes TIFF input data, uses specified APIs, and ensures resource cleanup. Since the input is a byte stream, we'll write it to a temporary file for TIFF to process.

The event we're achieving: Reading a TIFF file from input data, extracting tile information, swapping double array data, writing raw strip data, rewriting the directory, and unregistering a codec.

Here's the complete fuzz driver:

*/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<19) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to a file for TIFF to read
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Open the TIFF file for reading and writing
    TIFF *tif = TIFFOpen("input_file", fuzz_str_1);
    if (!tif) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 1. Use TIFFVGetFieldDefaulted (via wrapper) to get a field with default
    uint32_t width = 0;
    uint32_t height = 0;
    // Since TIFFVGetFieldDefaulted isn't in provided list, we'll use TIFFGetFieldDefaulted
    // which is available. For demonstration, we'll get image dimensions.
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width)) {
        // Got width
    }
    if (TIFFGetField(tif, fuzz_uint32_t_2, &height)) {
        // Got height
    }

    // 2. Use TIFFTileRowSize64 - get tile row size if tiled
    uint64_t tileRowSize = TIFFTileRowSize64(tif);

    // 3. Use TIFFSwabArrayOfDouble - swap byte order of double array
    // Create a small double array from input data for demonstration
    if (size >= sizeof(double) * 2) {
        double doubleArray[2];
        memcpy(doubleArray, data, sizeof(double) * 2);
        TIFFSwabArrayOfDouble(doubleArray, 2);
        // We could write this back to the file if needed
    }

    // 4. Use TIFFWriteRawStrip - write raw strip data
    // First check if image is organized in strips
    if (TIFFIsTiled(tif) == 0) {  // Not tiled, so strip-organized
        uint32_t stripCount = TIFFNumberOfStrips(tif);
        if (stripCount > 0) {
            // Create some dummy data to write (based on input)
            tmsize_t stripSize = TIFFStripSize(tif);
            if (stripSize > 0 && size >= (size_t)stripSize) {
                void *stripData = malloc(stripSize);
                if (stripData) {
                    memcpy(stripData, data, stripSize);
                    TIFFWriteRawStrip(tif, 0, stripData, stripSize);
                    free(stripData);
                }
            }
        }
    }

    // 5. Use TIFFRewriteDirectory - rewrite directory with updated information
    // We'll rewrite the directory to persist changes
    TIFFRewriteDirectory(tif);

    // 6. Use TIFFUnRegisterCODEC - unregister a codec (register first if needed)
    // Register a dummy codec first, then unregister it
    TIFFCodec *dummyCodec = (TIFFCodec *)malloc(sizeof(TIFFCodec));
    if (dummyCodec) {
        dummyCodec->name = "DUMMY";
        dummyCodec->scheme = 65535;  // High scheme value unlikely to conflict
        dummyCodec->init = NULL;
        // Note: TIFFRegisterCODEC is in provided APIs
        TIFFCodec *registered = TIFFRegisterCODEC(fuzz_uint16_t_4, fuzz_str_3, NULL);
        if (registered) {
            TIFFUnRegisterCODEC(registered);
        }
        free(dummyCodec);
    }

    // Clean up - close TIFF and remove temporary files
    TIFFClose(tif);
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}