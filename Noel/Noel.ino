#include <Peggy2.h>
#include "chars.h"
#include "draw.h"

Peggy2 screen;
char message1 [] = "Joyeux Noel! * * * * * Fumer tue, peter pue!  * * * * * * * * * * * ";
char message2 [] = "Merry Christmas! * * * Life is hard. After all, it kills you! * * * ";

// converti le message pour la fonte
int ParseMessage(char msg[]) {
  // encode le message
  int i = 0;
  char c;

  while (msg[i] != '\0') {
    c = msg[i];
    // caracteres autorisés
    if (c > 31 && c < 91) {
      msg[i] = msg[i] - 32;

      // minuscules
    } else if (c > 95) {
      msg[i] = msg[i] - 32 - 32;

      // defaut = space
    } else {
      msg[i] = 0;
    }
    i++;
  }
  return i;
}

// Converti la fonte
int ParseFonte(uint8_t fnt[], int ty, int tx , int nbchar) {
  uint8_t v, w;
  int i, j;

  // inverse les colonnes
  for (i = 0; i < nbchar * ty; i++) {
    v = fnt[i];
    w = 0;
    for (j = 0; j < tx; j++) {
      w <<= 1;
      w |= (v & 1);
      v >>= 1;
    }

    fnt[i] = w;
  }

  return 0;
}

// Affiche un caractere
void PutChar(int y, uint8_t* fontptr, char c, int font_ty) {
  int j;
  int src = 0;
  uint8_t *dst;

  dst = (uint8_t *)screen.buffer;
  dst += 4 * y;
  dst += 3;

  fontptr += (int)c * font_ty;
  for (j = 0; j < font_ty; j++) {
    *dst = *fontptr;
    dst += 4;
    fontptr ++;
  }
}

// décalle l'affichage vers la gauche, pleine largeur
void ShiftLeft(int y1, int y2)
{
  uint32_t *ptr1;
  ptr1 = (uint32_t *)screen.buffer;

  int i = 0;
  for (i = 0; i < y1; i++)
  {
    ptr1++;
  }

  // lignes à décaller
  for (; i < y2; i++) {
    (*ptr1) = (*ptr1) >> 1;
    ptr1++;
  }
}

// décalle tout l'affichage vers le bas
void ShiftUp()
{
  uint32_t *ptr_dst, *ptr_src;
  ptr_dst = (uint32_t *)screen.buffer;
  ptr_src = (uint32_t *)screen.buffer;

  ptr_src += 1; // source is the second line

  // 24 copies de lignes
  for (int i = 0; i < 24; i++)
  {
    (*ptr_dst) = (*ptr_src);
    ptr_dst ++;
    ptr_src ++;
  }

  // la derniere ligne est vidée
  (*ptr_dst) = 0;
}

// décalle tout l'affichage vers le bas
void ShiftDown()
{
  uint32_t *ptr_dst, *ptr_src;
  ptr_dst = (uint32_t *)screen.buffer;
  ptr_src = (uint32_t *)screen.buffer;

  ptr_dst += 24; // destination is the last line
  ptr_src += 23; // source is the before last line

  // 24 copies de lignes
  for (int i = 0; i < 24; i++)
  {
    (*ptr_dst) = (*ptr_src);
    ptr_dst --;
    ptr_src --;
  }

  // la derniere ligne est vidée
  (*ptr_dst) = 0;
}

int messageLen1;
int messageLen2;
void SeqMessage(int count) {

  int posCar = 0;
  int posMessage1 = 0;
  int posMessage2 = 0;

  while (count > 0) {

    // Si on est en position pour un nouveau caractère
    if (posCar == 0) {

      if (posMessage1 == messageLen1) {
        posMessage1 = 0;
      }

      if (posMessage2 == messageLen2) {
        posMessage2 = 0;
      }

      // Affiche un caractère du message
      PutChar(0, fonte, message1[posMessage1], FONTE_TY);

      // avance dans le message
      posMessage1 ++;

      // Affiche un caractère du message
      PutChar(11, fonte, message2[posMessage2], FONTE_TY);

      // avance dans le message
      posMessage2 ++;

      posCar = FONTE_TX;
    }
    posCar--;

    // fait défiler l'écran
    ShiftLeft(0, 24);

    // Update l'affichage
    screen.RefreshAll(50);

    count--;
  }
}

void SeqTest (int count){
uint32_t * ptr_dst;
  while (count > 0) {

  
  ptr_dst = (uint32_t *)screen.buffer;
  for(int y=0; y<25; y++){
    *ptr_dst = 0xffffffff;
    ptr_dst++;
  }

  // Update l'affichage
  screen.RefreshAll(50);

count--;
  }
}

void setup() {
  ParseFonte(fonte, FONTE_TY, FONTE_TX, FONTE_NB_CHAR);

  messageLen1 = ParseMessage(message1);
  messageLen2 = ParseMessage(message2);

  screen.HardwareInit();
  screen.Clear();
}

void loop() {
  //SeqTest(100);
  SeqMessage(FONTE_TX * messageLen2);
  
  screen.Clear();

}
