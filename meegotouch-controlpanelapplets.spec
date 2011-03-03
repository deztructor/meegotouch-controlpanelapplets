# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.21
# 
# >> macros
# << macros

Name:       meegotouch-controlpanelapplets
Summary:    Various MeeGo Touch Controlpanel Applets
Version:    0.20.17
Release:    1
Group:      System/GUI/Other
License:    LGPL v2.1
URL:        http://meego.gitorious.org/meegotouch/meegotouch-controlpanelapplets
Source0:    %{name}-%{version}.tar.bz2
Source100:  meegotouch-controlpanelapplets.yaml
BuildRequires:  pkgconfig(meegotouch) >= 0.20.37
BuildRequires:  pkgconfig(QtCore) >= 4.7.0
BuildRequires:  pkgconfig(QtGui) >= 4.7.0
BuildRequires:  libmeegocontrolpanel-devel
BuildRequires:  libqttracker-devel
BuildRequires:  meegotouch-systemui-devel
BuildRequires:  libthumbnailer-devel
BuildRequires:  clean-device-devel >= 0.0.3
BuildRequires:  qt-mobility-devel
BuildRequires:  doxygen
BuildRequires:  profiled-devel
BuildRequires:  gstreamer-devel


%description
Various Controlpanel applets for MeeGo Touch environment:
  - Offline switcher
  - USB settings
  - Battery info/settings
  - Display settings
  - Profiles settings
  - Theme-switcher
  - Wallpaper editor/setter
  - About product
  - Warranty applet
  - Profiles switcher
  - Reset applet
  - Sound settings
  



%package -n meegotouchcp-applets-l10n-eng-en
Summary:    MeeGo Touch Controlpanel applets engineering English messages
Group:      Application Framework/MeeGo Touch Settings
Provides:   meegotouchcp-applets-l10n

%description -n meegotouchcp-applets-l10n-eng-en
MeeGo Touch Controlpanel applets engineering English messages for various applets:
    - Offline switcher
    - USB settings
    - Battery info/settings
    - Display settings
    - Profiles settings
    - Theme-switcher
    - Wallpaper editor/setter
    - About product
    - Warranty applet
    - Profiles switcher


%package -n meegotouch-controlpanelapplets-tests
Summary:    Unit and functional test cases for meegotouch-controlpanelapplets
Group:      Development/Libraries

%description -n meegotouch-controlpanelapplets-tests
Unit and functional test cases for meegotouch-controlpanelapplets

%package -n meegotouchcp-profiles
Summary:    MeegoTouch Controlpanel profiles settings
Group:      System/GUI/Other
Requires:   meegotouch-controlpanel
Requires:   meegotouchcp-applets-l10n-eng-en

%description -n meegotouchcp-profiles
MeegoTouch Controlpanel profiles settings

%package -n meegotouchcp-wallpaper
Summary:    MeegoTouch Controlpanel wallpaper editor & setter
Group:      System/GUI/Other
Requires:   tumbler
Requires:   meegotouch-controlpanel
Requires:   meegotouchcp-applets-l10n-eng-en

%description -n meegotouchcp-wallpaper
MeegoTouch Controlpanel wallpaper editor & setter

%package -n meegotouchcp-about
Summary:    MeegoTouch Controlpanel about product applet
Group:      System/GUI/Other
Requires:   qt-mobility
Requires:   meegotouch-controlpanel
Requires:   meegotouchcp-applets-l10n-eng-en

%description -n meegotouchcp-about
MeegoTouch Controlpanel about product applet

%package -n meegotouchcp-usb
Summary:    MeegoTouch Controlpanel USB settings
Group:      System/GUI/Other
Requires:   meegotouch-controlpanel
Requires:   meegotouchcp-applets-l10n-eng-en

%description -n meegotouchcp-usb
MeegoTouch Controlpanel USB settings

