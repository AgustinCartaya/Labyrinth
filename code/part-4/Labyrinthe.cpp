//============================================================================
// Name        : Labyrinthe.cpp
// Author      : Agustun Ujarky Cartaya Lathulerie
// Version     : 1.0
//============================================================================

#include <time.h>

class Labyrinthe
{
private:

  //affichage
  char* motif;
  int motifCount;

  //Grille
  char* Grille;//Grille contenant le labyrinthe
  int nbColonnes; //Longueur de la grille (nombre de colonnes)
  int nbLignes; //Largeur de la grille (nombre de lignes)
  int aleatoire = 2;//pour generer des labyrantes aleaoires

  //Pour gerer la conexite du labyrinthe
  int* Pile;//pile pour gerer la connexite du labyrinthe
  int Sommet;//sommet de la pile


  //Methodes Internes

  //pour initialiser les variabes
  void initialiserLesChoses();
  char* getInitialMotif();

  //Partie 2
  void push(int x);//ajourter un elemente dans la pile
  int pop();//enlever un element de la pile
  void viderPile();//vider toute la pile

  int count(int type);//compte le nombre de cases type dans la labyrinthe
  int idPremier(int type);//Renvoi l'id de la premiere case type dans le labyrinthe
  void marquer( int id);//fait une marque a la case id
  void markVoisin( int id );//marque le voisin de la case id

  //génération  du labyrinthe
  void genLabyInternal(int noirs, int alea);
  void voisinsPointer(int id, char **t);
  void voisinsID(int id, int t[]);
  void genPerm(int t[], int n);
  void nettoyer(int m);
  void voisinsConexID(int id, int t[], char a);

  //Partie 3
  int** lab2Graphe( int *caseLibreInitial, int *caseLibreFinal);
  int* dijkstra( int** tabNoeud, int n, int init, int fin);
  void desingChemin( int* cases,  char mark);


public:
    /// Constructeurs et destructeurs
    Labyrinthe(int nLignes, int nColonnes); //Crée un labyrinthe vide.
    Labyrinthe(char data[]) ; //Crée un labyrinthe à partir d'un descripteur.
    ~Labyrinthe(); //Détruit le labyrinthe courant.
    /// Méthodes d'accès
    int getID(int ligne, int colonne); //Retourne l'identifiant d'une cellule.
    int getLigne(int id); //Retourne la ligne de la cellule d'identifiant id.
    int getCol(int id); //Retourne la colonne de la cellule d'identifiant id.
    void modifie(int ligne, int colonne, char x); //Modifie la valeur d'une cellule.
    char lit(int ligne, int colonne); //Retourne la valeur de la cellule de coordonnées (i,j).
    /// Méthode d'accès supplémentaires
    int getNbLignes(); //Retourne le nombre de lignes de la grille.
    int getNbColonnes(); //Retourne le nombre de lignes de la grille.
    int getNbCases(); //Retourne le nombre de cases de la grille.
    int lit(int id); //Retourne la valeur de la cellule id.
    void modifie(int id, char x); //Modifie la valeur de la cellule id.
    /// Méthodes d'affichage
    // Initialise les caractères utilisés pour l'affichage.
    // Si motif contient n caractères alors
    // motif[i] est affiché pour représenter une case de valeur i entre 0 et n-2.
    // Toute autre valeur provoque l'affichage du caractère motif[n-1].
    void setAff(const char* motifs);
    void affiche(); // Affiche le labyrinthe.
    void afficheDescr(); //Affiche le descripteur du labyrinthe courant.
    /// Méthode de haut niveau
    bool connexe(); //Vérifie si toutes les cellules de valeur 0 sont connectées.
    void genLaby(int nb); //Produit un labyrinthe aléatoire connexe avec nb cases blanches.
    int distMin(int id1, int id2); //Retourne la distance minimum entre les cases id1 et id2.
    /// Méthodes de démonstration
    // Matérialise un chemin de longueur minimale entre les cases d'identifiants
    // id1 et id2 en plaçant des valeurs 2 dans les cases de ce chemin.
    void chemin(int id1, int id2);
    // Lance une démonstration de productions de labyrinthes et de recherche de
    // chemins de longueur minimale.

    //mes methodes
    int* getCheminMin(int caseInitial, int caseFinal);

    //tests
    static bool test_getID();
    static bool test_getLigne();
    static bool test_getColonne();
    static bool test_modifie();
    static bool test_lit();
    static bool test_connexe();
    static bool test_pop();
    static bool test_push();
    static bool test_distMin();

    //DefaultLaby
    static char* genDefaultLaby(int lig, int col, int connexe);
    static char* genDefaultLaby(int lig, int col, int connexe, int* _disMin);
    static void pile2tas( char * tas, char *pile, int n);

    //demo
    static void demo();

    //constants
    static const char CASE_LIBRE = 0;
    static const char CASE_OCCUPE = 1;
    static const char CASE_MARQUE = 2;
    static const char CASE_CHEMIN = 3;

};

