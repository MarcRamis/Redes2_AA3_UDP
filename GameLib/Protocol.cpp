#include "Protocol.h"

OutputMemoryStream *Protocol::Send(PTS protocol, std::string str, std::string str2, int id)
{
	OutputMemoryStream *oms = new OutputMemoryStream();
	oms->Write(protocol);
	oms->WriteString(str); 
	oms->WriteString(str2);
	oms->Write(id);
	
	return oms;
}

OutputMemoryStream* Protocol::Send(PTS protocol, int id, int id2)
{
	OutputMemoryStream* oms = new OutputMemoryStream();
	oms->Write(protocol);
	oms->Write(id);
	oms->Write(id2);
	
	return oms;
}

OutputMemoryStream* Protocol::Send(PTS protocol, int id, int id2, int id3)
{
	OutputMemoryStream* oms = new OutputMemoryStream();
	oms->Write(protocol);
	oms->Write(id);
	oms->Write(id2);
	oms->Write(id3);

	return oms;
}

OutputMemoryStream* Protocol::Send(PTS protocol)
{
	OutputMemoryStream* oms = new OutputMemoryStream();
	oms->Write(protocol);

	return oms;
}

OutputMemoryStream* Protocol::Send(PTS protocol, std::string str)
{
	OutputMemoryStream* oms = new OutputMemoryStream();
	oms->Write(protocol);
	oms->WriteString(str);

	return oms;
}

OutputMemoryStream *Protocol::Send(STP protocol, std::string str)
{
	OutputMemoryStream* oms = new OutputMemoryStream();
	oms->Write(protocol);
	oms->WriteString(str);

	return oms;
}
OutputMemoryStream *Protocol::Send(STP protocol, int id)
{
	OutputMemoryStream* oms = new OutputMemoryStream();
	oms->Write(protocol);
	oms->Write(id);

	return oms;
}

OutputMemoryStream* Protocol::Send(STP protocol)
{
	OutputMemoryStream* oms = new OutputMemoryStream();
	oms->Write(protocol);

	return oms;
}

OutputMemoryStream *Protocol::Send(STP protocol, int id, int id2)
{
	OutputMemoryStream* oms = new OutputMemoryStream();
	oms->Write(protocol);
	oms->Write(id);
	oms->Write(id2);

	return oms;
}

OutputMemoryStream *Protocol::Send(STP protocol, std::string str, unsigned short id)
{
	OutputMemoryStream* oms = new OutputMemoryStream();
	oms->Write(protocol);
	oms->Write(id);
	oms->WriteString(str);
	
	return oms;
}
