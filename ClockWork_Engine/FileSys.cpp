#include "FileSys.h"
#include "Application.h"

#include "SDL/include/SDL.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <Shlwapi.h>

#include "PhysFS/include/physfs.h"

#pragma comment (lib, "PhysFS/libx86/physfs.lib")
#pragma comment (lib,"shlwapi.lib")



void FileSys::Init()
{
	char* base_path = SDL_GetBasePath();

	if (PHYSFS_init(nullptr) != 0)
		LOG("PhysFS initted correctly");

	SDL_free(base_path);

	AddPath("."); //Adding ProjectFolder (working directory)
	AddPath("Assets");

	if (PHYSFS_setWriteDir(".") == 0)
		LOG_ERROR("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	CreateLibraryDirectories();

	std::string path = PHYSFS_getWriteDir();

	normalize_scales = true;
}

void FileSys::CleanUp()
{
	PHYSFS_deinit();
}

void FileSys::GetPhysFSVersion(std::string& version_str)
{
	PHYSFS_Version version;
	PHYSFS_getLinkedVersion(&version);
	version_str = std::to_string(version.major) + "." + std::to_string(version.minor) + "." + std::to_string(version.patch);
}

void FileSys::CreateLibraryDirectories()
{
	//CreateDir(LIBRARY_PATH);
	//CreateDir(FOLDERS_PATH);
	//CreateDir(MESHES_PATH);
	//CreateDir(TEXTURES_PATH);
	//CreateDir(MATERIALS_PATH);
	CreateDir("Assets");
	CreateDir("Assets/street/");

	//CreateDir("Materials/");
	//CreateDir(ANIMATIONS_PATH);
	//CreateDir(PARTICLES_PATH);
	//CreateDir(SHADERS_PATH);
	//CreateDir(SCENES_PATH);
}

// Add a new zip file_path or folder
bool FileSys::AddPath(const char* path_or_zip)
{
	bool ret = false;

	if (PHYSFS_mount(path_or_zip, nullptr, 1) == 0)
	{
		LOG_ERROR("File System error while adding a path or zip: %s\n", PHYSFS_getLastError());
	}
	else
		ret = true;

	return ret;
}

// Check if a file_path exists
bool FileSys::Exists(const char* file)
{
	return PHYSFS_exists(file) != 0;
}

bool FileSys::CreateDir(const char* dir)
{
	if (IsDirectory(dir) == false)
	{
		PHYSFS_mkdir(dir);
		return true;
	}
	return false;
}

// Check if a file_path is a directory
bool FileSys::IsDirectory(const char* file)
{
	return PHYSFS_isDirectory(file) != 0;
}

const char* FileSys::GetWriteDir()
{
	//TODO: erase first annoying dot (".")
	return PHYSFS_getWriteDir();
}

void FileSys::DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list)
{
	char** rc = PHYSFS_enumerateFiles(directory);
	char** i;

	for (i = rc; *i != nullptr; i++)
	{
		std::string str = std::string(directory) + std::string("/") + std::string(*i);
		if (IsDirectory(str.c_str()))
			dir_list.push_back(*i);
		else
			file_list.push_back(*i);
	}

	PHYSFS_freeList(rc);
}

void FileSys::DiscoverFilesRecursive(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list)
{
	std::vector<std::string> files;
	std::vector<std::string> directories;

	DiscoverFiles(directory, files, directories);

	for (size_t i = 0; i < files.size(); i++)
	{
		std::string file = directory;
		file.append("/" + files[i]);
		file_list.push_back(file);
	}

	for (size_t i = 0; i < directories.size(); i++)
	{
		std::string dir = directory;
		dir.append("/" + directories[i]);
		dir_list.push_back(dir);
		DiscoverFilesRecursive(dir.c_str(), file_list, dir_list);
	}

}

void FileSys::GetAllFilesWithExtension(const char* directory, const char* extension, std::vector<std::string>& file_list)
{
	std::vector<std::string> files;
	std::vector<std::string> dirs;
	DiscoverFiles(directory, files, dirs);

	for (uint i = 0; i < files.size(); i++)
	{
		std::string ext;
		SplitFilePath(files[i].c_str(), nullptr, nullptr, &ext);

		if (ext == extension)
			file_list.push_back(files[i]);
	}
}

