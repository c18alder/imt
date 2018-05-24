#define UIP_SYN_SENT 2
#define uip_create_linklocal_allnodes_mcast(a) uip_ip6addr(a, 0xff02, 0, 0, 0, 0, 0, 0, 0x0001)
#define _IO_CURRENTLY_PUTTING 0x800
#define PRIXFAST32 __PRIPTR_PREFIX "X"
#define __SSP_STRONG__ 3
#define __DBL_MIN_EXP__ (-1021)
#define __FLT32X_MAX_EXP__ 1024
#define tsch_log_process_pending() 
#define uip_conn_active(conn) (uip_conns[conn].tcpstateflags != UIP_CLOSED)
#define LINKADDR_H_ 
#define FRAME802154_SECURITY_LEVEL_NONE (0)
#define __UINT_LEAST16_MAX__ 0xffff
#define __TSCH_CONF_H__ 
#define TSCH_LOG_ADD(log_type,init_code) 
#define __ATOMIC_ACQUIRE 2
#define __FLT128_MAX_10_EXP__ 4932
#define UIP_IPUDPH_LEN (UIP_UDPH_LEN + UIP_IPH_LEN)
#define _IOS_OUTPUT 2
#define __TSCH_PRIVATE_H__ 
#define PRIXFAST64 __PRI64_PREFIX "X"
#define UIP_DS6_ROUTE_NB UIP_CONF_MAX_ROUTES
#define LOADER_ERR_FMT 4
#define __FLT_MIN__ 1.17549435082228750796873653722224568e-38F
#define __GCC_IEC_559_COMPLEX 2
#define SCNiFAST8 "hhi"
#define uip_urgdatalen() uip_urglen
#define _IO_UNITBUF 020000
#define __UINT_LEAST8_TYPE__ unsigned char
#define __SIZEOF_FLOAT80__ 16
#define _T_WCHAR_ 
#define NETSTACK_CONF_RDC nordc_driver
#define INT_LEAST16_MIN (-32767-1)
#define __flexarr []
#define LOADER_ERR_READ 1
#define QUEUEBUF_NUM QUEUEBUF_CONF_NUM
#define TSCH_AUTOSTART TSCH_CONF_AUTOSTART
#define uip_is_addr_mcast_routable(a) ((((a)->u8[0]) == 0xFF) && (((a)->u8[1] & 0x0F) > 0x02))
#define __S64_TYPE long int
#define TSCH_JOIN_MY_PANID_ONLY 0
#define __TSCH_SCHEDULE_H__ 
#define uip_create_linklocal_allrouters_mcast(a) uip_ip6addr(a, 0xff02, 0, 0, 0, 0, 0, 0, 0x0002)
#define __stub_fchflags 
#define PRIuPTR __PRIPTR_PREFIX "u"
#define __SQUAD_TYPE long int
#define RPL_ROUTE_SET_NOPATH_RECEIVED(route) do { (route)->state.state_flags |= RPL_ROUTE_ENTRY_NOPATH_RECEIVED; } while(0)
#define __INTMAX_C(c) c ## L
#define _BSD_SIZE_T_DEFINED_ 
#define __tune_haswell__ 1
#define PROCESS_WAIT_EVENT_UNTIL(c) PROCESS_YIELD_UNTIL(c)
#define TSCH_SCHEDULE_MAX_SLOTFRAMES 4
#define __TIME_T_TYPE __SYSCALL_SLONG_TYPE
#define __CHAR_BIT__ 8
#define __FSWORD_T_TYPE __SYSCALL_SLONG_TYPE
#define _G_HAVE_MREMAP 1
#define UIP_CONF_TCP_SPLIT 0
#define UIP_CLOSED 0
#define uip_ipaddr_mask(dest,src,mask) do { ((uint16_t *)dest)[0] = ((uint16_t *)src)[0] & ((uint16_t *)mask)[0]; ((uint16_t *)dest)[1] = ((uint16_t *)src)[1] & ((uint16_t *)mask)[1]; } while(0)
#define __UINT8_MAX__ 0xff
#define RPL_ROUTE_ENTRY_DAO_PENDING 0x02
#define CTIMER_H_ 
#define FRAME802154_BEACONREQ (0x07)
#define UIP_RECEIVE_WINDOW (UIP_CONF_RECEIVE_WINDOW)
#define RTIMERTICKS_TO_US_64(T) (T)
#define RPL_ROUTE_ENTRY_NOPATH_RECEIVED 0x01
#define INT16_C(c) c
#define __WINT_MAX__ 0xffffffffU
#define TSCH_CALLBACK_PACKET_READY thunder_callback_packet_ready
#define __FLT32_MIN_EXP__ (-125)
#define LOADER_LOAD_DSC(name) NULL
#define TSCH_CLOCK_TO_SLOTS(c,timeslot_length) (TSCH_CLOCK_TO_TICKS(c) / timeslot_length)
#define __CONTIKI_VERSION__ 
#define INT8_MAX (127)
#define UINT_LEAST64_MAX (__UINT64_C(18446744073709551615))
#define stderr stderr
#define uip_periodic(conn) do { uip_conn = &uip_conns[conn]; uip_process(UIP_TIMER); } while (0)
#define __GLIBC_PREREQ(maj,min) ((__GLIBC__ << 16) + __GLIBC_MINOR__ >= ((maj) << 16) + (min))
#define DEBUG_ANNOTATE 2
#define UIP_UDP_CONNS (UIP_CONF_UDP_CONNS)
#define RADIO_DELAY_BEFORE_RX 0
#define FRAME802154_VERSION FRAME802154_CONF_VERSION
#define __OFF_T_MATCHES_OFF64_T 1
#define UIP_BROADCAST (UIP_CONF_BROADCAST)
#define UIP_APPCALL tcpip_uipcall
#define UIP_STOPPED 16
#define __ORDER_LITTLE_ENDIAN__ 1234
#define __SIZE_MAX__ 0xffffffffffffffffUL
#define __stub_putmsg 
#define __SSE4_1__ 1
#define TSCH_HOPPING_SEQUENCE_4_16 (uint8_t[]){ 20, 26, 25, 26, 15, 15, 25, 20, 26, 15, 26, 25, 20, 15, 20, 25 }
#define PRIdMAX __PRI64_PREFIX "d"
#define __WCHAR_MAX__ 0x7fffffff
#define SCNx16 "hx"
#define INT16_MAX (32767)
#define RADIO_DELAY_BEFORE_DETECT 0
#define RADIO_DELAY_BEFORE_TX 0
#define __BLKCNT_T_TYPE __SYSCALL_SLONG_TYPE
#define TSCH_HOPPING_SEQUENCE_1_1 (uint8_t[]){ 20 }
#define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 1
#define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 1
#define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 1
#define __DBL_DENORM_MIN__ ((double)4.94065645841246544176568792868221372e-324L)
#define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8 1
#define __GCC_ATOMIC_CHAR_LOCK_FREE 2
#define UIP_CONNS (UIP_CONF_MAX_CONNECTIONS)
#define __GCC_IEC_559 2
#define SLIP_BRIDGE_CONF_NO_PUTCHAR 1
#define _IO_flockfile(_fp) 
#define __FLT32X_DECIMAL_DIG__ 17
#define _IOFBF 0
#define LIST_CONCAT2(s1,s2) s1 ##s2
#define __FLT_EVAL_METHOD__ 0
#define _IO_USER_LOCK 0x8000
#define _IO_NO_WRITES 8
#define __ASMNAME2(prefix,cname) __STRING (prefix) cname
#define __unix__ 1
#define WITH_OVERHEARING 1
#define ENERGEST_OFF_LEVEL(type,level) do { } while(0)
#define __FLT64_DECIMAL_DIG__ 17
#define _G_HAVE_ST_BLKSIZE defined (_STATBUF_ST_BLKSIZE)
#define WITH_SWAP 0
#define __stub_setlogin 
#define __GCC_ATOMIC_CHAR32_T_LOCK_FREE 2
#define TSCH_CONF_DEFAULT_HOPPING_SEQUENCE TSCH_HOPPING_SEQUENCE_1_1
#define __SYSCALL_WORDSIZE 64
#define PRINTA(...) printf(__VA_ARGS__)
#define _IO_peekc_unlocked(_fp) (_IO_BE ((_fp)->_IO_read_ptr >= (_fp)->_IO_read_end, 0) && __underflow (_fp) == EOF ? EOF : *(unsigned char *) (_fp)->_IO_read_ptr)
#define PRINTF(...) printf(__VA_ARGS__)
#define FRAME802154_9_BYTE_KEY_ID_MODE (3)
#define INT8_MIN (-128)
#define __x86_64 1
#define LLSEC802154_H_ 
#define tsch_log_init() 
#define __core_avx2__ 1
#define NBR_TABLE_MAX_NEIGHBORS NBR_TABLE_CONF_MAX_NEIGHBORS
#define uip_are_solicited_bytes_equal(a,b) ((((a)->u8[13]) == ((b)->u8[13])) && (((a)->u8[14]) == ((b)->u8[14])) && (((a)->u8[15]) == ((b)->u8[15])))
#define PT_EXITED 2
#define UIP_TCPH_LEN 20
#define __UINT_FAST64_MAX__ 0xffffffffffffffffUL
#define __SIG_ATOMIC_TYPE__ int
#define __GID_T_TYPE __U32_TYPE
#define __DBL_MIN_10_EXP__ (-307)
#define INT16_MIN (-32767-1)
#define PROCINIT_H_ 
#define UIP_URGDATA 0
#define __FINITE_MATH_ONLY__ 0
#define CC_CONST_FUNCTION const
#define FRAME802154_BEACONFRAME (0x00)
#define PRIi32 "i"
#define UINTMAX_C(c) c ## UL
#define LINKADDR_SIZE LINKADDR_CONF_SIZE
#define __attribute_alloc_size__(params) __attribute__ ((__alloc_size__ params))
#define UIP_EXT_HDR_BITMAP_HBHO 0x01
#define CC_CONCAT(s1,s2) CC_CONCAT2(s1, s2)
#define uip_stop() (uip_conn->tcpstateflags |= UIP_STOPPED)
#define PRIxPTR __PRIPTR_PREFIX "x"
#define NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE 8
#define __GNUC_PATCHLEVEL__ 0
#define __FLT32_HAS_DENORM__ 1
#define _IO_pid_t __pid_t
#define __UINT_FAST8_MAX__ 0xff
#define __LEAF , __leaf__
#define UIP_CONF_DS6_PREFIX_NBU 2
#define TSCH_ASSOCIATION_POLL_FREQUENCY 100
#define __LDBL_REDIR1(name,proto,alias) name proto
#define __has_include(STR) __has_include__(STR)
#define FRAME802154_LONGADDRMODE (0x03)
#define __size_t 
#define UIP_CONF_DHCP_LIGHT 
#define SCNdLEAST8 "hhd"
#define INT_FAST8_MAX (127)
#define __DEC64_MAX_EXP__ 385
#define _WCHAR_T_DEFINED 
#define TSCH_PACKET_MAX_LEN MIN(127,PACKETBUF_SIZE)
#define __INT8_C(c) c
#define WCHAR_MAX __WCHAR_MAX
#define NET_DEBUG_H 
#define __INT_LEAST8_WIDTH__ 8
#define __UINT_LEAST64_MAX__ 0xffffffffffffffffUL
#define LINK_OPTION_OVERHEARING 32
#define _IO_FLAGS2_MMAP 1
#define uip_timedout() (uip_flags & UIP_TIMEDOUT)
#define SIMENVCHANGE_H_ 
#define uip_outstanding(conn) ((conn)->len)
#define PROCESS_NAME_STRING(process) (process)->name
#define __always_inline __inline __attribute__ ((__always_inline__))
#define __SHRT_MAX__ 0x7fff
#define SCNd8 "hhd"
#define __LDBL_MAX__ 1.18973149535723176502126385303097021e+4932L
#define uip_gethostaddr(addr) uip_ipaddr_copy((addr), &uip_hostaddr)
#define __FLT64X_MAX_10_EXP__ 4932
#define PROCESS_EVENT_INIT 0x81
#define PT_YIELD_UNTIL(pt,cond) do { PT_YIELD_FLAG = 0; LC_SET((pt)->lc); if((PT_YIELD_FLAG == 0) || !(cond)) { return PT_YIELDED; } } while(0)
#define UIP_SYN_RCVD 1
#define PT_SCHEDULE(f) ((f) < PT_EXITED)
#define UIP_DS6_ROUTE_H 
#define __POPCNT__ 1
#define _IOS_BIN 128
#define __fortify_function __extern_always_inline __attribute_artificial__
#define SCNd16 "hd"
#define _IO_MAGIC_MASK 0xFFFF0000
#define __UINT_LEAST8_MAX__ 0xff
#define __GCC_ATOMIC_BOOL_LOCK_FREE 2
#define FRAME802154_NOBEACONS (0x0F)
#define __FLT128_DENORM_MIN__ 6.47517511943802511092443895822764655e-4966F128
#define PT_WAIT_UNTIL(pt,condition) do { LC_SET((pt)->lc); if(!(condition)) { return PT_WAITING; } } while(0)
#define __UINTMAX_TYPE__ long unsigned int
#define PROCESS_YIELD_UNTIL(c) PT_YIELD_UNTIL(process_pt, c)
#define PT_BEGIN(pt) { char PT_YIELD_FLAG = 1; if (PT_YIELD_FLAG) {;} LC_RESUME((pt)->lc)
#define INT_FAST8_MIN (-128)
#define SCNd32 "d"
#define SCNi8 "hhi"
#define UIP_ICMPH_LEN 4
#define WITH_LEAPFROG 1
#define __linux 1
#define __DEC32_EPSILON__ 1E-6DF
#define WCHAR_MIN __WCHAR_MIN
#define __PRIPTR_PREFIX "l"
#define PT_YIELDED 1
#define __FLT_EVAL_METHOD_TS_18661_3__ 0
#define PROCESS_WAIT_WHILE(c) PT_WAIT_WHILE(process_pt, c)
#define _BITS_TYPES_H 1
#define _IO_cleanup_region_end(_Doit) 
#define _IO_DONT_CLOSE 0100000
#define __unix 1
#define SCNo16 "ho"
#define ____gwchar_t_defined 1
#define __UINT32_MAX__ 0xffffffffU
#define FRAME802154_SECURITY_LEVEL_ENC_MIC_64 (6)
#define __UID_T_TYPE __U32_TYPE
#define getc(_fp) _IO_getc (_fp)
#define __SIZE_T 
#define UIP_TS_MASK 15
#define SCNd64 __PRI64_PREFIX "d"
#define __LDBL_MAX_EXP__ 16384
#define SCNo32 "o"
#define _ATFILE_SOURCE 1
#define SCNuLEAST16 "hu"
#define UINT_FAST16_MAX (18446744073709551615UL)
#define _IO_ssize_t __ssize_t
#define __FLT128_MIN_EXP__ (-16381)
#define __WINT_MIN__ 0U
#define uip_is_addr_mcast(a) (((a)->u8[0]) == 0xFF)
#define UIP_CONF_MAX_LISTENPORTS 8
#define __linux__ 1
#define UINT16_C(c) c
#define EEPROM_CONF_SIZE 1024
#define TSCH_SECURITY_K1 { 0x36, 0x54, 0x69, 0x53, 0x43, 0x48, 0x20, 0x6D, 0x69, 0x6E, 0x69, 0x6D, 0x61, 0x6C, 0x31, 0x35 }
#define TSCH_SECURITY_K2 { 0xde, 0xad, 0xbe, 0xef, 0xfa, 0xce, 0xca, 0xfe, 0xde, 0xad, 0xbe, 0xef, 0xfa, 0xce, 0xca, 0xfe }
#define SCNuLEAST32 "u"
#define SCNo8 "hho"
#define uip_create_unspecified(a) uip_ip6addr(a, 0, 0, 0, 0, 0, 0, 0, 0)
#define __FLT128_MIN_10_EXP__ (-4931)
#define SCNo64 __PRI64_PREFIX "o"
#define SICSLOWPAN_CONF_MAXAGE 8
#define __INT_LEAST16_WIDTH__ 16
#define _SIZE_T_DEFINED_ 
#define TSCH_ASN_INC(asn,inc) do { uint32_t new_ls4b = (asn).ls4b + (inc); if(new_ls4b < (asn).ls4b) { (asn).ms1b++; } (asn).ls4b = new_ls4b; } while(0);
#define TSCH_SECURITY_KEY_INDEX_EB 1
#define QUEUEBUFRAM_NUM QUEUEBUF_NUM
#define TSCH_PACKET_EACK_WITH_DEST_ADDR 1
#define __LDBL_REDIR_NTH(name,proto) name proto __THROW
#define __SCHAR_MAX__ 0x7f
#define L_tmpnam 20
#define __FLT128_MANT_DIG__ 113
#define __USING_NAMESPACE_STD(name) 
#define FRAME802154_IEEERESERVED (0x00)
#define _IO_UNIFIED_JUMPTABLES 1
#define __WCHAR_MIN__ (-__WCHAR_MAX__ - 1)
#define __KERNEL_STRICT_NAMES 
#define SCNxFAST16 __PRIPTR_PREFIX "x"
#define _IO_stderr ((_IO_FILE*)(&_IO_2_1_stderr_))
#define _IO_ferror_unlocked(__fp) (((__fp)->_flags & _IO_ERR_SEEN) != 0)
#define __INT64_C(c) c ## L
#define uip_ip4addr_copy(dest,src) (*((uip_ip4addr_t *)dest) = *((uip_ip4addr_t *)src))
#define SCNuLEAST64 __PRI64_PREFIX "u"
#define __NTH(fct) __attribute__ ((__nothrow__ __LEAF)) fct
#define WITH_THUNDER_ADAPTIVE_EB_SLOT 1
#define __DBL_DIG__ 15
#define TSCH_HW_FRAME_FILTERING 1
#define LC_H_ 
#define RPL_ROUTE_CLEAR_DAO_PENDING(route) do { (route)->state.state_flags &= ~RPL_ROUTE_ENTRY_DAO_PENDING; } while(0)
#define __GCC_ATOMIC_POINTER_LOCK_FREE 2
#define _IO_HEX 0100
#define PROCESS_EVENT_COM 0x89
#define __FLT64X_MANT_DIG__ 64
#define UIP_CONF_ND6_RETRANS_TIMER 10000
#define SCNxFAST32 __PRIPTR_PREFIX "x"
#define TSCH_DEFAULT_TS_ACK_WAIT 400
#define RADIO_RX_MODE_ADDRESS_FILTER (1 << 0)
#define UIP_APPDATA_PTR (void *)&uip_buf[UIP_LLH_LEN + UIP_TCPIP_HLEN]
#define TSCH_DESYNC_THRESHOLD (2 * TSCH_MAX_KEEPALIVE_TIMEOUT)
#define TSCH_CALLBACK_LEAVING_NETWORK tsch_rpl_callback_leaving_network
#define UIP_CONNECTED 64
#define _POSIX_SOURCE 1
#define __SIZEOF_INT__ 4
#define __SIZEOF_POINTER__ 8
#define SCNu8 "hhu"
#define DATA_SEND_INTERVAL_TIME_IN_S 15
#define _DEFAULT_SOURCE 1
#define DATA_PACKET_MARKER "SxKkf0KzxcQ0B"
#define __attribute_used__ __attribute__ ((__used__))
#define PRIuLEAST16 "u"
#define SCNxFAST64 __PRI64_PREFIX "x"
#define RPL_ROUTE_CLEAR_DAO(route) do { (route)->state.state_flags &= ~(RPL_ROUTE_ENTRY_DAO_NACK|RPL_ROUTE_ENTRY_DAO_PENDING); } while(0)
#define _IO_IN_BACKUP 0x100
#define UIP_TIMEDOUT 128
#define PRIuFAST16 __PRIPTR_PREFIX "u"
#define UIP_CONF_IPV6_RPL 1
#define __USER_LABEL_PREFIX__ 
#define __MODE_T_TYPE __U32_TYPE
#define PROCESS_YIELD() PT_YIELD(process_pt)
#define RPL_ROUTE_CLEAR_DAO_NACKED(route) do { (route)->state.state_flags &= ~RPL_ROUTE_ENTRY_DAO_NACK; } while(0)
#define WITH_EXTRA_INFORMATION 1
#define SCNx8 "hhx"
#define PROCESS_EVENT_NONE 0x80
#define PRIuLEAST32 "u"
#define RTIMER_H_ 
#define _IOS_INPUT 1
#define MT_OK 1
#define uip_rexmit() (uip_flags & UIP_REXMIT)
#define RADIO_QUALITY_RANDOM_SET_MIN_QUALITY_IN_PERCENT 100
#define __GLIBC__ 2
#define __END_DECLS 
#define UIP_CONF_NETIF_MAX_ADDRESSES 3
#define PRIuFAST32 __PRIPTR_PREFIX "u"
#define __FLT64X_EPSILON__ 1.08420217248550443400745280086994171e-19F64x
#define __CONCAT(x,y) x ## y
#define PACKETBUF_ATTR_BIT 1
#define UIP_CONF_TCP 1
#define UINT8_MAX (255)
#define __STDC_HOSTED__ 1
#define TSCH_SECURITY_KEY_SEC_LEVEL_OTHER 5
#define TSCH_MAX_INCOMING_PACKETS 4
#define __LDBL_HAS_INFINITY__ 1
#define PROCESS_CONTEXT_BEGIN(p) {struct process *tmp_current = PROCESS_CURRENT();process_current = p
#define __SLONG32_TYPE int
#define _IO_LINE_BUF 0x200
#define PRIuLEAST64 __PRI64_PREFIX "u"
#define _IOS_NOCREATE 32
#define NULLRDC_CONF_AFTER_ACK_DETECTED_WAIT_TIME 0
#define PRIuFAST64 __PRI64_PREFIX "u"
#define _BITS_TYPESIZES_H 1
#define LLSEC802154_HTONL(n) (n)
#define LLSEC802154_HTONS(n) (n)
#define RPL_CALLBACK_PARENT_SWITCH tsch_rpl_callback_parent_switch
#define __GNU_LIBRARY__ 6
#define SICSLOWPAN_CONF_COMPRESSION SICSLOWPAN_COMPRESSION_HC06
#define __SSIZE_T_TYPE __SWORD_TYPE
#define CC_ACCESS_NOW(type,variable) (*(volatile type *)&(variable))
#define __FLT32_DIG__ 6
#define NULLRDC_CONF_SEND_802154_ACK 1
#define __TSCH_PACKET_H__ 
#define COOJA_MTARCH_STACKSIZE 1024
#define _G_BUFSIZ 8192
#define __FLT_EPSILON__ 1.19209289550781250000000000000000000e-7F
#define UIP_DEFAULT_PREFIX_LEN 64
#define uip_ip6addr_copy(dest,src) (*((uip_ip6addr_t *)dest) = *((uip_ip6addr_t *)src))
#define _IO_EOF_SEEN 0x10
#define __ABM__ 1
#define __SHRT_WIDTH__ 16
#define PROCESS_NONE NULL
#define UIP_NEWDATA 2
#define UIP_CONF_ICMP_DEST_UNREACH 1
#define TMP_MAX 238328
#define CONFIGURATION_NAME 2ladder_8motes
#define __DEV_T_TYPE __UQUAD_TYPE
#define UIP_TIME_WAIT_TIMEOUT 120
#define _IO_SKIPWS 01
#define PACKETBUF_ATTR_PACKET_TYPE_STREAM_END 3
#define SIM_INTERFACES(...) const struct simInterface *simInterfaces[] = {__VA_ARGS__, NULL};
#define _IO_SCIENTIFIC 04000
#define __LDBL_MIN__ 3.36210314311209350626267781732175260e-4932L
#define __STDC_UTF_16__ 1
#define PT_SPAWN(pt,child,thread) do { PT_INIT((child)); PT_WAIT_THREAD((pt), (thread)); } while(0)
#define __nonnull(params) __attribute__ ((__nonnull__ params))
#define uip_udpconnection() (uip_conn == NULL)
#define _IO_OCT 040
#define __DEC32_MAX__ 9.999999E96DF
#define __PROJECT_CONF_H__ 
#define LLSEC802154_USES_EXPLICIT_KEYS 0
#define SIM_INTERFACE(name,doActionsBeforeTick,doActionsAfterTick) const struct simInterface name = { doActionsBeforeTick, doActionsAfterTick }
#define RTIMER_NOW() rtimer_arch_now()
#define _IO_SHOWPOS 02000
#define __F16C__ 1
#define NETSTACK_MAC NETSTACK_CONF_MAC
#define _WCHAR_T_ 
#define __FLT64X_DENORM_MIN__ 3.64519953188247460252840593361941982e-4951F64x
#define SICSLOWPAN_CONF_MAC_MAX_PAYLOAD (127 - 2)
#define CLOCK_CONF_SECOND 1000L
#define _STDDEF_H 
#define TSCH_SECURITY_KEY_SEC_LEVEL_ACK 5
#define __FLT32X_HAS_INFINITY__ 1
#define __INT32_MAX__ 0x7fffffff
#define uip_ipaddr_copy(dest,src) (*(dest) = *(src))
#define PROCESS_ZOMBIE ((struct process *)0x1)
#define uip_is_addr_linklocal(a) ((a)->u8[0] == 0xfe && (a)->u8[1] == 0x80)
#define SCNoLEAST16 "ho"
#define NETSTACK_CONF_LLSEC nullsec_driver
#define __INT_WIDTH__ 32
#define __SIZEOF_LONG__ 8
#define _IONBF 2
#define _IO_getc_unlocked(_fp) (_IO_BE ((_fp)->_IO_read_ptr >= (_fp)->_IO_read_end, 0) ? __uflow (_fp) : *(unsigned char *) (_fp)->_IO_read_ptr++)
#define NETSTACK_CONF_NETWORK sicslowpan_driver
#define __STDC_IEC_559__ 1
#define TSCH_CALLBACK_JOINING_NETWORK tsch_rpl_callback_joining_network
#define __STDC_ISO_10646__ 201505L
#define PRIoFAST16 __PRIPTR_PREFIX "o"
#define PACKETBUF_SIZE 128
#define __UINT16_C(c) c
#define __PTRDIFF_WIDTH__ 64
#define UIP_TCP (UIP_CONF_TCP)
#define CONTIKI 1
#define SCNoLEAST32 "o"
#define TSCH_ADAPTIVE_TIMESYNC 1
#define _IO_PENDING_OUTPUT_COUNT(_fp) ((_fp)->_IO_write_ptr - (_fp)->_IO_write_base)
#define RPL_ROUTE_SET_DAO_PENDING(route) do { (route)->state.state_flags |= RPL_ROUTE_ENTRY_DAO_PENDING; } while(0)
#define __DECIMAL_DIG__ 21
#define FRAME802154_IEEE802154E_2012 (0x02)
#define __USE_FORTIFY_LEVEL 0
#define TSCH_SECURITY_KEY_INDEX_ACK 2
#define SCNdLEAST16 "hd"
#define RADIO_QUALITY_RANDOM_START_SET_TIME_IN_S 60000000
#define PRIoFAST32 __PRIPTR_PREFIX "o"
#define CLIF 
#define __FLT64_EPSILON__ 2.22044604925031308084726333618164062e-16F64
#define __gnu_linux__ 1
#define __INTMAX_WIDTH__ 64
#define FRAME802154_SHORTADDRMODE (0x02)
#define INT_FAST16_MAX (9223372036854775807L)
#define uip_datalen() uip_len
#define __attribute_warn_unused_result__ __attribute__ ((__warn_unused_result__))
#define GRANDPARENT_KILL_TIME_IN_S 6000000
#define __has_include_next(STR) __has_include_next__(STR)
#define TIMER_H_ 
#define TSCH_SCHEDULE_MAX_LINKS TSCH_SCHEDULE_CONF_MAX_LINKS
#define RPL_ROUTE_CLEAR_NOPATH_RECEIVED(route) do { (route)->state.state_flags &= ~RPL_ROUTE_ENTRY_NOPATH_RECEIVED; } while(0)
#define SCNdLEAST32 "d"
#define FRAME802154_IEEE802154_2006 (0x01)
#define SCNoLEAST64 __PRI64_PREFIX "o"
#define PTRDIFF_MAX (9223372036854775807L)
#define uip_stopped(conn) ((conn)->tcpstateflags & UIP_STOPPED)
#define _IO_LEFT 02
#define PRIX8 "X"
#define __FLT64X_MIN_10_EXP__ (-4931)
#define TSCH_MAX_KEEPALIVE_TIMEOUT (60 * CLOCK_SECOND)
#define __LDBL_HAS_QUIET_NAN__ 1
#define INT64_C(c) c ## L
#define UIP_PINGADDRCONF (UIP_CONF_PINGADDRCONF)
#define PRIoFAST64 __PRI64_PREFIX "o"
#define __USE_ISOC11 1
#define UIP_TCPIP_HLEN UIP_IPTCPH_LEN
#define __FLT64_MANT_DIG__ 53
#define UIP_CLOSE 16
#define __attribute_const__ __attribute__ ((__const__))
#define COOJA_MT_H_ 
#define __THROW __attribute__ ((__nothrow__ __LEAF))
#define SCNxFAST8 "hhx"
#define CONTIKI_DEFAULT_CONF_H 
#define SCNdLEAST64 __PRI64_PREFIX "d"
#define PACKETBUF_ATTR_LAST { PACKETBUF_ATTR_NONE, 0 }
#define uip_is_addr_linklocal_allrouters_mcast(a) ((((a)->u8[0]) == 0xff) && (((a)->u8[1]) == 0x02) && (((a)->u16[1]) == 0) && (((a)->u16[2]) == 0) && (((a)->u16[3]) == 0) && (((a)->u16[4]) == 0) && (((a)->u16[5]) == 0) && (((a)->u16[6]) == 0) && (((a)->u8[14]) == 0) && (((a)->u8[15]) == 0x02))
#define ___int_wchar_t_h 
#define _T_PTRDIFF 
#define __GNUC__ 7
#define __SYSCALL_ULONG_TYPE __ULONGWORD_TYPE
#define PRIoLEAST16 "o"
#define __MMX__ 1
#define __XSAVES__ 1
#define PACKETBUF_WITH_PACKET_TYPE NETSTACK_CONF_WITH_RIME
#define RANDOM_SEED_NUMBER 1
#define PT_END(pt) LC_END((pt)->lc); PT_YIELD_FLAG = 0; PT_INIT(pt); return PT_ENDED; }
#define L_ctermid 9
#define __OFF64_T_TYPE __SQUAD_TYPE
#define UIP_CONF_RECEIVE_WINDOW (UIP_CONF_BUFFER_SIZE - 70)
#define PT_ENDED 3
#define PACKETBUF_ATTR_BYTE 8
#define __FLT_HAS_DENORM__ 1
#define __SIZEOF_LONG_DOUBLE__ 16
#define UIP_PROTO_DESTO 60
#define LIST_STRUCT(name) void *LIST_CONCAT(name,_list); list_t name
#define __XSAVEOPT__ 1
#define offsetof(TYPE,MEMBER) __builtin_offsetof (TYPE, MEMBER)
#define PRIoLEAST32 "o"
#define INT_FAST16_MIN (-9223372036854775807L-1)
#define PRIdLEAST16 "d"
#define CC_ALIGN(n) CC_CONF_ALIGN(n)
#define __LDBL_REDIR1_NTH(name,proto,alias) name proto __THROW
#define UIP_DATA 1
#define __BIGGEST_ALIGNMENT__ 32
#define PROCESS_H_ 
#define PTRDIFF_MIN (-9223372036854775807L-1)
#define w_memcpy memcpy
#define UIP_DS6_NOTIFICATION_DEFRT_ADD 0
#define __PRFCHW__ 1
#define PRIuMAX __PRI64_PREFIX "u"
#define __FLT64_MAX_10_EXP__ 308
#define PRIdLEAST32 "d"
#define COOJA_MTARCH_H_ 
#define ENERGEST_SWITCH(type_off,type_on) do { } while(0)
#define PRIoLEAST64 __PRI64_PREFIX "o"
#define __USE_ISOC95 1
#define __USE_ISOC99 1
#define __ASMNAME(cname) __ASMNAME2 (__USER_LABEL_PREFIX__, cname)
#define _IOS_ATEND 4
#define FRAME802154_BROADCASTPANDID (0xFFFF)
#define __CLOCKID_T_TYPE __S32_TYPE
#define LOADER_ERR_VERSION 8
#define _STDIO_H 1
#define TSCH_QUEUE_MAX_NEIGHBOR_QUEUES ((NBR_TABLE_CONF_MAX_NEIGHBORS) + 2)
#define __DBL_MAX__ ((double)1.79769313486231570814527423731704357e+308L)
#define PRIiFAST16 __PRIPTR_PREFIX "i"
#define __INT_FAST32_MAX__ 0x7fffffffffffffffL
#define FRAME802154_NOADDR (0x00)
#define __DBL_HAS_INFINITY__ 1
#define TSCH_ASN_DEC(asn,dec) do { uint32_t new_ls4b = (asn).ls4b - (dec); if(new_ls4b > (asn).ls4b) { (asn).ms1b--; } (asn).ls4b = new_ls4b; } while(0);
#define RF_CHANNEL 26
#define NETSTACK_NETWORK NETSTACK_CONF_NETWORK
#define COOJA_CONF_SIMULATE_TURNAROUND 0
#define PRIdLEAST64 __PRI64_PREFIX "d"
#define PRId8 "d"
#define __SSE4_2__ 1
#define PRIiFAST32 __PRIPTR_PREFIX "i"
#define UIP_H_ 
#define stdin stdin
#define PRIi16 "i"
#define __USE_XOPEN2K 1
#define UIP_TTL 64
#define UIP_LITTLE_ENDIAN 3412
#define CC_DOUBLE_HASH 0
#define __DEC32_MIN_EXP__ (-94)
#define TSCH_PACKET_EB_WITH_SLOTFRAME_AND_LINK 0
#define TSCH_HOPPING_SEQUENCE_2_2 (uint8_t[]){ 20, 25 }
#define __DADDR_T_TYPE __S32_TYPE
#define __END_NAMESPACE_C99 
#define TSCH_MAX_JOIN_PRIORITY 32
#define __INTPTR_WIDTH__ 64
#define TSCH_MAC_MIN_BE 1
#define WITH_ASCII 1
#define PRIiFAST64 __PRI64_PREFIX "i"
#define __FD_SETSIZE 1024
#define __attribute_format_strfmon__(a,b) __attribute__ ((__format__ (__strfmon__, a, b)))
#define _IO_MAGIC 0xFBAD0000
#define __FLT32X_HAS_DENORM__ 1
#define __INT_FAST16_TYPE__ long int
#define TSCH_LOG_LEVEL TSCH_LOG_CONF_LEVEL
#define TSCH_DEFAULT_TS_RX_TX 192
#define _SIZE_T_DEFINED 
#define _WCHAR_T_DEFINED_ 
#define PRIi8 "i"
#define NETSTACK_CONF_WITH_IPV6 1
#define SCNdFAST8 "hhd"
#define PRIi64 __PRI64_PREFIX "i"
#define PACKETBUF_ATTRIBUTES(...) { __VA_ARGS__ PACKETBUF_ATTR_LAST }
#define PT_YIELD(pt) do { PT_YIELD_FLAG = 0; LC_SET((pt)->lc); if(PT_YIELD_FLAG == 0) { return PT_YIELDED; } } while(0)
#define UINT_LEAST8_MAX (255)
#define __USE_POSIX199506 1
#define _FEATURES_H 1
#define __LDBL_HAS_DENORM__ 1
#define UIP_UDP_TIMER 5
#define TSCH_SECURITY_KEY_INDEX_OTHER 2
#define SCNiLEAST16 "hi"
#define PACKETBUF_ATTR_PACKET_TYPE_TIMESTAMP 4
#define TSCH_DEFAULT_TS_CCA_OFFSET 1800
#define __stub_getmsg 
#define LLSEC802154_USES_FRAME_COUNTER LLSEC802154_ENABLED
#define _IO_FIXED 010000
#define UIP_FIN_WAIT_1 4
#define UIP_FIN_WAIT_2 5
#define __stub_fattach 
#define __FLT128_HAS_INFINITY__ 1
#define __DEC128_MAX__ 9.999999999999999999999999999999999E6144DL
#define CONTIKI_TARGET_COOJA 1
#define __INT_LEAST32_MAX__ 0x7fffffff
#define _IO_UNBUFFERED 2
#define _GCC_WRAP_STDINT_H 
#define _IO_INTERNAL 010
#define __DEC32_MIN__ 1E-95DF
#define PROCESS_EVENT_MAX 0x8a
#define SCNiLEAST32 "i"
#define __S32_TYPE int
#define UIP_UDP_CHECKSUMS (UIP_CONF_UDP_CHECKSUMS)
#define UIP_TIMER 2
#define __glibc_unlikely(cond) __builtin_expect ((cond), 0)
#define __DBL_MAX_EXP__ 1024
#define __WCHAR_WIDTH__ 32
#define __RINGBUFINDEX_H__ 
#define __FLT32_MAX__ 3.40282346638528859811704183484516925e+38F32
#define _BITS_WCHAR_H 1
#define __DEC128_EPSILON__ 1E-33DL
#define __SSE2_MATH__ 1
#define __ATOMIC_HLE_RELEASE 131072
#define UIP_CONF_FWCACHE_SIZE 30
#define UIP_CONF_LL_802154 1
#define QUEUEBUF_DEBUG 0
#define TSCH_SCHEDULE_PRINT_INTERVAL_IN_S 60
#define SCNiLEAST64 __PRI64_PREFIX "i"
#define TSCH_QUEUE_NUM_PER_NEIGHBOR 16
#define __PTRDIFF_MAX__ 0x7fffffffffffffffL
#define uip_l2_l3_icmp_hdr_len (UIP_LLH_LEN + UIP_IPH_LEN + uip_ext_len + UIP_ICMPH_LEN)
#define __CPU_MASK_TYPE __SYSCALL_ULONG_TYPE
#define __amd64 1
#define NETSTACK_RADIO NETSTACK_CONF_RADIO
#define PROCESS_WAIT_EVENT() PROCESS_YIELD()
#define _IO_LINKED 0x80
#define CONTIKI_VERSION_STRING "Contiki-XYZ-97-g839b44b"
#define SCNdFAST16 __PRIPTR_PREFIX "d"
#define UIP_ICMP6_APPCALL tcpip_icmp6_call
#define _IO_HAVE_ST_BLKSIZE _G_HAVE_ST_BLKSIZE
#define __STDC_NO_THREADS__ 1
#define PACKETBUF_CONF_ATTRS_INLINE 1
#define __USECONDS_T_TYPE __U32_TYPE
#define _IO_DELETE_DONT_CLOSE 0x40
#define __BEGIN_NAMESPACE_STD 
#define _PTRDIFF_T_DECLARED 
#define MAKE_WITH_THUNDER 1
#define __OFF_T_TYPE __SYSCALL_SLONG_TYPE
#define UIP_LLADDR_LEN UIP_802154_LONGADDR_LEN
#define UIP_PROTO_FRAG 44
#define __AVX__ 1
#define __ATOMIC_HLE_ACQUIRE 65536
#define __FLT32_HAS_QUIET_NAN__ 1
#define DATA_SEND_START_TIME_IN_S 1200
#define WITH_THUNDER 1
#define _T_WCHAR 
#define PRINTLLADDR(lladdr) net_debug_lladdr_print(lladdr)
#define __LONG_LONG_MAX__ 0x7fffffffffffffffLL
#define __SIZEOF_SIZE_T__ 8
#define PRIiLEAST16 "i"
#define uip_is_addr_mcast_global(a) ((((a)->u8[0]) == 0xFF) && (((a)->u8[1] & 0x0F) == 0x0E))
#define _WCHAR_T 
#define UIP_UDPH_LEN 8
#define SCNiPTR __PRIPTR_PREFIX "i"
#define PROCESS_NAME(name) extern struct process name
#define __LZCNT__ 1
#define PT_RESTART(pt) do { PT_INIT(pt); return PT_WAITING; } while(0)
#define UIP_EXT_HDR_OPT_RPL 0x63
#define UIP_POLL 8
#define UIP_PROTO_ICMP 1
#define PRIu8 "u"
#define __FLT64X_MIN_EXP__ (-16381)
#define SIZE_MAX (18446744073709551615UL)
#define __SIZEOF_WINT_T__ 4
#define PRIxLEAST8 "x"
#define PRIiLEAST32 "i"
#define TSCH_DEFAULT_TS_TX_ACK_DELAY 1000
#define SCNdFAST64 __PRI64_PREFIX "d"
#define CC_CONCAT_EXT_2(s1,s2) CC_CONCAT2(s1, s2)
#define CC_CONCAT_EXT_3(s1,s2,s3) CC_CONCAT3(s1, s2, s3)
#define TSCH_CHECK_TIME_AT_ASSOCIATION 0
#define UINT64_C(c) c ## UL
#define INT32_MIN (-2147483647-1)
#define __LONG_LONG_WIDTH__ 64
#define FRAME_802154E_H 
#define __U16_TYPE unsigned short int
#define _GCC_WCHAR_T 
#define __FLT32_MAX_EXP__ 128
#define WINT_MAX (4294967295u)
#define PRIiFAST8 "i"
#define UINTPTR_MAX (18446744073709551615UL)
#define UIP_LINK_MTU 1280
#define PRIx8 "x"
#define _PTRDIFF_T 
#define PRIiLEAST64 __PRI64_PREFIX "i"
#define RPL_ROUTE_IS_DAO_PENDING(route) ((route->state.state_flags & RPL_ROUTE_ENTRY_DAO_PENDING) != 0)
#define __GCC_HAVE_DWARF2_CFI_ASM 1
#define __GXX_ABI_VERSION 1011
#define __RTM__ 1
#define uip_restart() do { uip_flags |= UIP_NEWDATA; uip_conn->tcpstateflags &= ~UIP_STOPPED; } while(0)
#define SCNoFAST8 "hho"
#define PROCESS_PAUSE() do { process_post(PROCESS_CURRENT(), PROCESS_EVENT_CONTINUE, NULL); PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_CONTINUE); } while(0)
#define AUTOSTART_H_ 
#define uip_udp_periodic(conn) do { uip_udp_conn = &uip_udp_conns[conn]; uip_process(UIP_UDP_TIMER); } while(0)
#define UIP_FRAGH_LEN 8
#define __FLT_MIN_EXP__ (-125)
#define UIP_TCP_MSS (UIP_CONF_TCP_MSS)
#define FRAME802154_CONF_VERSION FRAME802154_IEEE802154E_2012
#define SIM_INTERFACE_NAME(name) extern const struct simInterface name
#define LINK_OPTION_SHARED 4
#define _IO_NO_READS 4
#define UINT32_MAX (4294967295U)
#define uip_udp_remove(conn) (conn)->lport = 0
#define FRAME802154_SECURITY_LEVEL_ENC (4)
#define TSCH_HOPPING_SEQUENCE_MAX_LEN 16
#define __extern_always_inline extern __always_inline __attribute__ ((__gnu_inline__))
#define __PTRDIFF_T 
#define LLSEC_H_ 
#define __FLT64X_HAS_QUIET_NAN__ 1
#define __INT_FAST64_TYPE__ long int
#define PROCESS_EVENT_MSG 0x86
#define PACKETBUF_ADDR_FIRST PACKETBUF_ADDR_SENDER
#define __FP_FAST_FMAF 1
#define uip_poll_conn(conn) do { uip_conn = conn; uip_process(UIP_POLL_REQUEST); } while (0)
#define SCNuLEAST8 "hhu"
#define PROCESS_EVENT_EXIT 0x83
#define PROCESS_WAIT_UNTIL(c) PT_WAIT_UNTIL(process_pt, c)
#define __FLT64_DENORM_MIN__ 4.94065645841246544176568792868221372e-324F64
#define TSCH_PACKET_EACK_WITH_SRC_ADDR 0
#define DEBUG_FULL DEBUG_ANNOTATE | DEBUG_PRINT
#define __DBL_MIN__ ((double)2.22507385850720138309023271733240406e-308L)
#define WINT_MIN (0u)
#define PRIx16 "x"
#define uip_l3_icmp_hdr_len (UIP_IPH_LEN + uip_ext_len + UIP_ICMPH_LEN)
#define __PCLMUL__ 1
#define __TSCH_ASN_H__ 
#define TSCH_CONF_MAC_MAX_FRAME_RETRIES 1
#define UIP_CONF_TCP_MSS (UIP_CONF_BUFFER_SIZE - 70)
#define PRIx32 "x"
#define __REDIRECT(name,proto,alias) name proto __asm__ (__ASMNAME (#alias))
#define CLOCK_SECOND CLOCK_CONF_SECOND
#define __LP64__ 1
#define TSCH_EB_PERIOD (16 * CLOCK_SECOND)
#define _IO_off64_t __off64_t
#define RADIO_QUALITY_RANDOM_SET_INTERVAL_TIME_IN_S 60000000
#define SCNoMAX __PRI64_PREFIX "o"
#define __FLT32X_EPSILON__ 2.22044604925031308084726333618164062e-16F32x
#define PROCESS_ERR_FULL 1
#define PROCESS_BROADCAST NULL
#define uip_newdata() (uip_flags & UIP_NEWDATA)
#define uip_l2_l3_hdr_len (UIP_LLH_LEN + UIP_IPH_LEN + uip_ext_len)
#define UIP_REASS_MAXAGE 60
#define __LEAF_ATTR __attribute__ ((__leaf__))
#define __DECIMAL_BID_FORMAT__ 1
#define CC_CONF_VA_ARGS 1
#define TSCH_PACKET_DURATION(len) US_TO_RTIMERTICKS(32 * ((len) + 3))
#define __FLT64_MIN_EXP__ (-1021)
#define UIP_BYTE_ORDER (UIP_LITTLE_ENDIAN)
#define PRIx64 __PRI64_PREFIX "x"
#define __RLIM64_T_TYPE __UQUAD_TYPE
#define FRAMER_802154_H_ 
#define RANDOM_RAND_MAX 65535U
#define uip_clear_buf() { uip_len = 0; uip_ext_len = 0; }
#define TSCH_DEFAULT_HOPPING_SEQUENCE TSCH_CONF_DEFAULT_HOPPING_SEQUENCE
#define uip_input() uip_process(UIP_DATA)
#define INT_LEAST8_MAX (127)
#define NETSTACK_RDC_CHANNEL_CHECK_RATE NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE
#define UIP_ARP_MAXAGE 120
#define PROCESS_EXIT() PT_EXIT(process_pt)
#define CC_CONCAT2(s1,s2) s1 ##s2
#define CC_CONCAT3(s1,s2,s3) s1 ##s2 ##s3
#define _IO_BOOLALPHA 0200000
#define LINK_OPTION_TIME_KEEPING 8
#define LINK_OPTION_RX 2
#define uip_is_addr_unspecified(a) ((((a)->u16[0]) == 0) && (((a)->u16[1]) == 0) && (((a)->u16[2]) == 0) && (((a)->u16[3]) == 0) && (((a)->u16[4]) == 0) && (((a)->u16[5]) == 0) && (((a)->u16[6]) == 0) && (((a)->u16[7]) == 0))
#define __FLT64_MIN_10_EXP__ (-307)
#define __CLFLUSHOPT__ 1
#define PACKETBUF_IS_ADDR(type) ((type) >= PACKETBUF_ADDR_FIRST)
#define UIP_STATISTICS 0
#define __FLT64X_DECIMAL_DIG__ 21
#define FRAME802154_BROADCASTADDR (0xFFFF)
#define __DEC128_MIN__ 1E-6143DL
#define UIP_PROTO_NONE 59
#define LINK_OPTION_TX 1
#define __REGISTER_PREFIX__ 
#define PROCESS_EVENT_SERVICE_REMOVED 0x84
#define __UINT16_MAX__ 0xffff
#define QUEUEBUF_CONF_NUM 16
#define __DBL_HAS_DENORM__ 1
#define INT_LEAST32_MAX (2147483647)
#define uip_ipaddr_to_quad(a) (a)->u8[0],(a)->u8[1],(a)->u8[2],(a)->u8[3]
#define uip_abort() (uip_flags = UIP_ABORT)
#define POWERTRACKER_INTERVAL_TIME_IN_S 600
#define _IO_TIED_PUT_GET 0x400
#define uip_setdraddr(addr) uip_ipaddr_copy(&uip_draddr, (addr))
#define __USE_POSIX2 1
#define __FLT32_MIN__ 1.17549435082228750796873653722224568e-38F32
#define __UINT8_TYPE__ unsigned char
#define FRAME802154_SECURITY_LEVEL_ENC_MIC_32 (5)
#define __SLONGWORD_TYPE long int
#define RADIO_H_ 
#define __XSAVE__ 1
#define __REDIRECT_LDBL(name,proto,alias) __REDIRECT (name, proto, alias)
#define UIP_CONF_UDP 1
#define __NO_INLINE__ 1
#define tcp_markconn(conn,appstate) tcp_attach(conn, appstate)
#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH 3
#define __FSID_T_TYPE struct { int __val[2]; }
#define FRAME802154_5_BYTE_KEY_ID_MODE (2)
#define BUSYWAIT_UNTIL_ABS(cond,t0,offset) while(!(cond) && RTIMER_CLOCK_LT(RTIMER_NOW(), (t0) + (offset))) { simProcessRunValue = 1; cooja_mt_yield(); };
#define __warndecl(name,msg) extern void name (void) __attribute__((__warning__ (msg)))
#define uip_ipaddr1(addr) ((addr)->u8[0])
#define uip_ipaddr2(addr) ((addr)->u8[1])
#define uip_ipaddr3(addr) ((addr)->u8[2])
#define SCNiMAX __PRI64_PREFIX "i"
#define UIP_DS6_NOTIFICATIONS (UIP_CONF_MAX_ROUTES != 0)
#define UIP_DS6_NOTIFICATION_DEFRT_RM 1
#define TSCH_DEFAULT_TS_CCA 128
#define __FLT_MANT_DIG__ 24
#define __LDBL_DECIMAL_DIG__ 21
#define UIP_802154_SHORTADDR_LEN 2
#define INT_LEAST8_MIN (-128)
#define TSCH_MAC_MAX_BE 7
#define TSCH_RADIO_ON_DURING_TIMESLOT 0
#define __VERSION__ "7.2.0"
#define SCNoLEAST8 "hho"
#define MAC_H_ 
#define TSCH_PACKET_EB_WITH_TIMESLOT_TIMING 0
#define PT_INIT(pt) LC_INIT((pt)->lc)
#define UINT_LEAST16_MAX (65535)
#define TSCH_SECURITY_KEY_SEC_LEVEL_EB 1
#define __UINT64_C(c) c ## UL
#define _PTRDIFF_T_ 
#define PRIXLEAST16 "X"
#define _SYS_CDEFS_H 1
#define NETSTACK_H 
#define AODV_COMPLIANCE 
#define LOADER_ERR_NOLOADER 9
#define SCNi16 "hi"
#define __FMA__ 1
#define FRAME_802154_H 
#define UIP_UDP_SEND_CONN 4
#define _STDC_PREDEF_H 1
#define INT_LEAST32_MIN (-2147483647-1)
#define __ssize_t_defined 
#define RTIMER_SECOND RTIMER_ARCH_SECOND
#define PRIXLEAST32 "X"
#define __INT_WCHAR_T_H 
#define _IO_file_flags _flags
#define SICSLOWPAN_CONF_MAX_ADDR_CONTEXTS 2
#define __USE_XOPEN2K8 1
#define RADIO_QUALITY_SET_QUALITY_IN_PERCENT 90
#define UIP_LISTENPORTS (UIP_CONF_MAX_LISTENPORTS)
#define SCNi32 "i"
#define CLASSNAME Lib2
#define __UQUAD_TYPE unsigned long int
#define UIP_CONF_MAX_ROUTES 300
#define uip_ip6addr(addr,addr0,addr1,addr2,addr3,addr4,addr5,addr6,addr7) do { (addr)->u16[0] = UIP_HTONS(addr0); (addr)->u16[1] = UIP_HTONS(addr1); (addr)->u16[2] = UIP_HTONS(addr2); (addr)->u16[3] = UIP_HTONS(addr3); (addr)->u16[4] = UIP_HTONS(addr4); (addr)->u16[5] = UIP_HTONS(addr5); (addr)->u16[6] = UIP_HTONS(addr6); (addr)->u16[7] = UIP_HTONS(addr7); } while(0)
#define CC_CONF_REGISTER_ARGS 1
#define TSCH_KEEPALIVE_TIMEOUT (12 * CLOCK_SECOND)
#define __TSCH_H__ 
#define __STRING(x) #x
#define UIP_EXT_HDR_BITMAP_DESTO1 0x02
#define UIP_EXT_HDR_BITMAP_DESTO2 0x40
#define __GCC_ATOMIC_INT_LOCK_FREE 2
#define _T_PTRDIFF_ 
#define uip_getnetmask(addr) uip_ipaddr_copy((addr), &uip_netmask)
#define UIP_FIXEDADDR 0
#define NBR_TABLE_DECLARE(name) extern nbr_table_t *name
#define __FLT128_MAX_EXP__ 16384
#define PRIXLEAST64 __PRI64_PREFIX "X"
#define __INO_T_TYPE __SYSCALL_ULONG_TYPE
#define PACKETBUF_ADDRSIZE (LINKADDR_SIZE * PACKETBUF_ATTR_BYTE)
#define LOADER_ERR_ARCH 7
#define TSCH_MAX_EB_PERIOD (50 * CLOCK_SECOND)
#define _STDIO_USES_IOSTREAM 
#define SCNi64 __PRI64_PREFIX "i"
#define uip_l3_hdr_len (UIP_IPH_LEN + uip_ext_len)
#define LOADER_ERR_HDR 2
#define uip_aborted() (uip_flags & UIP_ABORT)
#define RADIO_RX_MODE_AUTOACK (1 << 1)
#define __GNUC_PREREQ(maj,min) ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#define uip_udp_periodic_conn(conn) do { uip_udp_conn = conn; uip_process(UIP_UDP_TIMER); } while(0)
#define PROJECT_CONF_H "../project-conf.h"
#define TSCH_DEFAULT_TS_RX_ACK_DELAY 800
#define __FLT32_MANT_DIG__ 24
#define ETIMER_H_ 
#define PT_WAITING 0
#define UIP_LLPREF_LEN 10
#define WITH_SECURITY 0
#define udp_markconn(conn,appstate) udp_attach(conn, appstate)
#define __FLOAT_WORD_ORDER__ __ORDER_LITTLE_ENDIAN__
#define __FSBLKCNT64_T_TYPE __UQUAD_TYPE
#define _IO_feof_unlocked(__fp) (((__fp)->_flags & _IO_EOF_SEEN) != 0)
#define __SUSECONDS_T_TYPE __SYSCALL_SLONG_TYPE
#define SCNoPTR __PRIPTR_PREFIX "o"
#define __SIZE_T__ 
#define UIP_EXT_HDR_OPT_PAD1 0
#define CONTIKIMAC_CONF_WITH_PHASE_OPTIMIZATION 0
#define DATA_PACKET_MARKER_LEN 13
#define __stub_gtty 
#define UIP_DS6_NOTIFICATION_ROUTE_ADD 2
#define SEEK_CUR 1
#define __NLINK_T_TYPE __SYSCALL_ULONG_TYPE
#define SIG_ATOMIC_MAX (2147483647)
#define UIP_EXT_HDR_OPT_PADN 1
#define __AVX2__ 1
#define __uint32_t_defined 
#define UIP_UDP UIP_CONF_UDP
#define __TSCH_QUEUE_H__ 
#define UIP_IPH_LEN 40
#define PROCESS_CONTEXT_END(p) process_current = tmp_current; }
#define LINKADDR_CONF_SIZE 8
#define _SYS_SIZE_T_H 
#define _IO_SHOWPOINT 0400
#define INT32_MAX (2147483647)
#define UIP_CONF_UDP_CHECKSUMS 1
#define __stub_sstk 
#define FRAME802154_SECURITY_LEVEL_ENC_MIC_128 (7)
#define __wur 
#define UIP_ACKDATA 1
#define __STDC_IEC_559_COMPLEX__ 1
#define _G_HAVE_MMAP 1
#define uip_getdraddr(addr) uip_ipaddr_copy((addr), &uip_draddr)
#define UIP_ARCH_IPCHKSUM 1
#define CC_CONF_FUNCTION_POINTER_ARGS 1
#define __FLT128_HAS_DENORM__ 1
#define UIP_MAXSYNRTX 5
#define __FLT128_DIG__ 33
#define RIMESTATS_CONF_ON 1
#define UIP_PROTO_ICMP6 58
#define __SCHAR_WIDTH__ 8
#define __INT32_C(c) c
#define SCNiLEAST8 "hhi"
#define __DEC64_EPSILON__ 1E-15DD
#define uip_ip6addr_cmp(addr1,addr2) (memcmp(addr1, addr2, sizeof(uip_ip6addr_t)) == 0)
#define __ORDER_PDP_ENDIAN__ 3412
#define __DEC128_MIN_EXP__ (-6142)
#define udp_bind(conn,port) uip_udp_bind(conn, port)
#define CC_CONF_INLINE inline
#define __FLT32_MAX_10_EXP__ 38
#define TSCH_SCHEDULE_WITH_6TISCH_MINIMAL TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL
#define _IOS_NOREPLACE 64
#define __INT_FAST32_TYPE__ long int
#define TSCH_DEFAULT_TS_MAX_TX 4256
#define UINT64_MAX (__UINT64_C(18446744073709551615))
#define LOADER_LOAD(name,arg) LOADER_ERR_NOLOADER
#define NBR_TABLE_FIND_REMOVABLE rpl_nbr_policy_find_removable
#define uip_create_linklocal_prefix(addr) do { (addr)->u16[0] = UIP_HTONS(0xfe80); (addr)->u16[1] = 0; (addr)->u16[2] = 0; (addr)->u16[3] = 0; } while(0)
#define __SYSCALL_SLONG_TYPE __SLONGWORD_TYPE
#define SIG_ATOMIC_MIN (-2147483647-1)
#define __UINT_LEAST16_TYPE__ short unsigned int
#define PRIuFAST8 "u"
#define CONTIKI_CONF_H_ 
#define NBR_TABLE(type,name) static type _ ##name ##_mem[NBR_TABLE_MAX_NEIGHBORS]; static nbr_table_t name ##_struct = { 0, sizeof(type), NULL, (nbr_table_item_t *)_ ##name ##_mem }; static nbr_table_t *name = &name ##_struct
#define UIP_BIG_ENDIAN 1234
#define __FLT64X_HAS_INFINITY__ 1
#define unix 1
#define SIMULATION_END_TIME_IN_S 151245
#define TSCH_CLOCK_TO_TICKS(c) (((c) * RTIMER_SECOND) / CLOCK_SECOND)
#define FRAME802154_SECURITY_LEVEL_MIC_128 (3)
#define uip_ipaddr_cmp(addr1,addr2) uip_ip6addr_cmp(addr1, addr2)
#define __INT16_MAX__ 0x7fff
#define __THROWNL __attribute__ ((__nothrow__))
#define _BSD_SIZE_T_ 
#define __SIZE_TYPE__ long unsigned int
#define __UINT64_MAX__ 0xffffffffffffffffUL
#define __va_arg_pack_len() __builtin_va_arg_pack_len ()
#define __ULONGWORD_TYPE unsigned long int
#define _SIZE_T_DECLARED 
#define TSCH_DEFAULT_TS_MAX_ACK 2400
#define DEBUG DEBUG_PRINT
#define __FLT64X_DIG__ 18
#define _IO_SHOWBASE 0200
#define __INT8_TYPE__ signed char
#define LOADER_ERR_OS 3
#define UIP_REXMIT 4
#define __ELF__ 1
#define RPL_ROUTE_ENTRY_DAO_NACK 0x04
#define __GCC_ASM_FLAG_OUTPUTS__ 1
#define PRIxMAX __PRI64_PREFIX "x"
#define __ID_T_TYPE __U32_TYPE
#define PROCESS_EVENT_CONTINUE 0x85
#define WITH_STABLETIMER 1
#define UIP_CONF_PINGADDRCONF 0
#define uip_initialmss() (uip_conn->initialmss)
#define __warnattr(msg) __attribute__((__warning__ (msg)))
#define LLSEC802154_MIC_LEN(sec_lvl) (2 << (sec_lvl & 3))
#define _IO_funlockfile(_fp) 
#define TSCH_JOIN_HOPPING_SEQUENCE TSCH_DEFAULT_HOPPING_SEQUENCE
#define __FLT_RADIX__ 2
#define PRIdLEAST8 "d"
#define __INT_LEAST16_TYPE__ short int
#define __LDBL_EPSILON__ 1.08420217248550443400745280086994171e-19L
#define INTPTR_MAX (9223372036854775807L)
#define __UINTMAX_C(c) c ## UL
#define __TSCH_SLOT_OPERATION_H__ 
#define uip_sethostaddr(addr) uip_ipaddr_copy(&uip_hostaddr, (addr))
#define _POSIX_C_SOURCE 200809L
#define SCNdPTR __PRIPTR_PREFIX "d"
#define _G_IO_IO_FILE_VERSION 0x20001
#define __SSE_MATH__ 1
#define __long_double_t long double
#define UIP_CONF_DS6_DEFRT_NBU 2
#define __LDBL_REDIR(name,proto) name proto
#define __SIG_ATOMIC_MAX__ 0x7fffffff
#define LEAPFROG_RETRIES 1
#define PT_EXIT(pt) do { PT_INIT(pt); return PT_EXITED; } while(0)
#define SCNxLEAST32 "x"
#define PRIu16 "u"
#define PROCESS_EVENT_EXITED 0x87
#define TCPIP_H_ 
#define INT_LEAST64_MAX (__INT64_C(9223372036854775807))
#define __GCC_ATOMIC_WCHAR_T_LOCK_FREE 2
#define _IO_IS_FILEBUF 0x2000
#define SCNx32 "x"
#define SICSLOWPAN_CONF_COMPRESSION_THRESHOLD 0
#define UIP_FIXEDETHADDR 0
#define __WCHAR_MAX __WCHAR_MAX__
#define UIP_LLH_LEN (UIP_CONF_LLH_LEN)
#define __SIZEOF_PTRDIFF_T__ 8
#define SEEK_SET 0
#define _ANSI_STDDEF_H 
#define TSCH_CONF_AUTOSTART 0
#define PRIu32 "u"
#define __haswell 1
#define _CC_GCC_H_ 
#define SCNu64 __PRI64_PREFIX "u"
#define UIP_TIME_WAIT 7
#define NETSTACK_RDC NETSTACK_CONF_RDC
#define SCNxLEAST64 __PRI64_PREFIX "x"
#define _IO_FLAGS2_NOTCANCEL 2
#define __stub_sigreturn 
#define UIP_LOGGING (UIP_CONF_LOGGING)
#define __errordecl(name,msg) extern void name (void) __attribute__((__error__ (msg)))
#define PROFILE_CONF_ON 0
#define UIP_CLOSING 6
#define __FLT32X_MANT_DIG__ 53
#define SCNx64 __PRI64_PREFIX "x"
#define __restrict_arr __restrict
#define INTPTR_MIN (-9223372036854775807L-1)
#define INT8_C(c) c
#define NBR_TABLE_GLOBAL(type,name) static type _ ##name ##_mem[NBR_TABLE_MAX_NEIGHBORS]; static nbr_table_t name ##_struct = { 0, sizeof(type), NULL, (nbr_table_item_t *)_ ##name ##_mem }; nbr_table_t *name = &name ##_struct
#define NETSTACK_CONF_MAC tschmac_driver
#define PRIu64 __PRI64_PREFIX "u"
#define _IO_STDIO 040000
#define __attribute_artificial__ __attribute__ ((__artificial__))
#define __USE_MISC 1
#define __UWORD_TYPE unsigned long int
#define __x86_64__ 1
#define _SIZE_T_ 
#define __FLT32X_MIN_EXP__ (-1021)
#define __DEC32_SUBNORMAL_MIN__ 0.000001E-95DF
#define _IO_iconv_t _G_iconv_t
#define _IO_USER_BUF 1
#define uip_poll() (uip_flags & UIP_POLL)
#define TSCH_WITH_LINK_SELECTOR TSCH_CONF_WITH_LINK_SELECTOR
#define UINT_FAST32_MAX (18446744073709551615UL)
#define _WCHAR_T_H 
#define CC_CONF_ALIGN(n) __attribute__((__aligned__(n)))
#define PRIxLEAST16 "x"
#define UIP_PROTO_TCP 6
#define __INT_FAST16_MAX__ 0x7fffffffffffffffL
#define LLSEC802154_USES_AUX_HEADER LLSEC802154_ENABLED
#define __stub_revoke 
#define RPL_NS_CONF_LINK_NUM 300
#define RTIMER_ARCH_H_ 
#define INT_LEAST64_MIN (-__INT64_C(9223372036854775807)-1)
#define uip_closed() (uip_flags & UIP_CLOSE)
#define ABS(n) (((n) < 0) ? -(n) : (n))
#define PRIxLEAST32 "x"
#define _IO_off_t __off_t
#define __WCHAR_MIN __WCHAR_MIN__
#define __FLT64_DIG__ 15
#define RADIO_QUALITY_SET_TIME_IN_S 600
#define __UINT_FAST32_MAX__ 0xffffffffffffffffUL
#define __UINT_LEAST64_TYPE__ long unsigned int
#define TSCH_LOG_ID_FROM_LINKADDR(addr) ((addr) ? (addr)->u8[LINKADDR_SIZE - 1] : 0)
#define RTIMERTICKS_TO_US(T) (T)
#define DEBUG_NONE 0
#define PACKETBUF_ATTR_PACKET_TYPE_STREAM 2
#define __FLT_HAS_QUIET_NAN__ 1
#define UINTMAX_MAX (__UINT64_C(18446744073709551615))
#define _IO_STDIO_H 
#define INT32_C(c) c
#define __FLT_MAX_10_EXP__ 38
#define QUEUEBUF_H_ 
#define __LONG_MAX__ 0x7fffffffffffffffL
#define __WCHAR_T__ 
#define __FLT64X_HAS_DENORM__ 1
#define __DEC128_SUBNORMAL_MIN__ 0.000000000000000000000000000000001E-6143DL
#define PACKETBUF_H_ 
#define __FLT_HAS_INFINITY__ 1
#define RADIO_QUALITY_RANDOM_SET_MAX_QUALITY_IN_PERCENT 100
#define PRIxLEAST64 __PRI64_PREFIX "x"
#define CC_INLINE CC_CONF_INLINE
#define PRIxFAST16 __PRIPTR_PREFIX "x"
#define _BSD_PTRDIFF_T_ 
#define PRIXPTR __PRIPTR_PREFIX "X"
#define PROCESS_CURRENT() process_current
#define PRIX16 "X"
#define TSCH_LINK_NEIGHBOR_CALLBACK(dest,status,num) uip_ds6_link_neighbor_callback(status, num)
#define __UINT_FAST16_TYPE__ long unsigned int
#define __bos0(ptr) __builtin_object_size (ptr, 0)
#define __DEC64_MAX__ 9.999999999999999E384DD
#define uip_ipaddr_maskcmp(addr1,addr2,mask) (((((uint16_t *)addr1)[0] & ((uint16_t *)mask)[0]) == (((uint16_t *)addr2)[0] & ((uint16_t *)mask)[0])) && ((((uint16_t *)addr1)[1] & ((uint16_t *)mask)[1]) == (((uint16_t *)addr2)[1] & ((uint16_t *)mask)[1])))
#define TSCH_DEFAULT_TS_RX_OFFSET (TSCH_DEFAULT_TS_TX_OFFSET - (TSCH_CONF_RX_WAIT / 2))
#define __INT_FAST32_WIDTH__ 64
#define UIP_APPDATA_SIZE (UIP_BUFSIZE - UIP_LLH_LEN - UIP_TCPIP_HLEN)
#define __tune_core_avx2__ 1
#define NULL ((void *)0)
#define __CHAR16_TYPE__ short unsigned int
#define CC_ASSIGN_AGGREGATE(dest,src) *dest = *src
#define PRIxFAST32 __PRIPTR_PREFIX "x"
#define NETSTACK_RADIO_MAX_PAYLOAD_LEN 125
#define __PRAGMA_REDEFINE_EXTNAME 1
#define SCNuPTR __PRIPTR_PREFIX "u"
#define __USING_NAMESPACE_C99(name) 
#define _VA_LIST_DEFINED 
#define PRIX32 "X"
#define __SIZE_WIDTH__ 64
#define FRAME802154_SECURITY_LEVEL_MIC_64 (2)
#define __BLKSIZE_T_TYPE __SYSCALL_SLONG_TYPE
#define __SEG_FS 1
#define TSCH_INIT_SCHEDULE_FROM_EB 1
#define _IO_size_t size_t
#define __INT_LEAST16_MAX__ 0x7fff
#define TSCH_CHANNEL_SCAN_DURATION CLOCK_SECOND
#define __stub_lchmod 
#define __DEC64_MANT_DIG__ 16
#define PACKETBUF_ATTR_PACKET_TYPE_DATA 0
#define uip_is_addr_solicited_node(a) ((((a)->u8[0]) == 0xFF) && (((a)->u8[1]) == 0x02) && (((a)->u16[1]) == 0x00) && (((a)->u16[2]) == 0x00) && (((a)->u16[3]) == 0x00) && (((a)->u16[4]) == 0x00) && (((a)->u8[10]) == 0x00) && (((a)->u8[11]) == 0x01) && (((a)->u8[12]) == 0xFF))
#define uip_acked() (uip_flags & UIP_ACKDATA)
#define __INT64_MAX__ 0x7fffffffffffffffL
#define __UINT_LEAST32_MAX__ 0xffffffffU
#define __SEG_GS 1
#define __FLT32_DENORM_MIN__ 1.40129846432481707092372958328991613e-45F32
#define __GCC_ATOMIC_LONG_LOCK_FREE 2
#define _IOS_APPEND 8
#define __SIG_ATOMIC_WIDTH__ 32
#define PRIxFAST64 __PRI64_PREFIX "x"
#define __INT_LEAST64_TYPE__ long int
#define INT64_MAX (__INT64_C(9223372036854775807))
#define __INT16_TYPE__ short int
#define __INT_LEAST8_TYPE__ signed char
#define _INTTYPES_H 1
#define UIP_DS6_ROUTE_STATE_TYPE rpl_route_entry_t
#define TSCH_DEFAULT_TS_TX_OFFSET 2120
#define PRIX64 __PRI64_PREFIX "X"
#define _IO_FLAGS2_USER_WBUF 8
#define CLOCK_H_ 
#define __STDC_VERSION__ 201112L
#define PROCESS_PT_SPAWN(pt,thread) PT_SPAWN(process_pt, pt, thread)
#define ENERGEST_CONF_ON 0
#define NULLRDC_CONF_802154_AUTOACK 1
#define __FSFILCNT_T_TYPE __SYSCALL_ULONG_TYPE
#define __DEC32_MAX_EXP__ 97
#define RADIO_QUALITY_SET_ROOT_QUALITY_IN_PERCENT 100
#define __INT_FAST8_MAX__ 0x7f
#define UIP_802154_LONGADDR_LEN 8
#define IEEE802154_CONF_PANID 0xabcd
#define __PMT(args) args
#define __FLT128_MAX__ 1.18973149535723176508575932662800702e+4932F128
#define __INTPTR_MAX__ 0x7fffffffffffffffL
#define UIP_EXT_HDR_BITMAP_AH 0x10
#define LC_INIT(s) s = 0;
#define _OLD_STDIO_MAGIC 0xFABC0000
#define PT_WAIT_THREAD(pt,thread) PT_WAIT_WHILE((pt), PT_SCHEDULE(thread))
#define LIST(name) static void *LIST_CONCAT(name,_list) = NULL; static list_t name = (list_t)&LIST_CONCAT(name,_list)
#define linux 1
#define UIP_EXT_HDR_BITMAP_ROUTING 0x04
#define RIMESTATS_CONF_ENABLED 1
#define UIP_IPTCPH_LEN (UIP_TCPH_LEN + UIP_IPH_LEN)
#define SCNuFAST16 __PRIPTR_PREFIX "u"
#define DEBUG_PRINT 1
#define __int8_t_defined 
#define SCNdMAX __PRI64_PREFIX "d"
#define __FLT64_HAS_QUIET_NAN__ 1
#define ____FILE_defined 1
#define UIP_UDP_APPCALL tcpip_uipcall
#define _IOLBF 1
#define _IO_stdout ((_IO_FILE*)(&_IO_2_1_stdout_))
#define _IO_UPPERCASE 01000
#define __SGX__ 1
#define PROCESS_END() PT_END(process_pt)
#define AUTOSTART_PROCESSES(...) extern int _dummy
#define NETSTACK_FRAMER NETSTACK_CONF_FRAMER
#define SCNuFAST32 __PRIPTR_PREFIX "u"
#define PROCESS(name,strname) PROCESS_THREAD(name, ev, data); struct process name = { NULL, strname, process_thread_ ##name }
#define _XLOCALE_H 1
#define UIP_CONF_IPV6_CHECKS 1
#define __FLT32_MIN_10_EXP__ (-37)
#define __SSE2__ 1
#define US_TO_RTIMERTICKS(US) (US)
#define __SSSE3__ 1
#define uip_ip6addr_u8(addr,addr0,addr1,addr2,addr3,addr4,addr5,addr6,addr7,addr8,addr9,addr10,addr11,addr12,addr13,addr14,addr15) do { (addr)->u8[0] = addr0; (addr)->u8[1] = addr1; (addr)->u8[2] = addr2; (addr)->u8[3] = addr3; (addr)->u8[4] = addr4; (addr)->u8[5] = addr5; (addr)->u8[6] = addr6; (addr)->u8[7] = addr7; (addr)->u8[8] = addr8; (addr)->u8[9] = addr9; (addr)->u8[10] = addr10; (addr)->u8[11] = addr11; (addr)->u8[12] = addr12; (addr)->u8[13] = addr13; (addr)->u8[14] = addr14; (addr)->u8[15] = addr15; } while(0)
#define __FLT32X_DIG__ 15
#define TSCH_DEFAULT_TS_TIMESLOT_LENGTH 10000
#define __RDRND__ 1
#define __WORDSIZE 64
#define FRAMER_H_ 
#define RDC_WITH_DUPLICATE_DETECTION !LLSEC802154_ENABLED
#define STABLE_TIME (DATA_SEND_START_TIME_IN_S * CLOCK_SECOND)
#define INT64_MIN (-__INT64_C(9223372036854775807)-1)
#define __BEGIN_DECLS 
#define __LDBL_MANT_DIG__ 64
#define UIP_BUFSIZE (UIP_CONF_BUFFER_SIZE)
#define PRIo8 "o"
#define TSCH_LOG_QUEUE_LEN 8
#define PRIXLEAST8 "X"
#define ANNOTATE(...) 
#define SCNuFAST64 __PRI64_PREFIX "u"
#define __DBL_HAS_QUIET_NAN__ 1
#define _IO_BAD_SEEN 0x4000
#define __FLT64_HAS_INFINITY__ 1
#define __FLT64X_MAX__ 1.18973149535723176502126385303097021e+4932F64x
#define FRAME802154E_IE_MAX_LINKS 4
#define TSCH_SCHEDULE_DEFAULT_LENGTH TSCH_SCHEDULE_CONF_DEFAULT_LENGTH
#define _STRING_H 1
#define __GNUC_VA_LIST 
#define _GCC_MAX_ALIGN_T 
#define TSCH_SCHEDULE_CONF_MAX_LINKS 64
#define UIP_CONF_LLH_LEN 0
#define __SIG_ATOMIC_MIN__ (-__SIG_ATOMIC_MAX__ - 1)
#define __code_model_small__ 1
#define NBR_TABLE_CONF_MAX_NEIGHBORS 300
#define __RLIM_T_TYPE __SYSCALL_ULONG_TYPE
#define PACKETBUF_ATTR_PACKET_TYPE_ACK 1
#define uip_buf (uip_aligned_buf.u8)
#define FILENAME_MAX 4096
#define UIP_EXT_HDR_BITMAP_FRAG 0x08
#define UIP_CONF_LOGGING 0
#define __INTPTR_TYPE__ long int
#define __UINT16_TYPE__ short unsigned int
#define __WCHAR_TYPE__ int
#define RANDOM_H_ 
#define __SIZEOF_FLOAT__ 4
#define PROCESS_EVENT_TIMER 0x88
#define _IOS_TRUNC 16
#define LIST_H_ 
#define uip_mss() (uip_conn->mss)
#define PRIiPTR __PRIPTR_PREFIX "i"
#define __HAVE_COLUMN 
#define __stub_fdetach 
#define LC_RESUME(s) switch(s) { case 0:
#define UIP_PROTO_HBHO 0
#define __pic__ 2
#define COOJA 1
#define __UINTPTR_MAX__ 0xffffffffffffffffUL
#define LIST_CONCAT(s1,s2) LIST_CONCAT2(s1, s2)
#define __INT_FAST64_WIDTH__ 64
#define __DEC64_MIN_EXP__ (-382)
#define __stub_chflags 
#define SCNxPTR __PRIPTR_PREFIX "x"
#define uip_is_addr_mac_addr_based(a,m) ((((a)->u8[8]) == (((m)->addr[0]) ^ 0x02)) && (((a)->u8[9]) == (m)->addr[1]) && (((a)->u8[10]) == (m)->addr[2]) && (((a)->u8[11]) == (m)->addr[3]) && (((a)->u8[12]) == (m)->addr[4]) && (((a)->u8[13]) == (m)->addr[5]) && (((a)->u8[14]) == (m)->addr[6]) && (((a)->u8[15]) == (m)->addr[7]))
#define putc(_ch,_fp) _IO_putc (_ch, _fp)
#define CC_FUNCTION_POINTER_ARGS 1
#define MAKE_WITH_LEAPFROG 1
#define UIP_IPICMPH_LEN (UIP_IPH_LEN + UIP_ICMPH_LEN)
#define __FLT32_DECIMAL_DIG__ 9
#define __USE_POSIX 1
#define __INT_FAST64_MAX__ 0x7fffffffffffffffL
#define __GCC_ATOMIC_TEST_AND_SET_TRUEVAL 1
#define UIP_HTONL(n) (((uint32_t)UIP_HTONS(n) << 16) | UIP_HTONS((uint32_t)(n) >> 16))
#define UIP_HTONS(n) (uint16_t)((((uint16_t) (n)) << 8) | (((uint16_t) (n)) >> 8))
#define BUFSIZ _IO_BUFSIZ
#define __FLT_DIG__ 6
#define UIP_CONF_IP_FORWARD 0
#define __FLT32_HAS_INFINITY__ 1
#define PROCESS_ERR_OK 0
#define INT_FAST32_MAX (9223372036854775807L)
#define __FLT64X_MAX_EXP__ 16384
#define _WCHAR_T_DECLARED 
#define FRAME802154_CMDFRAME (0x03)
#define __UINT_FAST64_TYPE__ long unsigned int
#define LC_END(s) }
#define _IO_putc_unlocked(_ch,_fp) (_IO_BE ((_fp)->_IO_write_ptr >= (_fp)->_IO_write_end, 0) ? __overflow (_fp, (unsigned char) (_ch)) : (unsigned char) (*(_fp)->_IO_write_ptr++ = (_ch)))
#define PROCESS_THREAD(name,ev,data) static PT_THREAD(process_thread_ ##name(struct pt *process_pt, process_event_t ev, process_data_t data))
#define PRIXFAST8 "X"
#define RDC_H_ 
#define TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL 0
#define __LDBL_REDIR_DECL(name) 
#define UINT_FAST8_MAX (255)
#define __INT_MAX__ 0x7fffffff
#define __amd64__ 1
#define uip_is_addr_mcast_non_routable(a) ((((a)->u8[0]) == 0xFF) && (((a)->u8[1] & 0x0F) <= 0x02))
#define CFS_CONF_OFFSET_TYPE long
#define SCNoFAST16 __PRIPTR_PREFIX "o"
#define PACKETBUF_NUM_ADDRS 2
#define __S16_TYPE short int
#define UIP_LLIPH_LEN (UIP_LLH_LEN + UIP_IPH_LEN)
#define __bos(ptr) __builtin_object_size (ptr, __USE_FORTIFY_LEVEL > 1)
#define PRIXMAX __PRI64_PREFIX "X"
#define RTIMER_TIME(task) ((task)->time)
#define _T_SIZE_ 
#define SICSLOWPAN_CONF_FRAG 1
#define UIP_ESTABLISHED 3
#define SCNoFAST32 __PRIPTR_PREFIX "o"
#define _G_va_list __gnuc_va_list
#define RADIO_TX_MODE_SEND_ON_CCA (1 << 0)
#define LIST_STRUCT_INIT(struct_ptr,name) do { (struct_ptr)->name = &((struct_ptr)->LIST_CONCAT(name,_list)); (struct_ptr)->LIST_CONCAT(name,_list) = NULL; list_init((struct_ptr)->name); } while(0)
#define __INT64_TYPE__ long int
#define SCNuMAX __PRI64_PREFIX "u"
#define __FLT_MAX_EXP__ 128
#define UIP_DS6_NOTIFICATION_ROUTE_RM 3
#define UIP_PROTO_ROUTING 43
#define UINT32_C(c) c ## U
#define __core_avx2 1
#define UIP_RTO 3
#define __ORDER_BIG_ENDIAN__ 4321
#define __ptr_t void *
#define __DBL_MANT_DIG__ 53
#define ___int_size_t_h 
#define __TIMER_T_TYPE void *
#define CONTIKI_H_ 
#define SCNoFAST64 __PRI64_PREFIX "o"
#define __SIZEOF_FLOAT128__ 16
#define __INT_LEAST64_MAX__ 0x7fffffffffffffffL
#define __GCC_ATOMIC_CHAR16_T_LOCK_FREE 2
#define UINT_FAST64_MAX (__UINT64_C(18446744073709551615))
#define INT_FAST32_MIN (-9223372036854775807L-1)
#define __GLIBC_MINOR__ 23
#define __DEC64_MIN__ 1E-383DD
#define __WINT_TYPE__ unsigned int
#define __UINT_LEAST32_TYPE__ unsigned int
#define __SIZEOF_SHORT__ 2
#define RTIMER_CLOCK_DIFF(a,b) ((int64_t)((a)-(b)))
#define __SSE__ 1
#define SCNu16 "hu"
#define __intptr_t_defined 
#define UIP_STAT(s) 
#define __LDBL_MIN_EXP__ (-16381)
#define _IO_va_list __gnuc_va_list
#define NETSTACK_CONF_RADIO cooja_radio_driver
#define PT_THREAD(name_args) char name_args
#define NBR_TABLE_H_ 
#define __FLT64_MAX__ 1.79769313486231570814527423731704357e+308F64
#define RTIMER_ARCH_SECOND UINT64_C(1000000)
#define SCNu32 "u"
#define stdout stdout
#define __WINT_WIDTH__ 32
#define __REDIRECT_NTHNL(name,proto,alias) name proto __asm__ (__ASMNAME (#alias)) __THROWNL
#define EOF (-1)
#define CCIF 
#define __INT_LEAST8_MAX__ 0x7f
#define __extern_inline extern __inline __attribute__ ((__gnu_inline__))
#define uip_udp_send(len) uip_send((char *)uip_appdata, len)
#define __USE_POSIX199309 1
#define __FLT32X_MAX_10_EXP__ 308
#define ____mbstate_t_defined 1
#define __TSCH_LOG_H__ 
#define __SIZEOF_INT128__ 16
#define TSCH_DEQUEUED_ARRAY_SIZE 16
#define tcpip_set_forwarding(forwarding) tcpip_do_forwarding = (forwarding)
#define __BLKCNT64_T_TYPE __SQUAD_TYPE
#define __LDBL_MAX_10_EXP__ 4932
#define __ATOMIC_RELAXED 0
#define __FSBLKCNT_T_TYPE __SYSCALL_ULONG_TYPE
#define __DBL_EPSILON__ ((double)2.22044604925031308084726333618164062e-16L)
#define UIP_MAXRTX 8
#define PROCESS_POLLHANDLER(handler) if(ev == PROCESS_EVENT_POLL) { handler; }
#define __stub_stty 
#define __FLT128_MIN__ 3.36210314311209350626267781732175260e-4932F128
#define _SIZET_ 
#define _LP64 1
#define __REDIRECT_NTH_LDBL(name,proto,alias) __REDIRECT_NTH (name, proto, alias)
#define __UINT8_C(c) c
#define __FLT64_MAX_EXP__ 1024
#define __INT_LEAST32_TYPE__ int
#define P_tmpdir "/tmp"
#define __wchar_t__ 
#define SEEK_END 2
#define __SIZEOF_WCHAR_T__ 4
#define __USE_POSIX_IMPLICITLY 1
#define __UINT64_TYPE__ long unsigned int
#define uip_connected() (uip_flags & UIP_CONNECTED)
#define uip_is_mcast_group_id_all_nodes(a) ((((a)->u16[1]) == 0) && (((a)->u16[2]) == 0) && (((a)->u16[3]) == 0) && (((a)->u16[4]) == 0) && (((a)->u16[5]) == 0) && (((a)->u16[6]) == 0) && (((a)->u8[14]) == 0) && (((a)->u8[15]) == 1))
#define uip_periodic_conn(conn) do { uip_conn = conn; uip_process(UIP_TIMER); } while (0)
#define uip_ipaddr(addr,addr0,addr1,addr2,addr3) do { (addr)->u8[0] = addr0; (addr)->u8[1] = addr1; (addr)->u8[2] = addr2; (addr)->u8[3] = addr3; } while(0)
#define SCNiFAST16 __PRIPTR_PREFIX "i"
#define TSCH_ASN_DIFF(asn1,asn2) ((asn1).ls4b - (asn2).ls4b)
#define __REDIRECT_NTH(name,proto,alias) name proto __asm__ (__ASMNAME (#alias)) __THROW
#define TSCH_ASN_MOD(asn,div) ((uint16_t)((asn).ls4b % (div).val) + (uint16_t)((asn).ms1b * (div).asn_ms1b_remainder % (div).val)) % (div).val
#define _IO_RIGHT 04
#define __END_NAMESPACE_STD 
#define NETSTACK_CONF_FRAMER framer_802154
#define PROCESS_CONF_NUMEVENTS 32
#define __PRI64_PREFIX "l"
#define FRAME802154_SUPPR_SEQNO 0
#define TSCH_HOPPING_SEQUENCE_16_16 (uint8_t[]){ 16, 17, 23, 18, 26, 15, 25, 22, 19, 11, 12, 13, 24, 14, 20, 21 }
#define _STDDEF_H_ 
#define FRAME802154_DATAFRAME (0x01)
#define NETSTACK_LLSEC NETSTACK_CONF_LLSEC
#define __stub___compat_bdflush 
#define SCNiFAST32 __PRIPTR_PREFIX "i"
#define __FLT128_HAS_QUIET_NAN__ 1
#define CC_H_ 
#define __INT_FAST8_TYPE__ signed char
#define PRIiMAX __PRI64_PREFIX "i"
#define __PID_T_TYPE __S32_TYPE
#define __FLT64X_MIN__ 3.36210314311209350626267781732175260e-4932F64x
#define uip_is_mcast_group_id_all_routers(a) ((((a)->u16[1]) == 0) && (((a)->u16[2]) == 0) && (((a)->u16[3]) == 0) && (((a)->u16[4]) == 0) && (((a)->u16[5]) == 0) && (((a)->u16[6]) == 0) && (((a)->u8[14]) == 0) && (((a)->u8[15]) == 2))
#define POWERTRACKER_INITIAL_DELAY_TIME_IN_S 600
#define PT_WAIT_WHILE(pt,cond) PT_WAIT_UNTIL((pt), !(cond))
#define LOADER_UNLOAD() 
#define SCNiFAST64 __PRI64_PREFIX "i"
#define SICSLOWPAN_REASS_MAXAGE (SICSLOWPAN_CONF_MAXAGE)
#define SCNxMAX __PRI64_PREFIX "x"
#define __GNUC_STDC_INLINE__ 1
#define __attribute_pure__ __attribute__ ((__pure__))
#define __FLT64_HAS_DENORM__ 1
#define __FILE_defined 1
#define __WORDSIZE_TIME64_COMPAT32 1
#define NULLRDC_CONF_ACK_WAIT_TIME RTIMER_SECOND / 500
#define UIP_CONF_ROUTER 1
#define __FLT32_EPSILON__ 1.19209289550781250000000000000000000e-7F32
#define _IO_stdin ((_IO_FILE*)(&_IO_2_1_stdin_))
#define INTMAX_MAX (__INT64_C(9223372036854775807))
#define UIP_POLL_REQUEST 3
#define UIP_CONF_IPV6_REASSEMBLY 0
#define _IO_cleanup_region_start(_fct,_fp) 
#define UIP_ABORT 32
#define uip_setnetmask(addr) uip_ipaddr_copy(&uip_netmask, (addr))
#define _IO_DEC 020
#define __DBL_DECIMAL_DIG__ 17
#define __STDC_UTF_32__ 1
#define __INT_FAST8_WIDTH__ 8
#define __FXSR__ 1
#define PROCESS_EXITHANDLER(handler) if(ev == PROCESS_EVENT_EXIT) { handler; }
#define FRAME802154_IMPLICIT_KEY (0)
#define __DEC_EVAL_METHOD__ 2
#define _SIZE_T 
#define _IO_ERR_SEEN 0x20
#define __FLT32X_MAX__ 1.79769313486231570814527423731704357e+308F32x
#define PRIxFAST8 "x"
#define __ULONG32_TYPE unsigned int
#define LOADER_H_ 
#define _G_config_h 1
#define FRAME802154_1_BYTE_KEY_ID_MODE (1)
#define FOPEN_MAX 16
#define UIP_ARPTAB_SIZE 8
#define PACKETBUF_NUM_ATTRS (PACKETBUF_ATTR_MAX - PACKETBUF_NUM_ADDRS)
#define UIP_CONF_ND6_REACHABLE_TIME 600000
#define TSCH_JOIN_SECURED_ONLY LLSEC802154_ENABLED
#define __off_t_defined 
#define UIP_CONF_MAX_CONNECTIONS 4
#define _IO_IS_APPENDING 0x1000
#define TSCH_CONF_RX_WAIT 2200
#define UIP_CONF_ND6_SEND_NA (NETSTACK_CONF_WITH_IPV6)
#define TSCH_MAC_MAX_FRAME_RETRIES TSCH_CONF_MAC_MAX_FRAME_RETRIES
#define PRIdFAST16 __PRIPTR_PREFIX "d"
#define UIP_CONF_ND6_SEND_NS (NETSTACK_CONF_WITH_IPV6 && !UIP_CONF_IPV6_RPL)
#define uip_ip4addr_cmp(addr1,addr2) ((addr1)->u16[0] == (addr2)->u16[0] && (addr1)->u16[1] == (addr2)->u16[1])
#define _GCC_SIZE_T 
#define __INO64_T_TYPE __UQUAD_TYPE
#define __U64_TYPE unsigned long int
#define UIP_CONF_BUFFER_SIZE 1600
#define __UINT32_C(c) c ## U
#define PRIdFAST32 __PRIPTR_PREFIX "d"
#define __INTMAX_MAX__ 0x7fffffffffffffffL
#define LOG_CONF_ENABLED 1
#define RPL_ROUTE_IS_NOPATH_RECEIVED(route) (((route)->state.state_flags & RPL_ROUTE_ENTRY_NOPATH_RECEIVED) != 0)
#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#define __size_t__ 
#define TSCH_HOPPING_SEQUENCE_4_4 (uint8_t[]){ 15, 25, 26, 20 }
#define _IO_BUFSIZ _G_BUFSIZ
#define __FLT_DENORM_MIN__ 1.40129846432481707092372958328991613e-45F
#define uip_create_solicited_node(a,b) (((b)->u8[0]) = 0xFF); (((b)->u8[1]) = 0x02); (((b)->u16[1]) = 0); (((b)->u16[2]) = 0); (((b)->u16[3]) = 0); (((b)->u16[4]) = 0); (((b)->u8[10]) = 0); (((b)->u8[11]) = 0x01); (((b)->u8[12]) = 0xFF); (((b)->u8[13]) = ((a)->u8[13])); (((b)->u16[7]) = ((a)->u16[7]))
#define uip_ipaddr4(addr) ((addr)->u8[3])
#define PRIoPTR __PRIPTR_PREFIX "o"
#define PROCINIT(...) const struct process *procinit[] = {__VA_ARGS__, NULL}
#define INTMAX_MIN (-__INT64_C(9223372036854775807)-1)
#define __BEGIN_NAMESPACE_C99 
#define UIP_CONF_ND6_SEND_RA 0
#define __INT8_MAX__ 0x7f
#define LOADER_OK 0
#define __LONG_WIDTH__ 64
#define __PIC__ 2
#define PRIdFAST64 __PRI64_PREFIX "d"
#define __UINT_FAST32_TYPE__ long unsigned int
#define TSCH_AUTOSELECT_TIME_SOURCE 0
#define __INO_T_MATCHES_INO64_T 1
#define _IO_fpos_t _G_fpos_t
#define PROCESS_LIST() process_list
#define __CHAR32_TYPE__ unsigned int
#define _IO_uid_t __uid_t
#define __FLT_MAX__ 3.40282346638528859811704183484516925e+38F
#define MAX(n,m) (((n) < (m)) ? (m) : (n))
#define uip_is_addr_linklocal_allnodes_mcast(a) ((((a)->u8[0]) == 0xff) && (((a)->u8[1]) == 0x02) && (((a)->u16[1]) == 0) && (((a)->u16[2]) == 0) && (((a)->u16[3]) == 0) && (((a)->u16[4]) == 0) && (((a)->u16[5]) == 0) && (((a)->u16[6]) == 0) && (((a)->u8[14]) == 0) && (((a)->u8[15]) == 0x01))
#define LOADER_ERR_MEM 5
#define IEEE802154_PANID IEEE802154_CONF_PANID
#define INT_FAST64_MAX (__INT64_C(9223372036854775807))
#define __FP_FAST_FMA 1
#define UINT_LEAST32_MAX (4294967295U)
#define SCNxLEAST8 "hhx"
#define uip_close() (uip_flags = UIP_CLOSE)
#define FRAME802154_ACKFRAME (0x02)
#define __attribute_format_arg__(x) __attribute__ ((__format_arg__ (x)))
#define RTIMER_GUARD_TIME (RTIMER_ARCH_SECOND >> 14)
#define CC_UNSIGNED_CHAR_BUGS 0
#define PROCESS_EVENT_POLL 0x82
#define TSCH_CONF_DEFAULT_TIMESLOT_LENGTH 10000
#define __INT32_TYPE__ int
#define __XSAVEC__ 1
#define uip_ntohl uip_htonl
#define __SIZEOF_DOUBLE__ 8
#define UIP_ACTIVE_OPEN 1
#define uip_ntohs uip_htons
#define __FLT_MIN_10_EXP__ (-37)
#define FRAME802154_SECURITY_LEVEL_MIC_32 (1)
#define __FLT64_MIN__ 2.22507385850720138309023271733240406e-308F64
#define _IO_BE(expr,res) __builtin_expect ((expr), res)
#define TSCH_DEFAULT_TS_RX_WAIT TSCH_CONF_RX_WAIT
#define AODV_NUM_RT_ENTRIES 32
#define __INT_LEAST32_WIDTH__ 32
#define __KEY_T_TYPE __S32_TYPE
#define __SWORD_TYPE long int
#define uip_udp_bind(conn,port) (conn)->lport = port
#define __INTMAX_TYPE__ long int
#define DATA_SEND_INTERVAL (DATA_SEND_INTERVAL_TIME_IN_S * CLOCK_SECOND)
#define __DEC128_MAX_EXP__ 6145
#define __FSGSBASE__ 1
#define _T_SIZE 
#define UIP_EXT_HDR_BITMAP_ESP 0x20
#define UINT8_C(c) c
#define __va_arg_pack() __builtin_va_arg_pack ()
#define __RDSEED__ 1
#define PRIdFAST8 "d"
#define LOADER_UNLOAD_DSC(dsc) 
#define UIP_PROTO_UDP 17
#define __FLT32X_HAS_QUIET_NAN__ 1
#define __ATOMIC_CONSUME 1
#define __GNUC_MINOR__ 2
#define __INT_FAST16_WIDTH__ 64
#define __UINTMAX_MAX__ 0xffffffffffffffffUL
#define PRIuLEAST8 "u"
#define __DEC32_MANT_DIG__ 7
#define __FLT32X_DENORM_MIN__ 4.94065645841246544176568792868221372e-324F32x
#define PRIiLEAST8 "i"
#define INTMAX_C(c) c ## L
#define TSCH_CALLBACK_NEW_TIME_SOURCE thunder_callback_new_time_source
#define UIP_CONF_IPV6_QUEUE_PKT 1
#define INT_FAST64_MIN (-__INT64_C(9223372036854775807)-1)
#define _STDINT_H 1
#define __DBL_MAX_10_EXP__ 308
#define TCPIP_CONF_ANNOTATE_TRANSMISSIONS 1
#define UINT16_MAX (65535)
#define __LDBL_DENORM_MIN__ 3.64519953188247460252840593361941982e-4951L
#define __BMI2__ 1
#define RADIO_RX_MODE_POLL_MODE (1 << 2)
#define __INT16_C(c) c
#define __TSCH_SECURITY_H__ 
#define __U32_TYPE unsigned int
#define ENERGEST_H_ 
#define TSCH_ASN_DIVISOR_INIT(div,val_) do { (div).val = (val_); (div).asn_ms1b_remainder = ((0xffffffff % (val_)) + 1) % (val_); } while(0);
#define __STDC__ 1
#define __AES__ 1
#define __attribute_malloc__ __attribute__ ((__malloc__))
#define GRANDCHILD_OF_GRANDPARENT_KILL_MOTE_ID 8
#define __PTRDIFF_TYPE__ long int
#define TSCH_LOG_CONF_LEVEL 1
#define __haswell__ 1
#define STIMER_H_ 
#define DATA_PACKET_ID_H_ 
#define __attribute_noinline__ __attribute__ ((__noinline__))
#define LC_SET(s) s = __LINE__; case __LINE__:
#define ENERGEST_OFF(type) do { } while(0)
#define __ATOMIC_SEQ_CST 5
#define uip_is_addr_loopback(a) ((((a)->u16[0]) == 0) && (((a)->u16[1]) == 0) && (((a)->u16[2]) == 0) && (((a)->u16[3]) == 0) && (((a)->u16[4]) == 0) && (((a)->u16[5]) == 0) && (((a)->u16[6]) == 0) && (((a)->u8[14]) == 0) && (((a)->u8[15]) == 0x01))
#define LLSEC802154_ENABLED 0
#define PRIdPTR __PRIPTR_PREFIX "d"
#define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_16 1
#define UIP_REASSEMBLY 0
#define PT_H_ 
#define __ADX__ 1
#define __CLOCK_T_TYPE __SYSCALL_SLONG_TYPE
#define _IO_fpos64_t _G_fpos64_t
#define UIP_LAST_ACK 8
#define __UINT32_TYPE__ unsigned int
#define __FLT32X_MIN_10_EXP__ (-307)
#define PRId16 "d"
#define CC_REGISTER_ARG register
#define __UINTPTR_TYPE__ long unsigned int
#define RTIMER_CLOCK_LT(a,b) (RTIMER_CLOCK_DIFF((a),(b)) < 0)
#define RPL_ROUTE_IS_DAO_NACKED(route) ((route->state.state_flags & RPL_ROUTE_ENTRY_DAO_NACK) != 0)
#define __DEC64_SUBNORMAL_MIN__ 0.000000000000001E-383DD
#define __FSFILCNT64_T_TYPE __UQUAD_TYPE
#define FRAME802154_IEEE802154_2003 (0x00)
#define __DEC128_MANT_DIG__ 34
#define __LDBL_MIN_10_EXP__ (-4931)
#define __attribute_deprecated__ __attribute__ ((__deprecated__))
#define PRId32 "d"
#define __FLT128_EPSILON__ 1.92592994438723585305597794258492732e-34F128
#define ENERGEST_ON(type) do { } while(0)
#define MIN(n,m) (((n) < (m)) ? (n) : (m))
#define SCNxLEAST16 "hx"
#define _IO_wint_t wint_t
#define MAC_SEQUENCE_H 
#define __SIZEOF_LONG_LONG__ 8
#define uip_ipaddr_prefixcmp(addr1,addr2,length) (memcmp(addr1, addr2, length>>3) == 0)
#define PRIo16 "o"
#define __USE_ATFILE 1
#define _GCC_PTRDIFF_T 
#define TSCH_CONF_WITH_LINK_SELECTOR 1
#define __FLT128_DECIMAL_DIG__ 36
#define RPL_CALLBACK_NEW_DIO_INTERVAL tsch_rpl_callback_new_dio_interval
#define __GCC_ATOMIC_LLONG_LOCK_FREE 2
#define __FLT32X_MIN__ 2.22507385850720138309023271733240406e-308F32x
#define UIP_CONF_UDP_CONNS 12
#define __LDBL_DIG__ 18
#define UIPOPT_H_ 
#define PRId64 __PRI64_PREFIX "d"
#define SCNdFAST32 __PRIPTR_PREFIX "d"
#define FRAMER_FAILED -1
#define __FLT_DECIMAL_DIG__ 9
#define TSCH_SCHEDULE_PRINT_INTERVAL (TSCH_SCHEDULE_PRINT_INTERVAL_IN_S* CLOCK_SECOND)
#define __UINT_FAST16_MAX__ 0xffffffffffffffffUL
#define PRIo32 "o"
#define TSCH_PACKET_EB_WITH_HOPPING_SEQUENCE 0
#define UIP_CONF_BROADCAST 1
#define LOADER_ERR_OPEN 6
#define LC_SWITCH_H_ 
#define ___int_ptrdiff_t_h 
#define PRIoMAX __PRI64_PREFIX "o"
#define __GCC_ATOMIC_SHORT_LOCK_FREE 2
#define _IO_peekc(_fp) _IO_peekc_unlocked (_fp)
#define __INT_LEAST64_WIDTH__ 64
#define __BMI__ 1
#define PRIoFAST8 "o"
#define INT_LEAST16_MAX (32767)
#define _IO_ftrylockfile(_fp) 
#define PRIoLEAST8 "o"
#define RPL_ROUTE_SET_DAO_NACKED(route) do { (route)->state.state_flags |= RPL_ROUTE_ENTRY_DAO_NACK; } while(0)
#define __glibc_likely(cond) __builtin_expect ((cond), 1)
#define __SSE3__ 1
#define __UINT_FAST8_TYPE__ unsigned char
#define PROCESS_BEGIN() PT_BEGIN(process_pt)
#define __P(args) args
#define PRIo64 __PRI64_PREFIX "o"
#define PRIXFAST16 __PRIPTR_PREFIX "X"
#define __ATOMIC_ACQ_REL 4
#define SCNuFAST8 "hhu"
#define __WCHAR_T 
#define __ATOMIC_RELEASE 3
#define TSCH_ASN_INIT(asn,ms1b_,ls4b_) do { (asn).ms1b = (ms1b_); (asn).ls4b = (ls4b_); } while(0);
