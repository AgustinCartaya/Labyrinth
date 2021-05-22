//auteur: CARTAYA Agustin
// Partie 3 du projet : poursuite de robots

#include <iostream>
#include <algorithm>
#include <vector>
#include "Labyrinthe.h"

using namespace std;

// Attend que duree (en secondes) se soit écoulée depuis le dernier appel.
void synchro(double duree)
{
    static clock_t jalon = clock();
    jalon = jalon + (clock_t)(duree * CLOCKS_PER_SEC);;
    while(clock()<jalon);
}

///============================================================================
/// Vous devez compléter l'implémentation de cette classe qui hérite de
/// La classe Labyrinthe. Toutes les méthodes publiques de Labyrinthe sont
/// disponibles. Vous pouvez ajouter des attributs et des méthodes.
///============================================================================

class Laby : public Labyrinthe
{
private:
    // Positions des robots
    int idRobotA; //PREDATEUR
    int idRobotB; //PROIE

    // ------- Ajoutez ici des attributs supplémentaires si applicable --------

    // ------------------------------------------------------------------------
public:
    // Produit un labyrinthe avec une densité de cases blanches égale à density.
    // Les positions initiale des deux robots sont situées dans les coins supérieur gauche
    // et inférieur droit. Les codes des robots ne sont pas placés dans le grilles, dont
    // toutes les cases vides sont initialisées à 0.
    Laby(int nLignes, int nColonnes, double density);

    // Construit un labyrinthe d'après un descripteur.
    Laby(char* descr);

    int getIdRobotA() {return idRobotA;}
    int getIdRobotB() {return idRobotB;}
    void setIdRobotA(int id) {idRobotA = id;}
    void setIdRobotB(int id) {idRobotB = id;}

    // Produit un identifiant aléatoire de case vide (PAS un mur)/
    // utilise pour plaser aleatoirement les robots A et B dans des casses vides
    int caseRandom();

    // Réalise un mouvemement du robot prédateur. La grille ne doit contenir que des cases
    // vides et des murs.
    // L'attribut idRobotA est modifié. La grille n'est pas modifiée.
    // Retourne false si les robots sont en contact (dans ce cas A n'est pas déplacé),
    // et true si les robots n'étaient pas en contact avant le déplacement de A.
    // Au moins un algorithme de déplacement doit être implémenté. Les algorithmes
    // implémentés doivent être numéroté à partir de 1. Par exemple, si deux algorithmes sont
    // implémentés, ils sont numérotés 1 et 2. Le paramètre algo désigne l'algorithme à
    // utiliser. Si un seul algorithme est utilisé, sa valuer doit être 1.
    bool deplaceRobotA(int algo);

    // Réalise un mouvemement du robot proie. Mêmes convension que deplaceRobotB.
    bool deplaceRobotB(int algo);

    // Place les robots (identifiés par les valeurs 2 et 3) dans la grille. Efface l'écran.
    // Affiche la grille. Enlève les robots de lagrille (pour ne pas perturber distMin).
    void visualisation();

    // Lance une poursuite dans le labyrinthe courant. Si silent == true, aucun affichage n'est
    // réalisé, sinon il y a affichage de chaque étape.
    // Une étape = un mouvement de chaque robot. Si le nombre d'étape atteint timeout, la
    // valeur timeout+1 est retournée, sinon le nombre d'étape avant contact des robots est
    // retourné. Si random == true, les position initiales des robots sont aléatoires, sinon ce
    // sont les coins supérieur gauche et inféieur droit.
    // Les paramètres algoA et algoB indiquent le algorithmes utilisé pour les robots.
    int course(int timeout, bool silent, bool random, int algoA, int algoB);

    // Réalise nCourses courses et retourne la médiane des valeurs retournées par les
    // appels à la méthode courses. Le apramètres algoA et algoB donnent les algorithmes
    // à utiliser pour les robots A et B.
    int evalue(int nCourses, int timeOut, int algoA, int algoB);

    //rempli l'array 't' (de 4 max) positions qui representent les id cases conexes libres  de la casse courrente (id)
    //example pour un labyrinthe 3 x 3
    //  Labyrinthe    id de cases
    //  1 0 0         0 1 2
    //  1 0 0         3 4 5
    //  1 1 0         6 7 8
    // la liste returnee pour la case id = 4; sera [1, 5]
    // car la on cammnece a compter de haut en bas et de gauche a droit
    int* voisinsConnexesID(int id);


    //Renvoi un array avecl les id des cases qui sont le chemin de cout minimum de en partant de la caseInitial jusqu'a la  caseFinal
    //caseInitial: id de la case initial à partir de laquel le Parcours va commencer
    //caseLibreFinal: id de la case final
    int* cheminMin(int caseInitial, int caseFinal);

