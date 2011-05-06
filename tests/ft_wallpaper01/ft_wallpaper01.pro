include(../common_top.pri)

testbg_files.files += testwallpaper.png
testbg_files.path = /usr/share/$$TEST_PKG_NAME

ftprepare_files.files += ft_wallpaper01

INSTALLS +=  \
    ftprepare \
    testbg_files

