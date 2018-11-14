#pragma once
#include "core/Common/defines.h"

template<typename T>
void copy(T* src, T* dest) {
	for (int i = 0; i < sizeof(T); i++) {
		char byte = *(char*)((char*)src + (i * sizeof(char)));
		*(char*)((char*)dest + (i * sizeof(char))) = byte;
	}
};

template<typename T>
void make_bytes(T* src, char* out_bytes) {
	//char* ByteCode = new char[sizeof(T)];
	//printf("Bytes = {");
	for (int i = 0; i < sizeof(T); i++) {
		char byte = *(char*)((char*)src + (i * sizeof(char)));
		*(out_bytes + i) = byte;
		//printf(" %d ", (int)byte);
	}
	//printf("}\n");
	//return out_bytes;
};

#define to_bytes(type, src, array_name) \
	char array_name[sizeof(type)];\
	make_bytes<type>(src, array_name); \

template<typename T>
void make_from_bytes(const char* bytes, T* objectToCreate) {
	//LogTerminal("bytes given to the function %s", bytes);
	//printf("Bytes 2 = {");
	for (int i = 0; i < sizeof(T); i++) {
		char byte = bytes[i];
		*(char*)((char*)objectToCreate + (i * sizeof(char))) = byte;
		//printf(" %d ", (int)byte);
	}
	//printf("}\n");
};