/*		example
6	
E E+E
E E-E
E (E)
F Fi
F +F
F F-
*/
/*
2
S (S)S
S #
S
*/
/*
4
E E+E
E E*E
E (E)
E #
E
*/
/*		another example
8
E TG
G +TG
G #
T FH
H *FH 
H #
F i 
F (E)
E
*/

/*
first :
E	( i
G 	+ #
T	( i
H	* #
F	( i

follow :
E	) $
G	) $
T	+ ) $
H	+ ) $
F	+ * ) $
-------------------------------------------------------------------
		id		 + 		   * 		   (	  	 )			$
		
E		TG							  TG
G				+TG								 #			#
T		FH							  FH
H				#		  *FH					 #			#
F		i							  (E)
-------------------------------------------------------------------
*/

#include <bits/stdc++.h>
#define M 256
using namespace std;

map<char, vector<string> > mp;
map<char, string> first, follow;
set<char> NT;
string PT[M][M];
char SS;

void print_map()
{
    for (map<char, vector<string> > :: iterator p = mp.begin(); p != mp.end(); p++)
    {
        cout << p->first << " -> ";
        vector<string> t = p->second;
        
        for (int i = 0; i < t.size(); i++)
            cout << t[i] << " | ";
        cout << endl;
    }
}

bool isNonTerminal(char c)
{
    if(c >= 'A' && c <= 'Z')
        return true;
    return false;
}

string operator-(const string& a, const string& b)
{
	if(a.find(b) == -1)
		return a;
	int pos = a.find(b);
	string temp = a;
	temp.erase(temp.begin() + pos, temp.begin() + pos + b.length());
	return temp;
}

string add(string a, string b)
{
	if (a != "#" && b != "#")
		return (a + b);	
	if (a == "#")
		return b;
	return a;
}

string common(string a, string b)
{
	int l = min(a.length(), b.length());
	string res = "";
	
	for (int i = 0; i < l; i++)
	{
		if (a[i] != b[i])
			return res;
		res += a[i];	
	}
	return res;
}

vector<string> make_unique(vector<string> t)
{
	set<string> st;
	st.insert(t.begin(), t.end());
	vector<string> tmp;
	copy(st.begin(), st.end(), std::back_inserter(tmp));
	return tmp;
}

void remove_left_factoring()
{
	for (map<char, vector<string> > :: iterator p = mp.begin(); p != mp.end(); p++)
	{
		vector<string>& t = p->second;
		while (1)
		{
			string comm = "";
			for (int i = 0; i < t.size(); i++)
			{
				for (int j = i + 1; j < t.size(); j++)
				{
					if (common(t[i], t[j]).length() > comm.length())
						comm = common(t[i], t[j]);
				}
			}		
			if (comm.length() == 0)
				break;
				
			char nt = *NT.rbegin() + 1;
			NT.insert(nt);
			
			string tn(1, nt);
			vector<string> alpha;
			for (int i = 0; i < t.size(); i++)
			{
				if (t[i].find(comm) == 0)
				{
					alpha.push_back(t[i] - comm);
					t[i] = comm + tn;
				}
			}
			mp[p->first] = make_unique(t);
			mp[nt] = make_unique(alpha);
		}
	}    	
	print_map();
	cout << endl;
}

void remove_left_recursion()
{
    /*
	A -> Aa1 | Aa2 | .. | b1 | b2 .. 
	change that to
	
	A -> b1A' | b2A' | .. 
	A' -> a1A' | a2A' | .. | #
	*/
	for (map<char, vector<string> > :: iterator p = mp.begin(); p != mp.end(); p++)
    {
        vector<string> t = p->second;
        bool lr = 0;
        
        for (int i = 0; i < t.size(); i++)
        {
        	if (t[i][0] == p->first)
        	{
				lr = 1;
        		break;
        	}
		}
		
		if(lr)
		{
			vector<string> alpha, beta;
			char nt = *NT.rbegin() + 1;
			NT.insert(nt);
			
			string tn(1, nt);
			string f(1, p->first);
			
			for (int i = 0; i < t.size(); i++)
			{
				if (t[i][0] != p->first)
					beta.push_back(add(t[i], tn));
				else
					alpha.push_back(add(t[i], tn) - f);
			}
			alpha.push_back("#");
			mp[p->first] = beta;
			mp[nt] = alpha;	
		}
	}
	
	print_map();
	cout << endl;
}

