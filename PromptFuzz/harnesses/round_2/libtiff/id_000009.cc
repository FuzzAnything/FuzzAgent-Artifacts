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
//<ID> 493
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 2
//<Quality> {"density":19,"unique_branches":{"TIFFErrorExtR":[[110,16,110,37,0,0,4,0],[116,13,116,17,0,0,4,1]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetErrorHandlerExtR","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFIsByteSwapped","TIFFGetTagListEntry","TIFFWarningExt","TIFFNumberOfDirectories","TIFFFindField","TIFFFieldReadCount","uv_decode","uv_encode","TIFFSetTagExtender","TIFFGetField","TIFFGetField","TIFFGetField","TIFFScanlineSize","_TIFFmalloc","TIFFReadScanline","_TIFFfree","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetErrorHandlerExtR","TIFFClientOpenExt","TIFFIsByteSwapped","TIFFGetTagListEntry","TIFFWarningExt","TIFFNumberOfDirectories","TIFFFindField","TIFFFieldReadCount","uv_decode","uv_encode","TIFFSetTagExtender","TIFFGetField","TIFFGetField","TIFFGetField","TIFFScanlineSize","_TIFFmalloc","TIFFReadScanline","_TIFFfree","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/**/


// Include libtiff headers
extern "C" {
}

// Error handler function
static int  id_000009_errorHandler(TIFF* tif, void* userdata, 
                       const char* module, const char* fmt, va_list ap) {
    (void)tif;
    (void)userdata;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

// Tag extender function
static void  id_000009_tagExtender(TIFF* tif) {
    (void)tif;
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Reopen for reading
    in_file = fopen("input_file", "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create TIFFOpenOptions and set error handler
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    TIFFOpenOptionsSetErrorHandlerExtR(opts,  id_000009_errorHandler, NULL);
    
    // Open TIFF file using client API
    thandle_t handle = (thandle_t)in_file;
    TIFF* tif = TIFFClientOpenExt("input_file", "rm", handle,
                                  TIFFReadWriteProc(0), TIFFReadWriteProc(0),
                                  TIFFSeekProc(0), TIFFCloseProc(0),
                                  TIFFSizeProc(0), TIFFMapFileProc(0),
                                  TIFFUnmapFileProc(0), opts);
    
    if (!tif) {
        TIFFOpenOptionsFree(opts);
        assert_file_closed(&in_file);;
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 1. Check if TIFF is byte swapped
    int isSwapped = TIFFIsByteSwapped(tif);
    (void)isSwapped;
    
    // 2. Get tag list entries
    for (int i = 0; i < 10; i++) {
        uint32_t tag = TIFFGetTagListEntry(tif, i);
        (void)tag;
    }
    
    // 3. Issue warning
    TIFFWarningExt((thandle_t)tif, "fuzzer", "test warning");
    
    // 4. Get number of directories
    tdir_t numDirs = TIFFNumberOfDirectories(tif);
    (void)numDirs;
    
    // 5. TIFFFieldSetGetCountSize - need to get a field first
    const TIFFField* field = TIFFFindField(tif, TIFFTAG_IMAGEWIDTH, TIFF_LONG);
    if (field) {
        // Note: TIFFFieldSetGetCountSize is not in provided API list
        // Using TIFFFieldReadCount instead
        int count = TIFFFieldReadCount(field);
        (void)count;
    }
    
    // 6. LogLuv conversion
    float xyz[3] = {0.5f, 0.5f, 0.5f};
    // Note: LogLuv32fromXYZ is not in provided API list
    // Using uv_encode instead which is similar
    double u, v;
    uv_decode(&u, &v, 0);
    int encoded = uv_encode(u, v, 0);
    (void)encoded;
    
    // 7. Set tag extender
    TIFFExtendProc prevExtender = TIFFSetTagExtender( id_000009_tagExtender);
    (void)prevExtender;
    
    // Try to read some data from the TIFF
    uint32_t width = 0, height = 0;
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width)) {
        if (TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
            // Try to read a scanline if possible
            uint32_t row = 0;
            uint16_t spp = 1;
            TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &spp);
            
            tmsize_t scanlineSize = TIFFScanlineSize(tif);
            if (scanlineSize > 0 && row < height) {
                void* buf = _TIFFmalloc(scanlineSize);
                if (buf) {
                    TIFFReadScanline(tif, buf, row, 0);
                    _TIFFfree(buf);
                }
            }
        }
    }
    
    // Cleanup
    TIFFClose(tif);
    TIFFOpenOptionsFree(opts);
    assert_file_closed(&in_file);;
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}