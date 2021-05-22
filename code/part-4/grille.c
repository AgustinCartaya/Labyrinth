//auteur: CARTAYA Agustin

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
// #include "tests.h"


#define MARK 2 // case marquée
#define LIBRE 0  //case libre (chemin)
#define MUR 1 //case occupée (mur)
#define STACK_UNDERFLOW -1 //return pile stack underflow


//--------- Methodes -------------
//Partie 1
int getID(int ligne, int colonne);
int getLigne(int id);
int getCol(int id);
void modifie(int ligne, int colonne, char x);
char lit(int ligne, int colonne);
void affiche();//**
void afficheTab( int t[], int n);//**
//fin Partie 1

//Partie 2
void push(int x);
int pop();
void viderPile();

int connexe();
int countP(int type, int *idPremiere );
int count(int type);
void marquer( int id);
void markVoisin( int id );
void markAndPush( int id);


//génération  du labyrinthe
void genLaby(int k);
void genLabyInternal(int noirs, int alea);
void voisinsPointer(int id, char **t);
void voisinsID(int id, int t[]);
void genPerm(int t[], int n);
void nettoyer();
void pile2tas( char * tas, char *pile, int n);
//fin Partie 2

//Partie 3
void genLabyByList(char data[]);
char* genDefaultLaby(int lig, int col, int connexe);
int** lab2Graphe( int *casesLibres, int *caseLibreInitial, int *caseLibreFinal  );
int* dijkstra( int** tabNoeud, int n, int init, int fin);
int* cheminMin(int caseInitial, int caseFinal);
int distMin(int caseInitial, int caseFinal);
void desingChemin( int* cases,  char mark);
//fin Partie 3

//------------- Fonctions de tests --------------
//------------- Partie 1
int test_getID();
int test_getLigne();
int test_getColonne();
int test_modifie();
int test_lit();

//------------- Partie 2
int test_push();
int test_pop();
int test_connexe();
//------------- fin Fonctions de tests --------------


//------------- Variables globales -------------

int NB_COLONNES = 10; //Longueur de la grille (nombre de colonnes)
int NB_LIGNES = 15; //Largeur de la grille (nombre de lignes)
#define NB_CASES NB_COLONNES*NB_LIGNES//Nombre de cases

char AFF_VIDE  = '-';//Caractère représentant les cases vides pour l’affichage
char AFF_MUR = 'x'; //Caractère représentant les murs pour l’affichage
char AFF_BORD = ' '; //Caractère représentant les bords pour l’affichage
char AFF_CHEMIN = '*';

char* Grille = NULL;
int* Pile = NULL;
int Sommet = 0;

int Ale_labyranthe = 1;

int main()
{

  srand((unsigned) time(NULL));

  //Initializaion du tableau et pile
  //(commenter ces deux lignes si:
  //  -mode test
  //  -génération  du tableau par la fonction genlabybylist()
  Grille = (char*)calloc(NB_CASES, sizeof(char));
  Pile = (int*)calloc(NB_CASES, sizeof(int));


  //Tests ----------------------------------------------------------------------

  //partie 1
  // printf("getID: %d\n", tst_getID());
  // printf("tst_getLigne: %d\n", tst_getLigne());
  // printf("tst_getColonne: %d\n", tst_getColonne());
  // printf("test_modifie: %d\n", test_modifie());
  // printf("test_lit: %d\n", test_lit());

  //partie 2
  // printf("test_push: %d\n", test_push());
  // printf("test_pop: %d\n", test_pop());
  // printf("test_connexe: %d\n", test_connexe() );

  //fin Tests ------------------------------------------------------------------


  //Génération du labyrinthe ---------------------------------------------------

  //Mode aléatoire ***********
  genLaby(80);
  // ***********

  //Mode par défaut ***********
  // char* charLaby = genDefaultLaby(6,7,1);
  // genLabyByList(charLaby);
  // free(charLaby);
  // ***********

  affiche();

  //fin generetion du laMode par défautbyrinthe -----------------------------------------------

  //connexite ------------------------------------------------------------------
  printf("Connexe: %d\n", connexe());


  //plus court chemin ----------------------------------------------------------
  //Chemin ****************
  int* chemin = cheminMin(0,-1);
  desingChemin(chemin, 2 );
  affiche();
  printf("Distance minimale: %d\n", chemin[0]-1);
  free(chemin);
  //****************

  //Distance minimale ****************
  // printf("Distance minimale: %d\n", distMin(0,-1));
  //****************


  //fin plus court chemin ------------------------------------------------------

  //liberation du tableau et pile
  free(Grille);
  free(Pile);

  return 0;

}

// ----------------------------------------------PARTIE 1 --------------------------------------------------------------

/**
* retourne l'identifiant d'une case dont on donne les coordonnées
**/
int getID(int ligne, int colonne)
{
  return ligne * NB_COLONNES + colonne ;
}

/**
* retourne la première coordonnée (ligne) d'une case dont on donne l'identifiant
**/
int getLigne(int id)
{
  return id/NB_COLONNES;
}

/**
* retourne la deuxième coordonnée (colonne) d'une case dont on donne l'identifiant
**/
int getCol(int id)
{
  return id%NB_COLONNES;
}

