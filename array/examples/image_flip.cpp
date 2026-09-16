#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

namespace {

void flipGray(const cv::Mat& img, cv::Mat& flipped) {
    for (int j = 0; j < img.rows; ++j) {
        for (int k = 0; k < img.cols; ++k) {
            flipped.at<uchar>(j, k) = img.at<uchar>(j, img.cols - 1 - k);
        }
    }
}

void flipColor(const cv::Mat& img, cv::Mat& flipped) {
    for (int j = 0; j < img.rows; ++j) {
        for (int k = 0; k < img.cols; ++k) {
            for (int i = 0; i < 3; ++i) {
                flipped.at<cv::Vec3b>(j, k)[i] =
                    img.at<cv::Vec3b>(j, img.cols - 1 - k)[i];
            }
        }
    }
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: image_flip <gray|color> <input> <output>\n";
        return 1;
    }

    const std::string mode = argv[1];
    const int flag = (mode == "gray") ? cv::IMREAD_GRAYSCALE : cv::IMREAD_COLOR;
    cv::Mat img = cv::imread(argv[2], flag);

    if (img.empty()) {
        std::cerr << "Failed to read image: " << argv[2] << '\n';
        return 1;
    }

    if (mode == "gray") {
        std::cout << "Image shape: (" << img.rows << ", " << img.cols << ")\n";
    } else if (mode == "color") {
        std::cout << "Image shape: (" << img.rows << ", " << img.cols
                  << ", " << img.channels() << ")\n";
    } else {
        std::cerr << "mode must be gray or color\n";
        return 1;
    }

    cv::Mat flipped(img.rows, img.cols, img.type());
    if (mode == "gray")
        flipGray(img, flipped);
    else
        flipColor(img, flipped);

    if (!cv::imwrite(argv[3], flipped)) {
        std::cerr << "Failed to write image: " << argv[3] << '\n';
        return 1;
    }

    return 0;
}