%package -n meegotouchcp-battery
Summary:    MeegoTouch Controlpanel battery info/settings
Group:      System/GUI/Other
Requires:   meegotouch-controlpanel
Requires:   meegotouchcp-applets-l10n-eng-en

%description -n meegotouchcp-battery
MeegoTouch Controlpanel battery info/settings

%package -n meego-handset-statusindicators-profiles
Summary:    MeegoTouch profile-switcher statusindicatormenu plugin
Group:      System/GUI/Other
Requires:   meegotouch-controlpanel
Requires:   meegotouchcp-applets-l10n-eng-en
Requires:   meegotouch-systemui

%description -n meego-handset-statusindicators-profiles
MeegoTouch profile-switcher statusindicatormenu plugin

%package -n meegotouchcp-warranty
Summary:    MeegoTouch Controlpanel warranty applet
Group:      System/GUI/Other
Requires:   meegotouch-controlpanel
Requires:   meegotouchcp-applets-l10n-eng-en

%description -n meegotouchcp-warranty
MeegoTouch Controlpanel warranty applet

%package -n meegotouchcp-theme
Summary:    MeegoTouch Controlpanel theme switcher
Group:      System/GUI/Other
Requires:   meegotouch-controlpanel
Requires:   meegotouchcp-applets-l10n-eng-en

%description -n meegotouchcp-theme
MeegoTouch Controlpanel theme switcher

%package -n meegotouchcp-offline
Summary:    MeegoTouch Controlpanel Offline switcher
Group:      System/GUI/Other
Requires:   meegotouch-controlpanel
Requires:   meegotouchcp-applets-l10n-eng-en

%description -n meegotouchcp-offline
MeegoTouch Controlpanel Offline switcher

%package -n meegotouchcp-display
Summary:    MeegoTouch Controlpanel display settings
Group:      System/GUI/Other
Requires:   meegotouch-controlpanel
Requires:   meegotouchcp-applets-l10n-eng-en

%description -n meegotouchcp-display
MeegoTouch Controlpanel display settings

%package -n libmeegocontrol-doc
Summary:    Documentation for libmeegocontrol
Group:      Documentation
BuildArch:    noarch
Requires:   meegotouch-controlpanel
Requires:   meegotouchcp-applets-l10n-eng-en

%description -n libmeegocontrol-doc
Documentation for libmeegocontrol

%package -n libmeegocontrol
Summary:    libmeegocontrol to use some controlpanel applet as a library
Group:      System/GUI/Other
Requires:   meegotouch-controlpanel
Requires:   meegotouchcp-applets-l10n-eng-en
Requires:   gstreamer
Requires:   GConf-dbus
Requires:   libresourceqt
Requires:   libprofile-qt
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description -n libmeegocontrol
Documentation for libmeegocontrol

%package -n libmeegocontrol-devel
Summary:    Dev package for libmeegocontrol
Group:      Development/Libraries
Requires:   meegotouchcp-applets-l10n-eng-en
Requires:   gstreamer-devel
Requires:   libthumbnailer-devel
Requires:   libmeegocontrolpanel-devel
Requires:   libqttracker-devel
Requires:   libresourceqt-devel

%description -n libmeegocontrol-devel
Dev package for libmeegocontrol

%package -n meegotouchcp-resetapplet
Summary:    Device clearing and restore settings applet
Group:      System/GUI/Other
Requires:   meegotouch-controlpanel
Requires:   meegotouchcp-applets-l10n-eng-en
Requires:   clean-device
Requires:   meegotouch-systemui

%description -n meegotouchcp-resetapplet
Device clearing and restore settings applet

%package -n meegotouchcp-soundsettingsapplet
Summary:    Sound Settings Applet
Group:      System/GUI/Other
Requires:   meegotouch-controlpanel
Requires:   meegotouchcp-applets-l10n-eng-en
Requires:   libresourceqt

%description -n meegotouchcp-soundsettingsapplet
Sound Settings Applet

