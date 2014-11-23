#include "btree.h"

int width;
void brisi(BTree * cur){
	queue<BTree*> q;
	q.push(cur);
	while (!q.empty())
	{
		cur = q.front();
		q.pop();
		for (int i = 0; i < cur->zauzeto; i++)
		{
			if (cur->ptr[i])
				q.push(cur->ptr[i]);
		}
		delete cur;
	}
}

BTree::~BTree(){
	delete[] used;
	delete[] key;
	delete[] ptr;
	//delete father;
}

BTree::BTree(BTree&& cur) // move constructor
{
	key = cur.key;
	ptr = cur.ptr;
	zauzeto = cur.zauzeto;
	father = cur.father;
	leaf = cur.leaf;
	used = cur.used;
	empty = cur.empty;
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

BTree::SplitRes BTree::split(BTree::Kljuc k, BTree * l, BTree * r)
{
	//extern int width;
	int i;
	BTree::Kljuc * keys = new BTree::Kljuc[width];
	BTree ** branches = new BTree*[width + 1];
	for (i = 0; i < zauzeto; i++)
		keys[i] = key[i];
	i = 0;
	while (k.val > keys[i].val && i < zauzeto)
		i++;
	int j;
	for (j = 0; j < i; j++)
		branches[j] = ptr[j];
	
	while (j < width)
		branches[j+1] = ptr[j++];
	branches[i] = l;
	branches[i+1] = r;

	BTree::Kljuc temp1, temp2;
	temp1 = keys[i];
	keys[i].val = k.val;
	keys[i].opis = k.opis;
	keys[i].frekvencija = 0;
	for (i++; i < width; i++) 
	{
		temp2 = temp1;
		temp1 = keys[i];
		keys[i] = temp2;
		// shift keys;
	}
	int m = floor(width / 2);
	BTree::Kljuc mid = keys[m];
	BTree * left, *right;
	left = new BTree(width);
	right = new BTree(width);
	for (i = 0; i < m; i++) // left child
	{
		left->used[i] = true;
		left->key[i] = keys[i];
		left->ptr[i] = branches[i];
	}
	left->ptr[i] = branches[i];
	for (i = 0; i < width - m-1; i++)
	{
		right->key[i] = keys[m + i+1];
		right->ptr[i] = branches[m + i+1];
		right->used[i] = true;
	}
	right->ptr[i] = branches[m+i+1];

	left->father = this;
	right->father = this;
	right->zauzeto = width - m - 1;
	left->zauzeto = m;
	//delete cur;
	delete[] branches;
	delete[] keys;
	return BTree::SplitRes(mid, left, right);
}

BTree * BTree::findPtr(string str){
	int i = 0;
	for (i = 0; i < zauzeto; i++)
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
	for (i = 0; i < width-1; i++)
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

BTree * BTree::insert(string str, string ops)
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
		BTree::SplitRes *res = nullptr;
		while (!cur->hasRoom()) // goes at least once through while loop
		{
			if (res != nullptr)
				*res = cur->split(res->mid, res->left, res->right);
			else
			{
				res = new BTree::SplitRes;
				*res = cur->split(Kljuc(str, ops), res->left, res->right);
			}
			if (!cur->hasFather())
			{
				BTree* new_root = new BTree(width);
				new_root->empty = false;
				new_root->father = nullptr;
				new_root->leaf = false;
				new_root->push(res->mid, res->left, res->right);
				res->left->father = res->right->father = new_root;
				return new_root;
			}
			else // if cur->hasFather
			{
				BTree * temp = cur->father;
				res->left->father = temp;
				res->right->father = temp;
				delete cur;
				cur = temp;
				
			}
		}
		cur->push(res->mid, res->left, res->right);
	}
	return this;
}

void BTree::push(BTree::Kljuc kljuc, BTree * left, BTree * right)
{
	empty = false;
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
	//for (int i = 0; i < zauzeto; i++)
	//if (ptr[i])
	//	leaf = false;
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
		cout << cur << ",\n";
	}

}

ostream& operator<<(ostream& it, BTree * cur)
{
	extern int width;
	for (int i = 0; i < cur->zauzeto; i++){
		string val = cur->key[i].val; string opis = cur->key[i].opis; int f = cur->key[i].frekvencija;
		it << "(" << val << ")";
	}
	return it;
}

