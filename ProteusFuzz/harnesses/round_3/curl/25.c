// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_formget at formdata.c:627:5 in curl.h
// curl_formget at formdata.c:627:5 in curl.h
// curl_formget at formdata.c:627:5 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

/* Dummy callback for curl_formget matching the expected signature:
   size_t (*)(void *userp, const char *ptr, size_t len) */
static size_t dummy_formget_callback(void *userp, const char *ptr, size_t len) {
    (void)userp;
    (void)ptr;
    (void)len;
    return len;
}

/* Dummy write callback */
static size_t dummy_write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static void create_form_from_data(const uint8_t *Data, size_t Size, struct curl_httppost **form_out) {
    *form_out = NULL;
    struct curl_httppost *last = NULL;
    size_t offset = 0;

    while (offset < Size) {
        /* Ensure we have at least 2 bytes for name_len and content_len logic */
        if (offset + 1 >= Size) break;

        struct curl_httppost *new_part = calloc(1, sizeof(struct curl_httppost));
        if (!new_part) break;

        /* Create name */
        size_t name_len = (Data[offset] % 10) + 1;
        offset++;
        if (offset + name_len > Size) {
            free(new_part);
            break;
        }
        new_part->name = malloc(name_len + 1);
        if (!new_part->name) {
            free(new_part);
            break;
        }
        memcpy(new_part->name, Data + offset, name_len);
        new_part->name[name_len] = '\0';
        new_part->namelength = name_len;
        offset += name_len;

        /* Create contents */
        if (offset >= Size) {
            free(new_part->name);
            free(new_part);
            break;
        }
        size_t content_len = (Data[offset] % 20) + 1;
        offset++;
        if (offset + content_len > Size) {
            free(new_part->name);
            free(new_part);
            break;
        }
        new_part->contents = malloc(content_len + 1);
        if (!new_part->contents) {
            free(new_part->name);
            free(new_part);
            break;
        }
        memcpy(new_part->contents, Data + offset, content_len);
        new_part->contents[content_len] = '\0';
        new_part->contentslength = content_len;
        offset += content_len;

        /* Set flags based on data if available */
        if (offset < Size) {
            new_part->flags = (Data[offset] % 100);
            offset++;
        }

        /* Link to list */
        if (!(*form_out)) {
            *form_out = new_part;
        } else {
            last->next = new_part;
        }
        last = new_part;
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize curl_httppost list */
    struct curl_httppost *form = NULL;
    create_form_from_data(Data, Size, &form);

    /* Test curl_formget */
    if (form) {
        curl_formget(form, NULL, dummy_formget_callback);
        curl_formget(form, (void*)Data, dummy_formget_callback);
        /* Test with NULL callback to trigger error path */
        curl_formget(form, NULL, NULL);
    }

    /* Test curl_formfree */
    if (form) {
        curl_formfree(form);
    }

    /* Test curl_easy_setopt with various options using input data */
    if (Size > 0) {
        char url_buf[256];
        size_t url_len = Size < 255 ? Size : 255;
        memcpy(url_buf, Data, url_len);
        url_buf[url_len] = '\0';
        curl_easy_setopt(curl, CURLOPT_URL, url_buf);

        /* Test CURLOPT_HTTPHEADER with slist */
        if (Size > 10) {
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_slist_free_all(headers);
        }

        /* Test CURLOPT_POSTFIELDS */
        if (Size > 20) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char*)Data + 20);
        }

        /* Test CURLOPT_WRITEFUNCTION */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)Data);

        /* Test CURLOPT_HTTPPOST */
        if (Size > 0) {
            /* Re-create form for this test since it was freed */
            struct curl_httppost *form2 = NULL;
            create_form_from_data(Data, Size, &form2);
            if (form2) {
                curl_easy_setopt(curl, CURLOPT_HTTPPOST, form2);
                curl_formfree(form2);
            }
        }
    }

    /* Perform a dummy request (will fail but exercises code paths) */
    curl_easy_perform(curl);

    /* Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}