//Constructeurs
Labyrinthe::Labyrinthe(int nLignes, int nColonnes)
{
  this->nbLignes = nLignes;
  this->nbColonnes = nColonnes;
  this->initialiserLesChoses();
}

Labyrinthe::Labyrinthe(char data[])
{
  this->nbLignes = data[0];
  this->nbColonnes = data[1];
  this->initialiserLesChoses();
  for (int i = 0; i < getNbCases(); i++)
  {
    Grille[i] = data[i+2];
  }
}

Labyrinthe::~Labyrinthe()
{
  delete[] this->Grille;
  delete[] this->Pile;
  delete[] this->motif;
}

void Labyrinthe::initialiserLesChoses()
{
  this->Grille = new char[this->getNbCases()];
  this->Pile = new int[this->getNbCases()];
  this->Sommet = 0;
  this->motifCount = 0;
  srand((unsigned) time(NULL));
  this->setAff(getInitialMotif());
}

char* Labyrinthe::getInitialMotif()
{
	char* motif = new char[4];
	motif[0] = '-';
	motif[1] = 'X';
	motif[2] = '*';
	motif[3] = '.';
	return motif;
}

void Labyrinthe::demo(){
	Labyrinthe lab(10,10);
	lab.genLaby(50);
	lab.chemin(0,-1);
	lab.affiche();
}

// ----------------------------------------------PARTIE 1 --------------------------------------------------------------

/**
* retourne l'identifiant d'une case dont on donne les coordonnées
**/
int Labyrinthe::getID(int ligne, int colonne)
{
  return ligne * this->getNbColonnes() + colonne ;
}

/**
* retourne la première coordonnée (ligne) d'une case dont on donne l'identifiant
**/
int Labyrinthe::getLigne(int id)
{
  return id/this->getNbColonnes() ;
}

/**
* retourne la deuxième coordonnée (colonne) d'une case dont on donne l'identifiant
**/
int Labyrinthe::getCol(int id)
{
  return id%this->getNbColonnes() ;
}

/**
* place la valeur x dans le case de coordonnée (i,j)
**/
void Labyrinthe::modifie(int ligne, int colonne, char x)
{
  this->Grille[this->getID(ligne, colonne)] = x;
}

/**
* retourne la valeur de la case de coordonnées (i,j)
**/
char Labyrinthe::lit(int ligne, int colonne)
{
  return this->Grille[this->getID(ligne, colonne)];
}

/*
 *
 */
int Labyrinthe::lit(int id)
{
  return this->Grille[id];
}


/**
* affiche la grille
*/
void Labyrinthe::affiche()
{
  for (int i = 0; i <= (this->getNbLignes()+1); i++)
  {
    for (int j = 0; j <= (this->getNbColonnes()+1); j++)
    {
      int valeur = lit(i-1, j-1);
      if((i*j) == 0 || i == (this->getNbLignes()+1) || j == (getNbColonnes()+1) || valeur >= motifCount)
    	  printf(" %c ", motif[this->motifCount-1] );
      else
    	  printf( "%c", motif[(int)lit(i-1, j-1)]);
    }
    printf("\n");
  }
}

int Labyrinthe::getNbLignes()
{
  return this->nbLignes;
}

int Labyrinthe::getNbColonnes()
{
  return this->nbColonnes;
}

int Labyrinthe::getNbCases()
{
  return this->getNbLignes() * this->getNbColonnes();
}

void Labyrinthe::setAff(const char* motifs)
{
  if(this->motifCount >0)
    delete[] this->motif;

  this->motifCount = sizeof(motifs)/sizeof(char);
  this->motif = new char[motifCount];
  for (int i = 0; i < motifCount; i++) {
    this->motif[i] = motifs[i];
  }
}


// ----------------------------------------------PARTIE 2 --------------------------------------------------------------

/*
* Inserer une case dans la Pile
*/
void Labyrinthe::push(int x)
{
  // printf("%d\n", this->Sommet);
  if(this->Sommet < (this->getNbCases()))
  {
	  this->Pile[this->Sommet] =  x;
	  this->Sommet++;
  }
  else
  {
    printf("stackOverflow\n");
  }
}

/*
* Remover une case de la Pile
*/
int Labyrinthe::pop()
{
  if(this->Sommet >0)
  {
	  this->Sommet--;
    return this->Pile[this->Sommet];
  }
  else
  {
    return -1;
  }
}

/*
* vider la Pile
*/
void Labyrinthe::viderPile()
{
  this->Sommet = 0;
}

//Connexité ------------------------------------------------

/*
*Vérifier si toutes les cases sont connexes
*/
bool Labyrinthe::connexe()
{
  bool conex = 0; //1 = labyrinthe connexe
  int cconex = 0; // nombre de cases marquees
  int ncb = 0; //nombre de cases 0s
  int id = this->idPremier(Labyrinthe::CASE_LIBRE); //id de la case courrant
  ncb = this->count(Labyrinthe::CASE_LIBRE);//nombre de cases libres
  this->marquer(id);//marque la premiere case libre
  this->push(id);//mettre la premiere case libre dans la pile

  while(this->Sommet!=0)
  {
    int id = this->pop();
    this->markVoisin(id);
  }
  for (int i = 0; i < this->getNbCases(); i++)
  {
    if( this->Grille[i] == Labyrinthe::CASE_MARQUE )
    {
      cconex++;
      this->Grille[i] = Labyrinthe::CASE_LIBRE;
    }
  }
  conex = cconex == ncb;
  return conex;
}

