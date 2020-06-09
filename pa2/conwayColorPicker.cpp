#include "conwayColorPicker.h"
#include <math.h>

using std::pair;
using std::map;

conwayColorPicker::conwayColorPicker() : iter(0)
{
}
/**
 *  The function below returns either a black or a white HSLA pixels.
 *  The logic for determining whether a pixel is black or white is abstracted
 *  away in nextColor() which is given to you.
 * 
 *  However, nextColor() requires the number of WHITE pixels (representing dead cells)
 *  so your job here to figure out how to do an 8 point search around a pixel, eg
 *      NW  | North| NE
 *      West| (x,y)| East
 *      SW  | South| SE 
 *  All pixels around an initial (x,y) pixel
 * 
 *  HINT: in this filler, you will have to exploit and update the PNGmap field accordingly.
 *          it will also be useful if you look up the documentation for a C++ Map
 * 
 */
HSLAPixel conwayColorPicker::operator()(int x, int y)
{
    PNGmap::iterator stored;
    vector<PNGmap::iterator> neighbours;
    

    int deadNeighbourCount = 0;

    for (int i = x - 1; i <= x + 1; i++)
    {
        for (int j = y - 1; j <= y + 1; j++)
        {
            if (i == x && j == y)
            {
                continue;
            }
            pair<int, int> p = make_pair(i, j);
            
            if (prev.find(p) != prev.end()) {
                neighbours.push_back(prev.find(p));
            }
        }
    }

    for (int i = 0; i < (int) neighbours.size(); i++)
    {
        HSLAPixel pixel = neighbours[i]->second;
        if (pixel.l == 1)
        {
            deadNeighbourCount++;
        }
    }

    HSLAPixel retPixel = nextColor(deadNeighbourCount);

    prev[pair<int, int>(x, y)] = retPixel;

    return retPixel;
}


/**
 * GIVEN FUNCTION
 * 
 * for those interested, I did my best to mimic Conway's Game of Life in a colourpicker,
 * constrained to the 8 pixels and the propogating nature of a fill. See
 * https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
 * 
 * John Horton Conway, am active mathematician in the fields of number theory, 
 * combinatorial game theory and coding theory, died recently of COVID 19 complications.
*/
HSLAPixel conwayColorPicker::nextColor(int deadNeighbourCount)
{
    // extra variables for readability
    HSLAPixel alive = HSLAPixel(0, 0, 0, 1); // black means alive, true to the game
    HSLAPixel dead = HSLAPixel(0, 0, 1, 1);  // white means alive, true to the game

    HSLAPixel ret = dead;

    if (deadNeighbourCount == 2) 
    {
        return alive;
    }

    if (deadNeighbourCount > 2)
    { // possibility of death by population if more than 3 neighbours
        if (iter % deadNeighbourCount == 0)
        { // chance to be alive decreases with deadNeighbourCount
            ret = alive;
        }
        else
        {
            ret = dead;
        }
    }
    iter++;
    return ret;
}