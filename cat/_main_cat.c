#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tT_flag(unsigned char *buffer, unsigned char *buffer_Tab);
void eE_flag (unsigned char *buffer);
void v_flag (unsigned char *buffer);
void b_flag(unsigned char *buffer, int *lineNumber);
void n_flag(unsigned char *buffer, int *lineNumber);
int  s_flag(unsigned char *buffer, int *lastLineBlank);

int main(int argc, char **argv) {

  int bflag = 0, eflag = 0, nflag = 0, sflag = 0, tflag = 0, vflag = 0,
      Tflag = 0, Eflag = 0;
  int opt;

  const char *short_options = "vbnsteTE";

  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, no_argument, NULL, 't'},
      {NULL, no_argument, NULL, 'v'},
      {NULL, no_argument, NULL, 'e'},
      {NULL, no_argument, NULL, 'T'},
      {NULL, no_argument, NULL, 'E'},
      {NULL, 0, NULL, 0}};

  int rez;
  int option_index;

  while ((rez = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {

    switch (rez) {
    case 'v': {
      vflag++;
      break;
    };
    case 'b': {
      bflag++;
      break;
    };
    case 'n': {
      nflag++;
      break;
    };

    case 's': {
      sflag++;
      break;
    };
    case 't': {
      tflag++;
      break;
    };
    case 'e': {
      eflag++;
      break;
    };
    case 'T': {
      Tflag++;
      break;
    };
    case 'E': {
      Eflag++;
      break;
    };
    case '?':
    default: {
      printf("usage: cat [-bens] [file ...]\n");
      exit(1);
    };
    };
  };

  const int bufferSize = 4096;
  unsigned char buffer[bufferSize];
  unsigned char buffer_Tab[bufferSize];
  int currentFile = optind;
  FILE *fp;

  while (currentFile <= argc) {
    if (currentFile != argc) {
      fp = fopen(argv[currentFile], "rb");
      if (fp == NULL) {
        fprintf(stderr, "%s: %s: No such file or directory", argv[0],
                argv[currentFile]);
        exit(1);
      }
    }

    int lastLineBlank = 0;
    int lineNumber = 1;

    while (fgets(buffer, bufferSize, (fp == NULL ? stdin : fp))) {

      int length = strlen(buffer);
      buffer[length - 1] = '\0';

      if (tflag || Tflag) {
        tT_flag(buffer, buffer_Tab);
      }

      if (sflag) {
        if (s_flag(buffer, &lastLineBlank)) {
          continue;
        }
      }

      if (bflag) {
        b_flag(buffer, &lineNumber);
      }

      else if (nflag) {
        n_flag(buffer, &lineNumber);
      }

      if (eflag || Eflag) {
        eE_flag(buffer);
      }

      if (vflag || eflag || tflag) {
        v_flag(buffer);
      } else {
        fprintf(stdout, "%s\n", buffer);
      }
    }

    fclose(fp);
    currentFile++;
  }

  return 0;
}

void tT_flag(unsigned char *buffer, unsigned char *buffer_Tab){
        int length = strlen(buffer);
        int i = 0, shift = 0;
        for (i = 0; i < length; i++) {
          if ('\t' != buffer[i]) {
            buffer_Tab[i + shift] = buffer[i];
          } else {
            buffer_Tab[i + shift] = '^';
            shift++;
            buffer_Tab[i + shift] = 'I';
          }
        }
        buffer_Tab[i + shift] = '\0';
        for (i = 0; i < strlen(buffer_Tab); i++) {
          buffer[i] = buffer_Tab[i];
        }
        buffer[i] = '\0';
}

void eE_flag (unsigned char *buffer) {
        int length = strlen(buffer);
        buffer[length] = '$';
        buffer[length + 1] = '\0';
}

void v_flag (unsigned char *buffer) {
        unsigned char c;
        for (int i = 0; i < strlen(buffer); i++) {
          c = buffer[i];

          if (c >= 32) {
            if (c < 127)
              putchar(c);
            else if (c == 127) {
              printf("^?");
            } else {
              printf("M-");
              if (c >= 160) {
                if (c < 255)
                  putchar(c - 128);
                else {
                  printf("^?");
                }
              } else {
                printf("^");
                putchar(c - 64);
              }
            }
          } else if (c == '\t')
            putchar(c);
          else {
            printf("^");
            putchar(c + 64);
          }
        }
        printf("\n");
}

void b_flag(unsigned char *buffer, int *lineNumber) {
        int buff = *lineNumber;
        int length = strlen(buffer);
        if (length >= 1) {
          char *tmp = strdup(buffer);
          buffer[0] = '\0';
          sprintf(buffer, "%*d\t", 6, buff++);
          strcat(buffer, tmp);
          *lineNumber = buff;
        }
}

void n_flag(unsigned char *buffer, int *lineNumber) {
        int buff = *lineNumber;
        char *tmp = strdup(buffer);
        buffer[0] = '\0';
        sprintf(buffer, "%*d\t", 6, buff++);
        strcat(buffer, tmp);
        *lineNumber = buff;
}

int  s_flag(unsigned char *buffer, int *lastLineBlank) {
        int flag_continue = 0;
        int length = strlen(buffer);
        int currentLineBlank = (length <= 1) ? 1 : 0;
        if (*lastLineBlank && currentLineBlank) {
          flag_continue = 1;
        }
        *lastLineBlank = currentLineBlank;
        return flag_continue;
}