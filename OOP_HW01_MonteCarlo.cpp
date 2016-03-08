//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// National Chiao Tung University, Taiwan
// Computer Science
// Date: 2016/02/27
//
#include "mySystem_MonteCarlo.h"
#include <iostream>
#include <time.h>
#include <cmath>
#include <vector>
using namespace std;

#define MAX_SAMPLES 1000000

MONTE_CARLO_SYSTEM::MONTE_CARLO_SYSTEM()
{
	cout.precision(8);
	//
	mRadius = 5.0f;
	mNumSamples = 0;
	computeSamples();
}

void MONTE_CARLO_SYSTEM::askForInput()
{
	//
	//Show the system name
	//Ask to input the radius of the circle
	//Ask to input the number of sample points
	//Call computeSamples( ) to compute the sample points and pi
	//
	cout << "This is a Monte Carlo Simulation" << endl;
	cout << "Please input the radius of the circle" << endl;
	cin >> mRadius;
	cout << "Please input the number of sample points. Maximum number of sample is [2, 1000000]" << endl;
	cin >> mNumSamples;
	spreadSamplePoints();
	computeSamples();
}

void MONTE_CARLO_SYSTEM::computeSamples()
{
	//
	//Compute the coordinates of the sample points.
	//There are mNumSamples samples.
	//Compute pi which is equal to:
	// 4*(number of samples inside the circle / total samples)
	//
	int TotalSamples = 0, SamplesIn = 0;
	double PI = 0.0;
	TotalSamples = getNumSamples();
	bool flgInside = false;
	for (int i = 0; i < TotalSamples; ++i) {
		float x, z;
		flgInside = getSample(i, x, z);
		if (flgInside) {
			SamplesIn++;
		}
	}
	PI = 4 * static_cast<double> (SamplesIn) / static_cast<double>  (TotalSamples);
	cout << "PI:" << PI << endl;
}

double MONTE_CARLO_SYSTEM::getRadius() const
{
	//Return the radius of the circle
	return mRadius;
}

int MONTE_CARLO_SYSTEM::getNumSamples() const
{
	//Return the number of sample points
	return mNumSamples;
}
void MONTE_CARLO_SYSTEM::spreadSamplePoints() 
{
	int totalSample = 0;
	totalSample = getNumSamples();
	cout << totalSample << endl;
	for (int i = 0; i < totalSample; i++)
	{
		mX[i] = static_cast<float> (rand()) / ((RAND_MAX)-1.0f) * mRadius;
		mY[i] = static_cast<float> (rand()) / ((RAND_MAX)-1.0f) * mRadius;
	}

}
bool MONTE_CARLO_SYSTEM::getSample(int sampleIndex, float &x, float &y) const
{
	//
	//Return the sample coordinates in (x, y).
	//Return the boolean value indicating whether the sample lying inside the circle.
	//
	bool flgInside = false;
	float radius = 0.0;

	radius = getRadius();

	x = mX[sampleIndex];
	y = mY[sampleIndex];
	cout << x << " "<< y << endl;
	if (abs(y) <= sqrt(pow(radius, 2) - pow(x, 2))) flgInside = true;
	//cout << flgInside << endl;
	return flgInside;
}
