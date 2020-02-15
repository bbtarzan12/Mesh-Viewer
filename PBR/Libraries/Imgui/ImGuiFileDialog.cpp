/*
MIT License

Copyright (c) 2019-2020 Stephane Cuillerdier (aka aiekick)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "ImGuiFileDialog.h"
#include "imgui.h"


#include <dirent.h>
#define PATH_SEP '\\'
#ifndef PATH_MAX
#define PATH_MAX 260
#endif

#include "imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"

#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <utility>

static std::string s_fs_root = std::string(1u, PATH_SEP);

// for lets you define your button widget
// if you have like me a speial two color button
#ifndef IMGUI_PATH_BUTTON
#define IMGUI_PATH_BUTTON ImGui::Button
#endif
#ifndef IMGUI_BUTTON
#define IMGUI_BUTTON ImGui::Button
#endif

// locales
#ifndef createDirButtonString
#define createDirButtonString "+"
#endif
#ifndef okButtonString
#define okButtonString "OK"
#endif
#ifndef cancelButtonString
#define cancelButtonString "Cancel"
#endif
#ifndef resetButtonString
#define resetButtonString "R"
#endif
#ifndef drivesButtonString
#define drivesButtonString "Drives"
#endif
#ifndef searchString
#define searchString "Search : "
#endif
#ifndef dirEntryString
#define dirEntryString "[Dir] "
#endif
#ifndef linkEntryString
#define linkEntryString "[Link] "
#endif
#ifndef fileEntryString
#define fileEntryString "[File] "
#endif
#ifndef fileNameString
#define fileNameString "File Name : "
#endif

inline bool replaceString(::std::string& str, const ::std::string& oldStr, const ::std::string& newStr)
{
	bool found = false;
	size_t pos = 0;
	while ((pos = str.find(oldStr, pos)) != ::std::string::npos)
	{
		found = true;
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
	return found;
}

inline std::vector<std::string> splitStringToVector(const ::std::string& text, char delimiter, bool pushEmpty)
{
	std::vector<std::string> arr;
	if (!text.empty())
	{
		std::string::size_type start = 0;
		std::string::size_type end = text.find(delimiter, start);
		while (end != std::string::npos)
		{
			std::string token = text.substr(start, end - start);
			if (!token.empty() || (token.empty() && pushEmpty))
				arr.push_back(token);
			start = end + 1;
			end = text.find(delimiter, start);
		}
		arr.push_back(text.substr(start));
	}
	return arr;
}

inline std::vector<std::string> GetDrivesList()
{
	std::vector<std::string> res;

#ifdef WIN32
	DWORD mydrives = 2048;
	char lpBuffer[2048];

	DWORD countChars = GetLogicalDriveStringsA(mydrives, lpBuffer);

	if (countChars > 0)
	{
		std::string var = std::string(lpBuffer, countChars);
		replaceString(var, "\\", "");
		res = splitStringToVector(var, '\0', false);
	}
#endif

	return res;
}

inline bool IsDirectoryExist(const std::string& name)
{
	bool bExists = false;

	if (!name.empty())
	{
		DIR *pDir = nullptr;
		pDir = opendir(name.c_str());
		if (pDir != nullptr)
		{
			bExists = true;
			(void)closedir(pDir);
		}
	}

	return bExists;    // this is not a directory!
}

inline bool CreateDirectoryIfNotExist(const std::string& name)
{
	bool res = false;

	if (!name.empty())
	{
		if (!IsDirectoryExist(name))
		{
			res = true;

#ifdef WIN32
			CreateDirectoryA(name.c_str(), nullptr);
#elif defined(LINUX) or defined(APPLE)
			char buffer[PATH_MAX] = {};
			snprintf(buffer, PATH_MAX, "mkdir -p %s", name.c_str());
			const int dir_err = std::system(buffer);
			if (dir_err == -1)
			{
				std::cout << "Error creating directory " << name << std::endl;
				res = false;
			}
#endif
		}
	}

	return res;
}

struct PathStruct
{
	std::string path;
	std::string name;
	std::string ext;

	bool isOk;

	PathStruct()
	{
		isOk = false;
	}
};

inline PathStruct ParsePathFileName(const std::string& vPathFileName)
{
	PathStruct res;

	if (!vPathFileName.empty())
	{
		std::string pfn = vPathFileName;
		std::string separator(1u, PATH_SEP);
		replaceString(pfn, "\\", separator);
		replaceString(pfn, "/", separator);

		size_t lastSlash = pfn.find_last_of(separator);
		if (lastSlash != std::string::npos)
		{
			res.name = pfn.substr(lastSlash + 1);
			res.path = pfn.substr(0, lastSlash);
			res.isOk = true;
		}

		size_t lastPoint = pfn.find_last_of('.');
		if (lastPoint != std::string::npos)
		{
			if (!res.isOk)
			{
				res.name = pfn;
				res.isOk = true;
			}
			res.ext = pfn.substr(lastPoint + 1);
			replaceString(res.name, "." + res.ext, "");
		}
	}

	return res;
}

inline void AppendToBuffer(char* vBuffer, size_t vBufferLen, const std::string& vStr)
{
	std::string st = vStr;
	size_t len = vBufferLen - 1u;
	size_t slen = strlen(vBuffer);

	if (!st.empty() && st != "\n")
	{
		replaceString(st, "\n", "");
		replaceString(st, "\r", "");
	}
	vBuffer[slen] = '\0';
	std::string str = std::string(vBuffer);
	if (!str.empty()) str += "\n";
	str += vStr;
	if (len > str.size()) len = str.size();
	strncpy_s(vBuffer, vBufferLen, str.c_str(), len);
	vBuffer[len] = '\0';
}

inline void ResetBuffer(char* vBuffer)
{
	vBuffer[0] = '\0';
}

char ImGuiFileDialog::FileNameBuffer[MAX_FILE_DIALOG_NAME_BUFFER] = "";
char ImGuiFileDialog::DirectoryNameBuffer[MAX_FILE_DIALOG_NAME_BUFFER] = "";
char ImGuiFileDialog::SearchBuffer[MAX_FILE_DIALOG_NAME_BUFFER] = "";
int ImGuiFileDialog::FilterIndex = 0;

ImGuiFileDialog::ImGuiFileDialog()
{
	m_AnyWindowsHovered = false;
	IsOk = false;
	m_ShowDialog = false;
	m_ShowDrives = false;
	m_CreateDirectoryMode = false;
	dlg_optionsPane = nullptr;
	dlg_optionsPaneWidth = 250;
	dlg_filters = "";
}

ImGuiFileDialog::~ImGuiFileDialog() = default;

/* Alphabetical sorting */
/*#ifdef WIN32
static int alphaSort(const void *a, const void *b)
{
	return strcoll(((dirent*)a)->d_name, ((dirent*)b)->d_name);
}
#elif defined(LINUX) or defined(APPLE)*/
static int alphaSort(const struct dirent **a, const struct dirent **b)
{
	return strcoll((*a)->d_name, (*b)->d_name);
}
//#endif

