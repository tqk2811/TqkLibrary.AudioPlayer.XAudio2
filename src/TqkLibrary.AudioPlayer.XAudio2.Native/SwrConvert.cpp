#include "pch.h"
#include "libav.h"
#include "SwrConvert.h"


SwrConvert::SwrConvert() {
}

SwrConvert::~SwrConvert() {
	if (this->_swrContext)
	{
		swr_close(this->_swrContext);
		swr_free(&this->_swrContext);
	}
}

BOOL SwrConvert::Init(
	AVChannelLayout* out_ch_layout, enum AVSampleFormat out_sample_fmt, int out_sample_rate,
	AVChannelLayout* in_ch_layout, enum AVSampleFormat  in_sample_fmt, int in_sample_rate
) {
	if (this->_swrContext)
		return TRUE;

	this->_swrContext = swr_alloc();
	assert(this->_swrContext);

	int err = swr_alloc_set_opts2(
		&this->_swrContext,
		out_ch_layout, out_sample_fmt, out_sample_rate,
		in_ch_layout, in_sample_fmt, in_sample_rate,
		0, nullptr
	);
	if (!err)
		err = swr_init(this->_swrContext);

	if (err)
	{
		swr_free(&this->_swrContext);
	}

	return err == 0;
}
BOOL SwrConvert::Convert(const AVFrame* source, AVFrame* target) {
	if (!this->_swrContext)
		return FALSE;

	int err = CheckErr(swr_convert_frame(this->_swrContext, target, source));
	return err == 0;
}