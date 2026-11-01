// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
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

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test magic_open
    magic_t m = magic_open(0);
    if (m == NULL) return 0;

    // Test magic_check with a dummy file
    write_dummy_file(Data, Size);
    magic_check(m, "./dummy_file");

    // Test magic_compile with a dummy file
    magic_compile(m, "./dummy_file");

    // Test magic_file with a dummy file
    const char *result = magic_file(m, "./dummy_file");
    (void)result; // Suppress unused variable warning

    // Test magic_error
    const char *error_msg = magic_error(m);
    (void)error_msg; // Suppress unused variable warning

    // Clean up magic handle
    magic_close(m);

    // Test magic_list with a separate magic handle
    magic_t m_list = magic_open(0);
    if (m_list != NULL) {
        magic_list(m_list, "./dummy_file");
        magic_close(m_list);
    }

    return 0;
}