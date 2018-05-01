rm tga/*.tga 
cp "$1" build/font.fot
cd build
./animtext -i ../txt/test.txt -w 640 -h 480 -s 2 --dry
cd $OLDPWD
