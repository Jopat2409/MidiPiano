#include "..\include\io\BinaryFile.h"
#include "..\include\base\Logging.h"

const char* BinaryFile::m_lastError = nullptr;

BinaryFile::BinaryFile(endianness_t type) {
	c_endianness = type;
}

bool BinaryFile::CheckPath(const char* path) {
	if (FILE* f = fopen(path, "r")) {
		fclose(f);
		return true;
	}
	SetLastError("[ERROR] Incorrect path!!");
	return false;
}

bool BinaryFile::Open(const char* path) {
	if (!CheckPath(path))
		return false;
	this->isOpen = true;
	this->inputFile = new std::ifstream(path, std::ios_base::in | std::ios_base::binary);
	this->LoadBytes();
	INFO("Finished opening file!")
	return true;
}

bool BinaryFile::Open(std::string& path) {
	return Open(path.c_str());
}

void BinaryFile::Close() {
	if (this->isOpen)
	{
		// delete input char buffer
		delete[] this->inputStream;
		// close input stream
		this->inputFile->close();
		delete this->inputFile;
		this->isOpen = false;
	}
}

void BinaryFile::LoadBytes(int byteOffset) {
	// if initial size has not yet been determined
	if (!this->initialSize)
	{
		// go to end of file, get bytes and then set initial size
		this->inputFile->seekg(0, std::ios::end);
		size_t sizeLeft = this->inputFile->tellg();
		this->inputFile->seekg(0, std::ios::beg);
		this->initialSize = sizeLeft;
		// initialise input stream
		this->inputStream = new char[MAX_FILE_BUFFER]();
	}
	// bytes left in file
	size_t bytesLeft = this->initialSize - this->readBytes;
	// accounting for byte offset meaning slightly less bytes can be read
	size_t maxBufferSize = MAX_FILE_BUFFER - byteOffset;
	// set the length to read to either MAX_FILE_BUFFER - offset or simply to the amount of bytes left
	size_t length = maxBufferSize * (maxBufferSize <= bytesLeft) + bytesLeft * (bytesLeft < maxBufferSize);
	// read amount of bytes and add to length
	this->inputFile->read(&this->inputStream[byteOffset], length);
	this->readBytes += length;
	INFO("Finished Loading bytes from midi file!")
}

void BinaryFile::CheckSize(size_t size) {
	// bytes left in current buffer
	unsigned long bytesLeft = MAX_FILE_BUFFER - this->cPtr;
	// if there are less bytes left than bytes in object
	if (bytesLeft < size)
	{
		// copy the last x bits to the start of the file ONLY IF there are some bytes left
		if(bytesLeft < 0)
			memcpy_s(this->inputStream, bytesLeft, &this->inputStream[this->cPtr], bytesLeft);
		// set cPtr to 0 and load bytes from the byte offset
		this->cPtr = 0;
		this->LoadBytes(bytesLeft);
	}
}

unsigned long BinaryFile::LoadLEB128() {
	unsigned long total = 0;
	int currentShift = 0;
	char currentByte = 0;
	do {
		LoadType<char>(currentByte);
		total |= (currentByte & ~(1 << 7)) << currentShift;
		currentShift += 7;
	} while (currentByte & (1 << 7));
	return total;
}

void BinaryFile::LoadString(char*& dest) {
	unsigned long stringLength = LoadLEB128();
	CheckSize(stringLength);
	std::cout << "Loading string length: " << stringLength << "\n";
	dest = new char[stringLength + 1];
	strcpy(dest, this->inputStream + this->cPtr);
	this->cPtr += stringLength;
	std::cout << "Loaded string " << dest << "\n";
}