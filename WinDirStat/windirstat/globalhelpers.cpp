// globalhelpers.cpp - Implementation of global helper functions
//
// WinDirStat - Directory Statistics
// Copyright (C) 2003-2004 Bernhard Seifert
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: bseifert@users.sourceforge.net, bseifert@daccord.net
//
// Last modified: $Date$

#include "stdafx.h"
#include "globalhelpers.h"

//#include "windirstat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BASE 1024
#define HALF_BASE BASE/2
namespace
{
	CString FormatLongLongNormal( _In_ LONGLONG n ) {
		// Returns formatted number like "123.456.789".

		ASSERT( n >= 0 );
		CString all;
		do
		{
			auto rest = INT( n % 1000 );
			n /= 1000;

			CString s;
			if ( n > 0 ) {
				s.Format( _T( ",%03d" ) , rest );
				}
			else {
				s.Format( _T( "%d" ), rest );
				}
			all = s + all;
			}
		while ( n > 0 );
		return all;
		}

	_Success_( SUCCEEDED( return ) ) HRESULT CStyle_FormatLongLongNormal( _In_ LONGLONG n, _Out_writes_z_( strSize ) PWSTR psz_formatted_LONGLONG_normal, _In_range_( 3, 64 ) size_t strSize ) {
		size_t s_formatted_count = strSize;
		PWSTR working_ptr = psz_formatted_LONGLONG_normal;
		PWSTR output = psz_formatted_LONGLONG_normal;
		size_t unwrittenCharacters = strSize;
		do
		{
			auto rest = INT( n % 1000 );
			n /= 1000;
			const size_t sSize = 6;
			wchar_t s[ sSize ] = { 0 };
			
			

			HRESULT res = ERROR_FUNCTION_FAILED;
			HRESULT res2 = ERROR_FUNCTION_FAILED;
			if ( n > 0 ) {
				res = StringCchPrintf( s, sSize, L",%03d", rest );
				}
			else {
				res = StringCchPrintf( s, sSize, L"%d", rest );
				}
			if ( SUCCEEDED( res ) ) {
				res2 = StringCchCatEx( working_ptr, strSize - (strSize - unwrittenCharacters), s, &output, &unwrittenCharacters, 0 );
				if ( !SUCCEEDED( res2 ) ) {
					return res2;
					}
				working_ptr = output;
				ASSERT( ( psz_formatted_LONGLONG_normal + ( strSize - ( strSize - unwrittenCharacters ) ) ) == working_ptr );
				}
			else {
				psz_formatted_LONGLONG_normal[ 0 ] = 0;
				return res;
				}
			
			}
		while ( n > 0 );

		}

	void CacheString( _Inout_ CString& s, _In_ UINT resId, _In_z_ LPCTSTR defaultVal ) {
		ASSERT( lstrlen( defaultVal ) > 0 );
		if ( s.IsEmpty( ) ) {
			s = LoadString( resId );		
			if ( s.IsEmpty( ) ) {
				s = defaultVal;
				}
			}
		}

}

CString GetLocaleString( _In_ const LCTYPE lctype, _In_ const LANGID langid ) {
	LCID lcid = MAKELCID( langid, SORT_DEFAULT );
	auto len = GetLocaleInfo( lcid, lctype, NULL, 0 );
	CString s;
	GetLocaleInfo( lcid, lctype, s.GetBuffer( len ), len );
	s.ReleaseBuffer( );
	return s;
	}

CString GetLocaleLanguage( _In_ const LANGID langid ) {
	auto s = GetLocaleString( LOCALE_SNATIVELANGNAME, langid );

	// In the French case, the system returns "francais", but we want "Francais".

	if ( s.GetLength( ) > 0 ) {
		s.SetAt( 0, ( TCHAR ) _totupper( s[ 0 ] ) );
		}

	return s + _T(" - ") + GetLocaleString(LOCALE_SNATIVECTRYNAME, langid);
	}


CString FormatBytes( _In_ const LONGLONG n ) {
	if ( GetOptions( )->IsHumanFormat( ) ) {
#ifdef DEBUG 

#endif
		return FormatLongLongHuman( n );
		}
	else {
		return FormatLongLongNormal( n );
		}
	}

//_Success_( SUCCEEDED( return ) ) HRESULT CStyle_FormatBytes( _In_ LONGLONG n, _Out_writes_z_( strSize ) PWSTR psz_formatted_bytes, _In_range_( 3, 64 ) size_t strSize ) {
//	return ERROR_FUNCTION_FAILED;
	//}

CString FormatLongLongHuman( _In_ LONGLONG n ) {
	// Returns formatted number like "12,4 GB".
	ASSERT( n >= 0 );
	CString s;

	DOUBLE B  = INT( n % BASE );
	n /= BASE;
	DOUBLE KB = INT( n % BASE );
	n /= BASE;
	DOUBLE MB = INT( n % BASE );
	n /= BASE;
	DOUBLE GB = INT( n % BASE );
	n /= BASE;
	DOUBLE TB = INT( n );

	if ( TB != 0 || GB == BASE - 1 && MB >= HALF_BASE ) {
		s.Format( _T( "%s TB" ), FormatDouble( TB + GB / BASE ).GetString( ) );
		}
	else if ( GB != 0 || MB == BASE - 1 && KB >= HALF_BASE ) {
		s.Format( _T( "%s GB" ), FormatDouble( GB + MB / BASE ).GetString( ) );
		}
	else if ( MB != 0 || KB == BASE - 1 && B >= HALF_BASE ) {
		s.Format( _T( "%s MB" ), FormatDouble( MB + KB / BASE ).GetString( ) );
		}
	else if ( KB != 0 ) {
		s.Format( _T( "%s KB" ), FormatDouble( KB + B / BASE ).GetString( ) );
		}
	else if ( B  != 0 ) {
		s.Format( _T( "%i Bytes" ), ( INT ) B );
		}
	else {
		s = _T( "0" );
		}
	return s;
	}

