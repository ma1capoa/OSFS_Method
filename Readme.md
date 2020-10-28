The program has been made in a linux operating system (Ubuntu 18.04).
All the experiments were run using an Intel(R) Core(TM) i7-870 K CPU at 3.70GHz with 64GB of RAM

To obtain the executable program follow the following steps:
1. go to the OSFS_Code folder
2. mkdir build
3. cd buid
4. cmake ..
5. make

The OSFS_Code folder contains the TimeSeriesFiles folder with all the time series used.

To run the program with the BBVA.txt time series with a error bound guarante of 1.5:
./OSFS.x -f ../TimeSeriesFiles/BBVA.txt  -e 1.5

The output should be the following:
Results for file BBVA.txt
=================================================
Number of points of time series: 4174
RSME = 0.561235
Maximum error = 1.49606
Number of cut points = 23
Computational time (ms) = 1669.34
CUT POINTS (Index or position): 1,198,454,696,709,749,887,938,1139,2153,2352,2363,2431,2557,2559,2665,2785,2850,2986,3015,3483,4091,4174.

If you want to run the program for all time series with the error limit values used in the article, you can use the following lines:

./OSFS.x -f ../TimeSeriesFiles/HandOutlines_.txt -e 0.0185
./OSFS.x -f ../TimeSeriesFiles/MALLAT_.txt -e	0.2585
./OSFS.x -f ../TimeSeriesFiles/StarLightCurves_.txt -e 0.038
./OSFS.x -f	../TimeSeriesFiles/Donoho-Johnstone.txt -e 8.4052
./OSFS.x -f	../TimeSeriesFiles/IBEX.txt -e 891.712
./OSFS.x -f	../TimeSeriesFiles/BBVA.txt -e 1.3236
./OSFS.x -f	../TimeSeriesFiles/DEUTSCHE.txt -e 7.4668
./OSFS.x -f	../TimeSeriesFiles/SANPAOLO.txt -e 0.5272
./OSFS.x -f	../TimeSeriesFiles/SO_GENERAL.txt -e 8.8297
./OSFS.x -f	../TimeSeriesFiles/b46001f.txt -e	4.9136
./OSFS.x -f	../TimeSeriesFiles/b46075f.txt -e	3.9507
./OSFS.x -f	../TimeSeriesFiles/b41043f.txt -e	1.7515
./OSFS.x -f	../TimeSeriesFiles/b41044f.txt -e	1.4649
./OSFS.x -f	../TimeSeriesFiles/MIT_BIH_Arrhythmia_108.txt -e 0.1148