static bool stringComparator(const FileInfoStruct& a, const FileInfoStruct& b)
{
	bool res;
	if (a.type != b.type) res = (a.type < b.type);
	else res = (a.fileName < b.fileName);
	return res;
}

void ImGuiFileDialog::ScanDir(const std::string& vPath)
{
	struct dirent **files = nullptr;
	int             i = 0;
	int             n = 0;
	std::string		path = vPath;

#if defined(LINUX) or defined(APPLE)
	if (path.size() > 0)
	{
		if (path[0] != PATH_SEP)
		{
			//path = PATH_SEP + path;
		}
	}
#endif

	if (m_CurrentPath_Decomposition.empty())
	{
		SetCurrentDir(path);
	}

	if (!m_CurrentPath_Decomposition.empty())
	{
#ifdef WIN32
		if (path == s_fs_root)
		{
			path += PATH_SEP;
		}
#endif
		n = scandir(path.c_str(), &files, nullptr, alphaSort);
		if (n > 0)
		{
			m_FileList.clear();

			for (i = 0; i < n; i++)
			{
				struct dirent *ent = files[i];

				FileInfoStruct infos;

				infos.fileName = ent->d_name;
				if (("." != infos.fileName)/* && (".." != infos.fileName)*/)
				{
					switch (ent->d_type)
					{
					case DT_REG: infos.type = 'f'; break;
					case DT_DIR: infos.type = 'd'; break;
					case DT_LNK: infos.type = 'l'; break;
					}

					if (infos.type == 'f')
					{
						size_t lpt = infos.fileName.find_last_of('.');
						if (lpt != std::string::npos)
						{
							infos.ext = infos.fileName.substr(lpt);
						}
					}

					m_FileList.push_back(infos);
				}
			}

			for (i = 0; i < n; i++)
			{
				free(files[i]);
			}
			free(files);
		}

		std::sort(m_FileList.begin(), m_FileList.end(), stringComparator);
	}
}

