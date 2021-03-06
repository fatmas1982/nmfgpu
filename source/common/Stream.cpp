/*
nmfgpu - CUDA accelerated computation of Non-negative Matrix Factorizations (NMF)

Copyright (C) 2015-2016  Sven Koitka (sven.koitka@fh-dortmund.de)

This file is part of nmfgpu.

nmfgpu is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

nmfgpu is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with nmfgpu.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <common/Event.h>
#include <common/Stream.h>

namespace nmfgpu {
	void Stream::destroy() {
		if (m_stream != nullptr) {
			cudaStreamDestroy(m_stream);
			m_stream = nullptr;
		}
	}

	cudaStream_t Stream::release() {
		auto stream = m_stream;
		m_stream = nullptr;
		return stream;
	}

	Stream::Stream() {
		cudaStreamCreate(&m_stream);
	}

	Stream::Stream(unsigned flags) {
		cudaStreamCreateWithFlags(&m_stream, flags);
	}
	
	Stream::Stream(unsigned flags, unsigned priority) {
		cudaStreamCreateWithPriority(&m_stream, flags, priority);
	}

	Stream::~Stream() {
		destroy();
	}

	Stream::Stream(Stream&& other)
		: m_stream(other.release()) { }

	Stream& Stream::operator = (Stream&& other) {
		if (&other != this) {
			destroy();
			m_stream = other.release();
		}

		return *this;
	}

	void Stream::synchronize() {
		cudaStreamSynchronize(m_stream);
	}

	void Stream::waitFor(const Event& evt, unsigned flags /* = 0 */) {
		cudaStreamWaitEvent(m_stream, evt, flags);
	}
}