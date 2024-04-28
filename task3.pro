QT += widgets

HEADERS       = msr.h \
                thread_related.h \
                functions.h \
                approximation.h
SOURCES       = main.cpp \
                msr.cpp \
                solve.cpp \
                preconditioner.cpp \
                approximation_evaluation.cpp \
                precision_checks.cpp \
                thread_related.cpp \
                functions.cpp \
                initializations.cpp \
                approximation.cpp
TARGET        = a.out

DISTFILES += \
  Makefile

