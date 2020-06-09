#define CATCH_CONFIG_MAIN
#include "cs221util/catch.hpp"
#include <limits.h>
#include <vector>
#include <sys/stat.h>
#include <iostream>
#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"
#include "filler.h"
#include "stripeColorPicker.h"
#include "conwayColorPicker.h"
#include "borderColorPicker.h"
#include "gridColorPicker.h"

using namespace std;
using namespace cs221util;

#define FUNCTORTESTHEIGHT 60
#define FUNCTORTESTWIDTH 60
#define FUNCTORTESTFREQ 10
#define FUNCTORTESTX 40
#define FUNCTORTESTY 10
#define FUNCTORTESTRADIUS 45
#define FUNCTORTESTGRIDSPACING 8

// use the following for border tests
#define SOLIDX 80
#define SOLIDY 80
#define SOLIDTOLERANCE 0.01
#define SOLIDTESTIMAGE "originals/Canada.png"
#define SOLIDFRAMEFREQ 100

// use the following for stripe tests
#define GRIDX 120
#define GRIDY 75
#define GRIDTOLERANCE 0.008
#define GRIDTESTIMAGE "originals/mooncake.png"
#define GRIDFRAMEFREQ 50
#define GRIDGRIDSPACING 5

// use for conway
#define RAINFREQ 1.0/1000.0
#define RAINTOLERANCE 0.05
#define RAINX 75
#define RAINY 75
#define RAINTESTIMAGE "originals/cloud.png"
#define RAINFRAMEFREQ 500

PNG testColorPicker(colorPicker& picker)
{
    PNG img;
    img.resize(FUNCTORTESTWIDTH, FUNCTORTESTHEIGHT);
    HSLAPixel px;

    for (int x = 1; x < FUNCTORTESTWIDTH; x = x + x) {
        for (int y = 1; y < FUNCTORTESTHEIGHT; y = y + y) {
            px = picker(x, y);
            // these create the output useful for debugging fills
            cout << "\toperator()(" << x << ", " << y << ") = {"
                << (int)px.h << ", "
                << (int)px.s << ", "
                << (int)px.l << "}" << endl;
        }
    }

    for (int x = 0; x < FUNCTORTESTWIDTH; x++){
        for (int y = 0; y < FUNCTORTESTHEIGHT; y++){
            HSLAPixel * p = img.getPixel(x,y);
            * p = picker(x, y);
        }
    }

    return img;
}


/*
* given as an example
*/
TEST_CASE("colorPicker::basic stripe","[weight=1][part=colorPicker]"){
    HSLAPixel px1;
    px1.h = 120;
    px1.s = 1.0; px1.l = 0.25;
    stripeColorPicker stripePicker(px1, FUNCTORTESTGRIDSPACING);

    PNG result = testColorPicker(stripePicker);
    result.writeToFile("images/stripeColorPickerTest.png");
    PNG expected;
    expected.readFromFile("soln_images/stripeColorPickerTest.png");
    REQUIRE(result == expected);
}

TEST_CASE("fill::basic stripe bfs","[weight=1][part=fill]"){
    PNG img;
    img.readFromFile(GRIDTESTIMAGE);
    HSLAPixel px(200., 1.0, 0.5);

    animation anim;
    anim = filler::fillStripeBFS(img, GRIDX, GRIDY, px, GRIDGRIDSPACING,
                                     GRIDTOLERANCE, GRIDFRAMEFREQ);
    PNG result = anim.write("images/bfsstripe.gif");
    result.writeToFile("images/bfsstripe.png");
    PNG expected; expected.readFromFile("soln_images/bfsstripe.png");
    REQUIRE(result==expected);
}

TEST_CASE("fill::basic stripe dfs","[weight=1][part=fill]"){
    PNG img;
    img.readFromFile(GRIDTESTIMAGE);
    HSLAPixel px(40., 1.0, 0.5);

    animation anim;
    anim = filler::fillStripeDFS(img, GRIDX, GRIDY, px, GRIDGRIDSPACING,
                                     GRIDTOLERANCE, GRIDFRAMEFREQ);
    PNG result = anim.write("images/dfsstripe.gif");
    result.writeToFile("images/dfsstripe.png");
    PNG expected; expected.readFromFile("soln_images/dfsstripe.png");
    REQUIRE(result==expected);
}

/*
* the students will have to implement the rest themselves
*/

