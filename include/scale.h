#ifndef SCALE_H
#define SCALE_H

struct ScaleFactor;
/* Strukture ScaleFactor
 * Menyimpan faktor skala untuk mengubah ukuran objek dalam game.
 * Digunakan untuk mengubah ukuran objek dalam game agar sesuai dengan resolusi layar.
 */
typedef struct ScaleFactor {
    float x;
    float y;
} ScaleFactor;

ScaleFactor GetScreenScaleFactor(void);
#endif