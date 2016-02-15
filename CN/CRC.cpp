/*
2
11010011101100 1011
100100 1101
*/

#include <bits/stdc++.h>
using namespace std;

string eval(string a, string b)
{
	for (int i = 0; i < a.length(); i++)
	{
		a[i] = ((a[i] - '0') ^ (b[i] - '0')) + '0';
	}
	int pos = a.find("1");
	if(pos == -1)
		return "";
	else	
		return a.substr(pos);
}

string getCRCbits(string msg, string poly)
{
	int l = poly.length();
	string to = msg.substr(0, l);
	int pos = l, p;
	cout << msg << " " << poly << endl;
	while(true)
	{
		to = eval(to, poly);
		p = to.length();

		if(pos == msg.length())				//	if complete msg taken
			break;	
		if(pos + l - p > msg.length())		//	if msg ends here
		{
			to = to + msg.substr(pos);
			break;
		}

		to = to + msg.substr(pos, l - p);
		pos = pos + l - p;				
	}
	return string(l - to.length() - 1, '0') + to;
}

int main()
{
	string _temp;
	string _name = __FILE__;
	freopen(_name.c_str(), "r", stdin);
	getline(cin, _temp);

	int t;
	cin >> t;
	
	string poly, msg;
	while(t--)
	{
		cin >> msg >> poly;
		msg = msg + string(poly.length() - 1, '0');
		cout << getCRCbits(msg, poly) << endl;
	}

	getline(cin, _temp);
	return 0;
}
