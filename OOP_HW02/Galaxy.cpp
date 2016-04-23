//
// Instructor: Sai-Keung WONG
// Email:	cswingo@cs.nctu.edu.tw
//			wingo.wong@gmail.com
//
// National Chiao Tung University, Taiwan
// Computer Science
// Date: 2016/02/27
//
#include "mySystem_GalaxySystem.h"
#include <iostream>
#include <time.h>
#include <cmath>
#include <vector>
#include <iomanip>
using namespace std;

namespace {
    double getRandDouble( double L, double U ) {
        return L + (U-L)*rand( )/(double) RAND_MAX;
    }
};

GALAXY_SYSTEM::GALAXY_SYSTEM( )
{
    mTimeStep = 0.0025; 
    mMaxV = 100.0;
    mNumOfObjs = 100;
    mNumLargeObjs = 2;
    mMinR = 0.5;            //minimum radius of objects
    mMaxR = 3.0;            //maximum radius of objects
    mSpaceSize = 300.0;
    mG = 5.0;               //gravitational constant
    generateObjects( );
}

/*
Generate randomly the positions,  
velocities and radii of the objects.

Also, if the objects are large, 
set their masses to be a large number.

The mass of an  object depends on the object's radius.

The position of an object is inside 
[-halfSpaceSize, halfSpaceSize]x[-halfSpaceSize, halfSpaceSize].

The velocity of an object is inside
[-100.0, 100]x[-100.0, 100].

Initialize the alive flag of each object.
the alive flag indicates whether the object is alive or not.
If the object is not alive, ignore it in all the calculations.

If there's only one large object,
set its position to the origin
and set its velocity to zero.

*/
void GALAXY_SYSTEM::generateObjects( )
{
    //Fill in here
    // You can remove the following lines.
    mX.resize( mNumOfObjs );
    mY.resize( mNumOfObjs );
    mR.resize( mNumOfObjs );
    mMass.resize( mNumOfObjs );
    mVx.resize( mNumOfObjs );
    mVy.resize( mNumOfObjs );
    mFx.resize( mNumOfObjs );
    mFy.resize( mNumOfObjs );
	mAlive.resize( mNumOfObjs );

	bool sign = false;
	//Random mX and mY
	for (int i = 0; i < mNumOfObjs; i++)
	{
		sign = rand() % 2;
		mX[i] = static_cast<double> (rand() + 0.0) / (static_cast<double> (RAND_MAX + 0.0)) * mSpaceSize / 2;
		if (sign) mX[i] *= -1;
		mY[i] = static_cast<double> (rand() + 0.0) / (static_cast<double> (RAND_MAX + 0.0)) * mSpaceSize / 2;
		if (sign) mY[i] *= -1;
	}
	//Random Vx and Vy
	for (int i = 0; i < mNumOfObjs; i++)
	{
		sign = rand() % 2;
		mVx[i] = static_cast<double> (rand() + 0.0) / (static_cast<double> (RAND_MAX + 0.0)) * mMaxV;
		if (sign) mVx[i] *= -1;
		mVy[i] = static_cast<double> (rand() + 0.0) / (static_cast<double> (RAND_MAX + 0.0)) * mMaxV;
		if (sign) mVy[i] *= -1;
	}
	//Random Radius
	for (int i = 0; i < mNumOfObjs; i++)
	{
		mR[i] = static_cast<double> (rand() + 0.0) / (static_cast<double> (RAND_MAX + 0.0)) * (mMaxR - mMinR);
		mR[i] += mMinR;
	}
	//Random Mass
	for (int i = 0; i < mNumOfObjs; i++)
		mMass[i] = mR[i] * mR[i];
	//Set All Alive True
	for (int i = 0; i < mNumOfObjs; i++)
		mAlive[i] = true;
	//Set LargeObj
	if (mNumLargeObjs == 1)
	{
		mX[0] = 0;
		mY[0] = 0;
		mR[0] = mMaxR * 2.0;
		mMass[0] = mMaxR * mMaxR * 1000;
	}
	else
	{
		for (int i = 0; i < mNumLargeObjs; i++)
		{
			mR[i] = mMaxR * 2.0;
			mMass[i] = mMaxR * mMaxR * 1000;
		}
	}
	

}

/*
Handle the key events.
*/
bool GALAXY_SYSTEM::handleKeyPressedEvent( int key )
{
    bool flgHandled = false;
	switch (key)
	{
	case '1':
		mNumLargeObjs = 1;
		flgHandled = true;
		break;
	case '2' :
		mNumLargeObjs = 2;
		flgHandled = true;
		break;
	case '3' :
		mNumLargeObjs = 3;
		flgHandled = true;
		break;
	case 'r' :
		reset();
		flgHandled = true;
		break;
	}
    return flgHandled;
}

/*
Generate the objects.
Call generateObjects( ).
*/
void GALAXY_SYSTEM::reset( )
{
    generateObjects( );
}

/*
Show the system title.
Show the key usage.
*/
void GALAXY_SYSTEM::askForInput( )
{
    cout << "GALAXY_SYSTEM::askForInput" << endl;
	cout << "Key usuage:" << endl;
	cout << "1: one large object" << endl;
	cout << "2: two large object" << endl;
	cout << "3: three large object" << endl;
	cout << "r: reset" << endl;
}

