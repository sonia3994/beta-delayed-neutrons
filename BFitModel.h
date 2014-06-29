// Always enclose header file contents with these ifndef/endif directives.
#ifndef _BFitModel_h
#define _BFitModel_h

#include "Rtypes.h"
#include "TString.h"

namespace BFitNamespace {
	
	enum ParIndex { dt, DC, r1, r2, r3, p, rho, epsT, epsU, epsV, epsW, epsX, epsY, tau1, tau2, tau3, tauT1, tauT2, tauT3, tauU1, tauU2, tauU3 };
	// a[dt]	= bin width in ms
	// a[DC]	= DC detection rate in cycles/ms
	// a[r1]	= Species 1 injection rate in cycles/ms
	// a[r2]	= Species 2 injection rate in cycles/ms
	// a[r3]	= Species 3 injection rate in cycles/ms
	// a[p]		= Fraction of injected ions that is succesfully trapped
	// a[rho]	= Fraction of already-trapped ions that is retained in trap at each capture
	// a[epsT]	= Detection efficiency for trapped pop T
	// a[epsU]	= Detection efficiency for untrapped pop U
	// a[epsV]	= Detection efficiency for untrapped pop V
	// a[epsW]	= Detection efficiency for untrapped pop W
	// a[epsX]	= Detection efficiency for untrapped pop V
	// a[epsY]	= Detection efficiency for untrapped pop W
	// a[tau1]	= Radioactive lifetime of species 1 isotopes (ms)
	// a[tau2]	= Radioactive lifetime of species 2 isotopes (ms)
	// a[tau3]	= Radioactive lifetime of species 3 isotopes (ms)
	// a[tauT1]	= Net lifetime of trapped species 1 population -- radioactive and other effects (T1) (ms)
	// a[tauT2]	= Net lifetime of trapped species 2 population -- radioactive and other effects (T2) (ms)
	// a[tauT3]	= Net lifetime of trapped species 3 population -- radioactive and other effects (T3) (ms)
	// a[tauU1]	= Net lifetime of untrapped species 1 population -- radioactive and other effects (U1) (ms)
	// a[tauU2]	= Net lifetime of untrapped species 2 population -- radioactive and other effects (U2) (ms)
	// a[tauU3]	= Net lifetime of untrapped species 3 population -- radioactive and other effects (U3) (ms)
	
// Trapped and untrapped populations 1, 2, 3
	Double_t T1 (Double_t*, Double_t*);
	Double_t T2 (Double_t*, Double_t*);
	Double_t T3 (Double_t*, Double_t*);
	Double_t U1 (Double_t*, Double_t*);
	Double_t U2 (Double_t*, Double_t*);
	Double_t U3 (Double_t*, Double_t*);
	Double_t All (Double_t*, Double_t*);
	
// Functions to plot: (obs. decay rate)x(bin dt) = counts by bin
	Double_t yDC (Double_t*, Double_t*);
	Double_t yT1 (Double_t*, Double_t*);
	Double_t yT2 (Double_t*, Double_t*);
	Double_t yT3 (Double_t*, Double_t*);
	Double_t yU1 (Double_t*, Double_t*);
	Double_t yU2 (Double_t*, Double_t*);
	Double_t yU3 (Double_t*, Double_t*);
	Double_t yAll (Double_t*, Double_t*);
	
// Detection rates (/ms) for calculating N-beta
	Double_t rDC (Double_t*, Double_t*);
	Double_t rT1 (Double_t*, Double_t*);
	Double_t rT2 (Double_t*, Double_t*);
	Double_t rT3 (Double_t*, Double_t*);
	Double_t rU1 (Double_t*, Double_t*);
	Double_t rU2 (Double_t*, Double_t*);
	Double_t rU3 (Double_t*, Double_t*);
	Double_t rAll (Double_t*, Double_t*);
	
// Offset functions to imporve visualization: offT1 = yT1 + yDC
	Double_t oT1 (Double_t*, Double_t*);
	Double_t oT2 (Double_t*, Double_t*);
	Double_t oT3 (Double_t*, Double_t*);
	Double_t oU1 (Double_t*, Double_t*);
	Double_t oU2 (Double_t*, Double_t*);
	Double_t oU3 (Double_t*, Double_t*);
	
// Functions for evaluating and plotting the V, W, X, and Y pops	
	Double_t V1 (Double_t*, Double_t*);
	Double_t V2 (Double_t*, Double_t*);
	Double_t V3 (Double_t*, Double_t*);
	Double_t W1 (Double_t*, Double_t*);
	Double_t W2 (Double_t*, Double_t*);
	Double_t W3 (Double_t*, Double_t*);
	Double_t X2 (Double_t*, Double_t*);
	Double_t X3 (Double_t*, Double_t*);
	Double_t Y2 (Double_t*, Double_t*);
	Double_t Y3 (Double_t*, Double_t*);
	Double_t yV1 (Double_t*, Double_t*);
	Double_t yV2 (Double_t*, Double_t*);
	Double_t yV3 (Double_t*, Double_t*);
	Double_t yW1 (Double_t*, Double_t*);
	Double_t yW2 (Double_t*, Double_t*);
	Double_t yW3 (Double_t*, Double_t*);
	Double_t yX2 (Double_t*, Double_t*);
	Double_t yX3 (Double_t*, Double_t*);
	Double_t yY2 (Double_t*, Double_t*);
	Double_t yY3 (Double_t*, Double_t*);
	
}

// Variables to hold integrals of functions
Double_t T1_integral = 0.0;
Double_t T2_integral = 0.0;
Double_t T3_integral = 0.0;
Double_t U1_integral = 0.0;
Double_t U2_integral = 0.0;
Double_t U3_integral = 0.0;
Double_t DC_integral = 0.0;
Double_t All_integral = 0.0;

Double_t Integral_sum = 0.0;

Double_t T1_integral_error = 0.0;
Double_t T2_integral_error = 0.0;
Double_t T3_integral_error = 0.0;
Double_t U1_integral_error = 0.0;
Double_t U2_integral_error = 0.0;
Double_t U3_integral_error = 0.0;
Double_t DC_integral_error = 0.0;
Double_t All_integral_error = 0.0;

Double_t Integral_sum_error = 0.0;

#endif