// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_ERRBUF_SIZE 256
#define MAX_SOURCE_SIZE 1024
#define MAX_HOST_SIZE 256
#define MAX_PORT_SIZE 16
#define MAX_NAME_SIZE 256
#define MAX_USERNAME_SIZE 64
#define MAX_PASSWORD_SIZE 64

static void create_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void test_pcap_parsesrcstr(const uint8_t *data, size_t size) {
    char source[MAX_SOURCE_SIZE];
    char errbuf[MAX_ERRBUF_SIZE];
    int type;
    char host[MAX_HOST_SIZE];
    char port[MAX_PORT_SIZE];
    char name[MAX_NAME_SIZE];
    
    if (size > MAX_SOURCE_SIZE - 1) size = MAX_SOURCE_SIZE - 1;
    memcpy(source, data, size);
    source[size] = '\0';
    
    pcap_parsesrcstr(source, &type, host, port, name, errbuf);
}

static void test_pcap_createsrcstr(const uint8_t *data, size_t size) {
    char source[MAX_SOURCE_SIZE];
    char errbuf[MAX_ERRBUF_SIZE];
    int type = 0;
    char host[MAX_HOST_SIZE];
    char port[MAX_PORT_SIZE];
    char name[MAX_NAME_SIZE];
    
    if (size > 0) type = data[0] % 4;
    
    size_t offset = 1;
    size_t host_len = (size - offset) / 3;
    if (host_len > MAX_HOST_SIZE - 1) host_len = MAX_HOST_SIZE - 1;
    if (offset + host_len <= size) {
        memcpy(host, data + offset, host_len);
        host[host_len] = '\0';
        offset += host_len;
    } else {
        host[0] = '\0';
    }
    
    size_t port_len = (size - offset) / 2;
    if (port_len > MAX_PORT_SIZE - 1) port_len = MAX_PORT_SIZE - 1;
    if (offset + port_len <= size) {
        memcpy(port, data + offset, port_len);
        port[port_len] = '\0';
        offset += port_len;
    } else {
        port[0] = '\0';
    }
    
    size_t name_len = size - offset;
    if (name_len > MAX_NAME_SIZE - 1) name_len = MAX_NAME_SIZE - 1;
    if (offset + name_len <= size) {
        memcpy(name, data + offset, name_len);
        name[name_len] = '\0';
    } else {
        name[0] = '\0';
    }
    
    pcap_createsrcstr(source, type, host, port, name, errbuf);
}

static void test_pcap_findalldevs(const uint8_t *data, size_t size) {
    char errbuf[MAX_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    
    (void)data;
    (void)size;
    
    int ret = pcap_findalldevs(&alldevs, errbuf);
    if (ret == 0 && alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

static void test_pcap_findalldevs_ex(const uint8_t *data, size_t size) {
    char source[MAX_SOURCE_SIZE];
    char errbuf[MAX_ERRBUF_SIZE];
    struct pcap_rmtauth auth;
    char username[MAX_USERNAME_SIZE];
    char password[MAX_PASSWORD_SIZE];
    pcap_if_t *alldevs = NULL;
    
    if (size > MAX_SOURCE_SIZE - 1) size = MAX_SOURCE_SIZE - 1;
    memcpy(source, data, size);
    source[size] = '\0';
    
    if (size > 0) {
        auth.type = data[0] % 3;
        size_t user_len = size / 3;
        if (user_len > MAX_USERNAME_SIZE - 1) user_len = MAX_USERNAME_SIZE - 1;
        memcpy(username, data, user_len);
        username[user_len] = '\0';
        auth.username = username;
        
        size_t pass_len = size / 4;
        if (pass_len > MAX_PASSWORD_SIZE - 1) pass_len = MAX_PASSWORD_SIZE - 1;
        if (pass_len + user_len < size) {
            memcpy(password, data + user_len, pass_len);
            password[pass_len] = '\0';
            auth.password = password;
        } else {
            auth.password = NULL;
        }
    } else {
        auth.type = 0;
        auth.username = NULL;
        auth.password = NULL;
    }
    
    int ret = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
    if (ret == 0 && alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

static void test_pcap_open(const uint8_t *data, size_t size) {
    char source[MAX_SOURCE_SIZE];
    char errbuf[MAX_ERRBUF_SIZE];
    struct pcap_rmtauth auth;
    char username[MAX_USERNAME_SIZE];
    char password[MAX_PASSWORD_SIZE];
    
    if (size > MAX_SOURCE_SIZE - 1) size = MAX_SOURCE_SIZE - 1;
    memcpy(source, data, size);
    source[size] = '\0';
    
    int snaplen = 65535;
    int flags = 0;
    int read_timeout = 1000;
    
    if (size > 4) {
        snaplen = (data[0] << 8) | data[1];
        flags = data[2];
        read_timeout = data[3] * 100;
    }
    
    if (size > 8) {
        auth.type = data[4] % 3;
        size_t user_len = (size - 5) / 2;
        if (user_len > MAX_USERNAME_SIZE - 1) user_len = MAX_USERNAME_SIZE - 1;
        memcpy(username, data + 5, user_len);
        username[user_len] = '\0';
        auth.username = username;
        
        size_t pass_len = (size - 5 - user_len);
        if (pass_len > MAX_PASSWORD_SIZE - 1) pass_len = MAX_PASSWORD_SIZE - 1;
        if (5 + user_len + pass_len <= size) {
            memcpy(password, data + 5 + user_len, pass_len);
            password[pass_len] = '\0';
            auth.password = password;
        } else {
            auth.password = NULL;
        }
    } else {
        auth.type = 0;
        auth.username = NULL;
        auth.password = NULL;
    }
    
    pcap_t *pcap = pcap_open(source, snaplen, flags, read_timeout, &auth, errbuf);
    if (pcap != NULL) {
        pcap_close(pcap);
    }
}

static void test_pcap_create(const uint8_t *data, size_t size) {
    char device[MAX_NAME_SIZE];
    char errbuf[MAX_ERRBUF_SIZE];
    
    if (size > MAX_NAME_SIZE - 1) size = MAX_NAME_SIZE - 1;
    memcpy(device, data, size);
    device[size] = '\0';
    
    pcap_t *pcap = pcap_create(device, errbuf);
    if (pcap != NULL) {
        pcap_close(pcap);
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    create_dummy_file(data, size);
    
    unsigned char selector = data[0] % 6;
    
    switch (selector) {
        case 0:
            test_pcap_parsesrcstr(data, size);
            break;
        case 1:
            test_pcap_createsrcstr(data, size);
            break;
        case 2:
            test_pcap_findalldevs(data, size);
            break;
        case 3:
            test_pcap_findalldevs_ex(data, size);
            break;
        case 4:
            test_pcap_open(data, size);
            break;
        case 5:
            test_pcap_create(data, size);
            break;
    }
    
    unlink("./dummy_file");
    return 0;
}