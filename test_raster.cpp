#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include "render.h"
#include "sbuffer.h"

class RenderTest : public testing::Test {
public:
    Raster rast{16, 16};
    FrameBuffer1BPP frameBuffer{rast.sBuffer};

//   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
// 0 . . . . . . . . . . . . . . . . 0
// 1 . . . . . . . . . . . . . . . . 1
// 2 . . . . . a . . . . . . . . . . 2
// 3 . . . . . . . . . . . . . . . . 3
// 4 . . . . . . . . . . . . . d . . 4
// 5 . . . . . . . . . . . . . . . . 5
// 6 . c . . . . . . . . . . . . . . 6
// 7 . . . . . . . . . . . . . . . . 7
// 8 . . . . . . . . . . . . . . . . 8
// 9 . . . . . . . . . . . . . . . . 9
// 0 . . . . . . . . . . . . . . . . 0
// 1 . . . . . . . . . . . . . . . . 1
// 2 . . . . . . . . . . . . . . . . 2
// 3 . . . . . . . . . . . . . . . . 3
// 4 . . . . . . . . . . b . . . . . 4
// 5 . . . . . . . . . . . . . . . . 5
//   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5

    // a,b,c; a,d,b is in clockwise order
    TriVertex a{ Fix10{ 5}, Fix10{ 2} };
    TriVertex b{ Fix10{10}, Fix10{14} };
    TriVertex c{ Fix10{ 1}, Fix10{ 6} };
    TriVertex d{ Fix10{13}, Fix10{ 4} };

    // vertices Mirrorred to Top, Right, Bottom, Left edges
    TriVertex aMT{ Fix10{ 5}, Fix10{-3} };
    TriVertex aML{ Fix10{-6}, Fix10{ 2} };
    TriVertex bMB{ Fix10{10}, Fix10{17} };
    TriVertex bMR{ Fix10{21}, Fix10{14} };
    TriVertex cML{ Fix10{-2}, Fix10{ 6} };
    TriVertex cMB{ Fix10{ 1}, Fix10{25} };
    TriVertex dMR{ Fix10{18}, Fix10{ 4} };
    TriVertex dMT{ Fix10{13}, Fix10{-5} };


    bool checkBuffer(const std::string& suffix = {}) {
         std::string filename = TEST_RESOURCE_DIR + getTestName() + suffix + ".txt";

        // get expected file content
        std::string expected;
        std::getline(std::ifstream(filename), expected, '\0');

        // generate framebuffer and text representation
        frameBuffer.rasterize();
        std::ostringstream oss;
        //printBuffer(oss);
        //frameBuffer.print(oss);
        oss << frameBuffer;
        if (expected == oss.str()) {
            return true;
        }

        // handle failure
        std::string actualFilename = getTestName() + suffix + "_actual.txt";
        std::ofstream f(actualFilename);
        f << oss.str();

        std::cout << "Expected:" << std::endl << expected;
        std::cout << "Actual:" << std::endl << oss.str();
        std::cout << "SBuffer:" << std::endl << rast.sBuffer;
        return false;
    }

    std::string getTestName() {
        const testing::TestInfo* const test_info = testing::UnitTest::GetInstance()->current_test_info();
        return std::string{ test_info->test_suite_name() } + "_" + test_info->name();
    }
};


// TODO
// half pixel offset + in console app, animated
// missing top/bottom
// missing whole
// winding order
// adjacent triangles/fill convention

TEST_F(RenderTest, left) {
    rast.triangle(&a, &b, &c);
    ASSERT_TRUE(checkBuffer());
}

TEST_F(RenderTest, right) {
    rast.triangle(&a, &d, &b);
    ASSERT_TRUE(checkBuffer());
}

TEST_F(RenderTest, topClip) { // 1 vertex out
    rast.triangle(&aMT, &d, &b);
    ASSERT_TRUE(checkBuffer());
}

TEST_F(RenderTest, topClip2) { // 2 vertices out
    rast.triangle(&aMT, &dMT, &b);
    ASSERT_TRUE(checkBuffer());
}

TEST_F(RenderTest, bottomClip) { // 1 vertex out
    rast.triangle(&c, &d, &bMB);
    ASSERT_TRUE(checkBuffer());
}

TEST_F(RenderTest, bottomClip2) { // 2 vertices out
    rast.triangle(&cMB, &a, &bMB);
    ASSERT_TRUE(checkBuffer());
}

TEST_F(RenderTest, leftClip) { // 1 vertex out
    rast.triangle(&a, &b, &cML);
    ASSERT_TRUE(checkBuffer());
}

TEST_F(RenderTest, leftClip2) { // 2 vertices out
    rast.triangle(&aML, &cML, &d);
    ASSERT_TRUE(checkBuffer());
}

TEST_F(RenderTest, rightClip) { // 1 vertex out
    rast.triangle(&a, &dMR, &b);
    ASSERT_TRUE(checkBuffer());
}

TEST_F(RenderTest, rightClip2) { // 2 vertices out
    rast.triangle(&dMR, &bMR, &c);
    ASSERT_TRUE(checkBuffer());
}
