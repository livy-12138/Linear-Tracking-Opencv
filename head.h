#pragma once
#include<opencv2/opencv.hpp>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace cv;

int size = 128;
int ColorThreshold = 250;
int framecount = 0;
bool filpflop = 0;
int UseCamera = 2;
Mat Pic[10][10];
int Len = 10;
double Rage = 1.5;
Size reSize = {128,128};
Mat CnctPic;
bool HasR = 0;
bool HasG = 0;
bool HasB = 0;
bool HasY = 0;
bool Lose = 0;
bool End = 0;
std::vector<Vec4i> Lines;
int FPS;
String Orientation = " δ֪ ";

void LoadPic();
void ShowPic(int k);
Mat GetTrance(Mat InputPic);
Mat Smooth(Mat InputPic,int flag);
Mat GetBoundaries(Mat InputPic);
Mat PurifyColor(Mat InputPic);
Mat Binary(Mat InputPic);
Mat Thin(Mat InputPic);
Mat HoughTransform(Mat InputPic);
void ColorDetect(Mat InputPic);
Mat EndPoint(Mat InputPic);
Mat CornerPoint(Mat InputPic);
void StateDisplay();
void DetectOrientation(std::vector<Vec2i> ForePoint, std::vector<Vec2i> LatePoint);
void Init();