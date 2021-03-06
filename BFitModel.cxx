#include "BFitModel.h"
#include "CSVtoStruct.h"
#include "TMath.h"
#include "string.h"

Double_t BFitNamespace::SigmaT (Double_t rho, Double_t tau, Int_t n) {
	using namespace TMath;
	extern Double_t tCap;
	static Double_t a;
	a = Exp(-tCap/tau);
	return ( 1-Power(rho*a,n) ) / (1-rho*a);
//	return ( Exp(n*tCap/tau) - Power(rho,n) ) / ( Exp(tCap/tau) - rho );
}
Double_t BFitNamespace::T1 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	using namespace TMath;
	static Int_t n;
	static Double_t ST1, tT1, f;
	extern Double_t tCap, tBac, tCyc, t1, t2, t3;
	f = 0.0; //catch bad values of t[0]
	if (tBac <= t[0] && t[0] <= tCyc)
	{
		tT1 = 1.0 / ( 1.0/t1 + a[gammaT1]/1000.0 ); // net variable lifetime (1/e) in ms
		n = Ceil((t[0]-tBac)/tCap);
		f = a[p] * a[r1] * tCap * SigmaT(a[rho],tT1,n) * Exp(-(t[0]-tBac-(n-1)*tCap)/tT1);
	}
	return f;
}

Double_t BFitNamespace::T2 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	using namespace TMath;
	static Int_t n;
	static Double_t ST2, tT2, f;
	extern Double_t tCap, tBac, tCyc, t2;
	f = 0.0; //catch bad values of t[0]
	if (tBac <= t[0] && t[0] <= tCyc)
	{
		tT2 = 1.0 / ( 1.0/t2 + a[gammaT2]/1000.0 ); // net variable lifetime (1/e) in ms
		n = Ceil((t[0]-tBac)/tCap);
		f = a[p] * a[r2] * tCap * SigmaT(a[rho],tT2,n) * Exp(-(t[0]-tBac-(n-1)*tCap)/tT2);
	}
	return f;
}

Double_t BFitNamespace::T3 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	using namespace TMath;
	static Int_t n;
	static Double_t ST3, tT3, f;
	extern Double_t tCap, tBac, tCyc, t3;
	extern BDNCase_t stBDNCases[FILE_ROWS_BDN]; // This comes from the main program (BFit.cxx)
	extern Int_t iBDNCaseIndex; // This comes from the main program (BFit.cxx)
	extern bool b134sbFlag;
	f = 0.0; //catch bad values of t[0]
	if (tBac <= t[0] && t[0] <= tCyc)
	{
		if (b134sbFlag) a[gammaT3] = a[gammaT2];
		tT3 = 1.0 / ( 1.0/t3 + a[gammaT3]/1000.0 ); // net variable lifetime (1/e) in ms
		n = Ceil((t[0]-tBac)/tCap);
		f = a[p] * a[r3] * tCap * SigmaT(a[rho],tT3,n) * Exp(-(t[0]-tBac-(n-1)*tCap)/tT3);
	}
	return f;
}

Double_t BFitNamespace::U1 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	return V1(t,a) + W1(t,a);
}
Double_t BFitNamespace::U2 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	return V2(t,a) + W2(t,a) + X2(t,a) + Y2(t,a);
}
Double_t BFitNamespace::U3 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	return V3(t,a) + W3(t,a) + X3(t,a) + Y3(t,a);
}

//////////////////////////////////////////////////////////////////////////
// "y" functions
// Functions to plot: (obs. decay rate)x(bin dt) = counts by bin = y
//////////////////////////////////////////////////////////////////////////
Double_t BFitNamespace::yDC (Double_t *t, Double_t *a) { return a[nCyc]*a[dt]*a[DC]; }
Double_t BFitNamespace::yT1 (Double_t *t, Double_t *a) { return a[nCyc]*a[dt]*BFitNamespace::rT1(t,a); }
Double_t BFitNamespace::yT2 (Double_t *t, Double_t *a) { return a[nCyc]*a[dt]*BFitNamespace::rT2(t,a); }
Double_t BFitNamespace::yT3 (Double_t *t, Double_t *a) { return a[nCyc]*a[dt]*BFitNamespace::rT3(t,a); }
Double_t BFitNamespace::yU1 (Double_t *t, Double_t *a) { return a[nCyc]*a[dt]*BFitNamespace::rU1(t,a); }
Double_t BFitNamespace::yU2 (Double_t *t, Double_t *a) { return a[nCyc]*a[dt]*BFitNamespace::rU2(t,a); }
Double_t BFitNamespace::yU3 (Double_t *t, Double_t *a) { return a[nCyc]*a[dt]*BFitNamespace::rU3(t,a); }
Double_t BFitNamespace::yAll(Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	return a[nCyc]*a[dt]*a[DC] + yT1(t,a) + yT2(t,a) + yT3(t,a) + yU1(t,a) + yU2(t,a) + yU3(t,a);
}

//////////////////////////////////////////////////////////////////////////
// "o" functions
// Offset functions to improve visualization: oT1 = yT1 + yDC
//////////////////////////////////////////////////////////////////////////
Double_t BFitNamespace::oT1 (Double_t *t, Double_t *a) { return BFitNamespace::yDC(t,a) + BFitNamespace::yT1(t,a); }
Double_t BFitNamespace::oT2 (Double_t *t, Double_t *a) { return BFitNamespace::yDC(t,a) + BFitNamespace::yT2(t,a); }
Double_t BFitNamespace::oT3 (Double_t *t, Double_t *a) { return BFitNamespace::yDC(t,a) + BFitNamespace::yT3(t,a); }
Double_t BFitNamespace::oU1 (Double_t *t, Double_t *a) { return BFitNamespace::yDC(t,a) + BFitNamespace::yU1(t,a); }
Double_t BFitNamespace::oU2 (Double_t *t, Double_t *a) { return BFitNamespace::yDC(t,a) + BFitNamespace::yU2(t,a); }
Double_t BFitNamespace::oU3 (Double_t *t, Double_t *a) { return BFitNamespace::yDC(t,a) + BFitNamespace::yU3(t,a); }
//Double_t BFitNamespace::oT1 (Double_t *t, Double_t *a) { return a[nCyc]*a[dt]*(a[DC] + BFitNamespace::rT1(t,a)); }
//Double_t BFitNamespace::oT2 (Double_t *t, Double_t *a) { return a[nCyc]*a[dt]*(a[DC] + BFitNamespace::rT2(t,a)); }
//Double_t BFitNamespace::oT3 (Double_t *t, Double_t *a) { return a[nCyc]*a[dt]*(a[DC] + BFitNamespace::rT3(t,a)); }
//Double_t BFitNamespace::oU1 (Double_t *t, Double_t *a) { return a[nCyc]*a[dt]*(a[DC] + BFitNamespace::rU1(t,a)); }
//Double_t BFitNamespace::oU2 (Double_t *t, Double_t *a) { return a[nCyc]*a[dt]*(a[DC] + BFitNamespace::rU2(t,a)); }
//Double_t BFitNamespace::oU3 (Double_t *t, Double_t *a) { return a[nCyc]*a[dt]*(a[DC] + BFitNamespace::rU3(t,a)); }

