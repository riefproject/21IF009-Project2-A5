/**
 * MAIN PROGRAM - BLOCK SHOOTER GAME
 * ==================================
 *
 * File utama yang menjadi entry point untuk menjalankan game Block Shooter.
 * Program ini menginisialisasi game window dan menangani cleanup setelah game selesai.
 *
 * Alur program:
 * 1. Memanggil mainWindow() untuk memulai game
 * 2. Membersihkan layar console setelah game selesai
 * 3. Menampilkan pesan terima kasih kepada player
 * 4. Mengakhiri program dengan return code 0
 */

#include "all.h" 

 /**
  * Fungsi utama program
  * Menjalankan game dan menangani cleanup setelah selesai
  */
int main(void) {
    mainWindow();

    system("cls");
    printf("\n\nThanks for playing ""\e[0;32m""Block Shooter\n""\e[0m");

    return 0;
}