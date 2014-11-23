#include "btree.h"

int main()
{
	
	cout << "width? ";
	cin >> width;
	BTree* g = new BTree(width);
	g = g->insert("50");
	g = g->insert("40");
	g = g->insert("20");
	g = g->insert("30");
	g = g->insert("10");
	g = g->insert("11");
	g = g->insert("12");
	g = g->insert("41");
	g = g->insert("42");
	g = g->insert("46");
	g = g->insert("13");
	g = g->insert("14");
	g = g->insert("15");
	g = g->insert("51");
	g = g->insert("52");
	g = g->insert("38");
	g = g->insert("39");
	g = g->insert("05");
	g = g->insert("08");
	g = g->insert("04");
	g = g->insert("16");
	g->pisi();

	brisi(g);
	system("pause");
	return 0;
}