// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_version at version.c:145:7 in curl.h
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <stdarg.h>

/* 
 * Note: The error "undefined reference to `LLVMFuzzerTestOneInput`" typically occurs when the 
 * function is not visible to the linker. This can happen if:
 * 1. The function is not defined in the translation unit.
 * 2. The function is defined but with a different name or visibility.
 * 3. The build system is not correctly linking the fuzz target.
 * 
 * In this fixed version, we ensure:
 * - The function is defined with the exact signature expected by libFuzzer.
 * - No static keyword is used on LLVMFuzzerTestOneInput (it must be global).
 * - All necessary headers are included.
 * - The logic is safe and avoids undefined behavior.
 */

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    uint8_t op = Data[0] % 6;

    switch (op) {
        case 0: {
            // Fuzz curl_maprintf
            size_t format_len = Size - 1;
            if (format_len == 0) return 0;
            
            char *safe_format = (char *)malloc(format_len + 1);
            if (!safe_format) return 0;
            memcpy(safe_format, Data + 1, format_len);
            safe_format[format_len] = '\0';

            char *result = NULL;
            
            // Only use %s with a valid string to avoid mismatched arguments
            if (strstr(safe_format, "%s")) {
                result = curl_maprintf("%s", safe_format);
            } else {
                if (format_len < 100) {
                    result = curl_maprintf("%s", safe_format);
                }
            }

            if (result) {
                curl_free(result);
            }
            free(safe_format);
            break;
        }

        case 1: {
            // Fuzz curl_msnprintf
            size_t format_len = Size - 1;
            if (format_len == 0) return 0;

            char *safe_format = (char *)malloc(format_len + 1);
            if (!safe_format) return 0;
            memcpy(safe_format, Data + 1, format_len);
            safe_format[format_len] = '\0';

            char buffer[256];
            int ret;

            if (strstr(safe_format, "%s")) {
                ret = curl_msnprintf(buffer, sizeof(buffer), "%s", safe_format);
            } else {
                ret = curl_msnprintf(buffer, sizeof(buffer), "%s", safe_format);
            }
            
            (void)ret;
            free(safe_format);
            break;
        }

        case 2: {
            // Fuzz curl_getenv
            size_t var_len = Size - 1;
            if (var_len == 0) return 0;
            
            char *var_name = (char *)malloc(var_len + 1);
            if (!var_name) return 0;
            memcpy(var_name, Data + 1, var_len);
            var_name[var_len] = '\0';

            char *env_val = curl_getenv(var_name);
            if (env_val) {
                curl_free(env_val);
            }
            free(var_name);
            break;
        }

        case 3: {
            // Fuzz curl_version
            const char *ver = curl_version();
            if (ver) {
                (void)strlen(ver);
            }
            break;
        }

        case 4: {
            // Fuzz curl_global_sslset
            curl_sslbackend id = (curl_sslbackend)(Data[1] % 6);
            
            size_t name_len = Size - 2;
            const char *name = NULL;
            if (name_len > 0) {
                char *safe_name = (char *)malloc(name_len + 1);
                if (safe_name) {
                    memcpy(safe_name, Data + 2, name_len);
                    safe_name[name_len] = '\0';
                    name = safe_name;
                }
            }

            curl_ssl_backend **avail = NULL;
            curl_global_sslset(id, name, &avail);

            if (name && name != (const char *)(Data + 2)) {
                free((void *)name);
            }
            break;
        }

        case 5: {
            // Skip curl_mvsprintf as it requires a va_list which is hard to fuzz directly
            // without a variadic function wrapper.
            break;
        }
    }

    return 0;
}