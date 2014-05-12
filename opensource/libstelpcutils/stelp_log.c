/*
 * Copyright (C) ST-Ericsson SA 2012. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/inotify.h>

#include <linux_utils.h>


#define __DO_LOG_BSIZE 1024
#define __DO_LOG_TSIZE 32
#define QUOTE(str) #str

#if defined(ANDROID)
#include <sys/prctl.h>
#include <android/log.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define STE_DEBUG_DIR "/data/ste-debug"
#define DUMPER_DIR STE_DEBUG_DIR "/dump"
#define SUMMARY_HEADER "################################################################################"
#define ACTIONBUFFER_SIZE 1024


// Circular buffer pointer
static char* circ_buf=0;
static unsigned int circ_buf_size=0;
static unsigned int write_ptr_offset=0;
#define INITIAL_NB_MODULO 100
// When buffered traces are getting lost, warning is displayed once until error is solved. Then, lost trace bytecount is traced
static int __socket_lost=0;
static int __socket_warning_shown=0;
static int __socket_nb_lost=0;
static int __sock_nb_modulo=INITIAL_NB_MODULO;

static int __log_lost=0;
static int __log_nb_lost=0;
static int __log_warning_shown=0;
static int __log_nb_modulo=INITIAL_NB_MODULO;
// Property-settable variables
static int __syslog_verbose=0;
static int __syslog_walltime=1;
static int __syslog_uptime=1;
static int __syslog_filter_logcat=0;
static int priority_logto_logcat=-1;
static int logcat_has_us_ts=-1;

static char executablename[1024];

static int can_dump=0;

// to be notified on requested operation on circular buffer
static int inotify_fd = -1;
static char filename[FILENAME_MAX];
static char processname[FILENAME_MAX];
static char tmpbuf[__DO_LOG_BSIZE];

static int trace_to_file = 0;
static int trace_to_file_logrotate = 0;
static int trace_to_file_logrotate_filesize = 0;
static int trace_to_file_logrotate_filecount = 0;
static int trace_to_socket=0;
static int is_inotified=0;

static char syslog_filename[1024];
static char logfilename[1024];
#define DEFAULT_PORT 1981
static int __port=DEFAULT_PORT;

// Library is initialized once
static int isLibraryInitialized = 0;


// Those variables hold the process name and the android log tag
static char myself[128];
static char thisprocess[128];
// external variable set to prog name and path under linux
extern char *__progname;

static int fd_syslog_trace = -1;


static int discardable_bytecount=0;

// This variable is set when the circular buffer has been full at least once
// and thus contains data from beginning to end
int circular_buffer_full=0;

// This thread dumps circular buffer 
pthread_t dump_thread;
// This thread handles file logging, in both normal and rolling buffer mode
pthread_t logrotate_thread;
// This thread handles socket comm
pthread_t sock_thread;
// This thread handles inotify event management
pthread_t inotify_thread;

static int __stelp_sock;


// This mutext protects rolling buffer and write pointer offset
// as well as library initializer flag
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t fd_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t inotify_mutex = PTHREAD_MUTEX_INITIALIZER;

// This mutex protext the log file descriptor and associated temporary buffer,
// especially in rolling buffer mode
static pthread_mutex_t logrotate_tmpbuf_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t logrotate_cond = PTHREAD_COND_INITIALIZER;

static pthread_mutex_t sock_tmpbuf_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t sock_cond = PTHREAD_COND_INITIALIZER;
static pthread_cond_t dump_cond = PTHREAD_COND_INITIALIZER;

int stelp_dump_circularbuffer();

int __do_stelp_syslog_dump_circularbuffer(const char *path, int gzip_level);
int __init_stelp_syslog(void);
int __do_sock_read_from_client (int filedes);
int __do_stelp_make_socket (int port);

static int use_timeout=0; // To dump circular buffer after a timeout expires

// static variables for filedump
char tmpbuf_filedump[__DO_LOG_BSIZE*1024];
char tmpbuf_sock[__DO_LOG_BSIZE*1024];
char __dumpbuf_logrotate[__DO_LOG_BSIZE*1024];
unsigned int __dumpbuf_logrotate_size=0;
unsigned int __dumpbuf_sock_size=0;
char __dumpbuf_sock[__DO_LOG_BSIZE*1024];
unsigned int tmpbuf_filedump_size=0;
unsigned int tmpbuf_sock_size=0;
static int __message_counter=0;
#define STELOG_DUMP_TIMEOUT_S 60
#define NSECS_PER_USEC		1000ULL
#define USEC_PER_SEC	1000000L
#define USEC_PER_MSEC	1000L

#define STELOG_ROTATE_DEFAULT_FILESIZE 1000000
#define STELOG_ROTATE_DEFAULT_FILECOUNT 5
const char* prio_names[] = {
    "ANDROID_LOG_UNKNOWN" , /* 0 */
    "ANDROID_LOG_DEFAULT",  /* 1 */
    "ANDROID_LOG_VERBOSE",  /* 2 */
    "ANDROID_LOG_DEBUG",    /* 3 */
    "ANDROID_LOG_INFO",     /* 4 */
    "ANDROID_LOG_WARN",     /* 5 */
    "ANDROID_LOG_ERROR",    /* 6 */
    "ANDROID_LOG_FATAL",    /* 7 */
    "ANDROID_LOG_SILENT"    /* 8 */
};

#ifdef DEBUG
#define START_ACTIVITY(name) long long __start_##name = getTime();

#define END_ACTIVITY(name,acceptable) long long __end_##name = getTime();long long __duration_##name = __end_##name -  __start_##name; if ( __duration_##name > (long long)acceptable) { \
        __android_log_print(ANDROID_LOG_WARN, "syslog-perf","Activity %s lasted more than acceptable threshold %lld us : %lld us\n",QUOTE(name),(long long)acceptable,__duration_##name); \
}
#else
#define START_ACTIVITY(name)
#define END_ACTIVITY(name,acceptable)
#endif


static unsigned long long getTime() {
	struct timespec ts;
	unsigned long long result;

	if(clock_gettime(CLOCK_MONOTONIC, &ts))
	{
	  return 0;
	}
	else
	{
	  result = (unsigned long long)ts.tv_sec * USEC_PER_SEC + (unsigned long long)(ts.tv_nsec/NSECS_PER_USEC);
	  return result;
	}
}

