#include "stdafx.h"
#include<iostream>
#include<conio.h>
#include<string>
#include<map>
#include<fstream>
#include<vector>
using namespace std;

typedef map<string, int> M;
typedef vector<map<string, int>> VM;
typedef vector<pair<map<string, int>, int>> VMI;

void split(string & s, char c, map<string, int> & v)
{
	size_t p = 0;
	size_t q = s.find(c);

	while (q != string::npos) {
		v[s.substr(p, q - p)]++;
		p = ++q;
		q = s.find(c, q);

		if (q == string::npos)
			v[s.substr(p, s.length())]++;
	}
}
void split2(string & s, char c, vector<string> & v)
{
	size_t p = 0;
	size_t q = s.find(c);

	while (q != string::npos) {
		v.push_back(s.substr(p, q - p));
		p = ++q;
		q = s.find(c, q);

		if (q == string::npos)
			v.push_back(s.substr(p, s.length()));
	}
}

void counting(VM& all,VMI& join,int minsup,VMI& answer)
{
	int c, cc;
	for (int i = 0; i < join.size(); i++)
	{
		c = 0; cc = 0;
		for (int j = 0; j < all.size(); j++)
		{
			for (auto& g:join[i].first)
			{
				if (all[j].count(g.first))
				{
					c++;
				}
			}
			if (c == join[i].first.size())
				cc++;
			c = 0;
		}
		if (cc >= minsup)
			answer.push_back(make_pair(join[i].first, cc));
	}
}

void join_creator(VMI& join, VMI& answer)
{
	M m1, m2;
	bool flag = true;
	for (int i = 0; i < join.size()-1; i++)
	{
		
		m2 = join[i].first;
		for (int j = i + 1; j < join.size(); j++)
		{
			m1.clear();
			m1 = m2;
			for (auto & l : join[j].first)
				m1[l.first]++;
			if (m1.size() == join[0].first.size() + 1)
			{
				for (int l = 0; l < answer.size(); l++) {
					int s = 0;
					for (auto& f : m1)
					{
						if (answer[l].first.count(f.first))
							s++;
					}
					if (s == m1.size())
					{
						flag = false;
						break;
					}
				}
				if (flag == true)
					answer.push_back(make_pair(m1, 0));
				flag = true;
			}
		}
	}
}




int main()
{
	int min_sup = 2;
	string s;
	string S="";
	cout << "please enter min_sup:";
	cin >> min_sup;
	cout << "please insert file location:" << endl; //example:C:\Users\fatemeh Khalifeyi\Desktop\test.txt
	char a[300];
	string location;
	cin.get();
	cin.getline(a, 300);
	ifstream ifin(a);
	while (!ifin.eof())
	{
		getline(ifin, s);
		S += s;
		S += '|';
	}
	
	vector<string> v;
	string b="";
	for (int i = 13; i < S.length(); i++)
	{

			if (S[i] == '|')
			{
				v.push_back(b);
				b = "";
			}
			else
			{
				if ((S[i] >= 65 && S[i] <= 90) || (S[i] >= 97 && S[i] <= 122) || S[i] == ';')
				{
					if (S[i] != '|')
					{
						b += S[i];

					}
				}

			}

	}

	VM all;
	for (int i = 0; i < v.size(); i++)
	{
		M m2;
		m2.clear();
		split(v[i], ';', m2);
		all.push_back(m2);
	}

	map<string, int> itemset1;
	map<string, int> itemset2;
	vector<string> k_itemset;
	vector<string> k1_itemset;

	for (int i = 0; i < v.size(); i++)
	{
		split(v[i], ';', itemset1);
		split(v[i], ';', itemset2);
	}
	for (auto & i : itemset1)
	{
		int s = i.second;
		auto ss = i.first;
		if (s >= min_sup)
			k_itemset.push_back(ss);
	}

	

	VMI join_itemset;
	for (int i = 0; i < k_itemset.size() - 1; i++)
	{
		M m;
		for (int j = i + 1; j < k_itemset.size(); j++)
		{
			m.clear();
			m[k_itemset[i]]++;
			m[k_itemset[j]]++;
			join_itemset.push_back(make_pair(m, 0));
		}
	}
	VMI L2;
	counting(all, join_itemset, min_sup, L2);
	
	VMI vmi1=L2, vmi2,vmi3;
	while (true)
	{
		vmi2.clear();
		join_creator(vmi1, vmi2);

		if (vmi2.size() == 0)
			break; 
		vmi1.clear();
		counting(all, vmi2,min_sup, vmi1);
		counting(all, vmi2, min_sup, vmi3);
		if (vmi1.size() == 0)
			break;
		
	}

	for (int i = 0; i < vmi3.size(); i++)
	{
		for (auto & j : vmi3[i].first)
		{
			cout << j.first << " - ";
		}
		cout << " > " << vmi3[i].second << endl;
	}

	_getch();
	return 0;
}

