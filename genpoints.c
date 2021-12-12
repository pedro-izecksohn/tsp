#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Point
{
  int x;
  int y;
} Point;

Point *new_Point (int x, int y)
{
  Point *ret = malloc (sizeof(Point));
  if (!ret)
  {
    perror ("malloc returned NULL");
    exit (EXIT_FAILURE);
  }
  ret->x = x;
  ret->y = y;
  return ret;
}

typedef struct PointsVector
{
  size_t n;
  Point **array;
} PointsVector;

bool insert_uniq_PointsVector_Point (PointsVector *pv, Point *p)
{
  size_t u = 0;
  while (u<pv->n)
  {
    if ((p->x==pv->array[u]->x)&&(p->y==pv->array[u]->y))
    {
      return false;
    }
    u++;
  }
  pv->array = realloc (pv->array, sizeof(Point*)*(pv->n+1));
  if (!pv->array)
  {
    perror ("realloc returned NULL");
    exit (EXIT_FAILURE);
  }
  pv->array[pv->n]=p;
  pv->n++;
  return true;
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

int main (int argc, char **argv)
{
  int width = atoi (argv[1]);
  int height = atoi (argv[2]);
  int npoints = atoi (argv[3]);

  if ((width%2)==1)
  {
    width--;
  }
  if ((height%2)==1)
  {
    height--;
  }
  int half_width = width/2;
  int half_height = height/2;
  PointsVector pv;
  pv.n = 0;
  pv.array = malloc (0);
  while (pv.n<npoints)
  {
    Point *p = new_Point ((rand()%width)-half_width, (rand()%height)-half_height);
    if (false == insert_uniq_PointsVector_Point (&pv, p))
    {
      free (p);
    }
  }
  print_PointsVector (&pv);
  return EXIT_SUCCESS;
}