//////////////////////////////////////////////////////////////////////////
// "r" functions
// Instantaneous detection rate, not corrected for nCyc
// Used for calculating the desired integrals... apply nCyc to those
//////////////////////////////////////////////////////////////////////////
Double_t BFitNamespace::rDC (Double_t *t, Double_t *a) {
	return a[DC];
}
Double_t BFitNamespace::rT1 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	extern Double_t t1;
	return a[epsT]*T1(t,a)/t1;
}
Double_t BFitNamespace::rT2 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	extern Double_t t2;
	return a[epsT]*T2(t,a)/t2;
}
Double_t BFitNamespace::rT3 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	extern Double_t t3;
	return a[epsT]*T3(t,a)/t3;
}
Double_t BFitNamespace::rU1 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	extern Double_t t1;
	return (a[epsV]*V1(t,a) + a[epsW]*W1(t,a))/t1;
}
Double_t BFitNamespace::rU2 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	extern Double_t t2;
	return (a[epsV]*V2(t,a) + a[epsW]*W2(t,a) + a[epsX]*X2(t,a) + a[epsY]*Y2(t,a))/t2;
}
Double_t BFitNamespace::rU3 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	extern Double_t t3;
	return (a[epsV]*V3(t,a) + a[epsW]*W3(t,a) + a[epsX]*X3(t,a) + a[epsY]*Y3(t,a))/t3;
}
Double_t BFitNamespace::rAll (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	return a[DC] + rT1(t,a) + rT2(t,a) + rT3(t,a) + rU1(t,a) + rU2(t,a) + rU3(t,a);
}

Double_t BFitNamespace::V1 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	using namespace TMath;
	static Int_t n, N;
	static Double_t tU1, v10, v1, V1, f;
	extern Double_t tCap, tBac, tCyc, t1;
	tU1 = 1.0 / ( 1.0/t1 + a[gammaU1]/1000.0 ); // net variable lifetime (1/e) in ms
	f = 0.0; //catch bad values of t[0]
	n = Ceil((t[0]-tBac)/tCap);
	N = Ceil((tCyc-tBac)/tCap);
	// Untrapped populations at t=0
	v10 = ( a[r1] * tCap * (1-a[p]) * SigmaT(1,tU1,N) * Exp(-(tCyc-tBac-(N-1)*tCap)/tU1) ) / ( 1 - Exp(-tCyc/tU1) );
	// Untrapped population during background period
	v1 = v10 * Exp(-t[0]/tU1);
	// Background period
	if (0 <= t[0] && t[0] < tBac) {
		f = v1;
	}
	// Trapping period
	if (tBac <= t[0] && t[0] <= tCyc) {
		// Untrapped populations during trapping
		V1 = a[r1] * tCap * (1-a[p]) * SigmaT(1,tU1,n) * Exp(-(t[0]-tBac-(n-1)*tCap)/tU1);
		f = v1+V1;
	}
	return f;
}
Double_t BFitNamespace::V2 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	using namespace TMath;
	static Int_t n, N;
	static Double_t tU2, v20, v2, V2, f;
	extern Double_t tCap, tBac, tCyc, t2;
	tU2 = 1.0 / ( 1.0/t2 + a[gammaU2]/1000.0 ); // net variable lifetime (1/e) in ms
	f = 0.0; //catch bad values of t[0]
	n = Ceil((t[0]-tBac)/tCap);
	N = Ceil((tCyc-tBac)/tCap);
	// Untrapped populations at t=0
	v20 = ( a[r2] * tCap * (1-a[p]) * SigmaT(1,tU2,N) * Exp(-(tCyc-tBac-(N-1)*tCap)/tU2) ) / ( 1 - Exp(-tCyc/tU2) );
	// Untrapped population during background period
	v2 = v20 * Exp(-t[0]/tU2);
	// Background period
	if (0 <= t[0] && t[0] < tBac) {
		f = v2;
	}
	// Trapping period
	if (tBac <= t[0] && t[0] <= tCyc) {
		// Untrapped populations during trapping
		V2 = a[r2] * tCap * (1-a[p]) * SigmaT(1,tU2,n) * Exp(-(t[0]-tBac-(n-1)*tCap)/tU2);
		f = v2+V2;
	}
	return f;
}
Double_t BFitNamespace::V3 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	using namespace TMath;
	static Int_t n, N;
	static Double_t tU3, v30, v3, V3, f;
	extern Double_t tCap, tBac, tCyc, t3;
	tU3 = 1.0 / ( 1.0/t3 + a[gammaU3]/1000.0 ); // net variable lifetime (1/e) in ms
	f = 0.0; //catch bad values of t[0]
	n = Ceil((t[0]-tBac)/tCap);
	N = Ceil((tCyc-tBac)/tCap);
	// Untrapped populations at t=0
	v30 = ( a[r3] * tCap * (1-a[p]) * SigmaT(1,tU3,N) * Exp(-(tCyc-tBac-(N-1)*tCap)/tU3) ) / ( 1 - Exp(-tCyc/tU3) );
	// Untrapped population during background period
	v3 = v30 * Exp(-t[0]/tU3);
	// Background period
	if (0 <= t[0] && t[0] < tBac) {
		f = v3;
	}
	// Trapping period
	if (tBac <= t[0] && t[0] <= tCyc) {
		// Untrapped populations during trapping
		V3 = a[r3] * tCap * (1-a[p]) * SigmaT(1,tU3,n) * Exp(-(t[0]-tBac-(n-1)*tCap)/tU3);
		f = v3+V3;
	}
	return f;
}

