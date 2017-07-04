#ifndef _FFT_PERFORMANCE_TESTER_H_
#define _FFT_PERFORMANCE_TESTER_H_

#include "CommonTools.h"
#include "./mayer_fft/fft_routine.h"
#include "./slide_window/slide_window.h"
/**
 * 1:如果对fft之后的结果作分析，比如绘制频谱图、计算基频等，那么要在送给fft之前要对float的数组
 * 做一个汉明窗；
 *
 * 2:如果是对fft之后的结果，做数据处理，比如变调，然后在做逆FFT得到时域数据在写入音频文件，那么
 * 要对逆FFT之后的结果做一个汉明窗
 *
 */
class FFTPerformanceTester {
public:
	FFTPerformanceTester();
	virtual ~FFTPerformanceTester();
	void test(const char* audioFilePath, const char* mayerFFTResultPath, const char* dspFFTResultPath);

private:
	int mayerFFTTime;
	int mayerIFFTTime;
	int dspFFTTime;
	int dspIFFTTime;
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
	FILE* mayerIFFTResultPCMFile;
	void mayerFFTInit();
	void mayerFFTProcess(float* buffer, int size);
	void mayerIFFTProcess(float* buffer, int size);
	void mayerFFTDestroy();

	/** 使用DSPFFT所定义的变量与生命周期方法 **/
	ne10_fft_r2c_cfg_float32_t cfg;          // An FFT "configuration structure"
	ne10_float32_t* in;
	ne10_fft_cpx_float32_t* out;
	FILE* dspFFTResultFile;
	FILE* dspIFFTResultPCMFile;
	void dspFFTInit();
	void dspFFTProcess(float* buffer, int size);
	void dspIFFTProcess(float* buffer, int size);
	void dspFFTDestroy();


	void process(float* inBuffer, int actualSize);

    CSlideWindow  *m_slideWindow;
    float * m_inputDataBuf;
	void prcocessDSPIFFTResult(float* buffer, int actualSize);
    float *m_dspMixBuffer;
    float *m_mayerMixBuffer;
	void prcocessMayerIFFTResult(float* buffer, int actualSize);
	void writeToPCMFile(float* outBuffer, int actualSize, FILE* outputFile);
};
#endif	//_FFT_PERFORMANCE_TESTER_H_
