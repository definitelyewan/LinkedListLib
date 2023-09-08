
cmake -S . -B ./build -DBUILD_TESTS=ON
cd build/
make
#valgrind --leak-check=full ./main
cd test
valgrind --leak-check=full ./LinkedList_test