#include "QBuffer.h"
#include <cstring>

#define CAPACITY_INCREMENT 1024

QBuffer::QBuffer() : m_data(nullptr), m_len(0), m_capacity(0) {

}
QBuffer::~QBuffer() {
	free();
}

void* QBuffer::data() const {
	return m_data;
}

int QBuffer::length() const {
	return m_len;
}

void QBuffer::append(const void* data, int len) {
	if (m_len + len > m_capacity) {
		m_capacity += m_len + len + CAPACITY_INCREMENT;
		delete[] m_data;
		m_data = new char[m_capacity];
	}

	memcpy((char *)m_data + m_len, data, len);
	m_len += len;
}

void QBuffer::clear() {
	m_len = 0;
}

void QBuffer::free() {
	if (m_data != nullptr) {
		delete[] m_data;
		m_data = nullptr;
		m_len = 0;
		m_capacity = 0;
	}
}
