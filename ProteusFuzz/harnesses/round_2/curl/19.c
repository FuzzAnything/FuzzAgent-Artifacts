// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_meta at ws.c:1851:29 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>

/* 
 * The error "undefined reference to `LLVMFuzzerTestOneInput'" typically occurs when:
 * 1. The function is not defined in the linked object file.
 * 2. The function name is misspelled.
 * 3. The function is not visible (e.g., missing 'static' or 'extern' issues in some setups, though 'static' is usually fine for fuzzer drivers).
 * 4. The fuzzer driver is not being linked correctly with the fuzzer runtime.
 * 
 * However, in the context of providing a source file for a fuzzer, the most common cause for this specific linker error in a standalone compilation test 
 * (if the user is compiling it manually without the fuzzer flags) is that the symbol isn't found. 
 * 
 * But looking at the provided error, it says the linker is looking for `LLVMFuzzerTestOneInput` in the fuzzer runtime's main, 
 * which means the fuzzer runtime expects to find this function in the user-provided code.
 * 
 * The previous code defined `static int LLVMFuzzerTestOneInput_19(...)`. 
 * In C, `static` functions have internal linkage. While this is usually fine for the fuzzer (as the fuzzer runtime links against the object file containing the function),
 * sometimes build systems or specific configurations might have issues if the symbol isn't exported or if there's a mismatch.
 * 
 * However, the standard pattern for libFuzzer is indeed `static int LLVMFuzzerTestOneInput`.
 * 
 * Let's re-examine the error. The error comes from `FuzzerMain.cpp` which is part of the fuzzer runtime. It calls `LLVMFuzzerTestOneInput`.
 * If the user's code is compiled into an object file and linked, the symbol must be present.
 * 
 * Perhaps the issue is that the user is trying to compile this as a standalone program without the fuzzer flags, or the build system isn't picking up the function.
 * But the prompt asks to fix the code.
 * 
 * Wait, the error message says: `undefined reference to 'LLVMFuzzerTestOneInput'`.
 * This means the linker cannot find the definition of `LLVMFuzzerTestOneInput` in the user's code.
 * 
 * Is it possible the function name is wrong? No, it's standard.
 * Is it possible the function is not being compiled?
 * 
 * Let's check the function signature again.
 * `int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size)`
 * 
 * The previous code had:
 * `static int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size)`
 * 
 * Maybe the `static` keyword is causing an issue in some specific build environments or if the file is not being linked correctly? 
 * Although `static` is the recommended practice to avoid name collisions, if the build system expects a global symbol, removing `static` might help.
 * However, the standard libFuzzer documentation says to use `static`.
 * 
 * Another possibility: The user might be compiling this file as a standalone C program (e.g., `gcc fuzz.c -o fuzz`) instead of using `clang -fsanitize=fuzzer fuzz.c`.
 * If compiled as a standalone program, there is no `main` function provided by the fuzzer runtime, so the linker would complain about `main` being missing, not `LLVMFuzzerTestOneInput`.
 * 
 * The error message explicitly mentions `FuzzerMain.cpp.o`, which means the fuzzer runtime IS being linked.
 * So the fuzzer runtime is looking for `LLVMFuzzerTestOneInput` in the user's object file, but can't find it.
 * 
 * This implies that the function `LLVMFuzzerTestOneInput` is not present in the object file generated from the source code.
 * Why would that be?
 * 1. The function is not defined. (But it is in the code).
 * 2. The function is defined but with a different name (typo). (Checked, looks correct).
 * 3. The function is defined but with a different signature. (Checked, looks correct).
 * 4. The function is defined but with `static` and the linker is somehow not seeing it? (Unlikely for a single file).
 * 5. The code is not being compiled at all? (Unlikely).
 * 
 * Wait, could it be that the user is using a build system that requires the function to be non-static?
 * Or maybe the previous code had a syntax error that prevented compilation, but the user only showed the linker error?
 * 
 * Let's look at the previous code again.
 * It includes headers, defines the function, and returns 0.
 * 
 * One subtle issue: In some environments, if the function is defined as `static`, and the build system is doing something weird, it might not export the symbol.
 * But for libFuzzer, `static` is the norm.
 * 
 * Let's try removing `static` just in case. It's a common troubleshooting step for linker errors related to missing symbols in fuzzing contexts if the build system is non-standard.
 * Also, ensure the function signature matches exactly what libFuzzer expects.
 * 
 * Another thought: The error message says `undefined reference to 'LLVMFuzzerTestOneInput'`.
 * If the function was not compiled, the linker wouldn't find it.
 * 
 * Let's assume the issue is the `static` keyword for this specific environment. I will remove it.
 * Also, I will ensure all necessary headers are included and the code is clean.
 * 
 * Additionally, the previous code used `curl_easy_setopt` with `CURLOPT_CONNECT_ONLY` which is correct for the target functions.
 * 
 * Let's rewrite the code without `static` on the fuzzer function.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize buffer for send/recv operations */
    size_t buffer_size = Size > 1024 ? 1024 : Size;
    char *send_buffer = malloc(buffer_size);
    char *recv_buffer = malloc(buffer_size);
    size_t n = 0;
    size_t sent = 0;
    size_t recv = 0;
    curl_off_t fragsize = 0;
    unsigned int flags = 0;
    const struct curl_ws_frame *meta = NULL;

    if (!send_buffer || !recv_buffer) {
        curl_easy_cleanup(curl);
        free(send_buffer);
        free(recv_buffer);
        return 0;
    }

    memcpy(send_buffer, Data, buffer_size);

    /* Set up basic options to attempt connection */
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    /* Attempt to establish connection */
    (void)curl_easy_perform(curl);

    /* Fuzz curl_easy_send */
    (void)curl_easy_send(curl, send_buffer, buffer_size, &n);

    /* Fuzz curl_easy_recv */
    (void)curl_easy_recv(curl, recv_buffer, buffer_size, &n);

    /* Fuzz curl_ws_meta (only meaningful in callback, but we call it to test state) */
    meta = curl_ws_meta(curl);
    (void)meta;

    /* Fuzz curl_ws_send */
    if (Size > 4) {
        fragsize = *(const curl_off_t *)(Data + Size - 4);
        if (Size > 8) {
            flags = *(const unsigned int *)(Data + Size - 8);
        }
    }
    (void)curl_ws_send(curl, send_buffer, buffer_size, &sent, fragsize, flags);

    /* Fuzz curl_ws_recv */
    (void)curl_ws_recv(curl, recv_buffer, buffer_size, &recv, &meta);

    /* Cleanup */
    free(send_buffer);
    free(recv_buffer);
    curl_easy_cleanup(curl);

    return 0;
}