_Success_( SUCCEEDED( return ) ) HRESULT CStyle_FormatLongLongHuman( _In_ LONGLONG n, _Out_writes_z_( strSize ) PWSTR psz_formatted_LONGLONG_HUMAN, _In_range_( 3, 64 ) size_t strSize ) {
	ASSERT( n >= 0 );
	DOUBLE B  = INT( n % BASE );
	n /= BASE;
	DOUBLE KB = INT( n % BASE );
	n /= BASE;
	DOUBLE MB = INT( n % BASE );
	n /= BASE;
	DOUBLE GB = INT( n % BASE );
	n /= BASE;
	DOUBLE TB = INT( n );
	const size_t bufSize = 12;
	const size_t bufSize2 = bufSize * 2;
	wchar_t buffer[ bufSize ] = { 0 };
	wchar_t buffer2[ bufSize2 ] = { 0 };
	HRESULT res = ERROR_FUNCTION_FAILED;
	HRESULT res2 = ERROR_FUNCTION_FAILED;
	//return StringCchPrintf( psz_formatted_double, strSize, L"%.1f%%", d );
	if ( TB != 0 || GB == BASE - 1 && MB >= HALF_BASE ) {
		res = CStyle_FormatDouble( TB + GB / BASE, buffer, bufSize );
		if ( SUCCEEDED( res ) ) {
			res2 = StringCchPrintf( buffer2, bufSize2, L"%s TB", buffer );
			}
		}
	else if ( GB != 0 || MB == BASE - 1 && KB >= HALF_BASE ) {
		res = CStyle_FormatDouble( GB + MB / BASE, buffer, bufSize );
		if ( SUCCEEDED( res ) ) {
			res2 = StringCchPrintf( buffer2, bufSize2, L"%s GB", buffer );
			}
		}
	else if ( MB != 0 || KB == BASE - 1 && B >= HALF_BASE ) {
		res = CStyle_FormatDouble( MB + KB / BASE, buffer, bufSize );
		if ( SUCCEEDED( res ) ) {
			res2 = StringCchPrintf( buffer2, bufSize2, L"%s MB", buffer );
			}
		}
	else if ( KB != 0 ) {
		res = CStyle_FormatDouble( KB + B / BASE, buffer, bufSize );
		if ( SUCCEEDED( res ) ) {
			res2 = StringCchPrintf( buffer2, bufSize2, L"%s KB", buffer );
			}
		}
	else if ( B  != 0 ) {
		res = StringCchPrintf( buffer2, bufSize2, L"%i Bytes", INT( B ) );
		res2 = res;
		}
	else {
		res = StringCchPrintf( buffer2, bufSize2, L"0%s", L"\0" );
		res2 = res;
		}
	if ( !SUCCEEDED( res2 ) ) {
		buffer2[ 0 ] = 'B';
		buffer2[ 1 ] = 'A';
		buffer2[ 2 ] = 'D';
		buffer2[ 3 ] = '_';
		buffer2[ 4 ] = 'F';
		buffer2[ 5 ] = 'M';
		buffer2[ 6 ] = 'T';
		buffer2[ 7 ] = 0;
		}
	return StringCchCopy( psz_formatted_LONGLONG_HUMAN, strSize, buffer2 );
	}

CString FormatCount( _In_ const std::uint32_t n ) {
	return FormatLongLongNormal( LONGLONG( n ) );
	}

CString FormatCount( _In_ const LONGLONG n ) {
	return FormatLongLongNormal( n );
	}

CString FormatDouble( _In_ DOUBLE d ) {// "98,4" or "98.4"
	CString s;
	s.Format( _T( "%.1f" ), d );
	return s;
	}

_Success_( SUCCEEDED( return ) ) HRESULT CStyle_FormatDouble( _In_ DOUBLE d, _Out_writes_z_( strSize ) PWSTR psz_formatted_double, _In_range_( 3, 64 ) size_t strSize ) {
	//Range 3-64 is semi-arbitrary. I don't think I'll need to format a double that's more than 63 chars.
	return StringCchPrintf( psz_formatted_double, strSize, L"%.1f%%", d );
	}

CString PadWidthBlanks( _In_ CString n, _In_ const INT width ) {
	ASSERT( width >= 0 );
	auto blankCount = width - n.GetLength( );
	if ( blankCount > 0 ) {
		CString b;
		PTSTR psz = b.GetBuffer( blankCount + 1 );
		for ( INT i = 0; i < blankCount; i++ ) {
			psz[ i ] = _T( ' ' );
			psz[ i ] = 0;
			}
		b.ReleaseBuffer( );
		n = b + n;
		}
	return n;
	}

CString FormatFileTime( _In_ const FILETIME& t ) {
	ASSERT( &t != NULL );
	SYSTEMTIME st;
	if ( !FileTimeToSystemTime( &t, &st ) ) {
		return MdGetWinerrorText( HRESULT( GetLastError( ) ) );
		}
	LCID lcid = MAKELCID( GetUserDefaultLangID( ), SORT_DEFAULT );
	CString date;
	VERIFY( 0 < GetDateFormat( lcid, DATE_SHORTDATE, &st, NULL, date.GetBuffer( 256 ), 256 ) );//d M yyyy
	date.ReleaseBuffer( );

#ifdef C_STYLE_STRINGS
	wchar_t psz_formatted_datetime[ 73 ] = { 0 };
	auto res = CStyle_FormatFileTime( t, psz_formatted_datetime, 73 );

#ifdef _DEBUG
	CString time;
	VERIFY( 0 < GetTimeFormat( lcid, 0, &st, NULL, time.GetBuffer( 256 ), 256 ) );//h mm ss tt
	time.ReleaseBuffer( );
	CString result = date + _T( "  " ) + time;
#endif
#else
	CString time;
	VERIFY( 0 < GetTimeFormat( lcid, 0, &st, NULL, time.GetBuffer( 256 ), 256 ) );//h mm ss tt
	time.ReleaseBuffer( );
	CString result = date + _T( "  " ) + time;
	return result;
#endif


#ifdef C_STYLE_STRINGS
#ifdef _DEBUG
	auto didMatch = result.Compare( psz_formatted_datetime );
	ASSERT( didMatch == 0 );
	TRACE( _T( "Formatted file time (%i characters): %s\r\n" ), result.GetLength( ), result );
	TRACE( _T( "Formatted file time  C-STYLE       : %s\r\n" ), psz_formatted_datetime );
#endif
	if ( res == 0 ) {
		return psz_formatted_datetime;
		}
	else {
		psz_formatted_datetime[ 0 ] = 'B';
		psz_formatted_datetime[ 1 ] = 'A';
		psz_formatted_datetime[ 2 ] = 'D';
		psz_formatted_datetime[ 3 ] = '_';
		psz_formatted_datetime[ 4 ] = 'F';
		psz_formatted_datetime[ 5 ] = 'M';
		psz_formatted_datetime[ 6 ] = 'T';
		psz_formatted_datetime[ 7 ] = 0;

		}
#endif
	}

