#pragma once
class QBuffer
{
public:
	QBuffer();
	~QBuffer();


public:
	void* data() const;
	int length() const;

	void append(const void* data, int len);
	void clear();
	void free();

private:
	void* m_data;
	int m_len;
	int m_capacity;
};

