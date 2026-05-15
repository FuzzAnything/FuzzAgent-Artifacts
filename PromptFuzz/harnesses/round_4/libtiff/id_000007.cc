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
//<ID> 171
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":6,"unique_branches":{"TIFFUnlinkDirectory":[[2237,9,2237,34,0,0,4,0]],"_TIFFVGetField":[[1508,29,1508,43,0,0,4,0]]},"library_calls":["TIFFClientOpen","TIFFIsByteSwapped","TIFFIsMSB2LSB","TIFFUnlinkDirectory","TIFFPrintDirectory","TIFFClose"],"critical_calls":["TIFFClientOpen","TIFFIsByteSwapped","TIFFIsMSB2LSB","TIFFUnlinkDirectory","TIFFPrintDirectory","TIFFClose"],"visited":0}
/**/


// Callback functions for TIFFClientOpen
static tmsize_t  id_000007_myRead(thandle_t handle, void* buf, tmsize_t size) {
    return fread(buf, 1, size, (FILE*)handle);
}

static tmsize_t  id_000007_myWrite(thandle_t handle, void* buf, tmsize_t size) {
    return fwrite(buf, 1, size, (FILE*)handle);
}

static toff_t  id_000007_mySeek(thandle_t handle, toff_t offset, int whence) {
    fseek((FILE*)handle, offset, whence);
    return ftell((FILE*)handle);
}

static int  id_000007_myClose(thandle_t handle) {
    return fclose((FILE*)handle);
}

static toff_t  id_000007_mySize(thandle_t handle) {
    long current = ftell((FILE*)handle);
    fseek((FILE*)handle, 0, SEEK_END);
    long size = ftell((FILE*)handle);
    fseek((FILE*)handle, current, SEEK_SET);
    return size;
}

static int  id_000007_myMapFile(thandle_t, void**, toff_t*) { return 0; }
static void  id_000007_myUnmapFile(thandle_t, void*, toff_t) {}

// Helper function for TIFFVGetFieldDefaulted
static int  id_000007_myTIFFGetFieldDefaulted(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    int ret;
    va_start(ap, tag);
    ret = TIFFVGetFieldDefaulted(tif, tag, ap);
    va_end(ap);
    return ret;
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to file
    FILE* in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Open the file for reading
    in_file = fopen("input_file", "rb");
    if (!in_file) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Create TIFF handle using client API
    TIFF* tif = TIFFClientOpen(fuzz_str_1, fuzz_str_2, 
                               (thandle_t)in_file,
                                id_000007_myRead,  id_000007_myWrite,
                                id_000007_mySeek,  id_000007_myClose,
                                id_000007_mySize,
                                id_000007_myMapFile,  id_000007_myUnmapFile);
    
    if (tif) {
        // 1. Check if TIFF is byte-swapped
        int isByteSwapped = TIFFIsByteSwapped(tif);
        
        // 2. Check if TIFF is MSB to LSB
        int isMSB2LSB = TIFFIsMSB2LSB(tif);
        
        // 3. Get a field with default value (using ImageWidth tag 256)
        uint32_t width = 0;
        int hasWidth =  id_000007_myTIFFGetFieldDefaulted(tif, 256, &width);
        
        // 4. Try to unlink directory 0 if it exists
        TIFFUnlinkDirectory(tif, fuzz_uint32_t_3);
        
        // 5. Print directory to output file
        FILE* out_file = fopen("output_file", "w");
        if (out_file) {
            TIFFPrintDirectory(tif, out_file, fuzz_int64_t_4);
            assert_file_closed(&out_file);;
            remove("output_file");
        }
        
        // Close the TIFF file (which also closes the underlying FILE)
        TIFFClose(tif);
    } else {
        // If TIFFClientOpen failed, close the file manually
        assert_file_closed(&in_file);;
    }
    
    // Clean up temporary files
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}