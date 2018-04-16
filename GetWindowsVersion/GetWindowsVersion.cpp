// Copyright 2018 CEZEO software Ltd. ( http://www.cezeo.com ). All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.
// Getting Windows version directly from ntoskrnl.exe without virtualization
// GetWindowsVersion.cpp : Defines the entry point for the application.

#include "stdafx.h"
#include "GetWindowsVersion.h"
#include <string>
#include <windows.h>

#pragma comment( lib, "version.lib" )

std::string GetNtoskrnlVersion()
{
  std::string versionString;
  DWORD someHandle;
  wchar_t systemFolderPath[ _MAX_PATH + 1 ];
  UINT systemFolderPathSize = GetSystemDirectory( systemFolderPath, _MAX_PATH );
  if ( systemFolderPathSize > 0 )
  {
    std::wstring ntDllPath( systemFolderPath, systemFolderPathSize );
    ntDllPath += L"\\ntoskrnl.exe";
    DWORD versionSize = GetFileVersionInfoSize( ntDllPath.c_str(), &someHandle );
    if ( versionSize > 0 )
    {
      LPVOID dataPtr = malloc( versionSize );
      if ( dataPtr != NULL )
      {
        if ( GetFileVersionInfo( ntDllPath.c_str(), 0, versionSize, dataPtr ) )
        {
          UINT length;
          LPVOID outputPtr;
          if ( VerQueryValue( dataPtr, L"\\", &outputPtr, &length ) )
          {
            VS_FIXEDFILEINFO* versionStructPtr = ( VS_FIXEDFILEINFO* ) outputPtr;
            if ( versionStructPtr->dwSignature == 0xFEEF04BD )
            {
              versionString = std::to_string( HIWORD( versionStructPtr->dwFileVersionMS ) );
              versionString += ".";
              versionString += std::to_string( LOWORD( versionStructPtr->dwFileVersionMS ) );
              versionString += ".";
              versionString += std::to_string( HIWORD( versionStructPtr->dwFileVersionLS ) );
              versionString += ".";
              versionString += std::to_string( LOWORD( versionStructPtr->dwFileVersionLS ) );
            }
          }
        }
        free( dataPtr );
      }
    }
  }
  return versionString;
}

int APIENTRY _tWinMain( HINSTANCE, HINSTANCE, LPTSTR, int )
{
  MessageBoxA( NULL, GetNtoskrnlVersion().c_str(), "GetNtoskrnlVersion", SW_NORMAL );
	return 0;
}
