contains(BUILD_FEATURES,coverage) {
    QMAKE_CXXFLAGS += -O0 -g -fprofile-arcs --coverage
    QMAKE_LFLAGS += -lgcov --coverage
    LIBS += -lgcov
}
