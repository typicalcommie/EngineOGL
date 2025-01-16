#include "Filesystem.h"
#include <iostream>
#include <fstream>

using namespace std;

//COMMON

uint FindValue(char* input, char value)
{
	uint result{};
	for (; input[result] != value; result++) if (input[result] == 0) return 0;
	
	return result;
}

uint FindValue(char* input, char value, uint lenght)
{
	uint result{};
	for (uint i{}; input[result] != value && i < lenght; result++);

	return result;
}

uint FindValue(char* input, char value, char to_place)
{
	uint result{};
	for (; input[result] != value; result++) if (input[result] == 0) return 0;
	input[result] = to_place;
	return result;
}

uint CountValue(char* input, char value, char stop_sign)
{
	uint result{};
	for (uint i{}; input[i] != stop_sign; i++)
	{
		if (input[i] == value)
		{
			result++;
		}
	}
	return result;
}

//Same stuff for all three. It wraps cout, so i don't have to include iostream in other files o_O.
void Report(char text)
{
	cout << text;
}
void Report(char* text)
{
	cout << text;
}
void Report(const char* text)
{
	cout << text;
}
void Report(double value)
{
	cout << value;
}
void Report(uint value)
{
	cout << value;
}
void Report(int value)
{
	cout << value;
}

//FILE CLASS

char File::Read(String path)
{
	this->path = path;
	return Read();
}
char File::Read(char* path)
{
	this->path = path;
	return Read();
}


char File::Read()
{

	fstream file;
	file.open(path, ios::binary | ios::ate | ios::binary | ios::in);

	if (!file.is_open())
	{
		cout << "File by path - " << path << " cannot be opened.\n";
		return -1;
	}

	lenght = 0;
	lenght = file.tellg();
	if ( !(lenght > 0) )
	{
		cout << "File by path - " << path << " is empty.\n";
		return -2;
	}
	file.seekg(0, ios::beg);

	_deleteArray(data);
	data = new char[lenght += 1] {};

	file.read(data, lenght);
	file.close();
	return 1;
}

char File::Write(char* path)
{
	fstream file;
	this->path = path;
	file.open(path, ios::binary | ios::binary | ios::out);
	if (!file.is_open())
	{
		cout << "File by path - " << path << " cannot be opened.\n";
		return -1;
	}

	file.write(data, lenght);
	file.close();

	return 1;
}

char* File::Get()
{
	return data;
}

char* File::Path()
{
	return path.Link();
}

//uint File::Size()
//{
//	return size;
//}

uint File::Lenght()
{
	return lenght;
}


//STRING CLASS

//Lenght should be with null terminator!
void String::Set(char* input, uint lenght)
{

	this->lenght = lenght;
	if (lenght == 0) return;

	data = new char[lenght + 1] {};
	memcpy(data, input, lenght);

}

void String::Set(char* input)
{
	_deleteArray(data);
	lenght = FindValue(input, 0);

	if (lenght == 0)
		return;

	data = new char[lenght + 1] {}; //Plus one, so it will include null iterator.
	memcpy(data, input, lenght);
}

void String::Set(uint lenght)
{
	_deleteArray(data);
	this->lenght = lenght;
	data = new char[lenght] {};
}

void String::Copy(char* output)
{
	_deleteArray(output);
	output = new char[lenght];
	memcpy(output, data, lenght);
}

void String::Shrink()
{
	lenght = FindValue(data, 0);
	char* old = data;
	data = new char[lenght + 1] {}; //Plus one, so it will include null iterator.
	memcpy(data, old, lenght);
	_deleteArray(old);
}