_Success_( return == 0 ) int CStyle_FormatFileTime( _In_ const FILETIME& t, _Out_writes_z_( strSize ) PWSTR psz_formatted_datetime, int strSize  ) {
	ASSERT( &t != NULL );
	SYSTEMTIME st;
	if ( !FileTimeToSystemTime( &t, &st ) ) {
		psz_formatted_datetime = PWSTR( MdGetWinerrorText( HRESULT( GetLastError( ) ) ).GetString( ) );
		return 1;
		}
	LCID lcid = MAKELCID( GetUserDefaultLangID( ), SORT_DEFAULT );

	wchar_t psz_date_wchar[ 36 ] = { 0 };
	wchar_t psz_time_wchar[ 36 ] = { 0 };
	//wchar_t psz_formatted_datetime[ 73 ];
	auto gdfres = GetDateFormat( lcid, DATE_SHORTDATE, &st, NULL, psz_date_wchar, 36 );
	auto gtfres = GetTimeFormat( lcid, 0, &st, NULL, psz_time_wchar, 36 );

	auto cpyres  = wcscpy_s( psz_formatted_datetime, gdfres, psz_date_wchar );
	auto wcsres  = wcscat_s( psz_formatted_datetime, strSize, L"  " );
	auto wcsres2 = wcscat_s( psz_formatted_datetime, strSize, psz_time_wchar );

	auto lError = GetLastError( );

	return cpyres + wcsres + wcsres2;
	}

CString FormatAttributes( _In_ const DWORD attr ) {
	if ( attr == INVALID_FILE_ATTRIBUTES ) {
		return _T( "?????" );
		}

	CString attributes;
	attributes.Append( ( attr & FILE_ATTRIBUTE_READONLY )   ? _T( "R" ) : _T( "" ) );
	attributes.Append( ( attr & FILE_ATTRIBUTE_HIDDEN )     ? _T( "H" ) : _T( "" ) );
	attributes.Append( ( attr & FILE_ATTRIBUTE_SYSTEM )     ? _T( "S" ) : _T( "" ) );
	attributes.Append( ( attr & FILE_ATTRIBUTE_ARCHIVE )    ? _T( "A" ) : _T( "" ) );
	attributes.Append( ( attr & FILE_ATTRIBUTE_COMPRESSED ) ? _T( "C" ) : _T( "" ) );
	attributes.Append( ( attr & FILE_ATTRIBUTE_ENCRYPTED )  ? _T( "E" ) : _T( "" ) );

	return attributes;
	}

_Success_( return == 0 ) int CStyle_FormatAttributes( _In_ const DWORD attr, _Out_writes_z_( strSize ) PWSTR psz_formatted_attributes, _In_range_( 1, 6 ) int strSize ) {
	if ( attr == INVALID_FILE_ATTRIBUTES ) {
		psz_formatted_attributes = _T( "?????" );
		}
	int errCode[ 6 ] = { 0 };
	int charsWritten = 0;
	CString attributes;
	if ( ( attr & FILE_ATTRIBUTE_READONLY ) != 0 ) {
		errCode[ 0 ] = wcscpy_s( psz_formatted_attributes + charsWritten, strSize - 1 - charsWritten, L"R" );
		charsWritten += ( ( errCode[ 0 ] == 0 ) ? 1 : 0 );
		}
	if ( ( attr & FILE_ATTRIBUTE_HIDDEN ) != 0 ) {
		errCode[ 1 ] = wcscpy_s( psz_formatted_attributes + charsWritten, strSize - 1 - charsWritten, L"H" );
		charsWritten += ( ( errCode[ 1 ] == 0 ) ? 1 : 0 );
		}
	if ( ( attr & FILE_ATTRIBUTE_SYSTEM ) != 0 ) {
		errCode[ 2 ] = wcscpy_s( psz_formatted_attributes + charsWritten, strSize - 1 - charsWritten, L"S" );
		charsWritten += ( ( errCode[ 2 ] == 0 ) ? 1 : 0 );
		}
	if ( ( attr & FILE_ATTRIBUTE_ARCHIVE ) != 0 ) {
		errCode[ 3 ] = wcscpy_s( psz_formatted_attributes + charsWritten, strSize - 1 - charsWritten, L"A" );
		charsWritten += ( ( errCode[ 3 ] == 0 ) ? 1 : 0 );
		}
	if ( ( attr & FILE_ATTRIBUTE_COMPRESSED ) != 0 ) {
		errCode[ 4 ] = wcscpy_s( psz_formatted_attributes + charsWritten, strSize - 1 - charsWritten, L"C" );
		charsWritten += ( ( errCode[ 4 ] == 0 ) ? 1 : 0 );
		}
	if ( ( attr & FILE_ATTRIBUTE_ENCRYPTED ) != 0 ) {
		errCode[ 5 ] = wcscpy_s( psz_formatted_attributes + charsWritten, strSize - 1 - charsWritten, L"E" );
		charsWritten += ( ( errCode[ 5 ] == 0 ) ? 1 : 0 );
		}
	ASSERT( charsWritten < strSize );
	ASSERT( strSize > 0 );
	psz_formatted_attributes[ strSize - 1 ] = 0;
	return std::accumulate( errCode, errCode + 6, 0 );
	}

CString FormatMilliseconds( _In_ const unsigned long long ms ) {
	CString ret;
	unsigned long long  sec = ( ms + 500 ) / 1000;
	unsigned long long  s   = sec % 60;
	unsigned long long  min = sec / 60;
	unsigned long long  m   = min % 60;
	unsigned long long  h   = min / 60;

	static_assert( sizeof( unsigned long long ) == 8, "Format strings will be invalid!!! llu != 64 bit unsigned integer!" );
	if ( h > 0 ) {
		
		ret.Format( _T( "%llu:%02llu:%02llu" ), h, m, s );
		}
	else {
		ret.Format( _T( "%llu:%02llu" ), m, s );
		}
	return ret;
	}

