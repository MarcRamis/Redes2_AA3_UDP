#pragma once
#include <memory>
#include <vector>
#include <iostream>

class InputMemoryStream
{
private:
	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
public:
	InputMemoryStream(char* _inBuffer, uint32_t _inByteCount) : mCapacity(_inByteCount), mHead(0)
	{
		mBuffer = _inBuffer;
	}

	~InputMemoryStream()
	{

	}

	void Read(void* _outData, uint32_t _inByteCount)
	{
		//Después de haber leído, la posición del siguiente dato a leer quedará en resultHead
		uint32_t resultHead = mHead + _inByteCount;
		//Si resultHead supera la capacidad del mBuffer, estamos leyendo más de lo que escribimos
		if (resultHead > mCapacity)
		{
			throw std::exception("InputMemoryStream::No data to read");
		}
		//Copio en _outData el tamaño de _outData contado desde mHead
		std::memcpy(_outData, mBuffer + mHead, _inByteCount);

		//Avanzo mHead para el siguiente Read.
		mHead = resultHead;
	}

	template<typename T> void Read(T* _outData)
	{
		//Tal y cómo está hecho, este Read sólo funciona para tipos básicos.
		//Así evitamos deserializar algo que no se hará bien
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Este Read solo soporta tipos basicos.");
		//En sizeOf se guarda el tamaño de este tipo en bytes
		//Si es un char: 1
		//Si es un int: 4
		int sizeOf = sizeof(*_outData);
		Read(_outData, sizeOf);
	}

	template<typename T> void Read(std::vector<T>* _outVector)
	{
		//Cuando recuperamos un vector, lo primero que nos llega es el número de posiciones.
		int elementCount;
		Read(&elementCount);
		//Recuperamos todas las posiciones y las copiamos en el vector.
		for (size_t i = 0; i < elementCount; i++)
		{
			T element;
			Read(&element);
			_outVector->push_back(element);
		}
	}

	//Es necesario hacer esta función específica para strings 
	//para evitar que entre en el Read con template genérico
	std::string ReadString()
	{
		//Recuperamos la longitud del string
		int length;
		Read(&length);
		//Reservamos memoria para un char* y lo recuperamos con la función genérica.
		char* buffer = new char[length + 1];
		Read(buffer, length);
		buffer[length] = '\0';

		//Construimos el string a partir del buffer
		std::string str = std::string(buffer);
		//Liberamos memoria
		delete[] buffer;
		return str;
	}
};