void ImGuiFileDialog::SetCurrentDir(const std::string& vPath)
{
	std::string path = vPath;
#ifdef WIN32
	if (s_fs_root == path)
		path += PATH_SEP;
#endif
	DIR  *dir = opendir(path.c_str());
	char  real_path[PATH_MAX];

	if (nullptr == dir)
	{
		path = ".";
		dir = opendir(path.c_str());
	}

	if (nullptr != dir)
	{
#ifdef WIN32
		size_t numchar = GetFullPathNameA(path.c_str(), PATH_MAX - 1, real_path, nullptr);
#elif defined(LINUX) or defined(APPLE)
		char *numchar = realpath(path.c_str(), real_path);
#endif
		if (numchar != 0)
		{
			m_CurrentPath = real_path;
			if (m_CurrentPath[m_CurrentPath.size() - 1] == PATH_SEP)
			{
				m_CurrentPath = m_CurrentPath.substr(0, m_CurrentPath.size() - 1);
			}
			m_CurrentPath_Decomposition = splitStringToVector(m_CurrentPath, PATH_SEP, false);
#if defined(LINUX) or defined(APPLE)
			m_CurrentPath_Decomposition.insert(m_CurrentPath_Decomposition.begin(), std::string(1u, PATH_SEP));
#endif
			if (!m_CurrentPath_Decomposition.empty())
			{
#ifdef WIN32
				s_fs_root = m_CurrentPath_Decomposition[0];
#endif
			}
		}

		closedir(dir);
	}
}

bool ImGuiFileDialog::CreateDir(const std::string& vPath)
{
	bool res = false;

	if (!vPath.empty())
	{
		std::string path = m_CurrentPath + PATH_SEP + vPath;

		res = CreateDirectoryIfNotExist(path);
	}

	return res;
}

void ImGuiFileDialog::ComposeNewPath(std::vector<std::string>::iterator vIter)
{
	m_CurrentPath = "";

	while (true)
	{
		if (!m_CurrentPath.empty())
		{
#ifdef WIN32
			m_CurrentPath = *vIter + PATH_SEP + m_CurrentPath;
#elif defined(LINUX) or defined(APPLE)
			if (*vIter == s_fs_root)
			{
				m_CurrentPath = *vIter + m_CurrentPath;
			}
			else
			{
				m_CurrentPath = *vIter + PATH_SEP + m_CurrentPath;
			}
#endif
		}
		else
		{
			m_CurrentPath = *vIter;
		}

		if (vIter == m_CurrentPath_Decomposition.begin())
		{
#if defined(LINUX) or defined(APPLE)
			if (m_CurrentPath[0] != PATH_SEP)
				m_CurrentPath = PATH_SEP + m_CurrentPath;
#endif
			break;
		}

		vIter--;
	}
}

void ImGuiFileDialog::GetDrives()
{
	auto res = GetDrivesList();
	if (!res.empty())
	{
		m_CurrentPath = "";
		m_CurrentPath_Decomposition.clear();
		m_FileList.clear();
		for (auto & re : res)
		{
			FileInfoStruct infos;
			infos.fileName = re;
			infos.type = 'd';

			if (!infos.fileName.empty())
			{
				m_FileList.push_back(infos);
			}
		}
		m_ShowDrives = true;
	}
}

void ImGuiFileDialog::OpenDialog(const std::string& vKey, const char* vName, const char* vFilters,
	const std::string& vPath, const std::string& vDefaultFileName,
	const std::function<void(std::string, bool*)>& vOptionsPane, size_t vOptionsPaneWidth, const std::string& vUserString)
{
	if (m_ShowDialog) // si deja ouvert on ne fou pas la merde en voulant en ecrire une autre
		return;

	dlg_key = vKey;
	dlg_name = std::string(vName);
	dlg_filters = vFilters;
	dlg_path = vPath;
	dlg_defaultFileName = vDefaultFileName;
	dlg_optionsPane = std::move(vOptionsPane);
	dlg_userString = vUserString;
	dlg_optionsPaneWidth = vOptionsPaneWidth;

	dlg_defaultExt = "";

	m_ShowDialog = true;
}

