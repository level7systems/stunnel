/*
 *   stunnel       Universal SSL tunnel
 *   Copyright (c) 1998-2002 Michal Trojnara <Michal.Trojnara@mirt.net>
 *                 All Rights Reserved
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef CLIENT_H
#define CLIENT_H

#include "common.h"

/* I/O buffer size */
#define BUFFSIZE 16384

#ifdef HAVE_OPENSSL
#include <openssl/lhash.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#else
#include <lhash.h>
#include <ssl.h>
#include <err.h>
#endif

typedef struct {
    int fd; /* File descriptor */
    int rd; /* Open for read */
    int wr; /* Open for write */
    int is_socket; /* File descriptor is a socket */
} FD;

typedef struct {
    LOCAL_OPTIONS *opt;
    struct sockaddr_in addr; /* Local address */
    FD local_rfd, local_wfd; /* Read and write local descriptors */
    FD remote_fd; /* Remote descriptor */
    int negotiation_level; /* fdscanf() or fdprintf() number in negotiate() */
    SSL *ssl; /* SSL Connection */
    int ip; /* for connect_local() and connect_remote() */
    unsigned long pid; /* PID of local process */
    u32 *resolved_addresses; /* List of IP addresses for delayed lookup */

    char sock_buff[BUFFSIZE]; /* Socket read buffer */
    char ssl_buff[BUFFSIZE]; /* SSL read buffer */
    int sock_ptr, ssl_ptr; /* Index of first unused byte in buffer */
    FD *sock_rfd, *sock_wfd; /* Read and write socket descriptors */
    FD *ssl_rfd, *ssl_wfd; /* Read and write SSL descriptors */
    int sock_bytes, ssl_bytes; /* Bytes written to socket and ssl */
} CLI;

extern int max_clients;
#ifndef USE_WIN32
extern int max_fds;
#endif

#define sock_rd (c->sock_rfd->rd)
#define sock_wr (c->sock_wfd->wr)
#define ssl_rd (c->ssl_rfd->rd)
#define ssl_wr (c->ssl_wfd->wr)

/* descriptor versions of fprintf/fscanf */
int fdprintf(CLI *, int, const char *, ...)
#ifdef __GNUC__
       __attribute__ ((format (printf, 3, 4)));
#else
       ;
#endif
		      
int fdscanf(CLI *, int, const char *, char *)
#ifdef __GNUC__
       __attribute__ ((format (scanf, 3, 0)));
#else
       ;
#endif

/* Prototype for protocol.c */
int negotiate(CLI *c);

#endif /* defined CLIENT_H */

/* End of client.h */
