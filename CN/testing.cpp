#include <bits/stdc++.h>
using namespace std;

string toHex(string bin)
{
	int l = bin.length();
	cout << (4 - l%4) << endl;
//	bin = string(4 - l%4, '0') + bin;
//	cout << bin << endl;
}

int main()
{
	toHex("01010");
//	toHex("1010");
	toHex("001010");
	toHex("0001010");
}
