#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <stdexcept>

namespace harness_003 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0; // Need minimum input for iterator and exception testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 8;
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    uint8_t encoding_type = fdp.ConsumeIntegral<uint8_t>() % 5;
    uint8_t text_operation = fdp.ConsumeIntegral<uint8_t>() % 12;
    bool test_exceptions = fdp.ConsumeBool();
    
    // Consume variable-size data
    std::string xml_content = fdp.ConsumeRandomLengthString(4096);
    std::string node_name = fdp.ConsumeRandomLengthString(128);
    std::string attr_name = fdp.ConsumeRandomLengthString(128);
    std::string text_content = fdp.ConsumeRandomLengthString(512);
    
    // Create XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result;
    
    // Parse XML
    result = doc.load_buffer(xml_content.c_str(), xml_content.size(), parse_options);
    
    // If parsing failed, create a simple document for testing
    if (!result) {
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        for (int i = 0; i < 5; i++) {
            pugi::xml_node child = root.append_child(("node" + std::to_string(i)).c_str());
            child.append_attribute(("attr" + std::to_string(i)).c_str()) = ("value" + std::to_string(i)).c_str();
            child.text().set(("text" + std::to_string(i)).c_str());
        }
    }
    
    // Test different modes
    switch (test_mode) {
        case 0: {
            // Test xml_node_iterator patterns
            pugi::xml_node root = doc.document_element();
            if (root) {
                // Get children range using xml_object_range
                pugi::xml_object_range<pugi::xml_node_iterator> children_range = root.children();
                
                // Test iterator operations
                pugi::xml_node_iterator it_begin = children_range.begin();
                pugi::xml_node_iterator it_end = children_range.end();
                
                // Test iterator comparison
                bool iterators_equal = (it_begin == it_end);
                bool iterators_not_equal = (it_begin != it_end);
                (void)iterators_equal;
                (void)iterators_not_equal;
                
                // Test iterator traversal
                for (pugi::xml_node_iterator it = children_range.begin(); it != children_range.end(); ++it) {
                    pugi::xml_node node = *it;
                    
                    // Test node properties
                    node.name();
                    node.value();
                    node.type();
                    
                    // Test iterator post-increment
                    pugi::xml_node_iterator it_copy = it;
                    pugi::xml_node_iterator it_post = it++;
                    (void)it_post;
                    it = it_copy;
                    
                    // Test iterator decrement if possible
                    if (it != children_range.begin()) {
                        pugi::xml_node_iterator it_prev = --it;
                        (void)it_prev;
                        ++it;
                    }
                }
                
                // Test named children range
                pugi::xml_object_range<pugi::xml_named_node_iterator> named_children = root.children("node");
                for (pugi::xml_named_node_iterator nit = named_children.begin(); nit != named_children.end(); ++nit) {
                    (void)*nit;
                }
            }
            break;
        }
        
        case 1: {
            // Test xml_attribute_iterator patterns
            pugi::xml_node root = doc.document_element();
            if (root) {
                // Get attributes range using xml_object_range
                pugi::xml_object_range<pugi::xml_attribute_iterator> attrs_range = root.attributes();
                
                // Test iterator operations
                pugi::xml_attribute_iterator it_begin = attrs_range.begin();
                pugi::xml_attribute_iterator it_end = attrs_range.end();
                
                // Test iterator traversal
                for (pugi::xml_attribute_iterator it = attrs_range.begin(); it != attrs_range.end(); ++it) {
                    pugi::xml_attribute attr = *it;
                    
                    // Test attribute properties
                    attr.name();
                    attr.value();
                    attr.as_string();
                    attr.as_int();
                    attr.as_double();
                    attr.as_bool();
                    
                    // Test iterator operations
                    pugi::xml_attribute_iterator it_copy = it;
                    pugi::xml_attribute_iterator it_post = it++;
                    (void)it_post;
                    it = it_copy;
                }
            }
            break;
        }
        
        case 2: {
            // Test xml_text full API
            pugi::xml_node root = doc.document_element();
            if (root) {
                pugi::xml_text text_obj = root.text();
                
                // Test empty state
                bool is_empty = text_obj.empty();
                (void)is_empty;
                
                // Test get methods
                const char* text_value = text_obj.get();
                (void)text_value;
                
                // Test as_* conversion methods
                int int_val = text_obj.as_int(42);
                unsigned int uint_val = text_obj.as_uint(42u);
                double double_val = text_obj.as_double(42.0);
                float float_val = text_obj.as_float(42.0f);
                bool bool_val = text_obj.as_bool(false);
                
                // Test string conversion with default
                const char* string_val = text_obj.as_string("default");
                (void)string_val;
                
                // Test set methods based on text_operation
                switch (text_operation % 6) {
                    case 0:
                        text_obj.set(text_content.c_str());
                        break;
                    case 1:
                        text_obj.set(text_content.c_str(), text_content.size());
                        break;
                    case 2:
                        text_obj.set(fdp.ConsumeIntegral<int>());
                        break;
                    case 3:
                        text_obj.set(fdp.ConsumeFloatingPoint<double>());
                        break;
                    case 4:
                        text_obj.set(fdp.ConsumeBool());
                        break;
                    case 5:
                        text_obj.set(fdp.ConsumeIntegral<long>());
                        break;
                }
                
                // Test assignment operators
                switch (text_operation / 6) {
                    case 0:
                        text_obj = text_content.c_str();
                        break;
                    case 1:
                        text_obj = fdp.ConsumeIntegral<int>();
                        break;
                    case 2:
                        text_obj = fdp.ConsumeFloatingPoint<double>();
                        break;
                    case 3:
                        text_obj = fdp.ConsumeBool();
                        break;
                }
                
                // Test boolean conversion
                if (text_obj) {
                    // Text object is valid
                }
                
                // Test logical operators
                bool logical_and = text_obj && true;
                bool logical_or = text_obj || false;
                (void)logical_and;
                (void)logical_or;
            }
            break;
        }
        
        case 3: {
            // Test xpath_exception handling and error paths
            #ifndef PUGIXML_NO_EXCEPTIONS
            try {
                // Create invalid XPath expression to trigger exception
                std::string invalid_xpath = fdp.ConsumeRandomLengthString(128);
                pugi::xpath_query query(invalid_xpath.c_str());
                
                // Try to evaluate - should throw if expression is invalid
                if (doc.document_element()) {
                    pugi::xpath_node_set nodes = query.evaluate_node_set(doc.document_element());
                    (void)nodes;
                }
            } catch (const pugi::xpath_exception& e) {
                // Test exception methods
                const char* what_msg = e.what();
                const pugi::xpath_parse_result& parse_result = e.result();
                (void)what_msg;
                (void)parse_result;
            } catch (...) {
                // Catch any other exceptions
            }
            #endif
            
            // Test XPath with variables and error conditions
            pugi::xpath_variable_set vars;
            
            // Add variables with potential type mismatches
            if (fdp.ConsumeBool()) {
                vars.add("var1", pugi::xpath_type_number);
                vars.set("var1", fdp.ConsumeFloatingPoint<double>());
            }
            
            if (fdp.ConsumeBool()) {
                vars.add("var2", pugi::xpath_type_string);
                vars.set("var2", text_content.c_str());
            }
            
            // Create XPath query with variables
            std::string xpath_with_vars = fdp.ConsumeRandomLengthString(256);
            
            #ifndef PUGIXML_NO_EXCEPTIONS
            try {
                pugi::xpath_query query_with_vars(xpath_with_vars.c_str(), &vars);
                
                // Test evaluation with error handling
                pugi::xpath_parse_result parse_result = query_with_vars.result();
                (void)parse_result;
            } catch (const pugi::xpath_exception& e) {
                // Test exception methods
                const char* what_msg = e.what();
                const pugi::xpath_parse_result& parse_result = e.result();
                (void)what_msg;
                (void)parse_result;
            } catch (...) {
                // Catch any other exceptions
            }
            #else
            // Without exceptions, create query and check result
            pugi::xpath_query query_with_vars(xpath_with_vars.c_str(), &vars);
            pugi::xpath_parse_result parse_result = query_with_vars.result();
            (void)parse_result;
            #endif
            
            break;
        }
        
        case 4: {
            // Test memory allocation functions
            pugi::allocation_function alloc_func = pugi::get_memory_allocation_function();
            pugi::deallocation_function dealloc_func = pugi::get_memory_deallocation_function();
            
            // Test custom memory management
            if (alloc_func && dealloc_func) {
                // Allocate some memory using pugixml's allocator
                size_t alloc_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                void* allocated_mem = alloc_func(alloc_size);
                
                if (allocated_mem) {
                    // Use the memory
                    memset(allocated_mem, 0, alloc_size);
                    
                    // Note: pugixml doesn't have get_memory_total_allocated or get_memory_total_deallocated functions
                    // These were incorrectly assumed to exist
                    (void)alloc_size;
                    
                    // Deallocate the memory
                    dealloc_func(allocated_mem);
                }
            }
            
            // Test set_memory_management_functions
            if (test_exceptions) {
                // Note: We can't actually set custom functions in a fuzzing harness
                // as it would affect global state, but we can test the API exists
                // by calling get functions
                (void)pugi::get_memory_allocation_function();
                (void)pugi::get_memory_deallocation_function();
            }
            break;
        }
        
        case 5: {
            // Test advanced XPath features
            std::string complex_xpath = fdp.ConsumeRandomLengthString(512);
            
            #ifndef PUGIXML_NO_EXCEPTIONS
            try {
                pugi::xpath_query complex_query(complex_xpath.c_str());
                
                if (complex_query) {
                    pugi::xml_node root = doc.document_element();
                    if (root) {
                        // Test different evaluation methods
                        switch (fdp.ConsumeIntegral<uint8_t>() % 4) {
                            case 0: {
                                pugi::xpath_node_set node_set = complex_query.evaluate_node_set(root);
                                
                                // Test xpath_node_set iterator patterns
                                for (pugi::xpath_node_set::const_iterator it = node_set.begin(); it != node_set.end(); ++it) {
                                    pugi::xml_node node = it->node();
                                    pugi::xml_attribute attr = it->attribute();
                                    (void)node;
                                    (void)attr;
                                }
                                
                                // Test xpath_node_set properties
                                size_t set_size = node_set.size();
                                bool set_empty = node_set.empty();
                                pugi::xpath_node_set::type_t set_type = node_set.type();
                                pugi::xpath_node first_node = node_set.first();
                                
                                // Only access last node if set is not empty
                                if (set_size > 0) {
                                    pugi::xpath_node last_node = node_set[set_size - 1];
                                    (void)last_node;
                                }
                                
                                (void)set_size;
                                (void)set_empty;
                                (void)set_type;
                                (void)first_node;
                                break;
                            }
                            case 1: {
                                double number_result = complex_query.evaluate_number(root);
                                (void)number_result;
                                break;
                            }
                            case 2: {
                                std::string string_result = complex_query.evaluate_string(root);
                                (void)string_result;
                                break;
                            }
                            case 3: {
                                bool bool_result = complex_query.evaluate_boolean(root);
                                (void)bool_result;
                                break;
                            }
                        }
                    }
                }
            } catch (const pugi::xpath_exception& e) {
                // Test exception methods
                const char* what_msg = e.what();
                const pugi::xpath_parse_result& parse_result = e.result();
                (void)what_msg;
                (void)parse_result;
            } catch (...) {
                // Catch any other exceptions
            }
            #else
            // Without exceptions, create query and check result
            pugi::xpath_query complex_query(complex_xpath.c_str());
            if (complex_query) {
                pugi::xml_node root = doc.document_element();
                if (root) {
                    // Test different evaluation methods (same as above)
                    switch (fdp.ConsumeIntegral<uint8_t>() % 4) {
                        case 0: {
                            pugi::xpath_node_set node_set = complex_query.evaluate_node_set(root);
                            
                            // Test xpath_node_set iterator patterns
                            for (pugi::xpath_node_set::const_iterator it = node_set.begin(); it != node_set.end(); ++it) {
                                pugi::xml_node node = it->node();
                                pugi::xml_attribute attr = it->attribute();
                                (void)node;
                                (void)attr;
                            }
                            
                            // Test xpath_node_set properties
                            size_t set_size = node_set.size();
                            bool set_empty = node_set.empty();
                            pugi::xpath_node_set::type_t set_type = node_set.type();
                            pugi::xpath_node first_node = node_set.first();
                            
                            // Only access last node if set is not empty
                            if (set_size > 0) {
                                pugi::xpath_node last_node = node_set[set_size - 1];
                                (void)last_node;
                            }
                            
                            (void)set_size;
                            (void)set_empty;
                            (void)set_type;
                            (void)first_node;
                            break;
                        }
                        case 1: {
                            double number_result = complex_query.evaluate_number(root);
                            (void)number_result;
                            break;
                        }
                        case 2: {
                            std::string string_result = complex_query.evaluate_string(root);
                            (void)string_result;
                            break;
                        }
                        case 3: {
                            bool bool_result = complex_query.evaluate_boolean(root);
                            (void)bool_result;
                            break;
                        }
                    }
                }
            }
            #endif
            break;
        }
        
        case 6: {
            // Test XML encoding conversion
            pugi::xml_node root = doc.document_element();
            if (root) {
                // Create a writer to test encoding
                std::ostringstream oss;
                pugi::xml_writer_stream writer(oss);
                
                // Save with different encodings
                pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(encoding_type);
                
                // Test different format flags
                unsigned int format_flags = fdp.ConsumeIntegral<unsigned int>();
                std::string indent = fdp.ConsumeRandomLengthString(8);
                
                doc.save(writer, indent.c_str(), format_flags, encoding);
                
                // Test node print with encoding
                if (root) {
                    root.print(writer, indent.c_str(), format_flags, encoding);
                }
                
                // Note: pugixml doesn't have get_encoding_name or parse_encoding functions
                // We'll test encoding by using it in save/print functions
                (void)encoding;
            }
            break;
        }
        
        case 7: {
            // Test xml_object_range with various iterators
            pugi::xml_node root = doc.document_element();
            if (root) {
                // Test children range
                pugi::xml_object_range<pugi::xml_node_iterator> child_range = root.children();
                
                // Use range-based for loop (requires C++11)
                #if __cplusplus >= 201103L
                for (pugi::xml_node child : child_range) {
                    (void)child;
                }
                #endif
                
                // Test attributes range
                pugi::xml_object_range<pugi::xml_attribute_iterator> attr_range = root.attributes();
                
                // Test range properties
                pugi::xml_attribute_iterator attr_begin = attr_range.begin();
                pugi::xml_attribute_iterator attr_end = attr_range.end();
                
                // Test distance between iterators
                if (attr_begin != attr_end) {
                    // Can't use std::distance in C++98, but we can manually count
                    size_t count = 0;
                    for (pugi::xml_attribute_iterator it = attr_begin; it != attr_end; ++it) {
                        count++;
                    }
                    (void)count;
                }
                
                // Test named node range
                std::string search_name = fdp.ConsumeRandomLengthString(64);
                pugi::xml_object_range<pugi::xml_named_node_iterator> named_range = root.children(search_name.c_str());
                
                // Test the range
                for (pugi::xml_named_node_iterator it = named_range.begin(); it != named_range.end(); ++it) {
                    (void)*it;
                }
            }
            break;
        }
    }
    
    // Additional iterator and exception testing
    if (test_exceptions) {
        // Test edge cases for iterators
        pugi::xml_node root = doc.document_element();
        if (root) {
            // Test empty ranges by creating empty xml_object_range manually
            // (xml_node_iterator doesn't have range() method)
            pugi::xml_object_range<pugi::xml_node_iterator> empty_range(pugi::xml_node_iterator(), pugi::xml_node_iterator());
            (void)empty_range;
            
            pugi::xml_object_range<pugi::xml_attribute_iterator> empty_attr_range(pugi::xml_attribute_iterator(), pugi::xml_attribute_iterator());
            (void)empty_attr_range;

            // Test iterator default construction
            pugi::xml_node_iterator default_node_iter;
            pugi::xml_attribute_iterator default_attr_iter;
            (void)default_node_iter;
            (void)default_attr_iter;
            
            // Test iterator comparison with default
            bool node_iter_eq = (default_node_iter == default_node_iter);
            bool attr_iter_eq = (default_attr_iter == default_attr_iter);
            (void)node_iter_eq;
            (void)attr_iter_eq;
        }
    }
    
    // Test document saving with various options
    if (fdp.remaining_bytes() > 100) {
        std::ostringstream save_stream;
        pugi::xml_writer_stream writer(save_stream);
        
        unsigned int save_flags = fdp.ConsumeIntegral<unsigned int>();
        std::string save_indent = fdp.ConsumeRandomLengthString(16);
        pugi::xml_encoding save_encoding = static_cast<pugi::xml_encoding>(fdp.ConsumeIntegral<uint8_t>() % 5);
        
        doc.save(writer, save_indent.c_str(), save_flags, save_encoding);
    }
    
    return 0;
}

} // namespace harness_003
