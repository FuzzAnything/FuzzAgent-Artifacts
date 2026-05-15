#include <opencv2/core.hpp>
#include <opencv2/core/hal/hal.hpp>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <cmath>

namespace harness_003 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 128) return 0; // Minimum size for meaningful testing
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume parameters for array dimensions
    int n = fdp.ConsumeIntegralInRange(1, 100); // Size for 1D arrays for mathematical operations
    int m = fdp.ConsumeIntegralInRange(2, 20);  // Matrix dimension for Cholesky (must be square, at least 2x2)
    
    // Create input arrays for mathematical operations (log32f, sqrt32f, invSqrt32f)
    std::vector<float> src_math(n);
    std::vector<float> dst_math(n);
    
    // Fill src_math with valid data:
    // - For log32f: values must be > 0
    // - For sqrt32f and invSqrt32f: values must be >= 0
    // We'll generate values in range [0.1, 1000.0] to ensure positivity
    for (int i = 0; i < n; ++i) {
        // Generate positive floating point values
        float val = fdp.ConsumeFloatingPointInRange<float>(0.1f, 1000.0f);
        src_math[i] = val;
    }
    
    // Test mathematical operations
    try {
        // Test log32f - natural logarithm
        cv::hal::log32f(src_math.data(), dst_math.data(), n);
        
        // Test sqrt32f - square root
        cv::hal::sqrt32f(src_math.data(), dst_math.data(), n);
        
        // Test invSqrt32f - inverse square root (1/sqrt(x))
        cv::hal::invSqrt32f(src_math.data(), dst_math.data(), n);
    } catch (...) {
        // Ignore any exceptions from mathematical operations
    }
    
    // Create positive definite matrix for Cholesky decomposition
    // A positive definite matrix can be created as A = M*M^T + epsilon*I
    // where M is any real matrix and epsilon > 0 ensures positive definiteness
    std::vector<float> A(m * m);
    std::vector<float> b(m);  // Right-hand side vector (optional, can be NULL)
    
    // Create a random matrix M
    std::vector<float> M(m * m);
    for (int i = 0; i < m * m; ++i) {
        M[i] = fdp.ConsumeFloatingPointInRange<float>(-10.0f, 10.0f);
    }
    
    // Compute A = M*M^T + epsilon*I to ensure positive definiteness
    // First, compute M*M^T
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < m; ++k) {
                sum += M[i * m + k] * M[j * m + k];
            }
            A[i * m + j] = sum;
        }
    }
    
    // Add epsilon*I to ensure positive definiteness (diagonal dominance)
    float epsilon = fdp.ConsumeFloatingPointInRange<float>(0.1f, 10.0f);
    for (int i = 0; i < m; ++i) {
        A[i * m + i] += epsilon;
    }
    
    // Fill b vector with random values (optional for Cholesky)
    for (int i = 0; i < m; ++i) {
        b[i] = fdp.ConsumeFloatingPointInRange<float>(-10.0f, 10.0f);
    }
    
    // Test Cholesky decomposition
    try {
        // Cholesky32f signature: bool Cholesky32f(float* A, size_t astep, int m, float* b, size_t bstep, int n)
        // where A is m x m matrix, b is m x n right-hand side matrix (can be NULL)
        size_t astep = m * sizeof(float); // Assuming row-major, contiguous storage
        size_t bstep = sizeof(float);     // For vector b
        
        // Test with b = NULL (just decomposition)
        bool success1 = cv::hal::Cholesky32f(A.data(), astep, m, nullptr, 0, 0);
        
        // Test with b vector (solving linear system)
        bool success2 = cv::hal::Cholesky32f(A.data(), astep, m, b.data(), bstep, 1);
        
        // Also test with different right-hand side dimensions
        if (fdp.ConsumeBool() && m > 1) {
            int n_rhs = fdp.ConsumeIntegralInRange(1, 5);
            std::vector<float> b_multi(m * n_rhs);
            for (int i = 0; i < m * n_rhs; ++i) {
                b_multi[i] = fdp.ConsumeFloatingPointInRange<float>(-5.0f, 5.0f);
            }
            size_t bstep_multi = n_rhs * sizeof(float);
            bool success3 = cv::hal::Cholesky32f(A.data(), astep, m, b_multi.data(), bstep_multi, n_rhs);
        }
    } catch (...) {
        // Ignore any exceptions from Cholesky decomposition
    }
    
    // Additional test: Test mathematical operations on different ranges
    // Test with very small positive values (near zero)
    if (fdp.ConsumeBool() && n > 0) {
        std::vector<float> src_small(n);
        std::vector<float> dst_small(n);
        for (int i = 0; i < n; ++i) {
            src_small[i] = fdp.ConsumeFloatingPointInRange<float>(1e-10f, 1e-5f);
        }
        
        try {
            cv::hal::log32f(src_small.data(), dst_small.data(), n);
            cv::hal::sqrt32f(src_small.data(), dst_small.data(), n);
            cv::hal::invSqrt32f(src_small.data(), dst_small.data(), n);
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test with very large values
    if (fdp.ConsumeBool() && n > 0) {
        std::vector<float> src_large(n);
        std::vector<float> dst_large(n);
        for (int i = 0; i < n; ++i) {
            src_large[i] = fdp.ConsumeFloatingPointInRange<float>(1e5f, 1e10f);
        }
        
        try {
            cv::hal::log32f(src_large.data(), dst_large.data(), n);
            cv::hal::sqrt32f(src_large.data(), dst_large.data(), n);
            cv::hal::invSqrt32f(src_large.data(), dst_large.data(), n);
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    return 0;
}

} // namespace harness_003
