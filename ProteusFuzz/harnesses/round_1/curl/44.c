// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

static char* extract_string(const uint8_t *data, size_t size, size_t *offset, size_t max_len) {
    if (*offset >= size) {
        return NULL;
    }

    size_t len = 0;
    while (len < max_len && *offset + len < size && data[*offset + len] != '\0') {
        len++;
    }

    if (len == 0) {
        *offset += 1;
        char *empty = malloc(1);
        if (empty) empty[0] = '\0';
        return empty;
    }

    *offset += len + 1;
    char *res = malloc(len + 1);
    if (!res) return NULL;
    memcpy(res, data + (*offset - len - 1), len);
    res[len] = '\0';
    return res;
}

static char* extract_filename(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) {
        char *res = malloc(14);
        if (res) strcpy(res, "./dummy_file");
        return res;
    }
    
    size_t len = 0;
    while (len < 32 && *offset + len < size && data[*offset + len] != '\0' && data[*offset + len] != '/') {
        len++;
    }
    *offset += len + 1;
    
    if (len == 0) {
        char *res = malloc(14);
        if (res) strcpy(res, "./dummy_file");
        return res;
    }
    
    char *res = malloc(64);
    if (!res) return NULL;
    snprintf(res, 64, "./fuzz_%zu", *offset);
    return res;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        return 0;
    }

    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        return 0;
    }

    size_t idx = 0;
    
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fwrite("dummy content for fuzzing", 1, 25, f);
        fclose(f);
    }

    while (idx < Size) {
        if (idx >= Size) break;
        uint8_t cmd = Data[idx];
        idx++;

        if (idx >= Size) break;

        switch (cmd % 7) {
            case 0: {
                char *filename = extract_filename(Data, Size, &idx);
                if (filename) {
                    curl_mime_filedata(part, filename);
                    free(filename);
                } else {
                    curl_mime_filedata(part, "./dummy_file");
                }
                break;
            }
            case 1: {
                char *type = extract_string(Data, Size, &idx, 64);
                if (type) {
                    curl_mime_type(part, type);
                    free(type);
                } else {
                    curl_mime_type(part, NULL);
                }
                break;
            }
            case 2: {
                char *name = extract_string(Data, Size, &idx, 64);
                if (name) {
                    curl_mime_name(part, name);
                    free(name);
                } else {
                    curl_mime_name(part, NULL);
                }
                break;
            }
            case 3: {
                char *enc = extract_string(Data, Size, &idx, 32);
                if (enc) {
                    curl_mime_encoder(part, enc);
                    free(enc);
                } else {
                    curl_mime_encoder(part, NULL);
                }
                break;
            }
            case 4: {
                if (idx + 1 >= Size) break;
                size_t data_size = Data[idx];
                idx++;
                if (data_size > 1024) data_size = 1024;
                if (idx + data_size > Size) {
                    data_size = Size - idx;
                }
                if (data_size > 0) {
                    curl_mime_data(part, (const char*)(Data + idx), data_size);
                    idx += data_size;
                } else {
                    curl_mime_data(part, NULL, 0);
                }
                break;
            }
            case 5: {
                char *filename = extract_filename(Data, Size, &idx);
                if (filename) {
                    curl_mime_filename(part, filename);
                    free(filename);
                } else {
                    curl_mime_filename(part, "./dummy_file");
                }
                break;
            }
            case 6: {
                curl_mimepart *new_part = curl_mime_addpart(mime);
                if (new_part) {
                    part = new_part;
                }
                break;
            }
        }
    }

    curl_mime_free(mime);
    return 0;
}