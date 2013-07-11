#include "dew_point.h"
#include <math.h>


// from HYT271 reference c code
/**		-> dew point calculation with Magnus formula
 **
 **
 **		Parameter	type		name							mission
 **		------------------------------------------------------------------------------
 **				1:	double		dTemperatureInDegreeCelsius		air temperature in [°C]
 **				2:	double		dRelativeHumidity				relative humidity in [%]
 **		   
 **		   return:	double		dResult							dew point based on given temperature and humidity in [°C]
 **
 **.........................................................
 **		point of origin:
 **
 **							  (	17.5043 * dp )
 **			E(dp) = E(0) * exp(--------------)		Magnus formula
 **							  (	241.2°C + dp )
 **
 **					E(dp)
 **			phi = -------- * 100%					value correlation
 **					E(T)
 **
 **
 **		both formulas solved to the DEW POINT:
 **
 **							  (	phi )	 4222.03716 * T
 **					241.2 * ln(-----) + ----------------
 **							  ( 100 )		241.2 + T
 **			dp =  ----------------------------------------
 **							  	( phi )	   17.5043 * T
 **					17.5043 - ln(-----) - --------------
 **							  	( 100 )		241.2 + T
 **
 **
 **		parameter legend:
 **
 **			E	:=	saturation vapour pressure	[ hPa ]
 **			dp	:=	dew point temperature		[  °C ]
 **			phi	:=	relative humidity			[   % ]
 **			T	:=	air temperature				[  °C ]
 **
 **/


double dew_point(double air_temperature, double rel_humidity) {
  //more than one time needed values
  double dTempConstant_1 = log( (double)(rel_humidity / 100) );
  double dTempConstant_2 = air_temperature + 241.2;
  //above slash
  double dResult = 4222.03716 / dTempConstant_2 * air_temperature + 241.2 * dTempConstant_1;
  //below slash
  dResult /= 17.5043 - 17.5043 * air_temperature / dTempConstant_2 - dTempConstant_1;
  return dResult;
}