void __do_stelp_syslog(int priority, const char *tag,int filtered,const char *fmt, ...)
{
    va_list ap;
    int size=0,ts_size=0;
    int current_message=0;

    START_ACTIVITY(log_to_buffer);
    START_ACTIVITY(overall_log);
    //long long from = getTime();
	pthread_mutex_lock(&mutex);
    if (!isLibraryInitialized) {
        if (__init_stelp_syslog()) {
            __builtin_trap();
        }
        isLibraryInitialized=1;
    }
    current_message = __message_counter++;
	pthread_mutex_unlock(&mutex);
    

    // prepend microsecond timestamp
    struct timespec ts;
    struct tm tm;
    
    int ts_up_size=0,ts_wall_size=0,msg_size=0;

    if(__syslog_uptime) {
        if (!clock_gettime(CLOCK_MONOTONIC, &ts))
        {
            localtime_r(&ts.tv_sec, &tm);
            ts_up_size += strftime(tmpbuf+ts_up_size,__DO_LOG_BSIZE - ts_up_size, "up=%H:%M:%S", &tm);
            ts_up_size += snprintf(tmpbuf+ts_up_size,__DO_LOG_BSIZE - ts_up_size,".%06d ", ((int) ts.tv_nsec)/1000);

        }
    }
    
    if(__syslog_walltime) {
        ts_wall_size += snprintf(tmpbuf+ts_up_size+ts_wall_size,__DO_LOG_BSIZE - ts_up_size-ts_wall_size,"h=");
        ts_wall_size += stelp_get_timeofday(tmpbuf+ts_up_size+ts_wall_size,24);//sprintf(tmpbuf,"%12lld",stelp_get_timestamp);//
        ts_wall_size += snprintf(tmpbuf+ts_up_size+ts_wall_size,__DO_LOG_BSIZE - ts_up_size-ts_wall_size," ");
    }


    va_start(ap, fmt);
    msg_size = vsnprintf(tmpbuf+ts_up_size+ts_wall_size, __DO_LOG_BSIZE-(ts_up_size+ts_wall_size)-1, fmt, ap);
    va_end(ap);

    size = ts_up_size+ts_wall_size+msg_size;


    if (tmpbuf[size-1]!='\n') {

        // add '\n'
        tmpbuf[size] = '\n';
        tmpbuf[size+1] = 0;
        size+=1;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // LOG TO LOGCAT if prio low enough
    ////////////////////////////////////////////////////////////////////////////////
#if 0
    int offset=0;
    if (!filtered || !__syslog_filter_logcat || priority>=priority_logto_logcat) {
        // message goes to logcat
        //
        // with timestamps in us ?
        if (!logcat_has_us_ts) {
            offset = ts_up_size+ts_wall_size;
        }
        else {
            offset=0;

        }
        __android_log_print(priority, tag,tmpbuf+offset);

    }
#endif


    ////////////////////////////////////////////////////////////////////////////////
    // LOG TO CIRCULAR BUFFER
    ////////////////////////////////////////////////////////////////////////////////

    START_ACTIVITY(mutex_lock);
	pthread_mutex_lock(&mutex); // to protect write_ptr_offset
    END_ACTIVITY(mutex_lock,1000);
    // -> is there enough room to hold our newly created message ?
    if (write_ptr_offset+size+1>=circ_buf_size) {
        int ret;
        // we erase the unfilled content of the buffer
        discardable_bytecount = circ_buf_size-write_ptr_offset-1;
        memset((char*)(circ_buf+write_ptr_offset),0,discardable_bytecount);
        // back to the beginning of the circular buffer
        write_ptr_offset=0;
        circular_buffer_full=1;
    }

    // actually copy to the circular buffer
    memcpy((char*)(circ_buf+write_ptr_offset),tmpbuf,size);
    write_ptr_offset+=size;

	pthread_mutex_unlock(&mutex); // to protect write_ptr_offset


    //long long to = getTime();
    //int duration = (int) ((to-from)/USEC_PER_MSEC);

    END_ACTIVITY(log_to_buffer,10000);

    //if (duration > 10) {
    //    __android_log_print(ANDROID_LOG_INFO,myself,"Duration of log to buffer : %d milliseconds\n",duration);
    //}
    ////////////////////////////////////////////////////////////////////////////////
    // LOG TO ROLLING FILES
    ////////////////////////////////////////////////////////////////////////////////
    if (__syslog_verbose>1) { __android_log_print(ANDROID_LOG_WARN, myself,"Tracing to file : %d to socket : %d\n",trace_to_file,trace_to_socket); }

    if (trace_to_file) {

        START_ACTIVITY(logrotate_tmpbuf_mutex_lock);
        pthread_mutex_lock(&logrotate_tmpbuf_mutex);
        END_ACTIVITY(logrotate_tmpbuf_mutex_lock,1000);
        START_ACTIVITY(logrotate_tmpbuf_mutex_hold);

        if (__syslog_verbose>1) {
            __android_log_print(ANDROID_LOG_WARN, myself,"Tracing to file %d bytes (tmpbuf_filedump_size=%d)\n",size,tmpbuf_filedump_size);
        }


        if (tmpbuf_filedump_size+size>=sizeof(tmpbuf_filedump)) {
            __log_nb_lost++;
            __log_lost += size;
            if (!__log_warning_shown || __log_nb_lost%__log_nb_modulo==0) {
                if (__log_nb_lost%__log_nb_modulo==0) {
                    __log_nb_modulo*=10;
                }
                __android_log_print(ANDROID_LOG_WARN, myself,"WARNING : traces to file will be lost, buffered trace size : %d/%d, requested : %d, lost : %d bytes (%d traces)\n",tmpbuf_filedump_size,sizeof(tmpbuf_filedump),size,__log_lost,__log_nb_lost);
                __log_warning_shown=1;

            }
        }else {
            if (__log_lost) {
                __android_log_print(ANDROID_LOG_WARN, myself,"WARNING : traces to file were lost : %d bytes, %d traces\n",__log_lost,__log_nb_lost);
            }

            __log_nb_modulo=INITIAL_NB_MODULO;
            __log_warning_shown=0;
            __log_lost=0;
            __log_nb_lost=0;
            memcpy(tmpbuf_filedump+tmpbuf_filedump_size,tmpbuf,size);
            tmpbuf_filedump_size += size;

        }
        pthread_mutex_unlock(&logrotate_tmpbuf_mutex);  // to protect tmpbuf_filedump_size and tmpbuf_filedump
        END_ACTIVITY(logrotate_tmpbuf_mutex_hold,1000);

        int pendingcalls=0;

        if (__syslog_verbose>1)
        {
            __android_log_print(ANDROID_LOG_INFO, myself,"Notifying log thread\n");
            
        }
        int s = pthread_cond_broadcast(&logrotate_cond);
        if (s == -1) {
            __android_log_print(ANDROID_LOG_ERROR, myself,"Error in logrotate pthread_cond_broadcast : %s",strerror(errno));
        }




    }
    if (trace_to_socket) {
        START_ACTIVITY(sock_tmpbuf_mutex_lock);
        pthread_mutex_lock(&sock_tmpbuf_mutex);
        END_ACTIVITY(sock_tmpbuf_mutex_lock,1000);
        START_ACTIVITY(sock_tmpbuf_mutex_hold);

        if (__syslog_verbose>1) {
            __android_log_print(ANDROID_LOG_WARN, myself,"Tracing to socket %d bytes (tmpbuf_sock_size=%d)\n",size,tmpbuf_sock_size);
        }

        if (tmpbuf_sock_size+size>=sizeof(tmpbuf_sock)) {
            __socket_nb_lost++;
            __socket_lost += size;
            if (!__socket_warning_shown || __socket_nb_lost%__sock_nb_modulo==0) {
                if (__socket_nb_lost%__sock_nb_modulo==0) {
                    __sock_nb_modulo*=10;
                }
                __android_log_print(ANDROID_LOG_WARN, myself,"WARNING : traces to socket will be lost, buffered trace size : %d/%d, requested : %d, lost : %d bytes (%d traces)\n",tmpbuf_sock_size,sizeof(tmpbuf_sock),size,__socket_lost,__socket_nb_lost);
                __socket_warning_shown=1;
            }
        }else {
            if (__socket_lost) {
                __android_log_print(ANDROID_LOG_WARN, myself,"WARNING : traces to socket were lost : %d bytes\n",__socket_lost);
            }
            __sock_nb_modulo=INITIAL_NB_MODULO;
            __socket_warning_shown=0;
            __socket_lost=0;
            memcpy(tmpbuf_sock+tmpbuf_sock_size,tmpbuf,size);
            tmpbuf_sock_size += size;

        }

        pthread_mutex_unlock(&sock_tmpbuf_mutex);  // to protect tmpbuf_filedump_size and tmpbuf_filedump
        END_ACTIVITY(sock_tmpbuf_mutex_hold,1000);

        int s = pthread_cond_broadcast(&sock_cond);
        if (s == -1) {
            __android_log_print(ANDROID_LOG_ERROR, myself,"Error in sock pthread_cond_broadcast : %s",strerror(errno));
        }


    }


    END_ACTIVITY(overall_log,10000);

}
void __stelp_syslog_write_summary() {
    FILE *file;
    __android_log_print(ANDROID_LOG_INFO, myself,"Re-arming watch on '%s'", filename);
    
    __android_log_print(ANDROID_LOG_INFO, myself,"Opening '%s' under lock", filename);
    if ((file = fopen(filename, "w+")) == NULL) {
        __android_log_print(ANDROID_LOG_ERROR, myself,"open of '%s' for writing failed: %s", filename, strerror(errno));
        pthread_mutex_unlock(&inotify_mutex);
        return;
    }

    fprintf(file, "%s\n", SUMMARY_HEADER);
    fclose(file);
    __android_log_print(ANDROID_LOG_INFO, myself,"Re-armed watch on '%s'", filename);

}

void * __stelp_syslog_inotify_threadentry(void * arg) {


#ifdef PR_SET_NAME
    prctl(PR_SET_NAME, "syslog-inotify", 0, 0, 0);
#endif
    __android_log_print(ANDROID_LOG_INFO, myself,"Entering inotify thread wait loop, monitoring actions in %s",filename);

    while (1) {
        fd_set fds;
        static char buffer[10 * sizeof(struct inotify_event)];

        FD_ZERO(&fds);
        FD_SET(inotify_fd, &fds);

        /* Wait file event */
        if (select(inotify_fd + 1, &fds, NULL, NULL, 0) <= 0) {
            continue;
        }

        /* Retrieve information about the file event */
        int length = read(inotify_fd, buffer, sizeof(buffer));

        if (length <= 0) {
            __android_log_print(ANDROID_LOG_ERROR, myself,"Thread read from inotify_fd failed: %s", strerror(errno));
            continue;
        }

        pthread_mutex_lock(&inotify_mutex);
        int i = 0;
        while (i < length) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            //__android_log_print(ANDROID_LOG_INFO, myself,"file event wd=%d, mask=0x%08x len=%d, name=%s",
            //        event->wd, event->mask, event->len, (event->len ? event->name : ""));
            // File content has been modified
            FILE *file = fopen(filename, "r");
            char actionBuffer[ACTIONBUFFER_SIZE];
            if (file == NULL) {
                __android_log_print(ANDROID_LOG_ERROR, myself,"open of '%s' for reading failed: %s", filename, strerror(errno));
            } else {
                int got_nb_char = fread(actionBuffer, 1,ACTIONBUFFER_SIZE , file);
                fclose(file);
                // check if it was a user action
                if (strncmp(actionBuffer, SUMMARY_HEADER, strlen(SUMMARY_HEADER))) {
                    // Got a user action
                    __android_log_print(ANDROID_LOG_INFO, myself,"User action was trigged : %s\n",actionBuffer);
                    if (strncmp(actionBuffer, "dump", strlen("dump")) == 0) {
                        __android_log_print(ANDROID_LOG_ERROR, myself,"User action: dump logging circular buffer and context");
                        stelp_syslog_dump_circularbuffer();
                    } else if (strncmp(actionBuffer, "fatal",strlen("fatal")) == 0) {
                        __android_log_print(ANDROID_LOG_ERROR, myself,"User action: fatal (dump circular buffer, then context, then trap)");
                        stelp_syslog_dump_circularbuffer();
                        STELP_FATAL("FATAL ERROR TRAPPED IN ENS-CONTEXT-TRACE");
                    } else {
                        __android_log_print(ANDROID_LOG_ERROR, myself,"User action: %s is not implemented\n",actionBuffer);
                    }
                    __stelp_syslog_write_summary();

                }
            }
            i += event->len + sizeof (struct inotify_event);
        }
        pthread_mutex_unlock(&inotify_mutex);
    }
    return (void*)0;

}




