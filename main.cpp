#include "head.h"

void main(void)
{
	if (UseCamera==1)
	{
		VideoCapture cap;
		cap.open(0);
		while (true)
		{
			Mat frame[20];
			cap >> frame[0];
			resize(frame[0], frame[1], reSize);
			frame[2] = Smooth(GetTrance(frame[1]),2);
			frame[3] = Smooth(PurifyColor(frame[2]),2);
			ColorDetect(frame[3]);
			frame[4] = Binary(frame[3]);
			frame[5] = Thin(frame[4]);
			frame[6] = HoughTransform(frame[5]);
			frame[7] = Binary(frame[6]);
			frame[8] = Thin(frame[7]);
			frame[9] = HoughTransform(frame[8]);
			frame[10] = frame[1] + frame[6];
			String windowname = "Camera";
			namedWindow(windowname, 0);
			resizeWindow(windowname, 500, 500);
			moveWindow(windowname, 1000,0);
			imshow(windowname, frame[9]);
			imshow("PurifyColor", frame[3]);
			namedWindow("Thin", 0);
			moveWindow("Thin", 0, 100);
			imshow("Thin", frame[7]);
			
			waitKey(30);
		}
	}
	else if (UseCamera == 0)
	{
		LoadPic();
		for (int i = 0; i < Len; i++)
		{
			Pic[i][1] = GetTrance(Pic[i][0]);
			Pic[i][2] = PurifyColor(Pic[i][1]);
			Pic[i][3] = Binary(Pic[i][2]);
			Pic[i][4] = Thin(Pic[i][3]);
			Pic[i][5] = HoughTransform(Pic[i][4]);
		}
		ShowPic(2);
		ShowPic(3);
		ShowPic(4);
		ShowPic(5);
		waitKey(0);
	}
	else
	{
		VideoCapture cap;
		cap.open("E:/VS2019_Project/LinearTracking/pic/video0.mp4");
		while (true)
		{
			Mat frame[20];
			while (cap.read(frame[0]))
			{
				resize(frame[0], frame[1], reSize);
				frame[2] = Smooth(GetTrance(frame[1]),2);
				frame[3] = Smooth(PurifyColor(frame[2]),2);
				ColorDetect(frame[3]);
				frame[4] = Binary(frame[3]);
				frame[5] = Thin(frame[4]);
				frame[6] = HoughTransform(frame[5]);
				frame[7] = Binary(frame[6]);
				frame[8] = EndPoint(frame[7]);
				for (int i = 0; i < 8; i++)
				{
					String windowname = "Win" + std::to_string(i);
					namedWindow(windowname, 0);
					resizeWindow(windowname, 200, 200);
					int k = i % 4;
					int j = i / 4;
					moveWindow(windowname,k*220 , j*220);
					imshow(windowname, frame[i]);	
				}
				FPS = cap.get(CAP_PROP_FPS);
				StateDisplay();
				waitKey(20);
			}
			cap.release();
			waitKey(0);
		}
	}
}

void LoadPic()
{
	for (int i = 0; i < Len; i++)
	{
		String path = "E:/VS2019_Project/LinearTracking/pic/"+std::to_string(i)+".jpg";
		resize(imread(path,1), Pic[i][0],reSize);
	}
}

void ShowPic(int k)
{
	for (int i = 0; i < Len; i++)
	{
		String windowname = "Pic[" + std::to_string(i) + "][" + std::to_string(k) + "]";
		namedWindow(windowname,0);
		resizeWindow(windowname, 150,150);
		moveWindow(windowname,150 * i,180*(k-2));
		imshow(windowname,Pic[i][k]);
	}
}

