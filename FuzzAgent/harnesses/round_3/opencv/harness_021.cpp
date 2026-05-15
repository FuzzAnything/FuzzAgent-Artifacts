#include <cstddef>
#include <cstdint>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn.hpp>

namespace harness_021 {


using namespace cv;
using namespace cv::dnn;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for basic parameters and some model/image data
    const size_t MIN_SIZE = sizeof(int) * 6 + sizeof(double) * 3 + 1024; // Basic params + some data
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Consume parameters for network type selection
    uint8_t net_type = fdp.ConsumeIntegral<uint8_t>() % 5; // 0: Caffe, 1: TensorFlow, 2: Darknet, 3: ONNX, 4: TFLite
    
    // Step 2: Consume model configuration and weights data
    // For fuzzing, we'll use buffer-based network loading functions
    size_t model_config_size = fdp.ConsumeIntegralInRange<size_t>(100, 4096);
    size_t model_weights_size = fdp.ConsumeIntegralInRange<size_t>(100, 4096);
    
    if (fdp.remaining_bytes() < model_config_size + model_weights_size + 1024) {
        // Not enough data for model and image
        return 0;
    }
    
    std::vector<uchar> config_buffer = fdp.ConsumeBytes<uchar>(model_config_size);
    std::vector<uchar> weights_buffer = fdp.ConsumeBytes<uchar>(model_weights_size);
    
    // Step 3: Create and load network based on selected type
    Net net;
    try {
        switch (net_type) {
            case 0: // Caffe
                if (config_buffer.size() > 0 && weights_buffer.size() > 0) {
                    net = readNetFromCaffe(config_buffer, weights_buffer);
                }
                break;
            case 1: // TensorFlow
                if (config_buffer.size() > 0 && weights_buffer.size() > 0) {
                    net = readNetFromTensorflow(weights_buffer, config_buffer);
                }
                break;
            case 2: // Darknet
                if (config_buffer.size() > 0 && weights_buffer.size() > 0) {
                    net = readNetFromDarknet(config_buffer, weights_buffer);
                }
                break;
            case 3: // ONNX
                if (weights_buffer.size() > 0) {
                    net = readNetFromONNX(weights_buffer);
                }
                break;
            case 4: // TFLite
                if (weights_buffer.size() > 0) {
                    net = readNetFromTFLite(weights_buffer);
                }
                break;
        }
    } catch (...) {
        // Network loading failed, which is expected with random data
        return 0;
    }
    
    // If network is empty (loading failed), we can still test other APIs
    // but we need at least some basic network structure for setInput/forward
    
    // Step 4: Create random image for blob creation
    int width = fdp.ConsumeIntegralInRange<int>(10, 640);
    int height = fdp.ConsumeIntegralInRange<int>(10, 480);
    int channels = fdp.ConsumeIntegralInRange<int>(1, 4);
    
    int image_type = CV_8UC1;
    if (channels == 1) image_type = CV_8UC1;
    else if (channels == 3) image_type = CV_8UC3;
    else if (channels == 4) image_type = CV_8UC4;
    
    // Create image with random pixel data
    cv::Mat image(height, width, image_type);
    size_t image_bytes = image.total() * image.elemSize();
    
    if (fdp.remaining_bytes() >= image_bytes) {
        std::vector<uchar> image_data = fdp.ConsumeBytes<uchar>(image_bytes);
        if (image_data.size() == image_bytes) {
            std::memcpy(image.data, image_data.data(), image_bytes);
        }
    } else {
        // Not enough data for full image, create smaller image
        width = std::min(width, 100);
        height = std::min(height, 100);
        image = cv::Mat(height, width, image_type);
        image_bytes = image.total() * image.elemSize();
        
        if (fdp.remaining_bytes() >= image_bytes) {
            std::vector<uchar> image_data = fdp.ConsumeBytes<uchar>(image_bytes);
            if (image_data.size() == image_bytes) {
                std::memcpy(image.data, image_data.data(), image_bytes);
            }
        }
    }
    
