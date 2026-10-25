#include <iostream>
#include <cstdlib>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/videoio.hpp>

#include "nanotrack.hpp" 

void cxy_wh_2_rect(const cv::Point& pos, const cv::Point2f& sz, cv::Rect &rect) 
{   
    rect.x = std::max(0, pos.x - int(sz.x / 2));
    rect.y = std::max(0, pos.y - int(sz.y / 2));
    rect.width = int(sz.x);   
    rect.height = int(sz.y);    
}

void track(NanoTrack *siam_tracker, const char *video_path)

{
    // Read video 
    cv::VideoCapture capture; 
    bool ret;
    if (strlen(video_path)==1)
        ret = capture.open(atoi(video_path));  
    else
        ret = capture.open(video_path); 

    // Exit if video not opened.
    if (!ret)
    {
        std::cout << "Open cap failed!" << std::endl;
        return;
    }

    // Read first frame. 
    cv::Mat frame; 
    
    bool ok = capture.read(frame);
    if (!ok)
    {
        std::cout<< "Cannot read video file" << std::endl;
        return; 
    }
    
    // Select a rect.（接屏时开窗口；纯串口环境请设置 QT_QPA_PLATFORM=offscreen）
    cv::namedWindow("demo");
    cv::Rect trackWindow =cv::Rect(244,161,74,70);         // 固定值 
    
    // Initialize tracker with first frame and rect.
    State state; 
    
    //writer
    cv::VideoWriter writer;
    //writer.open("output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 25, cv::Size(640, 480), true);
    //cv::Mat framee = cv::Mat::zeros(640, 640, CV_8UC3);
    //   frame, (cx,cy) , (w,h), state
    siam_tracker->init(frame, trackWindow);
    std::cout << "==========================" << std::endl;
    std::cout << "Init done!" << std::endl; 
    std::cout << std::endl; 
    cv::Mat init_window;
    frame(trackWindow).copyTo(init_window); 
    // 分段计时：decode（读帧/解码）、track（预处理+推理+后处理）、show（画框+imshow+waitKey）
    int frame_count = 0;
    double t_decode = 0.0, t_track = 0.0, t_show = 0.0, t_total = 0.0;
    const double tick_freq = cv::getTickFrequency();
    auto ms_avg = [tick_freq](double ticks, int n) {
        return ticks * 1000.0 / (tick_freq * n);
    };

    for (;;)
    {
        // Read a new frame.
        double t0 = cv::getTickCount();
        bool ok = capture.read(frame);
        if (!ok)
        {
            std::cout << "Cannot read video file" << std::endl;
            break; 
        }
        double t1 = cv::getTickCount();

        float cls_score_max;
        //siam_tracker->update(frame, target_pos, target_sz, 1.0, cls_score_max);
        siam_tracker->track(frame);
        double t2 = cv::getTickCount();

        // Draw the tracked object.
        cv::Rect result_rect;
        cxy_wh_2_rect(siam_tracker->state.target_pos, siam_tracker->state.target_sz, result_rect);
        cv::rectangle(frame, result_rect, cv::Scalar(0, 255, 0), 2, 1);
        cv::imshow("demo", frame);
        //cv::resize(frame, frame, cv::Size(640, 480));
        //writer.write(frame);
        
        // Exit if ESC pressed.
        int k = cv::waitKey(1);
        double t3 = cv::getTickCount();
        if (k == 27)
            break;

        t_decode += t1 - t0;
        t_track  += t2 - t1;
        t_show   += t3 - t2;
        t_total  += t3 - t0;

        frame_count++;
        if (frame_count % 100 == 0)
        {
            const int n = 100;
            std::cout << "fps: " << n * tick_freq / t_total
                      << "  | decode " << ms_avg(t_decode, n) << " ms"
                      << ", track "   << ms_avg(t_track, n)  << " ms"
                      << ", show "    << ms_avg(t_show, n)   << " ms"
                      << ", total "   << ms_avg(t_total, n)  << " ms" << std::endl;
            t_decode = t_track = t_show = t_total = 0.0;
        }
    }
}

int main(int argc, char **argv)
{
    // if (argc != 2)
    // {
    //     std::cout << "Usage: " << argv[0] << " video_path" << std::endl;
    //     return -1;
    // }
    // string video_path = "girl_dance.mp4"

    std::string video_path = (argc > 1) ? argv[1] : "girl_dance.mp4";
    NanoTrack *siam_tracker = new NanoTrack();
    track(siam_tracker, video_path.c_str());


    delete siam_tracker;
    return 0;
}
