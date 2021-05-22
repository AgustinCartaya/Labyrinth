//auteur: CARTAYA Agustin

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
// #include "tests.h"


#define MARK 2 // case marque
#define LIBRE 0  //case blanche (chemin)
#define MUR 1 //case noir (mur)
#define STACK_UNDERFLOW -1 //case noir (mur)


//--------- Methodes -------------
//Partie 1
int getID(int ligne, int colonne);
int getLigne(int id);
int getCol(int id);
void modifie(int ligne, int colonne, char x);
char lit(int ligne, int colonne);
void affiche();
void afficheTab( int t[], int n);

//Partie 2
void push(int x);
int pop();
void viderPile();

int conexe();
int countP(int type, int *idPremiere );
int count(int type);
void marquer( int id);
void markVoisin( int id );


//generation du labyranthe
void genLaby(int k);
void genLabyInternal(int noirs, int alea);
void voisinsPointer(int id, char **t);
void voisinsID(int id, int t[]);
void genPerm(int t[], int n);
void nettoyer();

//------------- Foctions de test --------------
//------------- Partie 1
int test_getID();
int test_getLigne();
int test_getColonne();
int test_modifie();
int test_lit();

//------------- Partie 2
int test_push();
int test_pop();


//------------- Variables globales -------------

int NB_COLONNES = 10; //Longueur de la grille (nombre de colonnes)
int NB_LIGNES = 6; //Largeur de la grille (nombre de lignes)
int NB_CASES  = 0; //Nombre de cases

char AFF_VIDE  = '-';//Caractère représentant les cases vides pour l’affichage
char AFF_MUR = 'x'; //Caractère représentant les murs pour l’affichage
char AFF_BORD = ' '; //Caractère représentant les bords pour l’affichage


char* Grille = NULL;
int* Pile = NULL;
int Sommet = 0;

int Ale_labyranthe = 1;

int main()
{

  srand((unsigned) time(NULL));

   //initializaion  du tableaux
  NB_CASES = NB_LIGNES * NB_COLONNES;
  Grille = (char*)calloc(NB_CASES, sizeof(char));
  Pile = (int*)calloc(NB_CASES, sizeof(int));


  //Tests
  //partie 1
  // printf("getID: %d\n", tst_getID());
  // printf("tst_getLigne: %d\n", tst_getLigne());
  // printf("tst_getColonne: %d\n", tst_getColonne());
  // printf("test_modifie: %d\n", test_modifie());
  // printf("test_lit: %d\n", test_lit());

  //partie 2
  // printf("test_push: %d\n", test_push());
  // printf("test_pop: %d\n", test_pop());

  //generetion du labyranthe
  genLaby(30);
  affiche();

  free(Grille);
  free(Pile);

  return 0;

}

