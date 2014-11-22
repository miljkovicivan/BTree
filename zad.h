#ifndef __PRVI_H__
#define __PRVI_H__

#include <vld.h>
#include <cstdlib>
#include <string>
#include <queue>
#include <iostream>
using namespace std;
int red;

class BTree
{
	struct Kljuc
	{
		string val, opis;
		int frekvencija;
	};
	int zauzeto; // mozda red ovde
	BTree ** ptr, *father;
	Kljuc * key;
	bool leaf;
	bool * used, empty;
	void brisi()
	{
		BTree * tek = this;
		queue<BTree*> q;
		q.push(tek);
		while (!q.empty())
		{
			tek = q.front();
			q.pop();
			for (int i = 0; i < red; i++)
			{
				if (tek->ptr[i])
					q.push(tek->ptr[i]);
			}
			tek->brisiCvor();
		}
	}
	void brisiCvor()
	{
		delete [] used;
		delete [] key;
		for (int i = 0; i < red; i++)
			delete ptr[i];
		delete [] ptr;
		delete father;
		//delete tek;
	}
public:
	BTree(int r) {
		red = r;
		key = new Kljuc[red - 1];
		used = new bool[red - 1];
		for (int i = 0; i < red - 1; i++)
			used[i] = false;
		ptr = new BTree*[red];
		for (int i = 0; i < red; i++)
			ptr[i] = 0;	
		zauzeto = 0;
		empty = true;
		leaf = true;
		father = nullptr;
	}
	~BTree() { brisi(); }
	BTree * findPtr(string str)  //proveri
	{
		int i = 0;
		extern int red;
		for (i = 0; i < red - 1; i++)
		{
			if (key[i].val > str)
				return ptr[i];
		}
		return ptr[i];
	}
	int findInNode(string str)  //proveri
	{
		extern int red;
		for (int i = 0; i < red - 1; i++)
		if (used[i])
		if (key[i].val == str)
			return i;
		return -1;
	}
	bool imaMesta(/*BTree* tek*/)  //proveri
	{
		int i;
		extern int red;
		for (i = 0; i < red - 1; i++)
		if (!used[i])
			return true;
		return false;
	}
	bool isLeaf() const { return leaf; }
	/*friend void sort(BTree * tek) // pri brisanju OBAVEZNO zajebi used, i sve used napakuj uz levu ivicu, zbog sorta. OBAVEZNO
	{
		extern int red;
		for (int i = 0; i < red - 2; i++)
		for (int j = i + 1; j < red - 1; j++)
		if (tek->used[i] && tek->used[j])
		if (tek->key[i].val > tek->key[j].val)
		{
			Kljuc temp = tek->key[i];
			tek->key[i] = tek->key[j];
			tek->key[j] = temp;
		}
	}*/
/*	void add(string str)
	{
		BTree * tek = this, *preth = nullptr;

		while (!tek->isLeaf()){   ////// find
			preth = tek;
			tek = tek->findPtr(str);
		}
		if (tek->imaMesta())
			// add opusteno
		else
		{
			//propagiraj
		}
	}
	void push(string std)
	{
		if (imaMesta())
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
	void insert(string str, string ops = "")
	{
		if (isLeaf() && imaMesta())
			push(str, ops);
		else
			cout << "not leaf or not space\n";
	}
	void push(string str, string ops)
	{
		int i = 0;
		Kljuc temp1, temp2;
		temp1.frekvencija = 0;
		temp1.opis = ops;
		temp1.val = str;
		while (str > key[i].val && i < zauzeto)
			i++;
		for (i; i <= zauzeto; i++)
		{
			temp2 = temp1;
			temp1 = key[i];
			key[i] = temp2;
		}
		used[zauzeto] = true;
		zauzeto++;
	}

	void pisi() // proveri
	{
		BTree * tek = this;
		queue<BTree*> q;
		q.push(tek);
		while (!q.empty())
		{
			tek = q.front();
			q.pop();
			for (int i = 0; i < red; i++)
			{
				if (tek->ptr[i])
					q.push(tek->ptr[i]);
			}
			cout << tek;
		}
		
	}
	friend ostream& operator<<(ostream& it, BTree * tek)
	{
		extern int red;
		for (int i = 0; i < red - 1; i++)
		if (tek->used[i]){
			string val = tek->key[i].val;
			it << val << endl;
		}
		return it;
	}

};
/*
bool operator>(string a, string b)
{
	int i = 0;
	while (a[i] == b[i++]);
	if (a[i] > b[i])
		return true;
	else
		return false;
}
*/
#endif