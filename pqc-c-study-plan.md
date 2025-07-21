# 3-Month C Study Plan for Post-Quantum Cryptography Implementation

## Overview
This plan takes you from C basics to implementing real PQC algorithms. Each week includes theory, code examples, and exercises. By the end, you'll be able to read and write production PQC code.

---

## Month 1: PQC-Specific C Foundations

### Week 1: Fixed-Size Integer Arithmetic
**Goal**: Master the integer types and modular arithmetic used in every PQC scheme.

#### Day 1-2: Integer Types and Overflow
```c
#include <stdint.h>
#include <stdio.h>

// PQC uses specific bit widths everywhere
void understand_types() {
    uint8_t  byte = 255;      // 0 to 255
    uint16_t half = 65535;    // 0 to 65,535  
    uint32_t word = 0xFFFFFFFF;
    uint64_t dword = 0xFFFFFFFFFFFFFFFF;
    
    // Overflow behavior is defined for unsigned
    byte += 1;  // Wraps to 0
    printf("255 + 1 = %u\n", byte);
    
    // Signed overflow is undefined! Never use in crypto
    int16_t signed_bad = 32767;
    // signed_bad += 1;  // DON'T DO THIS
}

// Exercise 1: Implement safe addition with overflow detection
int safe_add_u16(uint16_t a, uint16_t b, uint16_t *result) {
    uint32_t sum = (uint32_t)a + (uint32_t)b;
    if (sum > UINT16_MAX) return -1;  // Overflow
    *result = (uint16_t)sum;
    return 0;
}
```

#### Day 3-4: Modular Arithmetic Basics
```c
// Kyber uses q = 3329, a prime for NTT-friendly arithmetic
#define KYBER_Q 3329

// Naive modular reduction (slow but clear)
uint16_t mod_naive(uint32_t a) {
    return a % KYBER_Q;
}

// Barrett reduction - constant time, no division
uint16_t barrett_reduce(uint16_t a) {
    uint32_t v = ((uint32_t) 20159 * a) >> 26;
    return a - v * KYBER_Q;
}

// Exercise 2: Implement Montgomery reduction
// Hint: R = 2^16, R^(-1) mod q = 169, q'= -q^(-1) mod R = 3327
uint16_t montgomery_reduce(uint32_t a) {
    uint16_t t = (uint16_t)(a * 3327);
    uint32_t t2 = (uint32_t)t * KYBER_Q;
    t2 = a - t2;
    t2 >>= 16;
    return (uint16_t)t2;
}
```

#### Day 5-7: Bitwise Operations for Crypto
```c
// Constant-time conditional move (fundamental PQC operation)
void cmov(uint8_t *r, const uint8_t *x, size_t len, uint8_t b) {
    // b must be 0 or 1
    // if b=1, copy x to r; if b=0, keep r unchanged
    // NO BRANCHES ALLOWED
    b = -b;  // 0x00 or 0xFF
    for(size_t i = 0; i < len; i++) {
        r[i] ^= b & (x[i] ^ r[i]);
    }
}

// Extract/pack bits (for compressed public keys)
void pack_bits(uint8_t *out, const uint16_t *in, size_t bits) {
    // Exercise 3: Pack array of 16-bit values using 'bits' bits each
    // E.g., pack three 10-bit values into 30 bits (4 bytes)
}

// Constant-time comparison
int ct_compare(const uint8_t *a, const uint8_t *b, size_t len) {
    uint8_t d = 0;
    for(size_t i = 0; i < len; i++) {
        d |= a[i] ^ b[i];
    }
    return (int)((d - 1) >> 8) & 1;  // 1 if equal, 0 if different
}
```

**Week 1 Project**: Build a modular arithmetic library with all operations in constant time.

---

### Week 2: Memory Patterns in PQC
**Goal**: Understand stack allocation, alignment, and memory safety patterns.

#### Day 1-2: Stack-Based Memory Management
```c
// PQC avoids malloc - everything on stack
typedef struct {
    uint16_t coeffs[256];  // Polynomial coefficients
} poly;

typedef struct {
    poly vec[4];  // Vector of polynomials
} polyvec;

// Stack allocation patterns
void pqc_patterns() {
    // Pattern 1: Fixed-size arrays
    uint8_t seed[32];  // Seeds are always 32 bytes
    uint8_t pk[800];   // Public key fixed size
    
    // Pattern 2: Aligned allocation for SIMD
    __attribute__((aligned(32))) uint16_t aligned_poly[256];
    
    // Pattern 3: Temporary workspace
    uint16_t workspace[512];  // Reused for different operations
    
    // Pattern 4: Zeroing sensitive data
    explicit_bzero(seed, 32);  // Not memset (can be optimized away)
}

// Safe array operations
void poly_add(poly *r, const poly *a, const poly *b) {
    // No bounds checking needed - size known at compile time
    for(int i = 0; i < 256; i++) {
        r->coeffs[i] = (a->coeffs[i] + b->coeffs[i]) % KYBER_Q;
    }
}
```

