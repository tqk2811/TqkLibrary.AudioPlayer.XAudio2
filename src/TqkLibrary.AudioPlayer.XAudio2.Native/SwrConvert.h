#ifndef _H_SwrConvert_H_
#define _H_SwrConvert_H_
class SwrConvert
{
public:
	SwrConvert();
	~SwrConvert();
	BOOL Init(
		AVChannelLayout* out_ch_layout, enum AVSampleFormat out_sample_fmt, int out_sample_rate,
		AVChannelLayout* in_ch_layout, enum AVSampleFormat in_sample_fmt, int  in_sample_rate
	);
	BOOL Convert(const AVFrame* source, AVFrame* target);

private:
	SwrContext* _swrContext{ nullptr };

};


#endif // !_H_SwrConvert_H_
