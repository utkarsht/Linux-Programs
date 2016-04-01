/*
1
10110110
*/

#include <bits/stdc++.h>
using namespace std;

bool is2Power(int x)
{
	return x && (!(x & (x - 1)));
}

string toHex(string bin)
{
	int deci = 0;
	int l = bin.length();
	int t = 1;
	
	for (int i = l - 1; i >= 0; i--)
	{
		deci += (bin[i] - '0') * t;
		t <<= 1;
	}
	string hexa;
	stringstream ss;
	ss << hex << deci;
	ss >> hexa;
	return hexa;
}

string fromHex(string hexa)
{
	int l = hexa.length();
	string bin = "";
	for (int i = 0; i < l; i++)
	{
		
	}
}

int getCount(string op, int pos)
{
	int mask = 1 << pos;
	int cnt = 0;

	for (int i = 1; i < op.length(); i++)
	{
		if(i & mask)
		{
			if(op[i] == '1')
				cnt++;
		}
	}
	return cnt; 
}

int main()
{
	string _temp;
	string _name = __FILE__;
	freopen(_name.c_str(), "r", stdin);
	getline(cin, _temp);


	int t, xtra, cnt, idx;
	cin >> t;
	string msg;
	while(t--)
	{
		cin >> msg;
		xtra = 4;          						    //  manually

		string op(msg.length() + xtra + 1, '0');    //	extra 1 for 0th idx
		op[0] = ' ';		

		idx = 0;
		for (int i = 1; i < op.length(); i++)
		{
			if(!is2Power(i))
				op[i] = msg[idx++];	
		}

		for (int i = 0; i < xtra; i++)
		{
			cnt = getCount(op, i);
			op[1 << i] = (cnt & 1) ? '1' : '0';
		}
		cout << op << endl;
		
		op[8] = '1';								//	making error
		idx = 0;
		for (int i = 0; i < xtra; i++)
		{
			cnt = getCount(op, i);
			idx += (cnt & 1) ? (1 << i) : 0;
		}		
		cout << idx << endl;						//	flipped index
	}

	getline(cin, _temp);
	return 0;
}
