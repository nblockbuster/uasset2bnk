#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <filesystem>
#include <fstream>

bool convert_uasset(std::string path);
unsigned char* convert_memory_uasset(unsigned char* uasset, int fileSize, int& bnkSize);