void ImGuiFileDialog::OpenDialog(const std::string& vKey, const char* vName, const char* vFilters,
	const std::string& vFilePathName,
	const std::function<void(std::string, bool*)>& vOptionsPane, size_t vOptionsPaneWidth, const std::string& vUserString)
{
	if (m_ShowDialog) // si deja ouvert on ne fou pas la merde en voulant en ecrire une autre
		return;

	dlg_key = vKey;
	dlg_name = std::string(vName);
	dlg_filters = vFilters;

	auto ps = ParsePathFileName(vFilePathName);
	if (ps.isOk)
	{
		dlg_path = ps.path;
		dlg_defaultFileName = vFilePathName;
		dlg_defaultExt = "." + ps.ext;
	}
	else
	{
		dlg_path = ".";
		dlg_defaultFileName = "";
		dlg_defaultExt = "";
	}

	dlg_optionsPane = std::move(vOptionsPane);
	dlg_userString = vUserString;
	dlg_optionsPaneWidth = vOptionsPaneWidth;

	m_ShowDialog = true;
}

void ImGuiFileDialog::OpenDialog(const std::string& vKey, const char* vName, const char* vFilters,
	const std::string& vFilePathName, const std::string& vUserString)
{
	if (m_ShowDialog) // si deja ouvert on ne fou pas la merde en voulant en ecrire une autre
		return;

	dlg_key = vKey;
	dlg_name = std::string(vName);
	dlg_filters = vFilters;

	auto ps = ParsePathFileName(vFilePathName);
	if (ps.isOk)
	{
		dlg_path = ps.path;
		dlg_defaultFileName = vFilePathName;
		dlg_defaultExt = "." + ps.ext;
	}
	else
	{
		dlg_path = ".";
		dlg_defaultFileName = "";
		dlg_defaultExt = "";
	}

	dlg_optionsPane = nullptr;
	dlg_userString = vUserString;
	dlg_optionsPaneWidth = 0;

	m_ShowDialog = true;
}

void ImGuiFileDialog::OpenDialog(const std::string& vKey, const char* vName, const char* vFilters,
	const std::string& vPath, const std::string& vDefaultFileName, const std::string& vUserString)
{
	if (m_ShowDialog) // si deja ouvert on ne fou pas la merde en voulant en ecrire une autre
		return;

	dlg_key = vKey;
	dlg_name = std::string(vName);
	dlg_filters = vFilters;
	dlg_path = vPath;
	dlg_defaultFileName = vDefaultFileName;
	dlg_optionsPane = nullptr;
	dlg_userString = vUserString;
	dlg_optionsPaneWidth = 0;

	dlg_defaultExt = "";

	m_ShowDialog = true;
}

void ImGuiFileDialog::CloseDialog(const std::string& vKey)
{
	if (dlg_key == vKey)
	{
		dlg_key.clear();
		m_ShowDialog = false;
	}
}

void ImGuiFileDialog::SetPath(const std::string& vPath)
{
	m_ShowDrives = false;
	m_CurrentPath = vPath;
	m_FileList.clear();
	m_CurrentPath_Decomposition.clear();
	ScanDir(m_CurrentPath);
}

