regex='(https?|ftp|file)://[-A-Za-z0-9\+&@#/%?=~_|!:,.;]*[-A-Za-z0-9\+&@#/%=~_|]'
string='http://www.google.com/test/link.php'
if [ $string = $regex ]
then 
    echo "Link valid"
else
    echo "Link not valid"
fi