/**
* retourne l'identifiant d'une case dont on donne les coordonnées
**/
int getID(int ligne, int colonne)
{
  // printf("%s\n", );
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
  // printf("%d\n", getID(ligne, colonne));
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
      else if( Grille[ getID(i-1, j-1) ] )
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
* Aficche un tableau
*/
void afficheTab( int t[], int n)
{

  for (int i = 0; i <n; i++)
  {
      printf("%d, ", t[i] );
  }
  printf("\n" );
}

/*
* Insere une case a la Pile
*/
void push(int x)
{
  if( Sommet < (NB_LIGNES*NB_COLONNES) )
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
* Remove une case de la Pile
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

/*
*Verifier si toutes les cases son connectees
*/
int conexe()
{

  int conex = 0; //1 = connexite; 0 = non connexite
  int cconex = 0; // nombre de cases marques
  int ncb = 0; //nombre de cases libres
  int id = -1; //id de la case courrant


  //Pas 1
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
* Parcour la la tableaux pour compter les nombre de cases "Type" et
* enregistre la position de la premiere case type dans idPremiere
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
* Parcour la la tableaux pour compter les nombre de cases "Type"
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

/*
* Marque les voisins et les mets das la Pile
*/
void markVoisin( int id)
{

  int lig = getLigne(id);
  int col = getCol( id );

  if( col != 0 &&  Grille[id-1] == 0  )
  {
    push( getID(lig, col-1 ) );
    marquer( getID(lig, col-1 ) );
  }

  if( col < (NB_COLONNES-1)  && Grille[id+1] == 0)
  {
    push( getID(lig, col+1 ) );
    marquer( getID(lig, col+1 ) );

  }

  if( lig != 0 && Grille[ getID(lig-1, col ) ] == 0 )
  {
    push( getID(lig-1, col ) );
    marquer( getID(lig-1, col ) );
  }

  if( lig < (NB_LIGNES-1) && Grille[ getID(lig+1, col ) ] == 0 )
  {
    push( getID(lig+1, col ) );
    marquer( getID(lig+1, col )  );
  }

}

/*
* Genere un libyranthe conexe avec une cantite de cases blhes k
*/
void genLaby(int k)
{

  printf("--------- Creation du labyranthe %d x %d --------------\n", NB_LIGNES,NB_COLONNES);
  int c = 0;
  int kk = k;
  int maxEssai = NB_CASES*1000;

  if ( k < (NB_COLONNES+NB_LIGNES) )// le cas ou les cases blanches ne sont pas sufissants
    kk = (NB_COLONNES+NB_LIGNES+2);
  else if(k > NB_CASES)// le cas ou les cases blanches sont plus grands que le tableaux
      kk = NB_CASES/2;

  do
  {
    c++;

    genLabyInternal(NB_CASES-kk, Ale_labyranthe);
    if(c > maxEssai && c%100 == 0)
      kk++;
  }
  while(!conexe() );
  printf("labyranthe cree apres %d essais\n",c);
  printf("   ...   \tAttendu\t\tReel\n" );
  printf("Noires:\t\t%d\t\t%d\n", NB_CASES - k, count(1)  );
  printf("Blanches:\t%d\t\t%d\n", k, count(0)  );
  printf("Total:\t\t%d\t\t%d\n",NB_CASES ,count(0)+count(1) );
  printf("-------------------------------------------------------");
}

/*
* Generation interne du labyranthe avec un nombre de cases mur k et
* une parametrealeatoire 0 aleatoire minnimun 2 aleatoire max
*/
void genLabyInternal(int noirs, int alea)
{

  int minNoir = (int)(NB_COLONNES/2) * (int)(NB_LIGNES/2); //min de cases noir
  int perm[ minNoir ]; //tableauxde permutation de cases noir
  char *voisins[9]; //tableaux de voisins
  int al = 0; //index aleatoire du voisin
  int ck = 0; //compteru de cases noir


  nettoyer( LIBRE );
  genPerm( perm,minNoir );

  //compteur ciclique des cases noires guides
  int c = 0;

  while( ck < noirs)
  {
    int i = c % minNoir ; //pour repeter les cases guides noirs

    //permutation que permet de ne pas faires les case noir guides
    // par ordre mais aleaoires
    int id = 2*perm[i] + 1;
    id = (2*getLigne(id)+ (rand()%2)*1)*NB_COLONNES + id%NB_COLONNES;

    //chercher lar voisins aleatoires des cases noirs guides
    voisinsPointer( id, voisins);

    //obtention du voisin aleatoire
    if(alea > 1)
      al = rand()%9 ; //on peut choisir les toutes les cases voisines (id inclu)
    else
     al = 2*(rand()%4)+1 ; //on ne peut choisir que les cases voisines conexes (id pas inclu)

    //Marquer la case noir obligatoir
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
    }    //marque la case voisine si elle existe et si elle n'es pas marquee
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
* Cree la permutation des n premieres intere dans le tableau t
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
* la case id est contenue dans le tableau dans la pasition 4
* si le voisin n'existe pas le case va pointer au meme adresse du id
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
* la case id est contenue dans le tableau dans la pasition 4
* si le voisin n'existe pas le case va pointer au meme adresse du id
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
* renvoi un tableau  de 4 positions avec les voisins conexes de la case id et qui vauont a
* la case n'est pas contenue dans le tableau
* si le voisin n'existe pas la case vaudra -1
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

/**
* met toutes les cases de la grilla avec une valeur egale a m
*/
void nettoyer(int m)
{
  for (int i = 0; i < NB_CASES; i++)
  {
    Grille[i] = m;
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
      NB_CASES = NB_LIGNES * NB_COLONNES;
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
  NB_CASES = NB_LIGNES * NB_COLONNES;
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
      NB_CASES = NB_LIGNES * NB_COLONNES;
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
  NB_CASES = NB_LIGNES * NB_COLONNES;
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
      NB_CASES = NB_LIGNES * NB_COLONNES;
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
  NB_CASES = NB_LIGNES * NB_COLONNES;
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
