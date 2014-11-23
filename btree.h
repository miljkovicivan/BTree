//#ifndef __BTREE_H__
//#define __BTREE_H__

#include <vld.h>
#include <cstdlib>
#include <string>
#include <queue>
#include <iostream>
#include <math.h>

using namespace std;
extern int width;

class BTree
{
	struct Kljuc
	{
		string val, opis;
		int frekvencija;
		Kljuc(string v="", string o="", int f = 0){ val = v; opis = o; frekvencija = f; }
	};
	int zauzeto; // mozda width ovde
	BTree ** ptr, *father;
	Kljuc * key;
	bool leaf;
	bool * used, empty;
	void brisi();
	void brisiCvor();
public:
	struct SplitRes
	{
		BTree * right, *left;
		Kljuc mid;
		SplitRes(BTree::Kljuc m, BTree * l, BTree * r) { mid = m; left = l; right = r; }
		SplitRes() { right = left = nullptr; }
	};
	BTree::BTree(BTree&& cur);
	BTree(int r);
	~BTree();
	friend void brisi(BTree * cur);
	BTree * findPtr(string str);  //proveri
	int findInNode(string str);  //proveri
	bool hasRoom();  //proveri
	bool isLeaf() const { return leaf; }
	/*friend void sort(BTree * cur) // pri brisanju OBAVEZNO zajebi used, i sve used napakuj uz levu ivicu, zbog sorta. OBAVEZNO
	{
		extern int width;
		for (int i = 0; i < width - 2; i++)
		for (int j = i + 1; j < width - 1; j++)
		if (cur->used[i] && cur->used[j])
		if (cur->key[i].val > cur->key[j].val)
		{
			BTree::Kljuc temp = cur->key[i];
			cur->key[i] = cur->key[j];
			cur->key[j] = temp;
		}
	}*/
/*	void add(string str)
	{
		BTree * cur = this, *preth = nullptr;

		while (!cur->isLeaf()){   ////// find
			preth = cur;
			cur = cur->findPtr(str);
		}
		if (cur->hasRoom())
			// add opusteno
		else
		{
			//propagiraj
		}
	}
	void push(string std)
	{
		if (hasRoom())
		{
			// ubaci opusteno
			int i = 0;
			while (used[i++])
		}
		else
		{
			// propagiraj
		}
	}*/
	int hasKey(string str);
	BTree * insert(string str, string ops = "");
	SplitRes split(Kljuc key, BTree * l = nullptr, BTree * r = nullptr);
	friend ostream& operator<<(ostream& it, BTree::SplitRes split_res);
	bool hasFather() const { return father == nullptr ? 0 : 1; }
	void push(Kljuc key, BTree * left = nullptr, BTree * right = nullptr);
	void pisi();
	
	friend ostream& operator<<(ostream& it, BTree * cur);
};

//#endif