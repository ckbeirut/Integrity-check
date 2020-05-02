#include <iostream>
#include <string>
#include <windows.h>

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif

/*******************************************/
bool DirectoryExists(const std::string& path) {

DWORD present = GetFileAttributesA(path.c_str());

if (present == INVALID_FILE_ATTRIBUTES)
	return false;

if (present & FILE_ATTRIBUTE_DIRECTORY)
	return true;

return false;    
}

/*******************************************/
std::string replace (const std::string& path) {

std::string in = path;
std::replace(in.begin(), in.end(), '\\', '/');
return in;

}

/*******************************************/

void findVideos (std::string& fspath){

while(*(fspath.rbegin()) =='/')
	fspath.pop_back();	

size_t i=1;
WIN32_FIND_DATA FindFileData;

std::string destpath = fspath + std::string("/")+ std::string("*.mp4");
std::cout <<"destpath "<<destpath<<std::endl;
std::cout<<"destpath length "<<destpath.length()<<std::endl;

std::string ffmpegPath = "ffmpeg.exe -v error -f null - -i ";

HANDLE hFind = FindFirstFile(destpath.c_str(), &FindFileData);

if (hFind != INVALID_HANDLE_VALUE)
	{
	do	{
		std::string fullpath = std::string(fspath)+std::string("/")+std::string(FindFileData.cFileName);
		std::cout<<i<<"-"<<"Checking "<<fullpath<<" for errors"<<std::endl;
		std::string command = ffmpegPath +std::string("\"")+fullpath+std::string("\"");
		system (command.c_str());
		i++;
		}
	while(FindNextFile(hFind, &FindFileData));
	}
	FindClose(hFind);
}


/*******************************************/
int main(int argc, char**argv) {
	
const char* path = argv[1];

if (path == nullptr)
	{
	std::cout<<"No path provided"<<std::endl;
	return 0;
	}
else
	if ( DirectoryExists(path) )
		std::cout<<"Provided path is: "<<path<<std::endl;
	else
		{
		std::cout<<"Path doesn't exist"<<std::endl;
		return 0;
		}

/***********************************************/
std::string fspath;
fspath = replace(path);

std::cout<<"fspath "<<fspath<<std::endl;
std::cout<<fspath.length()<<std::endl;
findVideos (fspath);

return 0;
}