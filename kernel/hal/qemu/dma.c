/*
 * QEMU DMA Stub
 *
 * QEMU doesn't have DMA acceleration, so we use CPU memcpy as fallback.
 * This allows the same code to work on both platforms.
 */

#include "../hal.h"
#include "../../string.h"

int hal_dma_init(void) {
    // No DMA on QEMU, but return success so caller doesn't error out
    return 0;
}

int hal_dma_available(void) {
    // DMA is not actually available on QEMU
    return 0;
}

int hal_dma_copy(void *dst, const void *src, uint32_t len) {
    // Fallback to CPU copy
    memcpy(dst, src, len);
    return 0;
}

int hal_dma_copy_2d(void *dst, uint32_t dst_pitch,
                    const void *src, uint32_t src_pitch,
                    uint32_t width, uint32_t height) {
    // Fallback to row-by-row CPU copy
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;

    for (uint32_t y = 0; y < height; y++) {
        memcpy(d, s, width);
        d += dst_pitch;
        s += src_pitch;
    }
    return 0;
}

int hal_dma_fb_copy(uint32_t *dst, const uint32_t *src, uint32_t width, uint32_t height) {
    size_t pixels, bytes;

    if (!dst || !src)
        return -1;

    if (height != 0 && width > SIZE_MAX / height)
        return -1;
    
    pixels = (size_t)height * width;

    if (pixels > SIZE_MAX / sizeof(uint32_t))
        return -1;

    bytes = pixels * sizeof(uint32_t);

    memcpy(dst, src, bytes);
    return 0;
}

int hal_dma_fill(void *dst, uint32_t value, uint32_t len) {
    // Fallback to CPU fill (len is in bytes, value is 32-bit)
    uint32_t *d = (uint32_t *)dst;
    uint32_t count = len / 4;
    for (uint32_t i = 0; i < count; i++) {
        d[i] = value;
    }
    return 0;
}