bool GetVolumeName( _In_z_ const LPCTSTR rootPath, _Out_ CString& volumeName ) {
	CString ret;
	DWORD dummy;

	auto old = SetErrorMode( SEM_FAILCRITICALERRORS );
	
	//GetVolumeInformation returns 0 on failure
	BOOL b = GetVolumeInformation( rootPath, volumeName.GetBuffer( 256 ), 256, &dummy, &dummy, &dummy, NULL, 0 );
	volumeName.ReleaseBuffer( );

	if ( b == 0 ) {
		TRACE( _T( "GetVolumeInformation(%s) failed: %u\n" ), rootPath, GetLastError( ) );
		}
	SetErrorMode( old );
	
	return ( b != 0 );
	}

CString FormatVolumeNameOfRootPath( _In_ const CString rootPath ) {
	/*
	  Given a root path like "C:\", this function obtains the volume name and returns a complete display string like "BOOT (C:)".
	*/
	ASSERT( rootPath != _T( "" ) );
	CString volumeName;
	bool b = GetVolumeName( rootPath, volumeName );
	if ( b ) {
		return FormatVolumeName( rootPath, volumeName );
		}
	return rootPath;
	}

CString FormatVolumeName( _In_ const CString rootPath, _In_ const CString volumeName ) {
	ASSERT( rootPath != _T( "" ) );
	CString ret;
	ret.Format( _T( "%s (%s)" ), volumeName.GetString( ), rootPath.Left( 2 ).GetString( ) );
	return ret;
	}

CString PathFromVolumeName( _In_ const CString name ) {
	/*
	  The inverse of FormatVolumeNameOfRootPath().
	  Given a name like "BOOT (C:)", it returns "C:" (without trailing backslash).
	  Or, if name like "C:\", it returns "C:".
	*/
	ASSERT( name != _T( "" ) );
	auto i = name.ReverseFind( _T( ')' ) );
	if ( i == -1 ) {
		ASSERT( name.GetLength( ) == 3 );
		return name.Left( 2 );
		}

	ASSERT( i != -1 );
	auto k = name.ReverseFind( _T( '(' ) );
	ASSERT( k != -1 );
	ASSERT( k < i );
	auto path = name.Mid( k + 1, i - k - 1 );
	ASSERT( path.GetLength( ) == 2 );
	ASSERT( path[ 1 ] == _T( ':' ) );

	return path;
	}


CString GetParseNameOfMyComputer( ) /*throw ( CException * )*/ {
	/*
	  Retrieve the "fully qualified parse name" of "My Computer"
	*/
	CComPtr<IShellFolder> sf;
	HRESULT hr = SHGetDesktopFolder( &sf );
	MdThrowFailed( hr, _T( "SHGetDesktopFolder" ) );

	CCoTaskMem<LPITEMIDLIST> pidl;

	hr = SHGetSpecialFolderLocation( NULL, CSIDL_DRIVES, &pidl );
	MdThrowFailed( hr, _T( "SHGetSpecialFolderLocation(CSIDL_DRIVES)" ) );

	STRRET name;
	name.pOleStr = NULL;
	name.uOffset = NULL;
	name.uType = STRRET_CSTR;
	hr = sf->GetDisplayNameOf( pidl, SHGDN_FORPARSING, &name );
	MdThrowFailed( hr, _T( "GetDisplayNameOf(My Computer)" ) );

	return MyStrRetToString( pidl, &name );
	}

void GetPidlOfMyComputer( _Inout_ LPITEMIDLIST *ppidl ) /*throw ( CException * )*/ {
	CComPtr<IShellFolder> sf;
	HRESULT hr = SHGetDesktopFolder( &sf );
	if ( hr == S_OK ) {
		MdThrowFailed( hr, _T( "SHGetDesktopFolder" ) );

		hr = SHGetSpecialFolderLocation( NULL, CSIDL_DRIVES, ppidl ); //TODO: DEPRECIATED! 
		if ( hr != S_OK ) {
			ASSERT( false );
			TRACE( _T( "Failed SHGetSpecialFolderLocation!!\r\n" ) );
			}
		MdThrowFailed( hr, _T( "SHGetSpecialFolderLocation( CSIDL_DRIVES )" ) );
		}
	else {
		ASSERT( false );
		TRACE( _T( "Failed to get PidlOfMyComputer!\r\n" ) );
		displayWindowsMsgBoxWithError( );
		}
	}

void ShellExecuteWithAssocDialog( _In_ const HWND hwnd, _In_z_ const LPCTSTR filename ) /*throw ( CException * )*/ {
	CWaitCursor wc;
	auto u = reinterpret_cast<UINT>( ShellExecute( hwnd, NULL, filename, NULL, NULL, SW_SHOWNORMAL ) );
	if ( u == SE_ERR_NOASSOC ) {
		// Q192352
		CString sysDir;
		//-- Get the system directory so that we know where Rundll32.exe resides.
		GetSystemDirectory( sysDir.GetBuffer( _MAX_PATH ), _MAX_PATH );
		sysDir.ReleaseBuffer( );
		
		CString parameters = _T( "shell32.dll,OpenAs_RunDLL " );
		u = reinterpret_cast<UINT>( ShellExecute( hwnd, _T( "open" ), _T( "RUNDLL32.EXE" ), parameters + filename, sysDir, SW_SHOWNORMAL ) );
		}
		
	if ( u <= 32 ) {
		std::wstring a;
		a += ( _T( "ShellExecute failed: %1!s!" ), GetShellExecuteError( u ) );
		//MdThrowStringExceptionF( _T( "ShellExecute failed: %1!s!" ), GetShellExecuteError( u ) );
		MdThrowStringExceptionF( a.c_str( ) );
		}
	}

void MyGetDiskFreeSpace( _In_z_ const LPCTSTR pszRootPath, _Inout_ LONGLONG& total, _Inout_ LONGLONG& unused ) {
	//ASSERT( pszRootPath != _T( "" ) );
	ULARGE_INTEGER uavailable = { { 0 } };
	ULARGE_INTEGER utotal     = { { 0 } };
	ULARGE_INTEGER ufree      = { { 0 } };
	uavailable.QuadPart       = 0;
	utotal.QuadPart           = 0;
	ufree.QuadPart            = 0;

	// On NT 4.0, the 2nd Parameter to this function must NOT be NULL.
	BOOL b = GetDiskFreeSpaceEx( pszRootPath, &uavailable, &utotal, &ufree );
	if ( !b ) {
		TRACE( _T( "\tGetDiskFreeSpaceEx(%s) failed.\r\n" ), pszRootPath );
		}
	else {
		TRACE( _T( "\tGetDiskFreeSpaceEx(%s) successfully returned uavailable: %llu, utotal: %llu, ufree: %llu\r\n" ), pszRootPath, uavailable, utotal, ufree );
		ASSERT( uavailable.QuadPart <= utotal.QuadPart);
		ASSERT( ufree.QuadPart <= utotal.QuadPart );
		ASSERT( uavailable.QuadPart != utotal.QuadPart );
		ASSERT( ufree.QuadPart != utotal.QuadPart );
		}
	total  = LONGLONG( utotal.QuadPart ); // will fail, when more than 2^63 Bytes free ....
	unused = LONGLONG( ufree.QuadPart  );
	ASSERT( unused <= total );
	}