/**
* place la valeur x dans le case de coordonnée (i,j)
**/
void modifie(int ligne, int colonne, char x)
{
  Grille[ getID(ligne, colonne) ] = x;
}

/**
* retourne la valeur de la case de coordonnées (i,j)
**/
char lit(int ligne, int colonne)
{
  return Grille[ getID(ligne, colonne) ];
}

/**
* affiche la grille
*/
void affiche()
{

  for (int i = 0; i <= (NB_LIGNES+1); i++)
  {
    for (int j = 0; j <= (NB_COLONNES+1); j++)
    {
      if( (i*j) == 0 || i == (NB_LIGNES+1) || j == (NB_COLONNES+1))
        printf( " %c ", AFF_BORD );
      else if( lit(i-1, j-1) == 2){
        printf( "%c", AFF_CHEMIN);
      }
      else if( lit(i-1, j-1) )
      {
        printf( "%c", AFF_MUR);
        // printf( " %d ", Grille[ getID(i-1, j-1) ] );
      }
      else
      {
        printf( "%c", AFF_VIDE );
        // printf( " %d ", Grille[ getID(i-1, j-1) ] );
      }

    }
    printf("\n");
  }

}

/*
* Aficche un tableau quelconque
*/
void afficheTab( int t[], int n)
{

  for (int i = 0; i <n; i++)
  {
      printf("%d, ", t[i] );
  }
  printf("\n" );
}

// ----------------------------------------------PARTIE 2 --------------------------------------------------------------

/*
* Inserer une case dans la Pile
*/
void push(int x)
{
  if( Sommet < (NB_CASES) )
  {
    Pile[ Sommet ] =  x;
    Sommet++;
  }
  else
  {
    printf("stackOverflow\n");
  }
}

/*
* Remover une case de la Pile
*/
int pop()
{
  if( Sommet >0 )
  {
    Sommet--;
    return Pile[ Sommet ];

  }
  else
  {
    return STACK_UNDERFLOW;
  }

}

/*
* vider la Pile
*/
void viderPile()
{
  Sommet = 0;
}

//Connexité ------------------------------------------------

/*
*Vérifier si toutes les cases sont connexes
*/
int connexe()
{

  int conex = 0; //1 = labyrinthe connexe
  int cconex = 0; // nombre de cases marquees
  int ncb = 0; //nombre de cases libres
  int id = -1; //id de la case courrant


  ncb = countP( LIBRE, &id);
  marquer(id);
  push( id );

  while( Sommet!=0  )
  {
    int id = pop();
    markVoisin(id);
  }

  for (int i = 0; i < NB_CASES; i++)
  {
    if( Grille[i] == MARK )
    {
      cconex++;
      Grille[i] = 0;
    }
  }
  conex = cconex == ncb;
  return conex;
}

/*
* Parcours du tableau pour compter les nombres de cases "type"
* et enregistrer la position de la première case type dans idPremiere
*/
int countP(int type, int *idPremiere )
{
  int nbCases = 0;
  *idPremiere = -1;
  for (int i = 0; i < NB_CASES; i++)
  {
    if(Grille[i] == type)
    {
      nbCases++;
      if(*idPremiere == -1)
        *idPremiere = i;
    }
  }
  return nbCases;
}

/**
* Parcours du tableau pour compter les nombre de cases "Type"
*/
int count( int type )
{
  int a;
  return countP(type, &a);
}

/*
* marque une case avec la marque par defaut
*/
void marquer( int id)
{
  Grille[id] = MARK;
}



/**
* renvoi un tableau  de 4 positions avec les id des voisins conexes de la case id et qui ont comme valeur 'a'
* si le voisin n'existe pas ou la case n'est pas égale a 'a' le valeur de la case sera égale a -1
*/
void voisinsConexID(int id, int t[], char a)
{
  int lig = getLigne(id);
  int col = getCol( id );
  for (int i = 0; i < 4; i++)
    t[i] = -1;

  if( col != 0 && Grille[id-1] == a )
    t[1] = id-1;

  if( col < (NB_COLONNES-1) && Grille[id + 1] == a)
    t[2] = id+1;

  if( lig != 0  && lit(lig-1, col) == a )
    t[0] = getID(lig-1, col );

  if( lig < (NB_LIGNES-1) && lit(lig+1, col) == a )
    t[3] = getID(lig+1, col );

}


/*
* Marque les voisins et les mets das la Pile
*/
void markVoisin( int id)
{
  int t[4];
  voisinsConexID(id, t, LIBRE);

  for (int i = 0; i < 4; i++) {
    if(t[i] != -1){
      push( t[i] );
      marquer( t[i] );
    }
  }
}


//génération du labyrinthe ------------------------------------------------