bool ImGuiFileDialog::FileDialog(const std::string& vKey, ImGuiWindowFlags vFlags)
{
	if (m_ShowDialog && dlg_key == vKey)
	{
		bool res = false;

		std::string name = dlg_name + "##" + dlg_key;

		if (m_Name != name)
		{
			m_FileList.clear();
			m_CurrentPath_Decomposition.clear();
		}

		IsOk = false;

		if (ImGui::Begin(name.c_str(), (bool*)nullptr, vFlags | ImGuiWindowFlags_NoScrollbar))
		{

			m_Name = name;

			m_AnyWindowsHovered |= ImGui::IsWindowHovered();

			if (dlg_path.empty()) dlg_path = ".";

			if (m_FileList.empty() && !m_ShowDrives)
			{
				replaceString(dlg_defaultFileName, dlg_path, ""); // local path

				if (!dlg_defaultFileName.empty())
				{
					ResetBuffer(FileNameBuffer);
					AppendToBuffer(FileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER, dlg_defaultFileName);
					m_SelectedFileName = dlg_defaultFileName;

					if (!dlg_defaultExt.empty())
					{
						m_SelectedExt = dlg_defaultExt;

						ImGuiFileDialog::FilterIndex = 0;
						size_t size = 0;
						const char* p = dlg_filters;       // FIXME-OPT: Avoid computing this, or at least only when combo is open
						while (*p)
						{
							size += strlen(p) + 1;
							p += size;
						}
						int idx = 0;
						auto arr = splitStringToVector(std::string(dlg_filters, size), '\0', false);
						for (auto & it : arr)
						{
							if (m_SelectedExt == it)
							{
								ImGuiFileDialog::FilterIndex = idx;
								break;
							}
							idx++;
						}
					}
				}

				ScanDir(dlg_path);
			}

			if (IMGUI_BUTTON(createDirButtonString))
			{
				if (!m_CreateDirectoryMode)
				{
					m_CreateDirectoryMode = true;
					ResetBuffer(DirectoryNameBuffer);
				}
			}

			if (m_CreateDirectoryMode)
			{
				ImGui::SameLine();

				ImGui::PushItemWidth(100.0f);
				ImGui::InputText("##DirectoryFileName", DirectoryNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER);
				ImGui::PopItemWidth();

				ImGui::SameLine();

				if (IMGUI_BUTTON(okButtonString))
				{
					std::string newDir = std::string(DirectoryNameBuffer);
					if (CreateDir(newDir))
					{
						SetPath(m_CurrentPath + PATH_SEP + newDir);
					}

					m_CreateDirectoryMode = false;
				}

				ImGui::SameLine();

				if (IMGUI_BUTTON(cancelButtonString))
				{
					m_CreateDirectoryMode = false;
				}
			}

			ImGui::SameLine();

			ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

			ImGui::SameLine();

			if (IMGUI_BUTTON(resetButtonString))
			{
				SetPath(".");
			}

			bool drivesClick = false;

#ifdef WIN32
			ImGui::SameLine();

			if (IMGUI_BUTTON(drivesButtonString))
			{
				drivesClick = true;
			}
#endif

			ImGui::SameLine();

			ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

			// show current path
			bool pathClick = false;
			if (!m_CurrentPath_Decomposition.empty())
			{
				ImGui::SameLine();
				for (auto itPathDecomp = m_CurrentPath_Decomposition.begin();
					itPathDecomp != m_CurrentPath_Decomposition.end(); ++itPathDecomp)
				{
					if (itPathDecomp != m_CurrentPath_Decomposition.begin())
						ImGui::SameLine();
					if (IMGUI_PATH_BUTTON((*itPathDecomp).c_str()))
					{
						ComposeNewPath(itPathDecomp);
						pathClick = true;
						break;
					}
				}
			}

			ImGuiContext& g = *GImGui;
			const float itemsHeight = (g.FontSize + g.Style.FramePadding.y * 2.0f + g.Style.ItemSpacing.y * 2.0f) * 4.0f + g.Style.WindowPadding.y * 2.0f;
			ImVec2 size = ImGui::GetContentRegionMax() - ImVec2((float)dlg_optionsPaneWidth, itemsHeight);

			// search field
			if (IMGUI_BUTTON("R##ImGuiFileDialogSearchFiled"))
			{
				ResetBuffer(SearchBuffer);
				searchTag.clear();
			}
			ImGui::SameLine();
			ImGui::Text(searchString);
			ImGui::SameLine();
			if (ImGui::InputText("##ImGuiFileDialogSearchFiled", SearchBuffer, MAX_FILE_DIALOG_NAME_BUFFER))
			{
				searchTag = SearchBuffer;
			}

			ImGui::BeginChild("##FileDialog_FileList", size);

			for (auto & it : m_FileList)
			{
				const FileInfoStruct& infos = it;

				bool show = true;

				std::string str;
				if (infos.type == 'd') str = dirEntryString + infos.fileName;
				if (infos.type == 'l') str = linkEntryString + infos.fileName;
				if (infos.type == 'f') str = fileEntryString + infos.fileName;
				if (infos.type == 'f' && !m_SelectedExt.empty() && (infos.ext != m_SelectedExt && m_SelectedExt != ".*"))
				{
					show = false;
				}
				if (!searchTag.empty() && infos.fileName.find(searchTag) == std::string::npos)
				{
					show = false;
				}
				if (show)
				{
					ImVec4 c;
					bool showColor = GetFilterColor(infos.ext, &c);
					if (showColor)
						ImGui::PushStyleColor(ImGuiCol_Text, c);

					if (ImGui::Selectable(str.c_str(), (infos.fileName == m_SelectedFileName)))
					{
						if (infos.type == 'd')
						{
							if (it.fileName == "..")
							{
								if (m_CurrentPath_Decomposition.size() > 1)
								{
									ComposeNewPath(m_CurrentPath_Decomposition.end() - 2);
									pathClick = true;
								}
							}
							else
							{
								std::string newPath;

								if (m_ShowDrives)
								{
									newPath = infos.fileName + PATH_SEP;
								}
								else
								{
#ifdef LINUX
									if (s_fs_root == m_CurrentPath)
									{
										newPath = m_CurrentPath + infos.fileName;
									}
									else
									{
#endif
										newPath = m_CurrentPath + PATH_SEP + infos.fileName;
#ifdef LINUX
									}
#endif
								}

								if (IsDirectoryExist(newPath))
								{
									if (m_ShowDrives)
									{
										m_CurrentPath = infos.fileName;
										s_fs_root = m_CurrentPath;
									}
									else
									{
										m_CurrentPath = newPath;
									}
									pathClick = true;
								}
							}
						}
						else
						{
							m_SelectedFileName = infos.fileName;
							ResetBuffer(FileNameBuffer);
							AppendToBuffer(FileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER, m_SelectedFileName);
						}
						if (showColor)
							ImGui::PopStyleColor();
						break;
					}

					if (showColor)
						ImGui::PopStyleColor();
				}
			}

			// changement de repertoire
			if (pathClick)
			{
				SetPath(m_CurrentPath);
			}

			if (drivesClick)
			{
				GetDrives();
			}

			ImGui::EndChild();

			bool _CanWeContinue = true;

			if (dlg_optionsPane)
			{
				ImGui::SameLine();

				size.x = (float)dlg_optionsPaneWidth;

				ImGui::BeginChild("##FileTypes", size);

				dlg_optionsPane(m_SelectedExt, &_CanWeContinue);

				ImGui::EndChild();
			}

			ImGui::Text(fileNameString);

			ImGui::SameLine();

			float width = ImGui::GetContentRegionAvail().x;
			if (dlg_filters) width -= 120.0f;
			ImGui::PushItemWidth(width);
			ImGui::InputText("##FileName", FileNameBuffer, MAX_FILE_DIALOG_NAME_BUFFER);
			ImGui::PopItemWidth();

			if (dlg_filters)
			{
				ImGui::SameLine();

				ImGui::PushItemWidth(100.0f);
				bool comboClick = ImGui::Combo("##Filters", &FilterIndex, dlg_filters) || m_SelectedExt.empty();
				ImGui::PopItemWidth();
				if (comboClick)
				{
					int itemIdx = 0;
					const char* p = dlg_filters;
					while (*p)
					{
						if (FilterIndex == itemIdx)
						{
							m_SelectedExt = std::string(p);
							break;
						}
						p += strlen(p) + 1;
						itemIdx++;
					}
				}
			}

			if (IMGUI_BUTTON(cancelButtonString))
			{
				IsOk = false;
				res = true;
			}

			if (_CanWeContinue)
			{
				ImGui::SameLine();

				if (IMGUI_BUTTON(okButtonString))
				{
					if ('\0' != FileNameBuffer[0])
					{
						IsOk = true;
						res = true;
					}
				}
			}
		}

		ImGui::End();

		if (res)
		{
			m_FileList.clear();
		}

		return res;
	}

	return false;
}

