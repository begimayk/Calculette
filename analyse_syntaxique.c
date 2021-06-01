#include "analyse_syntaxique.h"
#include "analyse_lexicale.h"
#include "ast_construction.h"
#include "lecture_caracteres.h"
#include "type_ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TS.h"


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


//-------------------------------------------------------------------------------------

extern void initTS(){
    NbSymb = 0;
}

extern void afficheTS(){
    for(int i=0; i< NbSymb; i++){
        printf("%s\n", TS[i].nom);
		printf("%d\n", TS[i].valeur);
    }
}
extern int estPresentTS(char *idf, int *v){
    for(int i=0; i< NbSymb; i++){
        if (strcmp(idf, TS[i].nom)==0){
            *v = TS[i].valeur;
			return 1;
		}
    }
    return 0;
}

extern void insererTS(char *idf, int v){
	int x = 0;
    for(int i=0; i< NbSymb; i++){
        if (strcmp(idf, TS[i].nom)==0){
			TS[i].valeur = v;
			x = 1;
		}
    }
	if (x != 1){
		NbSymb++;
		strcpy(TS[NbSymb-1].nom, idf);
    	TS[NbSymb-1].valeur = v;
	}
}

//-----------------------------------------------------------------------------------------------


void afficher_operateur(TypeOperateur op) {
  switch (op) {
  case N_PLUS:
    printf("+");
    break;
  case N_MOINS:
    printf("-");
    break;
  case N_MUL:
    printf("*");
    break;
  case N_DIV:
    printf("/");
    break;
  }
}

void afficherA(Ast expr) {
  switch (expr->nature) {
  case N_SEPINST:
    afficherA(expr->gauche);
    printf(";\n");
    afficherA(expr->droite);
    break;
  case N_AFF:
    afficherA(expr->gauche);
    printf("=");
    afficherA(expr->droite);
    break;
  case N_LIRE:
    printf("lire :");
    afficherA(expr->gauche);
    break;
  case N_ECRIRE:
    printf("afficher :");
    afficherA(expr->gauche);
    break;
  case N_IDF:
    printf("%s", expr->idf);
    break;
  case OPERATION:
    printf("(");
    afficherA(expr->gauche);
    afficher_operateur(expr->operateur);
    afficherA(expr->droite);
    printf(")");
    break;
  case VALEUR:
    printf("%d", expr->valeur);
    break;
  default:
    break;
  }
}

Ast creer_operation(TypeOperateur opr, Ast opde_gauche, Ast opde_droit) {
  Ast expr;
  expr = (Ast)malloc(sizeof(NoeudAst));
  expr->nature = OPERATION;
  expr->operateur = opr;
  if (opde_gauche == NULL || opde_droit == NULL) {
    printf("ERREUR_EXPRESSION\n");
    exit(1);
  } else {
    expr->gauche = opde_gauche;
    expr->droite = opde_droit;
  };
  return expr;
}

Ast creer_valeur(int val) {
  Ast expr;
  expr = (Ast)malloc(sizeof(NoeudAst));
  expr->nature = VALEUR;
  expr->valeur = val;
  return expr;
}

int evaluation(Ast expr) {
  int Vg, Vd;
  int trouve;
  int *valeur = malloc(sizeof(int));
  switch (expr->nature) {
  case N_IDF:
    trouve = estPresentTS(expr->idf, valeur);
    if (trouve){
      return *valeur;
    }
    else{
      fprintf(stderr, "ERREUR : variable introuvable\n");
      exit(1);
    }
  case VALEUR:
    return expr->valeur;
    break;
  case OPERATION:
    Vg = evaluation(expr->gauche);
    Vd = evaluation(expr->droite);
    switch (expr->operateur) {
    case N_PLUS:
      return Vg + Vd;
      break;
    case N_MOINS:
      return Vg - Vd;
      break;

    case N_MUL:
      return Vg * Vd;
      break;
    case N_DIV:
      return Vg / Vd;
      break;
    }
    break;
  default:
    fprintf(stderr,"ERREUR de calcul\n");
    break;
  }
  exit(1);
}

void Rec_op1( TypeOperateur *Op) {
  switch (lexeme_courant().nature) {
  case PLUS:
    *Op = N_PLUS;
    avancer();
    break;

  case MOINS:
    *Op = N_MOINS;
    avancer();
    break;
  default:
    printf("Erreur dans l'operateur 1\n");
    break;
  }
}
void Rec_op2( TypeOperateur *Op) {
  switch (lexeme_courant().nature) {
  case MUL:
    *Op = N_MUL;
    avancer();
    break;
  case DIV:
    *Op = N_DIV;
    avancer();
    break;
  default:
    printf("Erreur dans l'operateur 2\n");
    break;
  }
}

