#include <wasm3.h>
#include <m3_env.h>

#include "../wasm.h"
#include "../runtime.h"

static M3Environment* env;
static M3Runtime* runtime;
static M3Module* module;

static M3Function* start;
static M3Function* update;

static m3ApiRawFunction (blit) {
    m3ApiGetArgMem(const uint8_t*, sprite);
    m3ApiGetArg(int, x);
    m3ApiGetArg(int, y);
    m3ApiGetArg(int, width);
    m3ApiGetArg(int, height);
    m3ApiGetArg(int, flags);
    w4_runtimeBlit(sprite, x, y, width, height, flags);
    m3ApiSuccess();
}

static m3ApiRawFunction (blitSub) {
    m3ApiGetArgMem(const uint8_t*, sprite);
    m3ApiGetArg(int, x);
    m3ApiGetArg(int, y);
    m3ApiGetArg(int, width);
    m3ApiGetArg(int, height);
    m3ApiGetArg(int, srcX);
    m3ApiGetArg(int, srcY);
    m3ApiGetArg(int, stride);
    m3ApiGetArg(int, flags);
    w4_runtimeBlitSub(sprite, x, y, width, height, srcX, srcY, stride, flags);
    m3ApiSuccess();
}

static m3ApiRawFunction (line) {
    m3ApiGetArg(int, x1);
    m3ApiGetArg(int, y1);
    m3ApiGetArg(int, x2);
    m3ApiGetArg(int, y2);
    w4_runtimeLine(x1, y1, x2, y2);
    m3ApiSuccess();
}

static m3ApiRawFunction (hline) {
    m3ApiGetArg(int, x);
    m3ApiGetArg(int, y);
    m3ApiGetArg(int, len);
    w4_runtimeHLine(x, y, len);
    m3ApiSuccess();
}

static m3ApiRawFunction (vline) {
    m3ApiGetArg(int, x);
    m3ApiGetArg(int, y);
    m3ApiGetArg(int, len);
    w4_runtimeVLine(x, y, len);
    m3ApiSuccess();
}

static m3ApiRawFunction (oval) {
    m3ApiGetArg(int, x);
    m3ApiGetArg(int, y);
    m3ApiGetArg(int, width);
    m3ApiGetArg(int, height);
    w4_runtimeOval(x, y, width, height);
    m3ApiSuccess();
}

static m3ApiRawFunction (rect) {
    m3ApiGetArg(int, x);
    m3ApiGetArg(int, y);
    m3ApiGetArg(int, width);
    m3ApiGetArg(int, height);
    w4_runtimeRect(x, y, width, height);
    m3ApiSuccess();
}

static m3ApiRawFunction (text) {
    m3ApiGetArgMem(const char*, str);
    m3ApiGetArg(int, x);
    m3ApiGetArg(int, y);
    w4_runtimeText(str, x, y);
    m3ApiSuccess();
}

static m3ApiRawFunction (textUtf8) {
    m3ApiGetArgMem(const uint8_t*, str);
    m3ApiGetArg(int, byteLength);
    m3ApiGetArg(int, x);
    m3ApiGetArg(int, y);
    w4_runtimeTextUtf8(str, byteLength, x, y);
    m3ApiSuccess();
}

static m3ApiRawFunction (textUtf16) {
    m3ApiGetArgMem(const uint8_t*, str);
    m3ApiGetArg(int, byteLength);
    m3ApiGetArg(int, x);
    m3ApiGetArg(int, y);
    w4_runtimeTextUtf16(str, byteLength, x, y);
    m3ApiSuccess();
}

static m3ApiRawFunction (tone) {
    m3ApiGetArg(int, frequency);
    m3ApiGetArg(int, duration);
    m3ApiGetArg(int, volume);
    m3ApiGetArg(int, flags);
    w4_runtimeTone(frequency, duration, volume, flags);
    m3ApiSuccess();
}

