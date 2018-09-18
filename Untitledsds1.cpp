// C++ program to demonstrate working of BK-Tree
#include "bits/stdc++.h"
using namespace std;

// maximum number of words in dict[]
#define MAXN 100000

// defines the tolerence value
#define TOL  2

// defines maximum length of a word
#define LEN 20

struct Node
{
    // stores the word of the current Node
    string word;

    // links to other Node in the tree
    int next[2*LEN];

    // constructors
    Node(string x):word(x)
    {
        // initializing next[i] = 0
        for(int i=0; i<2*LEN; i++)
            next[i] = 0;
    }
    Node() {}
};

// stores the root Node
Node RT;

// stores every Node of the tree
Node tree[MAXN];

// index for current Node of tree
int ptr;

int min(int a, int b, int c)
{
    return min(a, min(b, c));
}

// Edit Distance
// Dynamic-Approach O(m*n)
int editDistance(string& a,string& b)
{
    int m = a.length(), n = b.length();
    int dp[m+1][n+1];

    // filling base cases
    for (int i=0; i<=m; i++)
        dp[i][0] = i;
    for (int j=0; j<=n; j++)
        dp[0][j] = j;

    // populating matrix using dp-approach
    for (int i=1; i<=m; i++)
    {
        for (int j=1; j<=n; j++)
        {
            if (a[i-1] != b[j-1])
            {
                dp[i][j] = min( 1 + dp[i-1][j],  // deletion
                                1 + dp[i][j-1],  // insertion
                                1 + dp[i-1][j-1] // replacement
                              );
            }
            else
                dp[i][j] =  min( 1 + dp[i-1][j],  // deletion
                                1 + dp[i][j-1],  // insertion
                                dp[i-1][j-1] // replacement
                              );
        }
    }
    return dp[m][n];
}

// adds curr Node to the tree
void add(Node& root,Node& curr)
{
    if (root.word == "" )
    {
        // if it is the first Node
        // then make it the root Node
        root = curr;
        return;
    }

    // get its editDist from the Root Node
    int dist = editDistance(curr.word,root.word);

    if (tree[root.next[dist]].word == "")
    {
        /* if no Node exists at this dist from root
         * make it child of root Node*/

        // incrementing the pointer for curr Node
        ptr++;

        // adding curr Node to the tree
        tree[ptr] = curr;

        // curr as child of root Node
        root.next[dist] = ptr;
    }
    else
    {
        // recursively find the parent for curr Node
        add(tree[root.next[dist]],curr);
    }
}

vector <string> getSimilarWords(Node& root,string& s, bool& a)
{
    vector < string > ret;
    if (root.word == "")
       return ret;

    // calculating editdistance of s from root
    int dist = editDistance(root.word,s);


    if (dist == 0)
    {
        ret.push_back(root.word);
        a = true;
        return ret;
    }
    else{
        a = false;
    }
    // if dist is less than tolerance value
    // add it to similar words
    if (dist <= TOL) ret.push_back(root.word);



    // iterate over the string havinng tolerane
    // in range (dist-TOL , dwordist+TOL)
    int start = dist - TOL;
    if (start < 0)
       start = 1;

    while (!a && start < dist + TOL)
    {
        vector <string> tmp =
             getSimilarWords(tree[root.next[start]],s, a);
        if(a)
        {
            return tmp;
        }
        for (int i = 0 ; i < tmp.size(); i++)
            ret.push_back(tmp[i]);
        start++;
    }
    return ret;
}

string userInput(vector<string> match)
{
    int a;
    cout << "Choose word.\n";
    cin >> a;
    return match[a];
}

// driver program to run above functions
int main(int argc, char const *argv[])
{
    bool a = false;
    string correctSen;
    // dictionary words
    /*string dictionary[] = {"hell","help","shel","smell",
                           "fell","felt","oops","pop","oouch","halt"
                          };*/
    ptr = 0;
    /*int sz = sizeof(dictionary)/sizeof(string);

    // adding dict[] words on to tree
    for(int i=0; i<sz; i++)
    {
        Node tmp = Node(dictionary[i]);
        add(RT,tmp);
    }*/
    ifstream ins;
    ins.open("wordlist3.txt");
    string word;
    while(!ins.eof()){
        ins >> word;
        Node tmp = Node(word);
        add(RT,tmp);
    }

    string sentence;
    cout << "Enter a sentence: ";
    getline(cin, sentence, '.');
    //cout << "Sentence: "<< sentence << endl;
    stringstream sen(sentence);
    while(sen >> word){
        //cout << word;
        //word[0] = tolower(word[0]);
        vector < string > match = getSimilarWords(RT, word, a);
        if(match[0] == word)
            correctSen += word;
        else
        {
            cout << word << " is incorrect\n";
            cout << "Correct words in dictionary for " << word << ":\n";
            for (int x = 0; x < match.size(); x++)
                cout << x << ": " << match[x] << endl;
            correctSen += userInput(match);
        }
        correctSen += " ";
    }
    cout << endl << endl << endl << correctSen << endl;
    return 0;
}


