#include "ua2bnk.h"

bool convert_uasset(std::string path)
{
	uintmax_t fsz = std::filesystem::file_size(path);
	unsigned char* uadata = new unsigned char[fsz];

	std::fstream input_uasset;
	input_uasset.open(path, std::ios_base::in | std::ios_base::binary);
	input_uasset.seekg(0, std::ios_base::beg);
	input_uasset.read((char*)uadata, fsz);
	input_uasset.close();

	int bnkSize;

	unsigned char* bnkBuffer = convert_memory_uasset(uadata, fsz, bnkSize);

	std::string bnkPath = path.substr(0, path.size() - 7) + ".bnk";
	FILE* outputBnk = NULL;
	outputBnk = fopen(bnkPath.c_str(), "wb");
	if (outputBnk != NULL)
	{
		fwrite(bnkBuffer, bnkSize, 1, outputBnk);
		fclose(outputBnk);
		delete[] bnkBuffer;
		delete[] uadata;
		return true;
	}
	else
	{
		perror("Conversion Failed");
		return false;
	}
}

unsigned char* convert_memory_uasset(unsigned char* uasset, int fileSize, int& bnkSize)
{
	uint32_t val = 0, off = 0;

	while (true)
	{
		if (off < fileSize)
		{
			memcpy((char*)&val, uasset + off, 4);
		}
		else
		{
			std::cerr << "Could not find a valid soundbank.";
			exit(5);
		}

		if (val == 0x44484B42) //search for BKHD
		{
			break;
		}
		else if (val == 0x484B4200) //alternative 0BKH for files where the start of the bnk isn't an increment of 4
		{
			off += 1;
			break;
		}
		off += 4;
	}

	memcpy((char*)&bnkSize, uasset + off - 0x4, 4);
	unsigned char* bnkBuffer = new unsigned char[bnkSize];
	memcpy(bnkBuffer, uasset + off, bnkSize);
	return bnkBuffer;
}

int main(int argc, char* argv[])
{
	bool bFolder = false;
	if (!argv[1])
	{
		std::cerr << "Path not found\n"
			<< "\nCorrect Usage: 'ua2bnk.exe [path_to_uasset]' or 'ua2bnk.exe [path_to_folder]'\n";
		exit(1);
	}
	std::string path = argv[1];
	if (!std::filesystem::exists(path))
	{
		std::cerr << "Path does not exist.\n";
		exit(2);
	}
	if (path.substr(path.size() - 7, 7) != ".uasset")
	{
		bFolder = true;
	}
	if (!bFolder)
	{
		convert_uasset(path);

		exit(0);
	}
	else
	{
		for (auto const& dir_entry : std::filesystem::directory_iterator{ path })
		{
			if (dir_entry.path().string().substr(dir_entry.path().string().size() - 7, 7) != ".uasset") continue;

			convert_uasset(dir_entry.path().string());
		}
	}
	exit(0);
}
