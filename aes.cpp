#include "aes.h"

void aes::AddRoundKey(UCHAR* state, const UCHAR* roundKey) {
	for (int i = 0; i < 16; i++)
		state[i] ^= roundKey[i];
}

void aes::SubBytes(UCHAR* state, const UCHAR* sbox) {
	for (int i = 0; i < 16; i++)
		state[i] = sbox[state[i]];
}

void aes::ShiftRowsLeft(UCHAR* state) {
	UCHAR tmp[16];

	tmp[0] = state[0];
	tmp[1] = state[5];
	tmp[2] = state[10];
	tmp[3] = state[15];

	tmp[4] = state[4];
	tmp[5] = state[9];
	tmp[6] = state[14];
	tmp[7] = state[3];

	tmp[8] = state[8];
	tmp[9] = state[13];
	tmp[10] = state[2];
	tmp[11] = state[7];

	tmp[12] = state[12];
	tmp[13] = state[1];
	tmp[14] = state[6];
	tmp[15] = state[11];

	for (int i = 0; i < 16; i++)
		state[i] = tmp[i];
}

void aes::ShiftRowsRight(UCHAR* state) {
	UCHAR tmp[16];

	tmp[0] = state[0];
	tmp[1] = state[13];
	tmp[2] = state[10];
	tmp[3] = state[7];

	tmp[4] = state[4];
	tmp[5] = state[1];
	tmp[6] = state[14];
	tmp[7] = state[11];

	tmp[8] = state[8];
	tmp[9] = state[5];
	tmp[10] = state[2];
	tmp[11] = state[15];

	tmp[12] = state[12];
	tmp[13] = state[9];
	tmp[14] = state[6];
	tmp[15] = state[3];

	for (int i = 0; i < 16; i++)
		state[i] = tmp[i];
}

void aes::MixColumns(UCHAR* state) {
	UCHAR tmp[16];

	tmp[0] = (UCHAR)(mul2[state[0]] ^ mul3[state[1]] ^ state[2] ^ state[3]);
	tmp[1] = (UCHAR)(state[0] ^ mul2[state[1]] ^ mul3[state[2]] ^ state[3]);
	tmp[2] = (UCHAR)(state[0] ^ state[1] ^ mul2[state[2]] ^ mul3[state[3]]);
	tmp[3] = (UCHAR)(mul3[state[0]] ^ state[1] ^ state[2] ^ mul2[state[3]]);

	tmp[4] = (UCHAR)(mul2[state[4]] ^ mul3[state[5]] ^ state[6] ^ state[7]);
	tmp[5] = (UCHAR)(state[4] ^ mul2[state[5]] ^ mul3[state[6]] ^ state[7]);
	tmp[6] = (UCHAR)(state[4] ^ state[5] ^ mul2[state[6]] ^ mul3[state[7]]);
	tmp[7] = (UCHAR)(mul3[state[4]] ^ state[5] ^ state[6] ^ mul2[state[7]]);

	tmp[8] = (UCHAR)(mul2[state[8]] ^ mul3[state[9]] ^ state[10] ^ state[11]);
	tmp[9] = (UCHAR)(state[8] ^ mul2[state[9]] ^ mul3[state[10]] ^ state[11]);
	tmp[10] = (UCHAR)(state[8] ^ state[9] ^ mul2[state[10]] ^ mul3[state[11]]);
	tmp[11] = (UCHAR)(mul3[state[8]] ^ state[9] ^ state[10] ^ mul2[state[11]]);

	tmp[12] = (UCHAR)(mul2[state[12]] ^ mul3[state[13]] ^ state[14] ^ state[15]);
	tmp[13] = (UCHAR)(state[12] ^ mul2[state[13]] ^ mul3[state[14]] ^ state[15]);
	tmp[14] = (UCHAR)(state[12] ^ state[13] ^ mul2[state[14]] ^ mul3[state[15]]);
	tmp[15] = (UCHAR)(mul3[state[12]] ^ state[13] ^ state[14] ^ mul2[state[15]]);

	for (int i = 0; i < 16; i++)
		state[i] = tmp[i];
}

