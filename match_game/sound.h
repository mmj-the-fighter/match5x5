//This file is 100% AI generated
#ifndef _SOUND_H_
#define _SOUND_H_

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

// =====================================================
// GLOBAL AUDIO ENABLE/DISABLE FLAG
// =====================================================
static int g_audioEnabled = 1;   // enabled by default

static void EnableAudio() {
    g_audioEnabled = 1;
}

static void DisableAudio() {
    g_audioEnabled = 0;
}

// =====================================================
// ENGINE
// =====================================================
static ma_engine g_engine;
static int g_engineInitialized = 0;

static void InitSoundEngine() {
    if (!g_audioEnabled) return;  // engine stays off
    if (!g_engineInitialized) {
        ma_engine_init(NULL, &g_engine);
        g_engineInitialized = 1;
    }
}

static void ShutdownSoundEngine() {
    if (g_engineInitialized) {
        ma_engine_uninit(&g_engine);
        g_engineInitialized = 0;
    }
}

// =====================================================
// BGM (Looping Background Music)
// =====================================================
static ma_sound g_bgm;
static int g_bgmLoaded = 0;

static void PlaySoundLooped(const char* filename) {
    if (!g_audioEnabled) return;

    InitSoundEngine();

    // Stop previous BGM
    if (g_bgmLoaded) {
        ma_sound_stop(&g_bgm);
        ma_sound_uninit(&g_bgm);
        g_bgmLoaded = 0;
    }

    // Load and start looping
    if (ma_sound_init_from_file(
        &g_engine,
        filename,
        MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC | MA_SOUND_FLAG_STREAM,
        NULL,
        NULL,
        &g_bgm
    ) == MA_SUCCESS)
    {
        ma_sound_set_looping(&g_bgm, MA_TRUE);
        ma_sound_start(&g_bgm);
        g_bgmLoaded = 1;
    }
}

// =====================================================
// Pause / Resume
// =====================================================
static void PauseBGM() {
    if (!g_audioEnabled) return;
    if (g_bgmLoaded)
        ma_sound_stop(&g_bgm); // does not reset position
}

static void ResumeBGM() {
    if (!g_audioEnabled) return;
    if (g_bgmLoaded)
        ma_sound_start(&g_bgm);
}

// =====================================================
// One-Shot Sound Effect
// =====================================================
static void PlaySoundOnce(const char* filename) {
    if (!g_audioEnabled) return;

    InitSoundEngine();
    ma_engine_play_sound(&g_engine, filename, NULL);
}

// =====================================================
// Stop Everything
// =====================================================
static void StopAllSounds() {
    if (!g_engineInitialized) return;

    // Stop BGM
    if (g_bgmLoaded) {
        ma_sound_stop(&g_bgm);
        ma_sound_uninit(&g_bgm);
        g_bgmLoaded = 0;
    }

    // Stop all SFX
    ma_engine_stop(&g_engine);
}

#endif


////This file is 100% AI generated
//#ifndef _SOUND_H_
//#define _SOUND_H_
//
//#define MINIAUDIO_IMPLEMENTATION
//#include "miniaudio.h"
//
//// A single engine handles all mixing and playback.
//static ma_engine g_engine;
//static int g_engineInitialized = 0;
//
//static void InitSoundEngine() {
//    if (!g_engineInitialized) {
//        ma_engine_init(NULL, &g_engine);
//        g_engineInitialized = 1;
//    }
//}
//
//static void ShutdownSoundEngine() {
//    if (g_engineInitialized) {
//        ma_engine_uninit(&g_engine);
//        g_engineInitialized = 0;
//    }
//}
//
//// ---- BGM (looping) ----
//
//static ma_sound g_bgm;
//static int g_bgmLoaded = 0;
//
//static void PlaySoundLooped(const char* filename) {
//    InitSoundEngine();
//
//    // Stop previous BGM
//    if (g_bgmLoaded) {
//        ma_sound_stop(&g_bgm);
//        ma_sound_uninit(&g_bgm);
//        g_bgmLoaded = 0;
//    }
//
//    // Load and start looping
//    if (ma_sound_init_from_file(&g_engine, filename,
//        MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC | MA_SOUND_FLAG_STREAM,
//        NULL, NULL, &g_bgm) == MA_SUCCESS) {
//        ma_sound_set_looping(&g_bgm, MA_TRUE);
//        ma_sound_start(&g_bgm);
//        g_bgmLoaded = 1;
//    }
//}
//
//// ---- One-shot sound effects ----
//
//static void PlaySoundOnce(const char* filename) {
//    InitSoundEngine();
//    ma_engine_play_sound(&g_engine, filename, NULL);  // plays independently on its own voice
//}
//
//// ---- Stop everything ----
//
//static void StopAllSounds() {
//    if (!g_engineInitialized) return;
//
//    // Stop BGM
//    if (g_bgmLoaded) {
//        ma_sound_stop(&g_bgm);
//        ma_sound_uninit(&g_bgm);
//        g_bgmLoaded = 0;
//    }
//
//    // Stop all sound effects & voices
//    ma_engine_stop(&g_engine);
//}
//
//
//// ---- Pause & Resume BGM ----
//
//static void PauseBGM() {
//    if (g_bgmLoaded) {
//        ma_sound_stop(&g_bgm);      // stop but do not uninit
//    }
//}
//
//static void ResumeBGM() {
//    if (g_bgmLoaded) {
//        ma_sound_start(&g_bgm);     // resume from paused position
//    }
//}
//
//#endif
