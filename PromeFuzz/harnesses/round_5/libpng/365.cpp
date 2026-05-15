// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_progressive_read_fn at pngpread.c:1109:1 in png.h
// png_set_progressive_frame_fn at pngpread.c:1125:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_process_data at pngpread.c:50:1 in png.h
// png_get_current_pass_number at pngtrans.c:859:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_set_tRNS_to_alpha at pngrtran.c:969:1 in png.h
// png_set_strip_16 at pngrtran.c:185:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_set_progressive_read_fn at pngpread.c:1109:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_get_current_pass_number at pngtrans.c:859:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
// png_get_current_pass_number at pngtrans.c:859:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <csetjmp>

#include "code/png.h"

// User data structure for progressive reading
struct user_data {
    png_infop info_ptr;
    png_uint_32 rowbytes;
};

// Dummy callbacks for progressive reading
static void info_callback(png_structp png_ptr, png_infop info_ptr) {
    // Store info pointer in user data
    user_data* ud = (user_data*)png_get_progressive_ptr(png_ptr);
    if (ud) {
        ud->info_ptr = info_ptr;
        ud->rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    }
}

static void row_callback(png_structp png_ptr, png_bytep new_row, 
                         png_uint_32 row_num, int pass) {
    // Call png_progressive_combine_row if new_row is not NULL
    if (new_row != NULL) {
        user_data* ud = (user_data*)png_get_progressive_ptr(png_ptr);
        if (ud && ud->rowbytes > 0) {
            png_byte* old_row = (png_byte*)malloc(ud->rowbytes);
            if (old_row) {
                // Initialize with some data
                memset(old_row, 0, ud->rowbytes);
                png_progressive_combine_row(png_ptr, old_row, new_row);
                free(old_row);
            }
        }
    }
    
    // Call png_get_current_pass_number
    png_byte pass_num = png_get_current_pass_number(png_ptr);
    
    // Call png_get_progressive_ptr
    void* prog_ptr = png_get_progressive_ptr(png_ptr);
}

static void end_callback(png_structp png_ptr, png_infop info_ptr) {
    // Do nothing
}

// Dummy callbacks for APNG frame handling
static void frame_info_callback(png_structp png_ptr, png_uint_32 frame_num) {
    // Do nothing
}

static void frame_end_callback(png_structp png_ptr, png_uint_32 frame_num) {
    // Do nothing
}

// Error handling callback
static void error_callback(png_structp png_ptr, png_const_charp error_msg) {
    // Longjmp to the setjmp point
    longjmp(png_jmpbuf(png_ptr), 1);
}

// Warning handling callback  
static void warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_365(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for sequential reading
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Test 1: Progressive reading API
    {
        // Create PNG read structure
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                    nullptr, error_callback, warning_callback);
        if (!png_ptr) {
            return 0;
        }
        
        // Create PNG info structure
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
            return 0;
        }
        
        // Set up error handling with setjmp
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            return 0;
        }
        
        // Create user data for progressive reading
        user_data ud;
        ud.info_ptr = info_ptr;
        ud.rowbytes = 0;
        
        // Set up progressive read callbacks
        png_set_progressive_read_fn(png_ptr, &ud, info_callback, 
                                   row_callback, end_callback);
        
        // Set up progressive frame callbacks for APNG
        png_set_progressive_frame_fn(png_ptr, frame_info_callback, 
                                    frame_end_callback);
        
        // Get progressive pointer (should return &ud as set above)
        void* prog_ptr = png_get_progressive_ptr(png_ptr);
        
        // Process some data to trigger callbacks
        if (Size > 0) {
            // Process in chunks to simulate progressive reading
            size_t chunk_size = Size > 100 ? 100 : Size;
            png_process_data(png_ptr, info_ptr, 
                           const_cast<png_bytep>(Data), chunk_size);
            
            // Call png_get_current_pass_number
            png_byte pass_num = png_get_current_pass_number(png_ptr);
        }
        
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
    
    // Test 2: Sequential reading API
    {
        fp = fopen("./dummy_file", "rb");
        if (!fp) {
            return 0;
        }
        
        // Create PNG read structure
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                    nullptr, error_callback, warning_callback);
        if (!png_ptr) {
            fclose(fp);
            return 0;
        }
        
        // Create PNG info structure
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
            fclose(fp);
            return 0;
        }
        
        // Set up error handling with setjmp
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            fclose(fp);
            return 0;
        }
        
        // Initialize I/O
        png_init_io(png_ptr, fp);
        
        // Read PNG header
        png_read_info(png_ptr, info_ptr);
        
        // Get image dimensions
        png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
        png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        png_byte color_type = png_get_color_type(png_ptr, info_ptr);
        
        // Set up transformations if needed
        if (color_type == PNG_COLOR_TYPE_PALETTE) {
            png_set_palette_to_rgb(png_ptr);
        }
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
            png_set_tRNS_to_alpha(png_ptr);
        }
        if (bit_depth == 16) {
            png_set_strip_16(png_ptr);
        }
        if (bit_depth < 8) {
            png_set_packing(png_ptr);
        }
        if (color_type == PNG_COLOR_TYPE_GRAY ||
            color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
            png_set_gray_to_rgb(png_ptr);
        }
        
        // Update info structure after transformations
        png_read_update_info(png_ptr, info_ptr);
        
        // Allocate row buffer
        png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        png_bytep row = (png_bytep)png_malloc(png_ptr, rowbytes);
        
        // Read rows using png_read_row
        for (png_uint_32 y = 0; y < height; y++) {
            png_read_row(png_ptr, row, nullptr);
        }
        
        // Clean up
        png_free(png_ptr, row);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
    }
    
    // Test 3: Additional tests with different configurations
    {
        // Create another PNG structure for more testing
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                    nullptr, error_callback, warning_callback);
        if (!png_ptr) {
            return 0;
        }
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
            return 0;
        }
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            return 0;
        }
        
        // Test with different progressive pointer values
        int dummy_data = 42;
        user_data ud2;
        ud2.info_ptr = info_ptr;
        ud2.rowbytes = 0;
        
        png_set_progressive_read_fn(png_ptr, &ud2, info_callback, 
                                   row_callback, end_callback);
        
        // Verify progressive pointer
        void* retrieved_ptr = png_get_progressive_ptr(png_ptr);
        
        // Test png_get_current_pass_number with null pointer
        png_byte pass_num = png_get_current_pass_number(nullptr);
        
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
    
    return 0;
}