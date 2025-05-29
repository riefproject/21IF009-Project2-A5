#ifndef SCALE_H
#define SCALE_H

// =============================================================================
// SCALING AND RESOLUTION SYSTEM
// Sistem scaling untuk adaptasi berbagai resolusi layar
// =============================================================================

// Struktur untuk menyimpan faktor skala X dan Y
// Digunakan untuk mengadaptasi UI dan game objects ke berbagai resolusi layar
typedef struct ScaleFactor {
    float x;    // Faktor skala horizontal (width scaling)
    float y;    // Faktor skala vertikal (height scaling)
} ScaleFactor;

// =============================================================================
// SCALE CALCULATION UTILITIES
// Fungsi-fungsi untuk menghitung faktor skala
// =============================================================================

// Menghitung faktor skala berdasarkan resolusi layar saat ini
// Membandingkan resolusi current dengan base resolution untuk mendapatkan scale factor
ScaleFactor GetScreenScaleFactor(void);

#endif