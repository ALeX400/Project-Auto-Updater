#if defined _math_included
	#endinput
#endif
#define _math_included


#define PI 3.14159265358979323846

stock Float:sin(Float:angle)
{
	// Converteste unghiul din grade in radiani
	angle = angle * PI / 180.0;

	// Implementarea simplificată a funcției sin
	return angle - (angle * angle * angle) / 6.0 + (angle * angle * angle * angle * angle) / 120.0;
}

stock Float:cos(Float:angle)
{
	// Converteste unghiul din grade in radiani
	angle = angle * PI / 180.0;

	// Implementarea simplificată a funcției cos
	return 1.0 - (angle * angle) / 2.0 + (angle * angle * angle * angle) / 24.0;
}