// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently
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

#pragma once


#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
//#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
//#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
//#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0600	// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS			// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0600	// Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0800	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

//From helpmap.h
#define IDH_Treemap 1003


//Things that I will eventually get rid of/add to program, but can't safely do so as of yet.
#define CHILDVEC
//#define DRAW_PACMAN
//#define DRAW_ICONS
//#define ITEM_DRAW_SUBITEM
//#define GETSPEC_STATIC
#define C_STYLE_STRINGS
//#define SUSPEND_BUTTON

//Debugging defs
//#define DUMP_MEMUSAGE
//#define GRAPH_LAYOUT_DEBUG
//#define MEMUSAGE_THREAD
//#define EXTENSION_LIST_DEBUG

//this gets size of CArray AND vector
#ifdef CHILDVEC
#define polySize size
#else
#define polySize GetSize
#endif


//this `Add`s a value for CArray AND vector
#ifdef CHILDVEC
#define polyAdd push_back
#else
#define polyAdd Add
#endif


//this returns the `emptyness` of a CArray AND a vector
#ifdef CHILDVEC
#define polyEmpty empty
#else
#define polyEmpty IsEmpty
#endif


#ifdef CHILDVEC
#define polyAt( i ) .at( i )
#else
#define polyAt( i ) [ i ]
#endif

#ifdef CHILDVEC
#define polyClear( ) clear( )
#else
#define polyClear( ) SetSize( 0 )
#endif



#pragma warning(disable:4061) //enumerator 'identifier' in switch of enum 'enumeration' is not explicitly handled by a case label. The enumerate has no associated handler in a switch statement.
#pragma warning(disable:4062) //The enumerate has no associated handler in a switch statement, and there is no default label.
#pragma warning(disable:4191) //'operator/operation' : unsafe conversion from 'type of expression' to 'type required'
#pragma warning(disable:4265) //'class' : class has virtual functions, but destructor is not virtual
#pragma warning(disable:4350) //An rvalue cannot be bound to a non-const reference. In previous versions of Visual C++, it was possible to bind an rvalue to a non-const reference in a direct initialization. This code now gives a warning.


#ifndef DUMP_MEMUSAGE
#pragma warning(disable:4820) //'bytes' bytes padding added after construct 'member_name'. The type and order of elements caused the compiler to add padding to the end of a struct
#endif

#pragma warning(disable:4917) //'declarator' : a GUID can only be associated with a class, interface or namespace. A user-defined structure other than class, interface, or namespace cannot have a GUID.
#pragma warning(disable:4987) //nonstandard extension used: 'throw (...)'

//noisy
#pragma warning(disable:4548) //expression before comma has no effect; expected expression with side-effect
#pragma warning(disable:4625) //A copy constructor was not accessible in a base class, therefore not generated for a derived class. Any attempt to copy an object of this type will cause a compiler error. //ANYTHING that inherits from CWND will warn!
#pragma warning(disable:4626) //An assignment operator was not accessible in a base class and was therefore not generated for a derived class. Any attempt to assign objects of this type will cause a compiler error.

#pragma warning(disable:4264) //'virtual_function' : no override available for virtual member function from base 'class'; function is hidden
//#pragma warning(disable:4263) //A class function definition has the same name as a virtual function in a base class but not the same number or type of arguments. This effectively hides the virtual function in the base class.
//#pragma warning(disable:4189) //A variable is declared and initialized but not used.
//#pragma warning(disable:4755) //Conversion rules for arithmetic operations in the comparison mean that one branch cannot be executed in an inlined function. Cast '(nBaseTypeCharLen + ...)' to 'ULONG64' (or similar type of 8 bytes).
//#pragma warning(disable:4280) //'operator �>' was self recursive through type 'type'. Your code incorrectly allows operator�> to call itself.
//#pragma warning(disable:4127) //The controlling expression of an if statement or while loop evaluates to a constant.
//#pragma warning(disable:4365) //'action' : conversion from 'type_1' to 'type_2', signed/unsigned mismatch

#pragma warning(disable:4514) //'function' : unreferenced inline function has been removed
#pragma warning(disable:4710) //The given function was selected for inline expansion, but the compiler did not perform the inlining.
#pragma warning(disable:4711) //function 'function' selected for inline expansion. The compiler performed inlining on the given function, although it was not marked for inlining.



#ifndef _DEBUG
#pragma warning(disable:4555) //expression has no effect; expected expression with side-effect //Happens alot with AfxCheckMemory in debug builds.
#endif