static m3ApiRawFunction (diskr) {
    m3ApiGetArgMem(uint8_t*, dest);
    m3ApiGetArg(int, size);
    m3ApiReturnType(int);
    m3ApiReturn(w4_runtimeDiskr(dest, size));
}

static m3ApiRawFunction (diskw) {
    m3ApiGetArgMem(const uint8_t*, src);
    m3ApiGetArg(int, size);
    m3ApiReturnType(int);
    m3ApiReturn(w4_runtimeDiskw(src, size));
}

static m3ApiRawFunction (trace) {
    m3ApiGetArgMem(const char*, str);
    w4_runtimeTrace(str);
    m3ApiSuccess();
}

static m3ApiRawFunction (traceUtf8) {
    m3ApiGetArgMem(const uint8_t*, str);
    m3ApiGetArg(int, byteLength);
    w4_runtimeTraceUtf8(str, byteLength);
    m3ApiSuccess();
}

static m3ApiRawFunction (traceUtf16) {
    m3ApiGetArgMem(const uint8_t*, str);
    m3ApiGetArg(int, byteLength);
    w4_runtimeTraceUtf16(str, byteLength);
    m3ApiSuccess();
}

static m3ApiRawFunction (tracef) {
    m3ApiGetArgMem(const char*, str);
    m3ApiGetArgMem(const void*, stack);
    w4_runtimeTracef(str, stack);
    m3ApiSuccess();
}

uint8_t* w4_wasmInit () {
    env = m3_NewEnvironment();
    runtime = m3_NewRuntime(env, 1024, NULL);

    runtime->memory.maxPages = 1;
    ResizeMemory(runtime, 1);

    return m3_GetMemory(runtime, NULL, 0);
}

void w4_wasmLoadModule (const uint8_t* wasmBuffer, int byteLength) {
    m3_ParseModule(env, &module, wasmBuffer, byteLength);
    m3_LoadModule(runtime, module);

    m3_LinkRawFunction(module, "env", "blit", "v(iiiiii)", blit);
    m3_LinkRawFunction(module, "env", "blitSub", "v(iiiiiiiii)", blitSub);
    m3_LinkRawFunction(module, "env", "line", "v(ii)", line);
    m3_LinkRawFunction(module, "env", "hline", "v(iii)", hline);
    m3_LinkRawFunction(module, "env", "vline", "v(iii)", vline);
    m3_LinkRawFunction(module, "env", "oval", "v(iiii)", oval);
    m3_LinkRawFunction(module, "env", "rect", "v(iiii)", rect);
    m3_LinkRawFunction(module, "env", "text", "v(iii)", text);
    m3_LinkRawFunction(module, "env", "textUtf8", "v(iiii)", textUtf8);
    m3_LinkRawFunction(module, "env", "textUtf16", "v(iiii)", textUtf16);

    m3_LinkRawFunction(module, "env", "tone", "v(iiii)", tone);

    m3_LinkRawFunction(module, "env", "diskr", "i(ii)", diskr);
    m3_LinkRawFunction(module, "env", "diskw", "i(ii)", diskw);

    m3_LinkRawFunction(module, "env", "trace", "v(i)", trace);
    m3_LinkRawFunction(module, "env", "traceUtf8", "v(ii)", traceUtf8);
    m3_LinkRawFunction(module, "env", "traceUtf16", "v(ii)", traceUtf16);
    m3_LinkRawFunction(module, "env", "tracef", "v(ii)", tracef);

    m3_FindFunction(&start, runtime, "start");
    m3_FindFunction(&update, runtime, "update");

    // First call wasm built-in start
    m3_RunStart(module);

    // Call WASI start functions
    M3Function* func;
    m3_FindFunction(&func, runtime, "_start");
    if (func) {
        m3_CallV(func);
    }
    m3_FindFunction(&func, runtime, "_initialize");
    if (func) {
        m3_CallV(func);
    }
}

void w4_wasmCallStart () {
    if (start) {
        m3_CallV(start);
    }
}

void w4_wasmCallUpdate () {
    if (update) {
        m3_CallV(update);
    }
}