#### Day 3-4: Endianness and Serialization
```c
// PQC uses little-endian serialization
void store16_le(uint8_t *buf, uint16_t x) {
    buf[0] = x & 0xFF;
    buf[1] = x >> 8;
}

uint16_t load16_le(const uint8_t *buf) {
    return (uint16_t)buf[0] | ((uint16_t)buf[1] << 8);
}

// Pack polynomial for transmission
void poly_tobytes(uint8_t *r, const poly *a) {
    for(int i = 0; i < 128; i++) {
        uint16_t t0 = a->coeffs[2*i];
        uint16_t t1 = a->coeffs[2*i+1];
        r[3*i] = t0 & 0xFF;
        r[3*i+1] = (t0 >> 8) | ((t1 & 0x0F) << 4);
        r[3*i+2] = t1 >> 4;
    }
}

// Exercise 4: Implement poly_frombytes (inverse of above)
void poly_frombytes(poly *r, const uint8_t *a) {
    // Your code here
}
```

#### Day 5-7: Hash Functions and RNGs
```c
#include <openssl/sha.h>

// Deterministic key generation from seed
void expand_seed(uint8_t *output, size_t outlen, 
                 const uint8_t *seed, uint8_t nonce) {
    SHA3_CTX ctx;
    SHA3_256_Init(&ctx);
    SHA3_Update(&ctx, seed, 32);
    SHA3_Update(&ctx, &nonce, 1);
    SHA3_Final(output, &ctx);
    
    // For longer output, use SHAKE
    if (outlen > 32) {
        // Use SHAKE128 for arbitrary-length output
    }
}

// Rejection sampling for uniform distribution
void sample_uniform(uint16_t *out, size_t len, const uint8_t *seed) {
    uint8_t buf[256];
    size_t i = 0, j = 0;
    
    while(i < len) {
        SHA3_256(buf, seed, 32);
        for(j = 0; j < 256 && i < len; j += 2) {
            uint16_t t = load16_le(&buf[j]) & 0x1FFF;  // 13 bits
            if(t < KYBER_Q) {
                out[i++] = t;
            }
        }
    }
}
```

**Week 2 Project**: Implement a complete polynomial packing/unpacking library with hashing.

---

### Week 3: Advanced Arithmetic Patterns
**Goal**: Implement the core mathematical operations used in lattice-based crypto.

#### Day 1-3: Number Theoretic Transform (NTT)
```c
// NTT is the PQC equivalent of FFT
#define ZETA 17  // Primitive 512-th root of unity mod 3329

// Precomputed powers of zeta
static const uint16_t zetas[128] = {
    // Bit-reversed order for NTT
    2285, 2571, 2970, 1812, /* ... more values ... */
};

// Cooley-Tukey NTT (most important algorithm in lattice crypto)
void ntt(uint16_t r[256]) {
    unsigned int j, k = 1;
    uint16_t zeta;
    
    // 7 layers of butterflies
    for(int level = 7; level >= 0; level--) {
        for(int start = 0; start < 256; start += 2 * (1 << level)) {
            zeta = zetas[k++];
            for(j = start; j < start + (1 << level); j++) {
                uint16_t t = montgomery_reduce((uint32_t)zeta * r[j + (1 << level)]);
                r[j + (1 << level)] = r[j] - t;
                r[j] = r[j] + t;
            }
        }
    }
}

// Exercise 5: Implement inverse NTT
void invntt(uint16_t r[256]) {
    // Hint: Use zetas in reverse order, then multiply by n^(-1)
}

// Polynomial multiplication via NTT
void poly_mul_ntt(poly *r, const poly *a, const poly *b) {
    poly a_ntt = *a, b_ntt = *b;
    
    ntt(a_ntt.coeffs);
    ntt(b_ntt.coeffs);
    
    // Pointwise multiplication in NTT domain
    for(int i = 0; i < 256; i++) {
        r->coeffs[i] = montgomery_reduce(
            (uint32_t)a_ntt.coeffs[i] * b_ntt.coeffs[i]
        );
    }
    
    invntt(r->coeffs);
}
```

