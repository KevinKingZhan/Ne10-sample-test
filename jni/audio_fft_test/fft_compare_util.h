#ifndef _FFT_COMPARE_UTIL_H_
#define _FFT_COMPARE_UTIL_H_

#include "CommonTools.h"
#include "./mayer_fft/fft_routine.h"

typedef struct tag_FFTResult {
	float re; //real part 实部
	float im; //imaginary part 虚部
} FFTResult;

class FFTCompareUtil {
public:
	FFTCompareUtil();
	virtual ~FFTCompareUtil();
	void test(const char* audioFilePath, const char* mayerFFTResultPath, const char* dspFFTResultPath);

private:
	/** 汉明窗 **/
	float* m_hannwindow;
	int m_corrsize;
	/** 声音的源文件 **/
	FILE* audioFile;
	/** 使用MayerFFT所定义的变量与生命周期方法 **/
	float* mayerFFTTmpResult;
	FftRoutine* fftRoutine;
	float* m_ffttime;
	float* m_fftfreqre;
	float* m_fftfreqim;
	FILE* mayerFFTResultFile;
	void mayerFFTInit();
	void mayerFFTProcess(float* buffer, int size);
	void mayerIFFTProcess(float* buffer, int size);
	void mayerFFTDestroy();

	/** 使用DSPFFT所定义的变量与生命周期方法 **/
	ne10_fft_r2c_cfg_float32_t cfg;          // An FFT "configuration structure"
	ne10_float32_t* in;
	ne10_fft_cpx_float32_t* out;
	FILE* dspFFTResultFile;
	void dspFFTInit();
	void dspFFTProcess(float* buffer, int size);
	void dspIFFTProcess(float* buffer, int size);
	void dspFFTDestroy();
};
#endif	//_FFT_COMPARE_UTIL_H_
