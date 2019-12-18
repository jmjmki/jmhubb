// simple_camera.cpp
// MIT License
// Copyright (c) 2019 JetsonHacks
// See LICENSE for OpenCV license and additional information
// Using a CSI camera (such as the Raspberry Pi Version 2) connected to a 
// NVIDIA Jetson Nano Developer Kit using OpenCV
// Drivers for the camera and OpenCV are included in the base image

// #include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
// #include <opencv2/videoio.hpp>
// #include <opencv2/highgui.hpp>
#include <iostream>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/vfs.h>

#define SEC 60

using namespace cv;
using namespace std;


int capture_width = 640 ;
int capture_height = 480 ;
double framerate = 30.0 ;
int flip_method = 0 ;

int avail_disk_size = 0;

#define CURRENT_DISK 4

#define FILE_NAME_LENGTH 128
#define FOLDER_NAME_LENGTH 128

#define TIME_DIR 0
#define TIME_FILE 1

#define PATH_NAME "/home/jmjm/coding/"

#define LIMIT 1024*1024*2
/*
   string gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
   return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + to_string(capture_width) + ", height=(int)" +
   to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + to_string(framerate) +
   "/1 ! nvvidconv flip-method=" + to_string(flip_method) + " ! video/x-raw, width=(int)" + to_string(display_width) + ", height=(int)" +
   to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
   }
   */

string gstreamer_pipeline (int capture_width, int capture_height, int framerate) {
        return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + to_string(capture_width) + ", height=(int)" +
                to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + to_string(framerate) +
                "/1 ! nvvidconv ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

void ttime(int types, char *buf)
{
        time_t realtime;
        struct tm* local_timeinfo;
        time(&realtime);
        local_timeinfo = localtime(&realtime);

        if(types == TIME_DIR)
        {
                strftime(buf, FOLDER_NAME_LENGTH,"%Y%m%d_%H/", local_timeinfo);
        }
        else
        {
                strftime(buf, FILE_NAME_LENGTH,"%Y%m%d_%H%M%S", local_timeinfo);
        }
}

int avail_disk()
{
        struct statfs fa;
        statfs("/", &fa);
        int avail_GB = (fa.f_bsize * fa.f_bavail) /1024/1024/1024;
        if(avail_GB <= CURRENT_DISK)
                return 0;
        return avail_GB;
}

int main()
{


        string pipeline = gstreamer_pipeline(capture_width, capture_height, framerate);

        cout << "Using pipeline: \n\t" << pipeline << "\n";

        fflush(stdout);

        VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
        if(!cap.isOpened())
        {
                cout<<"Failed to open camera."<<std::endl;
                return (-1);
        }



        Mat img;
        int fourcc = VideoWriter::fourcc('D', 'I', 'V', 'X');
        Size size = Size(capture_width, capture_height);
        VideoWriter writer;

        // 파일 이름 생성
        char *mkdirName;
        char *mkfileName;

        char temp[100];
        char path_temp[100];
        char file_temp[100];
        char file_name2[100];

        mkfileName =(char *) malloc(FILE_NAME_LENGTH);
        mkdirName  =(char *) malloc(FOLDER_NAME_LENGTH);




        while(true) {
                sprintf(path_temp, "%s", PATH_NAME);
                avail_disk_size = avail_disk();
                if(avail_disk_size >= LIMIT)
                {
                        sprintf(temp, "rm -rf %s", PATH_NAME);
                        system(temp);
                }
                else if(errno ==17)
                        perror("mkdir error\n");
                else
                {
                        ttime(TIME_DIR, mkdirName);
                        strcat(path_temp, mkdirName);
                        mkdir(path_temp, 0777);
                        strerror(mkdir(path_temp, 0777));
                        //없으면 폴더 생성 및 그 안에  파일까지 생성
                        sprintf(file_temp, "%s", PATH_NAME);
                        strcat(file_temp, mkfileName);
                        //프레임 1800당 파일생성

                        int frameCnt = 0;

                        ttime(TIME_FILE, mkfileName);
                        sprintf(file_name2, "%s", PATH_NAME);

                        strcat(file_name2, mkdirName);
                        strcat(file_name2, mkfileName);
                        strcat(file_name2, ".avi");

                        if(!writer.open(file_name2, fourcc, framerate, size, true)){
                                perror("error");
                        }

                        while(frameCnt <=  1800) {
                                if(!cap.read(img)){
                                        cout<<"Capture read error"<<std::endl;
                                        break;
                                }
                                writer.write(img);
                                frameCnt++;

                                imshow("CSI Camera", img);
                                if(waitKey(5) ==27){
                                        break;
                                }

                        }
                        writer.release();


                }



        }


        cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);

        cout << "Hit ESC to exit" << "\n" ;


        cap.release();
        cv::destroyAllWindows() ;

        return 0;
}

                                                                                 197,0-1      바닥