Double_t BFitNamespace::yV1 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	extern Double_t t1;
	return a[nCyc]*a[dt]*a[epsV]*V1(t,a)/t1;
}
Double_t BFitNamespace::yV2 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	extern Double_t t2;
	return a[nCyc]*a[dt]*a[epsV]*V2(t,a)/t2;
}
Double_t BFitNamespace::yV3 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	extern Double_t t3;
	return a[nCyc]*a[dt]*a[epsV]*V3(t,a)/t3;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// W populations
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Double_t BFitNamespace::SigmaW (Double_t rho, Double_t tT, Double_t tU, Int_t n) {
	using namespace TMath;
	static Double_t expT, expU, f;
	static Int_t k;
	extern Double_t iota, tCap;
	f = 0.0;
	if (n>=2) {
		expT = Exp(-tCap/tT);
		expU = Exp(-tCap/tU);
		f = 1.0/(rho*(expU-1.0)) * ( Power(expU,n)*(Power(rho*expT/expU,n)-rho*expT/expU)/(rho*expT/expU-1.0) - (Power(rho*expT,n)-rho*expT)/(rho*expT-1.0) );
		//for (k=1; k<=n-1; k++) f += SigmaT(1,tU,n-k) * TMath::Power(rho*TMath::Exp(-tCap/tT),k);
//		f = 1.0/(rho*(Exp(-tCap/tU)-1.0)) * ( Exp(-n*tCap/tU)*(Power(rho*Exp(-tCap*(1.0/tT-1.0/tU)),n)-rho*Exp(-tCap*(1.0/tT-1.0/tU)))/(rho*Exp(-tCap*(1.0/tT-1.0/tU))-1.0) - (Power(rho*Exp(-tCap/tT),n)-rho*Exp(-tCap/tT))/(rho*Exp(-tCap/tT)-1.0) );
	}
//	f *= 1.0/(rho+iota);
	return f;
}
Double_t BFitNamespace::W1 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	using namespace TMath;
// externs: constant over all computations
	extern Double_t iota, tCap, tBac, tCyc, t1;
// statics: recomputed many times (keep memory allocated)
	static Int_t n, N;
	static Double_t tT1, tU1, w10, w1, W1, f, amplitude;
// Initialize function value
	f = 0.0;
// Recompute variables
	tT1 = 1.0 / ( 1.0/t1 + a[gammaT1]/1000.0 ); // net variable lifetime (1/e) in ms
	tU1 = 1.0 / ( 1.0/t1 + a[gammaU1]/1000.0 ); // net variable lifetime (1/e) in ms
	n = Ceil((t[0]-tBac)/tCap);
	N = Ceil((tCyc-tBac)/tCap);
	amplitude = (1-a[rho]) * a[p] * a[r1] * tCap;
// Initial value (t=0)
	w10 = amplitude * SigmaW(a[rho],tT1,tU1,N) * Exp(-(tCyc-tBac-(N-1)*tCap)/tU1) / (1 - Exp(-tCyc/tU1) );
// Background solution
	w1 = w10 * Exp(-t[0]/tU1);
// Background period
	if (0 <= t[0] && t[0] < tBac + tCap) { // Different from other pops: No effect from 1st injection
		f = w1;
	}
	// Trapping period
	if (tBac + tCap <= t[0] && t[0] <= tCyc) {
		// Untrapped populations during trapping
		W1 = amplitude * SigmaW(a[rho],tT1,tU1,n) * Exp(-(t[0]-tBac-(n-1)*tCap)/tU1);
		f = w1 + W1;
	}
	return f;
}
Double_t BFitNamespace::W2 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	using namespace TMath;
// externs: constant over all computations
	extern Double_t iota, tCap, tBac, tCyc, t2;
// statics: recomputed many times (keep memory allocated)
	static Int_t n, N;
	static Double_t tT2, tU2, w20, w2, W2, f, amplitude;
// Initialize function value
	f = 0.0;
// Recompute variables
	tT2 = 1.0 / ( 1.0/t2 + a[gammaT2]/1000.0 ); // net variable lifetime (1/e) in ms
	tU2 = 1.0 / ( 1.0/t2 + a[gammaU2]/1000.0 ); // net variable lifetime (1/e) in ms
	n = Ceil((t[0]-tBac)/tCap);
	N = Ceil((tCyc-tBac)/tCap);
	amplitude = (1-a[rho]) * a[p] * a[r2] * tCap;
// Initial value (t=0)
	w20 = amplitude * SigmaW(a[rho],tT2,tU2,N) * Exp(-(tCyc-tBac-(N-1)*tCap)/tU2) / (1 - Exp(-tCyc/tU2) );
// Background solution
	w2 = w20 * Exp(-t[0]/tU2);
// Background period
	if (0 <= t[0] && t[0] < tBac + tCap) { // Different from other pops: No effect from 1st injection
		f = w2;
	}
	// Trapping period
	if (tBac + tCap <= t[0] && t[0] <= tCyc) {
		// Untrapped populations during trapping
		W2 = amplitude * SigmaW(a[rho],tT2,tU2,n) * Exp(-(t[0]-tBac-(n-1)*tCap)/tU2);
		f = w2 + W2;
	}
	return f;
}
Double_t BFitNamespace::W3 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	using namespace TMath;
// externs: constant over all computations
	extern Double_t iota, tCap, tBac, tCyc, t3;
// statics: recomputed many times (keep memory allocated)
	static Int_t n, N;
	static Double_t tT3, tU3, w30, w3, W3, f, amplitude;
// Initialize function value
	f = 0.0;
// Recompute variables
	tT3 = 1.0 / ( 1.0/t3 + a[gammaT3]/1000.0 ); // net variable lifetime (1/e) in ms
	tU3 = 1.0 / ( 1.0/t3 + a[gammaU3]/1000.0 ); // net variable lifetime (1/e) in ms
	n = Ceil((t[0]-tBac)/tCap);
	N = Ceil((tCyc-tBac)/tCap);
	amplitude = (1-a[rho]) * a[p] * a[r3] * tCap;
// Initial value (t=0)
	w30 = amplitude * SigmaW(a[rho],tT3,tU3,N) * Exp(-(tCyc-tBac-(N-1)*tCap)/tU3) / (1 - Exp(-tCyc/tU3) );
// Background solution
	w3 = w30 * Exp(-t[0]/tU3);
