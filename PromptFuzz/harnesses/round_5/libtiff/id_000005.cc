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
//<ID> 378
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 3
//<Quality> {"density":9,"unique_branches":{"TIFFUnRegisterCODEC":[[270,35,270,54,0,0,4,0],[271,13,271,26,0,0,4,0]],"TIFFRegisterCODEC":[[244,9,244,19,0,0,4,0]]},"library_calls":["TIFFClientOpen","TIFFClientdata","TIFFRegisterCODEC","TIFFIsUpSampled","TIFFFieldWithTag","TIFFFieldName","TIFFGetField","TIFFVStripSize","TIFFGetUnmapFileProc","TIFFUnRegisterCODEC","TIFFClose"],"critical_calls":["TIFFClientOpen","TIFFClientdata","TIFFRegisterCODEC","TIFFIsUpSampled","TIFFFieldWithTag","TIFFFieldName","TIFFGetField","TIFFVStripSize","TIFFGetUnmapFileProc","TIFFUnRegisterCODEC","TIFFClose"],"visited":0}
/**/


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





    // 1. Write input data to a temporary file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // 2. Reopen the file for reading with libtiff
    in_file = fopen("input_file", "rb");
    if (!in_file) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 3. Get file descriptor for TIFF client operations
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Define custom I/O functions for TIFFClientOpen
    auto readproc = [](thandle_t handle, void* buf, tmsize_t size) -> tmsize_t {
        return fread(buf, 1, size, (FILE*)handle);
    };
    
    auto writeproc = [](thandle_t handle, void* buf, tmsize_t size) -> tmsize_t {
        return fwrite(buf, 1, size, (FILE*)handle);
    };
    
    auto seekproc = [](thandle_t handle, toff_t off, int whence) -> toff_t {
        fseek((FILE*)handle, off, whence);
        return ftell((FILE*)handle);
    };
    
    auto closeproc = [](thandle_t handle) -> int {
        return fclose((FILE*)handle);
    };
    
    auto sizeproc = [](thandle_t handle) -> toff_t {
        long pos = ftell((FILE*)handle);
        fseek((FILE*)handle, 0, SEEK_END);
        long size = ftell((FILE*)handle);
        fseek((FILE*)handle, pos, SEEK_SET);
        return size;
    };
    
    auto mapproc = [](thandle_t, void** base, toff_t* size) -> int {
        *base = nullptr;
        *size = 0;
        return 0;
    };
    
    auto unmapproc = [](thandle_t, void* base, toff_t size) {};
    
    // 5. Open TIFF using client API
    TIFF* tif = TIFFClientOpen("input_file", "r", (thandle_t)in_file,
                              readproc, writeproc,
                              seekproc, closeproc,
                              sizeproc, mapproc, unmapproc);
    
    if (!tif) {
        assert_file_closed(&in_file);;
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // 6. Use TIFFFileno (simulated via TIFFClientdata and fuzz_fileno)
    // Note: TIFFFileno isn't in provided APIs, so we'll use TIFFClientdata
    // and get file descriptor from the FILE*
    thandle_t client_data = TIFFClientdata(tif);
    // client_data is our FILE*, get fd from it
    int tiff_fd = fuzz_fileno((FILE*)client_data);
    
    // 7. Register a custom codec (dummy for fuzzing)
    auto dummy_init = [](TIFF*, int) -> int { return 1; };
    TIFFCodec* codec = TIFFRegisterCODEC(65000, "FUZZ_CODEC", dummy_init);
    
    // 8. Check if image is upsampled
    int is_upsampled = TIFFIsUpSampled(tif);
    
    // 9. Get field name for a common tag
    const TIFFField* field = TIFFFieldWithTag(tif, TIFFTAG_IMAGEWIDTH);
    if (field) {
        const char* field_name = TIFFFieldName(field);
        // Use field_name to avoid unused variable warning
        (void)field_name;
    }
    
    // 10. Calculate strip size for 1 row
    uint32_t image_height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &image_height);
    if (image_height > 0) {
        tmsize_t strip_size = TIFFVStripSize(tif, 1);
        (void)strip_size;
    }
    
    // 11. Get unmap file procedure
    TIFFUnmapFileProc unmap_proc = TIFFGetUnmapFileProc(tif);
    (void)unmap_proc;
    
    // 12. Cleanup
    if (codec) {
        TIFFUnRegisterCODEC(codec);
    }
    
    TIFFClose(tif);
    // Note: TIFFClose will call our closeproc which closes the FILE*
    
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(tiff_fd);
	assert_fd_closed(fd);
	return 0;
}