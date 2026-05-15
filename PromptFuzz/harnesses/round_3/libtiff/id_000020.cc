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
//<ID> 969
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 16
//<Quality> {"density":6,"unique_branches":{"td_lfind":[[554,34,554,47,0,0,4,0],[555,13,555,34,0,0,4,0],[555,13,555,34,0,0,4,1]],"_TIFFRand":[[781,9,781,18,0,0,4,0],[781,9,781,18,0,0,4,1]],"TIFFFieldWithName":[[944,9,944,13,0,0,4,1]],"LogL10fromY":[[896,9,896,20,0,0,4,0],[898,14,898,28,0,0,4,1]],"tagNameCompare":[[619,9,619,12,0,0,4,0],[619,9,619,12,0,0,4,1],[622,16,622,44,0,0,4,0]],"tiff_itrunc":[[792,9,792,35,0,0,4,1]],"_TIFFFindFieldByName":[[909,9,909,28,0,0,4,0],[915,9,915,25,0,0,4,1],[927,35,927,38,0,0,4,0],[66,21,66,40,1,0,4,1]]},"library_calls":["TIFFClientOpenExt","TIFFReadGPSDirectory","TIFFFieldWithName","TIFFFieldReadCount","LogLuv24fromXYZ","LogL10fromY","TIFFReadDirectory","TIFFClose"],"critical_calls":["TIFFClientOpenExt","TIFFReadGPSDirectory","TIFFFieldWithName","TIFFFieldReadCount","LogLuv24fromXYZ","LogL10fromY","TIFFReadDirectory","TIFFClose"],"visited":1}
/**/


// Memory buffer structure for TIFF client I/O
struct MemoryBuffer {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// I/O callbacks for TIFFClientOpenExt
static tmsize_t  id_000020_memoryReadProc(thandle_t handle, void* buf, tmsize_t size) {
    MemoryBuffer* buffer = reinterpret_cast<MemoryBuffer*>(handle);
    tmsize_t available = buffer->size - buffer->pos;
    tmsize_t to_read = (size < available) ? size : available;
    
    if (to_read > 0) {
        memcpy(buf, buffer->data + buffer->pos, to_read);
        buffer->pos += to_read;
    }
    return to_read;
}

static tmsize_t  id_000020_memoryWriteProc(thandle_t, void*, tmsize_t) {
    return 0; // Read-only
}

static toff_t  id_000020_memorySeekProc(thandle_t handle, toff_t offset, int whence) {
    MemoryBuffer* buffer = reinterpret_cast<MemoryBuffer*>(handle);
    
    switch (whence) {
        case SEEK_SET:
            buffer->pos = (offset < buffer->size) ? offset : buffer->size;
            break;
        case SEEK_CUR:
            buffer->pos = ((buffer->pos + offset) < buffer->size) ? 
                         (buffer->pos + offset) : buffer->size;
            break;
        case SEEK_END:
            buffer->pos = buffer->size;
            break;
    }
    return buffer->pos;
}

static int  id_000020_memoryCloseProc(thandle_t handle) {
    MemoryBuffer* buffer = reinterpret_cast<MemoryBuffer*>(handle);
    delete buffer;
    return 0;
}

static toff_t  id_000020_memorySizeProc(thandle_t handle) {
    MemoryBuffer* buffer = reinterpret_cast<MemoryBuffer*>(handle);
    return buffer->size;
}

static int  id_000020_memoryMapProc(thandle_t, void**, toff_t*) {
    return 0;
}

static void  id_000020_memoryUnmapProc(thandle_t, void*, toff_t) {}

// Helper function for TIFFVGetField
static int  id_000020_MyTIFFVGetField(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    int result = TIFFVGetField(tif, tag, ap);
    va_end(ap);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<40) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for meaningful testing
    
    // 1. Create TIFF from memory using TIFFClientOpenExt
    MemoryBuffer* memBuffer = new MemoryBuffer{data, size, 0};
    
    TIFFOpenOptions* opts = nullptr;
    // Note: TIFFOpenOptions is opaque, we can't create it without proper API
    // We'll use null options
    
    TIFF* tif = TIFFClientOpenExt(
        fuzz_str_1, fuzz_str_2, 
        reinterpret_cast<thandle_t>(memBuffer),
         id_000020_memoryReadProc,  id_000020_memoryWriteProc,
         id_000020_memorySeekProc,  id_000020_memoryCloseProc,
         id_000020_memorySizeProc,  id_000020_memoryMapProc,  id_000020_memoryUnmapProc,
        opts
    );
    
    if (!tif) {
        delete memBuffer;
        return 0;
    }
    
    // 2. Read GPS directory if present
    // First try to get GPS IFD offset from main directory
    toff_t gps_offset = 0;
    int has_gps =  id_000020_MyTIFFVGetField(tif, TIFFTAG_GPSIFD, &gps_offset);
    
    if (has_gps && gps_offset > 0) {
        TIFFReadGPSDirectory(tif, fuzz_uint64_t_3);
    }
    
    // 3. Use TIFFFieldReadCount on a known field
    const TIFFField* widthField = TIFFFieldWithName(tif, fuzz_str_4);
    if (widthField) {
        int count = TIFFFieldReadCount(widthField);
        // count can be used, but we don't need to store it
    }
    
    // 4. Test LogLuv24fromXYZ and LogL10fromY conversions
    // Extract test values from input data
    float xyz[3];
    if (size >= 12) {
        memcpy(xyz, data, 12);
        // Ensure valid colorimetric type (0 or 1)
        int colortype = data[12] & 1;
        uint32_t logluv = LogLuv24fromXYZ(xyz, colortype);
        // Result can be ignored for fuzzing
    }
    
    // Test LogL10fromY
    if (size >= 20) {
        double y_value;
        memcpy(&y_value, data + 12, 8);
        // Ensure valid colorimetric type (0 or 1)
        int colortype = data[20] & 1;
        int logl10 = LogL10fromY(y_value, colortype);
        // Result can be ignored for fuzzing
    }
    
    // 5. Try to read more directories if present
    while (TIFFReadDirectory(tif)) {
        // Continue reading directories
    }
    
    // 6. Cleanup
    TIFFClose(tif);
    // Note:  id_000020_memoryCloseProc will delete the MemoryBuffer
    
    return 0;
}