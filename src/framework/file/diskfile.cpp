#include "../debug.h"
#include "../log.h"

#include "diskfile.h"

DiskFile::DiskFile()
{
	m_fp = NULL;
	m_mode = 0;
	m_canRead = false;
	m_canWrite = false;
}

DiskFile::~DiskFile()
{
	Close();
}

bool DiskFile::Open(const stl::string &filename, int mode)
{
	ASSERT(IsOpen() == false);
	m_filename = filename;

	char fopenMode[3] = { '\0', '\0', '\0' };
	if (mode & FILEMODE_READ)
	{
		fopenMode[0] = 'r';
		m_canRead = true;
	}
	else if (mode & FILEMODE_WRITE)
	{
		fopenMode[0] = 'w';
		m_canWrite = true;
	}
	else if (mode & FILEMODE_APPEND)
	{
		fopenMode[0] = 'a';
		m_canWrite = true;
	}
	if (mode & FILEMODE_BINARY && fopenMode[0] != '\0')
		fopenMode[1] = 'b';

	if (fopenMode[0] == '\0')
	{
		ASSERT(!"Unrecognized mode.");
		return false;
	}
	else
	{
		ASSERT(m_canRead == true || m_canWrite == true);

		m_fp = fopen(filename.c_str(), fopenMode);
		if (m_fp)
		{
			LOG_INFO(LOGCAT_FILEIO, "Opened DiskFile \"%s\", mode = %s\n", filename.c_str(), fopenMode);
			m_mode = mode;
			return true;
		}
		else
		{
			LOG_WARN(LOGCAT_FILEIO, "Failed to open DiskFile \"%s\", mode = %s\n", filename.c_str(), fopenMode);
			return false;
		}
	}
}

void DiskFile::Close()
{
	if (IsOpen())
	{
		LOG_INFO(LOGCAT_FILEIO, "Closed DiskFile \"%s\"\n", m_filename.c_str());
		fclose(m_fp);
	}

	m_fp = NULL;
	m_mode = 0;
	m_canRead = false;
	m_canWrite = false;
	m_filename.clear();
}

int8_t DiskFile::ReadChar()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	int8_t buffer;
	fread(&buffer, sizeof(int8_t), 1, m_fp);
	return buffer;
}

uint8_t DiskFile::ReadUnsignedChar()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	uint8_t buffer;
	fread(&buffer, sizeof(uint8_t), 1, m_fp);
	return buffer;
}

int16_t DiskFile::ReadShort()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	int16_t buffer;
	fread(&buffer, sizeof(int16_t), 1, m_fp);
	return buffer;
}

uint16_t DiskFile::ReadUnsignedShort()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	uint16_t buffer;
	fread(&buffer, sizeof(uint16_t), 1, m_fp);
	return buffer;
}

int32_t DiskFile::ReadInt()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	int32_t buffer;
	fread(&buffer, sizeof(int32_t), 1, m_fp);
	return buffer;
}

uint32_t DiskFile::ReadUnsignedInt()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	uint32_t buffer;
	fread(&buffer, sizeof(uint32_t), 1, m_fp);
	return buffer;
}

int64_t DiskFile::ReadLong()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	int64_t buffer;
	fread(&buffer, sizeof(int64_t), 1, m_fp);
	return buffer;
}

uint64_t DiskFile::ReadUnsignedLong()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	uint64_t buffer;
	fread(&buffer, sizeof(uint64_t), 1, m_fp);
	return buffer;
}

float DiskFile::ReadFloat()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	float buffer;
	fread(&buffer, sizeof(float), 1, m_fp);
	return buffer;
}

double DiskFile::ReadDouble()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	double buffer;
	fread(&buffer, sizeof(double), 1, m_fp);
	return buffer;
}

size_t DiskFile::Read(int8_t *buffer, size_t bytesToRead)
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	ASSERT(buffer != NULL);
	ASSERT(bytesToRead > 0);
	size_t numRead = fread(buffer, bytesToRead, 1, m_fp);
	return (numRead * bytesToRead);
}

size_t DiskFile::ReadString(stl::string &buffer)
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	size_t charactersRead = 0;
	int8_t c;
	do
	{
		c = ReadChar();
		if (c)
		{
			buffer += c;
			++charactersRead;
		}
	} while (c != '\0');

	return charactersRead;
}

size_t DiskFile::ReadFixedString(stl::string &buffer, size_t maxLength)
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	size_t charactersRead = 0;
	int8_t c;
	for (size_t i = 0; i < maxLength; ++i)
	{
		c = ReadChar();
		if (c)
		{
			buffer += c;
			++charactersRead;
		}
		if (c == '\0')
			break;
	}

	return charactersRead;
}

size_t DiskFile::WriteChar(int8_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = fwrite(&data, sizeof(int8_t), 1, m_fp);
	return (numWritten * sizeof(int8_t));
}

size_t DiskFile::WriteUnsignedChar(uint8_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = fwrite(&data, sizeof(uint8_t), 1, m_fp);
	return (numWritten * sizeof(uint8_t));
}

size_t DiskFile::WriteShort(int16_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = fwrite(&data, sizeof(int16_t), 1, m_fp);
	return (numWritten * sizeof(int16_t));
}

size_t DiskFile::WriteUnsignedShort(uint16_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = fwrite(&data, sizeof(uint16_t), 1, m_fp);
	return (numWritten * sizeof(uint16_t));
}

size_t DiskFile::WriteInt(int32_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = fwrite(&data, sizeof(int32_t), 1, m_fp);
	return (numWritten * sizeof(int32_t));
}

size_t DiskFile::WriteUnsignedInt(uint32_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = fwrite(&data, sizeof(uint32_t), 1, m_fp);
	return (numWritten * sizeof(uint32_t));
}

size_t DiskFile::WriteLong(int64_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = fwrite(&data, sizeof(int64_t), 1, m_fp);
	return (numWritten * sizeof(int64_t));
}

size_t DiskFile::WriteUnsignedLong(uint64_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = fwrite(&data, sizeof(uint64_t), 1, m_fp);
	return (numWritten * sizeof(uint64_t));
}

size_t DiskFile::WriteFloat(float data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = fwrite(&data, sizeof(float), 1, m_fp);
	return (numWritten * sizeof(float));
}

size_t DiskFile::WriteDouble(double data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = fwrite(&data, sizeof(double), 1, m_fp);
	return (numWritten * sizeof(double));
}

size_t DiskFile::Write(int8_t *buffer, size_t bytesToWrite)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	ASSERT(buffer != NULL);
	ASSERT(bytesToWrite > 0);
	size_t numWritten = fwrite(buffer, bytesToWrite, 1, m_fp);
	return (numWritten * bytesToWrite);
}

size_t DiskFile::Tell()
{
	ASSERT(IsOpen());
	return (size_t)ftell(m_fp);
}

void DiskFile::Seek(size_t offset, FileSeek from)
{
	ASSERT(IsOpen());

	int origin = SEEK_CUR;
	if (from == FILESEEK_BEGINNING)
		origin = SEEK_SET;
	else if (from == FILESEEK_END)
		origin = SEEK_END;

	fseek(m_fp, offset, origin);
}

bool DiskFile::AtEOF()
{
	ASSERT(IsOpen());
	return feof(m_fp) != 0;
}

size_t DiskFile::GetFileSize()
{
	ASSERT(IsOpen());
	size_t currentPos = Tell();
	Seek(0, FILESEEK_END);
	size_t filesize = Tell();
	Seek(currentPos, FILESEEK_BEGINNING);
	return filesize;
}