// Background period
	if (0 <= t[0] && t[0] < tBac + tCap) { // Different from other pops: No effect from 1st injection
		f = w3;
	}
	// Trapping period
	if (tBac + tCap <= t[0] && t[0] <= tCyc) {
		// Untrapped populations during trapping
		W3 = amplitude * SigmaW(a[rho],tT3,tU3,n) * Exp(-(t[0]-tBac-(n-1)*tCap)/tU3);
		f = w3 + W3;
	}
	return f;
}
Double_t BFitNamespace::yW1 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	extern Double_t t1;
	return a[nCyc]*a[dt]*a[epsW]*W1(t,a)/t1;
}
Double_t BFitNamespace::yW2 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	extern Double_t t2;
	return a[nCyc]*a[dt]*a[epsW]*W2(t,a)/t2;
}
Double_t BFitNamespace::yW3 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	extern Double_t t3;
	return a[nCyc]*a[dt]*a[epsW]*W3(t,a)/t3;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Z populations
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Double_t BFitNamespace::SigmaZ (Double_t rho, Double_t tT, Double_t tU, Int_t n) {
	using namespace TMath;
	static Double_t expT, expU, f;
	static Int_t k;
	extern Double_t tCap;
	f = 0.0;
	if (n>=2) {
		expT = Exp(-tCap/tT);
		expU = Exp(-tCap/tU);
		f = (expU-expT) * Power(expU,n-1)/(1.0-rho*expT) * ( (Power(1/expU,n)-1/expU)/(1/expU-1.0) - (Power(rho*expT/expU,n)-rho*expT/expU)/(rho*expT/expU-1.0) );
//		for (k=1; k<=n-1; k++) f += SigmaT(rho,tT,k) * TMath::Exp(-(n-k-1)*tCap/tU);
//		f = Exp(-(n-1)*tCap/tU)/(1.0-rho*Exp(-tCap/tT)) * ( (Exp(n*tCap/tU)-Exp(tCap/tU))/(Exp(tCap/tU)-1.0) - (Power(rho*Exp(tCap*(1.0/tU-1.0/tT)),n)-rho*Exp(tCap*(1.0/tU-1.0/tT)))/(rho*Exp(tCap*(1.0/tU-1.0/tT))-1.0) );
	}
//	f *= ( TMath::Exp(-tCap/tU) - TMath::Exp(-tCap/tT) );
	return f;
}
Double_t BFitNamespace::Z1 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	using namespace TMath;
// externs: constant over all computations
	extern Double_t iota, tCap, tBac, tCyc, t1;
// statics: recomputed many times (keep memory allocated)
	static Int_t n, N;
	static Double_t tT1, tU1, z10, z1, Z1, f, amplitude;
// Initialize function value
	f = 0.0;
// Recompute variables
	tT1 = 1.0 / ( 1.0/t1 + a[gammaT1]/1000.0 ); // net variable lifetime (1/e) in ms
	tU1 = 1.0 / ( 1.0/t1 + a[gammaU1]/1000.0 ); // net variable lifetime (1/e) in ms
	n = Ceil((t[0]-tBac)/tCap);
	N = Ceil((tCyc-tBac)/tCap);
	amplitude = a[p] * a[r1] * tCap * (a[gammaT1]+iota)/((a[gammaT1]-a[gammaU1])+iota);
// Initial value (t=0)
	z10 = amplitude * ( (SigmaZ(a[rho],tT1,tU1,N)+SigmaT(a[rho],tT1,N))*Exp(-(tCyc-tBac-(N-1)*tCap)/tU1) - SigmaT(a[rho],tT1,N)*Exp(-(tCyc-tBac-(N-1)*tCap)/tT1) ) / (1 - Exp(-tCyc/tU1) );
// Background solution
	z1 = z10 * Exp(-t[0]/tU1);
// Background period
	if (0 <= t[0] && t[0] < tBac) {
		f = z1;
	}
	// Trapping period
	if (tBac <= t[0] && t[0] <= tCyc) {
		// Untrapped populations during trapping
		Z1 = amplitude * ( (SigmaZ(a[rho],tT1,tU1,n)+SigmaT(a[rho],tT1,n))*Exp(-(t[0]-tBac-(n-1)*tCap)/tU1) - SigmaT(a[rho],tT1,n)*Exp(-(t[0]-tBac-(n-1)*tCap)/tT1) );
		f = z1 + Z1;
	}
	return f;
}
Double_t BFitNamespace::Z2 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	using namespace TMath;
// externs: constant over all computations
	extern Double_t iota, tCap, tBac, tCyc, t2;
// statics: recomputed many times (keep memory allocated)
	static Int_t n, N;
	static Double_t tT2, tU2, z20, z2, Z2, f, amplitude;
// Initialize function value
	f = 0.0;
// Recompute variables
	tT2 = 1.0 / ( 1.0/t2 + a[gammaT2]/1000.0 ); // net variable lifetime (1/e) in ms
	tU2 = 1.0 / ( 1.0/t2 + a[gammaU2]/1000.0 ); // net variable lifetime (1/e) in ms
	if (t[0]==tBac)	n = 1;
	else			n = Ceil((t[0]-tBac)/tCap);
	N = Ceil((tCyc-tBac)/tCap);
	amplitude = a[p] * a[r2] * tCap * (a[gammaT2]+iota)/((a[gammaT2]-a[gammaU2])+iota);
// Initial value (t=0)
	z20 = amplitude * ( (SigmaZ(a[rho],tT2,tU2,N)+SigmaT(a[rho],tT2,N))*Exp(-(tCyc-tBac-(N-1)*tCap)/tU2) - SigmaT(a[rho],tT2,N) * Exp(-(tCyc-tBac-(N-1)*tCap)/tT2) ) / (1 - Exp(-tCyc/tU2) );
//	z20 = amplitude * ( SigmaZ(a[rho],tT2,tU2,N)*Exp(-(tCyc-tBac-(N-1)*tCap)/tU2) + SigmaT(a[rho],tT2,N) * (Exp(-(tCyc-tBac-(N-1)*tCap)/tU2) - Exp(-(tCyc-tBac-(N-1)*tCap)/tT2)) ) / (1 - Exp(-tCyc/tU2) );
// Background solution
	z2 = z20 * Exp(-t[0]/tU2);
// Background period
	if (0 <= t[0] && t[0] < tBac) {
		f = z2;
	}
	// Trapping period
	if (tBac <= t[0] && t[0] <= tCyc) {
		// Untrapped populations during trapping
		Z2 = amplitude * ( (SigmaZ(a[rho],tT2,tU2,n)+SigmaT(a[rho],tT2,n))*Exp(-(t[0]-tBac-(n-1)*tCap)/tU2) - SigmaT(a[rho],tT2,n) * Exp(-(t[0]-tBac-(n-1)*tCap)/tT2) );
//		Z2 = amplitude * ( SigmaZ(a[rho],tT2,tU2,n)*Exp(-(t[0]-tBac-(n-1)*tCap)/tU2) + SigmaT(a[rho],tT2,n) * (Exp(-(t[0]-tBac-(n-1)*tCap)/tU2) - Exp(-(t[0]-tBac-(n-1)*tCap)/tT2)) );
		f = z2 + Z2;
	}
	return f;
}
Double_t BFitNamespace::Z3 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	using namespace TMath;
// externs: constant over all computations
	extern Double_t iota, tCap, tBac, tCyc, t3;