/**
*Renvoi l'id de la premiere case type dans le labyrinthe
*/
int Labyrinthe::idPremier(int type){
	int res = -1;
	for (int i = 0; i < this->getNbCases() && res == -1; i++)
		if(this->Grille[i] == type)
			res = i;
	return res;
}

/**
* Parcours du tableau pour compter les nombre de cases "Type"
*/
int Labyrinthe::count( int type )
{
	int nbCases = 0;
	for (int i = 0; i < this->getNbCases(); i++)
		if(this->Grille[i] == type)
		  nbCases++;
	return nbCases;
}

/*
* marque une case avec la marque par defaut
*/
void Labyrinthe::marquer( int id)
{
	this->Grille[id] = Labyrinthe::CASE_MARQUE;
}

/**
* renvoi un tableau  de 4 positions avec les id des voisins conexes de la case id et qui ont comme valeur 'a'
* si le voisin n'existe pas ou la case n'est pas égale a 'a' le valeur de la case sera égale a -1
*/
void Labyrinthe::voisinsConexID(int id, int t[], char a)
{
  int lig = this->getLigne(id);
  int col = this->getCol( id );
  for (int i = 0; i < 4; i++)
    t[i] = -1;

  if( col != 0 && Grille[id-1] == a )
    t[1] = id-1;

  if( col < (this->getNbColonnes()-1) && this->Grille[id + 1] == a)
    t[2] = id+1;

  if( lig != 0  && this->lit(lig-1, col) == a )
    t[0] = this->getID(lig-1, col );

  if( lig < (this->getNbLignes()-1) && this->lit(lig+1, col) == a )
    t[3] = this->getID(lig+1, col );

}

/*
* Marquer les voisins et les mettre das la Pile
*/
void Labyrinthe::markVoisin(int id)
{
  int t[4];
  this->voisinsConexID(id, t, Labyrinthe::CASE_LIBRE);

  for (int i = 0; i < 4; i++) {
    if(t[i] != -1){
    	this->push(t[i]);
    	this->marquer(t[i]);
    }
  }
}


//génération du labyrinthe ------------------------------------------------

/*
*  Génère un labyrinthe connexe avec une quantité de cases blanches k
*/
void Labyrinthe::genLaby(int k)
{
  printf("--------- Creation du labyrinthe %d x %d --------------\n", this->getNbLignes(),this->getNbColonnes());
  int c = 0;
  int kk = k;
  int maxEssais = this->getNbCases()*1000;

  if ( k < (this->getNbColonnes()+this->getNbLignes()) )// le cas ou les cases blanches ne sont pas sufissantes
    kk = (this->getNbColonnes()+this->getNbLignes()+2);
  else if(k > this->getNbCases())// le cas ou les cases blanches sont plus grands que le tableaux
      kk = this->getNbCases()/2;

  do
  {
    c++;
    //génération  si un labyrinthe avec (this->getNbCases()-kk) cases noirs
    this->genLabyInternal(this->getNbCases()-kk, aleatoire);
    //cas ou la génération  du labyrinthe n'est pas possible avec la quantité de cases blanches donne
    if(c > maxEssais && c%100 == 0)
      kk++; //augmentation de cases blanches
  }
  while(!connexe() );//jusqu’à trouver un labyrinthe connexe

  printf("labyrinthe cree apres %d essais\n",c);
  printf("  Cases  \tAttendu\t\tReel\n" );
  printf("Noires:\t\t%d\t\t%d\n", this->getNbCases() - k, count(1)  );
  printf("Blanches:\t%d\t\t%d\n", k, count(0)  );
  printf("Total:\t\t%d\t\t%d\n",this->getNbCases() ,count(0)+count(1) );
  printf("-------------------------------------------------------");
}

/*
* génération interne du labyrinthe avec un nombre de cases mur k et
* une paramètre aléatoire 0 aléatoire minimum 2 aléatoire max
*/
void Labyrinthe::genLabyInternal(int noirs, int alea)
{

  int minNoir = (int)(this->getNbColonnes()/2) * (int)(this->getNbLignes()/2); //min de cases noires
  int perm[ minNoir ]; //tableau de permutation de cases noir
  char *voisins[9]; //tableau de voisins
  int al = 0; //index aleatoire du voisin
  int ck = 0; //compteur de cases noires

  this->nettoyer(Labyrinthe::CASE_LIBRE);
  genPerm( perm,minNoir );

  //compteur cyclique des cases noires guides
  int c = 0;

  while( ck < noirs)
  {
    int i = c % minNoir ; //pour répéter les cases noires guides

    //permutation que permet de ne pas faire les case noir guides  par ordre mais aleaoires
    int id = 2*perm[i] + 1;
    id = (2*getLigne(id)+ (rand()%2)*1)*this->getNbColonnes() + id%this->getNbColonnes();

    //chercher les voisins aléatoires des cases noirs guides
    this->voisinsPointer( id, voisins);

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
        *voisins[4] != Labyrinthe::CASE_OCCUPE &&
        voisins[4]  != &Grille[0] &&
        voisins[4]  != &Grille[this->getNbCases()-1])
    {
      *voisins[4] = Labyrinthe::CASE_OCCUPE;
      ck++;
    }    //marque la case voisine si elle existe et si elle n'es pas marquée
    if( voisins[al]  != &Grille[0] &&
        voisins[al]  != &Grille[this->getNbCases()-1] &&
        voisins[al] != voisins[4] &&
        *voisins[al] != Labyrinthe::CASE_OCCUPE
      )
    {
      *voisins[al] = Labyrinthe::CASE_OCCUPE;
      ck++;
    }
    c++;
  }
}

