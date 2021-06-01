#include "analyse_syntaxique.h"
#include "type_ast.h"
#include <stdio.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Erreur: nombre d'arguments incorrect");
    return 1;
  }
  Ast A;
  analyser(argv[1], &A);


  return 0;
}