#undef max
#define max(x,y) ((x) > (y) ? (x) : (y))

/* This thread waits on a pthread_cond and sends data on socket when notified */
void *__stelp_syslog_sock_threadentry(void *arg) {
    fd_set read_fd_set,write_fd_set,active_read_fd_set;
    int r,max_fd=0;
#ifdef PR_SET_NAME
    prctl(PR_SET_NAME, (unsigned long)"ste-syslog-skw", 0, 0, 0);
#endif

    /* Create the socket and set it up to accept connections. */
    __stelp_sock = __do_stelp_make_socket (__port);
    if (__stelp_sock == -1) {
        __android_log_print(ANDROID_LOG_ERROR, myself," ERROR creating socket, disabling write to socket");
        trace_to_socket = 0;
        return (void*)0;
    }
    if (listen (__stelp_sock, 1) < 0)
    {
        __android_log_print(ANDROID_LOG_ERROR, myself," ERROR on listen() : %s",strerror(errno));
        trace_to_socket = 0;
    }
    __android_log_print(ANDROID_LOG_INFO, myself,"%s syslog socket is bound on port: %d",__progname,__port);


    for(;;) {
        FD_ZERO (&active_read_fd_set);
        FD_SET (__stelp_sock, &active_read_fd_set);
        read_fd_set = active_read_fd_set;
        int s;
        s = pthread_cond_wait(&sock_cond,NULL);
        if (s == -1) {
            __android_log_print(ANDROID_LOG_ERROR, myself,"Error in sock pthread_cond_wait : %s",strerror(errno));
        }
        else{
            int size_to_write;

            pthread_mutex_lock(&sock_tmpbuf_mutex);
            size_to_write=tmpbuf_sock_size;
            pthread_mutex_unlock(&sock_tmpbuf_mutex);
            if (__syslog_verbose>1) __android_log_print(ANDROID_LOG_INFO, myself,"Writing on socket %d bytes",size_to_write);
            if (size_to_write) {


                pthread_mutex_lock(&sock_tmpbuf_mutex);
                START_ACTIVITY(sock_tmpbuf_mutex_hold_writetosock);
                memcpy(__dumpbuf_sock,tmpbuf_sock,tmpbuf_sock_size);
                __dumpbuf_sock_size=tmpbuf_sock_size;
                tmpbuf_sock_size=0;
                END_ACTIVITY(sock_tmpbuf_mutex_hold_writetosock,1000);
                pthread_mutex_unlock(&sock_tmpbuf_mutex);


                if (__syslog_verbose>1) __android_log_print(ANDROID_LOG_ERROR, myself,"before select");

                int r = select (FD_SETSIZE, &read_fd_set, &write_fd_set, NULL, NULL);
                if (r == -1 && errno == EINTR)
                    continue;
                if (r == -1) {
                    __android_log_print(ANDROID_LOG_ERROR, myself," ERROR on select() : %s, disabling write to socket",strerror(errno));
                    trace_to_socket = 0;
                    return (void*)0;
                }
                if (__syslog_verbose>1) __android_log_print(ANDROID_LOG_ERROR, myself,"after select, FD_SETSIZE=%d",FD_SETSIZE);
                int i;
                /* Service all the sockets with input pending. */
                for (i = 0; i < FD_SETSIZE; ++i) {

                    // Actually accept connections
                    if (FD_ISSET (i, &read_fd_set))
                    {
                        if (__syslog_verbose>1) __android_log_print(ANDROID_LOG_INFO, myself,"read_fd_set selected (%d)",i);
                        if (i == __stelp_sock)
                        {
                            __android_log_print(ANDROID_LOG_INFO, myself,"New client : accepting");
                            /* Connection request on original socket. */
                            int new;
                            struct sockaddr_in clientname;
                            int size = sizeof (clientname);
                            new = accept (__stelp_sock,
                                    (struct sockaddr *) &clientname,
                                    &size);
                            if (new < 0)
                            {
                                __android_log_print(ANDROID_LOG_ERROR, myself," ERROR on accept() : %s",strerror(errno));
                                trace_to_socket = 0;
                                return (void*)0; 
                            }
                            __android_log_print(ANDROID_LOG_INFO,myself,"Server: connect from host %s, port %hd.\n",
                                    inet_ntoa (clientname.sin_addr),
                                    ntohs (clientname.sin_port));
                            FD_SET (new, &active_read_fd_set);
                            FD_SET (new, &write_fd_set);
                        }
                        else
                        {
                           if (__syslog_verbose>1) __android_log_print(ANDROID_LOG_INFO, myself,"read_fd_set  : data available");
                            /* Data arriving on an already-connected socket. */
                            if (__do_sock_read_from_client (i) < 0)
                            {
                                __android_log_print(ANDROID_LOG_INFO, myself,"Closing read socket %d",i);
                                if(shutdown(i,SHUT_RDWR)){
                                    __android_log_print(ANDROID_LOG_INFO,myself,"shutdown read socket returned an error : %s\n",strerror(errno));
                                }

                                close (i);
                                FD_CLR (i, &active_read_fd_set);
                            }
                            else {
                                if (__syslog_verbose>1) __android_log_print(ANDROID_LOG_INFO,myself,"read data from socket \n");
                            }
                        }
                    }
                    // Actually write data to socket
                    if (FD_ISSET (i, &write_fd_set)) {
                        if (__syslog_verbose>1) __android_log_print(ANDROID_LOG_INFO, myself,"write_fd_set selected (%d)",i);


                        if (__syslog_verbose>1) __android_log_print(ANDROID_LOG_INFO,myself,"Actually writing to socket %d bytes\n",__dumpbuf_sock_size);

                        int nbytes=0;
                        int tot=0;
                        char * __ptr = __dumpbuf_sock;
                        int __towrite = __dumpbuf_sock_size;
                        do {
                            nbytes = write (i, __ptr , __towrite-nbytes);
                            if (nbytes == -1) {
                                __android_log_print(ANDROID_LOG_ERROR,myself,"wrote returned -1 (%s) clearing %d from write fd set\n",strerror(errno),i);
                                FD_CLR (i, &write_fd_set);
                                break;
                            }
                            else {
                                __towrite-=nbytes;
                                __ptr+=nbytes;
                                tot+=nbytes;
                                 if (__syslog_verbose>1) __android_log_print(ANDROID_LOG_INFO,myself,"Wrote to socket %d bytes\n",nbytes);
                            }
                        } while( __towrite && nbytes);
                        if (__syslog_verbose>1) __android_log_print(ANDROID_LOG_INFO,myself,"Wrote to socket %d bytes\n",tot);

                    }
                }


            }


        }


    }



}

