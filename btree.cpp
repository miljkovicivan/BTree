#include "btree.h"

int width;
void BTree::brisi(){
	BTree * cur = this;
	queue<BTree*> q;
	q.push(cur);
	while (!q.empty())
	{
		cur = q.front();
		q.pop();
		for (int i = 0; i < width; i++)
		{
			if (cur->ptr[i])
				q.push(cur->ptr[i]);
		}
		cur->brisiCvor();
	}
}

void BTree::brisiCvor(){
	delete[] used;
	delete[] key;
	for (int i = 0; i < width; i++)
		delete ptr[i];
	delete[] ptr;
	delete father;
}

BTree::BTree(int r) {
	width = r;
	key = new BTree::Kljuc[width - 1];
	used = new bool[width - 1];
	for (int i = 0; i < width - 1; i++)
		used[i] = false;
	ptr = new BTree*[width];
	for (int i = 0; i < width; i++)
		ptr[i] = 0;
	zauzeto = 0;
	empty = true;
	leaf = true;
	father = nullptr;
}

ostream& operator<<(ostream& it, BTree::SplitRes split_res)
{
	return it << "levo " << split_res.left << endl << "desno " << split_res.right << "prenosi se " << split_res.mid.val << endl;
}

BTree::SplitRes split(BTree * cur, string str, string opis)  // radi, ne diraj nista xD
{
	//extern int width;
	int i;
	BTree::Kljuc * vector = new BTree::Kljuc[width];
	for (i = 0; i < cur->zauzeto; i++)
		vector[i] = cur->key[i];
	i = 0;
	while (str > vector[i].val && i < cur->zauzeto)
		i++;
	BTree::Kljuc temp1, temp2;
	temp1 = vector[i];
	vector[i].val = str;
	vector[i].opis = opis;
	vector[i].frekvencija = 0;
	for (i++; i < width; i++) 
	{
		temp2 = temp1;
		temp1 = vector[i];
		vector[i] = temp2;
		// shift keys;
	}
	int m = floor(width / 2);
	BTree::Kljuc mid = vector[m];
	BTree * left, *right;
	left = new BTree(width);
	right = new BTree(width);
	for (i = 0; i < m; i++) // left child
	{
		left->used[i] = true;
		left->key[i] = vector[i];
		left->ptr[i] = cur->ptr[i];
	}
	left->ptr[i] = cur->ptr[i];
	for (i = 0; i < width - m-1; i++)
	{
		right->key[i] = cur->key[m + i];
		right->ptr[i] = cur->ptr[m + i];
		right->used[i] = true;
	}
	right->ptr[i] = cur->ptr[i];

	left->father = cur->father;
	right->father = cur->father;
	right->zauzeto = width - m - 1;
	left->zauzeto = m;
	//delete cur;
	delete[] vector;
	return BTree::SplitRes(mid, left, right);
}

BTree * BTree::findPtr(string str){
	int i = 0;
	extern int width;
	for (i = 0; i < width - 1; i++)
	{
		if (key[i].val > str)
			return ptr[i];
	}
	return ptr[i];
}

int BTree::findInNode(string str){
	extern int width;
	for (int i = 0; i < width - 1; i++)
	if (used[i])
	if (key[i].val == str)
		return i;
	return -1;
}

bool BTree::hasRoom(){
	int i;
	extern int width;
	for (i = 0; i < zauzeto; i++)
	if (!used[i])
		return true;
	return false;
}

int BTree::hasKey(string str)
{
	for (int i = 0; i < zauzeto; i++)

	if (key[i].val == str)
		return i;
	return -1;
}

BTree * BTree::insert(string str, string ops)  //insert ne radi, ne znam sta, pogledaj sutra
{
	BTree * cur = this;

	while (!cur->isLeaf())
	{
		int ind;
		if ((ind = cur->hasKey(str)) == -1)
			cur = cur->findPtr(str);
		else
		{
			cur->key[ind].frekvencija++;
			return this;
		}
	}   // find

	int ind;
	if ((ind = cur->hasKey(str)) != -1){
		cur->key[ind].frekvencija++;
		return this;
	} // if key is in leaf node.
		
	if (cur->hasRoom())
		cur->push(Kljuc(str, ops));
	else
	{
		BTree::SplitRes res;
		while (!cur->hasRoom()) // goes at least once through while loop
		{
			 res = split(cur, str, ops);
			if (!cur->hasFather())
			{
				BTree* new_root = new BTree(width);
				new_root->insert(str, ops);
				return new_root;
			}
			else
			{
				BTree * temp = cur->father;
				delete cur;
				cur = temp;
			}
		}
		cur->push(res.mid, res.left, res.right);
	}
}

void BTree::push(BTree::Kljuc kljuc, BTree * left, BTree * right)
{
	int i = 0;
	BTree::Kljuc temp1, temp2;
	BTree * t1, *t2;
	temp1 = kljuc;
	while (temp1.val > key[i].val && i < zauzeto)
		i++;
	int ind = i; // remeber push place
	t1 = ptr[i + 1];
	for (i; i <= zauzeto; i++)
	{
		temp2 = temp1;
		temp1 = key[i];
		key[i] = temp2;
		// swap keys
		t2 = t1;
		t1 = ptr[i + 1];
		ptr[i + 1] = t2;
		//swap pointers
	}
	ptr[ind] = left;
	ptr[ind + 1] = right;
	//////////////////////////////////////////////// WET FLOOR!!!
	used[zauzeto] = true;
	zauzeto++;
}

void BTree::pisi()
{
	BTree * cur = this;
	queue<BTree*> q;
	q.push(cur);
	while (!q.empty())
	{
		cur = q.front();
		q.pop();
		for (int i = 0; i < width; i++)
		{
			if (cur->ptr[i])
				q.push(cur->ptr[i]);
		}
		cout << cur << ",";
	}

}

ostream& operator<<(ostream& it, BTree * cur)
{
	extern int width;
	for (int i = 0; i < cur->zauzeto; i++){
		string val = cur->key[i].val; string opis = cur->key[i].opis; int f = cur->key[i].frekvencija;
		it << "(" << val << "," << opis << "," << f << ")";
	}
	return it;
}

