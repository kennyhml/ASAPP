#pragma once
#include <filesystem>
#include <opencv2/imgproc.hpp>

namespace asa::resources
{
    bool init();
    cv::Mat load_resource(std::filesystem::path path);

    namespace interfaces
    {
        bool init();
        inline cv::Mat cb_arrowdown;
        inline cv::Mat regions;
        inline cv::Mat lay_on;
        inline cv::Mat day;
        inline cv::Mat esc;
        inline cv::Mat accept;
        inline cv::Mat back;
        inline cv::Mat join_last_session;
        inline cv::Mat refresh;
        inline cv::Mat armor;
        inline cv::Mat damage;
        inline cv::Mat deposit;
        inline cv::Mat ride;
        inline cv::Mat tribemanager;
        inline cv::Mat day_log;
        inline cv::Mat server_transition;
    }

    namespace text
    {
        bool init();
        inline cv::Mat added;
        inline cv::Mat removed;
        inline cv::Mat day;
        inline cv::Mat lootcrate;
        inline cv::Mat default_teleport;
        inline cv::Mat fast_travel;
        inline cv::Mat teleport_to;
        inline cv::Mat access_inventory;
        inline cv::Mat ride;
        inline cv::Mat pick_up;
        inline cv::Mat detected_enemy;
        inline cv::Mat x;
        inline cv::Mat bp_weight;
        inline cv::Mat durability;
        inline cv::Mat teleports;
        inline cv::Mat beds;
        inline cv::Mat resume;
    }

    namespace wheel_icon
    {
        bool init();
        inline cv::Mat lay_on;
    }

}



