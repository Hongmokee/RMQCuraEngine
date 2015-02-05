#ifndef AUTOSPEED_H
#define AUTOSPEED_H

#include <vector>
#include <stdio.h>
#include <math.h>
#include <string.h>

extern int EachLayer;

namespace autospeed{

	class AutoSpeedSetting
	{
	public:
		int line=0;

		double logB(double x, double base) {
		  return log(x) / log(base);
		}

		int setEachSpeed_New(double eachArea)
		{
			//float speed = 0.3*(3.44*logB(eachArea,4)+10)+0.7*(0.164*sqrt(eachArea)+9.672);
			float speed = (0.164*sqrt(eachArea)+9.672);
			if(EachLayer==1)
				return speed / 35.0*100;
			else
				return 100;
		}
	};
}
#endif//AUTOSPEED_H
