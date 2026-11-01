// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
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
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
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
#include <stdio.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    magic_t cookie = magic_open(0);
    if (!cookie) {
        return 0;
    }

    // Test magic_setflags
    magic_setflags(cookie, MAGIC_PRESERVE_ATIME);
    magic_setflags(cookie, MAGIC_MIME_TYPE);
    magic_setflags(cookie, MAGIC_MIME_ENCODING);
    magic_setflags(cookie, MAGIC_CONTINUE);
    magic_setflags(cookie, MAGIC_RAW);
    magic_setflags(cookie, MAGIC_RAW | MAGIC_PRESERVE_ATIME);

    // Test magic_setparam with various parameters
    size_t param_val = 0;
    if (Size > 0) param_val = Data[0];
    magic_setparam(cookie, MAGIC_PARAM_INDIR_MAX, &param_val);
    magic_setparam(cookie, MAGIC_PARAM_NAME_MAX, &param_val);
    magic_setparam(cookie, MAGIC_PARAM_ELF_SHNUM_MAX, &param_val);
    magic_setparam(cookie, MAGIC_PARAM_ELF_PHNUM_MAX, &param_val);
    magic_setparam(cookie, MAGIC_PARAM_ELF_NOTES_MAX, &param_val);
    magic_setparam(cookie, MAGIC_PARAM_REGEX_MAX, &param_val);
    magic_setparam(cookie, MAGIC_PARAM_MAGWARN_MAX, &param_val);
    magic_setparam(cookie, MAGIC_PARAM_BYTES_MAX, &param_val);
    magic_setparam(cookie, MAGIC_PARAM_ENCODING_MAX, &param_val);
    magic_setparam(cookie, MAGIC_PARAM_ELF_SHSIZE_MAX, &param_val);

    // Test magic_getparam
    size_t retrieved_val = 0;
    magic_getparam(cookie, MAGIC_PARAM_INDIR_MAX, &retrieved_val);
    magic_getparam(cookie, MAGIC_PARAM_NAME_MAX, &retrieved_val);
    magic_getparam(cookie, MAGIC_PARAM_ELF_SHNUM_MAX, &retrieved_val);
    magic_getparam(cookie, MAGIC_PARAM_ELF_PHNUM_MAX, &retrieved_val);
    magic_getparam(cookie, MAGIC_PARAM_ELF_NOTES_MAX, &retrieved_val);
    magic_getparam(cookie, MAGIC_PARAM_REGEX_MAX, &retrieved_val);
    magic_getparam(cookie, MAGIC_PARAM_MAGWARN_MAX, &retrieved_val);
    magic_getparam(cookie, MAGIC_PARAM_BYTES_MAX, &retrieved_val);
    magic_getparam(cookie, MAGIC_PARAM_ENCODING_MAX, &retrieved_val);
    magic_getparam(cookie, MAGIC_PARAM_ELF_SHSIZE_MAX, &retrieved_val);

    // Test magic_compile (using dummy file)
    write_dummy_file(Data, Size);
    magic_compile(cookie, "./dummy_file");

    // Test magic_buffer
    const char *result = magic_buffer(cookie, Data, Size);
    if (result) {
        // Use result to avoid unused variable warning, though we don't care about content
        (void)strlen(result);
    }

    // Test magic_load_buffers
    void *buffers[1];
    size_t sizes[1];
    buffers[0] = (void *)Data;
    sizes[0] = Size;
    magic_load_buffers(cookie, buffers, sizes, 1);

    // Try loading multiple buffers if size permits
    if (Size >= 2) {
        void *buffers2[2];
        size_t sizes2[2];
        buffers2[0] = (void *)Data;
        sizes2[0] = Size / 2;
        buffers2[1] = (void *)(Data + Size / 2);
        sizes2[1] = Size - Size / 2;
        magic_load_buffers(cookie, buffers2, sizes2, 2);
    }

    // Cleanup
    magic_close(cookie);
    unlink("./dummy_file");

    return 0;
}