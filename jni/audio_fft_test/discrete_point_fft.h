#ifndef _DISCRETE_POINT_FFT_H_
#define _DISCRETE_POINT_FFT_H_

#include "CommonTools.h"
#include "./mayer_fft/fft_routine.h"

class DisCretePointFFT {
public:
	DisCretePointFFT();
	virtual ~DisCretePointFFT();
	void test();

private:
	/** 汉明窗 **/
	float* m_hannwindow;
	int m_corrsize;
	void buildData(short* buffer, int size);
	/** 使用MayerFFT所定义的变量与生命周期方法 **/
	FftRoutine* fftRoutine;
	float* m_ffttime;
	float* m_fftfreqre;
	float* m_fftfreqim;
	void mayerFFTInit();
	void mayerFFTProcess(short* buffer, int size);
	void mayerFFTDestroy();

	/** 使用DSPFFT所定义的变量与生命周期方法 **/
	ne10_fft_r2c_cfg_float32_t cfg;          // An FFT "configuration structure"
	void dspFFTInit();
	void dspFFTProcess(short* buffer, int size);
	void dspFFTDestroy();
};
#endif	//_DISCRETE_POINT_FFT_H_