void MyGetDiskFreeSpace( _In_z_ const LPCTSTR pszRootPath, _Inout_ LONGLONG& total, _Inout_ LONGLONG& unused, _Inout_ LONGLONG& available ) {
	//ASSERT( pszRootPath != _T( "" ) );
	ULARGE_INTEGER uavailable = { { 0 } };
	ULARGE_INTEGER utotal     = { { 0 } };
	ULARGE_INTEGER ufree      = { { 0 } };
	uavailable.QuadPart       = 0;
	utotal.QuadPart           = 0;
	ufree.QuadPart            = 0;

	// On NT 4.0, the 2nd Parameter to this function must NOT be NULL.
	BOOL b = GetDiskFreeSpaceEx( pszRootPath, &uavailable, &utotal, &ufree );
	if ( !b ) {
		TRACE( _T( "\tGetDiskFreeSpaceEx(%s) failed.\r\n" ), pszRootPath );
		}
	else {
		TRACE( _T("\tGetDiskFreeSpaceEx(%s) successfully returned uavailable: %llu, utotal: %llu, ufree: %llu\r\n"), pszRootPath, uavailable, utotal, ufree);
		ASSERT( uavailable.QuadPart <= utotal.QuadPart);
		ASSERT( ufree.QuadPart <= utotal.QuadPart );
		ASSERT( uavailable.QuadPart != utotal.QuadPart );
		ASSERT( ufree.QuadPart != utotal.QuadPart );
		}
	total     = LONGLONG( utotal.QuadPart ); // will fail, when more than 2^63 Bytes free ....
	unused    = LONGLONG( ufree.QuadPart);
	available = LONGLONG( uavailable.QuadPart );
	ASSERT( unused <= total );
	}

LONGLONG GetTotalDiskSpace( _In_ const CString path ) {
	auto lpcstr_path = ( LPCTSTR ) path;
	ULARGE_INTEGER uavailable = { { 0 } };
	ULARGE_INTEGER utotal     = { { 0 } };
	ULARGE_INTEGER ufree      = { { 0 } };
	uavailable.QuadPart       = 0;
	utotal.QuadPart           = 0;
	ufree.QuadPart            = 0;

	BOOL res = GetDiskFreeSpaceEx( lpcstr_path, &uavailable, &utotal, &ufree );
	if ( res ) {
		return ( LONGLONG ) utotal.QuadPart;
		}
	else {
		return MAXULONGLONG;
		}
	}

/*
		LONGLONG total = 0;
		LONGLONG free  = 0;
		TRACE( _T( "MyGetDiskFreeSpace, path: %s\r\n" ), GetPath( ) );
		MyGetDiskFreeSpace( GetPath( ), total, free );
		LONGLONG before = freeSpaceItem->GetSize( );
		LONGLONG diff  = free - before;

*/

LONGLONG GetFreeDiskSpace( _In_ const CString path ) {
	LONGLONG total = 0;
	LONGLONG free  = 0;
	MyGetDiskFreeSpace( path, total, free );
	return free;
	}


CString GetFolderNameFromPath( _In_z_ const LPCTSTR path ) {
	//ASSERT( path != _T( "" ) );
	CString s = path;
	auto i = s.ReverseFind( _T( '\\' ) );
	if ( i < 0 ) {
		return s;
		}
	return s.Left( i );
	}

CString GetCOMSPEC( ) {
	CString cmd;

	auto dw = GetEnvironmentVariable( _T( "COMSPEC" ), cmd.GetBuffer( _MAX_PATH ), _MAX_PATH );
	cmd.ReleaseBuffer( );

	if (dw == 0) {
		TRACE(_T("COMSPEC not set.\n"));
		cmd = _T( "cmd.exe" );
		}
	return cmd;
	}

void WaitForHandleWithRepainting( _In_ const HANDLE h ) {
	/*
	  Code derived from MSDN sample "Waiting in a Message Loop".
	*/
	ASSERT( h != NULL );
	while ( true ) {
		// Read all of the messages in this next loop, removing each message as we read it.
		MSG msg; 
		while ( PeekMessage( &msg, NULL, WM_PAINT, WM_PAINT, PM_REMOVE ) ) {
			DispatchMessage( &msg );
			}

		// Wait for WM_PAINT message sent or posted to this queue or for one of the passed handles be set to signaled.
		DWORD r = MsgWaitForMultipleObjects( 1, &h, FALSE, INFINITE, QS_PAINT );

		// The result tells us the type of event we have.
		if ( r == WAIT_OBJECT_0 + 1 ) {
			// New messages have arrived. Continue to the top of the always while loop to dispatch them and resume waiting.
			continue;
			}
		else {
			// The handle became signaled. 
			break;
			}
		}
	}

bool FolderExists( _In_z_ const LPCTSTR path ) {
	CFileFind finder;
	//ASSERT( path != _T( "" ) );
	BOOL b = finder.FindFile( path );
	if ( b ) {
		finder.FindNextFile( );

		//IsDirectory returns 0 on failure
		return ( finder.IsDirectory( ) != 0 );
		}
	else {
		// Here we land, if path is an UNC drive. In this case we try another FindFile:
		b = finder.FindFile( CString( path ) + _T( "\\*.*" ) );
		if ( b ) {
			return true;
			}
		return false;
		}
	}

bool DriveExists( _In_ const CString& path ) {
	//ASSERT( path != _T( "" ) );
	if ( path.GetLength( ) != 3 || path[ 1 ] != _T( ':' ) || path[ 2 ] != _T( '\\' ) ) {
		return false;
		}
	auto letter = path.Left( 1 );
	letter.MakeLower( );
	INT d = letter[ 0 ] - _T( 'a' );//????BUGBUG TODO: ?
	
	DWORD mask = 0x1 << d;

	if ( ( mask & GetLogicalDrives( ) ) == 0 ) {
		return false;
		}

	CString dummy;
	if ( !GetVolumeName( path, dummy ) ) {
		return false;
		}

	return true;
	}

