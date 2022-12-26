#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

  int eflag = 0, iflag = 0, vflag = 0, cflag = 0, lflag = 0, nflag = 0;
  int opt;

  const char *short_options = "e:ivcln";

  const struct option long_options[] = {
      {NULL, required_argument, NULL, 'e'},
      {NULL, no_argument, NULL, 'i'},
      {NULL, no_argument, NULL, 'v'},
      {NULL, no_argument, NULL, 'c'},
      {NULL, no_argument, NULL, 'l'},
      {NULL, no_argument, NULL, 'n'},
      {NULL, 0, NULL, 0}};

  int rez;
  int option_index;

  while ((rez = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {

    switch (rez) {
    case 'v': {
      eflag++;
      break;
    };
    case 'b': {
      iflag++;
      break;
    };
    case 'n': {
      vflag++;
      break;
    };

    case 's': {
      cflag++;
      break;
    };
    case 't': {
      lflag++;
      break;
    };
    case 'e': {
      nflag++;
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

     /*
     реализация флагов
     */

    }

    fclose(fp);
    currentFile++;
  }

  return 0;
}