%package -n meegotouchcp-testapplet
Summary:    Testapplet to manual test duicontrolpanel.
Group:      System/GUI/Other
Requires:   meegotouch-controlpanel
Requires:   meegotouchcp-applets-l10n-eng-en
Requires:   meegotouch-systemui

%description -n meegotouchcp-testapplet
Testapplet to manual test duicontrolpanel.


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qmake 

make %{?jobs:-j%jobs}

# >> build post
# << build post
%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake_install

# >> install post
# << install post














































%post -n libmeegocontrol -p /sbin/ldconfig

%postun -n libmeegocontrol -p /sbin/ldconfig















%files -n meegotouchcp-applets-l10n-eng-en
%defattr(-,root,root,-)
/usr/share/l10n/meegotouch/meegotouchcp-applets.qm
/usr/share/doc/systemui-applets-l10n-engineering-english/meegotouchcp-applets.ts
# >> files meegotouchcp-applets-l10n-eng-en
# << files meegotouchcp-applets-l10n-eng-en

%files -n meegotouch-controlpanelapplets-tests
%defattr(-,root,root,-)
/bin/cylontest.sh
%dir /usr/share/meegotouch-controlpanelapplets-tests
/usr/share/meegotouch-controlpanelapplets-tests/tests.xml
%dir /usr/lib/meegotouch-controlpanelapplets-tests
/usr/lib/meegotouch-controlpanelapplets-tests/ut_wallpaperapplet
/usr/lib/meegotouch-controlpanelapplets-tests/ut_warrantyapplet
/usr/lib/meegotouch-controlpanelapplets-tests/ft_statusmenuplugins
/usr/lib/meegotouch-controlpanelapplets-tests/ut_displaywidget
/usr/lib/meegotouch-controlpanelapplets-tests/ut_themeapplet
/usr/lib/meegotouch-controlpanelapplets-tests/ut_profiledialog
/usr/lib/meegotouch-controlpanelapplets-tests/ut_wallpapermodel
/usr/lib/meegotouch-controlpanelapplets-tests/ft_themebusinesslogic
/usr/lib/meegotouch-controlpanelapplets-tests/ut_themebusinesslogic
/usr/lib/meegotouch-controlpanelapplets-tests/ut_displayapplet
/usr/lib/meegotouch-controlpanelapplets-tests/ut_usbapplet
/usr/lib/meegotouch-controlpanelapplets-tests/ut_displaybusinesslogic
/usr/lib/meegotouch-controlpanelapplets-tests/ut_themecellcreator
/usr/lib/meegotouch-controlpanelapplets-tests/ut_offlineapplet
/usr/lib/meegotouch-controlpanelapplets-tests/ut_wallpapereditorwidget
/usr/lib/meegotouch-controlpanelapplets-tests/ut_batteryimage
/usr/lib/meegotouch-controlpanelapplets-tests/ut_wallpaperwidget
/usr/lib/meegotouch-controlpanelapplets-tests/ut_themedescriptor
/usr/lib/meegotouch-controlpanelapplets-tests/ut_wallpaperbusinesslogic
/usr/lib/meegotouch-controlpanelapplets-tests/ut_themelistmodel
/usr/lib/meegotouch-controlpanelapplets-tests/ut_batterybusinesslogic
/usr/lib/meegotouch-controlpanelapplets-tests/ft_wallpapermodel
/usr/lib/meegotouch-controlpanelapplets-tests/ut_wallpaperlist
/usr/lib/meegotouch-controlpanelapplets-tests/ut_aboutbusinesslogic
/usr/lib/meegotouch-controlpanelapplets-tests/ft_wallpaperdescriptor
/usr/lib/meegotouch-controlpanelapplets-tests/ut_themewidget
/usr/lib/meegotouch-controlpanelapplets-tests/ut_profiledatainterface
/usr/lib/meegotouch-controlpanelapplets-tests/ft_wallpaperbusinesslogic
/usr/lib/meegotouch-controlpanelapplets-tests/ut_profileapplet
/usr/lib/meegotouch-controlpanelapplets-tests/ut_themedialog
/usr/lib/meegotouch-controlpanelapplets-tests/ut_wallpaperdescriptor
/usr/lib/meegotouch-controlpanelapplets-tests/ut_wallpapercurrentdescriptor
/usr/lib/meegotouch-controlpanelapplets-tests/ut_batteryapplet
/usr/lib/meegotouch-controlpanelapplets-tests/ft_applets
/usr/lib/meegotouch-controlpanelapplets-tests/ut_aboutapplet
/usr/lib/meegotouch-controlpanelapplets-tests/ut_resetapplet
/usr/lib/meegotouch-controlpanelapplets-tests/ft_profiledatainterface
/usr/lib/meegotouch-controlpanelapplets-tests/ut_alerttone
/usr/lib/meegotouch-controlpanelapplets-tests/ut_alerttoneappletmaps
/usr/lib/meegotouch-controlpanelapplets-tests/ut_alerttoneappletwidget
/usr/lib/meegotouch-controlpanelapplets-tests/ut_alerttonebrowser
/usr/lib/meegotouch-controlpanelapplets-tests/ut_alerttonedefaults
/usr/lib/meegotouch-controlpanelapplets-tests/ut_alerttonedefaultsmodel
/usr/lib/meegotouch-controlpanelapplets-tests/ut_alerttonepreview
/usr/lib/meegotouch-controlpanelapplets-tests/ut_alerttonetoplevel
/usr/lib/meegotouch-controlpanelapplets-tests/ut_alerttonevolume
/usr/lib/meegotouch-controlpanelapplets-tests/ut_alerttonewidget
/usr/lib/meegotouch-controlpanelapplets-tests/ut_gconfstringcombo
/usr/lib/meegotouch-controlpanelapplets-tests/ut_profilebackend
/usr/lib/meegotouch-controlpanelapplets-tests/ut_profileintcombo
/usr/lib/meegotouch-controlpanelapplets-tests/ut_profilevalue
/usr/lib/meegotouch-controlpanelapplets-tests/ut_qgconfdirmanager
/usr/lib/meegotouch-controlpanelapplets-tests/ut_qgconfvalue
/usr/lib/meegotouch-controlpanelapplets-tests/ut_soundsettingsapplet
/usr/lib/meegotouch-controlpanelapplets-tests/ut_trackedvariant
# >> files meegotouch-controlpanelapplets-tests
# << files meegotouch-controlpanelapplets-tests