/*
*  Génère un labyrinthe connexe avec une quantité de cases blanches k
*/
void genLaby(int k)
{

  printf("--------- Creation du labyrinthe %d x %d --------------\n", NB_LIGNES,NB_COLONNES);
  int c = 0;
  int kk = k;
  int maxEssais = NB_CASES*1000;

  if ( k < (NB_COLONNES+NB_LIGNES) )// le cas ou les cases blanches ne sont pas sufissantes
    kk = (NB_COLONNES+NB_LIGNES+2);
  else if(k > NB_CASES)// le cas ou les cases blanches sont plus grands que le tableaux
      kk = NB_CASES/2;

  do
  {
    c++;

    //génération  si un labyrinthe avec (NB_CASES-kk) cases noirs
    genLabyInternal(NB_CASES-kk, Ale_labyranthe);

    //cas ou la génération  du labyrinthe n'est pas possible avec la quantité de cases blanches donne
    if(c > maxEssais && c%100 == 0)
      kk++; //augmentation de cases blanches
  }
  while(!connexe() );//jusqu’à trouver un labyrinthe connexe

  printf("labyrinthe cree apres %d essais\n",c);
  printf("  Cases  \tAttendu\t\tReel\n" );
  printf("Noires:\t\t%d\t\t%d\n", NB_CASES - k, count(1)  );
  printf("Blanches:\t%d\t\t%d\n", k, count(0)  );
  printf("Total:\t\t%d\t\t%d\n",NB_CASES ,count(0)+count(1) );
  printf("-------------------------------------------------------");
}

/*
* génération interne du labyrinthe avec un nombre de cases mur k et
* une paramètre aléatoire 0 aléatoire minimum 2 aléatoire max
*/
void genLabyInternal(int noirs, int alea)
{

  int minNoir = (int)(NB_COLONNES/2) * (int)(NB_LIGNES/2); //min de cases noires
  int perm[ minNoir ]; //tableau de permutation de cases noir
  char *voisins[9]; //tableau de voisins
  int al = 0; //index aleatoire du voisin
  int ck = 0; //compteur de cases noires


  nettoyer( LIBRE );
  genPerm( perm,minNoir );

  //compteur cyclique des cases noires guides
  int c = 0;

  while( ck < noirs)
  {
    int i = c % minNoir ; //pour répéter les cases noires guides

    //permutation que permet de ne pas faire les case noir guides  par ordre mais aleaoires
    int id = 2*perm[i] + 1;
    id = (2*getLigne(id)+ (rand()%2)*1)*NB_COLONNES + id%NB_COLONNES;

    //chercher les voisins aléatoires des cases noirs guides
    voisinsPointer( id, voisins);

    //obtention du voisin aleatoire
    if(alea > 1)
      al = rand()%9 ; //on peut choisir les toutes les cases voisines (id inclut)
    else
     al = 2*(rand()%4)+1 ; //on ne peut choisir que les cases voisines connexes (id pas inclut)

    //Marquer la case noire guide
    int ob = 0;
    if(alea > 0 )
      ob = rand()%2;

    if( ob ==0 &&
        *voisins[4] != MUR &&
        voisins[4]  != &Grille[0] &&
        voisins[4]  != &Grille[NB_CASES-1])
    {
      *voisins[4] = MUR;
      ck++;
    }    //marque la case voisine si elle existe et si elle n'es pas marquée
    if( voisins[al]  != &Grille[0] &&
        voisins[al]  != &Grille[NB_CASES-1] &&
        voisins[al] != voisins[4] &&
        *voisins[al] != MUR
      )
    {
      *voisins[al] = MUR;
      ck++;
    }

    c++;
  }

}

/*
* Créer la permutation des n premières inters dans le tableau t
*/
void genPerm(int t[], int n)
{
  int temp = 0;
  int b = 0;

  for(int i = 0; i< n; i++)
  {
      b = rand() % (i+1);
      if(b!=i)
      {
        temp = t[b];
        t[b] = i;
        t[i] = temp;
      }
      else
      {
        t[i] = i;
      }
  }

}

/**
* renvoi un tableau de pointeurs de 9 positions avec les adresses des
* voisins de la case id
* la case id est contenue dans le tableau dans la position 4
* si le voisin n'existe pas le case va pointer au même adresse du id
*/
void voisinsPointer(int id, char **t)
{

    int lig = getLigne(id);
    int col = getCol( id );
    for (int i = 0; i < 9; i++)
      t[i] = &(Grille[id]);


    if( col != 0   )
    {
      t[3] = &(Grille[id-1]);
      if( lig != 0  )
        t[0] = &(Grille[ getID(lig-1, col-1 )]);
      if( lig < (NB_LIGNES-1)  )
        t[6] = &(Grille[ getID(lig+1, col-1 )]);
    }

    if( col < (NB_COLONNES-1) )
    {
        t[5] = &(Grille[id+1]);
        if( lig != 0 )
          t[2] = &(Grille[ getID(lig-1, col+1 )]);
        if( lig < (NB_LIGNES-1)  )
          t[8] = &(Grille[ getID(lig+1, col+1 )]);
    }

    if( lig != 0 )
    {
      t[1] = &(Grille[ getID(lig-1, col )]);
    }

    if( lig < (NB_LIGNES-1) )
    {
      t[7] = &(Grille[ getID(lig+1, col )]);
    }

}