/*
Return the number of objects
return mNumOfObjs
*/
int GALAXY_SYSTEM::getNumOfObjs( ) const
{
    return mNumOfObjs;
}

/*
Get the object information based on the object index.
(x,y) = position
r = radius
Return the alive flag.
*/
bool GALAXY_SYSTEM::getObjInfo( int objIndex, double &x, double &y, double &r ) const
{
    x = mX[objIndex];
    y = mY[objIndex];
    r = mR[objIndex];
	//if (x <= mSpaceSize / 2 && x >= (-1)*mSpaceSize / 2 && y <= mSpaceSize / 2 && y >= (-1)*mSpaceSize / 2) alive = true;
	return mAlive[objIndex];
	// NOT SURE !!!!!
}

/*
Merge the objects if they overlap with each other.
Modify the velocities of the objects after merging
based on the conservation of momentum.
Set the alive flags of the objects accordingly.

Pseudo-code

For each pair of the objects
  if they do not overlap continue
  If they overlap
    do
      turn off the alive flag of the object with smaller radius
      compute the new velocity of the larger object
*/
void GALAXY_SYSTEM::mergeObjects( )
{
	for (int i = 0; i < mNumOfObjs; ++i)
	{
		if (!mAlive[i]) continue;
		for (int j = i + 1; j < mNumOfObjs; ++j)
		{
			bool merge = false;
			if (!mAlive[j]) continue;
			// right / left
			if (mX[j] > mX[i]) // j is at right of i
			{
				if (mX[i] + mR[i] >= mX[j] - mR[j] && mX[i] + mR[i] <= mX[j] + mR[j]) //Overlap
				{
					if (mR[i] > mR[j])
					{
						mAlive[j] = false;
						mVx[i] = mVx[i] * mMass[i] + mVx[j] * mMass[j];
						mVy[i] = mVy[i] * mMass[i] + mVy[j] * mMass[j];
						mMass[i] += mMass[j];
						mVx[i] /= mMass[i];
						mVy[i] /= mMass[i];
						merge = true;
					}
					else
					{
						mAlive[i] = false;
						mVx[j] = mVx[i] * mMass[i] + mVx[j] * mMass[j];
						mVx[j] = mVx[i] * mMass[i] + mVx[j] * mMass[j];
						mMass[j] += mMass[i];
						mVx[j] /= mMass[j];
						mVy[j] /= mMass[j];
						merge = true;
					}; // Smaller is Killed
				}
			}
			else
			{
				if (mX[j] + mR[j] >= mX[i] - mR[i] && mX[j] + mR[j] <= mX[i] + mR[i] ) // j is at left of i
				{
					if (mR[i] > mR[j])
					{
						mAlive[j] = false;
						mVx[i] = mVx[i] * mMass[i] + mVx[j] * mMass[j];
						mVy[i] = mVy[i] * mMass[i] + mVy[j] * mMass[j];
						mMass[i] += mMass[j];
						mVx[i] /= mMass[i];
						mVy[i] /= mMass[i];
						merge = true;
					}
					else
					{
						mAlive[i] = false;
						mVx[j] = mVx[i] * mMass[i] + mVx[j] * mMass[j];
						mVx[j] = mVx[i] * mMass[i] + mVx[j] * mMass[j];
						mMass[j] += mMass[i];
						mVx[j] /= mMass[j];
						mVy[j] /= mMass[j];
						merge = true;
					}; // Smaller is Killed
				}
			}
			// top / down
			if (mY[j] > mY[i]) // j is at top of i
			{
				if (mY[i] + mR[i] >= mY[j] - mR[j] && mY[i] + mR[i] <= mY[j] + mR[j]) //Overlap
				{
					if (mR[i] > mR[j])
					{
						mAlive[j] = false;
						mVx[i] = mVx[i] * mMass[i] + mVx[j] * mMass[j];
						mVy[i] = mVy[i] * mMass[i] + mVy[j] * mMass[j];
						mMass[i] += mMass[j];
						mVx[i] /= mMass[i];
						mVy[i] /= mMass[i];
						merge = true;
					}
					else
					{
						mAlive[i] = false;
						mVx[j] = mVx[i] * mMass[i] + mVx[j] * mMass[j];
						mVy[j] = mVx[i] * mMass[i] + mVx[j] * mMass[j];
						mMass[j] += mMass[i];
						mVx[j] /= mMass[j];
						mVy[j] /= mMass[j];
						merge = true;
					}; // Smaller is Killed
				}
			}
			else
			{
				if (mY[j] + mR[j] >= mY[i] - mR[i] && mY[j] + mR[j] <= mY[i] + mR[i]) // j is at low of i
				{
					if (mR[i] > mR[j])
					{
						mAlive[j] = false;
						mVx[i] = mVx[i] * mMass[i] + mVx[j] * mMass[j];
						mVy[i] = mVy[i] * mMass[i] + mVy[j] * mMass[j];
						mMass[i] += mMass[j];
						mVx[i] /= mMass[i];
						mVy[i] /= mMass[i];
						merge = true;
					}
					else
					{
						mAlive[i] = false;
						mVx[j] = mVx[i] * mMass[i] + mVx[j] * mMass[j];
						mVy[j] = mVy[i] * mMass[i] + mVy[j] * mMass[j];
						mMass[j] += mMass[i];
						mVx[j] /= mMass[j];
						mVy[j] /= mMass[j];
						merge = true;
					}; // Smaller is Killed
				}
			}
			if (!merge) continue;
		}
	}
	
}

