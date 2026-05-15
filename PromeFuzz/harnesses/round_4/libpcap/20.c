// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pcap/pcap.h>

#define PCAP_ERRBUF_SIZE 256
#define MAX_DEVICE_NAME_LEN 256
#define MAX_SOURCE_LEN 1024
#define MAX_AUTH_STR_LEN 256

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size);
        } else {
            const char *default_data = "dummy pcap data";
            write(fd, default_data, strlen(default_data));
        }
        close(fd);
    }
}

static void cleanup_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_20(const uint8_t *data, size_t size) {
    if (size == 0) return 0;

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    struct pcap_rmtauth auth;
    char *source = NULL;
    char *device = NULL;
    char *username = NULL;
    char *password = NULL;
    pcap_t *pcap_handle = NULL;

    uint8_t selector = data[0];
    const uint8_t *remaining_data = data + 1;
    size_t remaining_size = size - 1;

    if (remaining_size > 0) {
        source = (char *)malloc(MAX_SOURCE_LEN);
        if (source) {
            size_t copy_len = (remaining_size < MAX_SOURCE_LEN - 1) ? remaining_size : MAX_SOURCE_LEN - 1;
            memcpy(source, remaining_data, copy_len);
            source[copy_len] = '\0';
        }

        if (remaining_size > MAX_SOURCE_LEN) {
            device = (char *)malloc(MAX_DEVICE_NAME_LEN);
            if (device) {
                size_t offset = MAX_SOURCE_LEN - 1;
                size_t device_len = (remaining_size - offset < MAX_DEVICE_NAME_LEN - 1) ? 
                                   remaining_size - offset : MAX_DEVICE_NAME_LEN - 1;
                memcpy(device, remaining_data + offset, device_len);
                device[device_len] = '\0';
            }
        }

        if (remaining_size > MAX_SOURCE_LEN + MAX_DEVICE_NAME_LEN) {
            username = (char *)malloc(MAX_AUTH_STR_LEN);
            password = (char *)malloc(MAX_AUTH_STR_LEN);
            if (username && password) {
                size_t offset = MAX_SOURCE_LEN + MAX_DEVICE_NAME_LEN - 2;
                size_t auth_len = (remaining_size - offset < MAX_AUTH_STR_LEN - 1) ? 
                                 remaining_size - offset : MAX_AUTH_STR_LEN - 1;
                memcpy(username, remaining_data + offset, auth_len);
                username[auth_len] = '\0';
                
                if (remaining_size > offset + MAX_AUTH_STR_LEN) {
                    size_t pass_offset = offset + MAX_AUTH_STR_LEN - 1;
                    size_t pass_len = (remaining_size - pass_offset < MAX_AUTH_STR_LEN - 1) ? 
                                     remaining_size - pass_offset : MAX_AUTH_STR_LEN - 1;
                    memcpy(password, remaining_data + pass_offset, pass_len);
                    password[pass_len] = '\0';
                } else {
                    strcpy(password, "fuzzpass");
                }
            }
        }
    }

    if (username && password) {
        auth.type = selector % 3;
        auth.username = username;
        auth.password = password;
    }

    switch (selector % 6) {
        case 0:
            if (source) {
                pcap_findalldevs_ex(source, 
                                   (username && password) ? &auth : NULL, 
                                   &alldevs, 
                                   errbuf);
                if (alldevs) {
                    pcap_freealldevs(alldevs);
                    alldevs = NULL;
                }
            }
            break;
            
        case 1:
            pcap_findalldevs(&alldevs, errbuf);
            if (alldevs) {
                pcap_freealldevs(alldevs);
                alldevs = NULL;
            }
            break;
            
        case 2:
            write_dummy_file(data, size);
            if (source) {
                pcap_handle = pcap_open("./dummy_file", 
                                       128,
                                       selector & 0x01 ? 1 : 0,
                                       1000,
                                       NULL,
                                       errbuf);
                if (pcap_handle) {
                    pcap_close(pcap_handle);
                    pcap_handle = NULL;
                }
            }
            cleanup_file();
            break;
            
        case 3:
            if (device) {
                pcap_handle = pcap_open(device,
                                       64,
                                       selector & 0x02 ? 4 : 0,
                                       500,
                                       (username && password) ? &auth : NULL,
                                       errbuf);
                if (pcap_handle) {
                    pcap_close(pcap_handle);
                    pcap_handle = NULL;
                }
            }
            break;
            
        case 4:
            if (device) {
                pcap_handle = pcap_create(device, errbuf);
                if (pcap_handle) {
                    int activate_result = pcap_activate(pcap_handle);
                    if (activate_result >= 0) {
                        int datalink = pcap_datalink(pcap_handle);
                        int snapshot = pcap_snapshot(pcap_handle);
                        (void)datalink;
                        (void)snapshot;
                    }
                    pcap_close(pcap_handle);
                    pcap_handle = NULL;
                }
            }
            break;
            
        case 5:
            if (device) {
                pcap_handle = pcap_open_live(device,
                                             (selector & 0x0F) * 100 + 68,
                                             selector & 0x10 ? 1 : 0,
                                             (selector & 0x0F) * 100,
                                             errbuf);
                if (pcap_handle) {
                    pcap_setnonblock(pcap_handle, selector & 0x20 ? 1 : 0, errbuf);
                    pcap_close(pcap_handle);
                    pcap_handle = NULL;
                }
            }
            break;
    }

    if (source) free(source);
    if (device) free(device);
    if (username) free(username);
    if (password) free(password);
    if (alldevs) pcap_freealldevs(alldevs);
    if (pcap_handle) pcap_close(pcap_handle);

    return 0;
}