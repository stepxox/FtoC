#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double convertFtoC(double fahrenheit) {
  double celsius = (fahrenheit - 32) / 1.8;
  return celsius;
}

void fahrenheitToCelsius() {
  char input[1000000]; // celkovy vstup
  fgets(input, sizeof(input), stdin);
  // Nahradzujeme \n za NULL, aby nim nase posledne slovo nekoncilo
  size_t pos = strcspn(input, "\n");
  size_t total = strlen(input);
  input[pos] = '\0';
  int hasEndl = 0;
  if (pos != total) {
    hasEndl = 1;
  }

  // Prechadzame cez jednotlive slova (oddelene medzerami)
  char *pch = strtok(input, " ");
  while (pch != NULL) {
    size_t len = strlen(pch);
    // Edge case pripady, kde je koncovy charakter . alebo ,
    // v tychto pripadoch tento charakter chceme zmazat, lebo znamena koniec
    // vety pripadne jednoduchu ciarku za slovom. Musime si ich ale ulozit aby
    // sme ich potom vypisali za slovom.
    char extra_char = ' ';
    int hasExtra = 0;
    if (pch[len - 1] == '.' || pch[len - 1] == ',') {
      extra_char = pch[len - 1];
      pch[len - 1] = '\0';
      len -= 1;
      hasExtra = 1;
    }

    // Ak sa slovo nekonci F, nieje to fahrenheit hodnota
    // mozeme ju jednoducho naspat vypisat
    if (pch[len - 1] != 'F') {
      for (size_t i = 0; i < len; i++) {
        printf("%c", pch[i]);
      }
    } else {
      // Overime ci je kazdy charakter, okrem posledneho (F) cislo alebo bodka
      // ak zistime ze nie, nieje to platna fahrenheit hodnota, vypiseme
      int isNumber = 1;
      int decimalDots = 0;
      for (size_t i = 0; i < len - 1; i++) {
        if (pch[i] == '.') {
          // Overenie ze cislo ma iba jednu desatinnu bodku
          if (decimalDots == 0) {
            decimalDots = 1;
          } else {
            isNumber = 0;
            break;
          }
        }
        // Overenie ze charakter (ak to uz nebola boda) je ciselny
        // ak nieje, tak to nieje cislo
        else if (!isdigit(pch[i])) {
          isNumber = 0;
          break;
        }
      }

      // Aj ked by malo cislo iba jednu desatinnu bodku,
      // ak je tato bodka na konci, tak cislo nieje platne (12. nieje cislo),
      // tak isto ak je boda na zaciatku (.12 nieje cislo)
      // Edge case -- niekto pouziva .12 ako 0.12?
      if ((pch[len - 1] == '.') || pch[0] == '.') {
        isNumber = 0;
      }

      if (isNumber == 0) {
        for (size_t i = 0; i < len; i++) {
          printf("%c", pch[i]);
        }
      } else {
        // Nastavime posledny znak slova (F) na \0, aby ho atof necital,
        // a vedel tak premenit cislo na float.
        pch[len - 1] = '\0';
        double fahrenheitHodnota = atof(pch);
        double celsiusHodnota = convertFtoC(fahrenheitHodnota);
        printf("%0.2fC", celsiusHodnota);
      }
    }

    if (hasExtra == 1) {
      printf("%c", extra_char);
    }
    pch = strtok(NULL, " ");
    if (pch != NULL) {
      printf(" ");
    }
  }
  if (hasEndl == 1) {
    printf("\n");
  }
}

int main() {
  fahrenheitToCelsius();
  return 0;
}
