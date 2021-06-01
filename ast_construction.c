#include <stdlib.h>
#include <stdio.h> 

#include "type_ast.h"

Ast creer_operation(TypeOperateur opr , Ast opde_gauche , Ast opde_droit) 
{
      Ast expr ;
      expr = (Ast) malloc (sizeof(NoeudAst));
      expr->nature = OPERATION;
      expr->operateur = opr;
      if (opde_gauche == NULL || opde_droit == NULL) {
         printf("ERREUR_EXPRESSION\n") ;
	 exit(1) ;
      } else {
         expr->gauche = opde_gauche;
         expr->droite = opde_droit;
      } ;
      return expr ;
}

Ast creer_valeur(int val) {
  Ast expr;
  expr = (Ast)malloc(sizeof(NoeudAst));
  expr->nature = VALEUR;
  expr->valeur = val;
  return expr;
}

Ast creer_idf(char *idf) {
  Ast expr;
  expr = (Ast)malloc(sizeof(NoeudAst));
  expr->nature = N_IDF;
  expr->idf = malloc(10*sizeof(char));
  strcpy(expr->idf,idf);
  return expr;
}

Ast creer_lire(Ast Ag) {
  Ast expr;
  expr = (Ast)malloc(sizeof(NoeudAst));
  expr->nature = N_LIRE;
  expr->gauche = Ag;
  expr->droite = NULL;
  return expr;
}
Ast creer_ecrire(Ast Ag) {
  Ast expr;
  expr = (Ast)malloc(sizeof(NoeudAst));
  expr->nature = N_ECRIRE;
  expr->gauche = Ag;
  expr->droite = NULL;
  return expr;
}
Ast creer_aff(Ast Ag, Ast Ad) {
  Ast expr;
  expr = (Ast)malloc(sizeof(NoeudAst));
  expr->nature = N_AFF;
  expr->gauche = Ag;
  expr->droite = Ad;
  return expr;
}

Ast creer_seqinst(Ast Ag,Ast Ad){
  Ast expr;
  expr = (Ast)malloc(sizeof(NoeudAst));
  expr->nature = N_SEPINST;
  expr->gauche = Ag;
  expr->droite = Ad;
  return expr;
}