// statics: recomputed many times (keep memory allocated)
	static Int_t n, N;
	static Double_t tT3, tU3, z30, z3, Z3, f, amplitude;
// Initialize function value
	f = 0.0;
// Recompute variables
	tT3 = 1.0 / ( 1.0/t3 + a[gammaT3]/1000.0 ); // net variable lifetime (1/e) in ms
	tU3 = 1.0 / ( 1.0/t3 + a[gammaU3]/1000.0 ); // net variable lifetime (1/e) in ms
	n = Ceil((t[0]-tBac)/tCap);
	N = Ceil((tCyc-tBac)/tCap);
	amplitude = a[p] * a[r3] * tCap * (a[gammaT3]+iota)/((a[gammaT3]-a[gammaU3])+iota);
// Initial value (t=0)
	z30 = amplitude * ( (SigmaZ(a[rho],tT3,tU3,N)+SigmaT(a[rho],tT3,N))*Exp(-(tCyc-tBac-(N-1)*tCap)/tU3) - SigmaT(a[rho],tT3,N) * Exp(-(tCyc-tBac-(N-1)*tCap)/tT3) ) / (1 - Exp(-tCyc/tU3) );
// Background solution
	z3 = z30 * Exp(-t[0]/tU3);
// Background period
	if (0 <= t[0] && t[0] < tBac) {
		f = z3;
	}
	// Trapping period
	if (tBac <= t[0] && t[0] <= tCyc) {
		// Untrapped populations during trapping
		Z3 = amplitude * ( (SigmaZ(a[rho],tT3,tU3,n)+SigmaT(a[rho],tT3,n))*Exp(-(t[0]-tBac-(n-1)*tCap)/tU3) - SigmaT(a[rho],tT3,n) * Exp(-(t[0]-tBac-(n-1)*tCap)/tT3) );
		f = z3 + Z3;
	}
	return f;
}
Double_t BFitNamespace::yZ1 (Double_t *t, Double_t *a) {
	extern Double_t t1;
	return a[nCyc]*a[dt]*a[epsZ]*BFitNamespace::Z1(t,a)/t1;
}
Double_t BFitNamespace::yZ2 (Double_t *t, Double_t *a) {
	extern Double_t t2;
	return a[nCyc]*a[dt]*a[epsZ]*BFitNamespace::Z2(t,a)/t2;
}
Double_t BFitNamespace::yZ3 (Double_t *t, Double_t *a) {
	extern Double_t t3;
	return a[nCyc]*a[dt]*a[epsZ]*BFitNamespace::Z3(t,a)/t3;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// X populations
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Double_t BFitNamespace::X2 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	using namespace TMath;
// externs: constant over all computations
	extern Double_t iota, tCap, tBac, tCyc, t1, t2;
// statics: recomputed many times (keep memory allocated)
	static Int_t n, N;
	static Double_t tT1, tU2, x20, x2, X2, f, amplitude, boundary;
// Initialize function value
	f = 0.0;
// Recompute variables
	tT1 = 1.0 / ( 1.0/t1 + a[gammaT1]/1000.0 ); // net variable lifetime (1/e) in ms
	tU2 = 1.0 / ( 1.0/t2 + a[gammaU2]/1000.0 ); // net variable lifetime (1/e) in ms
	n = Ceil((t[0]-tBac)/tCap);
	N = Ceil((tCyc-tBac)/tCap);
	amplitude = a[p] * a[r1] * tCap * (1/t1) * (tT1*tU2/(tU2-tT1));
// Initial value (t=0)
	x20 = amplitude * ( (SigmaZ(a[rho],tT1,tU2,N)+SigmaT(a[rho],tT1,N))*Exp(-(tCyc-tBac-(N-1)*tCap)/tU2) - SigmaT(a[rho],tT1,N) * Exp(-(tCyc-tBac-(N-1)*tCap)/tT1) ) / (1 - Exp(-tCyc/tU2) );
// Background solution
	x2 = x20 * Exp(-t[0]/tU2);
// Background period
	if (0 <= t[0] && t[0] < tBac) {
		f = x2;
	}
	// Trapping period
	if (tBac <= t[0] && t[0] <= tCyc) {
		// Untrapped populations during trapping
		X2 = amplitude * ( (SigmaZ(a[rho],tT1,tU2,n)+SigmaT(a[rho],tT1,n))*Exp(-(t[0]-tBac-(n-1)*tCap)/tU2) - SigmaT(a[rho],tT1,n) * Exp(-(t[0]-tBac-(n-1)*tCap)/tT1) );
		f = x2 + X2;
	}
	return f;
}
Double_t BFitNamespace::X3 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	using namespace TMath;
// externs: constant over all computations
	extern Double_t iota, tCap, tBac, tCyc, t2, t3;
// statics: recomputed many times (keep memory allocated)
	static Int_t n, N;
	static Double_t tT2, tU3, x30, x3, X3, f, amplitude, boundary;
// Initialize function value
	f = 0.0;
// Recompute variables
	tT2 = 1.0 / ( 1.0/t2 + a[gammaT2]/1000.0 ); // net variable lifetime (1/e) in ms
	tU3 = 1.0 / ( 1.0/t3 + a[gammaU3]/1000.0 ); // net variable lifetime (1/e) in ms
	n = Ceil((t[0]-tBac)/tCap);
	N = Ceil((tCyc-tBac)/tCap);
	amplitude = a[p] * a[r2] * tCap * (1/t2) * (tT2*tU3/(tU3-tT2));
// Initial value (t=0)
	x30 = amplitude * ( (SigmaZ(a[rho],tT2,tU3,N)+SigmaT(a[rho],tT2,N))*Exp(-(tCyc-tBac-(N-1)*tCap)/tU3) - SigmaT(a[rho],tT2,N) * Exp(-(tCyc-tBac-(N-1)*tCap)/tT2) ) / (1 - Exp(-tCyc/tU3) );
// Background solution
	x3 = x30 * Exp(-t[0]/tU3);
