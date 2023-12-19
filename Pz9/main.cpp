#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

int main(int argc, char* argv[])
{
    VideoCapture cap("Video_output.mp4");
    if(!cap.isOpened())
    {
        std::cout << "Error open video!" << std::endl;
        return -1;
    }
    std::vector<Point> centers;
    Mat gray;

    while (cap.isOpened())
    {
    	Mat frame;
        bool ret = cap.read(frame);
        if (!ret)
        {
            std::cout << "Video ended" <<std::endl;
            break;
        }
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        Mat thr;
        threshold(gray, thr, 210, 255, THRESH_BINARY);
        Moments m = moments(thr, true);
        Point p(m.m10/m.m00, m.m01/m.m00);
        centers.push_back(p);
        for (int i=1; i<centers.size(); i++)
            line(gray, centers[i-1], centers[i], 0, 3);
        imshow("Frame", gray);
        waitKey(33);
    }
	if (!imwrite("result.jpg", gray))
		std::cout << "Error save frame!" << std::endl;
    cap.release();
    destroyAllWindows();
}
