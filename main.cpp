#include <iostream>
#include <stack>
#include <cstring>
#include <fstream>

using namespace std;

int	get_func_res(char *buffer, int i, int last)
{
	if (last == 30)
	{
		return (buffer[i - 30] ^ buffer[i - 29] ^ buffer[i - 26] ^ buffer[i - 24]);
	}
	return (buffer[i - 31] ^ buffer[i - 28]);
}

int	get_func_res(int *buffer, int i, int last)
{
	if (last == 30)
	{
		return (buffer[i - 30] ^ buffer[i - 29] ^ buffer[i - 26] ^ buffer[i - 24]);
	}
	return (buffer[i - 31] ^ buffer[i - 28]);
}

void generate_to_last(stack <int*> &L1, char *buffer, int last, char *fromfile)
{
	int count = 0;
	for (int i = last; i < 208; i++)
		buffer[i] = get_func_res(buffer, i, last);
	for (int i = 0; i < 208; i++)
	{
		count = count + ((int)buffer[i] ^ (int)fromfile[i]);
	}
	if (count < 60)
	{
		int *saved = new int[2048];
		for (int i = 0; i < 208; i++)
			saved[i] = buffer[i];
		for (int i = 208; i < 2048; i++)
			saved[i] = get_func_res(saved, i, last);
		cout << "-----------------------------------------------\n";
		for (int i = 0; i < 2048; i++)
			cout << saved[i];
		L1.push(saved);
	}
}

void create_stack(stack <int*> &L1, char *buffer, int pos, int last, char *fromfile)
{
	int flag = 0;
	if (pos == 5)
		cout << "here\n";
	if (pos != last)
	{
		buffer[pos] = flag;
		create_stack(L1, buffer, pos + 1, last, fromfile);
		buffer[pos] = ++flag;
		create_stack(L1, buffer, pos + 1, last, fromfile);
	}
	else
		generate_to_last(L1, buffer, last, fromfile);
}

int get_pattern(int *L1, int *L2, char *fromfile, char *pattern)
{
	int count = 32;
	for (int i = 0; i < 32; i++)
	{
		if (L1[i] != L2[i])
		{
			if (fromfile[i] == L1[i])
				pattern[i] = 1;
			else
				pattern[i] = 0;
			count -= 1;
		}
		else
		{
			if (((0 & L1[i]) ^ ((1 ^ 0) & L2[i])) != fromfile[i])
				if (((1 & L1[i]) ^ ((1 ^ 1) & L2[i])) != fromfile[i])
					return (-1);
			pattern[i] = 2;
		}
	}
	return (count);
}

void create_new_pat(char *pattern, char *buffer, int len, char *newpat)
{
	int buf = -1;
	for (int i = 0; i < 32; i++)
	{
		if (pattern[i] != 2)
			newpat[i] = pattern[i];
		else
			newpat[i] = buffer[++buf];
	}
}

void print_L(int *L)
{
	cout << "result" << endl;
	cout << "----------------------------------------------------------------------" << endl;
	for (int i = 0; i < 2048; i++)
	{
		cout << L[i];
	}
	cout << endl;
}

void print_L(char *L)
{
	cout << "result" << endl;
	cout << "----------------------------------------------------------------------" << endl;
	for (int i = 0; i < 2048; i++)
	{
		cout << L[i];
	}
	cout << endl;
}


void check_L3(int *L1, int *L2, char *fromtext, char *newpat)
{
	for (int i = 32; i < 2048; i++)
	{
		newpat[i] = newpat[i - 32] ^ newpat[i - 31] ^ newpat[i - 30] ^ newpat[i - 29] ^ newpat[i - 27] ^ newpat[i - 25];
		if (((newpat[i] & L1[i]) ^ ((1 ^ newpat[i]) & L2[i])) != fromtext[i])
			return ;
	}
	print_L(L1);
	print_L(L2);
	for (int i = 0; i < 2048; i++)
		newpat[i] += '0';
	print_L(newpat);
}

void generate_L3(int *L1, int *L2, char *fromfile, char *pattern, int len, char *buffer, int num)
{
	if (num != len)
	{
		buffer[num] = 0;
		generate_L3(L1, L2, fromfile, pattern, len, buffer, num + 1);
		buffer[num] = 1;
		generate_L3(L1, L2, fromfile, pattern, len, buffer, num + 1);
	}
	else
	{
		char newpattern[2050];
		create_new_pat(pattern, buffer, len, newpattern);
		check_L3(L1, L2, fromfile, newpattern);
	}
}

void print_pattern(char *pat)
{
	cout << "pattern" << endl;
	for (int i = 0; i < 32; i++)
	{
		cout << (int)pat[i];
	}
	cout << endl;
}
void create_L3(stack<int*> &L1, stack <int*> &L2, char *buffer, char *fromfile)
{
	int L2_mas_size = L2.size();
	int **L2_mas = new int*[L2.size()];
	for (int i = 0; i < L2_mas_size; i++)
	{
		L2_mas[i] = L2.top();
		L2.pop();
	}
	while (L1.size() != 0)
	{
		int *fromf = L1.top();
		for (int i = 0; i < L2_mas_size; i++)
		{
			char pattern[2050];
			int len = get_pattern(fromf, L2_mas[i], fromfile, pattern);
			print_pattern(pattern);
			if (len == -1)
				continue;
			generate_L3(fromf, L2_mas[i], fromfile, pattern, len, buffer, 0);
		}
		delete(fromf);
		L1.pop();
	}
}

int *convert(char *buffer)
{
	int *ret = new int[2048];
	for (int i = 0; i < 2048; i++)
		ret[i] = buffer[i] - '0';
	return(ret);
}

int main()
{
	int forsin;
	char fromfile[2050];
	stack <int*> L1;
	stack <int*> L2;
	char buffer[2048];
	ifstream text;
	text.open("Crypto_CP4_variants_2018.txt");
	if (!text)
	{
		cout << "bad file\n";
		return (1);
	}
	text >> fromfile;
	for (int i = 0; i < 2048; i++)
		fromfile[i] -= '0';
	create_stack(L1, buffer, 0, 30, fromfile);
	create_stack(L2, buffer, 0, 31, fromfile);
	create_L3(L1, L2, buffer, fromfile);
	cin >> forsin;
}