void FileSys::GetRealDir(const char* path, std::string& output)
{
	output = PHYSFS_getBaseDir();

	std::string baseDir = PHYSFS_getBaseDir();
	std::string searchPath = *PHYSFS_getSearchPath();
	std::string realDir = PHYSFS_getRealDir(path);

	output.append(*PHYSFS_getSearchPath()).append("/");
	output.append(PHYSFS_getRealDir(path)).append("/").append(path);
}

std::string FileSys::GetPathRelativeToAssets(const char* originalPath)
{
	std::string file_path = originalPath;

	std::size_t pos = file_path.find("Assets");

	if (pos > file_path.size())
	{
		file_path.clear();
	}
	else
	{
		file_path = file_path.substr(pos);
	}

	return file_path;
}

bool FileSys::HasExtension(const char* path)
{
	std::string ext = "";
	SplitFilePath(path, nullptr, nullptr, &ext);
	return ext != "";
}

bool FileSys::HasExtension(const char* path, std::string extension)
{
	std::string ext = "";
	SplitFilePath(path, nullptr, nullptr, &ext);
	return ext == extension;
}

bool FileSys::HasExtension(const char* path, std::vector<std::string> extensions)
{
	std::string ext = "";
	SplitFilePath(path, nullptr, nullptr, &ext);
	if (ext == "")
		return true;
	for (uint i = 0; i < extensions.size(); i++)
	{
		if (extensions[i] == ext)
			return true;
	}
	return false;
}

std::string FileSys::ProcessPath(const char* path)
{
	std::string final_path = path;

	final_path = NormalizePath(final_path.c_str());
	std::string tmp_path = GetPathRelativeToAssets(final_path.c_str());

	//The file_path is inside a directory
	if (tmp_path.size() > 0)
	{
		return tmp_path.c_str();
	}

	return final_path.c_str();
}

std::string FileSys::NormalizePath(const char* full_path)
{
	std::string newPath(full_path);
	for (int i = 0; i < newPath.size(); ++i)
	{
		if (newPath[i] == '\\')
			newPath[i] = '/';
	}
	return newPath;
}

void FileSys::SplitFilePath(const char* full_path, std::string* path, std::string* file, std::string* extension)
{
	if (full_path != nullptr)
	{
		std::string full(full_path);
		size_t pos_separator = full.find_last_of("\\/");
		size_t pos_dot = full.find_last_of(".");

		if (path != nullptr)
		{
			if (pos_separator < full.length())
				*path = full.substr(0, pos_separator + 1);
			else
				path->clear();
		}

		if (file != nullptr)
		{
			if (pos_separator < full.length())
				*file = full.substr(pos_separator + 1, pos_dot - pos_separator - 1);
			else
				*file = full.substr(0, pos_dot);
		}

		if (extension != nullptr)
		{
			if (pos_dot < full.length())
				*extension = full.substr(pos_dot + 1);
			else
				extension->clear();
		}
	}
}

unsigned int FileSys::Load(const char* path, const char* file, char** buffer)
{
	std::string full_path(path);
	full_path += file;
	return Load(full_path.c_str(), buffer);
}

