TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    table.cpp \
    record.cpp \
    sql_parser.cpp \
    state_machine.cpp \
    stokenizer.cpp \
    record_utility_functions.cpp \
    mysql.cpp \
    ftokenizer.cpp

HEADERS += \
    bplustree.h \
    lower_level_bplustree_functions.h \
    mmap.h \
    mpair.h \
    pair.h \
    map.h \
    table.h \
    record.h \
    record_utility_functions.h \
    sql_parser.h \
    token.h \
    state_machine.h \
    constants.h \
    my_queue.h \
    list_functions.h \
    stokenizer.h \
    mysql.h \
    ftokenizer.h
