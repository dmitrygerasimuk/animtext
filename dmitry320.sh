rm -r tga/; mkdir tga; 
cd build

./animtext -i ../txt/dmitry320.txt -w 320 -h 200 -s 2 -x 65 -o ../tga/dmitry%.5d.tga