void *__stelp_syslog_logrotate_threadentry(void *arg) {

#ifdef PR_SET_NAME
	prctl(PR_SET_NAME, (unsigned long)"ste-syslog-lrot", 0, 0, 0);
#endif
    for(;;) {

        int s;
        s = pthread_cond_wait(&logrotate_cond,NULL);
        if (s == -1) {
            __android_log_print(ANDROID_LOG_ERROR, myself,"Error in logrotate pthread_cond_wait : %s",strerror(errno));
        }
        else{
            int size_to_write;
            pthread_mutex_lock(&logrotate_tmpbuf_mutex);
            size_to_write=tmpbuf_filedump_size;
            pthread_mutex_unlock(&logrotate_tmpbuf_mutex);

            if (size_to_write) {
                if (__syslog_verbose>1)  __android_log_print(ANDROID_LOG_INFO, myself," stelp_syslog_logrotate processing");
                // Should we rotate logs ?
                pthread_mutex_lock(&fd_mutex);
                if (trace_to_file_logrotate) {
                    int pos = lseek(fd_syslog_trace, 0, SEEK_CUR);
                    if(fd_syslog_trace == -1 || pos >trace_to_file_logrotate_filesize  ) {
                        int i;
                        int cntr=0;
                        char from[FILENAME_MAX],to[FILENAME_MAX];
                        if(fd_syslog_trace == -1) {
                            __android_log_print(ANDROID_LOG_WARN, myself,"Invalid log file descriptor. Reopening log file\n");
                        } else {

                            if (__syslog_verbose && pos >trace_to_file_logrotate_filesize )  {
                                __android_log_print(ANDROID_LOG_INFO, myself,"Log file limit (%d) exceeded\n",trace_to_file_logrotate_filesize);
                            }
                            close( fd_syslog_trace );
                            fd_syslog_trace= -1;
                        }
                        // Renaming existing logs
                        for( i = (trace_to_file_logrotate_filecount - 1); i >= 0; i-- ) {
                            int ret=0;
                            sprintf(from,"%s-%s.%d.txt",syslog_filename,thisprocess,i);
                            sprintf(to,"%s-%s.%d.txt", syslog_filename ,thisprocess, i+1 );
                            ret = rename( from, to );
                            if (!ret) {
                                cntr++;
                            }
                        }
                        sprintf(logfilename,"%s-%s.%d.txt",syslog_filename,thisprocess,0);
                        if (__syslog_verbose) __android_log_print(ANDROID_LOG_INFO, myself,"Renamed %d rotated logs and re-opening %s",cntr,logfilename);
                        fd_syslog_trace = open(logfilename, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
                        if (fd_syslog_trace == -1) {
                            __android_log_print(ANDROID_LOG_ERROR, myself,"Error opening log file %s : %s\n",syslog_filename,strerror(errno));
                            trace_to_file_logrotate = 0;
                            trace_to_file = 0;
                            __android_log_print(ANDROID_LOG_ERROR, myself,"!!!! Disabling logging !!!!\n");
                            return (void*)0;
                        }

                    }

                    else {
                        if (__syslog_verbose>1) __android_log_print(ANDROID_LOG_INFO, myself," Position in log file : %d\n",pos);
                    }

                }
                else {
                    if(fd_syslog_trace == -1) {
                        sprintf(logfilename,"%s.%s.txt",syslog_filename,thisprocess);
                        __android_log_print(ANDROID_LOG_WARN, myself,"Re-opening log file %s\n",logfilename);
                        fd_syslog_trace = open(logfilename, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
                        if (fd_syslog_trace == -1) {
                            __android_log_print(ANDROID_LOG_ERROR, myself,"Error re-opening log file %s : %s\n",logfilename,strerror(errno));
                            trace_to_file_logrotate = 0;
                            trace_to_file = 0;
                            __android_log_print(ANDROID_LOG_ERROR, myself,"!!!! Disabling logging !!!!\n");
                        }

                    }
                }


                if (__syslog_verbose>1) {
                    __android_log_print(ANDROID_LOG_WARN, myself,"Actually writing to file, tmpbuf_filedump_size_written=%d\n",size_to_write);
                }


                pthread_mutex_lock(&logrotate_tmpbuf_mutex);
                START_ACTIVITY(logrotate_tmpbuf_mutex_hold_writetofile);
                memcpy(__dumpbuf_logrotate,tmpbuf_filedump,tmpbuf_filedump_size);
                __dumpbuf_logrotate_size=tmpbuf_filedump_size;
                tmpbuf_filedump_size=0;
                END_ACTIVITY(logrotate_tmpbuf_mutex_hold_writetofile,1000);
                pthread_mutex_unlock(&logrotate_tmpbuf_mutex);


                int count = write(fd_syslog_trace,__dumpbuf_logrotate,__dumpbuf_logrotate_size );
                if (count == -1) {
                    __android_log_print(ANDROID_LOG_ERROR, myself,"Error writing to log file : %s\n",strerror(errno));
                    close(fd_syslog_trace);
                    fd_syslog_trace = -1;

                }

                pthread_mutex_unlock(&fd_mutex);

            }
        }
    }

    return NULL;
}



int __do_stelp_syslog_dump_circularbuffer(const char *path, int gzip_level)
{
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0) {
        __android_log_print(ANDROID_LOG_ERROR, myself,"%s: %s\n", path, strerror(errno));
        return 1;
    }

    int initial_offset,count=0;
    int tot=0;
    int still_to_write = circ_buf_size;
    char * dump_write_ptr;
    int circ_buf_filled_length;


    pthread_mutex_lock(&mutex); // to protect write_ptr_offset
    if (circular_buffer_full) {
         char * startfrom = strstr((char*)(circ_buf+write_ptr_offset), "\n");
         if (!startfrom) {
             // Not really normal...
             dump_write_ptr = circ_buf+write_ptr_offset;
             circ_buf_filled_length = circ_buf_size - discardable_bytecount;
             //__android_log_print(ANDROID_LOG_INFO, myself,"Could NOT find newline from offset %d,  discardable_bytecount = %d\n",write_ptr_offset,discardable_bytecount);
         } else {
             int todiscard=((startfrom+1)-circ_buf-write_ptr_offset);
             dump_write_ptr=circ_buf+write_ptr_offset+todiscard;
             //__android_log_print(ANDROID_LOG_INFO, myself,"Found newline, to discard : %d, discardable_bytecount = %d\n",todiscard,discardable_bytecount);
             circ_buf_filled_length =
             /*overall size*/ circ_buf_size
             -/*end of buffer where there was no room to store a message*/ discardable_bytecount
             -/*offset to next message beginning*/ todiscard;
         }
     } else {
        circ_buf_filled_length = write_ptr_offset;
        dump_write_ptr = circ_buf;
    }
    pthread_mutex_unlock(&mutex);


    initial_offset = dump_write_ptr-circ_buf;
    if (__syslog_verbose)  __android_log_print(ANDROID_LOG_INFO, myself,"Initial offset = %d, circ_buf_filled_length=%d, overall size=%d\n",initial_offset,circ_buf_filled_length,circ_buf_size);

    // write from write pointer till end of the circ buffer
    count += write(fd,dump_write_ptr,circ_buf_filled_length-initial_offset-1);
    if (count>0) {
        still_to_write -= count;
        tot+=count;
    }
    // now write from buffer beginning till initial_offset
    count += write(fd,circ_buf,initial_offset+1);


    __android_log_print(ANDROID_LOG_INFO, myself,"Dumped %d bytes to  %s (buffer size : %d)\n", tot, path,circ_buf_size);
    close(fd);

    pid_t gzip_pid = -1;

    if (gzip_level > 0) {
        gzip_pid = fork();
        if (gzip_pid < 0) {
            fprintf(stderr, "fork: %s\n", strerror(errno));
            exit(1);
        }

        if (gzip_pid == 0) {

            close(fd);
            char level[10];
            int size = snprintf(level, sizeof(level), "-%d", gzip_level);
            execlp("gzip", "gzip", level, path,NULL);
            __android_log_print(ANDROID_LOG_ERROR, myself,"exec(gzip): %s\n", strerror(errno));
            return(1);
        }
        else {
            __android_log_print(ANDROID_LOG_INFO, myself,"gzipping ste-syslog circular buffer to %s.gz\n",path);
        }

    }
    else {
        __android_log_print(ANDROID_LOG_INFO, myself,"Dumped ste-syslog circular buffer to %s\n",path);
    }

    return 0;

}

int stelp_syslog_dump_circularbuffer(void) {
    if (can_dump) {

        int s = pthread_cond_broadcast(&dump_cond);
        if (s == -1) {
            __android_log_print(ANDROID_LOG_ERROR, myself,"Error while dumping circular buffer in pthread_cond_broadcast : %s",strerror(errno));
            return 1;
        }
    } else {

        __android_log_print(ANDROID_LOG_ERROR, myself,"Dump thread not alive. Dumping in context\n");
        char filename[1024];
        sprintf(filename,"/data/ste-debug/%s-dump.txt",myself);
        __do_stelp_syslog_dump_circularbuffer(filename,5);
    }
    return 0;
}


void *__stelp_syslog_dump_threadentry(void *arg) {

#ifdef PR_SET_NAME
	prctl(PR_SET_NAME, (unsigned long)"ste-syslog-dump", 0, 0, 0);
#endif
    for(;;) {
        int s = pthread_cond_wait(&dump_cond,NULL);
        if (s == -1) {
            __android_log_print(ANDROID_LOG_ERROR, myself,"Error in dump pthread_cond_wait : %s",strerror(errno));
        }
        else{
            if (__syslog_verbose)  __android_log_print(ANDROID_LOG_INFO, myself,"Dumping buffer on request");
            char filename[1024];
            sprintf(filename,"/data/ste-debug/%s-dump.txt",myself);
            __do_stelp_syslog_dump_circularbuffer(filename,5);
        }
    }
}

int __init_stelp_syslog(void) {

    // Retrieve executable name in array executablename
    char *tmp_slash;
    {
        char tmp_array[1024];
        char *tmp_ptr=tmp_array;
        strncpy(tmp_ptr,__progname,1023);

        while((tmp_slash = strstr(tmp_ptr+1,"/"))) {
            tmp_ptr=tmp_slash+1;
        }
        strncpy(executablename,tmp_ptr,1023);
    }

    sprintf(myself,"ste-syslog-%s",executablename);
    sprintf(thisprocess,"%s",executablename);

    {
        GET_PROPERTY("ste.syslog.verbose",value,"0");
        __syslog_verbose=atoi(value);
    }
    {
        GET_PROPERTY("ste.syslog.walltime",value,"1");
        __syslog_walltime=atoi(value);
    }
    {
        GET_PROPERTY("ste.syslog.uptime",value,"1");
        __syslog_uptime=atoi(value);
    }
    {
        GET_PROPERTY("ste.syslog.filter.logcat",value,"1");
        __syslog_filter_logcat=atoi(value);
    }

    int prop_value;
    int ret;
    __android_log_print(ANDROID_LOG_WARN, myself,"Initializing ste-syslog\n");

    if (priority_logto_logcat == -1) {

        int _fromprio;
        GET_PROPERTY("ste.syslog.tologcat.fromprio",value,"5");
        _fromprio=atoi(value);
        if (_fromprio>=ANDROID_LOG_UNKNOWN && _fromprio<=ANDROID_LOG_FATAL) {
            priority_logto_logcat=_fromprio;
            __android_log_print(ANDROID_LOG_INFO, myself,"Logging to logcat messages from priority %d (%s)\n",priority_logto_logcat,prio_names[priority_logto_logcat]);
        }
        else {
            priority_logto_logcat=ANDROID_LOG_WARN;
            __android_log_print(ANDROID_LOG_WARN, myself,"Invalid ste.syslog.tologcat.fromprio property : %s,using default : %d (%s)\n",value,priority_logto_logcat,prio_names[priority_logto_logcat]);
        }
    }
    if (logcat_has_us_ts==-1) {
        GET_PROPERTY("ste.syslog.logcat.us.timestamp",value,"0");
        logcat_has_us_ts=atoi(value);
    }
    {
        GET_PROPERTY("ste.syslog.filecount", value, QUOTE(STELOG_ROTATE_DEFAULT_FILECOUNT));
        prop_value = atoi(value);
        if (__syslog_verbose)  __android_log_print(ANDROID_LOG_INFO,myself,"ste.syslog.filecount=%d\n",prop_value);
        if (prop_value) {
            trace_to_file_logrotate_filecount = prop_value;
        }
    }



    {
        GET_PROPERTY("ste.syslog.filesize", value, QUOTE(STELOG_ROTATE_DEFAULT_FILESIZE));
        prop_value = atoi(value);
        if (__syslog_verbose)  __android_log_print(ANDROID_LOG_INFO,myself,"ste.syslog.filesize=%d\n",prop_value);
        if (prop_value) {
            trace_to_file_logrotate_filesize = prop_value;
        }
    }


    {
        GET_PROPERTY("ste.syslog.rotate", value, "0");
        prop_value = atoi(value);
        if (__syslog_verbose)  __android_log_print(ANDROID_LOG_INFO,myself,"ste.syslog.rotate=%d\n",prop_value);
        if (prop_value == 1) {
            trace_to_file_logrotate = 1;
            if (trace_to_file_logrotate_filesize ==0) {
                trace_to_file_logrotate_filesize = STELOG_ROTATE_DEFAULT_FILESIZE;
            }
            if (trace_to_file_logrotate_filecount == 0) {
                trace_to_file_logrotate_filecount = STELOG_ROTATE_DEFAULT_FILECOUNT;
            }

        }
    }
    {
        GET_PROPERTY("ste.syslog.socket", value, "0");
        prop_value = atoi(value);
        if (__syslog_verbose)  __android_log_print(ANDROID_LOG_INFO,myself,"ste.syslog.socket=%d\n",prop_value);
        if (prop_value == 1) {
            trace_to_socket = 1;
        }
    }
    {
        GET_PROPERTY("ste.syslog.socket.port", value, QUOTE(DEFAULT_PORT));
        prop_value = atoi(value);
        if (prop_value) {
            if (__syslog_verbose)  __android_log_print(ANDROID_LOG_INFO,myself,"ste.syslog.socket.port=%d\n",prop_value);
            __port = prop_value;
        }
    }
    {
        char port_byexecname[256];
        snprintf(port_byexecname,255,"ste.syslog.socket.%s.port",executablename);
        GET_PROPERTY(port_byexecname, value, "0");
        prop_value = atoi(value);
        if (__syslog_verbose)  __android_log_print(ANDROID_LOG_INFO,myself,"Port index for %s : %s=%d\n",executablename,port_byexecname,prop_value);
        if (prop_value) {
            __port = prop_value;
        }
    }

    {
        GET_PROPERTY("ste.syslog.tofile", value, "0");
        prop_value = atoi(value);
        if (__syslog_verbose)  __android_log_print(ANDROID_LOG_INFO,myself,"ste.syslog.tofile=%d\n",prop_value);

        if (trace_to_file_logrotate || prop_value == 1) {
            char _filename[FILENAME_MAX];

         if (fd_syslog_trace == -1) {
                {
                    GET_PROPERTY("ste.syslog.filename", value, "/data/ste-debug/ste-syslog");
                    strncpy(syslog_filename, value, 1024);
                }
                
                pthread_mutex_lock(&fd_mutex);


                if (trace_to_file_logrotate) {
                    int i;
                    int cntr=0;
                    char from[FILENAME_MAX],to[FILENAME_MAX];


                    if (__syslog_verbose)  __android_log_print(ANDROID_LOG_INFO,myself,"Using %d rolling logs of max size  %d bytes.\n",trace_to_file_logrotate_filecount,trace_to_file_logrotate_filesize);

                    for( i = (trace_to_file_logrotate_filecount - 1); i >= 0; i-- ) {
                        int ret=0;
                        sprintf(from,"%s-%s.%d.txt",syslog_filename,thisprocess,i);
                        sprintf(to,"%s-%s.%d.txt", syslog_filename ,thisprocess, i+1 );
                        ret = rename( from, to );
                        if (!ret) {
                            cntr++;
                        }
                    }
                    sprintf(logfilename,"%s-%s.%d.txt",syslog_filename,thisprocess,0);
                    __android_log_print(ANDROID_LOG_INFO, myself,"Renamed %d logs from previous session and opening %s",cntr,logfilename);
                    fd_syslog_trace = open(logfilename, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
                    if (fd_syslog_trace == -1) {
                        __android_log_print(ANDROID_LOG_ERROR, myself,"Error opening log file %s : %s\n",logfilename,strerror(errno));
                        trace_to_file_logrotate = 0;
                        trace_to_file = 0;
                        __android_log_print(ANDROID_LOG_ERROR, myself,"!!!! Disabling logging !!!!\n");
                    }
                    else {
                        trace_to_file = 1;
                        trace_to_file_logrotate = 1;
                    }

                } else {
                    sprintf(logfilename,"%s-%s.txt",syslog_filename,thisprocess);
                    fd_syslog_trace = open(logfilename, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
                    if (fd_syslog_trace == -1) {
                        __android_log_print(ANDROID_LOG_ERROR,myself,"Could not open STE OSTTRACE log file: %s (%s)\n",logfilename,strerror(errno));
                        trace_to_file =0;
                        __android_log_print(ANDROID_LOG_ERROR, myself,"!!!! Disabling logging !!!!\n");
                        trace_to_file_logrotate=0;
                    }
                    else {
                        if (__syslog_verbose)  __android_log_print(ANDROID_LOG_INFO,myself,"Log file: %s opened\n", logfilename);
                        trace_to_file = 1;
                    }

                }
                pthread_mutex_unlock(&fd_mutex);


            } else   {
                if (__syslog_verbose)  __android_log_print(ANDROID_LOG_INFO,myself,"ste-syslog file already open\n");
                trace_to_file = 1;
            }
        }
    }




    if (circ_buf == 0) {
        int _size;
        if (__syslog_verbose)  __android_log_print(ANDROID_LOG_WARN, myself,"Allocating circular buffer\n");
        GET_PROPERTY("ste.syslog.buffersize",value,/* "10485760" 10*1024*1024*/ "1048576" /*1024*1024*/);
        _size = atoi(value);
        circ_buf_size = _size*sizeof(char) ;
        circ_buf = (char*)malloc(circ_buf_size);
        if (circ_buf==0) {
            circ_buf_size = 0;
            __android_log_print(ANDROID_LOG_ERROR, myself,"Could not allocate circular buffer of size %s !!\n",value);
            return 1;
        }
        else {
	    memset(circ_buf,0,circ_buf_size);
            write_ptr_offset=0;
        }
    }
    
     ret = pthread_create(&dump_thread, NULL,__stelp_syslog_dump_threadentry,NULL);
    if (ret) {
        __android_log_print(ANDROID_LOG_ERROR, myself,"Error creating dump thread : %s",strerror(errno));
        can_dump=0;
    }
    else {
        can_dump=1;
    }


    if (trace_to_file || trace_to_file_logrotate) {
        ret = pthread_create(&logrotate_thread, NULL,__stelp_syslog_logrotate_threadentry,NULL);
        if (ret) {
            __android_log_print(ANDROID_LOG_ERROR, myself,"Error creating log thread : %s",strerror(errno));
            trace_to_file = trace_to_file_logrotate = 0;
        }

    }
    if (trace_to_socket) {
        ret = pthread_create(&sock_thread, NULL,__stelp_syslog_sock_threadentry,NULL);
        if (ret) {
            __android_log_print(ANDROID_LOG_ERROR, myself,"Error creating socket thread : %s",strerror(errno));
            trace_to_socket = 0;
        }
    }
    if (__syslog_verbose>1) { __android_log_print(ANDROID_LOG_WARN, myself,"In init : Tracing to file : %d to socket : %d\n",trace_to_file,trace_to_socket); }


    // Inotify related actions
    //
    inotify_fd = inotify_init();
        if (inotify_fd < 0) {
        __android_log_print(ANDROID_LOG_ERROR, myself,"Init inotify_init failed: %s", strerror(errno));
        goto END_INOTIFY;
        is_inotified=0;

    }
   // Create directories
    if (mkdir(DUMPER_DIR, S_IRWXU | S_IRWXG | S_IRWXO)) {
        if (errno != EEXIST) {
            __android_log_print(ANDROID_LOG_ERROR, myself,"mkdir of '" DUMPER_DIR "' failed: %s",
                 strerror(errno));
        }
    }

    // Get the process name
    FILE *f = fopen("/proc/self/comm", "r");
    fscanf(f, "%s", processname);
    fclose(f);

    sprintf(filename, DUMPER_DIR "/%s",processname );
    __stelp_syslog_write_summary();
    // Watch the file
    int wd = inotify_add_watch(inotify_fd, filename, IN_CLOSE_WRITE);
    if (wd < 0) {
        __android_log_print(ANDROID_LOG_ERROR, myself,"Init inotify_add_watch failed wd=%d, errno=%s, filename=%s",
             wd, strerror(errno), filename);
        inotify_fd = -1;
        is_inotified=0;
        goto END_INOTIFY;
    }


    ret = pthread_create(&inotify_thread, NULL,__stelp_syslog_inotify_threadentry,NULL);
    if (ret) {
        __android_log_print(ANDROID_LOG_ERROR, myself,"Error creating inotify thread : %s",strerror(errno));
        is_inotified = 0;
    }

END_INOTIFY:


    return 0;

}
int __do_sock_read_from_client (int filedes)
{
    char buffer[__DO_LOG_BSIZE];
    int nbytes;

    nbytes = read (filedes, buffer,__DO_LOG_BSIZE );
    if (nbytes < 0)
    {
        /* Read error. */
        perror ("read");
        exit (EXIT_FAILURE);
    }
    else if (nbytes == 0)
        /* End-of-file. */
        return -1;
    else
    {
        /* Data read. */
        fprintf (stderr, "Server: got message: `%s'\n", buffer);
        return 0;
    }
}
int __do_stelp_make_socket (int port)
{
    int sock;
    struct sockaddr_in name;

    /* Create the socket. */
    sock = socket (PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror ("socket");
        exit (EXIT_FAILURE);
    }

    int bound=0;
    int attempt=0;
    while (!bound) {
        /* Give the socket a name. */
        name.sin_family = AF_INET;
        name.sin_port = htons (__port);
        name.sin_addr.s_addr = htonl (INADDR_ANY);

        int ret = bind (sock, (struct sockaddr *) &name, sizeof (name));
        if (ret<0) {

            {
                if (attempt<10) {
                    __port++;
                    attempt++;
                    __android_log_print(ANDROID_LOG_ERROR, myself,"Error binding socket : %s, retrying on port %d (attempt : %d)",strerror(errno),__port,attempt);
                }
                else {
                    __android_log_print(ANDROID_LOG_ERROR, myself,"Error binding socket : %s (last attempt : %d)",strerror(errno),attempt);
                    return -1;
                }
            }

        }
        else {
            bound=1;
            __android_log_print(ANDROID_LOG_INFO, myself,"socket bound on port %d (attempt : %d)",__port,attempt);

        }
    }

    return sock;
}
#endif

void __do_stelp_log(const char *fmt, ...)
{

  va_list ap;
  char buffmsg[__DO_LOG_BSIZE];
  char buffout[__DO_LOG_BSIZE];
  char sftime[__DO_LOG_TSIZE];
  struct timeval tv;
  struct tm tm;
  int len = strlen(fmt);
  int tid = (int) syscall(__NR_gettid);
  int pid = getpid();

  va_start(ap, fmt);
  vsnprintf(buffmsg, __DO_LOG_BSIZE, fmt, ap);
  va_end(ap);

  gettimeofday(&tv, NULL);
  localtime_r(&tv.tv_sec, &tm);
  strftime(sftime, __DO_LOG_TSIZE, "%m-%d %H:%M:%S", &tm);

  len = snprintf(buffout, __DO_LOG_BSIZE, "%s.%03d  %04d  %04d %s",
                 sftime, (int) tv.tv_usec / 1000, pid, tid, buffmsg);

  if ((len >= 1) && (buffout[len-1] != '\n') && (len < __DO_LOG_BSIZE - 2)) {
    buffout[len] = '\n';
    buffout[len+1] = '\0';
    len++;
  }
  buffout[__DO_LOG_BSIZE - 2] = '\n';
  buffout[__DO_LOG_BSIZE - 1] = '\0';

  write(STDERR_FILENO, buffout, len);
}