// Background period
	if (0 <= t[0] && t[0] < tBac) {
		f = x3;
	}
	// Trapping period
	if (tBac <= t[0] && t[0] <= tCyc) {
		// Untrapped populations during trapping
		X3 = amplitude * ( (SigmaZ(a[rho],tT2,tU3,n)+SigmaT(a[rho],tT2,n))*Exp(-(t[0]-tBac-(n-1)*tCap)/tU3) - SigmaT(a[rho],tT2,n) * Exp(-(t[0]-tBac-(n-1)*tCap)/tT2) );
		f = x3 + X3;
	}
	return f;
}
Double_t BFitNamespace::yX2 (Double_t *t, Double_t *a) {
	extern Double_t t2;
	return a[nCyc]*a[dt]*a[epsX]*BFitNamespace::X2(t,a)/t2;
}
Double_t BFitNamespace::yX3 (Double_t *t, Double_t *a) {
	extern Double_t t3;
	return a[nCyc]*a[dt]*a[epsX]*BFitNamespace::X3(t,a)/t3;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Y populations
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Double_t BFitNamespace::Y2InitialValue (Double_t *t, Double_t *a, Double_t t0, Double_t y0) {
	using namespace BFitNamespace;
	using namespace TMath;
	static Double_t tT1, tU1, tU2, cT1, cU1, cU2, expT1, expU1, expU2, tk, A, B, f;
	extern Double_t iota, tCap, tBac, t1, t2;
	tT1 = 1.0 / ( 1.0/t1 + a[gammaT1]/1000.0 ); // net variable lifetime (1/e) in ms
	tU1 = 1.0 / ( 1.0/t1 + a[gammaU1]/1000.0 ); // net variable lifetime (1/e) in ms
	tU2 = 1.0 / ( 1.0/t2 + a[gammaU2]/1000.0 ); // net variable lifetime (1/e) in ms
	f = 0.0;
	A = 0.0;
	B = 0.0;
	cT1 = tT1*(tU2-tU1);
	cU1	= tU1*(tU2-tT1);
	cU2	= tU2*(tU1-tT1);
	tk = t[0]-t0;
	A = a[p] * (a[gammaT1]+iota)/(a[gammaT1]-a[gammaU1]+iota)/tU1/(tU2-tT1) * SigmaT(a[rho],tT1,1) * (cT1*Exp(-tk/tT1) - cU1*Exp(-tk/tU1) + cU2*Exp(-tk/tU2));
	B = ( (1-a[p])*SigmaT(a[rho],tT1,1) + a[p]*(1-a[rho])*SigmaW(a[rho],tT1,tU1,1) + a[p]*(a[gammaT1]+iota)/(a[gammaT1]-a[gammaU1]+iota)*SigmaZ(a[rho],tT1,tU1,1) ) * ( Exp(-tk/tU2) - Exp(-tk/tU1) );
	f = a[r1] * (tCap/t1) * tU1*tU2/(tU2-tU1) * (A + B);
	return y0*Exp(-(t[0]-t0)/tU2) + f;
}

Double_t BFitNamespace::Y2Trapping (Double_t *t, Double_t *a, Int_t n) {
	using namespace BFitNamespace;
	using namespace TMath;
	static Double_t tk, f;
	static Int_t k;
	extern Double_t tCap, tBac;
	f = 0.0;
	for (k=1; k<=n; k++) {
		tk = tBac+(k-1)*tCap;
		f += Y2InitialValue(t, a, tk, 0.0);
	}
	return f;
}

Double_t BFitNamespace::Y2 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	using namespace TMath;
	static Int_t n, N;
	static Double_t tT1, tU1, tU2, tn, tN, u10, y20, y2, Y2, f;
	extern Double_t tCap, tBac, tCyc, t1, t2, t3;
//	bookGlobals();
	tT1 = 1.0 / ( 1.0/t1 + a[gammaT1]/1000.0 ); // net variable lifetime (1/e) in ms
	tU1 = 1.0 / ( 1.0/t1 + a[gammaU1]/1000.0 ); // net variable lifetime (1/e) in ms
//	tT2 = 1.0 / ( 1.0/t2 + a[gammaT2]/1000.0 ); // net variable lifetime (1/e) in ms
	tU2 = 1.0 / ( 1.0/t2 + a[gammaU2]/1000.0 ); // net variable lifetime (1/e) in ms
	f = 0.0; //catch bad values of t[0]
	n = Ceil((t[0]-tBac)/tCap);
	N = Ceil((tCyc-tBac)/tCap);
	Double_t tCycArg[1] = {tCyc};
	u10 = V1(tCycArg,a) + W1(tCycArg,a) + Z1(tCycArg,a) / (1-Exp(-tCyc/tU1));
	y20 = Y2Trapping(tCycArg,a,N) / (1-Exp(-tCyc/tU2));
// Background solution
	y2 = y20 * Exp(-t[0]/tU2) + u10 * tU1/t1 * tU2/(tU2-tU1) * ( Exp(-t[0]/tU2) - Exp(-t[0]/tU1 ) );
// Background period
	if (0 <= t[0] && t[0] < tBac) {
		f = y2;
	}
// Trapping period
	if (tBac <= t[0] && t[0] <= tCyc) {
		f = y2 + Y2Trapping(t,a,n);
	}
	return f;
}

