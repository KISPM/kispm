#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <regex>

using namespace std;
bool urlcheck(string url){
    const regex pattern("((http|https|ftp)://)(www.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");

    if(regex_match(url, pattern) and !url.empty()){
        return true;
    } else {
        return false;
    }
}

bool packageexists(string url, string package){
    string command="wget " + url + "/dist.kis -q -O -";
    system((command + " > /tmp/readdist").c_str());
    ifstream readdist;
    readdist.open("/tmp/readdist");
    string line;
    while(getline(readdist, line)){
        if (line == package){
            return true;
        } else {
            return false;
        }
    }

    return false;
}

vector<string> urls{};

// Driver Code
int main(int argc, char *argv[])
{
    ifstream repo;
    repo.open("/usr/local/kis/repo.kis");
    const regex urlpattern("((http|https|ftp)://)(www.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");
    string line;
    string url="";
    while(getline(repo, line)){ 
        if (regex_match(line, urlpattern)){
            url = line;
        } else {
            continue;
        }
    }

    if (urlcheck(url)){
        bool autorun=false;
        string isarg1 = argv[1];
        string isarg2 = argv[2];
        string install="install";
        string search="search";
        string isyesshort="-y";
        string isyeslong="--yes";
        if (isarg1 == install){
            if (isarg2 == isyesshort || isarg2 == isyeslong){
                autorun=true;
                for (int i = 3; i < argc; ++i){
                    if(packageexists(url, argv[i])){
                        cout << "Exists" << endl;
                    } else {
                        cout << "ERROR, package " << argv[i] << " does not exist!" << endl;
                    }
                }
            } else {
                for (int i = 2; i < argc; ++i){
                    cout << argv[i] << endl;
                }
            }
        } else if (isarg1 == search){
            for (int i = 2; i < argc; ++i){
                cout << argv[i] << endl;
            }
        }
    } else {
        cout << "Error: Repo url" << endl;
        return 1;
    }

    return 0;
}