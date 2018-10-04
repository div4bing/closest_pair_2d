#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DEBUG_ENABLE 0    // Enable or Disable the Debug logs

// Global declaration
struct point_t
{
  long long x, y;
};

typedef struct point_t point;

float calculateDistance(point a, point b);
int qsortCompX (const void *arg1, const void *arg2);
int qsortCompY (const void *arg1, const void *arg2);
float findClosestPair(point p[], long long size, point *A, point *B);
float findMin(float L, float R, int *findMinFlag);
float findMinInCenter(point p[], long long mid, float dminLR, long long size, point *A, point *B);
long long getTotalLines(FILE *fp);
long long totalLines;

//**************** Print Point Values for Debug *******************
void printPoints(point p[], long long size);
void printPoints(point p[], long long size)
{
  printf("Print Points: Size: %lld\n", size);
  for(long long i = 0; i< size; i++)
  {
    printf("(%lld, %lld)\n", p[i].x, p[i].y);
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
  return sqrt(((a.x - b.x)*(a.x - b.x))+((a.y - b.y)*(a.y - b.y)));
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

  qsort(pCenter, sizeCenter, sizeof( point), qsortCompY);   // Sort the center part with Y- coordinate

#if DEBUG_ENABLE                    // Print Only if Debug enabled
  printf("After Y Sort: dminCenter-> %f\n", dminCenter);
  for(i = 0; i< sizeCenter; i++)
  {
    printf("(%lld, %lld)\n", pCenter[i].x, pCenter[i].y);
  }
#endif

  for (i = 0; i < sizeCenter; i++)
  {
    for (j = i+1; (j < sizeCenter) && ((pCenter[j].y - pCenter[i].y) < dminCenter); j++)
    {
      tempDist = calculateDistance(pCenter[i], pCenter[j]);
      if (tempDist < dminCenter)
      {
        dminCenter = tempDist;
        *A = pCenter[i];
        *B = pCenter[j];
#if DEBUG_ENABLE                    // Print Only if Debug enabled
        printf("PRINT-1: \n");
        printPoints(A, 1);
        printPoints(B, 1);
#endif
      }
    }
  }

#if DEBUG_ENABLE                    // Print Only if Debug enabled
  printf("DIST: %f\n", dminCenter);
#endif
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

#if DEBUG_ENABLE                    // Print Only if Debug enabled
  printf("findClosestPair: Size: %lld\n", size);
  for(long long i = 0; i< size; i++)
  {
    printf("(%lld, %lld)\n", p[i].x, p[i].y);
  }
#endif

  if (size <= 3)
  {
    float dmin = calculateDistance(p[0], p[1]);     // Get initial min Distance
    float tempDist = 0.0;

    *A = p[0];          // Updte closest point every time we find a dmin
    *B = p[1];

#if DEBUG_ENABLE                    // Print Only if Debug enabled
    printf("PRINT-2: \n");
    printPoints(A, 1);
    printPoints(B, 1);
#endif

#if DEBUG_ENABLE                    // Print Only if Debug enabled
    printf("BruteDist: %f\n", dmin);
#endif

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

#if DEBUG_ENABLE                    // Print Only if Debug enabled
          printf("PRINT-3: \n");
          printPoints(A, 1);
          printPoints(B, 1);
#endif
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

#if DEBUG_ENABLE                    // Print Only if Debug enabled
    printf("PRINT-4: \n");
    printPoints(&LRa, 1);
    printPoints(&LRb, 1);
#endif
  }
  else        // right is small
  {
    LRa = Ra;
    LRb = Rb;

#if DEBUG_ENABLE                    // Print Only if Debug enabled
    printf("PRINT-5: \n");
    printPoints(&LRa, 1);
    printPoints(&LRb, 1);
#endif
  }
  findMinFlag = 0;  // Reset the flag

#if DEBUG_ENABLE                    // Print Only if Debug enabled
  printf("dminL: %f dminR: %f dminLR: %f\n", dminL, dminR, dminLR);
#endif

  dminAll = findMin (dminLR, findMinInCenter(p, mid, dminLR, size, A, B), &findMinFlag);

  if(findMinFlag == 0)  // left is small. If right is small we already have A and B loaded
  {
    *A = LRa;
    *B = LRb;

#if DEBUG_ENABLE                    // Print Only if Debug enabled
    printf("PRINT-6: \n");
    printPoints(A, 1);
    printPoints(B, 1);
#endif
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

  for (long long i=0; i<size; i++)
  {
    fgets(string, 100, inputFD);
    sscanf(string, "%lld %lld", &(p[i].x), &(p[i].y));
  }

  fclose(inputFD);

  outputFD = fopen(argv[2], "w");                                               // Open file for Writing the output
  if (outputFD == NULL)
  {
    perror("Error opening the ouput file");
    return -1;
  }

#if DEBUG_ENABLE                    // Print Only if Debug enabled
  printf("Input with Size: %lld -> [ ", size);
  for (long long i=0; i< size; i++)
    printf("(%lld, %lld),", p[i].x, p[i].y);
  printf(" ]\n");
#endif

  qsort(p, size, sizeof( point), qsortCompX);         // Sort the array with respect to X - coordinate

#if DEBUG_ENABLE                    // Print Only if Debug enabled
  printf("After X Sort\n");
  for(long long i = 0; i< size; i++)
  {
    printf("(%lld, %lld)\n", p[i].x, p[i].y);
  }
#endif

  minDistance = findClosestPair(p, size, &A, &B);

  fprintf(outputFD, "(%lld, %lld) (%lld, %lld) %f\n", A.x, A.y, B.x, B.y, minDistance);       // Write the output to the file

  fclose(outputFD);
  free(p);
  return 0;
}
