/*
9
if a + (b - c) *d;
if (i == 0) i++; else i--;
abc >>> cd;
bd // de
ab*=cd
ab;def-=gh
a f c + - * eg
df *9 ld !! ds
if else a -= c; d <>
*/

//	add input above in one line and increment the count of test case

#include <bits/stdc++.h>
using namespace std;

vector<pair<string, string> > result;

bool isOperator(char c)
{	
	if(c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '%' || c == '<' || c == '>' || c == ',' 
	|| c == ';' || c == '(' || c == ')' || c == '{' || c == '}')
		return true;
    return false;
}

bool isNum(char c)
{
    for (char i = '0'; i <= '9'; i++)
    {
        if (c == i)
           return true;
    }
    return false;
}

bool isPunctuation(string c)
{
	if(c == ";" || c == ",")
		return true;
	return false;
}

bool checkPrec(char a, char b)
{
	if(a == '>' && b == '>')	return true;
	if(a == '<' && b == '<')	return true;
	if(a == '!' && b == '=')	return true;
	if(a == '=' && b == '=')	return true;
	if(a == '-' && b == '>')	return true;
	if(a == '+' && b == '=')	return true;
	if(a == '-' && b == '=')	return true;
	if(a == '*' && b == '=')	return true;
	if(a == '/' && b == '=')	return true;
	if(a == '%' && b == '=')	return true;
	if(a == '+' && b == '+')	return true;
	if(a == '-' && b == '-')	return true;
	return false;	
}

string removeOperator(string input)
{
	string o = "operator";
    int l = input.length();
    for (int i = 0; i < input.length(); i++)
    {
        if(isOperator(input[i]))
        {
			if(i < l - 1 && isOperator(input[i + 1]))
			{
				if(checkPrec(input[i], input[i + 1]))
				{				
					string t = "";		t += input[i + 2];
					if(i < l - 2 && isOperator(input[i + 2]) && !isPunctuation(t))
					{
						while(isOperator(input[i]))
							i++;
						continue;
					}
					string temp = "";
					temp += input[i];
					temp += input[i + 1];
					result.push_back(make_pair(o, temp));
					input = input.substr(0, i) + " " + input.substr(i + 2);
				}
				else
				{
					while(isOperator(input[i]))
						i++;
					continue;
				}
			}
			else
			{
				string temp = "";
				temp += input[i];
				result.push_back(make_pair(o, temp));
				input = input.substr(0, i) + " " + input.substr(i + 1);
			}
        }
    }
    return input;
}

bool isKeyword(string str)
{
	if(str == "int" || str == "float" || str == "double" || str == "char" || str == "for" || str == "while" || str == "do" 
	|| str == "if" || str == "else" || str == "cin" || str == "cout") 
		return true;
	return false;
}

void classify(string str)
{
	int l = str.length();
	if(!isalnum(str[0]))
	{
		result.push_back(make_pair("error", str));
		return;
	}
	if(isNum(str[0]))
	{
		for (int i = 1; i < l; i++)
		{
			if(!isNum(str[i]))
			{
				result.push_back(make_pair("error", str));
				return;
			}
		}
		result.push_back(make_pair("constant", str));
	}
	else
	{
		for (int i = 1; i < l; i++)
		{
			if(!isalnum(str[i]))
			{
				result.push_back(make_pair("error", str));
				return;
			}
		}
		result.push_back(make_pair("identifier", str));
	}
}

int main()
{
	string __path = __FILE__;
	freopen(__path.c_str(), "r", stdin);
	string __tmp;
	cin >> __tmp;
	
	int t;
	cin >> t;
	while(t--)
	{
		string input;
   		getline(cin, input);
   		cout << input << endl;
   		
		input = removeOperator(input);
		
		cout << input << endl;
		stringstream ss(input);
		string temp;
		while(ss >> temp)
		{
			classify(temp);
		}
		
		for (int i = 0; i < result.size(); i++)
		{
			pair<string, string> t = result[i];
			if(t.first == "identifier")
			{
				if(isKeyword(t.second))
					result[i].first = "keyword";
			}
			if(t.first == "operator")
			{
				if(isPunctuation(t.second))
				{
					result[i].first = "punctuation";
				}
			}
		}
		
		for (int i = 0; i < result.size(); i++)
		{
			pair<string, string> t = result[i];
			cout << t.first << " : " << t.second << endl;
		}
		cout << endl;
		result.clear();
	}

    cin >> __tmp;
}
