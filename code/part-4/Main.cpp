
#include <iostream>
#include <stdio.h>
#include "Labyrinthe.cpp"


using namespace std;
int main() {
	// Labyrinthe::demo();
	cout<<"test_getID\t"<<Labyrinthe::test_getID()<<endl;
	cout<<"test_getLigne\t"<<Labyrinthe::test_getLigne()<<endl;
	cout<<"test_getColonne\t"<<Labyrinthe::test_getColonne()<<endl;
	cout<<"test_modifie\t"<<Labyrinthe::test_modifie()<<endl;
	cout<<"test_lit\t"<<Labyrinthe::test_lit()<<endl;
	cout<<"test_push\t"<<Labyrinthe::test_push()<<endl;
	cout<<"test_pop\t"<<Labyrinthe::test_pop()<<endl;
	cout<<"test_connexe\t"<<Labyrinthe::test_connexe()<<endl;
  cout<<"test_distMin\t"<<Labyrinthe::test_distMin()<<endl;


	// char* test = Labyrinthe::genDefaultLaby(4,4,1);

	// Labyrinthe::demo();
	// Labyrinthe lab(15,15);
	// lab.genLaby(80);

	// int _disMin = 0;
	// char* data = Labyrinthe::genDefaultLaby(6, 3, 1, &_disMin);
	//
	// Labyrinthe lab(data);
	//
	// int* chemin = lab.getCheminMin(-1,-1);
	// for (int i = 0; i <= chemin[0]; i++) {
	// 	printf("%d, ", chemin[i]);
	// }
	//
	// printf("\n%d = %d\n",lab.distMin(-1,-1), _disMin);
	// lab.chemin(-1,-1);
	// lab.affiche();
	//
	// delete[] data;
	// delete[] chemin;
	return 0;
}
