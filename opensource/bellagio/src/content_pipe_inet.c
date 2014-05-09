/**
  src/content_pipe_inet.c

  This file implements the support for content pipes related to inet resources.

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

#include "content_pipe_inet.h"

/*
 * Create a socket
*/

static CPresult Create( CPhandle *hContent, CPstring szURI )
{
  inet_ContentPipe* pPipe = (inet_ContentPipe*) hContent;
  CPresult err = 0;
  int nHostPort;
  struct sockaddr_in sAddress;  /* Internet socket address stuct */
  int nAddressSize=sizeof(struct sockaddr_in);

  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_inet:%s \n", __func__);

  if(1 != sscanf(szURI, "inet://%d", &nHostPort))
    err = KD_EINVAL;

  pPipe->sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(SOCKET_ERROR == pPipe->sfd) {
    printf("\nCould not make a socket\n");
    err = KD_EIO;
  }

  if(0 == err) {

    /* fill address struct */
    sAddress.sin_addr.s_addr = INADDR_ANY;
    sAddress.sin_port = htons(nHostPort);
    sAddress.sin_family = AF_INET;

    /* bind to a port */
    if(SOCKET_ERROR == bind(pPipe->sfd ,(struct sockaddr*) &sAddress, sizeof(sAddress))) {
      printf("\nCould not connect to host\n");
      err = KD_EIO;
    }

  }

  if(0 == err) /* establish listen queue */
    if(listen(pPipe->sfd, QUEUE_SIZE) == SOCKET_ERROR) {
      printf("\nCould not listen\n");
      err = KD_EIO;
    }

  if(0 == err) /* get the connected socket */
    pPipe->cfd = accept(pPipe->sfd, (struct sockaddr*) &sAddress, (socklen_t*) &nAddressSize);

  return err;
}

/** Open socket for reading or writing.
 ** e.g. inet://faex05:2102
*/
static CPresult Open( CPhandle* hContent, CPstring szURI, CP_ACCESSTYPE eAccess )
{
  inet_ContentPipe* pPipe = (inet_ContentPipe*) hContent;
  CPresult err = 0;
  char strHostName[80];
  int nHostPort = 0;

  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_inet:%s \n", __func__);

  {
    char *pc = strrchr(szURI, ':');
    if(pc != NULL) {
      strncpy(strHostName, szURI+7, (long) pc - (long) szURI - 7);
      strHostName[(long) pc - (long) szURI - 7] = '\0';
      nHostPort = atoi(++pc);
    }
  }

  /* make a socket */
  pPipe->cfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(SOCKET_ERROR == pPipe->cfd) {
    err = KD_EIO;
  }

  if(0 == err) {

    struct hostent* pHostInfo;    /* holds info about a machine */
    long nHostAddress;
    struct sockaddr_in sAddress;  /* Internet socket address stuct */

    /* get IP address from name */
    pHostInfo = gethostbyname(strHostName);

    /* copy address into long */
    memcpy_unsafe(&nHostAddress, pHostInfo->h_addr, pHostInfo->h_length);

    /* fill address struct */
    sAddress.sin_addr.s_addr = nHostAddress;
    sAddress.sin_port = htons(nHostPort);
    sAddress.sin_family = AF_INET;

    /* connect to host */
    if(SOCKET_ERROR == connect(pPipe->cfd, (struct sockaddr*) &sAddress, sizeof(sAddress))) {
      printf("\nCould not connect to host\n");
      err = KD_EIO;
    }

  }

  return err;
}

/** Close socket */
static CPresult Close( CPhandle hContent )
{
  inet_ContentPipe* pPipe = (inet_ContentPipe*) hContent;
  CPresult err = 0;

  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_inet:%s \n", __func__);

  /* close socket */
  if(SOCKET_ERROR == close(pPipe->cfd)) {
    printf("\nCould not close client socket\n");
    err = KD_EIO;
  }

  return err;
}

/** Check the that specified number of bytes are available for reading or writing (depending on access type).*/
static CPresult CheckAvailableBytes( CPhandle hContent, CPuint nBytesRequested, CP_CHECKBYTESRESULTTYPE *eResult )
{
  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_inet:%s \n", __func__);

  return KD_EBADF;
}