/*
* Créer la permutation des n premières inters dans le tableau t
*/
void Labyrinthe::genPerm(int t[], int n)
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
void Labyrinthe::voisinsPointer(int id, char **t)
{
    int lig = this->getLigne(id);
    int col = this->getCol( id );
    for (int i = 0; i < 9; i++)
      t[i] = &(this->Grille[id]);

    if( col != 0   )
    {
      t[3] = &(this->Grille[id-1]);
      if( lig != 0  )
        t[0] = &(this->Grille[ getID(lig-1, col-1 )]);
      if( lig < (this->getNbLignes()-1)  )
        t[6] = &(this->Grille[ getID(lig+1, col-1 )]);
    }

    if( col < (this->getNbColonnes()-1) )
    {
        t[5] = &(Grille[id+1]);
        if( lig != 0 )
          t[2] = &(this->Grille[ this->getID(lig-1, col+1 )]);
        if( lig < (this->getNbLignes()-1)  )
          t[8] = &(this->Grille[ this->getID(lig+1, col+1 )]);
    }

    if( lig != 0 )
    {
      t[1] = &(this->Grille[ this->getID(lig-1, col )]);
    }

    if( lig < (this->getNbLignes()-1) )
    {
      t[7] = &(this->Grille[ this->getID(lig+1, col )]);
    }

}

/**
* renvoi un tableau  de 9 positions avec les voisins de la case id
* la case id est contenue dans le tableau dans la position 4
* si le voisin n'existe pas le case va pointer au même adresse du id
*/
void Labyrinthe::voisinsID(int id, int t[])
{
  int lig = this->getLigne(id);
  int col = this->getCol( id );
  for (int i = 0; i < 9; i++)
    t[i] = id;

  if( col != 0 )
  {
    t[3] = id-1;
    if( lig != 0 )
      t[0] = this->getID(lig-1, col-1 );
    if( lig < (this->getNbLignes()-1) )
      t[6] = this->getID(lig+1, col-1 );

  }

  if( col < (this->getNbColonnes()-1) )
  {
      t[5] = id+1;
      if( lig != 0 )
        t[2] = this->getID(lig-1, col+1 );
      if( lig < (this->getNbLignes()-1))
        t[8] = this->getID(lig+1, col+1 );
  }

  if( lig != 0 )
  {
    t[1] = this->getID(lig-1, col );
  }

  if( lig < (this->getNbLignes()-1)  )
  {
    t[7] = this->getID(lig+1, col );
  }

}

/**
* met toutes les cases de la grilla avec une valeur égale a m
*/
void Labyrinthe::nettoyer(int m)
{
  for (int i = 0; i < this->getNbCases(); i++)
  {
    this->Grille[i] = m;
  }
}


//----------------------------------------------- PARTIE 3 -----------------------------------------------


