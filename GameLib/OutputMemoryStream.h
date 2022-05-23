#pragma once
#include <memory>
#include <vector>
#include <math.h>
#include <iostream>

class OutputMemoryStream
{
private:
	
	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
	void ReallocBuffer(uint32_t _newLength)
	{
		//Redimensionamos el buffer --> ya no nos cabe todo lo que queremos copiar
		mBuffer = static_cast<char*>(std::realloc(mBuffer, _newLength));
		//TODO: Controlar error en realloc
		mCapacity = _newLength;
	}
public:
	OutputMemoryStream() :mBuffer(0), mHead(0), mCapacity(0)
	{
		//Inicialmente usamos un buffer de 32
		ReallocBuffer(32);
	}

	~OutputMemoryStream()
	{
		//Pedimos memoria con realloc --> Liberamos con free.
		std::free(mBuffer);
	};

	char* GetBufferPtr() const
	{
		return mBuffer;
	}

	uint32_t GetLength() const
	{
		//mHead con tiene la siguiente posici�n donde hay que copiar informaci�n
		//as� que es la longitud hasta donde hay informaci�n que nos interesa enviar.
		return mHead;
	}

	void Write(const void* _inData, size_t _inByteCount)
	{
		//Nos aseguramos de que hay espacio suficiente en mBuffer para copiar estos datos
		uint32_t resultHead = mHead + static_cast<uint32_t>(_inByteCount);
		if (resultHead > mCapacity)
		{
			//Si no hay espacio suficiente, pedimos m�s memoria
			uint32_t size = resultHead;
			if (resultHead < mCapacity * 2)
				size = mCapacity * 2;
			ReallocBuffer(size);
		}
		//Copiar en el buffer a partir de mHead
		std::memcpy(mBuffer + mHead, _inData, _inByteCount);
		//Incrementamos mHead para que el siguiente Write escriba a continuaci�n
		mHead = resultHead;
	}

	template<typename T> void Write(T _data)
	{
		//Tal y c�mo est� hecho, este Write s�lo funciona para tipos b�sicos.
		//As� evitamos que se nos cuele algo que se serializar� mal
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "Este Write solo soporta tipos basicos.");
		Write(&_data, sizeof(_data));
	}

	template<typename T> void Write(const std::vector<T>& _inVector)
	{
		//Para serializar vectores, pasamos el tama�o del vector
		//y a continuaci�n todos sus elementos.
		int elementCount = _inVector.size();
		Write(elementCount);
		for (const T& element : _inVector)
		{
			Write(element);
		}
	}

	//Es necesario hacer esta funci�n espec�fica 
	//para strings para evitar que entre en el Write con template gen�rico
	void WriteString(std::string _inString)
	{
		//Escribo el tama�o del string y su informaci�n
		int s = _inString.size();
		Write(s);
		Write(_inString.c_str(), _inString.size());
	}
};
