// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_load at magic.c:317:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    if (size > 0) {
        if (write(fd, data, size) != (ssize_t)size) {
            close(fd);
            return -1;
        }
    }
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    magic_t co, lo, co2, lo2, co3, lo3;
    int ret;

    /* Test magic_open with various flags */
    co = magic_open(MAGIC_NONE);
    if (co == NULL) return 0;

    lo = magic_open(MAGIC_MIME_TYPE);
    if (lo == NULL) {
        magic_close(co);
        return 0;
    }

    /* Test magic_open with error flag for robustness */
    co2 = magic_open(MAGIC_ERROR);
    if (co2 == NULL) {
        magic_close(co);
        magic_close(lo);
        return 0;
    }

    /* Test magic_open with MIME_TYPE and ERROR */
    lo2 = magic_open(MAGIC_MIME_TYPE | MAGIC_ERROR);
    if (lo2 == NULL) {
        magic_close(co);
        magic_close(lo);
        magic_close(co2);
        return 0;
    }

    /* Test magic_open with MIME_TYPE and MIME */
    co3 = magic_open(MAGIC_MIME_TYPE | MAGIC_MIME);
    if (co3 == NULL) {
        magic_close(co);
        magic_close(lo);
        magic_close(co2);
        magic_close(lo2);
        return 0;
    }

    lo3 = magic_open(MAGIC_NONE);
    if (lo3 == NULL) {
        magic_close(co);
        magic_close(lo);
        magic_close(co2);
        magic_close(lo2);
        magic_close(co3);
        return 0;
    }

    /* Write fuzz data to dummy file for magic_file tests */
    if (write_dummy_file(Data, Size) == 0) {
        /* Test magic_file on dummy file */
        const char *res;
        res = magic_file(co, "./dummy_file");
        (void)res; /* Ignore result, just test for crash */
        res = magic_file(lo, "./dummy_file");
        (void)res;
        res = magic_file(co2, "./dummy_file");
        (void)res;
        res = magic_file(lo2, "./dummy_file");
        (void)res;
        res = magic_file(co3, "./dummy_file");
        (void)res;
        res = magic_file(lo3, "./dummy_file");
        (void)res;
    }

    /* Test magic_load with a non-existent file (should fail gracefully) */
    ret = magic_load(co, "/nonexistent/magic_file");
    (void)ret;
    ret = magic_load(lo, "/nonexistent/magic_file");
    (void)ret;

    /* Test magic_compile with a non-existent file */
    ret = magic_compile(co2, "/nonexistent/magic_file");
    (void)ret;
    ret = magic_compile(lo2, "/nonexistent/magic_file");
    (void)ret;

    /* Test magic_list with a non-existent file */
    ret = magic_list(co3, "/nonexistent/magic_file");
    (void)ret;
    ret = magic_list(lo3, "/nonexistent/magic_file");
    (void)ret;

    /* Test magic_check with a non-existent file */
    ret = magic_check(co, "/nonexistent/magic_file");
    (void)ret;
    ret = magic_check(lo, "/nonexistent/magic_file");
    (void)ret;

    /* Cleanup */
    magic_close(co);
    magic_close(lo);
    magic_close(co2);
    magic_close(lo2);
    magic_close(co3);
    magic_close(lo3);

    /* Clean up dummy file */
    unlink("./dummy_file");

    return 0;
}