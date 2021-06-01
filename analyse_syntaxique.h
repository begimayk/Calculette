#ifndef __AN_SYN__
#define __AN_SYN__
#include <stdio.h>
#include "analyse_lexicale.h"
#include "lecture_caracteres.h"
#include "type_ast.h"
#include "ast_construction.h"




void aff_operateur(TypeOperateur op);
void afficher_a(Ast expr);
void Rec_op1(TypeOperateur *Op);
void Rec_op2(TypeOperateur *Op);
void Rec_eag(Ast *A);
void Rec_terme(Ast *A);
void Rec_seq_terme(Ast *A);
void Rec_seq_facteur(Ast *A);
void Rec_suite_seq_facteur(Ast *Ag,Ast *A);
void Rec_suite_seq_terme(Ast *Ag,Ast *A);
void Rec_facteur(Ast *A);
int evaluation(Ast expr);

void Rec_seq_inst(Ast *A);
void Rec_suite_seq_inst(Ast *A1, Ast *A);
void Rec_inst(Ast *A);

void analyser (char *fichier,Ast *A) ;
// e.i : indifferent
// e.f : une Expression Arithmetique a ete lue dans fichier
// si elle ne contient pas d’erreur de syntaxe un message est affiche
// sinon le pgm termine sur un message d’erreur~

#endif
