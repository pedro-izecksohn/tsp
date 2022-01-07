/* tsp2g.c - Travelling Salesman Program Second Generation.
   By: Pedro Izecksohn, izecksohn@yahoo.com
   License: 1) No warranty in given.
            2) You may use this and or modify this and or distribute this at your own risk.
            3) You should know that I wrote a similar program for Google more than 10 years ago
            and our contract expired more than 5 years ago.
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Point
{
  int x;
  int y;
} Point;

Point *read_Point (FILE *in)
{
  Point *ret = malloc (sizeof(Point));
  if (!ret)
  {
    perror("malloc");
    exit (EXIT_FAILURE);
  }
  if (fscanf (in, "%d,%d", &ret->x, &ret->y)!=2)
  {
    free (ret);
    return NULL;
  }
  return ret;
}

typedef struct PointsVector
{
  size_t n;
  Point **array;
} PointsVector;

PointsVector read_all_Points (FILE *in)
{
  PointsVector ret;
  ret.n = 0;
  ret.array = malloc (0);
  Point *p;
  while ((p = read_Point (in)))
  {
    ret.n++;
    ret.array = realloc (ret.array, sizeof(Point*)*ret.n);
    if (NULL==ret.array)
    {
      perror ("realloc");
      exit (EXIT_FAILURE);
    }
    ret.array[ret.n-1] = p;
  }
  if (ferror(in))
  {
    perror ("ferror(in)");
    exit (EXIT_FAILURE);
  }
  return ret;
}

void print_PointsVector (PointsVector *pv)
{
  size_t u = 0;
  while (u<pv->n)
  {
    Point *p = pv->array[u];
    printf ("%d,%d\n",p->x,p->y);
    ++u;
  }
}

PointsVector copy_PointsVector (PointsVector *orig)
{
  PointsVector ret;
  size_t u = 0;
  ret.n = orig->n;
  ret.array = malloc (ret.n*sizeof(Point*));
  if (NULL==ret.array)
  {
    perror("malloc");
    exit (EXIT_FAILURE);
  }
  for (;u<ret.n;u++)
  {
    ret.array[u] = orig->array[u];
  }
  return ret;
}

void remove_PointsVector_index (PointsVector *pv, size_t index)
{
  for (;index<(pv->n-1); index++)
  {
    pv->array[index] = pv->array[index+1];
  }
  pv->n--;
}

double distance (Point *a, Point *b)
{
  long int xd = a->x-b->x;
  long int yd = a->y-b->y;
  return sqrt((xd*xd)+(yd*yd));
}

double total_distance (PointsVector *pv)
{
  double ret = 0.0;
  size_t u = 0;
  for (;u<(pv->n-1);u++)
  {
    double d = distance (pv->array[u], pv->array[u+1]);
    //printf ("d = %lf\n",d);
    ret += d;
  }
  double d = distance (pv->array[pv->n-1],pv->array[0]);
  //printf ("d = %lf\n",d);
  ret += d;
  return ret;
}

int main (void)
{
  PointsVector original = read_all_Points (stdin);
  if (original.n<4)
  {
    print_PointsVector (&original);
    return EXIT_SUCCESS;
  }
  const double orig_dist = total_distance (&original);
  //printf ("orig_dist = %lf\n", orig_dist);
  PointsVector copy = copy_PointsVector (&original);
  PointsVector sorted;
  sorted.n = original.n;
  sorted.array = malloc (sizeof(Point*)*sorted.n);
  sorted.array[0] = copy.array[0];
  remove_PointsVector_index (&copy, 0);
  size_t u = 1;
  for (;u<sorted.n;u++)
  {
    double d = INFINITY;
    size_t tbr = 0;
    size_t w = 0;
    for (; w<copy.n; w++)
    {
      double cd = distance (sorted.array[u-1], copy.array[w]);
      if (cd < d)
      {
        d = cd;
        tbr = w;
      }
    }
    sorted.array[u] = copy.array[tbr];
    remove_PointsVector_index (&copy, tbr);
  }
  //printf ("copy.n = %lu\n", copy.n);
  free (copy.array);
  //copy.n = 0;
  const double sorted_dist = total_distance (&sorted);
  //printf ("sorted_dist = %lf\n", sorted_dist);
  if (sorted_dist<orig_dist)
  {
    //printf ("sorted_dist = %lf\n", sorted_dist);
    free (original.array);
    original.array = sorted.array;
  }
  else
  {
    free (sorted.array);
  }
  print_PointsVector (&original);
  return EXIT_SUCCESS;
}
