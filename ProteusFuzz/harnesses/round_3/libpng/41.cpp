// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_valid at pngget.c:19:1 in png.h
// png_get_text at pngget.c:1158:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_get_PLTE at pngget.c:1121:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <setjmp.h>

static void dummy_error_fn(png_structp png_ptr, const char *error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do not abort, just return. The caller will handle via setjmp/longjmp if configured,
    // or we rely on the fact that png_read_png might not be called on invalid data.
    // However, standard libpng error handling expects longjmp.
    // We will use a custom jmp_buf in the main function to catch this.
}

static void dummy_warning_fn(png_structp png_ptr, const char *warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Custom read function that provides data from the fuzz input
static void dummy_read_data_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // We will pass the file pointer as the user pointer.
    // But since we are fuzzing, we might not have a valid file open or valid data.
    // This function is called by libpng. We need to read from the file.
    FILE *fp = (FILE *)png_get_io_ptr(png_ptr);
    if (fp) {
        size_t bytes_read = fread(data, 1, length, fp);
        if (bytes_read != length) {
            // End of file or error.
            // We can't easily signal EOF to libpng without setting error state,
            // but libpng usually handles short reads gracefully or errors out.
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const char *filename = "./dummy_file";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd != -1) {
        ssize_t written = write(fd, Data, Size);
        close(fd);
        (void)written;
    }

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_inforp info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        unlink(filename);
        return 0;
    }

    // We need to handle errors. Since png_struct is opaque, we cannot access jmpbuf directly.
    // We must use png_set_error_fn to set our own error handler, but that handler
    // typically calls longjmp(png_ptr->jmpbuf, 1).
    // To catch this, we must set the error function to one that uses a known jmpbuf,
    // or we rely on the fact that if we don't call png_read_png, we won't trigger the error.
    // However, png_read_png is the target.
    // The standard way to catch libpng errors is:
    // if (setjmp(png_jmpbuf(png_ptr))) { ... }
    // png_jmpbuf is a macro that accesses the internal jmpbuf.
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during libpng operations
        png_destroy_read_struct(&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);
        unlink(filename);
        return 0;
    }

    // Set custom error handlers
    // Note: The standard error handler calls longjmp(png_ptr->jmpbuf, 1).
    // We are using png_jmpbuf(png_ptr) which is the correct way to access it.
    png_set_error_fn(png_ptr, NULL, dummy_error_fn, dummy_warning_fn);

    // Open the file for reading
    FILE *fp = fopen(filename, "rb");
    if (fp) {
        // Set the read function
        png_set_read_fn(png_ptr, fp, dummy_read_data_fn);

        // Try to read the PNG. This will likely fail if Data is not a valid PNG,
        // but the setjmp above should catch the longjmp from the error handler.
        // We use PNG_TRANSFORM_IDENTITY to avoid complex transforms.
        // Note: png_read_png reads the whole image.
        // If the file is invalid, it will jump to the setjmp block.
        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
        
        fclose(fp);
    } else {
        // If we can't open the file, we still want to test the getter functions
        // on the initialized (but empty) info structure.
    }

    // 1. Test png_get_valid with various flags
    png_uint_32 valid_flags[] = {
        PNG_INFO_tRNS, PNG_INFO_PLTE, PNG_INFO_IDAT,
        PNG_INFO_gAMA, PNG_INFO_cHRM,
        0xFFFFFFFF, 0x00000000
    };
    const size_t num_flags = sizeof(valid_flags) / sizeof(valid_flags[0]);
    for (size_t i = 0; i < num_flags; ++i) {
        png_get_valid(png_ptr, info_ptr, valid_flags[i]);
    }

    // 2. Test png_get_text
    png_textp text_ptr = NULL;
    int num_text = 0;
    png_get_text(png_ptr, info_ptr, &text_ptr, &num_text);

    // 3. png_read_png was attempted above inside the file block.

    // 4. Test png_set_invalid with various masks
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_tRNS);
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_PLTE);
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_IDAT);
    png_set_invalid(png_ptr, info_ptr, 0xFFFFFFFF);

    // 5. Test png_get_PLTE
    png_colorp palette = NULL;
    int num_palette = 0;
    png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);

    // 6. Test png_get_palette_max
    int palette_max = png_get_palette_max(png_ptr, info_ptr);
    (void)palette_max;

    // Cleanup
    png_destroy_read_struct(&png_ptr, (png_infopp)&info_ptr, (png_infopp)NULL);

    // Remove dummy file
    unlink(filename);

    return 0;
}