#pragma warning(push, 1)

#include <afxwin.h>         // MFC Core //MUST BE INCLUDED FIRST!!!!!!!!!!!!!

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <future>
#include <map>
#include <numeric>
#include <cstdint>


#include <afxext.h>         // MFC Extensions
#include <afxdtctl.h>		// MFC IE 4
#include <afxcmn.h>			// MFC Common Controls
#include <afxtempl.h>		// MFC Container classes
#include <afxmt.h>			// MFC Multithreading
#include <atlbase.h>		// USES_CONVERSION, ComPtr<>


#include <math.h>			// floor(), fmod(), sqrt() etc.
#include <psapi.h>			// PROCESS_MEMORY_INFO

#include <lmcons.h>			// UNLEN
#include <float.h>			// DBL_MAX

#include <VersionHelpers.h>

#include <winioctl.h>
#include <strsafe.h>

// Headers placed in the common directory (used by windirstat and by setup)
#include "../common/mdexceptions.h"
#include "../common/cotaskmem.h"
#include "../common/commonhelpers.h"
//#include "../common/platform.h"
#include "../common/version.h"

// General purpose headers
#include "selectobject.h"
#include "set.h"





#ifndef _INC_STDARG
#include <stdarg.h>
#endif

#pragma warning(pop)


#define countof(arr) (sizeof(arr)/sizeof((arr)[0]))
#define pi2 1.5707963267948966192
#define RAM_USAGE_UPDATE_INTERVAL 1000

//helper functions
template<class T>
INT signum(T x) {
	if ( x < 0 ) {
		return -1;
		}
	else {
		if ( x == 0 ) {
			return 0;
			}
		else {
			return 1;
			}
		}
	//return ( x < 0 ) ? -1 : ( x == 0 ) ? 0 : 1;
	}


//template<typename T, typename ITEM>
//inline size_t findInVec( _In_ const T& vec, _In_ const ITEM& item ) {
//	const auto sizeOfVector = vec.size( );
//	for ( T::size_type i = 0; i < sizeOfVector; ++i ) {
//		if ( vec[ i ].ext == item ) {
//			return i;
//			}
//		}
//	return sizeOfVector;
//	}


//some generic structures!

struct SRECT {
	/*
	  short-based RECT, saves 8 bytes compared to tagRECT
	*/
	SRECT( ) : left( 0 ), top( 0 ), right( 0 ), bottom( 0 ) { }
	SRECT( std::int16_t iLeft, std::int16_t iTop, std::int16_t iRight, std::int16_t iBottom ) : left { iLeft }, top { iTop }, right { iRight }, bottom { iBottom } { }
	SRECT( const SRECT& in ) {
		left   = in.left;
		top    = in.top;
		right  = in.right;
		bottom = in.bottom;
		}
	SRECT( const CRect& in ) {
		left   = std::int16_t( in.right );
		top    = std::int16_t( in.top );
		right  = std::int16_t( in.right );
		bottom = std::int16_t( in.bottom );
		}
	static CRect BuildCRect( const SRECT& in ) {
		//ASSERT( ( in.left != -1 ) && ( in.top != -1 ) && ( in.right != -1 ) && ( in.bottom != -1 ) );
		ASSERT( ( in.right + 1 ) >= in.left );
		ASSERT( in.bottom >= in.top );
		CRect out;
		out.left   = LONG( in.left );
		out.top    = LONG( in.top );
		out.right  = LONG( in.right );
		out.bottom = LONG( in.bottom );
		ASSERT( out.left   == in.left );
		ASSERT( out.top    == in.top );
		ASSERT( out.right  == in.right );
		ASSERT( out.bottom == in.bottom );
		out.NormalizeRect( );
		ASSERT( out.right >= out.left );
		ASSERT( out.bottom >= out.top );
		return std::move( out );
		}
	std::int16_t left;
	std::int16_t top;
	std::int16_t right;
	std::int16_t bottom;
	};

// Item types
enum ITEMTYPE : std::uint8_t {
	IT_MYCOMPUTER,		// Pseudo Container "My Computer"
	IT_DRIVE,			// C:\, D:\ etc.
	IT_DIRECTORY,		// Folder
	IT_FILE,			// Regular file
	IT_FILESFOLDER,		// Pseudo Folder "<Files>"
	IT_FREESPACE,		// Pseudo File "<Free Space>"
	IT_UNKNOWN,			// Pseudo File "<Unknown>"