#### Day 4-5: Gaussian Sampling
```c
// Centered binomial distribution (approximates Gaussian)
uint16_t sample_cbd_eta2(const uint8_t buf[8]) {
    uint32_t t = load32_le(buf) | ((uint64_t)load32_le(buf + 4) << 32);
    uint16_t a = 0, b = 0;
    
    for(int i = 0; i < 32; i++) {
        a += (t >> i) & 1;
        b += (t >> (i + 32)) & 1;
    }
    
    return a - b;  // In [-2, 2]
}

// Sample polynomial with small coefficients
void poly_getnoise(poly *r, const uint8_t *seed, uint8_t nonce) {
    uint8_t buf[128];
    SHA3_256(buf, seed, 32);  // Simplified - use SHAKE in practice
    
    for(int i = 0; i < 256; i += 4) {
        uint16_t t0 = sample_cbd_eta2(&buf[2*i]);
        uint16_t t1 = sample_cbd_eta2(&buf[2*i + 8]);
        uint16_t t2 = sample_cbd_eta2(&buf[2*i + 16]);
        uint16_t t3 = sample_cbd_eta2(&buf[2*i + 24]);
        
        r->coeffs[i]   = t0;
        r->coeffs[i+1] = t1;
        r->coeffs[i+2] = t2;
        r->coeffs[i+3] = t3;
    }
}
```

#### Day 6-7: Matrix Operations
```c
// Matrix-vector multiplication (core of Module-LWE)
void polyvec_matrix_mul(polyvec *r, const polyvec mat[4], 
                        const polyvec *v) {
    poly temp;
    
    for(int i = 0; i < 4; i++) {
        poly_mul_ntt(&r->vec[i], &mat[i].vec[0], &v->vec[0]);
        
        for(int j = 1; j < 4; j++) {
            poly_mul_ntt(&temp, &mat[i].vec[j], &v->vec[j]);
            poly_add(&r->vec[i], &r->vec[i], &temp);
        }
    }
}

// Compress polynomial coefficients (lossy)
void poly_compress(uint8_t *r, const poly *a) {
    uint16_t t[8];
    for(int i = 0; i < 32; i++) {
        for(int j = 0; j < 8; j++) {
            t[j] = ((((uint32_t)a->coeffs[8*i+j] << 4) + KYBER_Q/2) 
                   / KYBER_Q) & 0x0F;
        }
        r[4*i]   = t[0] | (t[1] << 4);
        r[4*i+1] = t[2] | (t[3] << 4);
        r[4*i+2] = t[4] | (t[5] << 4);
        r[4*i+3] = t[6] | (t[7] << 4);
    }
}
```

**Week 3 Project**: Implement a working polynomial arithmetic library with NTT multiplication.

---

### Week 4: Cryptographic Patterns
**Goal**: Combine everything into cryptographic primitives.

#### Day 1-2: Key Encapsulation Mechanism (KEM) Structure
```c
// Simplified Kyber-like KEM structure
typedef struct {
    uint8_t pk[800];  // Public key
    uint8_t sk[1632]; // Secret key (includes pk + secret + hash)
} keypair;

// IND-CCA2 secure KEM interface
int crypto_kem_keypair(uint8_t *pk, uint8_t *sk) {
    polyvec a[4], s, e;
    uint8_t seed[32];
    
    // 1. Generate random seed
    randombytes(seed, 32);
    
    // 2. Expand seed to matrix A (public parameter)
    gen_matrix(a, seed);
    
    // 3. Sample secret and error
    poly_getnoise_vec(&s, seed, 0);
    poly_getnoise_vec(&e, seed, 1);
    
    // 4. Compute public key: b = As + e
    polyvec b;
    polyvec_matrix_mul(&b, a, &s);
    polyvec_add(&b, &b, &e);
    
    // 5. Pack everything
    pack_pk(pk, &b, seed);
    pack_sk(sk, &s, pk);
    
    return 0;
}

// Exercise 6: Implement encapsulation
int crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
    // 1. Unpack public key
    // 2. Sample randomness
    // 3. Compute ciphertext
    // 4. Derive shared secret
    return 0;
}
```