Mat GetTrance(Mat InputPic)
{
	Mat OutputPic = Mat::zeros(InputPic.rows, InputPic.cols, CV_8UC3);
	for (int i = 0; i < InputPic.rows; i++)
	{
		for (int j = 0; j < InputPic.cols; j++)
		{
			if (InputPic.at<Vec3b>(i, j)[0] * Rage < InputPic.at<Vec3b>(i, j)[1] || InputPic.at<Vec3b>(i, j)[0] * Rage < InputPic.at<Vec3b>(i, j)[2] || InputPic.at<Vec3b>(i, j)[1] * Rage < InputPic.at<Vec3b>(i, j)[0] || InputPic.at<Vec3b>(i, j)[1] * Rage < InputPic.at<Vec3b>(i, j)[2] || InputPic.at<Vec3b>(i, j)[2] * Rage < InputPic.at<Vec3b>(i, j)[0] || InputPic.at<Vec3b>(i, j)[2] * Rage < InputPic.at<Vec3b>(i, j)[1])
			{
				for (int k = 0; k < 3; k++)
					OutputPic.at<Vec3b>(i, j)[k] = InputPic.at<Vec3b>(i, j)[k];
			}
		}
	}
	return OutputPic;
}

Mat Smooth(Mat InputPic,int flag)
{
	Mat OutputPic = Mat::zeros(InputPic.rows, InputPic.cols, CV_8UC3);
	Mat kernel = Mat::zeros(3, 3, CV_8UC1);
	kernel.at<uchar>(1, 1) = 255;
	kernel.at<uchar>(0, 1) = 255;
	kernel.at<uchar>(2, 1) = 255;
	kernel.at<uchar>(1, 0) = 255;
	kernel.at<uchar>(1, 2) = 255;
	if (flag == 0 || flag == 2);
		morphologyEx(InputPic, OutputPic, MORPH_CLOSE, kernel);//关去噪声
	if (flag == 1 || flag == 2);
		morphologyEx(InputPic, OutputPic, MORPH_OPEN, kernel);//开去孔洞

	return OutputPic;
}

Mat GetBoundaries(Mat InputPic)
{
	Mat OutputPic = Mat::zeros(InputPic.rows, InputPic.cols, CV_8UC3);
	Canny(InputPic, OutputPic, 0, 10);
	return OutputPic;
}

