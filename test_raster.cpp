#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include "render.h"

class RenderTest : public testing::Test {
public:
    Raster rast{16, 16};

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
    TriVertex a{ Fix10{ 5*1024}, Fix10{ 2*1024} };
    TriVertex b{ Fix10{10*1024}, Fix10{14*1024} };
    TriVertex c{ Fix10{ 1*1024}, Fix10{ 6*1024} };
    TriVertex d{ Fix10{13*1024}, Fix10{ 4*1024} };

    // vertices Mirrorred to Top, Right, Bottom, Left edges
    TriVertex aMT{ Fix10{ 5*1024}, Fix10{-3*1024} };
    TriVertex aML{ Fix10{-6*1024}, Fix10{ 2*1024} };
    TriVertex bMB{ Fix10{10*1024}, Fix10{17*1024} };
    TriVertex bMR{ Fix10{21*1024}, Fix10{14*1024} };
    TriVertex cML{ Fix10{-2*1024}, Fix10{ 6*1024} };
    TriVertex cMB{ Fix10{ 1*1024}, Fix10{25*1024} };
    TriVertex dMR{ Fix10{18*1024}, Fix10{ 4*1024} };
    TriVertex dMT{ Fix10{13*1024}, Fix10{-5*1024} };


    void printBuffer(std::ostream& os = std::cout){
        for(int y=0; y<16; y++) {
            for(int x=0; x<16; x++) {
                char pix = rast.frameBuffer[y*16+x] ? '#' : '.';
                 os << pix << " ";
            }
            os << std::endl;
        }
    }
#if 1
    bool checkBuffer(const std::string& suffix = {}) {
        std::string filename = "../test_resources/" + getTestName() + suffix + ".txt";

        // get expected file content
        std::string expected;
        std::getline(std::ifstream(filename), expected, '\0');

        // generate framebuffer text representation
        std::ostringstream oss;
        printBuffer(oss);
        if (expected == oss.str()) {
            return true;
        }

        // handle failure
        std::string actualFilename = getTestName() + suffix + "_actual.txt";
        std::ofstream f(actualFilename);
        f << oss.str();

        std::cout << "Expected:" << std::endl << expected;
        std::cout << "Actual:" << std::endl << oss.str();
        return false;
    }
#else
    bool checkBuffer(const std::string& suffix = {}) {
        std::string filename = "../test_resources/" + getTestName() + suffix + ".txt";

        // get expected file content
        std::string expected;
        std::getline(std::ifstream(filename), expected, '\0');

        // generate framebuffer text representation
        std::ostringstream oss;
        printBuffer(oss);
        if (expected == oss.str()) {
            return true;
        }

        // handle failure
        std::string actualFilename = getTestName() + suffix + "_actual.txt";
        std::ofstream f(actualFilename);
        f << oss.str();

        std::cout << "Expected:" << std::endl << expected;
        std::cout << "Actual:" << std::endl << oss.str();
        return false;
    }
#endif
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