/** Seek to certain position in the content relative to the specified origin. */
static CPresult SetPosition( CPhandle  hContent, CPint nOffset, CP_ORIGINTYPE eOrigin)
{
  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_inet:%s \n", __func__);

  return KD_EBADF;
}

/** Retrieve the current position relative to the start of the content. */
static CPresult GetPosition( CPhandle hContent, CPuint *pPosition)
{
  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_inet:%s \n", __func__);

  return KD_EBADF;
}

/** Retrieve data of the specified size from the content stream (advance content pointer by size of data).
    Note: pipe client provides pointer. This function is appropriate for small high frequency reads. */
static CPresult Read( CPhandle hContent, CPbyte *pData, CPuint nSize)
{
  inet_ContentPipe* pPipe = (inet_ContentPipe*) hContent;
  CPresult err = 0;
  ssize_t count;

  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_inet:%s \n", __func__);

  count = read(pPipe->cfd, (void*) pData, (size_t) nSize);

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
  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_inet:%s \n", __func__);

  return KD_EBADF;
}

/** Release a buffer obtained by ReadBuffer back to the pipe. */
static CPresult ReleaseReadBuffer(CPhandle hContent, CPbyte *pBuffer)
{
  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_inet:%s \n", __func__);

  return KD_EBADF;
}

/** Write data of the specified size to the content (advance content pointer by size of data).
    Note: pipe client provides pointer. This function is appropriate for small high frequency writes. */
static CPresult Write( CPhandle hContent, CPbyte *pData, CPuint nSize)
{
  inet_ContentPipe* pPipe = (inet_ContentPipe*) hContent;
  CPresult err = 0;
  ssize_t count;

  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_inet:%s \n", __func__);

  count = write(pPipe->cfd, (void*) pData, (size_t) nSize);

  if(count < nSize) {
    err = KD_EIO;  /* ??? */
  } else if(count == -1) {
    err = KD_EIO;  /* ??? */
  }

  return err;
}

/** Retrieve a buffer allocated by the pipe used to write data to the content.
    Client will fill buffer with output data. Note: pipe provides pointer. This function is appropriate
    for large writes. The client must call WriteBuffer when done it has filled the buffer with data.*/
static CPresult GetWriteBuffer( CPhandle hContent, CPbyte **ppBuffer, CPuint nSize)
{
  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_inet:%s \n", __func__);

  return KD_EBADF;
}

/** Deliver a buffer obtained via GetWriteBuffer to the pipe. Pipe will write the
    the contents of the buffer to content and advance content pointer by the size of the buffer */
static CPresult WriteBuffer( CPhandle hContent, CPbyte *pBuffer, CPuint nFilledSize)
{
  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_inet:%s \n", __func__);

  return KD_EBADF;
}

/** Register a per-handle client callback with the content pipe. */
static CPresult RegisterCallback( CPhandle hContent, CPresult (*ClientCallback)(CP_EVENTTYPE eEvent, CPuint iParam))
{
  DEBUG(DEB_LEV_FUNCTION_NAME, "content_pipe_inet:%s \n", __func__);

  return KD_EBADF;
}

CPresult inet_pipe_Constructor(CP_PIPETYPE **ppPipe, CPstring szURI)
{
  inet_ContentPipe* pPipe;

  pPipe = (inet_ContentPipe*) calloc(1, sizeof(inet_ContentPipe));

  if(NULL != pPipe) {

    pPipe->pipe.Open = Open;
    pPipe->pipe.Close = Close;
    pPipe->pipe.Create = Create;
    pPipe->pipe.CheckAvailableBytes = CheckAvailableBytes;
    pPipe->pipe.SetPosition = SetPosition;
    pPipe->pipe.GetPosition = GetPosition;
    pPipe->pipe.Read = Read;
    pPipe->pipe.ReadBuffer = ReadBuffer;
    pPipe->pipe.ReleaseReadBuffer = ReleaseReadBuffer;
    pPipe->pipe.Write = Write;
    pPipe->pipe.GetWriteBuffer = GetWriteBuffer;
    pPipe->pipe.WriteBuffer = WriteBuffer;
    pPipe->pipe.RegisterCallback = RegisterCallback;

    pPipe->sfd = SOCKET_ERROR; /* Server file descriptor */
    pPipe->cfd = SOCKET_ERROR; /* Client file descriptor */

    *ppPipe = (CP_PIPETYPE*) pPipe;
  }

  return 0;
}

