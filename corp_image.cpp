#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <opencv2/opencv.hpp>

#define RESIZE_DEFAULT_WIDTH	640
#define RESIZE_DEFAULT_HEIGHT	360

int resizeAspect(cv::Mat im, int w, int h, cv::Mat& im_resized);
void help();

int main(int argc, char** argv)
{
	cv::Mat croppedImg;
	cv::Mat resizedImg;
	cv::Mat image;
	cv::Rect unionBox;

	int option_index = 0;
	

	struct option longOpt [] =
	{
		{"xOrd",required_argument,0,'x'},
		{"yOrd",required_argument,0,'y'},
		{"height",required_argument,0,'h'},
		{"width",required_argument,0,'w'},
		{"help", no_argument, 0,'H'},
		{0,0,0,0}
	};

	while(true) {
		int c = getopt_long(argc, argv, "x:y:h:w:", longOpt, &option_index);
		if (-1 == c){
			break;
		}

		switch(c) {
		case 'x':
			unionBox.x = atoi(optarg);
			break;
		case 'y':
			unionBox.y = atoi(optarg);
			break;
		case 'h':
			unionBox.height = atoi(optarg);
			break;
		case 'w':
			unionBox.width = atoi(optarg);
			break;
		case 'H':
			help();
			exit(0);
			break;
		case '?':
			break;
		default:
			break;
		}
	}

	
	//open the default camera on device.
	cv::VideoCapture cap(0);
	if(!cap.isOpened()){
		std::cout << "Unable to open device camera" << "\n";

	}

	cap >> image;
	croppedImg = image(unionBox).clone();

	// Resize object to fixed resolution, keep aspect ratio
        resizeAspect(croppedImg, RESIZE_DEFAULT_WIDTH, RESIZE_DEFAULT_HEIGHT, resizedImg);

	rectangle(image, unionBox, cv::Scalar(0, 255, 0));

	imwrite("original.bmp", image);
	imwrite("corpped.bmp", croppedImg);
	imwrite("resized.bmp", resizedImg);

	return 0;
}

void help()
{
}

int resizeAspect(cv::Mat im, int w, int h, cv::Mat& im_resized)
{
    im_resized = cv::Mat();

    if (im.empty()) {
        std::cerr << __FUNCTION__ << " : input image is empty" << std::endl;
        //RDK_LOG(RDK_LOG_ERROR,"LOG.RDK.SMARTTHUMBNAIL","%s(%d):input image is empty.\n", __FUNCTION__ , __LINE__);
        return -1;
    }

    int new_w = im.cols;
    int new_h = im.rows;

    if (((float)w/new_w) < ((float)h/new_h)) {
        new_w = w;
        new_h = (im.rows * w)/im.cols;
    } else {
        new_h = h;
        new_w = (im.cols * h)/im.rows;
    }

    cv::Mat resized;
    cv::resize(im,resized,cv::Size(new_w, new_h));

    // create image the size of the net layer
    im_resized = cv::Mat(h,w,resized.type());

    // embed image into im_resized image
    int dx = (w-new_w)/2;
    int dy = (h-new_h)/2;

    resized.copyTo(im_resized(cv::Rect(dx,dy,resized.cols, resized.rows)));

    return 0;
}	