TEST_CASE("colorPicker::basic border","[weight=1][part=colorPicker]"){

    PNG img;
    img.resize(FUNCTORTESTWIDTH, FUNCTORTESTHEIGHT);

    HSLAPixel px1;
    px1.h = 360.;
    px1.s = 1.0; px1.l = 0.5;
    HSLAPixel px2;

    borderColorPicker borderPicker(px1, img, 0.4, px2);

    PNG result = testColorPicker(borderPicker);
    result.writeToFile("images/borderColorPickerTest.png");
    PNG expected; expected.readFromFile("soln_images/borderColorPickerTest.png");
    REQUIRE(result == expected);
}


TEST_CASE("fill::basic border bfs","[weight=1][part=fill]"){
    PNG img;
    img.readFromFile(SOLIDTESTIMAGE);
    HSLAPixel px(200., 1.0, 0.5);
    animation anim;
    printf("PRE FX\n");
    anim = filler::fillBorderBFS(img, SOLIDX, SOLIDY, px, SOLIDTOLERANCE, SOLIDFRAMEFREQ);
    printf("post FX\n");
    PNG result = anim.write("images/bfsborder.gif");
    result.writeToFile("images/bfsborder.png");
    PNG expected; expected.readFromFile("soln_images/bfsborder.png");
    REQUIRE(result==expected);
}
TEST_CASE("fill::basic border dfs","[weight=1][part=fill]"){
    PNG img;
    img.readFromFile(SOLIDTESTIMAGE);
    HSLAPixel px(200., 1.0, 0.5);

    animation anim;
    anim = filler::fillBorderDFS(img, SOLIDX, SOLIDY, px, SOLIDTOLERANCE, SOLIDFRAMEFREQ);
    PNG result = anim.write("images/dfsborder.gif");
    result.writeToFile("images/dfsborder.png");
    PNG expected; expected.readFromFile("soln_images/dfsborder.png");
    REQUIRE(result==expected);
}
/*
* Grid color picker
*/
TEST_CASE("fill::basic grid bfs","[weight=1][part=fill]"){

    PNG img;
    img.readFromFile(RAINTESTIMAGE);
    HSLAPixel px(40., 1.0, 0.5);

    animation anim;
    anim = filler::fillGridBFS(img, GRIDX, GRIDY, px, GRIDGRIDSPACING,
                                     GRIDTOLERANCE, GRIDFRAMEFREQ);
    PNG result = anim.write("images/bfsgrid.gif");
    result.writeToFile("images/bfsgrid.png");
    PNG expected; expected.readFromFile("soln_images/bfsgrid.png");
    REQUIRE(result==expected);
}

TEST_CASE("fill::basic grid dfs","[weight=1][part=fill]"){

    PNG img;
    img.readFromFile(RAINTESTIMAGE);
    HSLAPixel px(40., 1.0, 0.5);

    animation anim;
    anim = filler::fillGridDFS(img, GRIDX, GRIDY, px, GRIDGRIDSPACING,
                                     GRIDTOLERANCE, GRIDFRAMEFREQ);
    PNG result = anim.write("images/dfsgrid.gif");
    result.writeToFile("images/dfsgrid.png");
    PNG expected; expected.readFromFile("soln_images/dfsgrid.png");
    REQUIRE(result==expected);
}


/*
* CONWAY
*/
TEST_CASE("colorPicker::basic conway","[weight=1][part=colorPicker]"){
    conwayColorPicker conwayPicker = conwayColorPicker();

    PNG result = testColorPicker(conwayPicker);
    result.writeToFile("images/conwayColorPickerTest.png");
    PNG expected; expected.readFromFile("soln_images/conwayColorPickerTest.png");
    REQUIRE(result == expected);
}

TEST_CASE("fill::basic conway bfs","[weight=1][part=fill]"){

    PNG img;
    img.readFromFile(RAINTESTIMAGE);

    animation anim;
    anim = filler::fillConwayBFS(img, RAINX, RAINY,
                                         RAINTOLERANCE,
                                         RAINFRAMEFREQ);
    PNG result = anim.write("images/bfsconway.gif");
    result.writeToFile("images/bfsconway.png");
    PNG expected; expected.readFromFile("soln_images/bfsconway.png");
    REQUIRE(result==expected);
}

TEST_CASE("fill::basic conway dfs","[weight=1][part=fill]"){

    PNG img;
    img.readFromFile(RAINTESTIMAGE);

    animation anim;
    anim = filler::fillConwayDFS(img, RAINX, RAINY,
                                         RAINTOLERANCE,
                                         RAINFRAMEFREQ);
    PNG result = anim.write("images/dfsconway.gif");
    result.writeToFile("images/dfsconway.png");
    PNG expected; expected.readFromFile("soln_images/dfsconway.png");
    REQUIRE(result==expected);
}