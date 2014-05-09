/**
  src/content_pipe_file.c

  This file implements the support for content pipes related to local files.

  Copyright (C) 2007-2009 STMicroelectronics
  Copyright (C) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).

  This library is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the Free
  Software Foundation; either version 2.1 of the License, or (at your option)
  any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public License
  along with this library; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St, Fifth Floor, Boston, MA
  02110-1301  USA

*/

#include "content_pipe_file.h"

/** Create a content source and open it for writing. */
static CPresult Create( CPhandle *hContent, CPstring szURI )
{
  file_ContentPipe* pFilePipe = (file_ContentPipe*) hContent;
  char* pathname = szURI + 7;
  CPresult err = 0;
  int flags, ret;

  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_file:%s \n", __func__);

  if(0 != strncmp(szURI, "file://", 7))
    err = KD_EINVAL;

  flags = O_WRONLY | O_CREAT;

  if(0 == err) {
    ret = open(pathname, flags, 0);
    if(-1 != ret) {
      pFilePipe->fd = ret;
    } else {
      err = KD_EIO;  /* ??? */
    }
  }

  return err;
}

/** Open a content stream for reading or writing. */
static CPresult Open( CPhandle* hContent, CPstring szURI, CP_ACCESSTYPE eAccess )
{
  file_ContentPipe* pFilePipe = (file_ContentPipe*) hContent;
  CPresult err = 0;
  int flags, ret;

  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_file:%s \n", __func__);

  if(0 != strncmp(szURI, "file://", 7))
    err = KD_EINVAL;

  if(0 == err)
    switch(eAccess) {
    case CP_AccessRead:
      flags = O_RDONLY;
      break;
    case CP_AccessWrite:
      flags = O_WRONLY;
      break;
    case CP_AccessReadWrite:
      flags = O_RDWR;
      break;
    default:
      err = KD_EINVAL;  /* ??? */
      break;
    }

  if(0 == err) {
    char* pathname = szURI + 7;
    ret = open(pathname, flags);
    if(-1 != ret) {
      pFilePipe->fd = ret;
    } else {
      /* Map errno */
      err = KD_EIO;  /* ??? */
    }
  }

  return err;
}

/** Close a content stream. */
static CPresult Close( CPhandle hContent )
{
  file_ContentPipe* pFilePipe = (file_ContentPipe*) hContent;
  CPresult err = 0;
  int ret;

  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_file:%s \n", __func__);

  ret = close(pFilePipe->fd);
  if(ret == -1) {
    /* Map errno */
    err = KD_EIO;  /* ??? */
  }

  return err;
}

/** Check the that specified number of bytes are available for reading or writing (depending on access type).*/
static CPresult CheckAvailableBytes( CPhandle hContent, CPuint nBytesRequested, CP_CHECKBYTESRESULTTYPE *eResult )
{
  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_file:%s \n", __func__);

  return KD_EBADF;
}

/** Seek to certain position in the content relative to the specified origin. */
static CPresult SetPosition( CPhandle  hContent, CPint nOffset, CP_ORIGINTYPE eOrigin)
{
  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_file:%s \n", __func__);

  return KD_EBADF;
}

/** Retrieve the current position relative to the start of the content. */
static CPresult GetPosition( CPhandle hContent, CPuint *pPosition)
{
  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_file:%s \n", __func__);

  return KD_EBADF;
}

/** Retrieve data of the specified size from the content stream (advance content pointer by size of data).
    Note: pipe client provides pointer. This function is appropriate for small high frequency reads. */
static CPresult Read( CPhandle hContent, CPbyte *pData, CPuint nSize)
{
  file_ContentPipe* pFilePipe = (file_ContentPipe*) hContent;
  CPresult err = 0;
  ssize_t count;

  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_file:%s \n", __func__);

  count = read(pFilePipe->fd, (void*) pData, (size_t) nSize);

  if(count < nSize) {
    err = KD_EIO;  /* ??? */
  } else if(count == -1) {
    err = KD_EIO;  /* ??? */
  }

  return err;
}