    // Step 5: Create blob from image using blobFromImage
    cv::Mat blob;
    double scale_factor = fdp.ConsumeFloatingPointInRange<double>(0.1, 2.0);
    cv::Size blob_size = cv::Size(width, height);
    cv::Scalar mean = cv::Scalar(fdp.ConsumeFloatingPoint<double>(), 
                                 fdp.ConsumeFloatingPoint<double>(),
                                 fdp.ConsumeFloatingPoint<double>(),
                                 fdp.ConsumeFloatingPoint<double>());
    bool swap_rb = fdp.ConsumeBool();
    bool crop = fdp.ConsumeBool();
    int ddepth = CV_32F;
    
    try {
        blobFromImage(image, blob, scale_factor, blob_size, mean, swap_rb, crop, ddepth);
    } catch (...) {
        // blobFromImage may fail with invalid parameters, continue with other tests
    }
    
    // Step 6: Set input and run forward pass if we have a valid network
    if (!net.empty() && !blob.empty()) {
        try {
            // Set the blob as network input
            net.setInput(blob);
            
            // Run forward pass
            cv::Mat output = net.forward();
            
            // Also test forward with output layer names
            std::vector<cv::String> out_names = net.getUnconnectedOutLayersNames();
            if (!out_names.empty()) {
                std::vector<cv::Mat> outputs;
                net.forward(outputs, out_names);
            }
            
            // Test async forward if supported
            if (fdp.ConsumeBool()) {
                try {
                    cv::AsyncArray async_output = net.forwardAsync();
                } catch (...) {
                    // Async may not be supported, ignore
                }
            }
            
        } catch (...) {
            // Forward pass may fail with random model/data, which is expected
        }
    }
    
    // Step 7: Test blobFromImages with multiple images
    if (fdp.remaining_bytes() > 1024) {
        std::vector<cv::Mat> images;
        int num_images = fdp.ConsumeIntegralInRange<int>(1, 3);
        
        for (int i = 0; i < num_images && fdp.remaining_bytes() > 512; i++) {
            int img_w = fdp.ConsumeIntegralInRange<int>(10, 200);
            int img_h = fdp.ConsumeIntegralInRange<int>(10, 200);
            cv::Mat img(img_h, img_w, CV_8UC3);
            size_t img_bytes = img.total() * img.elemSize();
            
            if (fdp.remaining_bytes() >= img_bytes) {
                std::vector<uchar> img_data = fdp.ConsumeBytes<uchar>(img_bytes);
                if (img_data.size() == img_bytes) {
                    std::memcpy(img.data, img_data.data(), img_bytes);
                    images.push_back(img);
                }
            }
        }
        
        if (!images.empty()) {
            try {
                cv::Mat multi_blob;
                blobFromImages(images, multi_blob, scale_factor, blob_size, mean, swap_rb, crop, ddepth);
            } catch (...) {
                // Ignore failures
            }
        }
    }
    
    // Step 8: Test network configuration APIs
    if (!net.empty()) {
        try {
            // Set backend and target preferences
            int backend = fdp.ConsumeIntegralInRange<int>(0, 5);
            int target = fdp.ConsumeIntegralInRange<int>(0, 7);
            net.setPreferableBackend(backend);
            net.setPreferableTarget(target);
            
            // Get network information
            std::vector<int> out_layers = net.getUnconnectedOutLayers();
            std::vector<cv::String> layer_names = net.getLayerNames();
            
            if (!layer_names.empty()) {
                // Try to get layer by ID
                int layer_id = fdp.ConsumeIntegralInRange<int>(0, (int)layer_names.size() - 1);
                cv::Ptr<cv::dnn::Layer> layer = net.getLayer(layer_id);
                
                // Get layer by name
                if (!layer_names.empty()) {
                    cv::Ptr<cv::dnn::Layer> layer_by_name = net.getLayer(layer_names[0]);
                }
            }
            
        } catch (...) {
            // Ignore API failures
        }
    }
    
    // Network destructor will be called automatically here
    
    return 0;
}

} // namespace harness_021