Double_t BFitNamespace::Y3InitialValue (Double_t *t, Double_t *a, Double_t t0, Double_t y0) {
	using namespace BFitNamespace;
	using namespace TMath;
	static Double_t tx, tT1, tT2, tU1, tU2, tU3, ThetaU, ThetaY;
	static Double_t expT1, expT2, expU1, expU2, expU3, ST1, SW11, SZ11, ST2, SW22, SZ12, SZ22;
	static Double_t cZT2, cZU2, cZU3, cXT1, cXU2, cXU3, cYU1, cYU2, cYU3;
	static Double_t ampV, ampW, ampZ, ampX, ampY_ST1, ampY_SW11, ampY_SZ11;
	static Double_t V, W, Z, X, Y;
	extern Double_t iota, tCap, tBac, t1, t2, t3;
	tT1 = 1.0 / ( 1.0/t1 + a[gammaT1]/1000.0 ); // net variable lifetime (1/e) in ms
	tT2 = 1.0 / ( 1.0/t2 + a[gammaT2]/1000.0 ); // net variable lifetime (1/e) in ms
	tU1 = 1.0 / ( 1.0/t1 + a[gammaU1]/1000.0 ); // net variable lifetime (1/e) in ms
	tU2 = 1.0 / ( 1.0/t2 + a[gammaU2]/1000.0 ); // net variable lifetime (1/e) in ms
	tU3 = 1.0 / ( 1.0/t3 + a[gammaU3]/1000.0 ); // net variable lifetime (1/e) in ms
	ThetaU	= (tU3-tU2)*(tU3-tU1)*(tU2-tU1);
	ThetaY	= (tU3-tT1)*(tU2-tT1)*(tU1-tT1);
	ampV		= a[r2]*(tCap/t2)*tU2*tU3/(tU3-tU2)*(1-a[p]);
	ampW		= a[r2]*(tCap/t2)*tU2*tU3/(tU3-tU2)*a[p]*(1-a[rho]);
	ampZ		= a[r2]*(tCap/t2)*tU2*tU3/(tU3-tU2)*a[p]*(a[gammaT2]+iota)/(a[gammaT2]-a[gammaU2]+iota); // after a[p], ampZ and ampX are complementary parts of T1 decay (1/t1 and a[gammaT1] : radioactive and non-radioactive)
	ampX		= a[r1]*(tCap/t2)*tU2*tU3/(tU3-tU2)*a[p]*tT1*tU2/(tU2-tT1)/t1; // a[gammaT2]/(a[gammaT2]-a[gammaU2]) is a simplification of tT2*tU2/(tU2-tT2)/(1/a[gammaT2])
	ampY_ST1	= a[r1]*(tCap/t2)*tU1*tU2*tU3/ThetaU/t1*(a[gammaT1]+iota)/(a[gammaT1]-a[gammaU1]+iota)*(tU1-tT1)/tU1/(a[gammaT1]/1000.0)/ThetaY;
	ampY_SW11	= a[r1]*(tCap/t2)*tU1*tU2*tU3/ThetaU/t1*a[p]*(1-a[rho]);
	ampY_SZ11	= a[r1]*(tCap/t2)*tU1*tU2*tU3/ThetaU/t1*a[p]*(a[gammaT1]+iota)/(a[gammaT1]-a[gammaU1]+iota);
	cZT2	= tT2*(tU3-tU2);
	cZU2	= tU2*(tU3-tT2);
	cZU3	= tU3*(tU2-tT2);
	cXT1	= tT1*(tU3-tU2);
	cXU2	= tU2*(tU3-tT1);
	cXU3	= tU3*(tU2-tT1);
	cYU1	= tU1*(tU3-tU2);
	cYU2	= tU2*(tU3-tU1);
	cYU3	= tU3*(tU2-tU1);
	tx		= t[0]-t0;
	expT1	= Exp(-tx/tT1);
	expT2	= Exp(-tx/tT2);
	expU1	= Exp(-tx/tU1);
	expU2	= Exp(-tx/tU2);
	expU3	= Exp(-tx/tU3);
//	Int_t k	= 1;
//	ST1		= SigmaT(a[rho],tT1,k);
//	ST2		= SigmaT(a[rho],tT2,k);
//	SW11	= SigmaW(a[rho],tT1,tU1,k);
//	SW22	= SigmaW(a[rho],tT2,tU2,k);
//	SZ11	= SigmaZ(a[rho],tT1,tU1,k);
//	SZ12	= SigmaZ(a[rho],tT1,tU2,k);
//	SZ22	= SigmaZ(a[rho],tT2,tU2,k);
//	V = ampV * ST2  * (expU3 - expU2); // feeding from V2
//	W = ampW * SW22 * (expU3 - expU2); // feeding from W2
//	Z = ampZ * ( (ST2/cZU2) * ( cZT2 * expT2 - cZU2 * expU2 + cZU3 * expU3 ) + SZ22*(expU3-expU2) ); // feeding from Z2
//	X = ampX * ( (ST1/cXU2) * ( cXT1 * expT1 - cXU2 * expU2 + cXU3 * expU3 ) + SZ12*(expU3-expU2) ); // feeding from X2
//	Y = ampY_ST1 * ST1 * 0.001 * ( // 0.001 for the a[gammaT1] from 1/s to 1/ms
//			- expT1 * tT1*tT1*(tU3-tU2)*(tU3-tU1)*(tU2-tU1)*a[p]*a[gammaT1]
//			+ expU1 * tU1*tU1*(tU3-tU2)*(tU3-tT1)*(tU2-tT1)*(a[gammaT1] - (1-a[p])*a[gammaU1])
//			- expU2 * tU2*(tU3-tU1)*(tU3-tT1)*((a[gammaT1])*(tU1*tU2-tT1*(a[p]*tU2+(1-a[p])*tU1))-(a[gammaU1])*(1-a[p])*tU1*(tU2-tT1))
//			+ expU3 * tU3*(tU2-tU1)*(tU2-tT1)*((a[gammaT1])*(tU1*tU3-tT1*(a[p]*tU3+(1-a[p])*tU1))-(a[gammaU1])*(1-a[p])*tU1*(tU3-tT1)) )
//		+ ampY_SW11 * SW11 * ( cYU1*expU1 - cYU2*expU2 + cYU3*expU3 );
//		+ ampY_SZ11 * SZ11 * ( cYU1*expU1 - cYU2*expU2 + cYU3*expU3 ); // feeding from Y2
	Int_t k	= 1;
	ST1		= SigmaT(a[rho],tT1,k);
	ST2		= SigmaT(a[rho],tT2,k);
	V = ampV * ST2  * (expU3 - expU2); // feeding from V2
	W = ampW * SW22 * (expU3 - expU2); // feeding from W2
	Z = ampZ * ( (ST2/cZU2) * ( cZT2 * expT2 - cZU2 * expU2 + cZU3 * expU3 ) );// + SZ22*(expU3-expU2) ); // feeding from Z2
	X = ampX * ( (ST1/cXU2) * ( cXT1 * expT1 - cXU2 * expU2 + cXU3 * expU3 ) );// + SZ12*(expU3-expU2) ); // feeding from X2
	Y = ampY_ST1 * ST1 * 0.001 * ( // 0.001 for the a[gammaT1] from 1/s to 1/ms
			- expT1 * tT1*tT1*(tU3-tU2)*(tU3-tU1)*(tU2-tU1)*a[p]*a[gammaT1]
			+ expU1 * tU1*tU1*(tU3-tU2)*(tU3-tT1)*(tU2-tT1)*(a[gammaT1] - (1-a[p])*a[gammaU1])
			- expU2 * tU2*(tU3-tU1)*(tU3-tT1)*((a[gammaT1])*(tU1*tU2-tT1*(a[p]*tU2+(1-a[p])*tU1))-(a[gammaU1])*(1-a[p])*tU1*(tU2-tT1))
			+ expU3 * tU3*(tU2-tU1)*(tU2-tT1)*((a[gammaT1])*(tU1*tU3-tT1*(a[p]*tU3+(1-a[p])*tU1))-(a[gammaU1])*(1-a[p])*tU1*(tU3-tT1)) );//
//		+ ampY_SW11 * SW11 * ( cYU1*expU1 - cYU2*expU2 + cYU3*expU3 );
//		+ ampY_SZ11 * SZ11 * ( cYU1*expU1 - cYU2*expU2 + cYU3*expU3 ); // feeding from Y2
	return V + W + X + Y + Z;
}

