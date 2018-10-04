1. Compile:
  To Compile this binary, run "make"

2. Clean:
  To clean the binary and .o files, run "make clean"

3. Run:
  To run the program, please follow below steps:
    A. To run 2D algorithm with Brute-Force:
        Please disable #define DIMENSION_3D 0 macro to 0 and re-Compile the code then input 2D coordinates as shown in the sample inputFile.txt separated with one white space. The output shall go in outputFile.txt where you'd see output like this:

              Algorithm-D&C-2D:
              0 1 1.414214
              execution time: 0.005542
              #################################

              Brute-force-2D:
              0 1 1.414214
              execution time: 0.328924
        Here first block shows the D&C conquer algorithm and point indexes, min distance and the execution time. Second block shows same for Brute-force method.

    B. To run 3D algorithm with Brute-Force:
    Please enable #define DIMENSION_3D 1 macro to 0 and re-Compile the code then input 3d coordinates as shown in the sample inputFile.txt separated with one white space. The output shall go in outputFile.txt where you'd see output like this:

              Algorithm-D&C-3D:
              0 1 1.732051
              execution time: 0.001751
              #################################

              Brute-force-3D:
              0 1 1.732051
              execution time: 0.397829

        Here first block shows the D&C conquer algorithm and point indexes, min distance and the execution time. Second block shows same for Brute-force method.