%files -n meegotouchcp-profiles
%defattr(-,root,root,-)
/usr/lib/duicontrolpanel/profile.desktop
/usr/lib/duicontrolpanel/applets/libprofileapplet.so
# >> files meegotouchcp-profiles
# << files meegotouchcp-profiles

%files -n meegotouchcp-wallpaper
%defattr(-,root,root,-)
/usr/lib/duicontrolpanel/wallpaper.desktop
/usr/lib/duicontrolpanel/applets/libwallpaperapplet.so
/usr/share/wallpaper
/usr/share/backup-framework/applications/wallpaper.conf
# >> files meegotouchcp-wallpaper
# << files meegotouchcp-wallpaper

%files -n meegotouchcp-about
%defattr(-,root,root,-)
/usr/lib/duicontrolpanel/about.desktop
/usr/lib/duicontrolpanel/applets/libaboutapplet.so
/usr/share/themes/base/meegotouch/libaboutapplet/style/libaboutapplet.css
/usr/share/duicontrolpanel-aboutapplet/license.txt
# >> files meegotouchcp-about
# << files meegotouchcp-about

%files -n meegotouchcp-usb
%defattr(-,root,root,-)
/usr/lib/duicontrolpanel/usbapplet.desktop
/usr/lib/duicontrolpanel/applets/libusbapplet.so
# >> files meegotouchcp-usb
# << files meegotouchcp-usb