    //implemente l'algorithme me dijkstra pour trouver le chemins plus court dans un graphe represente par une liste d'adjacence
    int* dijkstra( int** tabNoeud, int n, int init, int fin);

    //renvoi le labyrinthe en formme de graphe
    //renvoi un tableau où chaque ligne indique une casee libre les colonnes les connexions de cette case  avec les autres cases libres voisines
    int** lab2Graphe( int *casesLibres, int *caseLibreInitial, int *caseLibreFinal  );

};

// Constructeur fourni par O. Bailleux. Peut être complété si nécessaire.
Laby::Laby(int nLignes, int nColonnes, double density) : Labyrinthe(nLignes, nColonnes)
{
    setAff(" XAB.");
    idRobotA = getID(0,0);
    idRobotB = getID(getNbLignes()-1, getNbColonnes()-1);
    genLaby((int)(density * getNbColonnes() * getNbLignes()));
    // Ajouter les initialisations supplémentaire nécessaires, si applicable
}

// Constructeur fourni gracieusement par O. Bailleux. Peut être complété si nécessaire.
Laby::Laby(char* descr) : Labyrinthe(descr)
{
    setAff(" XAB.");
    idRobotA = getID(0,0);
    idRobotB = getID(getNbLignes()-1, getNbColonnes()-1);
    // Ajouter les initialisations supplémentaire nécessaires, si applicable
}

// Méthode fournie par O. Bailleux
void Laby::visualisation()
{
    // Les robots sont placés dans la grille avant l'affichage
    modifie(getIdRobotA(),2);
    modifie(getIdRobotB(),3);

    // system("clear");
    affiche();

    // Les robots sont retirés de la grille pour ne pas perturber la recherche de chemins
    // optimaux par la méthode distMin.
    modifie(getIdRobotA(),0);
    modifie(getIdRobotB(),0);
}

// Méthode fournie par O. Bailleux
int Laby::course(int timeout, bool silent, bool random, int algoA, int algoB)
{
    if(random)
    {// Course avec positions initiales des robots aléatoire
        idRobotB = caseRandom();
        idRobotA = caseRandom();
    }
    else
    {// Course avec départ dans les coins opposés.
        idRobotA = getID(0,0);
        idRobotB = getID(getNbLignes()-1, getNbColonnes()-1);
    }

    int nEtapes = 0;
    bool contact = false;

    // Arret quand les robots se touchent ou de nombre maxi d'étapes atteint
    while(!contact && nEtapes<timeout)
    {
        // On déplace le robot A
        contact = !deplaceRobotA(algoA);

        // On déplace le robot B sauf s'il est en contact avec A
        if (!contact) contact = !deplaceRobotB(algoB);

        nEtapes++;
        if(!silent)
        {// Délai et affichage seulement si on est pas en mode silencieux
            synchro(0.1);
            visualisation();
        }
    }

    if(contact) return nEtapes;
    else return timeout+1;
}

// Méthode fournie par O. Bailleux
int Laby::evalue(int nCourses, int timeout, int algoA, int algoB)
{
    std::vector<int> tab;
    //int* tab = new int[nCourses]; // Tableau des scores

    for(int i=0; i<nCourses; i++)
    {// On lance une course et récupère le score
        cout << i << " / " << nCourses;
        int score = course(timeout, true, true, algoA, algoB);
        tab.push_back(score);
        cout << " --> " << score << endl;
    }

    // Tri du tableau de score pour calcul de la médiane.
    sort(tab.begin(), tab.end());
    return tab[nCourses/2];
}

///============================================================================
/// Méthodes à compléter. Vous pouvez ajouter les méthodes supplémentaires
/// pour simplifier votre programme et le rendre plus lisible.
///============================================================================

int Laby::caseRandom()
{
    int casse = ( rand()% (getNbLignes() * getNbColonnes()) ) ;
    // random();
    while (lit( casse ) ) {
      casse = ( rand()% (getNbLignes() * getNbColonnes()) );
    }
    return  casse;
}

// Méthode à compléter