/** Retrieve a buffer allocated by the pipe that contains the requested number of bytes.
    Buffer contains the next block of bytes, as specified by nSize, of the content. nSize also
    returns the size of the block actually read. Content pointer advances the by the returned size.
    Note: pipe provides pointer. This function is appropriate for large reads. The client must call
    ReleaseReadBuffer when done with buffer.

    In some cases the requested block may not reside in contiguous memory within the
    pipe implementation. For instance if the pipe leverages a circular buffer then the requested
    block may straddle the boundary of the circular buffer. By default a pipe implementation
    performs a copy in this case to provide the block to the pipe client in one contiguous buffer.
    If, however, the client sets bForbidCopy, then the pipe returns only those bytes preceding the memory
    boundary. Here the client may retrieve the data in segments over successive calls. */
static CPresult ReadBuffer( CPhandle hContent, CPbyte **ppBuffer, CPuint *nSize, CPbool bForbidCopy)
{
  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_file:%s \n", __func__);

  return KD_EBADF;
}

/** Release a buffer obtained by ReadBuffer back to the pipe. */
static CPresult ReleaseReadBuffer(CPhandle hContent, CPbyte *pBuffer)
{
  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_file:%s \n", __func__);

  return KD_EBADF;
}

/** Write data of the specified size to the content (advance content pointer by size of data).
    Note: pipe client provides pointer. This function is appropriate for small high frequency writes. */
static CPresult Write( CPhandle hContent, CPbyte *pData, CPuint nSize)
{
  file_ContentPipe* pFilePipe = (file_ContentPipe*) hContent;
  CPresult err = 0;
  ssize_t ret;

  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_file:%s \n", __func__);

  ret = write(pFilePipe->fd, (void*) pData, (size_t) nSize);

  if(ret < nSize) {
    err = KD_EIO;  /* ??? */
  } else if(ret == -1) {
    err = KD_EIO;  /* ??? */
  }

  return err;
}

/** Retrieve a buffer allocated by the pipe used to write data to the content.
    Client will fill buffer with output data. Note: pipe provides pointer. This function is appropriate
    for large writes. The client must call WriteBuffer when done it has filled the buffer with data.*/
static CPresult GetWriteBuffer( CPhandle hContent, CPbyte **ppBuffer, CPuint nSize)
{
  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_file:%s \n", __func__);

  return KD_EBADF;
}

/** Deliver a buffer obtained via GetWriteBuffer to the pipe. Pipe will write the
    the contents of the buffer to content and advance content pointer by the size of the buffer */
static CPresult WriteBuffer( CPhandle hContent, CPbyte *pBuffer, CPuint nFilledSize)
{
  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_file:%s \n", __func__);

  return KD_EBADF;
}

/** Register a per-handle client callback with the content pipe. */
static CPresult RegisterCallback( CPhandle hContent, CPresult (*ClientCallback)(CP_EVENTTYPE eEvent, CPuint iParam))
{
  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_file:%s \n", __func__);

  return KD_EBADF;
}

CPresult file_pipe_Constructor(CP_PIPETYPE **ppPipe, CPstring szURI)
{
  file_ContentPipe* pFilePipe;

  pFilePipe = (file_ContentPipe*) calloc(1, sizeof(file_ContentPipe));

  if(NULL != pFilePipe) {

    pFilePipe->pipe.Open = Open;
    pFilePipe->pipe.Close = Close;
    pFilePipe->pipe.Create = Create;
    pFilePipe->pipe.CheckAvailableBytes = CheckAvailableBytes;
    pFilePipe->pipe.SetPosition = SetPosition;
    pFilePipe->pipe.GetPosition = GetPosition;
    pFilePipe->pipe.Read = Read;
    pFilePipe->pipe.ReadBuffer = ReadBuffer;
    pFilePipe->pipe.ReleaseReadBuffer = ReleaseReadBuffer;
    pFilePipe->pipe.Write = Write;
    pFilePipe->pipe.GetWriteBuffer = GetWriteBuffer;
    pFilePipe->pipe.WriteBuffer = WriteBuffer;
    pFilePipe->pipe.RegisterCallback = RegisterCallback;

    pFilePipe->fd = -1; /* An invalid value */

    *ppPipe = (CP_PIPETYPE*) pFilePipe;
  }

  return 0;
}

