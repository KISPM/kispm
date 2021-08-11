#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <fstream>
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
    system((command + " > /tmp/readdist.kis").c_str());
    ifstream readdist;
    readdist.open("/tmp/readdist.kis");
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

void installpackage(string url, string package, string tar, bool autoinstall){
    // TODO: Make this cleaner
    string command1="mkdir -p /tmp/" + package + ".kis";
    system((command1).c_str());
    string command2="wget " + url + "/" + package + ".build.kis -q -O -";
    system((command2 + " > /tmp/packagebuild." + package + ".kis").c_str());
    string command3="wget " + tar + " -P /tmp/" + package + ".kis/";
    system((command3).c_str());
    string command4="mkdir -p /tmp/" + package + ".kis/" + package;
    system((command4).c_str());
    string command5="tar xvf /tmp/" + package + ".kis/" + package + ".tar.xz -C /tmp/" + package + ".kis/" + package;
    system((command5).c_str());

    string src = "/tmp/" + package + ".kis/" + package + "/src/";
    string command6 = "cd " + src + " && ls";
    string command7 = "cp /tmp/packagebuild." + package + ".kis " + src + "build.sh";
    string command8 = "sh " + src + "build.sh";
    system((command6).c_str());
    system((command7).c_str());
    system((command8).c_str());
}

void getpackagemanifest(string url, string package, bool isinstall, bool autoinstall){
    string command="wget " + url + "/" + package + ".kis -q -O -";
    system((command + " > /tmp/readpackagedist.kis").c_str());
    ifstream readdist;
    readdist.open("/tmp/readpackagedist.kis");
    string line;
    while(getline(readdist, line)){
        cout << line << endl;
        if (isinstall){
            const regex pattern("^src: (.*)$");
            if (regex_match(line, pattern) and line != "" and line != " "){
                line.replace(0,5,"");
                cout << line << endl;
                installpackage(url, package, line, autoinstall);
            }
        }
    }
}

int main(int argc, char *argv[]){
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
                        getpackagemanifest(url, argv[i], true, autorun);
                    } else {
                        cout << "ERROR, package " << argv[i] << " does not exist!" << endl;
                    }
                }
            } else {
                for (int i = 2; i < argc; ++i){
                    if(packageexists(url, argv[i])){
                        getpackagemanifest(url, argv[i], true, autorun);
                    } else {
                        cout << "ERROR, package " << argv[i] << " does not exist!" << endl;
                    }
                }
            }
        } else if (isarg1 == search){
            for (int i = 2; i < argc; ++i){
                if(packageexists(url, argv[i])){
                    getpackagemanifest(url, argv[i], false, false);
                } else {
                    cout << "ERROR, package " << argv[i] << " does not exist!" << endl;
                }
            }
        }
    } else {
        cout << "Error: Repo url" << endl;
        exit(1);
    }

    return 0;
}