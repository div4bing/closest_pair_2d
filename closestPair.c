#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DIMENSION_3D 1	  // Enable to have 3D closest pair of points, If 0 - means input is 2D

// Global declaration

#if DIMENSION_3D	 // 3-Dimensional Enabled
	struct point_t      // 3-Dimensional Coordinate Point
	{
	  long long x, y, z;
	};
#else
	struct point_t
	{
	  long long x, y;
	};
#endif

typedef struct point_t point;

float calculateDistance(point a, point b);
int qsortCompX (const void *arg1, const void *arg2);
int qsortCompY (const void *arg1, const void *arg2);
#if DIMENSION_3D	 // 3-Dimensional Enabled
	int qsortCompZ (const void *arg1, const void *arg2);
#endif
float findClosestPair(point p[], long long size, point *A, point *B);
float findMin(float L, float R, int *findMinFlag);
float findMinInCenter(point p[], long long mid, float dminLR, long long size, point *A, point *B);
long long getTotalLines(FILE *fp);
long long totalLines;
#if DIMENSION_3D	 // 3-Dimensional Enabled
long long searchIndex(point p[], long long size, long long valX, long long valY, long long valZ);
#else
long long searchIndex(point p[], long long size, long long valX, long long valY);
#endif

//**************** Print Point Values for Debug *******************
void printPoints(point p[], long long size);
void printPoints(point p[], long long size)
{
  printf("Print Points: Size: %lld\n", size);
  for(long long i = 0; i< size; i++)
  {
#if DIMENSION_3D	 // 3-Dimensional Enabled
    printf("(%lld, %lld, %lld)\n", p[i].x, p[i].y, p[i].z);
#else
    printf("(%lld, %lld)\n", p[i].x, p[i].y);
#endif
  }
}
//*************************************************

float findMin(float L, float R, int *findMinFlag)
{
  if(L <= R)
  {
    *findMinFlag = 0;   // Tell that the left is small or equal
    return L;
  }
  else
  {
    *findMinFlag = 1;   // Tell that the right is small
    return R;
  }
}

float calculateDistance(point a, point b)
{
#if DIMENSION_3D	 // 3-Dimensional Enabled
  return sqrt(((a.x - b.x)*(a.x - b.x)) + ((a.y - b.y)*(a.y - b.y)) + ((a.z - b.z)*(a.z - b.z)));
#else
  return sqrt(((a.x - b.x)*(a.x - b.x))+((a.y - b.y)*(a.y - b.y)));
#endif
}

//qsort(p, size, sizeof( point), qsortCompX);
int qsortCompX (const void *arg1, const void *arg2)
{
   return((int)(( point *)arg1)->x - (( point *)arg2)->x);
}

//qsort(p, size, sizeof( point), qsortCompY);
int qsortCompY (const void *arg1, const void *arg2)
{
   return((int)(( point *)arg1)->y - (( point *)arg2)->y);
}

#if DIMENSION_3D	 // 3-Dimensional Enabled
//qsort(p, size, sizeof( point), qsortCompZ);
int qsortCompZ (const void *arg1, const void *arg2)
{
   return((int)(( point *)arg1)->z - (( point *)arg2)->z);
}
#endif

float findMinInCenter(point p[], long long mid, float dminLR, long long size, point *A, point *B)
{
  point pCenter[65535]; //TODO: Remove magic number
  long long count = 0;
  long long sizeCenter = 0;
  long long i, j;

  float dminCenter = dminLR;
  float tempDist = 0.0;

  for(i=0; i < size; i++)
  {
    if(abs(p[i].x - p[mid].x) < dminLR)
    {
      pCenter[count++] = p[i];
    }
  }

  sizeCenter = count;

#if DIMENSION_3D	 // 3-Dimensional Enabled
  qsort(pCenter, sizeCenter, sizeof( point), qsortCompZ);   // Sort the center part with Z- coordinate
#else
  qsort(pCenter, sizeCenter, sizeof( point), qsortCompY);   // Sort the center part with Y- coordinate
#endif

  // We will only try to find the points from particular point which is less than dminCenter (the rectangle)
  for (i = 0; i < sizeCenter; i++)
  {
#if DIMENSION_3D	 // 3-Dimensional Enabled
    for (j = i+1; (j < sizeCenter) && ((pCenter[j].z - pCenter[i].z) < dminCenter); j++)
#else
    for (j = i+1; (j < sizeCenter) && ((pCenter[j].y - pCenter[i].y) < dminCenter); j++)
#endif
    {
      tempDist = calculateDistance(pCenter[i], pCenter[j]);
      if (tempDist < dminCenter)
      {
        dminCenter = tempDist;
        *A = pCenter[i];
        *B = pCenter[j];
      }
    }
  }

  return dminCenter;
}


