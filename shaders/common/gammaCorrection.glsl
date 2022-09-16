#ifndef GAMMA_CORRECTION_H
#define GAMMA_CORRECTION_H

//
vec3 applyGammaCorrection(vec3 finalColor)
{
	float gamma = 2.2;
	return pow(finalColor, vec3(1.0/gamma));
}

#endif