void get_first_set()
{
    //  check starting of each prod
    //  put first of A into B if A -> B*
    //  if B has Eps in it then put first of A into C if A -> BC
    
    for (map<char, vector<string> > :: iterator p = mp.begin(); p != mp.end(); p++)
    {
        vector<string> t = p->second;
        first[p->first] = "";
        for (int i = 0; i < t.size(); i++)
        {
            if (!isNonTerminal(t[i][0]))
                first[p->first] += t[i][0];
        }
    }
    
    for (int i = 0; i < 2; i++)					//	twice so that later changes may visible to all
    for (map<char, vector<string> > :: iterator p = mp.begin(); p != mp.end(); p++)
    {
        vector<string> t = p->second;
        string f = first[p->first];
        for (int i = 0; i < t.size(); i++)
        {
            int k = 0;
            do
            {
                if (!isNonTerminal(t[i][k]))
                {
                	f += t[i][k];
                    break;
                }
                f += (first[t[i][k]] - "#");
            }
            while (k < t[i].length() && first[t[i][k++]].find('#') != -1);
        }
        first[p->first] += f;
    }
    
	cout << "First Set : \n";
    //  removing duplicate entries
    for (map<char, string> :: iterator p = first.begin(); p != first.end(); p++)
    {
        string f = p->second;
        set<char> st;
        st.insert(f.begin(), f.end());
        f = "";
        for (set<char> :: iterator p = st.begin(); p != st.end(); p++)
        	f += *p;
        p->second = f;       
        
        cout << p->first << "  " << p->second << endl;
	}
}

void get_follow_set()
{
	//	all follow set should be initialized with ""
    follow[SS] = "$"; 
    
    for (map<char, vector<string> > :: iterator p = mp.begin(); p != mp.end(); p++)
    {
        vector<string> t = p->second;
        for (int i = 0; i < t.size(); i++)
        {
            string f = t[i];
            for (int j = 0; j < f.length() - 1; j++)
            {
            	if (isNonTerminal(f[j]))
	            	follow[f[j]] += first[f[j + 1]] - "#";			//		for ab   follow(a)   <--   first(b)
	            	
                if (isNonTerminal(f[j]) && !isNonTerminal(f[j + 1]))
                {
                    follow[f[j]] += f[j + 1];
				}
            }
        }
    }
    
    for (int i = 0; i < 2; i++)
    for (map<char, vector<string> > :: iterator p = mp.begin(); p != mp.end(); p++)
    {
        vector<string> t = p->second;
        string f = follow[p->first];
        for (int i = 0; i < t.size(); i++)
        {
            int k = t[i].length() - 1;
            do
            {
                if (!isNonTerminal(t[i][k]))
                {
                    break;
                }
                follow[t[i][k]] += f;
            }
            while (k > 0 && first[t[i][k--]].find('#') != -1);
        }
    } 

    cout << "Follow Set : \n";
    for (map<char, string> :: iterator p = follow.begin(); p != follow.end(); p++)
    {
        string f = p->second;
        set<char> st;
        st.insert(f.begin(), f.end());
        f = "";
        for (set<char> :: iterator p = st.begin(); p != st.end(); p++)
        	f += *p;
        p->second = f;       
		
		cout << p->first << "  " << p->second << endl;   
    }
}

void build_parsing_table()
{
    for (map<char, vector<string> > :: iterator p = mp.begin(); p != mp.end(); p++)
    {
        vector<string> t = p->second;
        char E = p->first;
      	string fi = first[E];
       	string fo = follow[E];
        
        for (int i = 0; i < t.size(); i++)
        {
            char f = t[i][0];
            if(!isNonTerminal(f))
            {
                PT[E][f] += t[i]; 
            }
            else
            {
                for (int j = 0; j < fi.length(); j++) {
//                	cout << E << "  " << fi[j] << "   " << t[i] << endl;
                    PT[E][fi[j]] += t[i] + " ";
                }
            }
            
            if (fi.find('#') != -1)
            {
                for (int j = 0; j < fo.length(); j++)
                    PT[E][fo[j]] += "#";
            }       
        }
    }
}

//  # is for epsilon

int main()
{
    string _temp;
    string _name = __FILE__;        //    or use getcwd etc..
    freopen(_name.c_str(), "r", stdin);
    getline(cin, _temp);
    
    int prod;
    cin >> prod;
    char A;
    string alpha;
    
    while (prod--)
    {
        cin >> A;
        cin >> alpha;
        mp[A].push_back(alpha);
        NT.insert(A);
    }
    cin >> SS;
    getline(cin, _temp);
    
    for (int i = 0; i < M; i++)
    	for (int j = 0; j < M; j++)
    		PT[i][j] = "";
    
    remove_left_factoring();	
    remove_left_recursion();
    
    get_first_set();
    get_follow_set();

    build_parsing_table();   

	for (int i = 0; i < 25; i++)	cout << "-";
	cout << endl;
	
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (PT[i][j] != "")
			{
				cout << PT[i][j] << "(";
				printf("%c %c)\n", i, j);
			}
		}
	}
}