/**
 renvoi le labyrinthe en formme de graphe
 renvoi un tableau où chaque ligne indique une casee 0 les colonnes les connexions de cette case  avec les autres cases 0s voisines
*/
int** Labyrinthe::lab2Graphe( int *caseLibreInitial, int *caseLibreFinal)
{
  ///-------------------PAS 1 --------------------------------------
  //tableau utilisé pour compter le nombre de cases 0s
  //les cases 0s sont numérotés à partir de 0
  //les cases occupées ont une valeur de -1
  // Exemple:
  // position   0    1   2   3   4   5   6   7   8
  //
  // Grille =  [0,   1,  1,  0,  0,  1,  1,  0,  0]
  // id2LibreID[0,  -1, -1,  1,  2, -1, -1,  3,  4]

  int* id2LibreID = new int[this->getNbCases()];
  int casesLibres = 0; //nombre de cases 0s

  int truve_caseLibreInitial = -1;
  int truve_caseLibreFinal = -1;

  for (int i = 0; i < this->getNbCases(); i++) {
    if(this->Grille[i] == 0)
    {
      //converir id a numero de case blanche
      if(*caseLibreInitial == i){
        truve_caseLibreInitial = casesLibres;
      }
      if(*caseLibreFinal == i){
        truve_caseLibreFinal = casesLibres;
      }
      //fin converir id a numero de case blanche
      id2LibreID[i] = casesLibres;
      casesLibres = casesLibres + 1;
    }
    else
    {
      id2LibreID[i] = -1;
    }
  }
   //convertion de id a case 0
  *caseLibreInitial = truve_caseLibreInitial;
  *caseLibreFinal = truve_caseLibreFinal;
   if(*caseLibreInitial < 0 )
     *caseLibreInitial = 0;
   //convertion hors de rangue
   if(*caseLibreFinal < 0 )
     *caseLibreFinal = casesLibres-1;

  int cTab = 0;
///-------------------PAS 2 --------------------------------------
  //Cet array bidimensionnel contiendra un tableau où chaque colonne représentera une case blanche
  //et les lignes de cette colonne les cases voisines blanches connexes
  // la dernière colonne de la file représente l'id de la case 0 courante
   // Labyrinthe        nombre de cases         nombre de cases blanches
   //    0 1 1                 0 1 2                      0 -1 -1
   //    0 0 1                 3 4 5                      1  2 -1
   //    1 0 0                 6 7 8                     -1  3  4
   //
   //  table de Connexité
   //   up     <-    ->    down    id          Explication:
   //   -1    -1    -1     1      0            la case blanche #0 avec id = 0 a comme voisine(s) la case blanche #0
   //    0    -1     2    -1      3            la case blanche #1 avec id = 3 a comme voisine(s) la case blanche #0, #2
   //   -1     1    -1     3      4            la case blanche #2 avec id = 4 a comme voisine(s) la case blanche #1, #3
   //    2    -1     4    -1      7            la case blanche #3 avec id = 7 a comme voisine(s) la case blanche #2, #4
   //   -1     3    -1    -1      8            la case blanche #4 avec id = 8 a comme voisine(s) la case blanche #3


  int** tab = new int*[casesLibres];
  for (int i = 0; i < this->getNbCases() && cTab < casesLibres; i++)
  {
    if(this->Grille[i] == 0)
    {
      int tVoisinsConexID[4];
      this->voisinsConexID(i, tVoisinsConexID, 0 );
      tab[cTab] = new int[5];
      for (int j = 0; j < 4; j++)
      {
        if (tVoisinsConexID[j] != -1 )
          tab[cTab][j] = id2LibreID[tVoisinsConexID[j]];//les cases voisines sont ajoutées
        else
          tab[cTab][j] = -1;
      }
      tab[cTab][4] = i; //l'id de la case courante est ajouté dans la dernière colomne de la ligne
      cTab++;
    }
  }
  delete[] id2LibreID;
  return tab;
}

int* Labyrinthe::dijkstra( int** tabNoeud, int n, int init, int fin)
{
  ///---------------------------PAS 1 ------------------------------------
  //on crée le tableau dijkstra avec des 0 à l'intérieur
  int*** tab = new int**[n];
  for (int i = 0; i < n; i++) {
    tab[i] = new int*[n];
    for (int j = 0; j < n; j++) {
      tab[i][j] = new int[2];
      tab[i][j][0] = 0; //le nœud parent
      tab[i][j][1] = 0; //valeur cumulée minimale
    }
  }

  ///---------------------------PAS 2 ------------------------------------
  int pa = init; //position actual

  //donner des valeurs à la première colonne qui sont les fils du nœud init
  for (int i = 0; i < 4; i++){
    if( tabNoeud[init][i] != -1)
    {
      tab[0][ tabNoeud[init][i] ][0] = init; //on etabli comment pere le nœud init
      tab[0][ tabNoeud[init][i] ][1] = 1; //la valeur est 1 car on n'ai fait qu'un pas
      if( pa == init)
        pa = tabNoeud[init][i]; //choisir comme position actuelle le premier nœud connecté 0 enfant de init trouvé
    }
  }
  tab[n-1][ init ][1] = -1; //Fermer le nœud init dans la table car il ne sera plus traversé

  ///---------------------------PAS 3 ------------------------------------
  //traverser la table jusqu'à atteindre le dernier nœud 0 ou jusqu'à atteindre le nœud final
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
  ///---------------------------PAS 4 ------------------------------------
  //Parcours a l'inverse du tableau dijkstra pour obtenir les resultats
  //array qui contiendra le plus court chemin
  int* chemin =new int[n];

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

  //on enleve les espaces vides
  int* _chemin = new int[cChemin+1];
  for (int i = 0; i <= cChemin; i++)
    _chemin[i] = chemin[i];
  delete[] chemin;

  //efacer la memoire
  //effacer le tableau dijkstra

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
      delete[] tab[i][j];
    delete[] tab[i];
  }
  delete[] tab;

  return _chemin;
}