/**
* renvoi un tableau  de 9 positions avec les voisins de la case id
* la case id est contenue dans le tableau dans la position 4
* si le voisin n'existe pas le case va pointer au même adresse du id
*/
void voisinsID(int id, int t[])
{
  int lig = getLigne(id);
  int col = getCol( id );
  for (int i = 0; i < 9; i++)
    t[i] = id;

  if( col != 0 )
  {
    t[3] = id-1;
    if( lig != 0 )
      t[0] = getID(lig-1, col-1 );
    if( lig < (NB_LIGNES-1) )
      t[6] = getID(lig+1, col-1 );

  }

  if( col < (NB_COLONNES-1) )
  {
      t[5] = id+1;
      if( lig != 0 )
        t[2] = getID(lig-1, col+1 );
      if( lig < (NB_LIGNES-1))
        t[8] = getID(lig+1, col+1 );
  }

  if( lig != 0 )
  {
    t[1] = getID(lig-1, col );
  }

  if( lig < (NB_LIGNES-1)  )
  {
    t[7] = getID(lig+1, col );
  }

}



/**
* met toutes les cases de la grilla avec une valeur égale a m
*/
void nettoyer(int m)
{
  for (int i = 0; i < NB_CASES; i++)
  {
    Grille[i] = m;
  }
}


//----------------------------------------------- PARTIE 3 -----------------------------------------------
/**
* Générer un labyrinthe avec un tableau descripteur de type char passé comme paramètre
* contenant toute l'information du labyrinthe
*   -Les deux premières valeurs du tableau sont le nombre de lignes et le nombre de colonnes de la grille.
*   -Les valeurs suivantes sont les contenus des cellules de la grille (0 ou 1)
*/
void genLabyByList(char data[])
{

  NB_LIGNES = data[0];
  NB_COLONNES = data[1];

  Grille = (char*)calloc(NB_CASES, sizeof(char));
  Pile = (int*)calloc(NB_CASES, sizeof(int));

  for (int i = 0; i < NB_CASES; i++) {
    Grille[i] = data[i+2];
  }
}




/**
 renvoi le labyrinthe en formme de graphe
 renvoi un tableau où chaque ligne indique une casee libre les colonnes les connexions de cette case  avec les autres cases libres voisines
*/
int** lab2Graphe( int *casesLibres, int *caseLibreInitial, int *caseLibreFinal  )
{

  //tableau utilisé pour compter le nombre de cases libres
  //les cases libres sont numérotés à partir de 0
  //les cases occupées ont une valeur de -1
  /*Exemple:
  position   0    1   2   3   4   5   6   7   8

  Grille =  [0,   1,  1,  0,  0,  1,  1,  0,  0]
  id2libreID[0,  -1, -1,  1,  2, -1, -1,  3,  4]

  */
  int id2libreID[NB_CASES];
  *casesLibres = 0; //nombre de cases libres

  int truve_caseLibreInitial = -1;
  int truve_caseLibreFinal = -1;

  for (int i = 0; i < NB_CASES; i++) {
    if( Grille[i] == LIBRE)
    {
      //converir id a numero de case blanche
      if(*caseLibreInitial == i){
        truve_caseLibreInitial = *casesLibres;
      }
      if(*caseLibreFinal == i){
        truve_caseLibreFinal = *casesLibres;
      }
      //fin converir id a numero de case blanche

      id2libreID[i] = *casesLibres;
      *casesLibres = *casesLibres + 1;
    }
    else
    {
      id2libreID[i] = -1;
    }
  }

   //convertion de id a case libre
  *caseLibreInitial = truve_caseLibreInitial;
  *caseLibreFinal = truve_caseLibreFinal;
   if(*caseLibreInitial < 0 )
     *caseLibreInitial = 0;
   //convertion hors de rangue
   if(*caseLibreFinal < 0 )
     *caseLibreFinal = *casesLibres-1;


  int cTab = 0;

  //Cet array bidimensionnel contiendra un tableau où chaque colonne représentera une case blanche
  //et les lignes de cette colonne les cases voisines blanches connexes
  // la dernière colonne de la file représente l'id de la case libre courante

  /* Labyrinthe        nombre de cases         nombre de cases blanches
      0 1 1                 0 1 2                      0 -1 -1
      0 0 1                 3 4 5                      1  2 -1
      1 0 0                 6 7 8                     -1  3  4

    table de Connexité
     up     <-    ->    down    id          Explication:
     -1    -1    -1     1      0            la case blanche #0 avec id = 0 a comme voisine(s) la case blanche #0
      0    -1     2    -1      3            la case blanche #1 avec id = 3 a comme voisine(s) la case blanche #0, #2
     -1     1    -1     3      4            la case blanche #2 avec id = 4 a comme voisine(s) la case blanche #1, #3
      2    -1     4    -1      7            la case blanche #3 avec id = 7 a comme voisine(s) la case blanche #2, #4
     -1     3    -1    -1      8            la case blanche #4 avec id = 8 a comme voisine(s) la case blanche #3
  */

  int** tab = (int**)calloc(*casesLibres, sizeof(int*));

  for (int i = 0; i < NB_CASES && cTab < *casesLibres; i++)
  {
    if(Grille[i] == LIBRE)
    {

      int tVoisinsConexID[4];
      voisinsConexID(i, tVoisinsConexID, LIBRE );
      tab[ cTab ] = (int*) calloc(5, sizeof(int));
      for (int j = 0; j < 4; j++)
      {
        if (tVoisinsConexID[j] != -1 )
          tab[ cTab ][j] = id2libreID[ tVoisinsConexID[j] ];//les cases voisines sont ajoutées
        else
          tab[ cTab ][j] = -1;
      }
      tab[cTab][4] = i; //l'id de la case courante est ajouté dans la dernière colomne de la ligne
      cTab++;

    }
  }

  return tab;
}

