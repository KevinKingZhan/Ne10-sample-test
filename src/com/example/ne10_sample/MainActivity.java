package com.example.ne10_sample;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {

	static {
		System.loadLibrary("songstudio");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
//		Ne10Tester.test();
		String audioFilePath = "/mnt/sdcard/a_songstudio/vocal.pcm";
		String mayerFFTResultPath = "/mnt/sdcard/a_songstudio/mayer_fft_result.txt";
		String dspFFTResultPath = "/mnt/sdcard/a_songstudio/dsp_fft_result.txt";
		FFTComparator.doFFT(audioFilePath, mayerFFTResultPath, dspFFTResultPath);
	}

}