Double_t BFitNamespace::Y3Trapping (Double_t *t, Double_t *a, Int_t n) {
	using namespace BFitNamespace;
	using namespace TMath;
	static Double_t tk, expT1, expT2, expU1, expU2, expU3, ThetaU, ST1, SW11, SZ11, ST2, SW22, SZ22, bU1, bU2, bU3, cT2, cU2, cU3, dT1, dU1, dU2, dU3, A, B, C, D, f;
	static Double_t tT1, tT2, tU1, tU2, tU3;
	static Int_t k;
	extern Double_t iota, tCap, tBac, tCyc, t1, t2, t3;
	if (t[0] < tBac || t[0] > tCyc) return 0.0;
	else {
		f	= 0.0;
		tT1 = 1.0 / ( 1.0/t1 + a[gammaT1]/1000.0 ); // net variable lifetime (1/e) in ms
		tT2 = 1.0 / ( 1.0/t2 + a[gammaT2]/1000.0 ); // net variable lifetime (1/e) in ms
		tU1 = 1.0 / ( 1.0/t1 + a[gammaU1]/1000.0 ); // net variable lifetime (1/e) in ms
		tU2 = 1.0 / ( 1.0/t2 + a[gammaU2]/1000.0 ); // net variable lifetime (1/e) in ms
		tU3 = 1.0 / ( 1.0/t3 + a[gammaU3]/1000.0 ); // net variable lifetime (1/e) in ms
		for (k=1; k<=n; k++) {
			tk		= tBac+(k-1)*tCap;
			f += Y3InitialValue(t, a, tk, 0.0);
		}
//		A *= a[p] * (a[gammaT2]/1000.0+iota)/(tU2-tT2+iota) * tT2/(tU3-tT2);
//		f = a[r2]*(tCap/t2)*tU2*tU3/(tU3-tU2)*(A+B) + a[r1]*(tCap/t1)*tU1*tU2*tU3/t2*(C+D);
//		printf("t=%f,myY3=%f\n",t[0],f);
		return f;
	}
}
Double_t BFitNamespace::Y3 (Double_t *t, Double_t *a) {
	using namespace BFitNamespace;
	using namespace TMath;
	static Int_t n, N;
	static Double_t tT1, tT2, tU1, tU2, tU3, tn, tN, An, AN, Bn, BN, cT1, cU1, cU2, amplitude, u10, u20, y30, y3, Y3, f, A, B, ThetaU;
	static Double_t expT1n, expU1n, expU2n;
	static Double_t expT1N, expU1N, expU2N;
	extern Double_t iota, tCap, tBac, tCyc, t1, t2, t3;
	tT1 = 1.0 / ( 1.0/t1 + a[gammaT1]/1000.0 ); // net variable lifetime (1/e) in ms
	tU1 = 1.0 / ( 1.0/t1 + a[gammaU1]/1000.0 ); // net variable lifetime (1/e) in ms
	tT2 = 1.0 / ( 1.0/t2 + a[gammaT2]/1000.0 ); // net variable lifetime (1/e) in ms
	tU2 = 1.0 / ( 1.0/t2 + a[gammaU2]/1000.0 ); // net variable lifetime (1/e) in ms
//	tT3 = 1.0 / ( 1.0/t3 + a[gammaT3]/1000.0 ); // net variable lifetime (1/e) in ms
	tU3 = 1.0 / ( 1.0/t3 + a[gammaU3]/1000.0 ); // net variable lifetime (1/e) in ms
	ThetaU	= (tU3-tU2)*(tU3-tU1)*(tU2-tU1);
	f = 0.0; //catch bad values of t[0]
	A = 0.0;
	B = 0.0;
	n = Ceil((t[0]-tBac)/tCap);
	N = Ceil((tCyc-tBac)/tCap);
	Double_t tCycArg[1] = {tCyc};
	u10 = ( V1(tCycArg,a) + W1(tCycArg,a) + Z1(tCycArg,a) ) / (1-Exp(-tCyc/tU1));
	u20 = ( V2(tCycArg,a) + W2(tCycArg,a) + Z2(tCycArg,a) + X2(tCycArg,a) + Y2(tCycArg,a) ) / (1-Exp(-tCyc/tU2));
	y30 = ( Y3Trapping(tCycArg,a,N)
		+ u20 * tU2/t2 * tU3/(tU3-tU2) * ( Exp(-tCyc/tU3) - Exp(-tCyc/tU2) )
		+ u10 * tU1/t1 * tU2/t2 * tU3/ThetaU * ( tU1 * (tU3-tU2) * Exp(-tCyc/tU1) - tU2 * (tU3-tU1) * Exp(-tCyc/tU2) + tU3 * (tU2-tU1) * Exp(-tCyc/tU3) ) )
		/ ( 1 - Exp(-tCyc/tU3) );
	y3 = 
		y30 * Exp(-t[0]/tU3)
		+ u20 * tU2/t2 * tU3/(tU3-tU2) * ( Exp(-t[0]/tU3) - Exp(-t[0]/tU2) )
		+ u10 * tU1/t1 * tU2/t2 * tU3/ThetaU * ( tU1 * (tU3-tU2) * Exp(-t[0]/tU1) - tU2 * (tU3-tU1) * Exp(-t[0]/tU2) + tU3 * (tU2-tU1) * Exp(-t[0]/tU3) );
// Background period
	if (0 <= t[0] && t[0] < tBac) {
		f = y3;
	}
// Trapping period
	if (tBac <= t[0] && t[0] <= tCyc) {
		f = y3 + Y3Trapping(t,a,n);
	}
	return f;
}

Double_t BFitNamespace::yY2 (Double_t *t, Double_t *a) {
	extern Double_t t2;
	return a[nCyc]*a[dt]*a[epsY]*BFitNamespace::Y2(t,a)/t2;
}
Double_t BFitNamespace::yY3 (Double_t *t, Double_t *a) {
	extern Double_t t3;
	return a[nCyc]*a[dt]*a[epsY]*BFitNamespace::Y3(t,a)/t3;
}