CString lGetUserName( ) {
	CString s;
	DWORD size = UNLEN + 1;
	auto ret = GetUserName( s.GetBuffer( INT( size ) ), &size );//TODO: BUGBUG FIXME
	s.ReleaseBuffer( );
	if ( ret == 0 ) {
		TRACE( _T( "GetUserName Failed!!!!\r\n" ) );

		}
	return s;
	}

bool IsHexDigit( _In_ const INT c ) {
	if ( _istdigit( ( short ) c ) ) {
		return true;
		}

	if ( _T( 'a' ) <= c && c <= _T( 'f' ) ) {
		return true;
		}

	if ( _T( 'A' ) <= c && c <= _T( 'F' ) ) {
		return true;
		}

	return false;
	}

CString MyQueryDosDevice( _In_z_ const LPCTSTR drive ) {
	/*
	  drive is a drive spec like C: or C:\ or C:\path (path is ignored).
	  This function returns "", if QueryDosDevice is unsupported or drive doesn't begin with a drive letter, 'Information about MS-DOS device names' otherwise: Something like

	  \Device\Harddisk\Volume1                               for a local drive
	  \Device\LanmanRedirector\;T:0000000011e98\spock\temp   for a network drive 
	  \??\C:\programme                                       for a SUBSTed local path
	  \??\T:\Neuer Ordner                                    for a SUBSTed SUBSTed path
	  \??\UNC\spock\temp                                     for a SUBSTed UNC path

	  As always, I had to experimentally determine these strings, Microsoft didn't think it necessary to document them. (Sometimes I think, they even don't document such things internally...)

	  I hope that a drive is SUBSTed iff this string starts with \??\.

	  assarbad:
		It cannot be safely determined weather a path is or is not SUBSTed on NT via this API. You would have to lookup the volume mount points because SUBST only works per session by definition whereas volume mount points work across sessions (after restarts).
	*/
	CString d = drive;

	if ( d.GetLength( ) < 2 || d[ 1 ] != _T( ':' ) ) {//parenthesis, maybe?
		return _T( "" );
		}

	d = d.Left( 2 );

	//CQueryDosDeviceApi api;

	CString info;
	auto dw = QueryDosDevice( d, info.GetBuffer( 512 ), 512 );//eek
	info.ReleaseBuffer( );

	if ( dw == 0 ) {
		TRACE( _T( "QueryDosDevice(%s) failed: %s\r\n" ), d, MdGetWinerrorText( HRESULT( GetLastError( ) ) ) );
		return _T( "" );
		}

	return info;
	}

bool IsSUBSTedDrive( _In_z_ const LPCTSTR drive ) {
	/*
	  drive is a drive spec like C: or C:\ or C:\path (path is ignored).
	  This function returns true, if QueryDosDevice() is supported and drive is a SUBSTed drive.
	*/
	auto info = MyQueryDosDevice( drive );
	return ( info.GetLength( ) >= 4 && info.Left( 4 ) == "\\??\\" );
	}

//All the zeroInits assume this
static_assert( NULL == 0, "Check the zeroInit functions! Make sure that they're actually initializing to zero!" );

SHELLEXECUTEINFO zeroInitSEI( ) {
	SHELLEXECUTEINFO sei;
	sei.cbSize       = NULL;
	sei.dwHotKey     = NULL;
	sei.fMask        = NULL;
	sei.hIcon        = NULL;
	sei.hInstApp     = NULL;
	sei.hkeyClass    = NULL;
	sei.hMonitor     = NULL;
	sei.hProcess     = NULL;
	sei.hwnd         = NULL;
	sei.lpClass      = NULL;
	sei.lpDirectory  = NULL;
	sei.lpFile       = NULL;
	sei.lpIDList     = NULL;
	sei.lpParameters = NULL;
	sei.lpVerb       = NULL;
	sei.nShow        = NULL;
	return std::move( sei );
	}

WINDOWPLACEMENT zeroInitWINDOWPLACEMENT( ) {
	WINDOWPLACEMENT wp;
	wp.flags                   = NULL;
	wp.ptMaxPosition.x         = NULL;
	wp.ptMaxPosition.y         = NULL;
	wp.ptMinPosition.x         = NULL;
	wp.ptMinPosition.y         = NULL;
	wp.rcNormalPosition.bottom = NULL;
	wp.rcNormalPosition.left   = NULL;
	wp.rcNormalPosition.right  = NULL;
	wp.rcNormalPosition.top    = NULL;
	wp.showCmd                 = NULL;
	wp.length                  = sizeof( wp );

	return std::move( wp );
	}

USN_JOURNAL_DATA zeroInitUSN_JOURNAL_DATA( ) {
	USN_JOURNAL_DATA UpdateSequenceNumber_JournalData;
	UpdateSequenceNumber_JournalData.AllocationDelta = NULL;
	UpdateSequenceNumber_JournalData.FirstUsn        = NULL;
	UpdateSequenceNumber_JournalData.LowestValidUsn  = NULL;
	UpdateSequenceNumber_JournalData.MaximumSize     = NULL;
	UpdateSequenceNumber_JournalData.MaxUsn          = NULL;
	UpdateSequenceNumber_JournalData.NextUsn         = NULL;
	UpdateSequenceNumber_JournalData.UsnJournalID    = NULL;
	return std::move( UpdateSequenceNumber_JournalData );
	}

LVHITTESTINFO zeroInitLVHITTESTINFO( ) {
	LVHITTESTINFO hti;
	hti.flags    = NULL;
	hti.iGroup   = NULL;
	hti.iItem    = NULL;
	hti.iSubItem = NULL;
	hti.pt.x     = NULL;
	hti.pt.y     = NULL;
	return std::move( hti );
	}

HDITEM zeroInitHDITEM( ) {
	HDITEM hditem;

	hditem.cchTextMax = NULL;
	hditem.cxy        = NULL;
	hditem.fmt        = NULL;
	hditem.hbm        = NULL;
	hditem.iImage     = NULL;
	hditem.iOrder     = NULL;
	hditem.lParam     = NULL;
	hditem.mask       = NULL;
	hditem.pszText    = NULL;
	hditem.pvFilter   = NULL;
	hditem.state      = NULL;
	hditem.type       = NULL;
	return std::move( hditem );
	}

