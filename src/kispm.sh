# The 
if [ "$(id -u)" != "0" ]; then
    echo "ERROR: Must be root user to run this script" 1>&2
    exit 1
fi
export autorun="no"

usage (){
    echo "ERROR: Invalid topion.
USAGE: $0 [-s,-i][-y] packagename"
    echo "-s: Search for package info"
    echo "-i: Install package"
    echo "-y: Automatically run install"
    exit 1
}

install (){
    echo "Installing ${package}"
    while read repo
    do
        export url=$(perl -lne 'print "$1" if /^url: (.*)/' /usr/local/kis/repo.kis)
    done < /usr/local/kis/repo.kis
    export dist=$(wget ${url}/pycalc.kis -q -O -)

    export regex='(https?|ftp|file)://[-A-Za-z0-9\+&@#/%?=~_|!:,.;]*[-A-Za-z0-9\+&@#/%=~_|]'
    
    for item in ${dist}
    do
        if [ "${item}" = "${regex}" ]; then
            echo "${item}"
        fi
    done
}

search (){
    echo "Searching for ${package}"
}

while getopts 'i:s:h:y:' arg
do
    case $arg in
        i) export package=$OPTARG && install;;
        s) export package=$OPTARG && search;;
        y) export autorun="yes";;
        h) usage ;;
        *) usage ;;
    esac
done


# Read for pkgname in dist.kis from repo
# Read for pkgname.kis in repo
#   - Read and test for arch
#   - Read for download link
#   - Download in /tmp/.kipsm
#   - Extract it (tar.xz are valid file types)
#   - Run build script