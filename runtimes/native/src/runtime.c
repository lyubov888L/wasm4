#include "runtime.h"

#include <stdio.h>
#include <string.h>

#include "wasm.h"
#include "window.h"

#define WIDTH 160
#define HEIGHT 160

typedef struct {
    uint8_t _padding[4];
    uint32_t palette[4];
    uint16_t drawColors;
    uint8_t gamepads[4];
    int16_t mouseX;
    int16_t mouseY;
    uint8_t mouseButtons;
    uint8_t _reserved[129];
    uint8_t framebuffer[WIDTH*HEIGHT>>2];
} Memory;

static Memory* memory;

void w4_runtimeInit (uint8_t* memoryBytes) {
    memory = (Memory*)memoryBytes;

    // Set memory to initial state
    memset(memory, 0, 0xffff);
    memory->palette[0] = 0xe0f8cf;
    memory->palette[1] = 0x86c06c;
    memory->palette[2] = 0x306850;
    memory->palette[3] = 0x071821;
    memory->drawColors = 0x1203;
    memory->mouseX = 0x7fff;
    memory->mouseY = 0x7fff;
}

void w4_runtimeBlit (const uint8_t* sprite, int x, int y, int width, int height, int flags) {
    printf("blit: %p, %d, %d, %d, %d, %d\n", sprite, x, y, width, height, flags);
}

void w4_runtimeBlitSub (const uint8_t* sprite, int x, int y, int width, int height, int srcX, int srcY, int stride, int flags) {
    printf("blitSub: %p, %d, %d, %d, %d, %d, %d, %d, %d\n", sprite, x, y, width, height, srcX, srcY, stride, flags);
}

void w4_runtimeLine (int x1, int y1, int x2, int y2) {
    printf("line: %d, %d, %d, %d\n", x1, y1, x2, y2);
}

void w4_runtimeHLine (int x, int y, int len) {
    printf("hline: %d, %d, %d\n", x, y, len);
}

void w4_runtimeVLine (int x, int y, int len) {
    printf("vline: %d, %d, %d\n", x, y, len);
}

void w4_runtimeOval (int x, int y, int width, int height) {
    printf("oval: %d, %d, %d, %d\n", x, y, width, height);
}

void w4_runtimeRect (int x, int y, int width, int height) {
    printf("rect: %d, %d, %d, %d\n", x, y, width, height);
}

void w4_runtimeText (const char* str, int x, int y) {
    printf("text: %s, %d, %d\n", str, x, y);
}

void w4_runtimeTextUtf8 (const uint8_t* str, int byteLength, int x, int y) {
    printf("textUtf8: %p, %d, %d, %d\n", str, byteLength, x, y);
}

void w4_runtimeTextUtf16 (const uint8_t* str, int byteLength, int x, int y) {
    printf("textUtf16: %p, %d, %d, %d\n", str, byteLength, x, y);
}

void w4_runtimeTone (int frequency, int duration, int volume, int flags) {
    printf("tone: %d, %d, %d, %d\n", frequency, duration, volume, flags);
}

int w4_runtimeDiskr (uint8_t* dest, int size) {
    printf("diskr: %p, %d\n", dest, size);
    return 0;
}

int w4_runtimeDiskw (const uint8_t* src, int size) {
    printf("diskw: %p, %d\n", src, size);
    return 0;
}

void w4_runtimeTrace (const char* str) {
    puts(str);
}

void w4_runtimeTraceUtf8 (const uint8_t* str, int byteLength) {
    printf("traceUtf8: %p, %d\n", str, byteLength);
}

void w4_runtimeTraceUtf16 (const uint8_t* str, int byteLength) {
    printf("traceUtf16: %p, %d\n", str, byteLength);
}

void w4_runtimeTracef (const char* str, const void* stack) {
    // TODO(2021-09-27): Call printf with stack
    puts(str);
}

void w4_runtimeUpdate () {
    // Clear the framebuffer
    memset(memory->framebuffer, 0, sizeof(memory->framebuffer));

    w4_wasmCallUpdate();

    w4_windowComposite(memory->palette, memory->framebuffer);
}