LVFINDINFO zeroInitLVFINDINFO( ) {
	LVFINDINFO fi;
	fi.flags       = NULL;
	fi.lParam      = NULL;
	fi.psz         = NULL;
	fi.pt.x        = NULL;
	fi.pt.y        = NULL;
	fi.vkDirection = NULL;
	return std::move( fi );
	}

LVITEM zeroInitLVITEM( ) {
	LVITEM lvitem;
	lvitem.cchTextMax = NULL;
	lvitem.cColumns   = NULL;
	lvitem.iGroup     = NULL;
	lvitem.iGroupId   = NULL;
	lvitem.iImage     = NULL;
	lvitem.iIndent    = NULL;
	lvitem.iItem      = NULL;
	lvitem.iSubItem   = NULL;
	lvitem.lParam     = NULL;
	lvitem.mask       = NULL;
	lvitem.piColFmt   = NULL;
	lvitem.pszText    = NULL;
	lvitem.puColumns  = NULL;
	lvitem.state      = NULL;
	lvitem.stateMask  = NULL;
	return std::move( lvitem );
	}

LVITEM partInitLVITEM( ) {
	LVITEM lvitem;
	lvitem.cchTextMax = NULL;
	lvitem.cColumns   = NULL;
	lvitem.iGroup     = NULL;
	lvitem.iGroupId   = NULL;
	lvitem.iIndent    = NULL;
	lvitem.iSubItem   = NULL;
	lvitem.piColFmt   = NULL;
	lvitem.puColumns  = NULL;
	lvitem.state      = NULL;
	lvitem.stateMask  = NULL;
	return std::move( lvitem );
	}

MFT_ENUM_DATA_V0 zeroInitMFT_ENUM_DATA_V0( ) {
	MFT_ENUM_DATA_V0 data;
	data.HighUsn                  = NULL;
	data.LowUsn                   = NULL;
	data.StartFileReferenceNumber = NULL;
	return std::move( data );
	}

STORAGE_DEVICE_NUMBER zeroInitSTORAGE_DEVICE_NUMBER( ) {
	STORAGE_DEVICE_NUMBER driveNumber;
	driveNumber.DeviceNumber    = NULL;
	driveNumber.DeviceType      = NULL;
	driveNumber.PartitionNumber = NULL;
	return std::move( driveNumber );
	}

PROCESS_MEMORY_COUNTERS zeroInitPROCESS_MEMORY_COUNTERS( ) {
	PROCESS_MEMORY_COUNTERS pmc;
	pmc.cb                         = NULL;
	pmc.PageFaultCount             = NULL;
	pmc.PagefileUsage              = NULL;
	pmc.PeakPagefileUsage          = NULL;
	pmc.PeakWorkingSetSize         = NULL;
	pmc.QuotaNonPagedPoolUsage     = NULL;
	pmc.QuotaPagedPoolUsage        = NULL;
	pmc.QuotaPeakNonPagedPoolUsage = NULL;
	pmc.QuotaPeakPagedPoolUsage    = NULL;
	pmc.WorkingSetSize             = NULL;
	return std::move( pmc );
	}
STARTUPINFO zeroInitSTARTUPINFO( ) {
	STARTUPINFO si;
	si.cb              = NULL;
	si.cbReserved2     = NULL;
	si.dwFillAttribute = NULL;
	si.dwFlags         = NULL;
	si.dwX             = NULL;
	si.dwXCountChars   = NULL;
	si.dwXSize         = NULL;
	si.dwY             = NULL;
	si.dwYCountChars   = NULL;
	si.dwYSize         = NULL;
	si.hStdError       = NULL;
	si.hStdInput       = NULL;
	si.hStdOutput      = NULL;
	si.lpDesktop       = NULL;
	si.lpReserved      = NULL;
	si.lpReserved2     = NULL;
	si.lpTitle         = NULL;
	si.wShowWindow     = NULL;
	return std::move( si );
	}

PROCESS_INFORMATION zeroInitPROCESS_INFORMATION( ) {
	PROCESS_INFORMATION pi;
	pi.dwProcessId = NULL;
	pi.dwThreadId  = NULL;
	pi.hProcess    = NULL;
	pi.hThread     = NULL;
	return std::move( pi );
	}

NMLISTVIEW zeroInitNMLISTVIEW( ) {
	NMLISTVIEW listView;
	listView.hdr.code     = NULL;
	listView.hdr.hwndFrom = NULL;
	listView.hdr.idFrom   = NULL;
	listView.iItem        = NULL;
	listView.iSubItem     = NULL;
	listView.lParam       = NULL;
	listView.ptAction.x   = NULL;
	listView.ptAction.y   = NULL;
	listView.uChanged     = NULL;
	listView.uNewState    = NULL;
	listView.uOldState    = NULL;
	return std::move( listView );
	}

BROWSEINFO zeroInitBROWSEINFO( ) {
	BROWSEINFO bi;
	bi.hwndOwner      = NULL;
	bi.iImage         = NULL;
	bi.lParam         = NULL;
	bi.lpfn           = NULL;
	bi.lpszTitle      = NULL;
	bi.pidlRoot       = NULL;
	bi.pszDisplayName = NULL;
	bi.ulFlags        = NULL;
	return std::move( bi );
	}

SHFILEOPSTRUCT zeroInitSHFILEOPSTRUCT( ) {
	SHFILEOPSTRUCT sfos;
	sfos.fAnyOperationsAborted = NULL;
	sfos.fFlags                = NULL;
	sfos.hNameMappings         = NULL;
	sfos.hwnd                  = NULL;
	sfos.lpszProgressTitle     = NULL;
	sfos.pFrom                 = NULL;
	sfos.pTo                   = NULL;
	sfos.wFunc                 = NULL;
	return std::move( sfos );
	}

SExtensionRecord zeroInitSExtensionRecord( ) {
	SExtensionRecord ser;
	ser.bytes = NULL;
	ser.color = NULL;
	ser.files = NULL;
	return std::move( ser );
	}