int* dijkstra( int** tabNoeud, int n, int init, int fin)
{
  //on crée la table dijkstra avec des 0 à l'intérieur
  int tab[n][n][2];
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      tab[i][j][0] = 0; //le nœud parent
      tab[i][j][1] = 0; //valeur cumulée minimale
    }
  }
  int pa = init; //position actual

  //donner des valeurs à la première colonne qui sont les fils du nœud init
  for (int i = 0; i < 4; i++){
    if( tabNoeud[init][i] != -1)
    {
      tab[0][ tabNoeud[init][i] ][0] = init; //on etabli comment pere le nœud init
      tab[0][ tabNoeud[init][i] ][1] = 1; //la valeur est 1 car on n'ai fait qu'un pas
      if( pa == init)
        pa = tabNoeud[init][i]; //choisir comme position actuelle le premier nœud connecté libre enfant de init trouvé
    }
  }
  tab[n-1][ init ][1] = -1; //Fermer le nœud init dans la table car il ne sera plus traversé

  //traverser la table jusqu'à atteindre le dernier nœud libre ou jusqu'à atteindre le nœud final
  for (int pas = 1; pas < n && pa != fin; pas++)
  {
    //fermeture du nœud actuel
    tab[n-1][ pa ][1] = -1;

    //chercher les noeud cennexes au noeud courrant
    for (int i = 0; i < 4; i++)
    {
      if( tabNoeud[pa][i] != -1 && tab[n-1][ tabNoeud[pa][i] ][1] !=-1 )
      { //pas: mouvements horizontaux de la table
        //tabNoeud[pa][i]: nœud connecté au nœud courrant
          if( tab[pas-1][ tabNoeud[pa][i] ][1] == 0 || //si c'est la première fois que ce nœud est visité
            (tab[pas][ pa ][1] +1) < tab[pas-1][ tabNoeud[pa][i] ][1] ) // si ce prédécesseur est meilleur
          {
            tab[pas][ tabNoeud[pa][i] ][0] = pa; //définir pa comme nœud prédécesseur
            tab[pas][ tabNoeud[pa][i] ][1] = tab[pas-1][ pa ][1] +1; //ajouter un à la valeur accumulée
          }
          else
          {
            tab[pas][ tabNoeud[pa][i] ][0] = tab[pas-1][ tabNoeud[pa][i] ][0]; //copier le prédécesseur
            tab[pas][ tabNoeud[pa][i] ][1] = tab[pas-1][ tabNoeud[pa][i] ][1]; //copier la valeur accumulée

          }

      }
    }


      //copier la valeur précédent de tous les autres nœuds qui n'ont pas été traversés dans cette étape
      //et voir qui a la valeur minimale pour etablir comme noeud courrant
      int mp = -1; //meilleur prix
      for (int noeud = 0; noeud < n; noeud++)
      {

        //si le noeud n'est pas bloqué et  (s'il a déjà été visité ou vient de l'être)
        if(tab[n-1][noeud][1] !=-1 && (tab[pas-1][noeud][1] != 0 || tab[pas][noeud][1] != 0 ) )
        { //s'il ne fait pas partie des voisins du noeud courrant (copier les valeurs)
          if( tab[pas][noeud][1] == 0)
          {
            tab[pas][ noeud][0] = tab[pas-1][ noeud ][0];
            tab[pas][ noeud ][1] = tab[pas-1][ noeud ][1];
          }
          //recherche du meilleur prix
          if( mp == -1 || tab[pas][ noeud ][1] < mp)
          {
            mp = tab[pas][ noeud ][1];
            pa = noeud;
          }

        }

      }

  }

  //Parcours a l'inverse du tableau dijkstra pour obtenir les resultats

  //array qui contiendra le plus court chemin
  int* chemin = (int*)calloc(n, sizeof(int));

  int pos0 = fin; //position actuelle
  int cChemin = 2; //compteur de pas
  chemin[1] = tabNoeud[fin][4]; //le premier noeud a parcourir est le noeud d’arrivée

  //backtracking pour obtenir l'information
  do
  {
    int dec = n;
    do
    {
      dec--;

    } while( tab[dec][pos0][1]<= 0 && dec >= 0);
    pos0 = tab[dec][pos0][0];
    chemin[cChemin] = tabNoeud[pos0][4];
    cChemin++;

  }
  while ( pos0 != init &&  cChemin<=(n+1));
  chemin[0]=cChemin-1;

  chemin = (int*)realloc(chemin, cChemin*sizeof(int));

  //Montrer et effacer le tableau
  for (int i = 0; i < n; i++) {
    // printf("%d: ",i);
    // for (int j = 0; j < n; j++) {
      // printf("[%d,%d] ",tab[j][i][0],tab[j][i][1] );
    // }
    // printf("\n");
    free(tabNoeud[i]);
  }
  free(tabNoeud);


  return chemin;
}