bool Laby::deplaceRobotA(int algo)
{
    if(idRobotA != idRobotB)
    {
      //algorithme de deplacement aleatoire (pour avoir une reference du pire resultat)
      if(algo == 0)
      {
        //Voir ou le robot A peut avancer
        int* t = voisinsConnexesID(idRobotA);
        int dispo = 0;

        //obtenir les cases posibles a visiter
        for ( dispo = 0; dispo <4 && t[dispo] >= 0; dispo++);

        //choisir une de deux distances
        idRobotA = t[rand() % dispo];
        delete[] t;
      }
      //fait deplacer le depredateur en minimizant la distance avec la proi avec plusieurs choix
      else if( algo == 1)
      {
        //calculer la distance actuelle de robotA a robotB
        int distance = distMin(idRobotA, idRobotB);

        //Voir ou le robot A peut avancer
        int* t = voisinsConnexesID(idRobotA);
        int dispo = 0;

        //chercher si on peut reduire la distance
        int disAme[3];

        //si cette casse voisine repproche le robotA du robotB
        for (int i = 0; i < 4 && t[i] >= 0; i++)
          if( distMin(t[i], idRobotB) < distance)
            disAme[dispo++] = t[i];

        //choisir une de deux distances
        idRobotA = disAme[rand() % dispo];
        delete[] t;
      }
      //fait deplacer le depredateur en minimizant la distance avec la proi avec un seul choi
      else if(algo == 2)
      {
        if( getNbLignes() * getNbColonnes() > (30 * 35) ){
          cout<< "algorithme impossible pour labyrantes de cette taille choisisez l'algorithme 1" << endl;
          exit(0);
        }
        //On calcule la chemin plus court depuis le robot A au robot B
        int * chm = cheminMin( idRobotB, idRobotA );

        //choisir une de deux distances
        idRobotA = chm[2];

        delete[] chm;
      }

      return true;
    }

    return false;

}

bool Laby::deplaceRobotB(int algo)
{
  if(idRobotA != idRobotB)
  {


    //Voir ou le robot A peut avancer
    int* t = voisinsConnexesID(idRobotB);
    int dispo = 0;

    //algorithme de deplacement aleatoire
    //ne prend en compte ne cherche a maximecer la distance, seulemente fait deplacer la proie
    if(algo == 1)
    {

        //obtenir les cases posibles a visiter
        for ( dispo = 0; dispo <4 && t[dispo] >= 0; dispo++);

        //choisir une de deux distances
        idRobotB = t[rand() % dispo];

    }

    //fait deplacer la proie en maximixant la distance avec le depredateur
    else if( algo == 2)
    {
      //calculer la distance actuelle de robotA a robotB
      int distance = distMin(idRobotA, idRobotB);

      //chercher si on peut aumenter la distance
      int disAme[3];

      //si cette casse voisine repproche le robotA du robotB
      for (int i = 0; i < 4 && t[i] >= 0; i++){
        if( distMin(t[i], idRobotA) >= distance)
          disAme[dispo++] = t[i];

      }

      if( dispo > 0)
      idRobotB = disAme[rand() % dispo];
      else
      idRobotB = t[0];

    }

    delete[] t;
    return true;
  }
  return false;
    // A COMPLETER AVEC AU MOINS UN ALGORITHME DE POURSUITE PROIE

}

int* Laby::voisinsConnexesID(int id)
{
  int *t = new int[4];
  int cont = 0;
  int lig = getLigne(id);
  int col = getCol( id );

  for (int i = 0; i < 4; i++)
    t[i] = -1;

  if( lig != 0  && (int)lit(lig-1, col) == 0 )
    t[cont++] = getID(lig-1, col );

  if( col != 0 && lit(id-1) == 0 )
    t[cont++] = id-1;

  if( col < (getNbColonnes()-1) && lit(id+1) == 0)
    t[cont++] = id+1;

  if( lig < (getNbLignes()-1) && (int)lit(lig+1, col) == 0 )
    t[cont] = getID(lig+1, col );

    return t;

}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