%files -n meegotouchcp-battery
%defattr(-,root,root,-)
/usr/lib/duicontrolpanel/battery.desktop
/usr/lib/duicontrolpanel/applets/libbatteryapplet.so
/usr/share/themes/base/meegotouch/libbatteryapplet/style/libbatteryapplet.css
# >> files meegotouchcp-battery
# << files meegotouchcp-battery

%files -n meego-handset-statusindicators-profiles
%defattr(-,root,root,-)
/usr/lib/meegotouch/applicationextensions/libstatusindicatormenu-profile.so
/usr/share/meegotouch/applicationextensions/statusindicatormenu-profile.desktop
/usr/share/themes/base/meegotouch/libstatusindicatormenu-profile/style/libstatusindicatormenu-profile.css
# >> files meego-handset-statusindicators-profiles
# << files meego-handset-statusindicators-profiles

%files -n meegotouchcp-warranty
%defattr(-,root,root,-)
/usr/lib/duicontrolpanel/warranty.desktop
/usr/lib/duicontrolpanel/applets/libwarrantyapplet.so
/usr/share/themes/base/meegotouch/libwarrantyapplet/style/libwarrantyapplet.css
# >> files meegotouchcp-warranty
# << files meegotouchcp-warranty

%files -n meegotouchcp-theme
%defattr(-,root,root,-)
/usr/lib/duicontrolpanel/theme.desktop
/usr/lib/duicontrolpanel/applets/libthemeapplet.so
/usr/share/themes/base/meegotouch/libthemeapplet/style/libthemeapplet.css
# >> files meegotouchcp-theme
# << files meegotouchcp-theme

%files -n meegotouchcp-offline
%defattr(-,root,root,-)
/usr/lib/duicontrolpanel/offline.desktop
/usr/lib/duicontrolpanel/applets/libofflineapplet.so
# >> files meegotouchcp-offline
# << files meegotouchcp-offline

%files -n meegotouchcp-display
%defattr(-,root,root,-)
/usr/lib/duicontrolpanel/display.desktop
/usr/lib/duicontrolpanel/applets/libdisplayapplet.so
/usr/share/themes/base/meegotouch/libdisplayapplet/style/libdisplayapplet.css
# >> files meegotouchcp-display
# << files meegotouchcp-display

%files -n libmeegocontrol-doc
%defattr(-,root,root,-)
/usr/share/libmeegocontrol-doc
# >> files libmeegocontrol-doc
# << files libmeegocontrol-doc

%files -n libmeegocontrol
%defattr(-,root,root,-)
/usr/lib/libmeegocontrol.so.*
# >> files libmeegocontrol
# << files libmeegocontrol

%files -n libmeegocontrol-devel
%defattr(-,root,root,-)
/usr/lib/libmeegocontrol.so
/usr/include/libmeegocontrol
/usr/share/qt4/mkspecs/features
# >> files libmeegocontrol-devel
# << files libmeegocontrol-devel

%files -n meegotouchcp-resetapplet
%defattr(-,root,root,-)
/usr/lib/duicontrolpanel/reset.desktop
/usr/lib/duicontrolpanel/applets/libresetapplet.so
/etc/meego-rfs-scripts/mcpapplets-rfs.sh
# >> files meegotouchcp-resetapplet
# << files meegotouchcp-resetapplet

%files -n meegotouchcp-soundsettingsapplet
%defattr(-,root,root,-)
/usr/share/themes/base/meegotouch/libsoundsettingsapplet/style/libsoundsettingsapplet.css
/usr/lib/duicontrolpanel/applets/libsoundsettingsapplet.so
/usr/lib/duicontrolpanel/soundsettingsapplet.desktop
# >> files meegotouchcp-soundsettingsapplet
# << files meegotouchcp-soundsettingsapplet

%files -n meegotouchcp-testapplet
%defattr(-,root,root,-)
/usr/lib/duicontrolpanel/test.desktop
/usr/lib/duicontrolpanel/applets/libtestapplet.so
# >> files meegotouchcp-testapplet
# << files meegotouchcp-testapplet

