#include "zad.h"

int main()
{
	
	extern int red;
	cout << "red? ";
	cin >> red;
	BTree g(red);
	g.insert("bbb");
	g.insert("aaa");
	g.insert("ccc");
	
	g.pisi();

	system("pause");
	return 0;
}