#### Day 3-4: Constant-Time Decapsulation
```c
// Fujisaki-Okamoto transform for IND-CCA2 security
int crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
    uint8_t buf[32], kr[64], cmp[32];
    const uint8_t *pk = sk + 1632 - 800;  // pk is at end of sk
    
    // 1. Decrypt to get message
    indcpa_dec(buf, ct, sk);
    
    // 2. Re-encrypt and verify (implicit rejection)
    hash_g(kr, buf, pk);
    indcpa_enc(cmp, buf, pk, &kr[32]);
    
    // 3. Constant-time verification
    int fail = ct_compare(ct, cmp, 768);
    
    // 4. Hash to get shared secret
    hash_h(ss, ct);
    
    // 5. Overwrite ss with random if decryption failed
    cmov(ss, sk + 1600, 32, fail);  // sk contains pre-generated random
    
    return 0;
}
```

#### Day 5-7: Side-Channel Countermeasures
```c
// Masking for power analysis resistance
void masked_poly_add(poly *r, const poly *a, const poly *b, 
                     const poly *mask) {
    poly a_masked, b_masked;
    
    // Add masks
    poly_add(&a_masked, a, mask);
    poly_add(&b_masked, b, mask);
    
    // Compute on masked values
    poly_add(r, &a_masked, &b_masked);
    
    // Remove mask (2*mask cancels in Zq)
    poly temp_mask;
    poly_add(&temp_mask, mask, mask);
    poly_sub(r, r, &temp_mask);
}

// Shuffling for cache-timing resistance
void shuffled_ntt(uint16_t r[256]) {
    uint16_t permutation[256];
    uint16_t temp[256];
    
    // Generate random permutation
    generate_permutation(permutation, 256);
    
    // Apply permutation
    for(int i = 0; i < 256; i++) {
        temp[i] = r[permutation[i]];
    }
    
    // Compute NTT on shuffled data
    ntt(temp);
    
    // Reverse permutation
    for(int i = 0; i < 256; i++) {
        r[permutation[i]] = temp[i];
    }
}
```

**Week 4 Project**: Build a minimal but complete KEM with constant-time implementation.

---

## Month 2: Real PQC Implementation

### Week 5-6: CRYSTALS-Kyber Implementation
**Goal**: Implement a working version of Kyber512.

```c
// Full Kyber parameters
#define KYBER_K 2  // Dimension
#define KYBER_N 256  // Polynomial degree
#define KYBER_Q 3329  // Modulus
#define KYBER_ETA1 3  // Noise parameter
#define KYBER_ETA2 2

// Complete implementation structure
// Week 5: Core operations
// - Polynomial arithmetic
// - NTT with all optimizations
// - Serialization/compression

// Week 6: Full KEM
// - Key generation
// - Encapsulation with FO transform
// - Constant-time decapsulation

// By end of week 6, you should have ~1500 lines of working Kyber
```

### Week 7-8: Optimization Techniques
**Goal**: Make your implementation fast and secure.

```c
// Week 7: SIMD Optimization
// - AVX2 for parallel polynomial operations
// - 8x speedup for NTT

// Week 8: Advanced countermeasures
// - Complete constant-time verification
// - Cache-timing resistance
// - Fault injection protection
```

---

## Month 3: Advanced Topics and Portfolio

### Week 9-10: Alternative PQC Schemes
- Implement SPHINCS+ (hash-based signatures)
- Understand Classic McEliece (code-based)

### Week 11: Integration and Testing
- Integrate with TLS 1.3
- Comprehensive test suite
- Benchmarking framework

### Week 12: Portfolio and Review
- Clean up all code
- Write documentation
- Prepare for interviews

---

## Daily Routine

### Morning (1 hour)
- Review yesterday's code
- Read one section from reference implementation
- Implement one new function

### Evening (2 hours)
- Complete exercises
- Debug and test
- Commit to private Git repo

### Weekend Project
- Combine week's learning into working module
- Write blog post explaining one concept

---

## Success Metrics

### Month 1 Checkpoints
- [ ] Can implement any modular arithmetic operation
- [ ] Understand every line of NTT code
- [ ] Pass all constant-time tests

### Month 2 Checkpoints
- [ ] Working Kyber implementation
- [ ] Matches test vectors
- [ ] Within 10x of reference speed

### Month 3 Checkpoints
- [ ] Portfolio with 3 complete implementations
- [ ] Can explain any PQC concept
- [ ] Ready for Google interview

---

## Resources Embedded in Plan

All code above is self-contained. Additional references:
- Test vectors: Use NIST KAT files
- Debugging: Print intermediate values, compare with reference
- Questions: Implement first, understand why later

Remember: The goal isn't perfection, it's understanding. Every bug teaches you something about PQC implementation.