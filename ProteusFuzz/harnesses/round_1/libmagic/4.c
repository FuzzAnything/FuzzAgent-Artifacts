// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_getpath at magic.c:254:1 in magic.h
// magic_version at magic.c:607:1 in magic.h
// magic_version at magic.c:607:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static int write_dummy_file(const char *path, const uint8_t *data, size_t size) {
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    const char *dummy_path = "./dummy_file";
    const char *default_magic_path = "/usr/share/misc/magic.mgc";
    int ret;

    // Step 1: magic_getpath
    const char *path1 = magic_getpath(NULL, 0);
    (void)path1; // Use return value to suppress warnings

    // Step 2: magic_version (first call)
    int ver1 = magic_version();
    (void)ver1;

    // Step 3: magic_version (second call)
    int ver2 = magic_version();
    (void)ver2;

    // Step 4: magic_open (main handle)
    magic_t main_handle = magic_open(MAGIC_MIME_TYPE | MAGIC_ERROR);
    if (main_handle == NULL) {
        return 0;
    }

    // Step 5: magic_check (requires a separate handle)
    magic_t check_handle = magic_open(MAGIC_ERROR);
    if (check_handle != NULL) {
        // Write input data to dummy file for magic_check to validate
        if (Size > 0) {
            write_dummy_file(dummy_path, Data, Size);
        } else {
            // Create an empty file if input is empty
            write_dummy_file(dummy_path, NULL, 0);
        }
        ret = magic_check(check_handle, dummy_path);
        (void)ret;
        magic_close(check_handle);
    }

    // Step 6: magic_compile (on main handle)
    // We try to compile the dummy file. If it's not a valid magic file, it will fail, which is expected for fuzzing.
    if (Size > 0) {
        write_dummy_file(dummy_path, Data, Size);
    } else {
        write_dummy_file(dummy_path, NULL, 0);
    }
    ret = magic_compile(main_handle, dummy_path);
    (void)ret;

    // Step 7: magic_list (requires a separate handle)
    magic_t list_handle = magic_open(MAGIC_ERROR);
    if (list_handle != NULL) {
        ret = magic_list(list_handle, dummy_path);
        (void)ret;
        magic_close(list_handle);
    }

    // Step 8: magic_error
    const char *err_msg = magic_error(main_handle);
    (void)err_msg;

    // Step 9: magic_close (main handle)
    magic_close(main_handle);

    return 0;
}