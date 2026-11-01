// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:33331:18 in sqlite3.h
// sqlite3_vsnprintf at sqlite3.c:33380:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sqlite3.h>

/* Helper to extract a C string from fuzz input, ensuring null termination */
static const char *get_string(const uint8_t *data, size_t size, size_t *offset) {
  size_t start = *offset;
  size_t end = start;
  if (start >= size) return NULL;
  while (end < size && data[end] != 0) {
    end++;
  }
  if (end == start) {
    // Skip empty strings to avoid infinite loops or invalid format strings
    *offset = start + 1;
    return NULL;
  }
  
  // Allocate a buffer for the string and ensure it is null-terminated
  size_t len = end - start;
  char *str = (char *)malloc(len + 1);
  if (!str) return NULL;
  
  memcpy(str, &data[start], len);
  str[len] = '\0'; // Explicitly null-terminate
  
  *offset = end + 1;
  return (const char *)str;
}

/* Helper to get an integer from fuzz input */
static int get_int(const uint8_t *data, size_t size, size_t *offset) {
  if (*offset + 4 > size) {
    *offset = size;
    return 0;
  }
  int val = data[*offset] | (data[*offset + 1] << 8) | (data[*offset + 2] << 16) | (data[*offset + 3] << 24);
  *offset += 4;
  return val;
}

/* Helper function to safely start a va_list for fixed-argument functions */
static char *fuzz_vmprintf_helper(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char *result = sqlite3_vmprintf(fmt, args);
  va_end(args);
  return result;
}

static char *fuzz_vsnprintf_helper(int n, char *buf, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char *result = sqlite3_vsnprintf(n, buf, fmt, args);
  va_end(args);
  return result;
}

int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  size_t offset = 0;
  int choice = Data[0] % 6;

  if (choice == 0) {
    /* Fuzz sqlite3_mprintf */
    const char *fmt = get_string(Data, Size, &offset);
    if (!fmt) return 0;

    char *result = sqlite3_mprintf(fmt, "test", 123, 45.6, (void *)0x1234);
    if (result) {
      sqlite3_free(result);
    }
    free((void *)fmt); // Free the allocated string
  } 
  else if (choice == 1) {
    /* Fuzz sqlite3_vmprintf via helper */
    const char *fmt = get_string(Data, Size, &offset);
    if (!fmt) return 0;

    char *result = fuzz_vmprintf_helper(fmt, "test", 123, 45.6, (void *)0x1234);
    if (result) {
      sqlite3_free(result);
    }
    free((void *)fmt); // Free the allocated string
  } 
  else if (choice == 2) {
    /* Fuzz sqlite3_vsnprintf via helper */
    const char *fmt = get_string(Data, Size, &offset);
    if (!fmt) return 0;

    int n = get_int(Data, Size, &offset);
    if (n <= 0) n = 1;
    if (n > 4096) n = 4096;

    char *buf = malloc(n);
    if (!buf) {
      free((void *)fmt);
      return 0;
    }
    
    /* Ensure the buffer is initialized to avoid undefined behavior if snprintf writes less than expected */
    memset(buf, 0, n);

    char *result = fuzz_vsnprintf_helper(n, buf, fmt, "test", 123, 45.6, (void *)0x1234);
    
    if (result) {
      /* result is buf */
    }
    free(buf);
    free((void *)fmt); // Free the allocated string
  } 
  else if (choice == 3) {
    /* Fuzz sqlite3_str_appendf */
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (!str) return 0;

    const char *fmt = get_string(Data, Size, &offset);
    if (fmt) {
      sqlite3_str_appendf(str, fmt, "test", 123, 45.6, (void *)0x1234);
      free((void *)fmt); // Free the allocated string
    }
    
    /* Also try appending some raw data if available */
    if (offset < Size) {
      int len = get_int(Data, Size, &offset);
      if (len > 0 && len < 100) {
        if (offset + len <= Size) {
          sqlite3_str_append(str, (const char *)&Data[offset], len);
          offset += len;
        }
      }
    }

    char *result = sqlite3_str_finish(str);
    if (result) {
      sqlite3_free(result);
    }
  } 
  else if (choice == 4) {
    /* Fuzz sqlite3_str_finish directly (requires a valid object) */
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (!str) return 0;

    /* Append something to ensure it's not empty if we want to test non-null return */
    sqlite3_str_appendf(str, "test");
    
    char *result = sqlite3_str_finish(str);
    if (result) {
      sqlite3_free(result);
    }
  } 
  else if (choice == 5) {
    /* Fuzz sqlite3_snprintf */
    const char *fmt = get_string(Data, Size, &offset);
    if (!fmt) return 0;

    int n = get_int(Data, Size, &offset);
    if (n <= 0) n = 1;
    if (n > 4096) n = 4096;

    char *buf = malloc(n);
    if (!buf) {
      free((void *)fmt);
      return 0;
    }
    
    memset(buf, 0, n);

    char *result = sqlite3_snprintf(n, buf, fmt, "test", 123, 45.6, (void *)0x1234);
    if (result) {
      /* result is buf */
    }
    free(buf);
    free((void *)fmt); // Free the allocated string
  }

  return 0;
}