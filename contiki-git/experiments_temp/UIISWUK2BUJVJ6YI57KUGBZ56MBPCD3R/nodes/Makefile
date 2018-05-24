CONTIKI=/tmp/fakehome/mnt
CFLAGS += -DWITH_EXTRA_INFORMATION=1
CFLAGS += -DWITH_STABLETIMER=1
CFLAGS += -DPROJECT_CONF_H=\"../project-conf.h\"

#DATA_SEND_START_TIME_IN_S=15*60
#DATA_SEND_INTERVAL_TIME_IN_S=15
#TSCH_SCHEDULE_PRINT_INTERVAL_IN_S=1*60
#WITH_THUNDER_ADAPTIVE_EB_SLOT=1
#TSCH_CONF_MAC_MAX_FRAME_RETRIES=3
#LEAPFROG_RETRIES=0
#WITH_OVERHEARING=1

CONTIKI_WITH_IPV6 = 1
MAKE_WITH_ORCHESTRA ?= 0 # force Orchestra from command line
MAKE_WITH_SECURITY ?= 0 # force Security from command line
MAKE_WITH_LEAPFROG ?= 0 # force LeapFrog from command line
MAKE_WITH_LINKPEEK ?= 0 # force LinkPeek from command line
MAKE_WITH_THUNDER ?= 0 # force Thunder from command line


APPS += servreg-hack #now not used. for simple-udp
MODULES += core/net/mac/tsch


ifeq ($(MAKE_WITH_POWERTRACE),1)
CFLAGS += -DWITH_POWERTRACE=1
APPS += powertrace #for power trace
endif

ifeq ($(MAKE_WITH_ORCHESTRA),1)
CFLAGS += -DWITH_ORCHESTRA=1
APPS += orchestra
endif

ifeq ($(MAKE_WITH_LEAPFROG),1)
CFLAGS += -DWITH_LEAPFROG=1
APPS += leapfrog
endif

ifeq ($(MAKE_WITH_LINKPEEK),1)
CFLAGS += -DWITH_LINKPEEK=1
endif

ifeq ($(MAKE_WITH_THUNDER),1)
CFLAGS += -DWITH_THUNDER=1
#CFLAGS += -DWITH_THUNDER_ADAPTIVE_EB_SLOT=1
#CFLAGS += -DWITH_LEAPFROG_BEACON_SLOT=1
APPS += thunder #for manual configuration of TSCH schedule
endif


ifeq ($(MAKE_WITH_SECURITY),1)
CFLAGS += -DWITH_SECURITY=1
endif

include $(CONTIKI)/Makefile.include