	//ITF_FLAGS	 = 0xF000,
	//ITF_ROOTITEM = 0x40	// This is an additional flag, not a type.
	};

// Whether an item type is a leaf type
inline bool IsLeaf( const ITEMTYPE t ) {
#ifdef DEBUG
	auto val = ( t == IT_FILE ) || ( t == IT_FREESPACE ) || ( t == IT_UNKNOWN );
	return val;
#else
	return ( t == IT_FILE ) || ( t == IT_FREESPACE ) || ( t == IT_UNKNOWN );
#endif
	}


#pragma pack(push, 1)
#pragma message( "Whoa there! I'm changing the natural data alignment for SExtensionRecord. Look for a message that says I'm restoring it!" )
struct SExtensionRecord {
	SExtensionRecord( ) : files( 0 ), color( COLORREF( 0 ) ), bytes( 0 ) { }
	SExtensionRecord( _In_ std::uint32_t files_in, _In_ COLORREF color_in, _In_ std::uint64_t bytes_in, _In_ CString ext_in ) : files( files_in ), color( color_in ), bytes( bytes_in ), ext( ext_in ) { }
	/*
	  COMPARED BY BYTES!
	  Data stored for each extension.
	  4,294,967,295  (4294967295 ) is the maximum number of files in an NTFS filesystem according to http://technet.microsoft.com/en-us/library/cc781134(v=ws.10).aspx
	  18446744073709551615 is the maximum theoretical size of an NTFS file according to http://blogs.msdn.com/b/oldnewthing/archive/2007/12/04/6648243.aspx
	*/

	CString ext;
	_Field_range_(0, 4294967295 ) std::uint32_t files;//save 4 bytes :)
	_Field_range_(0, 18446744073709551615) std::uint64_t bytes;
	COLORREF color;

	//static bool compareSExtensionRecordByBytes( const SExtensionRecord& lhs, const SExtensionRecord& rhs ) { return ( lhs.bytes < rhs.bytes ); }
	//bool compareSExtensionRecordByNumberFiles ( const SExtensionRecord& lhs, const SExtensionRecord& rhs ) { return ( lhs.files < rhs.files ); }

	bool compareSExtensionRecordByExtensionAlpha( const SExtensionRecord& lhs, const SExtensionRecord& rhs ) { return ( lhs.ext.Compare( rhs.ext ) < 0 ); }

	};
#pragma message( "Restoring data alignment.... " )
#pragma pack(pop)



struct s_compareSExtensionRecordByBytes {
	public:
	bool operator()( const SExtensionRecord& lhs, const SExtensionRecord& rhs ) { return ( lhs.bytes < rhs.bytes ); }
	};

struct s_compareSExtensionRecordByNumberFiles {
	public:
	bool operator()( const SExtensionRecord& lhs, const SExtensionRecord& rhs ) { return ( lhs.files < rhs.files ); }
	};

static_assert( sizeof( short ) == sizeof( std::int16_t ), "y'all ought to check SRECT" );

const std::vector<COLORREF> defaultColorVec = { RGB( 0, 0, 255 ), RGB( 255, 0, 0 ), RGB( 0, 255, 0 ), RGB( 0, 255, 255 ), RGB( 255, 0, 255 ), RGB( 255, 255, 0 ), RGB( 150, 150, 255 ), RGB( 255, 150, 150 ), RGB( 150, 255, 150 ), RGB( 150, 255, 255 ), RGB( 255, 150, 255 ), RGB( 255, 255, 150 ), RGB( 255, 255, 255 ) };



//WDS headers
#include "mountpoints.h"
#include "osspecific.h"
#include "myimagelist.h"
#include "mainframe.h"
#include "sortinglistcontrol.h"
#include "ownerdrawnlistcontrol.h"
#include "layout.h"
#include "treemap.h"
#include "options.h"
#include "windirstat.h"
#include "Treelistcontrol.h"
#include "dirstatdoc.h"
#include "FileFindWDS.h"
#include "item.h"
#include "graphview.h"
// $Log$
// Revision 1.10  2004/11/12 22:14:16  bseifert
// Eliminated CLR_NONE. Minor corrections.
//
// Revision 1.9  2004/11/12 13:19:44  assarbad
// - Minor changes and additions (in preparation for the solution of the "Browse for Folder" problem)
//
// Revision 1.8  2004/11/05 16:53:08  assarbad
// Added Date and History tag where appropriate.
//
