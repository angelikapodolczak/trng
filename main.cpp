#include "opencv2/opencv.hpp"
#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include<fstream>

using namespace std;
using namespace cv;

uint8_t saturated_add(uint8_t val1, int8_t val2)
{
	int16_t val1_int = val1;
	int16_t val2_int = val2;
	int16_t tmp = val1_int + val2_int;

	if (tmp > 255)
		return 255;
	else if (tmp < 0)
		return 0;
	else
		return tmp;
}

int zamiana(int tab[])
{
	int liczba_dz = 0;
	for (int j = 0; j < 8; j++)
	{
		int x = 1;
		for (int k = j + 1; k < 8; k++)
			x = x * 2;
		int p = tab[j];
		liczba_dz = liczba_dz + p * x;
	}
	return liczba_dz;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
	Mat dst;
	VideoCapture cap;
	Mat img;
	Mat img_out;
	Mat img_grey;
	Mat dithImg;
	int err;
	int8_t a, b, c, d;

	/////POBRANIE OBRAZU//////
	if (argc == 1)
		cap.open(0);
	else
		cap.open(argv[1]);

	if (!cap.isOpened())
	{
		cerr << "Nie udalo sie otworzyc strumienia." << endl;
		return -1;
	}

	cap >> img;

	/////MODYFIKACJE OBRAZU//////
	Size size(512, 512);
	resize(img, img_out, size);		//zmiana rozmiaru

	cvtColor(img_out, img_grey, CV_BGR2GRAY);	//zamiana na szarosci	
	dithImg = img_grey;

	//zamiana na czern-biel - metoda Floyda-Steinberga (dithering)
	for (int i = 0; i < 512; i++)
	{
		for (int j = 0; j < 512; j++)
		{
			if (dithImg.at<uint8_t>(i, j) > 127)
			{
				err = dithImg.at<uint8_t>(i, j) - 255;
				dithImg.at<uint8_t>(i, j) = 255;
			}
			else
			{
				err = dithImg.at<uint8_t>(i, j) - 0;
				dithImg.at<uint8_t>(i, j) = 0;
			}

			a = (err * 7) / 16;
			b = (err * 1) / 16;
			c = (err * 5) / 16;
			d = (err * 3) / 16;

			if ((i != (512 - 1)) && (j != 0) && (j != (512 - 1)))
			{
				dithImg.at<uint8_t>(i + 0, j + 1) = saturated_add(dithImg.at<uint8_t>(i + 0, j + 1), a);
				dithImg.at<uint8_t>(i + 1, j + 1) = saturated_add(dithImg.at<uint8_t>(i + 1, j + 1), b);
				dithImg.at<uint8_t>(i + 1, j + 0) = saturated_add(dithImg.at<uint8_t>(i + 1, j + 0), c);
				dithImg.at<uint8_t>(i + 1, j - 1) = saturated_add(dithImg.at<uint8_t>(i + 1, j - 1), d);
			}
		}
	}



	////////////////////////////////////////////////////////////
	///////////ARNOLD'S CAT MAP/////////////////////////////////
	//// [ x' ] = [ 1    p  ] * [ x ] mod N  ////// N=512   ////
	//// [ y' ]   [ q  pq+1 ]   [ y ]        ////// p,q = 1 ////
	////////////////////////////////////////////////////////////
	int x1, x2, x3, x4, x5, x6, x7;
	int y1, y2, y3, y4, y5, y6, y7;
	int p = 1, q = 1, N = 512;
	Mat act;// (512, 512, uint8_t);
	Mat act_out1;// (512, 512, uint8_t);
	Mat act_out2;
	Mat act_out3;
	Mat act_out4;
	Mat act_out5;
	Mat act_out6;
	Mat act_out7;


	resize(act, act_out1, size);
	resize(act, act_out2, size);
	resize(act, act_out3, size);
	resize(act, act_out4, size);
	resize(act, act_out5, size);
	resize(act, act_out6, size);
	resize(act, act_out7, size);


	for (int x = 0; x < 512; x++)
	{
		for (int y = 0; y < 512; y++)
		{
			x1 = (1 * x + p * y) % N;
			y1 = (q*x + (p*q + 1)*y) % N;
			act_out1.at<uint8_t>(x1, y1) = dithImg.at<uint8_t>(x, y);
		}
	}
	for (int x = 0; x < 512; x++)
	{
		for (int y = 0; y < 512; y++)
		{
			x2 = (1 * x + p * y) % N;
			y2 = (q*x + (p*q + 1)*y) % N;
			act_out2.at<uint8_t>(x2, y2) = act_out1.at<uint8_t>(x, y);
		}
	}
	for (int x = 0; x < 512; x++)
	{
		for (int y = 0; y < 512; y++)
		{
			x3 = (1 * x + p * y) % N;
			y3 = (q*x + (p*q + 1)*y) % N;
			act_out3.at<uint8_t>(x3, y3) = act_out2.at<uint8_t>(x, y);
		}
	}
	for (int x = 0; x < 512; x++)
	{
		for (int y = 0; y < 512; y++)
		{
			x4 = (1 * x + p * y) % N;
			y4 = (q*x + (p*q + 1)*y) % N;
			act_out4.at<uint8_t>(x4, y4) = act_out3.at<uint8_t>(x, y);
		}
	}
	for (int x = 0; x < 512; x++)
	{
		for (int y = 0; y < 512; y++)
		{
			x5 = (1 * x + p * y) % N;
			y5 = (q*x + (p*q + 1)*y) % N;
			act_out5.at<uint8_t>(x5, y5) = act_out4.at<uint8_t>(x, y);
		}
	}
	for (int x = 0; x < 512; x++)
	{
		for (int y = 0; y < 512; y++)
		{
			x6 = (1 * x + p * y) % N;
			y6 = (q*x + (p*q + 1)*y) % N;
			act_out6.at<uint8_t>(x6, y6) = act_out5.at<uint8_t>(x, y);
		}
	}
	for (int x = 0; x < 512; x++)
	{
		for (int y = 0; y < 512; y++)
		{
			x7 = (1 * x + p * y) % N;
			y7 = (q*x + (p*q + 1)*y) % N;
			act_out7.at<uint8_t>(x7, y7) = act_out6.at<uint8_t>(x, y);
		}
	}

	

	namedWindow("dithImg", cv::WINDOW_AUTOSIZE);
	imshow("dithImg", dithImg);
	waitKey(0);
	destroyWindow("dithImg");

	namedWindow("act_out7", cv::WINDOW_AUTOSIZE);
	imshow("act_out7", act_out7);
	waitKey(0);
	destroyWindow("act_out7"); 



	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////PROBKA//////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	for (int z = 0; z < 512; z++)	//przejscie po liniach obrazu
	{
		int temp[512];
		int temp2[8];
		int hist[64];
		int t, l = 8;
		int power = 0;

		////////////strumien binarny////////////
		for (int i = 0; i < 512; i++)	//przejscie po kolumnach obrazu
		{
			t = (int)dithImg.at<uint8_t>(z, i);
			if (t == 255)
				t = 1;
			else t = 0;
			temp[i] = t;
			l--;
		}

		///////////zamiana na liczby dziesietne (bajty)////////////
		for (int i = 0; i < 64; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				temp2[j] = temp[j + power];
			}
			power = power + 8;
			hist[i] = zamiana(temp2);
		}

		///////////ZAPIS do pliku/////////////////////////
		ofstream zapis("strumien.txt", ios_base::app);
		for (int i = 0; i < 64; i++)
			zapis << hist[i] << " ";
		zapis.close();
	}

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////PROBKA//////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	for (int z = 0; z < 512; z++)	//przejscie po liniach obrazu
	{
		int temp[512];
		int temp2[8];
		int hist[64];
		int t, l = 8;
		int power = 0;

		////////////strumien binarny////////////
		for (int i = 0; i < 512; i++)	//przejscie po kolumnach obrazu
		{
			t = (int)act_out7.at<uint8_t>(z, i);
			if (t == 255)
				t = 1;
			else t = 0;
			temp[i] = t;
			l--;
		}

		///////////zamiana na liczby dziesietne (bajty)////////////
		for (int i = 0; i < 64; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				temp2[j] = temp[j + power];
			}
			power = power + 8;
			hist[i] = zamiana(temp2);
		}

		///////////ZAPIS do pliku/////////////////////////
		ofstream zapis("strumien_act.txt", ios_base::app);
		for (int i = 0; i < 64; i++)
			zapis << hist[i] << " ";
		zapis.close();
	}

	system("PAUSE");
	return 0;
}