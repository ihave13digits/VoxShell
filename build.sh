clear
echo Compiling project....
cd build
cmake -G "Unix Makefiles" .
make && 
(
    cd ../
    ./Vox
) ||
(
    echo Ooops, something failed. See above.
)