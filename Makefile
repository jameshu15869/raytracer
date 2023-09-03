CC=g++
MPICC?=mpicxx
CFLAGS=-I.

run: main.cpp ./*.h/
	$(CC) -o ./build/main main.cpp ./*.h
	./build/main > ./build/image.ppm

mpi: ./mpi/mpi_scene.cpp *.h ./mpi/*.h
	$(MPICC) -I./ -o ./build/mpi_scene ./mpi/mpi_scene.cpp 
	mpirun -n 4 ./build/mpi_scene > ./build/mpi.ppm
