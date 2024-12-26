#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <sys/stat.h> // For mkdir on POSIX systems
#include <string>

// Cross-platform directory existence and creation function
bool createDirectoryIfNotExists(const std::string& dirPath) {
#ifdef _WIN32
    struct _stat info;
    if (_stat(dirPath.c_str(), &info) != 0) {
        // Directory does not exist; try to create it
        return _mkdir(dirPath.c_str()) == 0;
    } else if (info.st_mode & _S_IFDIR) {
        return true; // Directory already exists
    }
    return false;
#else
    struct stat info;
    if (stat(dirPath.c_str(), &info) != 0) {
       
        return mkdir(dirPath.c_str(), 0777) == 0;
    } else if (info.st_mode & S_IFDIR) {
        return true; 
    }
    return false;
#endif
}

void convertToGrayscale(const char *inputFile, const char *outputFile) {
    cv::Mat img = cv::imread(inputFile, cv::IMREAD_COLOR);
    if (img.empty()) {
        fprintf(stderr, "Could not open or find the image: %s\n", inputFile);
        return;
    }

    cv::Mat grayImg;
    cv::cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);

    std::string outputPath(outputFile);
    size_t lastSlash = outputPath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        std::string dirPath = outputPath.substr(0, lastSlash);
        if (!createDirectoryIfNotExists(dirPath)) {
            fprintf(stderr, "Failed to create directory: %s\n", dirPath.c_str());
            return;
        }
    }

    if (!cv::imwrite(outputFile, grayImg)) {
        fprintf(stderr, "Failed to save the grayscale image to: %s\n", outputFile);
    }
}

void classifyImage(const char *originalFile, const char *suspectFile) {
    cv::Mat originalImg = cv::imread(originalFile, cv::IMREAD_GRAYSCALE);
    cv::Mat suspectImg = cv::imread(suspectFile, cv::IMREAD_GRAYSCALE);

    if (originalImg.empty() || suspectImg.empty()) {
        fprintf(stderr, "Could not open or find the images.\n");
        return;
    }

    int minHessian = 400;
    cv::Ptr<cv::xfeatures2d::SURF> detector = cv::xfeatures2d::SURF::create(minHessian);

    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;
    detector->detectAndCompute(originalImg, cv::noArray(), keypoints1, descriptors1);
    detector->detectAndCompute(suspectImg, cv::noArray(), keypoints2, descriptors2);

    cv::BFMatcher matcher(cv::NORM_L2);
    std::vector<cv::DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    double max_dist = 0, min_dist = 100;
    for (int i = 0; i < matches.size(); i++) {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }

    std::vector<cv::DMatch> good_matches;
    for (int i = 0; i < matches.size(); i++) {
        if (matches[i].distance <= std::max(3 * min_dist, 0.02)) {
            good_matches.push_back(matches[i]);
        }
    }

    if (good_matches.size() > 100) {
        printf("The image is likely NOT forged. Good matches: %lu\n", good_matches.size());
    } else {
        printf("The image is likely FORGED. Good matches: %lu\n", good_matches.size());
    }

    cv::Mat imgMatches;
    cv::drawMatches(originalImg, keypoints1, suspectImg, keypoints2, good_matches, imgMatches);

    if (good_matches.size() > 100) {
        cv::putText(imgMatches, "Result: NOT FORGED", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
    } else {
        cv::putText(imgMatches, "Result: FORGED", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
    }

    cv::imshow("Good Matches with Result", imgMatches);
    cv::waitKey(0);
}

int main(int argc, char** argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <original_image> <output_grayscale_image> <suspect_image>\n", argv[0]);
        return 1;
    }

    const char *inputFile = argv[1];
    const char *outputFile = argv[2];
    const char *suspectFile = argv[3];

    convertToGrayscale(inputFile, outputFile);
    classifyImage(outputFile, suspectFile);

    return 0;
}
