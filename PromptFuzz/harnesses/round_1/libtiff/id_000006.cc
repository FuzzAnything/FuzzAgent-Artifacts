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
//<ID> 330
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 12
//<Quality> {"density":9,"unique_branches":{"TryChopUpUncompressedBigTiff":[[8138,9,8138,31,0,0,4,0]],"TIFFReadRawStrip1":[[642,9,642,18,0,0,4,1],[668,13,668,76,0,0,4,0],[668,13,668,76,0,0,4,1],[669,13,669,79,0,0,4,0],[669,13,669,79,0,0,4,1],[673,18,673,47,0,0,4,1],[680,17,680,35,0,0,4,0],[680,17,680,35,0,0,4,1],[685,13,685,22,0,0,4,0],[685,13,685,22,0,0,4,1]],"TIFFSetDirectory":[[2120,9,2120,29,0,0,4,1]]},"library_calls":["TIFFClientOpen","TIFFNumberOfDirectories","TIFFStripSize64","TIFFVStripSize64","_TIFFrealloc","TIFFWriteBufferSetup","LogL16fromY","TIFFCurrentDirectory","TIFFReadRawStrip","_TIFFrealloc","TIFFSetDirectory","TIFFGetField","TIFFGetField","TIFFVStripSize64","TIFFClose"],"critical_calls":["TIFFClientOpen","TIFFNumberOfDirectories","TIFFStripSize64","TIFFVStripSize64","_TIFFrealloc","TIFFWriteBufferSetup","LogL16fromY","TIFFCurrentDirectory","TIFFReadRawStrip","_TIFFrealloc","TIFFSetDirectory","TIFFGetField","TIFFGetField","TIFFVStripSize64","TIFFClose"],"visited":0}
/*Looking at the APIs and the requirements, I'll create a fuzz driver that uses these APIs together to process TIFF data from memory. The key insight is that we need to use `TIFFClientOpen` to create a TIFF handle from memory data, then perform various operations with the other APIs.

*/


// We need to include libtiff headers

// Memory buffer structure for TIFFClientOpen
struct mem_buffer {
    const uint8_t* data;
    size_t size;
    toff_t pos;
};

// Callback functions for memory I/O
static tmsize_t  id_000006_mem_read(thandle_t handle, void* buf, tmsize_t size) {
    mem_buffer* mem = reinterpret_cast<mem_buffer*>(handle);
    tmsize_t to_read = size;
    
    if (mem->pos + to_read > mem->size) {
        to_read = mem->size - mem->pos;
    }
    
    if (to_read > 0) {
        memcpy(buf, mem->data + mem->pos, to_read);
        mem->pos += to_read;
    }
    
    return to_read;
}

static tmsize_t  id_000006_mem_write(thandle_t handle, void* buf, tmsize_t size) {
    // Read-only for fuzzing
    return 0;
}

static toff_t  id_000006_mem_seek(thandle_t handle, toff_t offset, int whence) {
    mem_buffer* mem = reinterpret_cast<mem_buffer*>(handle);
    
    switch (whence) {
        case SEEK_SET:
            mem->pos = offset;
            break;
        case SEEK_CUR:
            mem->pos += offset;
            break;
        case SEEK_END:
            mem->pos = mem->size + offset;
            break;
    }
    
    if (mem->pos > mem->size) {
        mem->pos = mem->size;
    }
    
    return mem->pos;
}

static int  id_000006_mem_close(thandle_t handle) {
    // Nothing to clean up
    return 0;
}

static toff_t  id_000006_mem_size(thandle_t handle) {
    mem_buffer* mem = reinterpret_cast<mem_buffer*>(handle);
    return mem->size;
}

static int  id_000006_mem_map(thandle_t handle, void** base, toff_t* size) {
    mem_buffer* mem = reinterpret_cast<mem_buffer*>(handle);
    *base = const_cast<void*>(reinterpret_cast<const void*>(mem->data));
    *size = mem->size;
    return 1;
}

static void  id_000006_mem_unmap(thandle_t handle, void* base, toff_t size) {
    // Nothing to do
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
	//fuzzer shim end}




    if (size < 100) {
        return 0;  // Too small to be meaningful TIFF data
    }
    
    TIFF* tif = nullptr;
    mem_buffer mem_buf = {data, size, 0};
    
    // 1. Open TIFF from memory using TIFFClientOpen
    // Note: We're using "r" mode for reading only
    tif = TIFFClientOpen(fuzz_str_1, fuzz_str_2, 
                        reinterpret_cast<thandle_t>(&mem_buf),
                         id_000006_mem_read,  id_000006_mem_write,
                         id_000006_mem_seek,  id_000006_mem_close,
                         id_000006_mem_size,  id_000006_mem_map,  id_000006_mem_unmap);
    
    if (!tif) {
        return 0;  // Not valid TIFF data
    }
    
    // 2. Get number of directories
    tdir_t num_dirs = TIFFNumberOfDirectories(tif);
    
    if (num_dirs > 0) {
        // 3. Get strip size using TIFFStripSize64
        uint64_t strip_size = TIFFStripSize64(tif);
        
        // 4. Get vertical strip size for 1 row using TIFFVStripSize64
        uint64_t vstrip_size = TIFFVStripSize64(tif, fuzz_uint32_t_3);
        
        // 5. Set up write buffer (allocate some memory)
        size_t buffer_size = strip_size > 0 ? static_cast<size_t>(strip_size) : 4096;
        void* write_buffer = _TIFFrealloc(nullptr, buffer_size);
        
        if (write_buffer) {
            // 6. Try to set up write buffer in TIFF
            int setup_result = TIFFWriteBufferSetup(tif, write_buffer, buffer_size);
            
            // 7. Use LogL16fromY for some sample conversion
            // We need some sample values, so let's use data from the input
            if (size >= 16) {
                double y_value = static_cast<double>(data[0]) / 255.0;
                int preset = static_cast<int>(data[1] % 4);  // 0-3
                int logl16_result = LogL16fromY(y_value, preset);
                (void)logl16_result;  // Use result to avoid unused variable warning
            }
            
            // Try to read some data to exercise the buffer
            if (TIFFCurrentDirectory(tif) < num_dirs) {
                // Try to read a strip if possible
                uint32_t strip = 0;
                tmsize_t read_size = TIFFReadRawStrip(tif, fuzz_uint32_t_4, write_buffer, buffer_size);
                (void)read_size;  // Result may be -1, that's OK for fuzzing
            }
            
            // Free the write buffer
            _TIFFrealloc(write_buffer, 0);
        }
        
        // Try to process each directory
        for (tdir_t dir = 0; dir < num_dirs; dir++) {
            if (!TIFFSetDirectory(tif, fuzz_uint32_t_5)) {
                break;
            }
            
            // Get image dimensions if available
            uint32_t width = 0, height = 0;
            if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
                TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
                
                // Calculate strip size for different numbers of rows
                for (uint32_t nrows = 1; nrows <= 10 && nrows <= height; nrows *= 2) {
                    uint64_t vstrip = TIFFVStripSize64(tif, fuzz_uint32_t_6);
                    (void)vstrip;  // Use to avoid unused variable warning
                }
            }
        }
    }
    
    // Clean up
    if (tif) {
        TIFFClose(tif);
    }
    
    return 0;
}