float findClosestPair(point p[], long long size, point *A, point *B)
{
  float dminL = 0.0;    // Min distance in Left part
  float dminR = 0.0;    // Min distance in Right part
  float dminLR = 0.0;   // Min distance between Left and Right part
  float dminAll = 0.0;  // Overall Minimum distance
  int findMinFlag = 0;  // 0- Left is small and 1- right is small

  point La, Lb, Ra, Rb, LRa, LRb;   // La & Lb - Smallest point on Left side, Ra & Rb - Smallest point on right side

  if (size <= 3)
  {
    float dmin = calculateDistance(p[0], p[1]);     // Get initial min Distance
    float tempDist = 0.0;

    *A = p[0];          // Updte closest point every time we find a dmin
    *B = p[1];

    for (long long i=1; i< size; i++)
    {
      for (long long j=i+1; j < size; j++)
      {
        tempDist = calculateDistance(p[i], p[j]);
        if (tempDist < dmin)
        {
          dmin = tempDist;
          *A = p[i];
          *B = p[j];
        }
      }
    }
    return dmin;
  }

  long long mid = size/2;

  dminL = findClosestPair(p, mid, &La, &Lb);
  dminR = findClosestPair(p+mid, size-mid, &Ra, &Rb);

  dminLR = findMin(dminL, dminR, &findMinFlag);

  if(findMinFlag == 0)  // left is small
  {
    LRa = La;
    LRb = Lb;
  }
  else        // right is small
  {
    LRa = Ra;
    LRb = Rb;
  }
  findMinFlag = 0;  // Reset the flag

  dminAll = findMin (dminLR, findMinInCenter(p, mid, dminLR, size, A, B), &findMinFlag);

  if(findMinFlag == 0)  // left is small. If right is small we already have A and B loaded
  {
    *A = LRa;
    *B = LRb;
  }
  return dminAll;
}

long long getTotalLines(FILE *fp)
{
  long long totalLines = 0;
  char string[100];

  while(!feof(fp)) {
    fgets(string, 100, fp);
    totalLines++;
  }

  totalLines--;

  if(fseek(fp, 0L, SEEK_SET) == EOF) {
    perror("Error while seeking to begining of file");
    exit(0);
  }

  return totalLines;
}

#if DIMENSION_3D	 // 3-Dimensional Enabled
long long searchIndex(point p[], long long size, long long valX, long long valY, long long valZ)
#else
long long searchIndex(point p[], long long size, long long valX, long long valY)
#endif
{
  long long i =0;

  for(i = 0; i < size; i++)
  {
#if DIMENSION_3D	 // 3-Dimensional Enabled
    if (valX == p[i].x && valY == p[i].y && valZ == p[i].z)
#else
    if (valX == p[i].x && valY == p[i].y)
#endif
    {
      return i;
    }
  }

  return -1;
}


int main (int argc, char *argv[])
{
  FILE *inputFD;
  FILE *outputFD;
  char string[100];
  point A, B;         // Closest Points
  float minDistance = 0.0;

  if (argc != 3)                                                                // Make sure the number of input is correct
  {
    printf("Error! Invalid number of Arguments. Please run program as ./submission inputFile.txt outputFile.txt\n");
    return -1;
  }

  inputFD = fopen(argv[1], "r");                                                // Open file for Reading the input
  if (inputFD == NULL)
  {
    perror("Error opening the input file");
    return -1;
  }

  totalLines = getTotalLines(inputFD);

  long long size = totalLines;
  point *p = malloc(totalLines * sizeof(point));
  point *pOrg = malloc(totalLines * sizeof(point));

  for (long long i=0; i<size; i++)
  {
    fgets(string, 100, inputFD);
#if DIMENSION_3D	 // 3-Dimensional Enabled
    sscanf(string, "%lld %lld %lld", &(p[i].x), &(p[i].y), &(p[i].z));
    sscanf(string, "%lld %lld %lld", &(pOrg[i].x), &(pOrg[i].y), &(pOrg[i].z));
#else
    sscanf(string, "%lld %lld", &(p[i].x), &(p[i].y));
    sscanf(string, "%lld %lld", &(pOrg[i].x), &(pOrg[i].y));
#endif
  }

  fclose(inputFD);

  outputFD = fopen(argv[2], "w");                                               // Open file for Writing the output
  if (outputFD == NULL)
  {
    perror("Error opening the ouput file");
    return -1;
  }

#if 0                    // Enable to see input
  printf("Input with Size: %lld -> [ ", size);
  for (long long i=0; i< size; i++)
#if DIMENSION_3D	 // 3-Dimensional Enabled
    printf("(%lld, %lld, %lld),", p[i].x, p[i].y, p[i].z);
#else
    printf("(%lld, %lld),", p[i].x, p[i].y);
#endif
  printf(" ]\n");
#endif

  qsort(p, size, sizeof( point), qsortCompX);         // Sort the array with respect to X - coordinate
  minDistance = findClosestPair(p, size, &A, &B);

#if DIMENSION_3D	 // 3-Dimensional Enabled
  fprintf(outputFD, "%lld %lld %f\n", searchIndex(pOrg, size, A.x, A.y, A.z), searchIndex(pOrg, size, B.x, B.y, B.z), minDistance);       // Write the output to the file
#else
  fprintf(outputFD, "%lld %lld %f\n", searchIndex(pOrg, size, A.x, A.y), searchIndex(pOrg, size, B.x, B.y), minDistance);       // Write the output to the file
#endif

  fclose(outputFD);
  free(p);
  return 0;
}
