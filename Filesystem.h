#pragma once

typedef unsigned int uint;

#define _deleteArray(x) if(x) {delete[] x; x = 0;}
#define _delete(x) if(x) {delete x; x = 0;}

uint FindValue(char* input, char value);
uint FindValue(char* input, char value, char to_place);
uint CountValue(char* input, char value, char stop_sign);

void Report(char text);
void Report(char* text);
void Report(const char* text);
void Report(double value);
void Report(uint value);
void Report(int value);

//Use to contain unique data, that should remain for some time.\

class String
{
	char* data{};
public:
	int lenght{};
	
	
	String()
	{

	}
	String(char* data)
	{
		Set(data);
	}
	String(char* data, uint size)
	{
		Set(data, size);
	}
	String(const char* data)
	{
		Set((char*)data);
	}

	operator char* ()
	{
		return data;
	}

	//Lenght should be with null terminator included.
	void Set(char* input, uint lenght);

	//Data should be with null terminator at the end.
	void Set(char* input);

	void Set(uint lenght);

	void Copy(char* output);

	void Shrink();

	//Includes null terminator!
	
	inline char* Link()
	{
		return data;
	}

	inline void Delete()
	{
		_deleteArray(data);
	}

	void operator =(char* text)
	{
		Set(text);
	}
};

class File
{
	uint lenght{};
	uint size{};
	String path{};
	char* data{};

	char Read();
public:
	char Read(String path);
	char Read(char* path);

	char Write(char* path);
	char* Get();
	char* Path();
	//uint Size();
	uint Lenght();
};