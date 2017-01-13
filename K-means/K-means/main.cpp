#include <iostream>
#include <vector>
#include <deque>
#include <algorithm> 

#include "K_means.h"
#include "StopConditions.h"
#include "Point_2D.h"
#include "IntTest.h"
#include "bitmap_image.hpp"

void IntegerScenario_Class();
void Point2DScenario_Class();
void BitmapScenario(int k, string inputFileName, string outputFileName);

int main(int argc, char** argv)
{
	srand(time(NULL));

	//IntegerScenario_Class();
	Point2DScenario_Class();
	//BitmapScenario_Func(2, "ExampleData/input01.bmp", "ExampleData/outputK2.bmp");
	/*BitmapScenario_Func(3, "ExampleData/input01.bmp", "ExampleData/outputK3.bmp");
	BitmapScenario_Func(5, "ExampleData/input01.bmp", "ExampleData/outputK5.bmp");
	BitmapScenario_Func(5, "ExampleData/input01.bmp", "ExampleData/outputK5_1.bmp");*/
	//BitmapScenario_Func(4, "ExampleData/random.bmp", "ExampleData/random1.bmp");
	//BitmapScenario_Func(5, "ExampleData/random.bmp", "ExampleData/random2.bmp");
	//BitmapScenario_Func(10, "ExampleData/random.bmp", "ExampleData/random3.bmp");

	cout << "Koniec przetwarzania" << endl;
	cin.get();
	return 0;
}

void IntegerScenario_Class()
{
	K_means<int> k_means;

	vector<int>::iterator* result;

	vector<int> vec = vector<int>();

	vec.push_back(3);
	vec.push_back(-5);
	vec.push_back(10);
	vec.push_back(4);
	vec.push_back(0);
	vec.push_back(-6);
	vec.push_back(-9);
	vec.push_back(9);

	k_means.DisplayCollection(vec.begin(), vec.end());

	result = k_means.Group(vec.begin(), vec.end(), Int_distance(), Int_average(), 3, 2, MaxIterations, true);

	k_means.DisplayCollection(vec.begin(), vec.end());
}

void Point2DScenario_Class()
{
	K_means<Point_2D> k_means;

	vector<Point_2D> vec = vector<Point_2D>();

	vec.push_back(Point_2D(0, 0));
	vec.push_back(Point_2D(3, 3));
	vec.push_back(Point_2D(-2, 0));
	vec.push_back(Point_2D(-3, 5));
	vec.push_back(Point_2D(2, 1));
	vec.push_back(Point_2D(0, 4));
	vec.push_back(Point_2D(-5, 5));
	vec.push_back(Point_2D(-2, -3));

	k_means.DisplayCollection(vec.begin(), vec.end());

	auto result = k_means.Group(vec.begin(), vec.end(), Point2D_distance(), Point2D_average(), 3, 2, StableState, true);

	k_means.DisplayCollection(vec.begin(), vec.end());
}

void BitmapScenario(int k, string inputFileName, string outputFileName)
{
	bitmap_image image(inputFileName);
	rgb_t colour;

	K_means<Point_2D> k_means;
	vector<Point_2D>::iterator* result;

	vector<Point_2D> vec = vector<Point_2D>();

	if (!image)
	{
		cout << "Nie uda�o si� otworzy� pliku: " << inputFileName.c_str() << endl;
		return;
	}

	// read only non-white (not #FFFFFF) pixels
	for (unsigned int i = 0; i < image.width(); i++)
	{
		for (unsigned int j = 0; j < image.height(); j++)
		{
			image.get_pixel(i, j, colour);

			if (colour.blue < 255 || colour.red < 255 || colour.green < 255)
				vec.push_back(Point_2D(i, j));
		}
	}

	// computing
	result = k_means.Group(vec.begin(), vec.end(), Point2D_distance(), Point2D_average(), 10, k, Both);

	// coloring
	for (int i = 0; i < k; i++)
	{
		for (std::vector<Point_2D>::iterator it = result[i]; (i == (k - 1)) ? it != vec.end() : it != result[i + 1]; it++)
		{
			image.set_pixel((int)it->x, (int)it->y, palette_colormap[50 / k * i]);
		}
	}
	image.save_image(outputFileName);

	cout << "Zapisano obraz " << outputFileName << endl;
}
