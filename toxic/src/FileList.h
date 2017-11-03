#pragma once

#include "stdafx.h"

#if WIN32

#include <windows.h>

class FileList
{
    public:
        enum
        {
            FILE = 0,
            DIR = 1,
        };
        
		inline FileList(const std::string &strPattern)
        {
            m_hFind = FindFirstFileA(strPattern.c_str(), &m_Wfd);
        }

		inline ~FileList()
        {
            if(m_hFind != INVALID_HANDLE_VALUE)
                FindClose(m_hFind);
        }

        /* Returns 1 for directory, 0 for file */
        inline int GetNextFile(std::string &strName)
        {
            int iRet;

            do
            {
                if(m_hFind == INVALID_HANDLE_VALUE)
                    return -1;

                strName = m_Wfd.cFileName;
                iRet = (m_Wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? DIR : FILE;

                if(!FindNextFileA(m_hFind, &m_Wfd))
                {
                    FindClose(m_hFind);
                    m_hFind = INVALID_HANDLE_VALUE;
                }
            } while(strName == "." || strName == "..");

            return iRet;
        }

    private:
        HANDLE m_hFind;
        WIN32_FIND_DATAA m_Wfd;
};

#else // LINUX

#include <glob.h>

class FileList
{
    public:
		 enum
        {
            FILE = 0,
            DIR = 1,
        };
        inline FileList(const std::string &strPattern)
        {
            // Escape special characters in pattern (except *)
            std::string strEscapedPattern;
            strEscapedPattern.reserve(strPattern.size());
            for(unsigned i = 0; i < strPattern.size(); ++i)
            {
                if(strPattern[i] == '[' || strPattern[i] == ']' || strPattern[i] == '\\' || strPattern[i] == '?')
                    strEscapedPattern += '\\';
                
                strEscapedPattern += strPattern[i];
            }
            
            glob(strEscapedPattern.c_str(), GLOB_MARK, NULL, &m_GlobBuf);
            m_nCurrentPath = 0;
        }

        inline ~FileList()
        {
            globfree(&m_GlobBuf);
        }

        /* Returns 1 for directory, 0 for file */
        inline int GetNextFile(std::string &strName)
        {
            int iRet;

            do
            {
                if(m_nCurrentPath >= m_GlobBuf.gl_pathc)
                    return -1;

                const char *pszPath = m_GlobBuf.gl_pathv[m_nCurrentPath++];
                strName = pszPath;
                if(!strName.empty() && strName[strName.size() - 1] == '/')
                {
                     iRet = DIR;
                     strName.resize(strName.size() - 1);
                } else
                     iRet = FILE;
                
                size_t SlashPos = strName.rfind('/');
                if(SlashPos != std::string::npos)
                    strName.erase(0, SlashPos + 1); // +1 for slash
            }
            while(strName == "." || strName == "..");

            return iRet;
        }

    private:
        unsigned m_nCurrentPath;
        glob_t m_GlobBuf;
};

#endif // LINUX