/*
Renvoi un array avecl les id des cases qui sont le chemin de cout minimum de en partant de la caseInitial jusqu'a la  caseFinal
 caseInitial: id de la case initial à partir de laquel le Parcours va commencer
 caseLibreFinal: id de la case final
*/
int* Labyrinthe::getCheminMin(int caseInitial, int caseFinal)
{
  int nbCasesLibres = this->count(Labyrinthe::CASE_LIBRE);
  int caseLibreInitial = caseInitial;
  int caseLibreFinal = caseFinal;
  int** tab = this->lab2Graphe(&caseLibreInitial, &caseLibreFinal);
  int* chemin = this->dijkstra( tab, nbCasesLibres,caseLibreInitial, caseLibreFinal);

  for (int i = 0; i < nbCasesLibres; i++)
    delete[] tab[i];
  delete[] tab;

  return chemin;
}

/*
 Renvoi un le cout d'alle de la caseInitial jusqu'a la  caseFinal
 caseInitial: id de la case initial à partir de laquel le Parcours va commencer
 caseLibreFinal: id de la case final
*/
int Labyrinthe::distMin(int caseInitial, int caseFinal)
{
  int* _data = this->getCheminMin(caseInitial, caseFinal);
  int res = _data[0]-1;
  delete[] _data;
  return res;
}

void Labyrinthe::desingChemin( int* cases,  char mark)
{
  for (int i = 1; i <= cases[0]; i++)
  {
    Grille[cases[i]] = mark;
  }
}

void Labyrinthe::chemin(int caseInitial, int caseFinal)
{
	int* chemin = this->getCheminMin(caseInitial, caseFinal);
	this->desingChemin(chemin, Labyrinthe::CASE_CHEMIN );
	delete[] chemin;
}

//--------------- Foctions de test ---------------------------------------
//--------------- test partie 1
/**
* Test la fonction getID avec 100 marices de different taille
* 1: fonction fonctione sans probleme
* 0: fonction avec des problemes
*/
bool Labyrinthe::test_getID()
{
  bool error = 0;

  //faire l'essai avec plusieurs labyrentes de differents taille
  for (int nbl = 1; nbl <= 10 && !error; nbl++)
  {
    for (int nbc = 1; nbc <= 10 && !error; nbc++)
    {
      Labyrinthe _lab(nbl, nbc);
      int id = 0;
      for(int i =0; i< nbl && !error; i++)
      {
        for(int j =0; j< nbc && !error; j++)
        {
          error = id != _lab.getID(i, j);
          id++;
        }
      }
    }
  }
  return !error;
}

/**
* Test la fonction getLigne
* 1: fonction fonctione sans probleme
* 0: fonction avec des problemes
*/
bool Labyrinthe::test_getLigne(){
  bool error = 0;

  //test avec differentes matrices de taille m,n {m,n E [1,10]}
  for (int nbl = 1; nbl <= 10 && !error; nbl++)
  {
    for (int nbc = 1; nbc <= 10 && !error; nbc++)
    {
	  Labyrinthe _lab(nbl, nbc);
      int c_ligne = -1;
      for( int id = 0; id< _lab.getNbCases() && !error; id++ ){
        if(id % _lab.getNbColonnes() == 0)
          c_ligne++;
        error = c_ligne != _lab.getLigne(id);
      }
    }
  }
  return !error;
}

/**
* Test la fonction getColonne
* 1: fonction fonctione sans probleme
* 0: fonction avec des problemes
*/
bool Labyrinthe::test_getColonne(){
  bool error = 0;
  //test avec differentes matrices de taille m,n {m,n E [1,10]}
  for (int nbl = 1; nbl <= 10 && !error; nbl++)
  {
    for (int nbc = 1; nbc <= 10 && !error; nbc++)
    {
      Labyrinthe _lab(nbl, nbc);
      int c_colonne = 0;
      for( int id = 0; id< _lab.getNbCases() && !error; id++ ){

        if(id % _lab.getNbColonnes() == 0)
          c_colonne = 0;
        else
          c_colonne++;

        error = c_colonne != _lab.getCol(id);
      }
    }
  }
  return !error;
}

/**
* Test la fonction modifie
* 1: fonction fonctione sans probleme
* 0: fonction avec des problemes
*/
bool Labyrinthe::test_modifie()
{
  bool error = !Labyrinthe::test_lit();
  //test avec differentes matrices de taille m,n {m,n E [1,10]}
  for (int nbl = 1; nbl <= 10 && !error; nbl++)
  {;
    for (int nbc = 1; nbc <= 10 && !error; nbc++)
    {
      Labyrinthe _lab(nbl, nbc);
      int c_total = 0;
      for (int i = 0; i < _lab.getNbLignes() && !error; i++) {

        for (int j = 0; j < _lab.getNbColonnes() && !error; j++) {
          int rdm = rand()%128;
          _lab.modifie(i,j, rdm);
          error = rdm != _lab.lit(i,j);
          c_total += rdm;
        }
      }
    }
  }
  return !error;
}

