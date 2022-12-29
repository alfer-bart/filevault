#ifndef FILEVAULT_H_INCLUDED
#define FILEVAULT_H_INCLUDED

const int BLOCK_SIZE = 16384;
const int THREAD_COUNT = 12;
const int ASYNC_SIZE = BLOCK_SIZE * THREAD_COUNT;

enum e_Mode { NONE, LOCK, UNLOCK };

struct AsyncData {
	UCHAR* block;
	UCHAR key[176];
	std::ifstream in_file;
	std::ofstream out_file;
	std::future<void> futures[THREAD_COUNT];
};

void EncryptAsync(void (*action)(UCHAR*, const UCHAR*, const int), AsyncData& ad);
void EncryptTrail(AsyncData& ad, int trail);
void DecryptTrail(AsyncData& ad, int trail);
void ClearKey(char* key);

#endif // FILEVAULT_H_INCLUDED
