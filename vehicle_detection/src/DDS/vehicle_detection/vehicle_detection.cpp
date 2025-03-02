#include <DDS/vehicle_detection/vehicle_detection.hpp>
#include <DDS/websocket/json.hpp>

#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

VehicleDetector::VehicleDetector(std::shared_ptr<WebsocketServer> server_, std::string cascade_filepath)
    : server(server_), cascade_classifier(new cv::CascadeClassifier(cascade_filepath))
{
    nf = av_frame_alloc();
    last = std::chrono::steady_clock::now();
}

VehicleDetector::~VehicleDetector()
{
    stop();
    delete cascade_classifier;
    av_frame_free(&nf);
}

using json = nlohmann::json;

namespace cv
{
    void to_json(json& j, const Rect rect)
    {
        j =
        {
            {"x", rect.x},
            {"y", rect.y},
            {"w", rect.width},
            {"h", rect.height}
        };
    }
}


void VehicleDetector::write_frame(const ClientID_t cid, AVFrame* frame)
{
    using namespace std::chrono_literals;

    auto now = std::chrono::steady_clock::now();
    if(now - last < 1s)
        return;
    last = now;

    add({cid, av_frame_clone(frame)});
}

void VehicleDetector::handle(ClientFramePacked cfp)
{
    AVFrame* frame = cfp.frame;

    if(!swsctx)
        swsctx = sws_getContext(frame->width, frame->height, (AVPixelFormat)frame->format, frame->width, frame->height, AV_PIX_FMT_BGR24, SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);

    int ret = sws_scale_frame(swsctx, nf, frame);
    if(ret < 0)
        return;

    cv::Mat src(frame->height, frame->width, CV_8UC3, nf->data[0], nf->linesize[0]);
    cv::Mat grey;

    cv::cvtColor(src, grey, cv::COLOR_BGR2GRAY);

    std::vector<cv::Rect> rects;
    cascade_classifier->detectMultiScale(grey, rects);

    json j =
    {
        {"type", "vehicle_detection_rects"},
        {"data", 
            {
                {"client_id", cid_to_hex(cfp.cid)},
                {"rects", rects}
            }
        }
    };
    server->broadcast(j.dump());

    av_frame_free(&frame);
}
