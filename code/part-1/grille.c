#include <stdio.h>


#define L 4 //Longueur de la grille (nombre de colonnes)
#define M 3 //Largeur de la grille (nombre de lignes)

#define AFF_VIDE '-'//Caractère représentant les cases vides pour l’affichage
#define AFF_MUR 'X' //Caractère représentant les murs pour l’affichage
#define AFF_BORD ' ' //Caractère représentant les bords pour l’affichage



int getID(int ligne, int colonne);
int getLigne(int id);
int getCol(int id);
void modifie(int ligne, int colonne, char x);
char lit(int ligne, int colonne);
void affiche();

char gril[L*M] ={0};

int main(){

  modifie(1,1,1);
  modifie(1,2,1);
  affiche();

  return 0;

}

/**
* retourne l'identifiant d'une case dont on donne les coordonnées
**/
int getID(int ligne, int colonne){
  return ligne * L + colonne+1;
}

/**
* retourne la première coordonnée (ligne) d'une case dont on donne l'identifiant
**/
int getLigne(int id){
  return id/L;
}


/**
* retourne la deuxième coordonnée (colonne) d'une case dont on donne l'identifiant
**/
int getCol(int id){
  return id%L;
}

/**
* place la valeur x dans le case de coordonnée (i,j)
**/
void modifie(int ligne, int colonne, char x){
  gril[ getID(ligne, colonne) ] = x;
}

/**
* retourne la valeur de la case de coordonnées (i,j)
**/
char lit(int ligne, int colonne){
  return gril[ getID(ligne, colonne) ];
}

/**
* affiche la grille
*/
void affiche(){
  
  for (int i = 0; i <= (M+1); i++) {
    for (int j = 0; j <= (L+1); j++) {
      if( (i*j) == 0 || i == (M+1) || j == (L+1))
        printf( "%c", AFF_BORD );
      else if( gril[ getID(i-1, j-1) ] )
        printf( "%c", AFF_MUR );
      else
        printf( "%c", AFF_VIDE );

    }
    printf("\n");
  }

}