void aes::InverseMixColumns(UCHAR* state) {
	UCHAR tmp[16];

	tmp[0] = (UCHAR)mul14[state[0]] ^ mul11[state[1]] ^ mul13[state[2]] ^ mul9[state[3]];
	tmp[1] = (UCHAR)mul9[state[0]] ^ mul14[state[1]] ^ mul11[state[2]] ^ mul13[state[3]];
	tmp[2] = (UCHAR)mul13[state[0]] ^ mul9[state[1]] ^ mul14[state[2]] ^ mul11[state[3]];
	tmp[3] = (UCHAR)mul11[state[0]] ^ mul13[state[1]] ^ mul9[state[2]] ^ mul14[state[3]];

	tmp[4] = (UCHAR)mul14[state[4]] ^ mul11[state[5]] ^ mul13[state[6]] ^ mul9[state[7]];
	tmp[5] = (UCHAR)mul9[state[4]] ^ mul14[state[5]] ^ mul11[state[6]] ^ mul13[state[7]];
	tmp[6] = (UCHAR)mul13[state[4]] ^ mul9[state[5]] ^ mul14[state[6]] ^ mul11[state[7]];
	tmp[7] = (UCHAR)mul11[state[4]] ^ mul13[state[5]] ^ mul9[state[6]] ^ mul14[state[7]];

	tmp[8] = (UCHAR)mul14[state[8]] ^ mul11[state[9]] ^ mul13[state[10]] ^ mul9[state[11]];
	tmp[9] = (UCHAR)mul9[state[8]] ^ mul14[state[9]] ^ mul11[state[10]] ^ mul13[state[11]];
	tmp[10] = (UCHAR)mul13[state[8]] ^ mul9[state[9]] ^ mul14[state[10]] ^ mul11[state[11]];
	tmp[11] = (UCHAR)mul11[state[8]] ^ mul13[state[9]] ^ mul9[state[10]] ^ mul14[state[11]];

	tmp[12] = (UCHAR)mul14[state[12]] ^ mul11[state[13]] ^ mul13[state[14]] ^ mul9[state[15]];
	tmp[13] = (UCHAR)mul9[state[12]] ^ mul14[state[13]] ^ mul11[state[14]] ^ mul13[state[15]];
	tmp[14] = (UCHAR)mul13[state[12]] ^ mul9[state[13]] ^ mul14[state[14]] ^ mul11[state[15]];
	tmp[15] = (UCHAR)mul11[state[12]] ^ mul13[state[13]] ^ mul9[state[14]] ^ mul14[state[15]];

	for (int i = 0; i < 16; i++)
		state[i] = tmp[i];
}

void aes::ExpandKeyCore(UCHAR* in, const UCHAR i) {
	// Rotate left
	unsigned int* q = (unsigned int*)in;
	*q = (*q >> 8) | ((*q & 0xff) << 24);
	// S-Box 4 bytes
	in[0] = s_box[in[0]];
	in[1] = s_box[in[1]];
	in[2] = s_box[in[2]];
	in[3] = s_box[in[3]];
	// RCon
	in[0] ^= rcon[i];
}

void aes::ExpandKey(const char* inputKey, UCHAR* expandedKey) {
	// First 16 bytes are the original key
	for (int i = 0; i < 16; i++)
		expandedKey[i] = inputKey[i];

	int bytesGenerated = 16;
	int rconIteration = 1;
	UCHAR temp[4];

	while (bytesGenerated < 176) {
		// Read last 4 bytes for the core
		for (int i = 0; i < 4; i++)
			temp[i] = expandedKey[i + bytesGenerated - 4];
		// Perform the core once for each 16 byte key
		if (bytesGenerated % 16 == 0)
			ExpandKeyCore(temp, rconIteration++);
		// XOR temp with [bytesGenerated - 16] and store in expandedKeys
		for (UCHAR a = 0; a < 4; a++) {
			expandedKey[bytesGenerated] = expandedKey[bytesGenerated - 16] ^ temp[a];
			bytesGenerated++;
		}
	}
}

void aes::AES_Encrypt(UCHAR* message, const UCHAR* key) {
	UCHAR state[16];
	for (int i = 0; i < 16; i++) {
		state[i] = message[i];
	}
	AddRoundKey(state, key);
	for (int i = 0; i < 10; i++) {
		SubBytes(state, s_box);
		ShiftRowsLeft(state);
		if (i < 9)
			MixColumns(state);
		AddRoundKey(state, key + (16 * (i + 1)));
	}
	for (int i = 0; i < 16; i++) {
		message[i] = state[i];
	}
}

void aes::AES_Decrypt(UCHAR* message, const UCHAR* key) {
	UCHAR state[16];
	for (int i = 0; i < 16; i++) {
		state[i] = message[i];
	}
	for (int i = 9; i >= 0; i--) {
		AddRoundKey(state, key + (16 * (i + 1)));
		if (i < 9)
			InverseMixColumns(state);
		ShiftRowsRight(state);
		SubBytes(state, inv_s);
	}
	AddRoundKey(state, key);
	for (int i = 0; i < 16; i++) {
		message[i] = state[i];
	}
}

void aes::AddPadding(UCHAR* block, int& size) {
	int padded_size = size;
	if ((size % 16) != 0)
		padded_size = (size / 16 + 1) * 16;
	for (int i = size; i < padded_size; i++)
		block[i] = 0;
	size = padded_size;
}

void aes::Encrypt(UCHAR* block, const UCHAR* key, const int size) {
	for (int i = 0; i < size; i += 16) {
		AES_Encrypt(block + i, key);
	}
}

void aes::Decrypt(UCHAR* block, const UCHAR* key, const int size) {
	for (int i = 0; i < size; i += 16) {
		AES_Decrypt(block + i, key);
	}
}

void aes::RemovePadding(const UCHAR* block, int& size) {
	for (int i = 0; i < 16; i++) {
		if (block[size - i - 1]) {
			size -= i;
			break;
		}
	}
}
