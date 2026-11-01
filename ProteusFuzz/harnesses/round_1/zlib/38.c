// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzvprintf at gzwrite.c:403:15 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

static void cleanup_file(const char *path) {
    if (path) {
        remove(path);
    }
}

static int create_temp_file(const char *path, const uint8_t *data, size_t size) {
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    if (size > 0 && write(fd, data, size) != (ssize_t)size) {
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

// Helper function to test gzvprintf with variable arguments
// This function must be variadic to allow va_start to work correctly
static int test_gzvprintf(gzFile file, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = gzvprintf(file, format, args);
    va_end(args);
    return ret;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    const char *dummy_path = "./dummy_file";
    gzFile file = NULL;
    int fd = -1;
    int errnum = 0;
    const char *err_str;
    int ret;
    char *format_str = NULL;
    char *string_buf = NULL;

    // Cleanup previous runs
    cleanup_file(dummy_path);

    // Ensure we have some data to work with
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file with the fuzz input
    if (create_temp_file(dummy_path, Data, Size) != 0) {
        return 0;
    }

    // Test gzdopen: Open file descriptor and wrap in gzFile
    fd = open(dummy_path, O_RDWR);
    if (fd >= 0) {
        // Try write mode "w"
        file = gzdopen(fd, "w");
        if (file) {
            // Test gzputc
            if (Size > 0) {
                ret = gzputc(file, Data[0]);
                (void)ret;
            }
            
            // Test gzputs
            // Ensure null termination for safety
            if (Size > 1) {
                string_buf = (char *)malloc(Size + 1);
                if (string_buf) {
                    memcpy(string_buf, Data, Size);
                    string_buf[Size] = '\0';
                    ret = gzputs(file, string_buf);
                    (void)ret;
                    free(string_buf);
                }
            }

            // Test gzprintf
            if (Size > 2) {
                format_str = (char *)malloc(Size + 1);
                if (format_str) {
                    memcpy(format_str, Data, Size);
                    format_str[Size] = '\0';
                    // Simple format string test with integer
                    ret = gzprintf(file, "%d", (int)Data[0]);
                    (void)ret;
                    
                    // Test with string argument if safe
                    if (Size > 4) {
                        ret = gzprintf(file, "%s", format_str);
                        (void)ret;
                    }
                    free(format_str);
                }
            }

            // Test gzvprintf using the helper function
            // We pass a safe format string and an integer argument
            if (Size > 0) {
                ret = test_gzvprintf(file, "%d", (int)Data[0]);
                (void)ret;
            }

            // Test gzerror on a valid file (should return NULL or empty string usually)
            err_str = gzerror(file, &errnum);
            (void)err_str;
            (void)errnum;

            // Close the file
            gzclose(file);
            file = NULL;
        } else {
            // gzdopen failed, close fd
            close(fd);
            fd = -1;
        }
    } else {
        // Failed to open file
    }

    // Test error handling with invalid file
    err_str = gzerror(NULL, &errnum);
    (void)err_str;
    (void)errnum;

    // Cleanup
    if (fd >= 0) {
        close(fd);
    }
    cleanup_file(dummy_path);

    return 0;
}