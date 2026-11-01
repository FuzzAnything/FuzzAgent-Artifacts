// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }
    if (size > 0 && write(fd, data, size) != (ssize_t)size) {
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test magic_open, magic_load, magic_file
    magic_t co = magic_open(0);
    if (co != NULL) {
        magic_load(co, "/etc/magic.mgc"); 
        magic_compile(co, "/etc/magic.mgc");
        
        if (write_dummy_file(Data, Size) == 0) {
            const char *res = magic_file(co, "./dummy_file");
            (void)res;
        }
        
        magic_close(co);
    }

    // Test magic_setparam
    magic_t cp = magic_open(0);
    if (cp != NULL) {
        size_t val = 0;
        magic_setparam(cp, MAGIC_PARAM_INDIR_MAX, &val);
        magic_setparam(cp, MAGIC_PARAM_NAME_MAX, &val);
        magic_setparam(cp, MAGIC_PARAM_ELF_SHNUM_MAX, &val);
        magic_setparam(cp, MAGIC_PARAM_ELF_PHNUM_MAX, &val);
        magic_setparam(cp, MAGIC_PARAM_ELF_NOTES_MAX, &val);
        magic_setparam(cp, MAGIC_PARAM_REGEX_MAX, &val);
        magic_setparam(cp, MAGIC_PARAM_MAGWARN_MAX, &val);
        magic_setparam(cp, MAGIC_PARAM_BYTES_MAX, &val);
        magic_setparam(cp, MAGIC_PARAM_ENCODING_MAX, &val);
        magic_setparam(cp, MAGIC_PARAM_ELF_SHSIZE_MAX, &val);
        
        magic_load(cp, "/etc/magic.mgc");
        if (write_dummy_file(Data, Size) == 0) {
            const char *res = magic_file(cp, "./dummy_file");
            (void)res;
        }
        magic_close(cp);
    }

    // Test magic_list (requires separate handle, using 0 flags as MAGIC_LIST is not standard)
    magic_t lp = magic_open(0);
    if (lp != NULL) {
        magic_list(lp, "/etc/magic.mgc");
        if (write_dummy_file(Data, Size) == 0) {
            const char *res = magic_file(lp, "./dummy_file");
            (void)res;
        }
        magic_close(lp);
    }

    // Test with different flags
    magic_t mp = magic_open(MAGIC_MIME_TYPE);
    if (mp != NULL) {
        magic_load(mp, "/etc/magic.mgc");
        if (write_dummy_file(Data, Size) == 0) {
            const char *res = magic_file(mp, "./dummy_file");
            (void)res;
        }
        magic_close(mp);
    }

    magic_t mp2 = magic_open(MAGIC_MIME_ENCODING);
    if (mp2 != NULL) {
        magic_load(mp2, "/etc/magic.mgc");
        if (write_dummy_file(Data, Size) == 0) {
            const char *res = magic_file(mp2, "./dummy_file");
            (void)res;
        }
        magic_close(mp2);
    }

    // Test with MAGIC_ERROR
    magic_t mp3 = magic_open(MAGIC_ERROR);
    if (mp3 != NULL) {
        magic_load(mp3, "/etc/magic.mgc");
        if (write_dummy_file(Data, Size) == 0) {
            const char *res = magic_file(mp3, "./dummy_file");
            (void)res;
        }
        magic_close(mp3);
    }

    // Test with MAGIC_RAW
    magic_t mp4 = magic_open(MAGIC_RAW);
    if (mp4 != NULL) {
        magic_load(mp4, "/etc/magic.mgc");
        if (write_dummy_file(Data, Size) == 0) {
            const char *res = magic_file(mp4, "./dummy_file");
            (void)res;
        }
        magic_close(mp4);
    }

    return 0;
}