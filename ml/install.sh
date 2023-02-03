gcc main.c ./options/help.c ./options/long.c ./options/all.c tools.c ./options/format.c ./options/ctime_sort.c  -o ml
sudo ln -s $(pwd)/ml /usr/bin/ml
echo "install done."