// Read a whole file_path and put it in a new buffer
uint FileSys::Load(const char* file, char** buffer)
{
	uint ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(file);

	if (fs_file != nullptr)
	{
		PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(fs_file);

		if (size > 0)
		{
			*buffer = new char[size + 1];
			uint readed = (uint)PHYSFS_read(fs_file, *buffer, 1, size);
			if (readed != size)
			{
				LOG_ERROR("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE_ARRAY(buffer);
			}
			else
			{
				ret = readed;
				//Adding end of file_path at the end of the buffer. Loading a shader file_path does not add this for some reason
				(*buffer)[size] = '\0';
			}
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG_ERROR("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG_ERROR("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

bool FileSys::DuplicateFile(const char* file, const char* dstFolder, std::string& relativePath)
{
	std::string fileStr, extensionStr;
	SplitFilePath(file, nullptr, &fileStr, &extensionStr);

	relativePath = relativePath.append(dstFolder).append("/") + fileStr.append(".") + extensionStr;
	std::string finalPath = std::string(*PHYSFS_getSearchPath()).append("/") + relativePath;

	return DuplicateFile(file, finalPath.c_str());

}

bool FileSys::DuplicateFile(const char* srcFile, const char* dstFile)
{
	//TODO: Compare performance to calling Load(srcFile) and then Save(dstFile)
	std::ifstream src;
	src.open(srcFile, std::ios::binary);
	bool srcOpen = src.is_open();
	std::ofstream  dst(dstFile, std::ios::binary);
	bool dstOpen = dst.is_open();

	dst << src.rdbuf();

	src.close();
	dst.close();

	if (srcOpen && dstOpen)
	{
		LOG("[success] File Duplicated Correctly");
		return true;
	}
	else
	{
		LOG("[error] File could not be duplicated");
		return false;
	}
}

void FileSys::Rename(const char* old_name, const char* new_name)
{
	char* fileBuffer;
	uint size = FileSys::Load(old_name, &fileBuffer);

	Save(new_name, fileBuffer, size);
	Delete(old_name);

	RELEASE_ARRAY(fileBuffer);
}

int close_sdl_rwops(SDL_RWops* rw)
{
	RELEASE_ARRAY(rw->hidden.mem.base);
	SDL_FreeRW(rw);
	return 0;
}

uint FileSys::Save(const char* file, const void* buffer, unsigned int size, bool append)
{
	unsigned int ret = 0;

	bool overwrite = PHYSFS_exists(file) != 0;
	PHYSFS_file* fs_file = (append) ? PHYSFS_openAppend(file) : PHYSFS_openWrite(file);

	if (fs_file != nullptr)
	{
		uint written = (uint)PHYSFS_write(fs_file, (const void*)buffer, 1, size);
		if (written != size)
		{
			LOG_ERROR("[error] File System error while writing to file %s: %s", file, PHYSFS_getLastError());
		}
		else
		{
			if (append == true)
			{
				LOG("Added %u data to [%s%s]", size, GetWriteDir(), file);
			}
			else if (overwrite == true)
			{
				LOG("File [%s%s] overwritten with %u bytes", GetWriteDir(), file, size);
			}
			else
			{
				LOG("New file created [%s%s] of %u bytes", GetWriteDir(), file, size);
			}

			ret = written;
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG_ERROR("[error] File System error while closing file %s: %s", file, PHYSFS_getLastError());
	}
	else
		LOG_ERROR("[error] File System error while opening file %s: %s", file, PHYSFS_getLastError());

	return ret;
}

// Save a whole buffer to disk


bool FileSys::Delete(const char* file)
{
	if (PHYSFS_delete(file) != 0)
		return true;
	else {
		LOG_ERROR("File System error while deleting file %s: %s", file, PHYSFS_getLastError());
		return false;
	}
}

uint64 FileSys::GetLastModTime(const char* filename)
{
	return PHYSFS_getLastModTime(filename);
}

std::string FileSys::GetUniqueName(const char* path, const char* name)
{
	//TODO: modify to distinguix files and dirs?
	std::vector<std::string> files, dirs;
	DiscoverFiles(path, files, dirs);

	std::string finalName(name);
	bool unique = false;

	for (uint i = 0; i < 50 && unique == false; ++i)
	{
		unique = true;

		//Build the compare name (name_i)
		if (i > 0)
		{
			finalName = std::string(name).append("_");
			if (i < 10)
				finalName.append("0");
			finalName.append(std::to_string(i));
		}

		//Iterate through all the files to find a matching name
		for (uint f = 0; f < files.size(); ++f)
		{
			if (finalName == files[f])
			{
				unique = false;
				break;
			}
		}
	}
	return finalName;
}

std::string FileSys::GetFileFormat(const char* path)
{
	std::string format = PathFindExtensionA(path);
	std::transform(format.begin(), format.end(), format.begin(), [](unsigned char c) { return std::tolower(c); });
	return format;
}

std::string FileSys::GetFile(const char* path)
{
	std::string file;
	std::string file_path;
	std::string extension;
	SplitFilePath(path, &file_path, &file, &extension);
	return file + "." + extension;
}

std::string FileSys::GetFileName(const char* path)
{
	std::string file;
	std::string file_path;
	SplitFilePath(path, &file_path, &file);
	return file;
}

std::string FileSys::GetFolder(const char* path)
{
	std::string folder;
	SplitFilePath(path, &folder);
	return folder;
}

std::string FileSys::ToLower(const char* path)
{
	std::string string = path;
	std::transform(string.begin(), string.end(), string.begin(), [](unsigned char c) { return std::tolower(c); });
	return string;
}