int** Laby::lab2Graphe( int *casesLibres, int *caseLibreInitial, int *caseLibreFinal  )
{

  //tableau utilisé pour compter le nombre de cases libres
  //les cases libres sont numérotés à partir de 0
  //les cases occupées ont une valeur de -1
  /*Exemple:
  position   0    1   2   3   4   5   6   7   8

  Grille =  [0,   1,  1,  0,  0,  1,  1,  0,  0]
  id2libreID[0,  -1, -1,  1,  2, -1, -1,  3,  4]

  */
  int id2libreID[getNbLignes() * getNbColonnes()];
  *casesLibres = 0; //nombre de cases libres

  int truve_caseLibreInitial = -1;
  int truve_caseLibreFinal = -1;

  for (int i = 0; i < getNbLignes() * getNbColonnes(); i++) {
    if( lit(i) == 0)
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

  for (int i = 0; i < getNbLignes() * getNbColonnes() && cTab < *casesLibres; i++)
  {
    if(lit(i) == 0)
    {

      int* tVoisinsConexID = voisinsConnexesID(i);
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

int* Laby::dijkstra( int** tabNoeud, int n, int init, int fin)
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
int* Laby::cheminMin(int caseInitial, int caseFinal)
{
  int casesLibres;
  int caseLibreInitial = caseInitial;
  int caseLibreFinal = caseFinal;
  int** tab = lab2Graphe( &casesLibres, &caseLibreInitial, &caseLibreFinal );

  int *chemin = dijkstra( tab, casesLibres,caseLibreInitial, caseLibreFinal);
  return chemin;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


///============================================================================
/// Quelques descripteurs qui seront utilisés pour évaluer les algos de poursuite
///============================================================================

// Gille de 20 lignes et 30 colonnes densité de cases vides 0.8
char descripteur1[] =
{ 20, 30,
0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0,
0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0,
0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0,
0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0,
0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1,
0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0,
1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
};

// Gille de 20 lignes et 30 colonnes sans murs
char descripteur2[] =
{ 20, 30,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// Gille de 20 lignes et 30 colonnes densité de cases vides 0.9
char descripteur3[] =
{ 20, 30,
0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// Gille de 20 lignes et 30 colonnes densité de cases vides 0.7
char descripteur4[] =
{ 20, 30,
0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0,
0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0,
0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1,
0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0,
1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0,
0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0,
1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1,
1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1,
1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1,
0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1,
0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0,
0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0,
0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0,
0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0,
1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0,
0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
};

char descripteur5[] =
{ 4, 4,
  0,0,0,0,
  0,1,1,0,
  0,0,0,0,
  1,1,1,0
};

///============================================================================
/// Exemple d'utilisation.
///============================================================================

void testPoursuite()
{
    int dureeMax = 300;
    Laby laby(30, 35, 0.9);

    int duree = laby.course(dureeMax, false, true, 1, 2);

    if(duree > dureeMax)
    {
        cout << "Echec apres " << dureeMax << " etapes" << endl;
    }
    else
    {
        cout << "Succes apres " << duree << " etapes" << endl;
    }
}

void testEval()
{
    Laby laby(descripteur2);
    // Laby laby(30, 30, 0.9);

    int score = laby.evalue(5, 500, 1, 2);

    cout << "Mediane : " << score << endl;
}

typedef struct
{
  int eficacity;
  double time;
}test;



void testAlgos()
{

  Laby lab(30, 30, 0.8);
  int tst = 10;
  int essais = 500;

  test tests[3][2];
  unsigned t0, t1;

  t0 = clock();
  tests[0][0].eficacity = lab.evalue(tst, essais, 0,1);
  t1 = clock();
  tests[0][0].time = (double(t1-t0)/CLOCKS_PER_SEC);

  t0 = clock();
  tests[0][1].eficacity = lab.evalue(tst, essais, 0,2);
  t1 = clock();
  tests[0][1].time = (double(t1-t0)/CLOCKS_PER_SEC);


  t0 = clock();
  tests[1][0].eficacity = lab.evalue(tst, essais, 1,1);
  t1 = clock();
  tests[1][0].time = (double(t1-t0)/CLOCKS_PER_SEC);

  t0 = clock();
  tests[1][1].eficacity = lab.evalue(tst, essais, 1,2);
  t1 = clock();
  tests[1][1].time = (double(t1-t0)/CLOCKS_PER_SEC);


  t0 = clock();
  tests[2][0].eficacity = lab.evalue(tst, essais, 2,1);
  t1 = clock();
  tests[2][0].time = (double(t1-t0)/CLOCKS_PER_SEC);

  t0 = clock();
  tests[2][1].eficacity = lab.evalue(tst, essais, 2,2);
  t1 = clock();
  tests[2][1].time = (double(t1-t0)/CLOCKS_PER_SEC);


  printf("|------------------- Results -------------------|\n" );
  printf("|\talgorithme\tEficacity\tTime\t|\n" );
  printf("|\tA, B\t\t\t\t\t|\n" );
  printf("|-----------------------------------------------|\n" );

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      if( tests[i][j].eficacity > essais)
        printf("|\t%d, %d\t\tECHEC\t\t%0.5f\t|\n", (i), (j+1), tests[i][j].time);
      else
        printf("|\t%d, %d\t\t%d\t\t%0.3f\t|\n", (i), (j+1), tests[i][j].eficacity, (tests[i][j].time));

    }
  }
  printf("|_______________________________________________|\n" );

}




// int main(int argc, const char * argv[])
// {
//     srand((unsigned)time(NULL));
//
//     testAlgos();
//     // testEval();
//     // testPoursuite();
//
//     return 0;
// }