std::string ImGuiFileDialog::GetFilepathName()
{
	std::string  result = m_CurrentPath;

	if (s_fs_root != result)
	{
		result += PATH_SEP;
	}

	result += GetCurrentFileName();

	return result;
}

std::string ImGuiFileDialog::GetCurrentPath()
{
	return m_CurrentPath;
}

std::string ImGuiFileDialog::GetCurrentFileName()
{
	std::string result = FileNameBuffer;

	size_t lastPoint = result.find_last_of('.');
	if (lastPoint != std::string::npos)
	{
		result = result.substr(0, lastPoint);
	}

	result += m_SelectedExt;

	return result;
}

std::string ImGuiFileDialog::GetCurrentFilter()
{
	return m_SelectedExt;
}

std::string ImGuiFileDialog::GetUserString()
{
	return dlg_userString;
}

void ImGuiFileDialog::SetFilterColor(const std::string& vFilter, ImVec4 vColor)
{
	m_FilterColor[vFilter] = vColor;
}

bool ImGuiFileDialog::GetFilterColor(const std::string& vFilter, ImVec4 *vColor)
{
	if (vColor)
	{
		if (m_FilterColor.find(vFilter) != m_FilterColor.end()) // found
		{
			*vColor = m_FilterColor[vFilter];
			return true;
		}
	}
	return false;;
}

void ImGuiFileDialog::ClearFilterColor()
{
	m_FilterColor.clear();
}
