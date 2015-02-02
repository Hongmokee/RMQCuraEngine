#ifndef AUTOSPEED_H
#define AUTOSPEED_H

#include <vector>
#include <stdio.h>
#include <math.h>
#include <string.h>

extern FILE* EachLayer;

namespace autospeed{

	class AutoSpeedSetting
	{
	public:
		std::vector<int> EachLayerCount;
		int line=0;

		bool readEachCount() {
			char buffer[100];
			int count;

			if (EachLayer == nullptr) return false;

			while(fgets(buffer, sizeof(buffer), EachLayer))
			{
				sscanf(buffer,"%d", &count);
				EachLayerCount.push_back(count);
				line++;
			}
			return true;
		}

		int setEachspeed(int layer, int layerThickness){
			int divition = 2*(1000/layerThickness);

			if(EachLayerCount.empty()||(line-1)<static_cast<int>(layer/divition))
			{
				return 100;
			}
			int count = EachLayerCount.at(static_cast<int>(layer/divition));
			float speed = (0.164*sqrt(count*4)+9.672);

			return speed / 35.0*100;
		}

		int setEachSpeed_New(double eachArea)
		{
			float speed = (0.164*sqrt(eachArea)+9.672);
			return speed / 35.0*100;
		}
	};
}
#endif//AUTOSPEED_H
