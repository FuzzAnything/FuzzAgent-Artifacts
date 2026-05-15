#include <fuzzer/FuzzedDataProvider.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/rand.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace harness_001 {
/*
 * Fuzzing harness for OpenSSL SSL/TLS handshake functions
 * Targets: Complete SSL/TLS handshake lifecycle with memory BIO
 * Coverage goal: 0% coverage SSL module (3000+ functions in ssl/ directory)
 */


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 64) {
        return 0; // Need minimum input for meaningful SSL testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize OpenSSL SSL library
    OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS | 
                    OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    ERR_clear_error();
    
    // Consume operation mode: 0 = client, 1 = server, 2 = both (create two SSL objects)
    uint8_t mode = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume cipher list preference
    size_t cipher_list_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string cipher_list = fdp.ConsumeBytesAsString(cipher_list_len);
    
    // Create SSL context
    SSL_CTX *ctx = SSL_CTX_new(TLS_method());
    if (ctx == NULL) {
        return 0;
    }
    
    // Set cipher list from fuzzed input
    if (!cipher_list.empty()) {
        SSL_CTX_set_cipher_list(ctx, cipher_list.c_str());
    }
    
    // Create memory BIOs for fuzzing input/output
    BIO *rbio = BIO_new(BIO_s_mem());
    BIO *wbio = BIO_new(BIO_s_mem());
    
    if (rbio == NULL || wbio == NULL) {
        if (rbio) BIO_free(rbio);
        if (wbio) BIO_free(wbio);
        SSL_CTX_free(ctx);
        return 0;
    }
    
    int result = 0;
    
    switch (mode) {
        case 0: { // Client mode
            // Create SSL object for client
            SSL *ssl = SSL_new(ctx);
            if (ssl == NULL) {
                break;
            }
            
            // Set BIOs and client mode
            SSL_set_bio(ssl, rbio, wbio);
            SSL_set_connect_state(ssl);
            
            // Write some fuzzed data to read BIO to simulate incoming data
            size_t incoming_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            std::vector<uint8_t> incoming_data = fdp.ConsumeBytes<uint8_t>(incoming_len);
            if (!incoming_data.empty()) {
                BIO_write(rbio, incoming_data.data(), incoming_data.size());
            }
            
            // Attempt handshake (will fail without proper server, but exercises code)
            SSL_do_handshake(ssl);
            
            // Try to read/write if handshake succeeded (unlikely without real peer)
            if (SSL_is_init_finished(ssl)) {
                // Try to read any data
                char read_buf[256];
                SSL_read(ssl, read_buf, sizeof(read_buf));
                
                // Try to write some fuzzed data
                size_t write_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
                std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_len);
                if (!write_data.empty()) {
                    SSL_write(ssl, write_data.data(), write_data.size());
                }
                
                // Attempt shutdown
                SSL_shutdown(ssl);
            }
            
            SSL_free(ssl);
            break;
        }
        
        case 1: { // Server mode
            // Create SSL object for server
            SSL *ssl = SSL_new(ctx);
            if (ssl == NULL) {
                break;
            }
            
            // Set BIOs and server mode
            SSL_set_bio(ssl, rbio, wbio);
            SSL_set_accept_state(ssl);
            
            // Write some fuzzed data to read BIO to simulate client hello
            size_t client_data_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            std::vector<uint8_t> client_data = fdp.ConsumeBytes<uint8_t>(client_data_len);
            if (!client_data.empty()) {
                BIO_write(rbio, client_data.data(), client_data.size());
            }
            
            // Attempt handshake
            SSL_do_handshake(ssl);
            
            // If handshake appears to succeed, try data exchange
            if (SSL_is_init_finished(ssl)) {
                // Try to read client data
                char read_buf[256];
                SSL_read(ssl, read_buf, sizeof(read_buf));
                
                // Generate response from remaining fuzzed data
                size_t response_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
                std::vector<uint8_t> response_data = fdp.ConsumeBytes<uint8_t>(response_len);
                if (!response_data.empty()) {
                    SSL_write(ssl, response_data.data(), response_data.size());
                }
                
                // Attempt shutdown
                SSL_shutdown(ssl);
            }
            
            SSL_free(ssl);
            break;
        }
        
        case 2: { // Both client and server (more complex test)
            // Create two SSL objects to simulate bidirectional communication
            SSL *client_ssl = SSL_new(ctx);
            SSL *server_ssl = SSL_new(ctx);
            
            if (client_ssl == NULL || server_ssl == NULL) {
                if (client_ssl) SSL_free(client_ssl);
                if (server_ssl) SSL_free(server_ssl);
                break;
            }
            
            // Create separate BIO pairs for client and server
            BIO *client_rbio = BIO_new(BIO_s_mem());
            BIO *client_wbio = BIO_new(BIO_s_mem());
            BIO *server_rbio = BIO_new(BIO_s_mem());
            BIO *server_wbio = BIO_new(BIO_s_mem());
            
            if (!client_rbio || !client_wbio || !server_rbio || !server_wbio) {
                if (client_rbio) BIO_free(client_rbio);
                if (client_wbio) BIO_free(client_wbio);
                if (server_rbio) BIO_free(server_rbio);
                if (server_wbio) BIO_free(server_wbio);
                SSL_free(client_ssl);
                SSL_free(server_ssl);
                break;
            }
            
            // Set up client
            SSL_set_bio(client_ssl, client_rbio, client_wbio);
            SSL_set_connect_state(client_ssl);
            
            // Set up server
            SSL_set_bio(server_ssl, server_rbio, server_wbio);
            SSL_set_accept_state(server_ssl);
            
            // Write initial client hello data from fuzzed input
            size_t client_hello_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
            std::vector<uint8_t> client_hello = fdp.ConsumeBytes<uint8_t>(client_hello_len);
            if (!client_hello.empty()) {
                BIO_write(client_wbio, client_hello.data(), client_hello.size());
                
                // Read what client wrote and feed to server
                char buf[1024];
                int len = BIO_read(client_wbio, buf, sizeof(buf));
                if (len > 0) {
                    BIO_write(server_rbio, buf, len);
                }
            }
            
            // Attempt handshake on both sides
            SSL_do_handshake(client_ssl);
            SSL_do_handshake(server_ssl);
            
            // Try data exchange if both sides are ready
            if (SSL_is_init_finished(client_ssl) && SSL_is_init_finished(server_ssl)) {
                // Client sends data to server
                size_t client_send_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                std::vector<uint8_t> client_send_data = fdp.ConsumeBytes<uint8_t>(client_send_len);
                if (!client_send_data.empty()) {
                    SSL_write(client_ssl, client_send_data.data(), client_send_data.size());
                    
                    // Read from client wbio and write to server rbio
                    char transfer_buf[1024];
                    int transfer_len = BIO_read(client_wbio, transfer_buf, sizeof(transfer_buf));
                    if (transfer_len > 0) {
                        BIO_write(server_rbio, transfer_buf, transfer_len);
                    }
                }
                
                // Server sends response to client
                size_t server_response_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                std::vector<uint8_t> server_response_data = fdp.ConsumeBytes<uint8_t>(server_response_len);
                if (!server_response_data.empty()) {
                    SSL_write(server_ssl, server_response_data.data(), server_response_data.size());
                    
                    // Read from server wbio and write to client rbio
                    char response_buf[1024];
                    int response_len = BIO_read(server_wbio, response_buf, sizeof(response_buf));
                    if (response_len > 0) {
                        BIO_write(client_rbio, response_buf, response_len);
                    }
                }
                
                // Attempt shutdown sequences
                SSL_shutdown(client_ssl);
                SSL_shutdown(server_ssl);
            }
            
            // Clean up
            // Do NOT free BIOs after SSL_set_bio() - ownership transferred to SSL objects
            // SSL_free() will automatically free the BIOs
            SSL_free(client_ssl);
            SSL_free(server_ssl);
            break;
        }
    }
    
    // Do NOT free shared BIOs for modes 0 and 1 - ownership transferred to SSL objects via SSL_set_bio()
    // SSL_free() has already freed the BIOs
    
    SSL_CTX_free(ctx);
    
    return 0;
}

} // namespace harness_001
