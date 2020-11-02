#include "StdAfx.h"
#include "archivefile.h"
#include "zlib.h"

//////////////////////////////////////////////////////////////////////////

ArchiveFile::ArchiveFile()
:	m_tFileMode( TArchive_UnknownMode )
{
}

ArchiveFile::~ArchiveFile()
{
}

bool ArchiveFile::Init( const TCHAR* szFileName, TArchiveFileMode mode, int nArchiveLevel /* = 0  */ )
{
	_ASSERT( m_tFileMode == TArchive_UnknownMode );
	_ASSERT( mode != TArchive_UnknownMode );
	m_sArchiveFileName = szFileName;
	m_tFileMode = mode;
	m_nArchiveLevel = nArchiveLevel;

	switch( m_tFileMode )
	{
	case TArchive_OpenForDeflate:
		m_pArchiveFile = _tfopen( szFileName, _T("wb") );
		break;
	case TArchive_OpenForInflate:
		m_pArchiveFile = _tfopen( szFileName, _T("rb") );
		break;
	}

	if ( !m_pArchiveFile )
	{
		_ASSERT( m_pArchiveFile );
		m_tFileMode = TArchive_UnknownMode;
	}

	return true;
}

void ArchiveFile::Release()
{
	if ( m_pArchiveFile )
		fclose( m_pArchiveFile );

	m_tFileMode = TArchive_UnknownMode;
}

int ArchiveFile::Inflate( const char* pData, size_t nDataSize, const TArchiveChunk& ch )
{
	_ASSERT( m_tFileMode == TArchive_OpenForInflate );
	_ASSERT( pData && nDataSize == ch.nSizeBeforArchive );

	fseek( m_pArchiveFile, ( long )ch.nOffset, SEEK_SET );
	size_t nArchiveSize = ch.nSizeAfterArchive;

	z_stream stream;
	stream.zalloc	= Z_NULL;
	stream.zfree	= Z_NULL;
	stream.opaque	= Z_NULL;
	stream.avail_in	= 0;
	stream.next_in	= Z_NULL;
	int	nRes		= Z_OK;

	nRes = inflateInit(&stream);
	if( nRes != Z_OK )
		return nRes;

	do
	{
		stream.avail_in = (uInt)fread( m_pArchiveBuffer, 1, ( nArchiveSize > TArchive_BufferSize ) ? TArchive_BufferSize : nArchiveSize, m_pArchiveFile );
		
		if ( ferror( m_pArchiveFile ))
		{
			inflateEnd( &stream );
			return Z_ERRNO;
		}

		if ( !stream.avail_in )
			break;

		stream.next_in = m_pArchiveBuffer;

		do
		{
			stream.avail_out	= (uInt)nDataSize;
			stream.next_out		= (Bytef*)pData;
			nRes = inflate( &stream, Z_NO_FLUSH );

			switch (nRes)
			{
			case Z_NEED_DICT:
				nRes = Z_DATA_ERROR;
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				inflateEnd(&stream);
				return nRes;
			}

			unsigned int nActualSize = ( unsigned int) nDataSize - stream.avail_out;
			_ASSERT( nActualSize >= 0 );
			pData		+= nActualSize;
			nDataSize	-= nActualSize;			
		}
		while( stream.avail_out == 0 && nDataSize );

		nArchiveSize = ( nArchiveSize >= TArchive_BufferSize ) ? nArchiveSize - TArchive_BufferSize : 0;		
	}
	while( nArchiveSize > 0 );

	return Z_OK;
}

int ArchiveFile::Deflate( const char* pData, size_t nDataSize, TArchiveChunk& ch )
{
	_ASSERT( m_tFileMode == TArchive_OpenForDeflate );
	_ASSERT( pData && nDataSize );
	memset( &ch, 0, sizeof( TArchiveChunk ) );

	ch.nSizeBeforArchive	= nDataSize;
	ch.nOffset				= ftell( m_pArchiveFile );

	z_stream stream;
	stream.zalloc	= Z_NULL;
	stream.zfree	= Z_NULL;
	stream.opaque	= Z_NULL;
	int	nRes		= Z_OK;
	int nFlush		= Z_NO_FLUSH;

	nRes = deflateInit( &stream, m_nArchiveLevel );
	if ( nRes != Z_OK )
		return nRes;	
	
	do
	{
		stream.avail_in = (uInt)(( nDataSize > TArchive_BufferSize ) ? TArchive_BufferSize : nDataSize );
		nFlush = ( nDataSize <= TArchive_BufferSize ) ? Z_FINISH : Z_NO_FLUSH;
		stream.next_in = (Bytef*)pData;

		do 
		{
			stream.avail_out	= TArchive_BufferSize;
			stream.next_out		= m_pArchiveBuffer;
			nRes = deflate( &stream, nFlush );
			unsigned int nActualSize = TArchive_BufferSize - stream.avail_out;
			ch.nSizeAfterArchive += nActualSize;

			if ( fwrite( m_pArchiveBuffer, 1, nActualSize, m_pArchiveFile ) != nActualSize || ferror( m_pArchiveFile ) )
			{
				deflateEnd( &stream );
				return Z_ERRNO;
			}
		}
		while(stream.avail_out == 0 );


		
		if ( nDataSize > TArchive_BufferSize )		
		{
			pData		+= TArchive_BufferSize;
			nDataSize	-= TArchive_BufferSize;
		}
		else
			nDataSize = 0;
		
	}
	while( nFlush != Z_FINISH );

	deflateEnd(&stream);
	return Z_OK;
}