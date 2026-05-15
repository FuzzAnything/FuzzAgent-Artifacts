#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <iterator>
#include <cstring>
#include <algorithm>
#include <stdexcept>

namespace harness_007 {

// Custom memory management functions for testing
static void* custom_allocate(size_t size) {
    return malloc(size);
}

static void custom_deallocate(void* ptr) {
    free(ptr);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for comprehensive testing
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 8;
    uint8_t parse_flags = fdp.ConsumeIntegral<uint8_t>();
    uint8_t encoding_type = fdp.ConsumeIntegral<uint8_t>() % 10;  // Up to 9 encoding types
    uint8_t iterator_pattern = fdp.ConsumeIntegral<uint8_t>() % 6;
    bool test_memory_functions = fdp.ConsumeBool();
    bool use_string_view = fdp.ConsumeBool();
    
    // Consume variable-size data
    std::string xml_content = fdp.ConsumeRandomLengthString(4096);
    std::string node_name = fdp.ConsumeRandomLengthString(128);
    std::string attr_name = fdp.ConsumeRandomLengthString(128);
    std::string text_content = fdp.ConsumeRandomLengthString(512);
    
    // Create XML document with specific parsing flags for different node types
    pugi::xml_document doc;
    pugi::xml_parse_result result;
    
    // Configure parse flags to include specific node types
    unsigned int actual_parse_flags = parse_flags;
    
    // Ensure specific node types are parsed based on test mode
    switch (test_mode % 4) {
        case 0:
            // Test processing instructions (node_pi)
            actual_parse_flags |= pugi::parse_pi;
            break;
        case 1:
            // Test CDATA sections (node_cdata) - already enabled by default
            actual_parse_flags |= pugi::parse_cdata;
            break;
        case 2:
            // Test comments (node_comment)
            actual_parse_flags |= pugi::parse_comments;
            break;
        case 3:
            // Test document type (node_doctype)
            actual_parse_flags |= pugi::parse_doctype;
            break;
    }
    
    // Always include full parsing for comprehensive testing
    actual_parse_flags |= pugi::parse_full;
    
    // Set encoding based on consumed encoding type
    pugi::xml_encoding encoding = pugi::encoding_auto;
    switch (encoding_type) {
        case 0: encoding = pugi::encoding_auto; break;
        case 1: encoding = pugi::encoding_utf8; break;
        case 2: encoding = pugi::encoding_utf16_le; break;
        case 3: encoding = pugi::encoding_utf16_be; break;
        case 4: encoding = pugi::encoding_utf16; break;
        case 5: encoding = pugi::encoding_utf32_le; break;
        case 6: encoding = pugi::encoding_utf32_be; break;
        case 7: encoding = pugi::encoding_utf32; break;
        case 8: encoding = pugi::encoding_wchar; break;
        case 9: encoding = pugi::encoding_latin1; break;
    }
    
    // Test memory management functions if requested
    if (test_memory_functions && fdp.remaining_bytes() > 100) {
        // Get current memory functions
        pugi::allocation_function orig_alloc = pugi::get_memory_allocation_function();
        pugi::deallocation_function orig_dealloc = pugi::get_memory_deallocation_function();
        
        // Set custom memory management functions
        pugi::set_memory_management_functions(custom_allocate, custom_deallocate);
        
        // Parse with custom memory functions
        result = doc.load_buffer(xml_content.c_str(), xml_content.size(), actual_parse_flags, encoding);
        
        // Restore original memory functions
        pugi::set_memory_management_functions(orig_alloc, orig_dealloc);
    } else {
        // Parse with default memory functions
        result = doc.load_buffer(xml_content.c_str(), xml_content.size(), actual_parse_flags, encoding);
    }
    
    // If parsing failed, create a structured document with diverse node types
    if (!result) {
        doc.reset();
        
        // Create document with declaration
        pugi::xml_node decl = doc.append_child(pugi::node_declaration);
        decl.append_attribute("version") = "1.0";
        decl.append_attribute("encoding") = "UTF-8";
        
        // Create document type
        pugi::xml_node doctype = doc.append_child(pugi::node_doctype);
        doctype.set_value("root [\n  <!ELEMENT root (child+)>\n  <!ELEMENT child (#PCDATA)>\n]");
        
        // Create root element
        pugi::xml_node root = doc.append_child("root");
        
        // Add processing instruction
        pugi::xml_node pi = root.append_child(pugi::node_pi);
        pi.set_value("xml-stylesheet type=\"text/xsl\" href=\"style.xsl\"");
        
        // Add comment
        pugi::xml_node comment = root.append_child(pugi::node_comment);
        comment.set_value(" This is a test comment ");
        
        // Add CDATA section
        pugi::xml_node cdata = root.append_child(pugi::node_cdata);
        cdata.set_value("<not>escaped</not> content");
        
        // Add regular elements
        for (int i = 0; i < 5; i++) {
            pugi::xml_node child = root.append_child("child");
            child.append_attribute("id") = std::to_string(i).c_str();
            child.append_attribute("type") = (i % 2 == 0) ? "even" : "odd";
            child.text().set(("Text content " + std::to_string(i)).c_str());
        }
    }
    
    // Get document root
    pugi::xml_node root = doc.document_element();
    if (!root) {
        root = doc.first_child();
        if (!root) {
            return 0;
        }
    }
    
    // Test comprehensive iterator patterns
    switch (iterator_pattern) {
        case 0: {
            // Test xml_node_iterator with xml_object_range
            pugi::xml_object_range<pugi::xml_node_iterator> children_range = root.children();
            
            // Test begin/end iterators
            pugi::xml_node_iterator it_begin = children_range.begin();
            pugi::xml_node_iterator it_end = children_range.end();
            
            // Test iterator traversal with different patterns
            for (pugi::xml_node_iterator it = it_begin; it != it_end; ++it) {
                pugi::xml_node node = *it;
                
                // Test node type enumeration
                pugi::xml_node_type node_type = node.type();
                switch (node_type) {
                    case pugi::node_null:
                    case pugi::node_document:
                    case pugi::node_element:
                    case pugi::node_pcdata:
                    case pugi::node_cdata:
                    case pugi::node_comment:
                    case pugi::node_pi:
                    case pugi::node_declaration:
                    case pugi::node_doctype:
                        // Just access the type
                        break;
                }
                
                // Test iterator post-increment
                pugi::xml_node_iterator it_post = it++;
                --it; // Go back
                
                // Test iterator decrement if possible
                if (it != it_begin) {
                    pugi::xml_node_iterator it_prev = --it;
                    ++it;
                }
            }
            break;
        }
        
        case 1: {
            // Test xml_attribute_iterator with xml_object_range
            pugi::xml_object_range<pugi::xml_attribute_iterator> attrs_range = root.attributes();
            
            // Test attribute iterator traversal
            for (pugi::xml_attribute_iterator ait = attrs_range.begin(); ait != attrs_range.end(); ++ait) {
                pugi::xml_attribute attr = *ait;
                
                // Test attribute properties
                std::string name = attr.name();
                std::string value = attr.value();
                
                // Test iterator post-increment
                pugi::xml_attribute_iterator ait_post = ait++;
                --ait;
                
                // Test iterator decrement
                if (ait != attrs_range.begin()) {
                    pugi::xml_attribute_iterator ait_prev = --ait;
                    ++ait;
                }
            }
            break;
        }
        
        case 2: {
            // Test xml_named_node_iterator
            std::string target_name = fdp.ConsumeRandomLengthString(32);
            pugi::xml_object_range<pugi::xml_named_node_iterator> named_range = root.children(target_name.c_str());
            
            // Test named iterator traversal
            for (pugi::xml_named_node_iterator nit = named_range.begin(); nit != named_range.end(); ++nit) {
                pugi::xml_node node = *nit;
                
                // Test iterator operations
                pugi::xml_named_node_iterator nit_copy = nit;
                pugi::xml_named_node_iterator nit_post = nit++;
                nit = nit_copy;
                
                // Test iterator decrement
                if (nit != named_range.begin()) {
                    pugi::xml_named_node_iterator nit_prev = --nit;
                    ++nit;
                }
            }
            break;
        }
        
        case 3: {
            // Test mixed iterator patterns
            // First test node iterators
            pugi::xml_object_range<pugi::xml_node_iterator> node_range = root.children();
            for (pugi::xml_node_iterator it = node_range.begin(); it != node_range.end(); ++it) {
                pugi::xml_node node = *it;
                
                // Then test attribute iterators on each node
                pugi::xml_object_range<pugi::xml_attribute_iterator> attr_range = node.attributes();
                for (pugi::xml_attribute_iterator ait = attr_range.begin(); ait != attr_range.end(); ++ait) {
                    pugi::xml_attribute attr = *ait;
                    // Access attribute properties
                    attr.name();
                    attr.value();
                    attr.as_string();
                    attr.as_int();
                    attr.as_double();
                    attr.as_bool();
                }
            }
            break;
        }
        
        case 4: {
            // Test reverse iterator patterns (manual)
            pugi::xml_object_range<pugi::xml_node_iterator> range = root.children();
            pugi::xml_node_iterator it_end = range.end();
            
            // Manual reverse traversal
            if (it_end != range.begin()) {
                pugi::xml_node_iterator it = it_end;
                do {
                    --it;
                    pugi::xml_node node = *it;
                    node.name();
                    node.value();
                } while (it != range.begin());
            }
            break;
        }
        
        case 5: {
            // Test range-based for loop with xml_object_range (C++11)
            for (pugi::xml_node child : root.children()) {
                child.name();
                child.value();
                
                // Test nested range-based for loops
                for (pugi::xml_attribute attr : child.attributes()) {
                    attr.name();
                    attr.value();
                }
            }
            break;
        }
    }
    
    // Test string_view interfaces if available and requested
#ifdef PUGIXML_HAS_STRING_VIEW
    if (use_string_view && fdp.remaining_bytes() > 50) {
        // Create string_view from fuzzer input
        std::string view_content = fdp.ConsumeRandomLengthString(256);
        pugi::string_view_t sv(view_content.data(), view_content.size());
        
        // Test string_view interfaces
        try {
            // Test node methods with string_view
            pugi::xml_node found_child = root.child(sv);
            pugi::xml_attribute found_attr = root.attribute(sv);
            pugi::xml_node found_sibling = root.next_sibling(sv);
            pugi::xml_node found_prev_sibling = root.previous_sibling(sv);
            
            // Test modification with string_view
            if (root) {
                root.set_name(sv);
                root.set_value(sv);
                
                // Test attribute operations with string_view
                pugi::xml_attribute new_attr = root.append_attribute(sv);
                if (new_attr) {
                    new_attr.set_name(sv);
                    new_attr.set_value(sv);
                }
                
                // Test child operations with string_view
                pugi::xml_node new_child = root.append_child(sv);
                if (new_child) {
                    new_child.set_name(sv);
                    new_child.set_value(sv);
                }
            }
        } catch (...) {
            // Ignore exceptions from string_view operations
        }
    }
#endif
    
    // Test encoding conversion functions
    if (fdp.remaining_bytes() > 100) {
#ifndef PUGIXML_NO_STL
        try {
            // Test UTF8 conversion
            std::string utf8_test = fdp.ConsumeRandomLengthString(128);
            std::wstring wide_result = pugi::as_wide(utf8_test.c_str());
            
            // Test wide to UTF8 conversion
            if (!wide_result.empty()) {
                std::string utf8_result = pugi::as_utf8(wide_result.c_str());
            }
        } catch (...) {
            // Ignore conversion exceptions
        }
#endif
    }
    
    // Test XML parsing with different encoding combinations
    if (fdp.remaining_bytes() > 200) {
        // Create additional document with different encoding
        pugi::xml_document doc2;
        std::vector<uint8_t> buffer_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        if (!buffer_data.empty()) {
            // Test different encoding types
            pugi::xml_encoding test_encoding = pugi::encoding_auto;
            switch (fdp.ConsumeIntegral<uint8_t>() % 5) {
                case 0: test_encoding = pugi::encoding_utf8; break;
                case 1: test_encoding = pugi::encoding_utf16_le; break;
                case 2: test_encoding = pugi::encoding_utf16_be; break;
                case 3: test_encoding = pugi::encoding_latin1; break;
                case 4: test_encoding = pugi::encoding_auto; break;
            }
            
            pugi::xml_parse_result result2 = doc2.load_buffer(
                buffer_data.data(), 
                buffer_data.size(), 
                actual_parse_flags, 
                test_encoding
            );
            
            // If successful, test iterator patterns on the new document
            if (result2) {
                pugi::xml_node root2 = doc2.document_element();
                if (root2) {
                    // Quick iterator test
                    for (pugi::xml_node child : root2.children()) {
                        child.name();
                        child.value();
                    }
                }
            }
        }
    }
    
    // Cleanup - document will be automatically destroyed
    // If we set custom memory functions, they should have been restored
    
    return 0;
}

} // namespace harness_007
