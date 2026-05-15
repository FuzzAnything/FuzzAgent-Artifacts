// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc at pngmem.c:172:1 in png.h
// png_set_sPLT at pngset.c:1215:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_iCCP at pngget.c:718:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_sPLT at pngget.c:745:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "png.h"

struct buffer_state {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Read from the fuzzer input buffer
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    buffer_state* state = static_cast<buffer_state*>(io_ptr);
    if (state->offset + length > state->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, state->data + state->offset, length);
    state->offset += length;
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Error handler - do nothing
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning handler - do nothing
}

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    // Create buffer state for reading
    buffer_state state = {Data, Size, 0};
    
    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up custom read function
    png_set_read_fn(png_ptr, &state, dummy_read_fn);
    
    // Try to read PNG info
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    png_read_info(png_ptr, info_ptr);
    
    // Test png_get_hIST
    png_uint_16* hist = nullptr;
    png_uint_32 hist_result = png_get_hIST(png_ptr, info_ptr, &hist);
    
    // Test png_get_iCCP
    char* name = nullptr;
    int compression_type = 0;
    png_byte* profile = nullptr;
    png_uint_32 proflen = 0;
    png_uint_32 iccp_result = png_get_iCCP(png_ptr, info_ptr, &name, &compression_type, 
                                           &profile, &proflen);
    
    // Test png_get_valid with various flags
    png_uint_32 valid_result = 0;
    valid_result |= png_get_valid(png_ptr, info_ptr, PNG_INFO_hIST);
    valid_result |= png_get_valid(png_ptr, info_ptr, PNG_INFO_iCCP);
    valid_result |= png_get_valid(png_ptr, info_ptr, PNG_INFO_sPLT);
    valid_result |= png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
    valid_result |= png_get_valid(png_ptr, info_ptr, PNG_INFO_gAMA);
    
    // Test png_get_sPLT
    png_sPLT_tp sPLT_entries = nullptr;
    int sPLT_count = png_get_sPLT(png_ptr, info_ptr, &sPLT_entries);
    
    // Test png_set_sPLT with dummy data if we have enough input
    if (Size > 100) {
        // Create a dummy sPLT entry
        png_sPLT_t dummy_entry;
        dummy_entry.name = (char*)"dummy";
        dummy_entry.depth = 8;
        dummy_entry.nentries = 1;
        
        // Allocate entries array
        dummy_entry.entries = (png_sPLT_entryp)png_malloc(png_ptr, sizeof(png_sPLT_entry));
        if (dummy_entry.entries) {
            // Initialize with some values
            dummy_entry.entries[0].red = 100;
            dummy_entry.entries[0].green = 150;
            dummy_entry.entries[0].blue = 200;
            dummy_entry.entries[0].alpha = 255;
            dummy_entry.entries[0].frequency = 1;
            
            png_set_sPLT(png_ptr, info_ptr, &dummy_entry, 1);
            
            // Free the allocated memory
            png_free(png_ptr, dummy_entry.entries);
        }
    }
    
    // Test png_set_invalid with various masks
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_hIST);
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_iCCP);
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_sPLT);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}