#include "borderColorPicker.h"

borderColorPicker::borderColorPicker(HSLAPixel fillColor, PNG & img, double tolerance,HSLAPixel center)
{   
   color = fillColor;
   im = img;
   tol = tolerance;
   ctr = center;
}

bool borderColorPicker::isInImage(int x, int y)
{
   return (x >= 0 && x < (int) im.width() && y >= 0 && y < (int) im.height());
}

bool borderColorPicker::isInFillRegion(int x, int y)
{
   HSLAPixel *pixel = im.getPixel((unsigned int)x, (unsigned int)y);
   double colorDiff = pixel->dist(ctr);
   return (colorDiff <= tol);
}

HSLAPixel borderColorPicker::operator()(int x, int y) {
   double euclidDist;

   for(int a = -3; a <= 3; a++) {
      for(int b = -3; b <= 3; b++) {
         euclidDist = a*a + b*b;
         if (euclidDist <= 9 && (!isInImage(x + a, y + b) || !isInFillRegion(x + a, y + b)))
         {
            return color;
         }
      }
   }
   return *(im.getPixel((unsigned) x, (unsigned) y));
}
