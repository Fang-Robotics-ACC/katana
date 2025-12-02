#include <opencv2/core/base.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/utility.hpp>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <array>

using Table256 = std::array<uint8_t, 256>;

int reduceColor(int color, int spaceSize)
{
    const int kReducedIndex{color / spaceSize} ;
    const int kRescaled{kReducedIndex * spaceSize};
    return kRescaled;
}

Table256 generateColorReductionTable256(int spaceSize)
{
    Table256 reductionTable{};

    for(int i{0}; i < 256; ++i)
    {
        reductionTable[i] = reduceColor(i, spaceSize);
    }

    return std::move(reductionTable);
}

cv::Mat& scanContinuousImageAndReduceColor(cv::Mat& I, const Table256& table)
{

    assert(I.isContinuous());
    const int kChannels = I.channels();
    const int kRows = I.rows;
    const int kCols = I.cols * kChannels;

    const int kColorValues{kRows * kCols};
    for(int i{0}; i < kColorValues; ++i)
    {
        uint8_t *const kElementPtr{I.ptr(0)}; //Only one row
        const uint8_t kElementValue{*kElementPtr};
        kElementPtr[i] = table[kElementValue];
        std::cout << kElementPtr[i];
    }
    return I;
}

cv::Mat& scanImageAndReduceColor(cv::Mat& I, const Table256& table)
{
    CV_Assert(I.depth() == CV_8U);

    const int kChannels = I.channels();
    const int kRows = I.rows;
    const int kCols = I.cols * kChannels;

    for(int row{0}; row < kRows; ++row)
    {
        uint8_t *const kRowArray{I.ptr(row)};
        for(int column{0}; column < kCols; ++column)
        {
            const uint8_t kColor{kRowArray[column]};
            kRowArray[column] = table[kRowArray[column]];
        }
    }
    return I;
}

int main(int argc, char** argv)
{
    // Verify arguments
	if (argc != 3)
	{
		printf("usage: Displayimage");
		return -1;
	}

    // Generate reduction table
    const int kSpaceSize{std::stoi(argv[2])};
    const Table256 kReductionTable{
        generateColorReductionTable256(kSpaceSize)
    };

    cv::Mat image{cv::imread(argv[1])};
    cv::Mat image2{cv::imread(argv[1])};

    scanContinuousImageAndReduceColor(image,kReductionTable );

    //cv::imshow("Yay", image);
    cv::imshow("yay", image2);

	return 0;
}
