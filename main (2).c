

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "leak_detector_c.h"

int trainerX;
int trainerY;

struct point {
  int x;
  int y;
};

void ReadData(struct point monsterPosition[], struct point queries[], int n,
              int s, int t);
void sort(struct point arr[], int length, int t);
void mergeSort(struct point arr[], int l, int r, int t);
void merge(struct point arr[], int l, int m, int r);
int compareTo(struct point *Pt1, struct point *Pt2);
void insertionSort(struct point arr[], int l, int r);
int binarySearch(struct point arr1[], struct point *Pt, int length1);

int main() {
  atexit(report_mem_leak);

  int n; // number of monsters, also length
  int s; // number of queries
  int t; // threshold to be used for determing type of sort

  scanf("%d %d %d %d %d", &trainerX, &trainerY, &n, &s, &t);
  struct point *listofMonsterLocation = malloc(sizeof(struct point) * n);
  struct point *queries = malloc(sizeof(struct point) * s);

  ReadData(listofMonsterLocation, queries, n, s, t);

  // sort points of monsters
  sort(listofMonsterLocation, n, t);

  // input for queries:
  for (int i = 0; i < s; i++) {
    scanf("%d %d", &queries[i].x, &queries[i].y);
  }

  FILE *fptr = fopen("sample.txt", "a");

  // display sorted list of monster location
  for (int i = 0; i < n; i++) {
    printf("%d %d\n", listofMonsterLocation[i].x, listofMonsterLocation[i].y);
    fprintf(fptr, "%d %d\n", listofMonsterLocation[i].x,
            listofMonsterLocation[i].y);
  }

  // Find Queries in monster points lists
  for (int j = 0; j < s; j++) {
    int rank = binarySearch(listofMonsterLocation, &queries[j], n - 1);
    if (rank >= 0) {
      // check if it has same coordinates
      printf("%d %d found at rank %d\n", queries[j].x, queries[j].y, rank + 1);
      fprintf(fptr, "%d %d found at rank %d\n", queries[j].x, queries[j].y,
              rank + 1);
    } else {
      printf("%d %d not found\n", queries[j].x, queries[j].y);
      fprintf(fptr, "%d %d not found\n", queries[j].x, queries[j].y);
    }
  }

  fclose(fptr);
  free(listofMonsterLocation);
  free(queries);

  return 0;
}

void ReadData(struct point *monsterPosition, struct point *queries, int n,
              int s, int t) {
  // read required input
  for (int i = 0; i < n; i++) {
    scanf("%d %d", &monsterPosition[i].x, &monsterPosition[i].y);
  }
}

void sort(struct point arr[], int length, int t) {
  // set left and right indices
  int l = 0;
  int r = length - 1;

  mergeSort(arr, l, r, t);
}

void mergeSort(struct point arr[], int l, int r, int t) {

  if (r - l <= t)
    insertionSort(arr, l, r);
  else {
    int mid = (l + r) / 2;
    mergeSort(arr, l, mid, t);
    mergeSort(arr, mid + 1, r, t);
    merge(arr, l, mid, r);
  }
}

void merge(struct point arr[], int l, int m, int r) {
  int i, j, k, result;
  int n1 = m - l + 1;
  int n2 = r - m;

  /* create temp arrays */
  struct point *L = malloc(n1 * sizeof(struct point));
  struct point *R = malloc(n2 * sizeof(struct point));

  /* Copy data to temp arrays L[] and R[] */
  for (i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[m + 1 + j];

  /* Merge the temp arrays back into arr[l..r]*/
  i = 0; // Initial index of first subarray
  j = 0; // Initial index of second subarray
  k = l; // Initial index of merged subarray
  while (i < n1 && j < n2) {
    // if L[i] closer to me than  R[j]
    result = compareTo(&L[i], &R[j]);
    if (result < 0) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  /* Copy the remaining elements of L[], if there
    are any */
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }
  /* Copy the remaining elements of R[], if there
     are any */
  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }

  free(L);
  free(R);
}

void insertionSort(struct point arr[], int l, int r) {
  int i, j;
  struct point myPoint;
  int result;
  for (i = l + 1; i <= r; i++) {
    myPoint = arr[i];

    for (j = i - 1; j >= l; j--) {
      // if my point is closer to me than prev point, shift it forward
      result = compareTo(&arr[j], &myPoint);
      if (result > 0)
        arr[j + 1] = arr[j];
      else
        break;
    }
    arr[j + 1] = myPoint;
  }
}

int compareTo(struct point *Pt1, struct point *Pt2) {
  int distance1;
  int distance2;

  distance1 = (((Pt1->x) - (trainerX)) * ((Pt1->x) - (trainerX))) +
              (((Pt1->y) - (trainerY)) * ((Pt1->y) - (trainerY)));
  distance2 = (((Pt2->x) - (trainerX)) * ((Pt2->x) - (trainerX))) +
              (((Pt2->y) - (trainerY)) * ((Pt2->y) - (trainerY)));

  if (distance1 != distance2)
    return distance1 - distance2;
  if (Pt1->x != Pt2->x)
    return Pt1->x - Pt2->x;
  if (Pt1->y != Pt2->y)
    return Pt1->y - Pt2->y;

  return 0;
}

int binarySearch(struct point arr1[], struct point *Pt, int length1) {

  int low = 0, high = length1, mid;

  while (low <= high) {
    mid = (high + low) / 2;

    if (compareTo(&arr1[mid], Pt) == 0)
      return mid;
    if (compareTo(&arr1[mid], Pt) > 0)
      high = mid - 1;
    else
      low = mid + 1;
  }
  return -1;
}
