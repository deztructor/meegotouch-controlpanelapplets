contains(BUILD_FEATURES,coverage) {
    QMAKE_CXXFLAGS += -O0 -g -fprofile-arcs -ftest-coverage
    QMAKE_LFLAGS += -lgcov
    LIBS += -lgcov
}
