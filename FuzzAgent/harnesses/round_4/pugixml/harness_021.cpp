#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>

namespace harness_021 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for wide character testing
    if (size < 64) {
        return 0; // Insufficient input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume variable-size data for XML content and strings
    std::string xml_content = fdp.ConsumeRandomLengthString(4096);
    std::string test_string1 = fdp.ConsumeRandomLengthString(256);
    std::string test_string2 = fdp.ConsumeRandomLengthString(256);
    std::string test_string3 = fdp.ConsumeRandomLengthString(256);
    
    // Step 1: Parse initial XML from char buffer using load()
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(xml_content.c_str(), xml_content.size(), parse_options);
    
    // If parsing succeeded, proceed with wide character operations
    if (result) {
        pugi::xml_node root = doc.document_element();
        if (!root) {
            root = doc.append_child("root");
        }
        
        // Test different wide character operations based on operation_type
        switch (operation_type) {
            case 0:
                // Test as_wide() and as_utf8() conversion functions
                #ifndef PUGIXML_NO_STL
                {
                    // Test as_wide with various inputs
                    std::wstring wide1 = pugi::as_wide(test_string1);
                    std::wstring wide2 = pugi::as_wide(std::string(test_string2));
                    
                    // Test as_utf8 with wide inputs
                    std::string utf8_1 = pugi::as_utf8(wide1);
                    std::string utf8_2 = pugi::as_utf8(wide2);
                    
                    // Create a node and attribute to use the converted strings
                    pugi::xml_node conv_node = root.append_child("conversion_test");
                    if (conv_node) {
                        pugi::xml_attribute conv_attr = conv_node.append_attribute("test_attr");
                        if (conv_attr) {
                            // Use converted strings
                            conv_attr.set_value(utf8_1.c_str());
                        }
                    }
                }
                #endif
                break;
                
            case 1:
                // Test set_name with potential wide character strings
                {
                    pugi::xml_node test_node = root.append_child("set_name_test");
                    if (test_node) {
                        pugi::xml_attribute attr = test_node.append_attribute("initial_name");
                        if (attr) {
                            // Set attribute value first
                            attr.set_value(test_string1.c_str());
                            
                            // Try to set name using different approaches
                            #ifndef PUGIXML_NO_STL
                            // Convert string to wide and potentially back depending on char_t
                            std::wstring wide_name = pugi::as_wide(test_string2);
                            std::string converted_name = pugi::as_utf8(wide_name);
                            attr.set_name(converted_name.c_str());
                            #else
                            // Without STL, just use the string directly
                            attr.set_name(test_string2.c_str());
                            #endif
                        }
                    }
                }
                break;
                
            case 2:
                // Test print to wchar_t stream
                #ifndef PUGIXML_NO_STL
                {
                    std::wstringstream wss;
                    
                    // Print document to wide character stream
                    doc.print(wss);
                    
                    // Also test individual node printing
                    if (root) {
                        root.print(wss);
                    }
                    
                    // Get the wide string output and convert back to UTF-8
                    std::wstring woutput = wss.str();
                    if (!woutput.empty()) {
                        std::string utf8_output = pugi::as_utf8(woutput);
                        // Create a new document from the printed output
                        pugi::xml_document printed_doc;
                        pugi::xml_parse_result printed_result = printed_doc.load_buffer(utf8_output.c_str(), utf8_output.size());
                        (void)printed_result;
                    }
                }
                #endif
                break;
                
            case 3:
                // Test load from wchar_t stream
                #ifndef PUGIXML_NO_STL
                {
                    // Convert XML content to wide string
                    std::wstring wide_xml = pugi::as_wide(xml_content);
                    std::wstringstream wss;
                    wss << wide_xml;
                    
                    pugi::xml_document doc2;
                    pugi::xml_parse_result result2 = doc2.load(wss);
                    
                    if (result2) {
                        // Process the loaded document
                        pugi::xml_node root2 = doc2.document_element();
                        if (root2) {
                            // Test attribute operations on the wide-loaded document
                            pugi::xml_node test_node2 = root2.append_child("wide_load_test");
                            if (test_node2) {
                                pugi::xml_attribute attr2 = test_node2.append_attribute("wide_attr");
                                if (attr2) {
                                    attr2.set_value(test_string3.c_str());
                                }
                            }
                        }
                        
                        // Test saving the wide-loaded document
                        std::wstringstream wss2;
                        doc2.print(wss2);
                        std::wstring saved_output = wss2.str();
                        (void)saved_output;
                    }
                }
                #endif
                break;
        }
        
        // Test save_file with potential wchar_t filename
        #ifndef PUGIXML_NO_STL
        {
            // Create a wide character filename from fuzzer input
            std::wstring wfilename = pugi::as_wide(test_string1);
            if (wfilename.length() > 100) {
                wfilename = wfilename.substr(0, 100);
            }
            
            // Append .xml extension
            wfilename += L".xml";
            
            // Save using wide character filename (ignoring result since file may not be writable)
            (void)doc.save_file(wfilename.c_str());
        }
        #endif
    }
    
    // Test reset() for cleanup
    doc.reset();
    
    return 0;
}

} // namespace harness_021
