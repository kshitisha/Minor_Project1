Image Classification through Handcrafted Method using Machine Algorithms

A C++ application for detecting image forgery using SURF (Speeded Up Robust Features) feature detection and matching algorithms.

## Features

- Converts input images to grayscale
- Extracts SURF features from images
- Matches features between original and suspect images
- Provides visual comparison with matching points
- Displays forgery detection results with confidence metrics

## Dependencies

- OpenCV (with contrib modules for SURF)
- C++ compiler
- CMake (for building)

## Installation

1. Install OpenCV with contrib modules:
```bash
git clone https://github.com/opencv/opencv_contrib.git
git clone https://github.com/opencv/opencv.git
cd opencv
mkdir build && cd build
cmake -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules ..
make -j4
sudo make install
```

2. Clone this repository:
```bash
git clone https://github.com/kshitisha/Minor_Project1.git
cd Minor_Project1
```

## Building the Project

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

Run the program with three arguments:
```bash
./image_classifier <original_image> <output_grayscale_image> <suspect_image>
```

Example:
```bash
./image_classifier original.jpg grayscale.jpg suspect.jpg
```

## How It Works

1. The program converts the original image to grayscale
2. SURF features are extracted from both images
3. Feature matching is performed using BFMatcher
4. Good matches are filtered based on distance thresholds
5. Results are displayed with visual matching points
6. Classification decision is made based on number of good matches

## Web Interface

A JavaScript-based web interface is included for easier interaction:
- Upload original and suspect images
- View image previews
- Get detailed forgery detection results
- Confidence scores and match statistics

