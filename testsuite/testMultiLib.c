/* cos example */
#include <math.h>       /* cos */
#include <stdio.h>

#define PI 3.14159265

int main ()
{
  double param, result;
  param = 60.0;
  printf("%f\n", param);
  result = cos ( param * PI / 180.0 );
  printf ("The cosine of %f degrees is %f.\n", param, result );
  return 0;
}
