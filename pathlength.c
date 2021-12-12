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

int main ()
{
  PointsVector pv = read_all_Points (stdin);
  double length = total_distance (&pv);
  printf ("%lf\n", length);
  return EXIT_SUCCESS;
}