Mat PurifyColor(Mat InputPic)
{
	Mat OutputPic = Mat::zeros(InputPic.rows, InputPic.cols, CV_8UC3);
	for (int i = 0; i < InputPic.rows; i++)
	{
		for (int j = 0; j < InputPic.cols; j++)
		{
			int r = InputPic.at<Vec3b>(i, j)[2];
			int g = InputPic.at<Vec3b>(i, j)[1];
			int b = InputPic.at<Vec3b>(i, j)[0];
			if (r > 150)
			{
				OutputPic.at<Vec3b>(i, j)[2] = 255;
			}
			if (g > 150)
			{
				OutputPic.at<Vec3b>(i, j)[1] = 255;
				if (r<200)
				{
					OutputPic.at<Vec3b>(i, j)[0] = 0;
					OutputPic.at<Vec3b>(i, j)[2] = 0;
				}
			}
			if (b > 150)
			{
				OutputPic.at<Vec3b>(i, j)[0] = 255;
				OutputPic.at<Vec3b>(i, j)[1] = 0;
				OutputPic.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
	return OutputPic;
}

Mat Binary(Mat InputPic)
{
	Mat OutputPic = Mat::zeros(InputPic.rows, InputPic.cols, CV_8UC1);
	cvtColor(InputPic, InputPic, COLOR_RGB2GRAY);
	threshold(InputPic, OutputPic, 1, 255, 0);
	return OutputPic;
}

Mat Thin(Mat InputPic)
{
	int i, j, k;
	uchar p[11];
	int pos[9][2] = { {0,0},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1} };
	int cond1, cond2, cond3, cond4, counter = 0;
	bool pointsDeleted = true;
	Mat mask, dstImg;
	dstImg = InputPic / 255;
	while (pointsDeleted)
	{
		mask = Mat::zeros(dstImg.size(), CV_8UC1);
		pointsDeleted = false;
		for (i = 1; i < dstImg.rows - 1; i++)
		{
			for (j = 1; j < dstImg.cols - 1; j++)
			{
				for (k = 1; k < 10; k++)
					p[k] = dstImg.at<uchar>(i + pos[k - 1][0], j + pos[k - 1][1]);
				if (p[1] == 0)continue;
				cond1 = 0;
				for (k = 2; k < 10; k++)cond1 += p[k];
				cond2 = 0;
				p[10] = p[2];
				for (k = 2; k < 10; k += 2)
					cond2 += ((p[k] == 0 && p[k + 1] == 1) + (p[k + 1] == 0 && p[k + 2] == 1));
				if (counter % 2 == 0)
				{
					cond3 = p[2] * p[4] * p[6];
					cond4 = p[4] * p[6] * p[8];
				}
				else
				{
					cond3 = p[2] * p[4] * p[8];
					cond4 = p[2] * p[6] * p[8];
				}
				if ((2 <= cond1 && cond1 <= 6) && (cond2 == 1) && (cond3 == 0) && (cond4 == 0))
				{
					pointsDeleted = true;
					mask.at<uchar>(i, j) = 1;
				}
			}
		}
		dstImg &= ~mask;
		counter++;
	}
	dstImg *= 255;
	return dstImg;
}

Mat HoughTransform(Mat InputPic)
{
	Mat OutputPic = Mat::zeros(InputPic.rows, InputPic.cols, CV_8UC3);
	HoughLinesP(InputPic,Lines,1,CV_PI/180,20,20,1000);
	std::vector<Vec2i>MidPoints1(Lines.size());
	std::vector<Vec2i>MidPoints2(Lines.size());
	for (int i = 0; i < Lines.size(); i++)
	{
		Vec4i HoughPoints = Lines[i];
		if (filpflop)
		{
			MidPoints1[i] = { (HoughPoints[0] + HoughPoints[2]) / 2,(HoughPoints[1] + HoughPoints[3]) / 2 };
			DetectOrientation(MidPoints2,MidPoints1);
			filpflop = 0;
		}
		else
		{
			MidPoints2[i] = { (HoughPoints[0] + HoughPoints[2]) / 2,(HoughPoints[1] + HoughPoints[3]) / 2 };
			DetectOrientation(MidPoints1, MidPoints2);
			filpflop = 1;
		}
		line(OutputPic, Point(HoughPoints[0], HoughPoints[1]), Point(HoughPoints[2], HoughPoints[3]), Scalar(255, 255, 255), 3);
	}
	framecount++;
	return OutputPic;
}

void ColorDetect(Mat InputPic)
{
	int sum = 0;
	int max = ColorThreshold;
	int r = 0;
	int g = 0;
	int y = 0;
	int b = 0;
	for (int i = 0; i < InputPic.rows; i++)
	{
		for (int j = 0; j < InputPic.cols; j++)
		{
			if (InputPic.at<Vec3b>(i,j)[0]>200&& InputPic.at<Vec3b>(i, j)[1]<100&& InputPic.at<Vec3b>(i, j)[2]<100)
				b++;
			if (InputPic.at<Vec3b>(i, j)[1] > 200 && InputPic.at<Vec3b>(i, j)[0] < 100 && InputPic.at<Vec3b>(i, j)[2] < 100)
				g++;
			if (InputPic.at<Vec3b>(i, j)[2] > 200 && InputPic.at<Vec3b>(i, j)[0] < 100 && InputPic.at<Vec3b>(i, j)[1] < 100)
				r++;
			if (InputPic.at<Vec3b>(i, j)[2] > 200 && InputPic.at<Vec3b>(i, j)[0] < 100 && InputPic.at<Vec3b>(i, j)[1] > 200)
				y++;  
		}
	}
	if (b > max)
	{
		HasB = 1;
	}
	else
	{
		HasB = 0;
	}
	if (g > max)
	{
		HasG = 1;
	}
	else
	{
		HasG = 0;
	}
	if (r > max)
	{
		HasR = 1;
	}
	else
	{
		HasR = 0;
	}
	if (y > max)
	{
		HasY = 1;
	}
	else
	{
		HasY = 0;
	}
	if (b + g + r + y < max)
	{
		Lose = 1;
	}
	else
	{
		Lose = 0;
	}
}

Mat EndPoint(Mat InputPic)
{
	int i, elemSize = 3;
	Mat dstImg, maskImg = Mat::zeros(InputPic.size(), CV_8UC1);
	Mat elemStruct(elemSize, elemSize, CV_8U, Scalar(1));
	elemStruct.at<uchar>(1, 1) = 0;
	for (i = 0; i < elemSize; i++)elemStruct.at<uchar>(2, i) = 0;
	dstImg = 255 - InputPic;
	erode(dstImg, dstImg, elemStruct);
	maskImg = InputPic & dstImg;
	elemStruct = Mat::ones(elemSize, elemSize, CV_8U);
	elemStruct.at<uchar>(1, 1) = 0;
	for (i = 0; i < elemSize; i++)elemStruct.at<uchar>(i, 2) = 0;
	dstImg = 255 - InputPic;
	erode(dstImg, dstImg, elemStruct);
	maskImg |= InputPic & dstImg;
	elemStruct = Mat::ones(elemSize, elemSize, CV_8U);
	elemStruct.at<uchar>(1, 1) = 0;
	for (i = 0; i < elemSize; i++)elemStruct.at<uchar>(0, i) = 0;
	dstImg = 255 - InputPic;
	erode(dstImg, dstImg, elemStruct);
	maskImg |= InputPic & dstImg;
	elemStruct = Mat::ones(elemSize, elemSize, CV_8U);
	elemStruct.at<uchar>(1, 1) = 0;
	for (i = 0; i < elemSize; i++)elemStruct.at<uchar>(i, 0) = 0;
	dstImg = 255 - InputPic;
	erode(dstImg, dstImg, elemStruct);
	maskImg |= InputPic & dstImg;
	maskImg = 255 - maskImg;
	Size s = maskImg.size();
	Mat OutputPic = Mat::zeros(InputPic.rows, InputPic.cols, CV_8UC3);
	for (int k = 0; k < Lines.size(); k++)
	{
		int count = 0;
		Vec4i HoughPoints = Lines[k];
		for (int i = 0; i < s.height; i++)
		{
			for (int j = 0; j < s.width; j++)
			{
				//bool inArea = 1;
				bool inArea = (((HoughPoints[0] - j) * (HoughPoints[0] - j) + (HoughPoints[1] - i) * (HoughPoints[1] - i)) < 50 || ((HoughPoints[2] - j) * (HoughPoints[2] - j) + (HoughPoints[3] - i) * (HoughPoints[3] - i)) < 50);
				if (inArea && (maskImg.at<uchar>(i, j) == 0 && i > 5 && i < 123 && j>5 && j < 123))
				{
					circle(OutputPic, Point(j, i), 5, { 255,255,255 }, 2);
					count++;
				}
			}
		}
		if (count == 2)
		{
			End = 1;
		}
		else
			End = 0;
	}
	return OutputPic;
}

Mat CornerPoint(Mat InputPic)
{
	Mat B_Img;
	threshold(InputPic, B_Img, 120, 255, 0);
	Mat G_Img;
	Mat kernel = Mat::zeros(3, 3, CV_8UC1);
	kernel.at<uchar>(1, 1) = 255;
	kernel.at<uchar>(1, 0) = 255;
	kernel.at<uchar>(1, 2) = 255;
	kernel.at<uchar>(0, 1) = 255;
	kernel.at<uchar>(2, 1) = 255;
	morphologyEx(B_Img, G_Img, MORPH_GRADIENT, kernel);
	Mat kernelc = Mat::zeros(5, 5, CV_8UC1);//十字形核
	Mat kerneld = Mat::zeros(5, 5, CV_8UC1);//菱形核
	Mat kernelx = Mat::zeros(5, 5, CV_8UC1);//x形核
	Mat kernelr = Mat::ones(5, 5, CV_8UC1);//矩形核
	for (int i = 0; i < 5; i++)
	{
		kernelc.at<uchar>(i, 2) = 255;
		kernelc.at<uchar>(2, i) = 255;
	}
	kerneld = kernelc.clone();
	kerneld.at<uchar>(1, 1) = 255;
	kerneld.at<uchar>(3, 1) = 255;
	kerneld.at<uchar>(1, 3) = 255;
	kerneld.at<uchar>(3, 3) = 255;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if ((i == j) || (i + j == 4))kernelx.at<uchar>(i, j) = 255;
		}
	}
	Mat R_Corner_Img;
	Mat D_Corner_Img;
	Mat dilate_Img;
	Mat erode_Img;
	dilate(B_Img, dilate_Img, kernelc);
	erode(dilate_Img, erode_Img, kerneld);
	R_Corner_Img = B_Img - erode_Img;
	dilate(B_Img, dilate_Img, kernelx);
	erode(dilate_Img, erode_Img, kernelr);
	D_Corner_Img = B_Img - erode_Img;
	Mat B_Corner_Img = Mat::zeros(B_Img.size(), CV_8UC1);
	B_Corner_Img = D_Corner_Img + R_Corner_Img;
	threshold(B_Corner_Img, B_Corner_Img, 100, 255, 0);
	Mat OutputPic = Mat::zeros(InputPic.rows, InputPic.cols, CV_8UC3);
	for (int i = 0; i < OutputPic.size().height; i++)
	{
		for (int j = 0; j < OutputPic.size().width; j++)
		{
			for (int k = 0; k < Lines.size(); k++)
			{
				Vec4i HoughPoints = Lines[k];
				bool inArea = ((double)((double)HoughPoints[0] - i) * ((double)HoughPoints[0] - i) + (double)((double)HoughPoints[1] - j) * ((double)HoughPoints[1] - j))<25||((double)((double)HoughPoints[2] - i) * ((double)HoughPoints[2] - i) + (double)((double)HoughPoints[3] - j) * ((double)HoughPoints[3] - j))<25;
				if (inArea && B_Corner_Img.at<uchar>(i, j) == 255)
					circle(OutputPic, Point(j, i), 5, { 0,0,255 }, 1, 5);
			}
		}
	}
	return OutputPic;
}
void DetectOrientation(std::vector<Vec2i> ForePoint, std::vector<Vec2i> LatePoint)
{
	if (framecount > 1)
	{
		for (int i = 0; i < ForePoint.size(); i++)
		{
			for (int j = 0; j < LatePoint.size(); j++)
			{
				int d = (ForePoint[i][0] - LatePoint[j][0]) * (ForePoint[i][0] - LatePoint[j][0]) + (ForePoint[i][1] - LatePoint[j][1]) * (ForePoint[i][1] - LatePoint[j][1]);
				if (d < 100)
				{
					int x = ForePoint[i][0] - LatePoint[j][0];
					int y = ForePoint[i][1] - LatePoint[j][1];
					if (y < 0 && y * y>x * x)
						Orientation = " 下方 ";
					else if (y > 0 && y * y > x * x)
						Orientation = " 上方 ";
					else if (x < 0 && x * x>y * y)
						Orientation = " 左方 ";
					else if (x > 0 && x * x > y * y)
						Orientation = " 右方 ";
					break;
				}
			}
		}
	}
	else
	{
		Orientation = " 稍候 ";
	}
}
void StateDisplay()
{
	String strR, strG, strB, strY, strLose, strEnd, strOrient;
	if (HasR)
		strR = " 存在 ";
	else
		strR = "不存在";
	if (HasG)
		strG = " 存在 ";
	else
		strG = "不存在";
	if (HasB)
		strB = " 存在 ";
	else
		strB = "不存在";
	if (HasY)
		strY = " 存在 ";
	else
		strY = "不存在";
	if (Lose)
	{
		strLose = "已丢失";
		strOrient = Orientation;
	}
	else
	{
		strLose = "跟踪中";
		strOrient = "未丢失";
	}
	if (End)
		strEnd = "已到达";
	else
		strEnd = "未到达";
	
	system("cls");
	printf("---------------------------------------------------------------------\n");
	printf("|                          LinearTracking                           |\n");
	printf("---------------------------------------------------------------------\n");
	printf("|      红色      |      绿色      |      蓝色      |      黄色      |\n");
	printf("---------------------------------------------------------------------\n");
	printf("|     %s     |     %s     |     %s     |     %s     |\n",strR.c_str(),strG.c_str(),strB.c_str(),strY.c_str());
	printf("---------------------------------------------------------------------\n");
	printf("|      目标      |      终点      |    消失方向    |      帧率      |\n");
	printf("---------------------------------------------------------------------\n");
	printf("|     %s     |     %s     |     %s     |       %2d       |\n",strLose.c_str(),strEnd.c_str(),strOrient.c_str(),FPS);
	printf("---------------------------------------------------------------------\n");
}
void Init()
{
	reSize = Size(size, size);
}