/*
Renvoi un array avecl les id des cases qui sont le chemin de cout minimum de en partant de la caseInitial jusqu'a la  caseFinal
 caseInitial: id de la case initial à partir de laquel le Parcours va commencer
 caseLibreFinal: id de la case final
*/
int* cheminMin(int caseInitial, int caseFinal)
{
  int casesLibres;
  int caseLibreInitial = caseInitial;
  int caseLibreFinal = caseFinal;
  int** tab = lab2Graphe( &casesLibres, &caseLibreInitial, &caseLibreFinal );

  int *chemin = dijkstra( tab, casesLibres,caseLibreInitial, caseLibreFinal);
  return chemin;
}

/*
Renvoi un le cout d'alle de la caseInitial jusqu'a la  caseFinal
 caseInitial: id de la case initial à partir de laquel le Parcours va commencer
 caseLibreFinal: id de la case final
*/
int distMin(int caseInitial, int caseFinal)
{
  return cheminMin(caseInitial, caseFinal)[0]-1;
}

void desingChemin( int* cases,  char mark){
  for (int i = 1; i <= cases[0]; i++) {
    Grille[cases[i]] = mark;
  }

}


//--------------- Foctions de test ---------------------------------------
//--------------- test partie 1
/**
* Test la fonction getID avec 100 marices de different taille
* 1: fonction fonctione sans probleme
* 0: fonction avec des problemes
*/
int test_getID()
{
  int error = 0;
  int o_NB_LIGNES = NB_LIGNES;
  int o_NB_COLONNES = NB_COLONNES;

  for (int nbl = 1; nbl <= 10 && !error; nbl++)
  {
    NB_LIGNES = nbl;
    for (int nbc = 1; nbc <= 10 && !error; nbc++)
    {
      NB_COLONNES = nbc;
      int id = 0;
      for(int i =0; i<NB_LIGNES && !error; i++)
      {
        for(int j =0; j<NB_COLONNES && !error; j++)
        {
          error = id != getID(i, j);
          id++;
        }

      }
    }
  }
  NB_LIGNES = o_NB_LIGNES;
  NB_COLONNES = o_NB_COLONNES;

  return !error;
}


/**
* Test la fonction getLigne
* 1: fonction fonctione sans probleme
* 0: fonction avec des problemes
*/
int test_getLigne(){
  int error = 0;
  int o_NB_LIGNES = NB_LIGNES;
  int o_NB_COLONNES = NB_COLONNES;

  //test avec differentes matrices de taille m,n {m,n E [1,10]}
  for (int nbl = 1; nbl <= 10 && !error; nbl++)
  {
    NB_LIGNES = nbl;
    for (int nbc = 1; nbc <= 10 && !error; nbc++)
    {
      NB_COLONNES = nbc;
      int c_ligne = -1;
      for( int id = 0; id< NB_CASES && !error; id++ ){

        if(id % NB_COLONNES == 0)
          c_ligne++;
        error = c_ligne != getLigne(id);
      }
    }
  }

  NB_LIGNES = o_NB_LIGNES;
  NB_COLONNES = o_NB_COLONNES;
  return !error;
}

/**
* Test la fonction getColonne
* 1: fonction fonctione sans probleme
* 0: fonction avec des problemes
*/
int test_getColonne(){
  int error = 0;
  int o_NB_LIGNES = NB_LIGNES;
  int o_NB_COLONNES = NB_COLONNES;

  //test avec differentes matrices de taille m,n {m,n E [1,10]}
  for (int nbl = 1; nbl <= 10 && !error; nbl++)
  {
    NB_LIGNES = nbl;
    for (int nbc = 1; nbc <= 10 && !error; nbc++)
    {
      NB_COLONNES = nbc;
      int c_colonne = 0;
      for( int id = 0; id< NB_CASES && !error; id++ ){

        if(id % NB_COLONNES == 0)
          c_colonne = 0;
        else
          c_colonne++;

        error = c_colonne != getCol(id);
      }
    }
  }

  NB_LIGNES = o_NB_LIGNES;
  NB_COLONNES = o_NB_COLONNES;

  return !error;
}


/**
* Test la fonction modifie
* 1: fonction fonctione sans probleme
* 0: fonction avec des problemes
*/
int test_modifie()
{
  int error = !test_lit();
  char* o_Grille = Grille;
  int o_NB_LIGNES = NB_LIGNES;
  int o_NB_COLONNES = NB_COLONNES;


  //test avec differentes matrices de taille m,n {m,n E [1,10]}
  for (int nbl = 1; nbl <= 10 && !error; nbl++)
  {
    NB_LIGNES = nbl;
    for (int nbc = 1; nbc <= 10 && !error; nbc++)
    {
      NB_COLONNES = nbc;
      Grille = (char*)calloc(NB_CASES, sizeof(char) );

      int c_total = 0;
      int c_total_grille = 0;
      for (int i = 0; i < NB_LIGNES && !error; i++) {

        for (int j = 0; j < NB_COLONNES && !error; j++) {
          int rdm = rand()%128;
          modifie(i,j, rdm);
          error = rdm != lit(i,j);
          c_total += rdm;
        }
      }
      //pour verifier que le changement d'une grille ne change pas une aute precedente
      for (int id = 0; id < NB_CASES && !error; id++) {
        c_total_grille += Grille[id];
      }
      error = c_total_grille != c_total;

      free(Grille);
    }
  }

  NB_LIGNES = o_NB_LIGNES;
  NB_COLONNES = o_NB_COLONNES;

  Grille = o_Grille;
  return !error;

}


