# Change these to match your configuration

ifdef BOOST_ROOT
	INCLUDES = $(BOOST_ROOT)/include
	LIBS = $(BOOST_ROOT)/lib
else
	INCLUDES = .
	LIBS = .
endif

base = -I$(INCLUDES) -L$(LIBS) -Wall

ifdef CONFIG_PROFILE
	prof = -pg
else
	prof =
endif

ifndef CONFIG_DBG
	CONFIG_FAST = 1
	FLAGS = $(base) -DCONFIG_FAST -std=c++11 -O3 $(prof) -DBOOST_TIMER_ENABLE_DEPRECATED
else
	FLAGS = $(base) -DCONFIG_FAST -DLOGGING -std=c++11 -g $(prof) -DBOOST_TIMER_ENABLE_DEPRECATED
endif

#FLAGS = -I$(INCLUDES) -Wall -g