void displayWindowsMsgBoxWithError( ) {
	LPVOID lpMsgBuf = NULL;
	auto err = GetLastError( );
	static_assert( sizeof( DWORD ) == sizeof( unsigned long ), "" );
	TRACE( _T( "Error number: %l0lu\t\n" ), err );///bugbug TODO: FIXME format string!
	MessageBox( NULL, TEXT( "Whoa! Err!" ), LPCWSTR( err ), MB_OK );
	auto ret = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, err, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), ( LPTSTR ) &lpMsgBuf, 0, NULL );
	//LPCTSTR msg = ( LPCTSTR ) lpMsgBuf;
	if ( ret > 0 ) {
		MessageBox( NULL, LPCTSTR( lpMsgBuf ), TEXT( "Error" ), MB_OK );
		TRACE( _T( "Error: %s\r\n" ), lpMsgBuf );
		}
	}

#ifdef GETSPEC_STATIC
CString GetSpec_Bytes( ) {
	static CString s;
	CacheString( s, IDS_SPEC_BYTES, _T( "Bytes" ) );
	return s;
	}

CString GetSpec_KB( ) {
	static CString s;
	CacheString( s, IDS_SPEC_KB, _T( "KB" ) );
	return s;
	}

CString GetSpec_MB( ) {
	static CString s;
	CacheString( s, IDS_SPEC_MB, _T( "MB" ) );
	return s;
	}

CString GetSpec_GB( ) {
	static CString s;
	CacheString( s, IDS_SPEC_GB, _T( "GB" ) );
	return s;
	}

CString GetSpec_TB( ) {
	static CString s;
	CacheString( s, IDS_SPEC_TB, _T( "TB" ) );
	return s;
	}
#endif

void check8Dot3NameCreationAndNotifyUser( ) {
	HKEY keyHandle = NULL;

	auto res = RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T( "SYSTEM\\CurrentControlSet\\Control\\FileSystem" ), NULL, KEY_READ, &keyHandle );

	if ( res != ERROR_SUCCESS ) {
		TRACE( _T( "key not found!\r\n" ) );
		return;
		}

	else {
		}

	DWORD valueType = 0;
	//std::unique_ptr<char[ ]> databuffer = std::make_unique<char[]>(4);//I wish...
	static_assert( sizeof( BYTE ) == 1, "bad BYTE size!" );
	BYTE data[ 4 ];
	static_assert( sizeof( data ) == sizeof( REG_DWORD ), "bad size!" );
			
	DWORD bufferSize = sizeof( data );
			
	res = RegQueryValueEx( keyHandle, _T( "NtfsDisable8dot3NameCreation" ), NULL, &valueType, &data[0], &bufferSize );

	if ( res != ERROR_SUCCESS ) {
		if ( res == ERROR_MORE_DATA ) {
			return;
			}
		else if ( res == ERROR_FILE_NOT_FOUND) {
			return;
			}
		else {
			return;
			}
		}
	DWORD value = data[ 0 ];
	/*
		0 = NTFS creates short file names. This setting enables applications that cannot process long file names and computers that use differentcode pages to find the files.
		1 = NTFS does not create short file names. Although this setting increases file performance, applications that cannot process long file names, and computers that use different code pages, might not be able to find the files.
		2 = NTFS sets the 8.3 naming convention creation on a per volume basis.
		3 = NTFS disables 8dot3 name creation on all volumes except the system volume.
	*/
	if ( value == 0 ) {
		MessageBox( NULL, _T( "Your computer is set to create short (8.3 style) names for files on all NTFS volumes. This can TREMENDOUSLY slow directory operations - As a result, the amount of time required to perform a directory listing increases with the square of the number of files in the directory! For more, see Microsoft KnowledgeBase article ID: 130694" ), _T( "Performance warning!"), MB_ICONWARNING );
		}

	if ( value == 2 ) {
		MessageBox( NULL, _T( "Your computer is set to create short (8.3 style) names for files on NTFS volumes, on a per-volume-setting basis. Shore file name creation can TREMENDOUSLY slow directory operations - As a result, the amount of time required to perform a directory listing increases with the square of the number of files in the directory! For more, see Microsoft KnowledgeBase article ID: 130694" ), _T( "Performance warning!"), MB_ICONWARNING );
		}

	if ( value == 3 ) {
		MessageBox( NULL, _T( "Your computer is set to create short (8.3 style) names for files on the system volume. If you're running WinDirStat against any other volume you can safely ignore this warning. Short file name creation can TREMENDOUSLY slow directory operations - As a result, the amount of time required to perform a directory listing increases with the square of the number of files in the directory! For more, see Microsoft KnowledgeBase article ID: 130694" ), _T( "Performance warning!"), MB_ICONWARNING );
		}
	}
void zeroDate( _Inout_ FILETIME& in ) {
	in.dwHighDateTime = 0;
	in.dwLowDateTime  = 0;
	}

// $Log$
// Revision 1.20  2004/11/28 14:40:06  assarbad
// - Extended CFileFindWDS to replace a global function
// - Now packing/unpacking the file attributes. This even spares a call to find encrypted/compressed files.
//
// Revision 1.19  2004/11/25 21:13:38  assarbad
// - Implemented "attributes" column in the treelist
// - Adopted width in German dialog
// - Provided German, Russian and English version of IDS_TREECOL_ATTRIBUTES
//
// Revision 1.18  2004/11/25 11:58:52  assarbad
// - Minor fixes (odd behavior of coloring in ANSI version, caching of the GetCompressedFileSize API)
//   for details see the changelog.txt
//
// Revision 1.17  2004/11/14 21:50:44  assarbad
// - Pre-select the last used folder
//
// Revision 1.16  2004/11/14 08:49:06  bseifert
// Date/Time/Number formatting now uses User-Locale. New option to force old behavior.
//
// Revision 1.15  2004/11/12 22:14:16  bseifert
// Eliminated CLR_NONE. Minor corrections.
//
// Revision 1.14  2004/11/12 13:19:44  assarbad
// - Minor changes and additions (in preparation for the solution of the "Browse for Folder" problem)
//
// Revision 1.13  2004/11/12 00:47:42  assarbad
// - Fixed the code for coloring of compressed/encrypted items. Now the coloring spans the full row!
//
// Revision 1.12  2004/11/07 21:10:25  assarbad
// - Corrected IF statement
// - Changed solution file to build consistent "Unicode Debug" configuration for "Unicode Debug" of main project
//
// Revision 1.11  2004/11/07 20:14:30  assarbad
// - Added wrapper for GetCompressedFileSize() so that by default the compressed file size will be shown.
//
// Revision 1.10  2004/11/05 16:53:07  assarbad
// Added Date and History tag where appropriate.
//