/**
* Test la fonction lit
* 1: fonction fonctione sans probleme
* 0: fonction avec des problemes
*/
int test_lit()
{
  int error = !test_getID();

  for (int i = 0; i < NB_LIGNES && !error; i++)
  {
      for (int j = 0; j < NB_COLONNES && !error ; j++)
      {
        error =  Grille[i] != lit(i, j);
      }
  }

  return !error;
}

//--------------- test partie 2
/**
* Test la fonction push
* 1: fonction fonctione sans probleme
* 0: fonction avec des problemes
*/
int test_push(){

  int error = 0;
  int *o_Pile = Pile;
  int o_Sommet = Sommet;



  Pile = (int*)calloc(NB_CASES, sizeof(int));
  Sommet = 0;
  for (int i = 0; i < NB_CASES + 5 && !error; i++) {
    push(i);
    if(i<NB_CASES)
    {
      error = Pile[Sommet -1] != i;
    }
    else
    {
      error = Pile[Sommet-1] != NB_CASES - 1;
    }

  }
  error = Sommet != NB_CASES ;

  Pile = o_Pile;
  Sommet = o_Sommet;

  return !error;
}

/**
* Test la fonction pop
* 1: fonction fonctione sans probleme
* 0: fonction avec des problemes
*/
int test_pop(){

  int error = 0;
  int *o_Pile = Pile;
  int o_Sommet = Sommet;

  //on cree une nouvelle pile et on la remplie avec des chiffres
  Pile = (int*)calloc(NB_CASES, sizeof(int));
  for (int i = NB_CASES -1; i >= 0; i--) {
    Pile[i] = i;

  }
  Sommet = NB_CASES;
  //on parcour la pille 5 unites en plus de sa taille et a chaque tour du boucle on enleve un elemente
  for (int i = NB_CASES -1; i >= -5 && !error; i--) {

    if(i>=0)
    {
      error = i != pop() ;
    }
    else
    {
      error = STACK_UNDERFLOW != pop();
    }

  }
  error = Sommet != 0 ;
  Pile = o_Pile;
  Sommet = o_Sommet;

  return !error;
}


/**
* Test la fonction connexe
* 1: fonction fonctione sans probleme
* 0: fonction avec des problemes
*/
int test_connexe(){

  int error = 0;
  char* o_Grille = Grille;
  int o_NB_LIGNES = NB_LIGNES;
  int o_NB_COLONNES = NB_COLONNES;

  int *o_Pile = Pile;
  int o_Sommet = Sommet;


  for (int cnx = 0; cnx < 2 && !error; cnx++) {
    for (int i = 3; i <= 6 && !error; i++) {
      for (int j = 3; j <= 6 && !error; j++) {
        char* charLaby = genDefaultLaby(i,j,cnx);
        genLabyByList(charLaby);
        error = cnx != connexe();
        printf("%d x %d == %d = %d\n",i,j,connexe(), cnx );
        affiche();
        free(charLaby);
        free(Grille);
        free(Pile);
      }
    }

  }
  printf("errir = %d\n",error );

  NB_LIGNES = o_NB_LIGNES;
  NB_COLONNES = o_NB_COLONNES;
  Grille = o_Grille;

  Pile = o_Pile;
  Sommet = o_Sommet;

  return !error;

}



//---------------------------labyranthes par dafaut


