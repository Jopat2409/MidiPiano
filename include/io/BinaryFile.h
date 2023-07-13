#ifndef _INTERNAL_BASEFILE_H
#define _INTERNAL_BASEFILE_H

#include <fstream>
#include <cstdint>
#include <string>

#include <intrin.h>

#define MAX_FILE_BUFFER 8192

typedef enum {
	LITTLE_ENDIAN = 0,
	BIG_ENDIAN = 1
} endianness_t;

/* Provides methods for loading binary data from a file */
class BinaryFile {
public:
	BinaryFile(endianness_t type = LITTLE_ENDIAN);
	static bool CheckPath(const char* path);
	bool Open(const char* path);
	bool Open(std::string& path);
	void Close();
	inline static void SetLastError(const char* error) { m_lastError = error; };
	inline static const char* GetLastError() { return m_lastError; };
protected:
	template <typename T>
	inline void LoadType(T& dest) {
		dest = ReverseEndianness(*(T*)(this->inputStream + this->cPtr));
		this->cPtr += sizeof(T);
	}
	template <typename T>
	inline T LoadType() {
		this->cPtr += sizeof(T);
		return ReverseEndianness(*(T*)(this->inputStream + this->cPtr - sizeof(T)));
	}
	unsigned long LoadLEB128();
	void LoadString(char*& dest);
	char* GetCurrentPtr() { return inputStream + cPtr; };
	//void LoadString(const char*& dest);
	//const char* LoadString();
	inline size_t GetReadBytes() { return readBytes; };
private:
	unsigned long cPtr{0};
	char* inputStream{nullptr};
	std::ifstream* inputFile{nullptr};
	size_t readBytes{ 0 };
	size_t initialSize{ 0 };
	bool isOpen{ false };

	endianness_t c_endianness;

	void LoadBytes(int byteOffset = 0);
	void CheckSize(size_t size);

	template <typename T>
	inline T ReverseEndianness(T val) {
		if (c_endianness == LITTLE_ENDIAN)
			return val;
		switch (sizeof(val)) {
		case 1:
			return val;
		case 2:
			return _byteswap_ushort(val);
		case 4:
			return _byteswap_ulong(val);
		case 8:
			return _byteswap_uint64(val);
		}
	}

	static const char* m_lastError;
};



#endif