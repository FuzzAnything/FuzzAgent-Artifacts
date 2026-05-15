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
//<ID> 241
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 2
//<Quality> {"density":13,"unique_branches":{"TIFFWarning":[[49,9,49,28,0,0,4,0],[55,9,55,31,0,0,4,1]]},"library_calls":["TIFFGetVersion","TIFFWarning","TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarningHandlerExtR","TIFFOpenOptionsSetWarnAboutUnknownTags","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFCIELabToRGBInit","TIFFXYZToRGB","_TIFFmalloc","TIFFGetField","TIFFGetField","TIFFWriteTile","_TIFFfree","TIFFRGBAImageEnd","TIFFGetCloseProc","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFGetVersion","TIFFWarning","TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarningHandlerExtR","TIFFOpenOptionsSetWarnAboutUnknownTags","TIFFClientOpenExt","TIFFCIELabToRGBInit","TIFFXYZToRGB","_TIFFmalloc","TIFFGetField","TIFFGetField","TIFFWriteTile","_TIFFfree","TIFFRGBAImageEnd","TIFFGetCloseProc","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/**/


extern "C" {
}

// Forward declarations for missing APIs
extern "C" tmsize_t TIFFWriteTile(TIFF *tif, void *buf, uint32_t x, uint32_t y, uint32_t z, uint16_t s);
extern "C" TIFFCloseProc TIFFGetCloseProc(TIFF *);
extern "C" void TIFFOpenOptionsSetWarningHandlerExtR(TIFFOpenOptions *opts, TIFFErrorHandlerExtR handler, void *warnhandler_user_data);
extern "C" void TIFFXYZToRGB(TIFFCIELabToRGB *, float, float, float, uint32_t *, uint32_t *, uint32_t *);
extern "C" const char *TIFFGetVersion();

// Custom warning handler to use with TIFFOpenOptionsSetWarningHandlerExtR
static int  id_000005_CustomWarningHandler(TIFF *tif, void *user_data, const char *module, const char *fmt, va_list ap) {
    // Just forward to default handler for this fuzzer
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
        return 0;  // Too small to be meaningful TIFF data
    }

    // 1. Write input data to a temporary file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // 2. Get libtiff version and issue warning
    const char *version = TIFFGetVersion();
    if (version) {
        TIFFWarning("Fuzzer", "Using libtiff version: %s", version);
    }

    // 3. Create TIFFOpenOptions and set warning handler
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    TIFFOpenOptionsSetWarningHandlerExtR(opts,  id_000005_CustomWarningHandler, nullptr);
    TIFFOpenOptionsSetWarnAboutUnknownTags(opts, 1);
	int fuzz_fd_0 = fuzz_fileno(fopen("input_file", "rb"));


    // 4. Open the input file as TIFF
    TIFF *tif = TIFFClientOpenExt("input_file", "rm", 
                                  (thandle_t)fuzz_fd_0,
                                  nullptr, nullptr, nullptr, nullptr,
                                  nullptr, nullptr, nullptr, opts);
    if (!tif) {
        TIFFOpenOptionsFree(opts);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }

    // 5. Initialize CIELab to RGB converter
    TIFFCIELabToRGB cielab;
    TIFFDisplay display;
    float refWhite[3] = {1.0f, 1.0f, 1.0f};
    int initResult = TIFFCIELabToRGBInit(&cielab, &display, refWhite);
    
    if (initResult) {
        // 6. Convert some XYZ values to RGB using the converter
        float X = 0.5f, Y = 0.5f, Z = 0.5f;
        uint32_t r, g, b;
        TIFFXYZToRGB(&cielab, X, Y, Z, &r, &g, &b);
        
        // 7. Create a simple tile buffer
        uint32_t tileWidth = 64;
        uint32_t tileHeight = 64;
        uint32_t samplesPerPixel = 3;
        size_t tileSize = tileWidth * tileHeight * samplesPerPixel;
        
        void *tileBuffer = _TIFFmalloc(tileSize);
        if (tileBuffer) {
            memset(tileBuffer, 0, tileSize);
            
            // 8. Try to write a tile (if image supports tiles)
            uint32_t imageWidth, imageHeight;
            if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &imageWidth) &&
                TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imageHeight)) {
                
                if (imageWidth > 0 && imageHeight > 0) {
                    // Write tile at position (0,0,0) for sample 0
                    TIFFWriteTile(tif, tileBuffer, 0, 0, 0, 0);
                }
            }
            
            _TIFFfree(tileBuffer);
        }
    }

    // 9. Create and end RGBA image
    TIFFRGBAImage rgba;
    TIFFRGBAImageEnd(&rgba);

    // 10. Get close procedure (though we'll use TIFFClose)
    TIFFCloseProc closeProc = TIFFGetCloseProc(tif);

    // 11. Cleanup
    TIFFClose(tif);
    TIFFOpenOptionsFree(opts);
    
    // Clean up temporary files
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fuzz_fd_0);
	return 0;
}