/**
* génération de labyranthes de taille lig x col par dafaut pour faire des test
connexe = 1 a fin de creer un labyrante connexe
connexe = 0 a fin de creer un labyrante non connexe

*/
char* genDefaultLaby(int lig, int col, int connexe)
{
  char *defLaby = (char*)calloc(lig*col+2, sizeof(char));
  //---------------------------------------- Connexes
  if(connexe){
    if( lig == 3 ){
      if(col == 3)
      {
        char tab_deflab[] =  {lig,col,
                    0,1,1,
                    0,0,1,
                    1,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));

      }
      else if(col == 4)
      {
        char tab_deflab[] =  {lig,col,
                    0,1,1,1,
                    0,0,1,0,
                    1,0,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));

      }
      else if(col == 5)
      {
        char tab_deflab[] =  {lig,col,
                    0,1,1,1,0,
                    0,0,0,0,0,
                    1,1,1,1,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 6)
      {
        char tab_deflab[] =  {lig,col,
                    0,1,1,1,0,1,
                    0,0,0,0,0,1,
                    1,1,1,1,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }

    }
    if( lig == 4){
      if(col == 3)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,
                    0,1,1,
                    0,0,0,
                    1,1,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 4)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,0,
                    0,1,1,0,
                    0,0,0,0,
                    1,1,1,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));

      }
      else if(col == 5)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,0,1,
                    0,1,1,0,1,
                    1,0,0,0,1,
                    1,1,1,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 6)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,0,1,0,
                    0,1,1,0,1,0,
                    1,0,0,0,1,0,
                    1,1,1,0,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 7 )
      {
        char tab_deflab[] =  {lig,col,
                  0,0,0,0,0,0,0,
                  0,0,0,0,1,0,0,
                  0,0,0,1,0,1,0,
                  0,0,1,0,0,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }

    }
    if( lig == 5){
      if(col == 3)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,
                    0,1,1,
                    0,0,0,
                    1,1,0,
                    1,1,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 4)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,0,
                    0,1,1,1,
                    0,0,0,0,
                    1,1,0,1,
                    1,1,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 5)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,0,1,
                    0,1,1,1,1,
                    0,0,0,0,1,
                    1,1,0,1,0,
                    1,1,0,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 6)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,0,0,0,
                    1,1,0,1,1,0,
                    0,0,0,0,1,0,
                    1,1,0,1,1,0,
                    1,1,0,0,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }

    }
    if( lig == 6){
      if(col == 3)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,
                    0,1,1,
                    0,0,0,
                    1,1,0,
                    1,1,0,
                    0,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 4)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,1,
                    0,1,1,0,
                    0,0,0,0,
                    1,1,0,1,
                    1,1,0,1,
                    0,0,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));

      }
      else if(col == 5)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,1,1,
                    0,1,1,0,1,
                    0,0,0,0,0,
                    1,1,0,1,0,
                    1,1,0,1,0,
                    0,0,0,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 6)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,0,0,0,
                    1,0,1,0,0,1,
                    1,0,1,1,0,1,
                    0,0,1,0,0,1,
                    0,1,0,0,1,0,
                    0,0,0,0,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));

      }
      else if(col == 7)
            {
              char tab_deflab[] =  {lig,col,
                          0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0};
              pile2tas( defLaby, tab_deflab, ((lig*col) + 2));

            }

    }
  }
  //---------------------------------------- Non connexes
  else{
    if( lig == 3 ){
      if(col == 3)
      {
        char tab_deflab[] =  {lig,col,
                    0,1,0,
                    0,1,1,
                    1,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));

      }
      else if(col == 4)
      {
        char tab_deflab[] =  {lig,col,
                    0,1,1,1,
                    0,0,1,0,
                    1,0,1,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));

      }
      else if(col == 5)
      {
        char tab_deflab[] =  {lig,col,
                    0,1,1,1,0,
                    1,0,0,0,0,
                    1,1,1,1,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 6)
      {
        char tab_deflab[] =  {lig,col,
                    0,1,1,1,0,1,
                    0,0,0,0,1,1,
                    1,1,1,1,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }

    }
    if( lig == 4){
      if(col == 3)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,
                    0,1,1,
                    0,1,0,
                    1,1,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 4)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,0,
                    0,1,1,0,
                    0,0,0,1,
                    1,1,1,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));

      }
      else if(col == 5)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,0,1,
                    0,1,1,0,1,
                    1,0,0,0,1,
                    1,1,1,1,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 6)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,0,1,0,
                    0,1,1,0,1,0,
                    1,0,0,0,1,0,
                    1,1,1,0,1,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 7 )
      {
        char tab_deflab[] =  {lig,col,
                  0,0,0,0,0,0,0,
                  0,0,0,0,1,0,0,
                  0,0,0,1,0,1,0,
                  0,0,1,0,0,1,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }

    }
    if( lig == 5){
      if(col == 3)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,
                    0,1,1,
                    1,0,0,
                    1,1,0,
                    1,1,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 4)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,0,
                    0,1,1,1,
                    0,1,0,0,
                    1,1,0,1,
                    1,1,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 5)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,0,1,
                    0,1,1,1,1,
                    0,0,0,0,1,
                    1,0,0,1,0,
                    0,1,0,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 6)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,0,0,0,
                    1,1,1,1,1,0,
                    0,0,0,0,1,0,
                    1,1,0,1,1,0,
                    1,1,0,0,1,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }

    }
    if( lig == 6){
      if(col == 3)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,
                    0,1,1,
                    0,0,0,
                    0,0,0,
                    0,1,1,
                    1,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 4)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,1,
                    0,1,1,0,
                    0,0,1,0,
                    1,1,0,1,
                    1,1,0,1,
                    0,0,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));

      }
      else if(col == 5)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,1,1,
                    0,1,1,0,1,
                    0,0,0,0,0,
                    1,1,0,1,0,
                    1,1,0,1,0,
                    0,0,1,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
      }
      else if(col == 6)
      {
        char tab_deflab[] =  {lig,col,
                    0,0,0,0,0,0,
                    1,0,1,0,1,1,
                    1,0,1,1,0,1,
                    0,0,1,0,0,1,
                    0,1,0,0,1,0,
                    1,0,0,0,0,0};
        pile2tas( defLaby, tab_deflab, ((lig*col) + 2));

      }

    }
  }

  return defLaby;
}

void pile2tas( char * tas, char *pile, int n){
  for (int i = 0; i < n; i++) {
    tas[i] = pile[i];
  }
}