/*
Update the position of the objects
Steps:
1. compute the forces exerting on each object
2. compute the velocity of each object
3. compute the position of each object

Constraints:
The component of a velocity must be inside [-mMaxV, mMaxV].

The component of a position must be inside [-halfSpaceSize, halfSpaceSize].
Warp the position if it's outside of the range.
Consider a position (x,y).
If x > halfSpaceSize, set x = -halfSpaceSize;


*/
void GALAXY_SYSTEM::update( ) 
{
    mergeObjects( ); // merge objects first if they overlap with each other
	
	for (int i = 0; i < mNumOfObjs; ++i)
	{
		if (!mAlive[i]) continue;
		for (int j = i + 1; j < mNumOfObjs; ++j)
		{
			if (!mAlive[j]) continue;

			double d = (mX[i] - mX[j])*(mX[i] - mX[j]) + (mY[i] - mY[j])*(mY[i] - mY[j]);
			d = sqrt(d); // distance

			//compute a unit vector which indicates the force direction
			double dx = (mX[i] - mX[j])*(mX[i] - mX[j]);
			dx = sqrt(dx);
			double dy = (mY[i] - mY[j])*(mY[i] - mY[j]);
			dy = sqrt(dy);
			double nx = dx / d; //normalize along the x-axis
			double ny = dy / d; //normalize along the y-axis

			double mij = mMass[i] * mMass[j];

			mFx[i] -= mij*mG*nx / d*d; 	//notice the negative sign	
			mFy[i] -= mij*mG*ny / d*d;	//notice the negative sign

			mFx[j] += mij*mG*nx / d*d;	//notice the positive sign
			mFy[j] += mij*mG*ny / d*d;  //notice the positive sign

			/*
			//Velocity j
			mVx[j] = mVx[j] + mFx[j] / m*mTimeStep;
			if (mVx[j] >mMaxV) mVx[j] = (-1) * mMaxV;
			else if (mVx[j] < (-1)*mMaxV) mVx[j] = mMaxV;
			mVy[j] = mVy[j] + mFy[j] / m*mTimeStep;
			if (mVy[i] >mMaxV) mVy[j] = (-1) * mMaxV;
			else if (mVy[j] < (-1)*mMaxV) mVy[j] = mMaxV;
			//Position j
			mX[j] = mX[j] + mVx[j] * mTimeStep;
			if (mX[j] > mSpaceSize / 2) mX[j] = (-1) * mSpaceSize / 2;
			else if (mX[j] < (-1)*mSpaceSize / 2) mX[j] = mSpaceSize / 2;
			mY[j] = mY[j] + mVy[j] * mTimeStep;
			if (mY[j] > mSpaceSize / 2) mY[j] = (-1) * mSpaceSize / 2;
			else if (mY[j] < (-1)*mSpaceSize / 2) mY[j] = mSpaceSize / 2;
			*/
		}
	}
	
	for (int i = 0; i < mNumOfObjs; i++)
	{
		double m = mMass[i];
		//Velocity i
		mVx[i] = mVx[i] + mFx[i] / m*mTimeStep;
		mVy[i] = mVy[i] + mFy[i] / m*mTimeStep;
	}
	for (int i = 0; i < mNumOfObjs; i++)
	{
		// Velocity Check
		if (mVx[i] >mMaxV) mVx[i] -= mMaxV;
		if (mVx[i] < (-1)*mMaxV) mVx[i] += mMaxV;
		if (mVy[i] >mMaxV) mVy[i] -= mMaxV;
		if (mVy[i] < ((-1)*mMaxV)) mVy[i] += mMaxV;
	}
	for (int i = 0; i < mNumOfObjs; i++)
	{
		double m = mMass[i];
		//Position i
		mX[i] = mX[i] + mVx[i] * mTimeStep;
		mY[i] = mY[i] + mVy[i] * mTimeStep;
	}
	for (int i = 0; i < mNumOfObjs; i++)
	{
		//Position Check
		if (mX[i] >(mSpaceSize / 2)) mX[i] = -mSpaceSize/2;
		if (mX[i] < ((-1)*mSpaceSize / 2)) mX[i] =/2 mSpaceSize/2;
		if (mY[i] > (mSpaceSize / 2)) mY[i] = -mSpaceSize/2;
		if (mY[i] < ((-1)*mSpaceSize / 2)) mY[i] = mSpaceSize/2;
	}
    //
    //Fill in here
    //compute forces exerting on objects
    //compute velocities of objects
    //compute positions of objects
    //make sure the objects inside the space
}