/**
* Test la fonction lit
* 1: fonction fonctione sans probleme
* 0: fonction avec des problemes
*/
bool Labyrinthe::test_lit()
{
  bool error = !Labyrinthe::test_getID();//pour verifier lit(ligne, colonne)
  char* _data;
  for (int nbl = 3; nbl <= 6 && !error; nbl++) {
    for (int nbc = 3; nbc <= 6 && !error; nbc++) {
    	_data = Labyrinthe::genDefaultLaby(nbl, nbc, 1);
    	Labyrinthe _lab(_data);
  		for (int id = 0; id < _lab.getNbCases() && !error; id++)
  		{
  			error =  _data[id+2] != _lab.lit(id);
  		}
      delete[] _data;
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
bool Labyrinthe::test_push(){

  bool error = 0;
  Labyrinthe _lab(5,5);
  for (int i = 0; i < _lab.getNbCases() + 5 && !error; i++){
	   _lab.push(i);
    if(i<_lab.getNbCases())
    {
      error = _lab.Pile[_lab.Sommet -1] != i;
    }
    else
    {
      error = _lab.Pile[_lab.Sommet-1] != _lab.getNbCases() - 1;
    }

  }
  error = _lab.Sommet != _lab.getNbCases() ;

  return !error;
}

/**
* Test la fonction pop
* 1: fonction fonctione sans probleme
* 0: fonction avec des problemes
*/
bool Labyrinthe::test_pop(){
  bool error = 0;
  //on cree une nouvelle pile et on la remplie avec des chiffres
  Labyrinthe _lab(5,5);
  for (int i = _lab.getNbCases() -1; i >= 0; i--)
	  _lab.push(i);

  _lab.Sommet = _lab.getNbCases();
  //on parcour la pille 5 unites en plus de sa taille et a chaque tour du boucle on enleve un elemente
  for (int i = 0; i < _lab.getNbCases()  && !error; i++)
  {
    if(i>=0)
    {
      error = i != _lab.pop() ;
    }
    else
    {
      error = -1 != _lab.pop();
    }
  }
  // printf("%s%d\n", "Error", error);
  error = _lab.Sommet != 0;

  return !error;
}

/**
* Test la fonction connexe
* 1: fonction fonctione sans probleme
* 0: fonction avec des problemes
*/
bool Labyrinthe::test_connexe(){
  bool error = 0;
  char* _data;
  for (int cnx = 0; cnx < 2 && !error; cnx++) {
    for (int i = 3; i <= 6 && !error; i++) {
      for (int j = 3; j <= 6 && !error; j++) {
        _data = Labyrinthe::genDefaultLaby(i,j,cnx);
        Labyrinthe _lab(_data);
        error = cnx != _lab.connexe();
        delete[] _data;
      }
    }
  }
  return !error;
}


/**
* Test la fonction distMin
* 1: fonction fonctione sans probleme
* 0: fonction avec des problemes
*/
bool Labyrinthe::test_distMin(){
  bool error = 0;
  char* _data;
  for (int i = 3; i <= 6 && !error; i++) {
    for (int j = 3; j <= 6 && !error; j++) {
      int _disMin = 0;
      _data = Labyrinthe::genDefaultLaby(i,j,1, &_disMin);
      Labyrinthe _lab(_data);
      error = (_disMin != _lab.distMin(-1, -1));
      delete[] _data;
    }
  }
  return !error;
}

//---------------------------labyrinthes par dafaut
/**
* génération de labyrinthes de taille lig x col par dafaut pour faire des test
connexe = 1 a fin de creer un labyrinthe connexe
connexe = 0 a fin de creer un labyrinthe non connexe

*/

char* Labyrinthe::genDefaultLaby(int lig, int col, int connexe){
  int a = 0;
  return Labyrinthe::genDefaultLaby(lig, col, connexe, &a);
}


 char* Labyrinthe::genDefaultLaby(int lig, int col, int connexe, int* _disMin)
 {
   char *defLaby = new char[lig*col+2];
   //---------------------------------------- Connexes
   if(connexe){
     if( lig == 3 ){
       if(col == 3)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,1,1,
                     0,0,1,
                     1,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
         *_disMin = 4;
       }
       else if(col == 4)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,1,1,1,
                     0,0,1,0,
                     1,0,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
         *_disMin = 5;
       }
       else if(col == 5)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,1,1,1,0,
                     0,0,0,0,0,
                     1,1,1,1,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
         *_disMin = 6;
       }
       else if(col == 6)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,1,1,1,0,1,
                     0,0,0,0,0,1,
                     1,1,1,1,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
         *_disMin = 7;
       }
     }
     else if( lig == 4){
       if(col == 3)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,
                     0,1,1,
                     0,0,0,
                     1,1,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
         *_disMin = 5;
       }
       else if(col == 4)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,0,
                     0,1,0,1,
                     0,1,0,0,
                     1,1,1,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
         *_disMin = 6;
       }
       else if(col == 5)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,0,1,
                     0,1,1,0,1,
                     1,0,0,0,1,
                     1,1,1,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
         *_disMin = 7;
       }
       else if(col == 6)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,0,1,0,
                     0,1,1,0,1,0,
                     1,0,0,0,1,0,
                     1,1,1,0,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
         *_disMin = 8;
       }
       else if(col == 7 )
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                   0,0,0,0,0,0,0,
                   0,0,0,0,1,0,0,
                   0,0,0,1,0,1,0,
                   0,0,1,0,0,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
         *_disMin = 10;
       }

     }
     else if( lig == 5){
       if(col == 3)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,
                     0,1,1,
                     0,0,0,
                     1,1,0,
                     1,1,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
         *_disMin = 6;
       }
       else if(col == 4)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,0,
                     0,1,1,1,
                     0,0,0,0,
                     1,1,0,1,
                     1,1,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
         *_disMin = 7;
       }
       else if(col == 5)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,0,1,
                     0,1,1,1,1,
                     0,0,0,0,1,
                     1,1,0,1,0,
                     1,1,0,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
         *_disMin = 8;
       }
       else if(col == 6)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,0,0,0,
                     1,1,0,1,1,0,
                     0,0,0,0,1,0,
                     1,1,0,1,1,0,
                     1,1,0,0,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
         *_disMin = 9;
       }

     }
     else if( lig == 6){
       if(col == 3)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,
                     0,1,1,
                     0,0,0,
                     1,1,0,
                     1,1,0,
                     0,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
         *_disMin = 7;
       }
       else if(col == 4)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,1,
                     0,1,1,0,
                     0,0,0,0,
                     1,1,0,1,
                     1,1,0,1,
                     0,0,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
         *_disMin = 8;
       }
       else if(col == 5)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,1,1,
                     0,1,1,0,1,
                     0,0,0,0,0,
                     1,1,0,1,0,
                     1,1,0,1,0,
                     0,0,0,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
         *_disMin = 9;
       }
       else if(col == 6)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,0,0,0,
                     1,0,1,0,0,1,
                     1,0,1,1,0,1,
                     0,0,1,0,0,1,
                     0,1,0,0,1,0,
                     0,0,0,0,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
         *_disMin = 12;

       }
       else if(col == 7)
         {
           char tab_deflab[] =  {(char)lig, (char)col,
                       0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0};
           Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
           *_disMin = 12;

         }
     }
   }
   //---------------------------------------- Non connexes
   else{
     if( lig == 3 ){
       if(col == 3)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,1,0,
                     0,1,1,
                     1,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));

       }
       else if(col == 4)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,1,1,1,
                     0,0,1,0,
                     1,0,1,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));

       }
       else if(col == 5)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,1,1,1,0,
                     1,0,0,0,0,
                     1,1,1,1,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
       }
       else if(col == 6)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,1,1,1,0,1,
                     0,0,0,0,1,1,
                     1,1,1,1,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
       }

     }
     else if( lig == 4){
       if(col == 3)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,
                     0,1,1,
                     0,1,0,
                     1,1,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
       }
       else if(col == 4)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,0,
                     0,1,1,0,
                     0,0,0,1,
                     1,1,1,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));

       }
       else if(col == 5)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,0,1,
                     0,1,1,0,1,
                     1,0,0,0,1,
                     1,1,1,1,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
       }
       else if(col == 6)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,0,1,0,
                     0,1,1,0,1,0,
                     1,0,0,0,1,0,
                     1,1,1,0,1,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
       }
       else if(col == 7 )
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                   0,0,0,0,0,0,0,
                   0,0,0,0,1,0,0,
                   0,0,0,1,0,1,0,
                   0,0,1,0,0,1,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
       }

     }
     else if( lig == 5){
       if(col == 3)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,
                     0,1,1,
                     1,0,0,
                     1,1,0,
                     1,1,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
       }
       else if(col == 4)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,0,
                     0,1,1,1,
                     0,1,0,0,
                     1,1,0,1,
                     1,1,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
       }
       else if(col == 5)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,0,1,
                     0,1,1,1,1,
                     0,0,0,0,1,
                     1,0,0,1,0,
                     0,1,0,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
       }
       else if(col == 6)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,0,0,0,
                     1,1,1,1,1,0,
                     0,0,0,0,1,0,
                     1,1,0,1,1,0,
                     1,1,0,0,1,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
       }
     }
     else if( lig == 6){
       if(col == 3)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,
                     0,1,1,
                     0,0,0,
                     0,0,0,
                     0,1,1,
                     1,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
       }
       else if(col == 4)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,1,
                     0,1,1,0,
                     0,0,1,0,
                     1,1,0,1,
                     1,1,0,1,
                     0,0,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
       }
       else if(col == 5)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,1,1,
                     0,1,1,0,1,
                     0,0,0,0,0,
                     1,1,0,1,0,
                     1,1,0,1,0,
                     0,0,1,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
       }
       else if(col == 6)
       {
         char tab_deflab[] =  {(char)lig, (char)col,
                     0,0,0,0,0,0,
                     1,0,1,0,1,1,
                     1,0,1,1,0,1,
                     0,0,1,0,0,1,
                     0,1,0,0,1,0,
                     1,0,0,0,0,0};
         Labyrinthe::pile2tas( defLaby, tab_deflab, ((lig*col) + 2));
       }
     }
   }
   return defLaby;
 }

 void Labyrinthe::pile2tas( char * tas, char *pile, int n){
   for (int i = 0; i < n; i++) {
     tas[i] = pile[i];
   }
 }