void Rec_eag(Ast *A) {
  Rec_seq_terme(A);
}
void Rec_facteur(Ast *A) {
  switch (lexeme_courant().nature) {
  case ENTIER:
    *A = creer_valeur(lexeme_courant().valeur);
    avancer();
    break;
  case PARO:
    avancer();
    Rec_eag(A);
    if (lexeme_courant().nature == PARF) {
      avancer();
    } else {
      printf("Erreur3\n");
    }
    break;
  case IDF:
    *A = creer_idf(lexeme_courant().chaine);
    avancer();
    break;
  default:
    printf("\n\nerreur\n\n");
  }
}

void Rec_terme( Ast *A) {
  Rec_seq_facteur(A);
}
void Rec_seq_terme(Ast *A) {
  Ast Ai;
  Rec_terme(&Ai);
  Rec_suite_seq_terme(&Ai, A);
}
void Rec_seq_facteur(Ast *A) {
  Ast Ai;
  Rec_facteur(&Ai);
  Rec_suite_seq_facteur(&Ai, A);
}

void Rec_suite_seq_facteur(Ast *Ag, Ast *A) {
  Ast Ad, Ai;
  TypeOperateur Op;
  switch (lexeme_courant().nature) {
  case MUL:
  case DIV:
    Rec_op2( &Op);
    Rec_facteur(&Ad);
    Ai = creer_operation(Op, *Ag, Ad);
    Rec_suite_seq_facteur(&Ai, A);
    break;
  default:
    *A = *Ag;
    break;
  }
}

void Rec_suite_seq_terme(Ast *Ag, Ast *A) {
  Ast Ad;
  Ast Ai;
  TypeOperateur Op;
  switch (lexeme_courant().nature) {
  case PLUS:
  case MOINS:
    Rec_op1(&Op);
    Rec_terme(&Ad);
    Ai = creer_operation(Op, *Ag, Ad);
    Rec_suite_seq_terme(&Ai, A);
    break;
  default:
    *A = *Ag;
    break;
  }
}


void Rec_seq_inst(Ast *A){
  Ast A1;
  Rec_inst(&A1);
  Rec_suite_seq_inst(&A1, A);
}
void Rec_suite_seq_inst(Ast *A1, Ast *A){
  Ast A2;
  switch(lexeme_courant().nature){
    case SEPAFF:
      avancer();
      Rec_seq_inst(&A2);
      *A = creer_seqinst(*A1, A2);
      break;
    default:
      *A = *A1;
      break;
  }
}
void Rec_inst(Ast *A) {
  Ast Ag;
  Ast Ad;
  Ag = (Ast) malloc (sizeof(NoeudAst));
  switch (lexeme_courant().nature) {
  case IDF: 
    Ag = creer_idf(lexeme_courant().chaine);
    avancer();
    if (lexeme_courant().nature == AFF) {
      avancer();
    } else {
      printf("Erreur6\n");
    }
    Rec_eag(&Ad); 
    *A = creer_aff(Ag, Ad);
    break;
  case LIRE:

    avancer();
    if (lexeme_courant().nature == PARO) {
      avancer();
    } else {
      printf("Erreur7\n");
    }
    
      Ag = creer_idf(lexeme_courant().chaine);
      avancer();
    *A = creer_lire(Ag);
    if (lexeme_courant().nature == PARF) {
      avancer();
    } else {
      printf("Erreur9\n");
    }
    break;
  case ECRIRE:
    avancer();
    if (lexeme_courant().nature == PARO) {
      avancer();
    } else {
      printf("Erreur10\n");
    }
    Rec_eag(&Ag);
    *A = creer_ecrire(Ag);
    if (lexeme_courant().nature == PARF) {
      avancer();
    } else {
      printf("Erreur11\n");
    };
    break;
  default:
    break;
  }
}
void interpreter_aff(Ast A){
  char *idf= malloc(10 * sizeof(char));
  int v;
  strcpy(idf, A->gauche->idf);
  v = evaluation(A->droite);
  insererTS(idf, v);
}
void interpreter_lire(Ast A){
  int v;
  printf(" lire la variable de clavier : ");
  scanf("%d",&v);
  printf("\n");
  insererTS(A->gauche->idf, v);
}
void interperter_ecrire(Ast A){
  int v;
  v = evaluation(A->gauche);
  printf("la valeur de ");
  afficherA(A->gauche);
  printf("   est %d\n",v);
}
void interpreter(Ast A){
  switch(A->nature){
    case N_SEPINST:
      interpreter(A->gauche);
      interpreter(A->droite);
      break;
    case N_AFF:
      interpreter_aff(A);
      break;
    case N_LIRE:
      interpreter_lire(A);
      break;
    case N_ECRIRE:
      interperter_ecrire(A);
      break;
    default:
      break;
  }
}
void analyser(char *fichier, Ast *A) {
  demarrer(fichier);
  initTS();
  Rec_seq_inst(A);
  afficherA(*A); // on affiche l'AST
  interpreter(*A);
  printf("La table des symboles: \n");
  afficheTS();
}
